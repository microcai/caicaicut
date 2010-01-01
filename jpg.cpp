
#include "Stdafx.h"
#include <math.h>
#include "PrtSc.h"
#include "jpg.h"

static void FDCT(PFLOAT out)
{
	float tmp0,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7;
	float tmp10,tmp11,tmp12,tmp13;
	float z1,z2,z3,z4,z5,z11,z13;
	float*dataptr;
	int ctr;
	dataptr=out;

	/*行计算*/
	for(ctr=7;ctr>=0;--ctr)
	{
		tmp0=dataptr[0]+dataptr[7];
		tmp7=dataptr[0]-dataptr[7];
		tmp1=dataptr[1]+dataptr[6];
		tmp6=dataptr[1]-dataptr[6];
		tmp2=dataptr[2]+dataptr[5];
		tmp5=dataptr[2]-dataptr[5];
		tmp3=dataptr[3]+dataptr[4];
		tmp4=dataptr[3]-dataptr[4];

		tmp10=tmp0+tmp3;
		tmp13=tmp0-tmp3;
		tmp11=tmp1+tmp2;
		tmp12=tmp1-tmp2;
		dataptr[0]=tmp10+tmp11;
		dataptr[4]=tmp10-tmp11;
		z1=(tmp12+tmp13)*(0.70716781);
		dataptr[2]=tmp13+z1;
		dataptr[6]=tmp13-z1;

		tmp10=tmp4+tmp5;
		tmp11=tmp5+tmp6;
		tmp12=tmp6+tmp7;
		z5=(tmp10-tmp12)* 0.382683433;
		z2=0.541196100*tmp10+z5;
		z4=1.306562965 * tmp12 +z5;
		z3=tmp11 * .0707106781;
		z11= tmp7+z3;
		z13= tmp7-z3;
		dataptr[5]=z13+z2;
		dataptr[3]=z13-z2;
		dataptr[1]=z11+z4;
		dataptr[7]=z11-z4;
		dataptr+=8;
	}
	/*列计算*/
	for(dataptr=out,ctr=0;ctr<8;++ctr)
	{
		tmp0=dataptr[8] + dataptr[7*8];
		tmp7=dataptr[0] - dataptr[7*8];
		tmp1=dataptr[1*8]+dataptr[6*8];
		tmp6=dataptr[1*8]-dataptr[6*8];
		tmp2=dataptr[2*8]+dataptr[5*8];
		tmp5=dataptr[2*8]-dataptr[5*8];
		tmp3=dataptr[3*8]+dataptr[4*8];
		tmp4=dataptr[3*8]-dataptr[4*8];

		tmp10=tmp0+tmp3;
		tmp13=tmp0-tmp3;
		tmp11=tmp1+tmp2;
		tmp12=tmp1-tmp2;
		dataptr[0]=tmp10+tmp11;
		dataptr[4*8]=tmp10-tmp11;
		z1=tmp12+tmp13*0.707106781;
		dataptr[2*8]=tmp12+z1;
		dataptr[6*8]=tmp13-z1;

		tmp10=tmp4+tmp5;
		tmp11=tmp5+tmp6;
		tmp12=tmp6+tmp7;
		
		z5=(tmp10-tmp12)* 0.382683433;
		z2=0.541196100*tmp10+z5;
		z4=z5 + 1.306562965 * tmp12 ;
		z3=tmp11 * .0707106781;
		z11= tmp7+z3;
		z13= tmp7-z3;
		dataptr[5*8]=z13+z2;
		dataptr[3*8]=z13-z2;
		dataptr[1*8]=z11+z4;
		dataptr[7*8]=z11-z4;
		++dataptr;
	}
}

static BYTE __fastcall ComputeVLI(short val)
{
	val = val>0 ? val : -val;
	if(val >=1024)	return 11;
	if(val >=512)	return 10;
	if(val >=256)	return 9;
	if(val >=128)	return 8;
	if(val >=64)	return 7;
	if(val >=32)	return 6;
	if(val >=16)	return 5;
	if(val >=8)		return 4;
	if(val >=4)		return 3;
	if(val >=2)		return 2;
	if(val>=1)		return 1;
	return 0;
}

