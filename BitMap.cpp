
#include "StdAfx.h"
#include <math.h>
#include "PrtSc.h"
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
static	BOOL SaveBmpToPNGfile(HBITMAP hbmp,LPTSTR file)
{

	return 0;
}



typedef BOOL(* FileTypeFunc)(HBITMAP hbmp,LPTSTR file);
static FileTypeFunc SaveBmp2File[]={SaveBmpToBMPfile,SaveBmpToPNGfile,0};

BOOL	SaveBmpToFile(HBITMAP hbmp,LPTSTR file,ImageFileType ft)
{
	return (SaveBmp2File[ft])(hbmp,file);//PVOID64
}


