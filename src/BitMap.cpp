
#include "StdAfx.h"
#include <math.h>
#include "PrtSc.h"
#include <png.h>
#include <jpeglib.h>

static	BOOL SaveBmpToBMPfile(HBITMAP hbmp,LPTSTR file)
{
	HANDLE	hfile,hmap;
	DWORD	fsize;
 	HDC		hdc;
	BITMAP	bmp;
	BITMAPINFOHEADER	bmhdr;
	void*	filebase;

	if(!GetObject(hbmp,sizeof(BITMAP),&bmp))return 0;

	bmhdr.biSize = sizeof(BITMAPINFOHEADER);
	bmhdr.biPlanes = bmp.bmPlanes;
	bmhdr.biBitCount = bmp.bmBitsPixel;
	bmhdr.biHeight = bmp.bmHeight;
	bmhdr.biWidth = bmp.bmWidth;

    bmhdr.biSizeImage = (bmhdr.biWidth + 7) /8 
                                  * bmhdr.biHeight 
                                 * bmp.bmBitsPixel; 
	if(bmp.bmBitsPixel < 24)
		bmhdr.biClrUsed = 2^bmp.bmBitsPixel;
	else bmhdr.biClrUsed = 0;
	bmhdr.biClrImportant = 0;
	bmhdr.biCompression = BI_RGB;
	
	fsize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
				bmhdr.biSize + bmhdr.biClrUsed * sizeof(RGBQUAD) 
					+ bmhdr.biSizeImage); 

	hfile = CreateFile(file,GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
	if(hfile==INVALID_HANDLE_VALUE)return 0;
	hmap=CreateFileMapping(hfile,0,PAGE_READWRITE,0,fsize,0);
	filebase=MapViewOfFile(hmap,FILE_MAP_WRITE,0,0,0);
	if(filebase==NULL)
	{
		CloseHandle(hmap);
		CloseHandle(hfile);
		return 0;	
	}
	((LPBITMAPFILEHEADER)filebase)->bfType = 'MB';
	((LPBITMAPFILEHEADER)filebase)->bfSize = fsize;
	((LPBITMAPFILEHEADER)filebase)->bfOffBits = 
		sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * bmhdr.biClrUsed ;
	memcpy((LPBYTE)filebase + sizeof(BITMAPFILEHEADER),&bmhdr,sizeof(bmhdr));

	hdc=CreateDC(_T("DISPLAY"),0,0,0);
	//GetBitmapBits(hbmp,bmp.bmWidth ,(LPBYTE)filebase + ((LPBITMAPFILEHEADER)filebase)->bfOffBits);
	GetDIBits(hdc,hbmp,0,bmp.bmHeight,(LPBYTE)filebase + ((LPBITMAPFILEHEADER)filebase)->bfOffBits,
		(LPBITMAPINFO)((LPBYTE)filebase + sizeof(BITMAPFILEHEADER)),DIB_RGB_COLORS);
	DeleteDC(hdc);
	DeleteObject(hbmp);
	UnmapViewOfFile(filebase);
	CloseHandle(hmap);
	CloseHandle(hfile);	
	return 1;
}

static void __cdecl user_err_fn(png_structp, png_const_charp err)
{
	
}