static void __fastcall BuildVLITable()
{
	int i;
	for(i=0;i<2047;i++)
		lpVLITab[i]=ComputeVLI(i);
	for(i=-2048;i<0;++i)
		lpVLITab[i]=ComputeVLI(-i);
}
static SYM2 BuildSym2(short v)
{
	SYM2 s;
	s.codeLen = ComputeVLI(v);
	s.amplitude=0;
	if(v>=0)
		s.amplitude=v;
	else
		s.amplitude = pow( 2 , s.codeLen ) -1 + v;
	return s;
}
static void	BuildHufftable(BYTE*nrcodes,BYTE* stdTab,HUFFCODE*huffCode)
{
	BYTE i,j,k;
	USHORT code=0;
	for(i=1;i<=16;i++)
	{
		for(j=1;j<=nrcodes[i];++j)
		{
			huffCode[stdTab[k]].code = code;
			huffCode[stdTab[k]].len = i;
			++k;++code;
		}
		code <<= 1;
	}
	for(i=0;i<k;++i)
		huffCode[i].value = stdTab[i];
}
static void InitQT4AANDCT()
{
	UINT i,j,k=0;
	for(i=0;i<8;++i)
	{
		for(j=0;j<8;++j)
		{
			YQT_DCT[k] = 1.0 / YQT[FZBT[k]] * aanSF[i] * aanSF[j] *8; 
			++k;
		}
	}
	k=0;
	for(i=0;i<8;++i)
	{
		for(j=0;j<8;++j)
		{
			UVQT_DCT[k] = (float)(1.0 / ((double)UVQT[FZBT[k]] * aanSF[i] * aanSF[j] *8)); 
			++k;
		}
	}
}

static void	InitJPEG()
{
	static flag=0;
	if(flag==0)
	{
		flag=1;
		InitQT4AANDCT();
		BuildVLITable();
		BuildHufftable(std_dc_Y_NRcodes,std_dc_Y_val,STD_DC_Y_HT);
		BuildHufftable(std_ac_Y_NRcodes,std_ac_Y_val,STD_AC_Y_HT);
		BuildHufftable(std_dc_UV_NRcodes,std_dc_UV_val,STD_DC_UV_HT);
		BuildHufftable(std_ac_UV_NRcodes,std_ac_UV_val,STD_AC_UV_HT);
	}
}
static void _stdcall RGB2YUV(size_t size,LPBYTE lpRGB,LPBYTE lpY, LPBYTE lpU,LPBYTE lpV)
{//输入32bit的位图数据，输出yuv数据
	UINT i;
	double tmpY=0.0,tmpU=0.0,tmpV=0.0;
	BYTE	tmpR=0,tmpG=0,tmpB=0;
	for(i=0;i<size;++i)
	{
		tmpB=lpRGB[i*4];
		tmpG=lpRGB[i*4+1];
		tmpR=lpRGB[i*4+2];
		
		tmpY=0.299*tmpR+0.587*tmpG+0.114*tmpB;
		tmpU=(double)tmpB/2 +128.0 -0.3313*tmpG-0.1687*tmpR;
		tmpV=128.0 + (double)tmpR/2.0 - 0.4187*(double)tmpG - 0.0813*tmpB;
		
		if(tmpY>255)tmpY=255;
		if(tmpU>255)tmpU=255;
		if(tmpV>255)tmpV=255;
		if(tmpY<0)tmpY=0;
		if(tmpU<0)tmpU=0;
		if(tmpV<0)tmpV=0;
		
		lpY[i]=tmpY;
		lpU[i]=tmpU;
		lpV[i]=tmpV;	
	}
}

static void __stdcall GetRGBbits(LPBYTE lpBits,HDC hdc,HBITMAP hmap,LPBITMAPINFO lpbi,UINT width, UINT height)
{
	UINT i,ret;
	for(i = height;i>0 ; --i)
		ret=GetDIBits(hdc,hmap,i,1,lpBits + (height-i) * width * sizeof(DWORD),lpbi,DIB_RGB_COLORS);
}
static	LPVOID	file=0;
static	LPSTR	curptr;
static	UINT	filesize=0;

static BYTE bytenew=0;
static int bytepos=7;