static	BOOL SaveBmpToPNGfile(HBITMAP hbmp,LPTSTR file)
{
	BITMAP	bmp;
	FILE * fp = NULL;
	
	if(!GetObject(hbmp,sizeof(BITMAP),&bmp))return FALSE;

	fp = _tfopen(file,TEXT("wb"));

	if(!fp)
	{
		DeleteObject(hbmp);
		return FALSE;
	}

	png_structp  png_ptr =
		png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL, user_err_fn,user_err_fn);

	if(!png_ptr)
	{
		DeleteObject(hbmp);
		return FALSE;
	}

	png_infop png_info_ptr =  png_create_info_struct(png_ptr);

	if(!png_info_ptr)
	{
		DeleteObject(hbmp);
		png_destroy_write_struct(&png_ptr,NULL);
		return FALSE;
	}
	
	if(setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_write_struct(&png_ptr,&png_info_ptr);
		fclose(fp);
		DeleteFile(file);
		DeleteObject(hbmp);
		return FALSE;
	}

	int color = PNG_COLOR_TYPE_RGB_ALPHA;
	if(bmp.bmBitsPixel < 32)
		color = PNG_COLOR_TYPE_RGB;
	
	LONG	row_length;
	BYTE * row_bytes;
	PBYTE * rows_bytes  =(PBYTE*) malloc( bmp.bmHeight * sizeof(png_byte *));

	row_length =bmp.bmHeight * bmp.bmWidthBytes;

	row_bytes = (PBYTE) png_malloc(png_ptr,row_length);
		
	GetBitmapBits(hbmp,row_length,row_bytes);	

	if(bmp.bmBitsPixel >=24)
	{
		for ( LONG i=0 ,j=0; i < bmp.bmHeight ; i ++ ,j +=bmp.bmWidthBytes )
		{
			rows_bytes[i] = row_bytes + j;
			if(color & PNG_COLOR_MASK_ALPHA)		
				for(LONG k=0; k < bmp.bmWidthBytes ; k+= 4)
					rows_bytes[i][k+3] = 255;		
		}
	}else //16 bit
	{

		for (int i =0; i < bmp.bmHeight ; i ++)
		{
			rows_bytes[i] =(PBYTE) malloc(bmp.bmWidth * 3);

			for(int  j=0,k=0; j < bmp.bmWidthBytes; j+=2, k+=3)
			{

				WORD   Color;//存16bit的颜色 

				double   Red,Green,Blue; 

				Color = *(PWORD)(row_bytes + i * bmp.bmWidthBytes + j );


				Blue=(Color&0xF800)>> 11; 

				Green=(Color&0x7e0)>> 5; 
				
				Red=(Color&0x1F);

				rows_bytes[i][k] = (BYTE)(Red * 256  / 32 +0.5);
				rows_bytes[i][k+1] = (BYTE)(Green * 256  / 64 +0.5);
				rows_bytes[i][k+2] = (BYTE)(Blue * 256  / 32 +0.5);
			}
		}
	}
	
	png_set_IHDR(png_ptr,png_info_ptr,bmp.bmWidth,bmp.bmHeight,8,
		color, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
		PNG_FILTER_TYPE_BASE);

	png_init_io(png_ptr,fp);

	png_set_rows(png_ptr,png_info_ptr,rows_bytes);

	png_set_bgr(png_ptr);

	//crc32(0, Z_NULL, 0);

	png_write_png(png_ptr, png_info_ptr,0,0);

	png_write_end(png_ptr,png_info_ptr);

	png_destroy_write_struct(&png_ptr, &png_info_ptr);

	if(!(color & PNG_COLOR_MASK_ALPHA))
	{
		for(int i = 0; i < bmp.bmHeight ; i++)
		{
			free(rows_bytes[i]);
		}
	}

	free(rows_bytes);
	free(row_bytes);	
	DeleteObject(hbmp);
	fclose(fp);
	return TRUE;
}

static BOOL SaveBmpToJPGfile(HBITMAP hbmp,LPTSTR file)
{
	LONG i;
	LONG	 bmp_length;
	PVOID	 bmp_data ;
	BITMAP	bmp;
	FILE * fp = NULL;
	
	if(!GetObject(hbmp,sizeof(BITMAP),&bmp))return FALSE;

	bmp_length = bmp.bmWidthBytes * bmp.bmHeight;
	
	fp = _tfopen(file,TEXT("wb"));
	
	if(!fp)
	{
		DeleteObject(hbmp);
		return FALSE;
	}

	jpeg_error_mgr err;

	jpeg_compress_struct cinfo={0};

	jpeg_create_compress(&cinfo);

	jpeg_stdio_dest(&cinfo,fp);

	cinfo.image_width = bmp.bmWidth;
	cinfo.image_height = bmp.bmHeight;

	cinfo.input_components = 3; //rgb only
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	
//cinfo.err = & err;

	cinfo.err = jpeg_std_error(&err);



	jpeg_start_compress(&cinfo, TRUE);

	bmp_data = malloc(bmp_length);
	
	GetBitmapBits(hbmp,bmp_length,bmp_data);

	PBYTE row_pointer;

	if(bmp.bmBitsPixel != 24)
		row_pointer = (PBYTE)malloc(bmp.bmWidth * 3);

	while (cinfo.next_scanline < cinfo.image_height) 
	{
		// 32bit to 24 bit, ro 16 bit to 24 bit

		switch(bmp.bmBitsPixel)
		{
		case 24:
			row_pointer= (PBYTE)bmp_data + bmp.bmWidthBytes * cinfo.next_scanline;
			jpeg_write_scanlines(&cinfo, &row_pointer, 1);
			row_pointer = NULL;
			break;
		case 32:
			for (i=0; i < bmp.bmWidth ; i++)
			{
				row_pointer[i*3+2] = ((PBYTE)bmp_data)[bmp.bmWidthBytes * cinfo.next_scanline + i*4 ];
				row_pointer[i*3+1] = ((PBYTE)bmp_data)[bmp.bmWidthBytes * cinfo.next_scanline +i*4+1];
				row_pointer[i*3] = ((PBYTE)bmp_data)[bmp.bmWidthBytes * cinfo.next_scanline +i*4 + 2];
			}
			jpeg_write_scanlines(&cinfo, &row_pointer, 1);
			break;
		case 16:
			for ( i=0; i < bmp.bmWidth ; i++)
			{
				WORD   Color;//存16bit的颜色 
				
				double   Red,Green,Blue; 
				
				Color = *(PWORD)((PBYTE)bmp_data + cinfo.next_scanline * bmp.bmWidthBytes + i*2 );
								
				Red=(Color&0xF800)>> 11; 
				
				Green=(Color&0x7e0)>> 5; 
				
				Blue=(Color&0x1F);
				
				row_pointer[i*3]= (BYTE)(Red * 256  / 32 +0.5);
				row_pointer[i*3+1] = (BYTE)(Green * 256  / 64 +0.5);
				row_pointer[i*3+2] = (BYTE)(Blue * 256  / 32 +0.5);
			}
			jpeg_write_scanlines(&cinfo, &row_pointer, 1);			
		}		
	}

	if(row_pointer)
		free(row_pointer);

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	fclose(fp);
	DeleteObject(hbmp);
	return FALSE;
}

typedef BOOL(* FileTypeFunc)(HBITMAP hbmp,LPTSTR file);
static FileTypeFunc SaveBmp2File[]={SaveBmpToBMPfile,SaveBmpToPNGfile,SaveBmpToJPGfile,0};

BOOL	SaveBmpToFile(HBITMAP hbmp,LPTSTR file)
{
	//根据扩展名
	LPTSTR lpstrExt	= file + lstrlen(file) - 1;

	while (*lpstrExt!=TEXT('.'))
	{
		lpstrExt--;
	}
	
	if(lstrcmpi(lpstrExt,TEXT(".bmp"))==0)
	{
		return (SaveBmp2File[0])(hbmp,file);
	}else if(lstrcmpi(lpstrExt,TEXT(".png"))==0)
	{
		return (SaveBmp2File[1])(hbmp,file);
	}else if(lstrcmpi(lpstrExt,TEXT(".jpg"))==0)
	{
		return (SaveBmp2File[2])(hbmp,file);
	}else if(lstrcmpi(lpstrExt,TEXT(".jpeg"))==0)
	{
		return (SaveBmp2File[2])(hbmp,file);
	}
	return FALSE;
}