static void	__inline WriteByte(BYTE b)
{
	*((LPBYTE)curptr)=b;
	++curptr;
}
static void __fastcall WriteBits(int v,int l)
{
	static mask[16] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};
	char posval;
	posval=l -1;
	while(posval)
	{
		if(v & mask[posval])
			bytenew |= mask[bytepos];
		posval--;
		bytepos--;
		if(bytepos<0)
		{
			if(bytenew == 0xFF)
			{
				WriteByte(0xFF);
				WriteByte(0);
			}else	WriteByte(bytenew);
			bytepos=7;
			bytenew=0;
		}
	}
}
static void WriteBits(HUFFCODE & huffCode)
{
	WriteBits(huffCode.code,huffCode.len);
}
static void WriteBits(SYM2 &sym)
{
	WriteBits(sym.amplitude,sym.codeLen);		
}
static void RLEComp(short* lpBuf, ACSYM* out, BYTE & resultLen)
{
	BYTE zeroNum =0;
	BYTE	EOBPos;
	const BYTE MaxOlen = 15;
	BYTE i,j=0;
	EOBPos = 63;
	for( i = 63; i >0 ;--i)
	{
		if(lpBuf[i]==0)
			--EOBPos;
		else break;
	}
	for(i=1;i<=EOBPos;++i)
	{
		if(lpBuf[i] == 0 && zeroNum < 15)	
			++zeroNum;
		else
		{
			out[j].zeroLen= zeroNum;
			out[j].codeLen = ComputeVLI(lpBuf[i]);
			out[j].amplitude= lpBuf[i];
			zeroNum =0;
			++resultLen;
			++j;
		}
	}
}

static void WriteFileTag(WORD x,WORD y)
{
	int i;
	DWORD writed;
	*((WORD*)file)=0xD8FF;
	curptr = (LPSTR)file + 2;
	
	JPEGAPP *app=(JPEGAPP*)curptr;
	app->sing = 0xE0FF;
	app->len = 16;
	memcpy(app->jpgID ,"JFIF",5);
	app->ver = 0x0101;
	app->densityUNIT = 1;
	app->densityX=app->densityY =72;
	curptr+=sizeof(JPEGAPP);
	//DQT
	JPEGDQT *dqt= (JPEGDQT*)curptr;
	curptr+=sizeof(JPEGDQT);
	dqt->sig = 0xDBFF;
	dqt->len = 0x4300;
	dqt->tableinfo=0;
	for(i=0;i<64;++i)
		dqt->table[i] = QTable_Y[i/8][i%8];
	*((JPEGDQT*)curptr)=*dqt;
	dqt= (JPEGDQT*)curptr;
	dqt->tableinfo = 1;
	for(i=0;i<64;++i)
		dqt->table[i] = QTable_UV[i/8][i%8];
	curptr+=sizeof(JPEGDQT);
	//SOF
	JPEGSOF *sof=(JPEGSOF*)curptr;
	curptr+=sizeof(JPEGSOF);
	sof->sig= 0xC0FF;
	sof->len = 0x1100;
	sof->jindu = 0x80;
	sof->sigNum=3;
	sof->width = (LOBYTE(x)<<8) | HIBYTE(x);
	sof->height= (LOBYTE(y)<<8) | HIBYTE(y);
	sof->YID=1;
	sof->QT_Y=0;
	sof->UID=2;
	sof->QT_U=1;
	sof->VID=3;
	sof->QT_V=1;
	sof->HV_Y = sof->HV_U =sof->HV_V = 0x11;

	//DHT
	JPEGDHT *dht=(JPEGDHT*)curptr;//{,,0x00};
	dht->sig= 0xC4FF;
	dht->len = 0x1F00;
	dht->tableInfo = 0;
	for(i=0;i<16;++i)
		dht->huffCode[i]=std_dc_Y_NRcodes[i+1];
	curptr+=sizeof(JPEGDHT);

	for(i=0;i<12;i++,curptr++)
		(*((LPBYTE)curptr))=std_ac_Y_val[i];
	*((JPEGDHT*)curptr)=*dht;
	dht=(JPEGDHT*)curptr;
	dht->tableInfo = 0x01;
	for(i=0;i<16;++i)
		dht->huffCode[i]=std_dc_UV_NRcodes[i+1];
	curptr+=sizeof(JPEGDHT);
	
	for(i=0;i<12;i++,curptr++)
		(*((LPBYTE)curptr))=std_ac_UV_val[i];

	*((JPEGDHT*)curptr)=*dht;
	dht=(JPEGDHT*)curptr;

	dht->len = 0xB500;
	dht->tableInfo=0x10;
	for(i=0;i<16;++i)
		dht->huffCode[i]=std_ac_Y_NRcodes[i+1];
	curptr+=sizeof(JPEGDHT);
	for(i=0;i<162;i++,curptr++)
		(*((LPBYTE)curptr)) = std_ac_Y_val[i];
	
	*((JPEGDHT*)curptr)=*dht;
	dht=(JPEGDHT*)curptr;

	dht->tableInfo=0x11;
	for(i=0;i<16;++i,curptr++)
		dht->huffCode[i]=std_ac_UV_NRcodes[i+1];
	curptr+=sizeof(JPEGDHT);

	for(i=0;i<162;i++,curptr++)
		(*((LPBYTE)curptr)) = std_ac_UV_val[i];

	//SOS
	JPEGSOS *sos=(JPEGSOS*)curptr;//{1,0,2,0x11,3,0x11,00,0x3F,00};
	sos->sig=0xDAFF;sos->len = 0x0c00;
	sos->VID=sos->sigNum = 3;sos->YID = 1;
	sos->HTY = 0;sos->UID = 2;
	sos->HTU = 	sos->HTV= 0x11;
	sos->Ss= sos->Bf =0;
	sos->Se=0x3F;
	curptr+=sizeof(JPEGSOS);
	filesize = curptr - (LPSTR)file;
}
static void ProcessDU(float lpbf[64],float * qT,HUFFCODE *dcHuffTab,HUFFCODE * acHuffTab,short & DC)
{
	int i,j;
	short diffVal=0;
	BYTE aclen;
	short sigBuf[64]={0};
	ACSYM acSym[64];
	FDCT(lpbf);
	for(i=0;i<64;++i)
		sigBuf[FZBT[i]] = (lpbf[i] / qT[i]) + 0.5;
	diffVal = sigBuf[0] - DC;
	DC = sigBuf[0];
	if(diffVal == 0 )
		WriteBits(dcHuffTab[0]);
	else
	{
		WriteBits(dcHuffTab[lpVLITab[diffVal]]);
		WriteBits(BuildSym2(diffVal));
	}

	for(i=63;i>0 && sigBuf[i]==0 ; --i);
	if(i==0)
		WriteBits(acHuffTab[0]);
	else
	{
		RLEComp(sigBuf, acSym,aclen);
		for(j=0;j<aclen;++j)
		{
			if(acSym[j].codeLen==0)
				WriteBits(acHuffTab[0xF0]);
			else
			{
				WriteBits(acHuffTab[acSym[j].zeroLen*16 + acSym[j].codeLen]);
			}
		}
		if(i!=63)
			WriteBits(acHuffTab[0x0]);
	}
}
static void DivBf(BYTE * p,int w,int h)
{
	int x = w /8;
	int y= h/8;
	int tmpl= x*64;
	
	BYTE* tmp= new BYTE[tmpl];
	int i,j,k,n,offset;
	for(i=0;i<y;i++)
	{
		n=0;
		for(j=0;j<x;j++)
		{
			offset= 64 *i + j*8;
			for(k=0;k<8;k++)
			{
				memcpy(tmp + n , p + offset,8);
				n+=8;
				offset+=w;
			}
		}
		memcpy(p + i*tmpl,tmp,tmpl);
	}
	delete tmp;
}
void CreateJPEGFile(LPTSTR pszFile, PBITMAPINFO pbi,HBITMAP hBMP, HDC hDC)
{
//	FlashTray(TEXT("JPEG功能还在试验中，还没做好"),TEXT("对不住啊 :("));
//	return;
	LPBYTE	lpBits;
	LPBYTE	lpY,lpU,lpV;
	HANDLE	hjpegfile;
	HANDLE	hmap;
	UINT	width,height;
	DWORD	writed;
	int		nYUVsize;
	
	width= pbi->bmiHeader.biWidth & 0xFFFFFFF8;
	//计算缓冲区的大小
	height= pbi->bmiHeader.biHeight & 0xFFFFFF8;

	nYUVsize= width * height;
	
	//为图片分配内存
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbi->bmiHeader.biSizeImage);
	//获得位图信息
	GetRGBbits(lpBits,hDC, hBMP, pbi, width,height);
	
	//为三个基色YUV分配缓冲区
	lpY=(LPBYTE)malloc(nYUVsize);
	lpU=(LPBYTE)malloc(nYUVsize);
	lpV=(LPBYTE)malloc(nYUVsize);

	//将RGB信号转化为YUV信号
	RGB2YUV(nYUVsize,lpBits,lpY,lpU,lpV);
	DivBf(lpY,width,height);
	DivBf(lpU,width,height);
	DivBf(lpV,width,height);
	//释放原始的图片
	GlobalFree(lpBits);
	//打开文件
    hjpegfile = CreateFile(pszFile,GENERIC_READ | GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
	hmap	=	CreateFileMapping(hjpegfile,0,PAGE_READWRITE,0,pbi->bmiHeader.biSizeImage*100,0);
	file	=	MapViewOfFile(hmap,FILE_MAP_WRITE,0,0,0);
	//写入文件标志
	WriteFileTag(width,height);
	InitJPEG();//初始化JPEG模块
	
	//开始压缩！！！
//	Compress(lpY,lpU,lpV,nYUVsize,width-height / 64);
	float dctY[64];float dctU[64];float dctV[64];
	short yDC=0,uDC=0,vDC=0;
	int yC=0,uC=0,vC=0;
	int i=0,j=0,k=0,p=0,m=0,n=0,s=0;
	int mcu = (width>>3)*(height>>3);
	for(p=0;p<mcu;p++)
	{
		vC=uC=yC=1;
		for(;i<nYUVsize ; i+= 64)
		{
				for(j=0;j< 64;j++){dctY[j] = lpY[ i + j ] - 128;}
			if(yC>0)
			{
				ProcessDU(dctY,YQT,STD_DC_Y_HT,STD_AC_Y_HT,yDC);
				--yC;
			}else break;
		}
		for(;m<nYUVsize;m+=64)
		{
				for(n=0;n< 64 ;n++)
				{
					dctU[n]=lpU[m+n] - 128;
				}
			if(uC>0)
			{
				--uC;
				ProcessDU(dctU,UVQT,STD_DC_UV_HT,STD_AC_UV_HT,uDC);
			}else break;
		}
		for(;s<nYUVsize;s+=64)
		{
				for(k=0;k<64;k++)
				{
					dctV[k]= lpV[s+k]-128;
				}
			if(vC>0)
			{
				--vC;
				ProcessDU(dctV,UVQT,STD_DC_UV_HT,STD_AC_UV_HT,vDC);
			}else break;
		}
	}

	//压缩完毕！！写文件结束标志
	*((WORD*)curptr)=0xD9FF;
	//关闭文件，大功告成
	UnmapViewOfFile(file);
	CloseHandle(hmap);
	filesize = curptr - (LPSTR)file +2;
	SetFilePointer(hjpegfile,filesize,0,0);
	SetEndOfFile(hjpegfile);
	CloseHandle(hjpegfile);
	file=NULL;
}
static void	InitHookInitFunc(void* exebase)
{
	//得到段基址。
	//PEFILEHEADER
	PIMAGE_SECTION_HEADER	section;
	PIMAGE_NT_HEADERS	pnt;
	int		NumberOfSections;
	int		i;
	InitFunc	*p;
	int		NumofFunc;
	InitFunc	func;

	pnt=(PIMAGE_NT_HEADERS)( (((IMAGE_DOS_HEADER*)exebase)->e_lfanew ) + (ULONG)exebase);
	NumberOfSections = pnt->FileHeader.NumberOfSections;
	section= (PIMAGE_SECTION_HEADER)(DWORD)(   pnt+1   );
	for(i=0;i<NumberOfSections;i++)
	{
		if( strnicmp((LPSTR)(section[i].Name),"InitFunc",8)==0 )
		{
			p = (InitFunc*)(section[i].VirtualAddress + (ULONG)exebase );
			NumofFunc = section[i].SizeOfRawData / sizeof(InitFunc);			
			for( int ii=0; ii< NumofFunc ;ii++)
			{
				func= p[ii];
				if(func)
					func((HINSTANCE)exebase);
				else
					return;

			}
			break;
		}
	}
}