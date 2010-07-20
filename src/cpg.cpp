#include "Stdafx.h"
#include <math.h>
#include "PrtSc.h"

//void _stdcall RGB2YUV(size_t size,LPBYTE lpRGB,LPBYTE lpY, LPBYTE lpU,LPBYTE lpV);

typedef struct{
	TCHAR		pszFile[_MAX_PATH];
	BITMAPINFO	bi;
	HBITMAP		hBMP;
	HDC			hdc;
	bool		CannotRelease;
}commonThreadparam;
static void cpg_DCT(PFLOAT out,BYTE in[8][8])
{
	float tmp0,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7;
	float tmp10,tmp11,tmp12,tmp13;
	float z1,z2,z3,z4,z5,z11,z13;
	float*dataptr;
	int ctr;
	dataptr=out;
	//数据搬运
	for(ctr=0;ctr<8;ctr++)
		for(int i =0 ; i < 8 ;++i)
			*dataptr++=in[ctr][i];

	/*行计算*/
	for(dataptr=out,ctr=0;ctr<7;++ctr)
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
static BYTE QTable_Y[8][8]=
{
	{16,11,10,16,24,40,51,61},
	{12,12,14,19,26,58,60,55},
	{14,13,16,24,40,57,69,56},
	{14,17,22,29,51,87,80,62},
	{18,22,37,56,68,109,103,77},
	{24,35,55,64,81,104,113,92},
	{49,64,78,87,103,121,120,101},
	{72,92,95,98,112,100,103,99}
};
static BYTE QTable_UV[8][8]=
{
	{17,18,24,47,99,99,99,99},
	{18,21,26,66,99,99,99,99},
	{24,26,56,99,99,99,99,99},
	{47,66,99,99,99,99,99,99},
	{99,99,99,99,99,99,99,99},
	{99,99,99,99,99,99,99,99},
	{99,99,99,99,99,99,99,99},
	{99,99,99,99,99,99,99,99}
};
static void cpg_round(PFLOAT dct,char* out)
{
	static qt[64]=
	{	16,11,10,16,24,40,55,65,
		12,12,14,19,26,58,60,60,
		14,13,16,24,40,57,71,63,
		14,17,22,33,60,87,80,70,
		22,22,37,56,68,109,103,80,
		40,35,55,70,90,110,122,101,
		70,64,78,87,110,121,130,107,
		90,100,110,120,130,103,108,105
	};
	for(int i=0;i<64;++i)
		out[i]=dct[i]/qt[i];
}
void cpg_RLC(char *in,BYTE* out)
{
	/*Z字变换表*/
	static const int Z_L[64]=
	{0,1,5,6,14,15,27,28,2,4,7,13,16,26,29,42,3,8,12,17,25,30,41,43,9,11,18,24,31,40,44,53,10,19,23,33,38,46,51,55,60,21,34,37,47,50,56,59,61,35,36,48,49,57,58,62,63};
	
	char c=1,last=63;
	char m;
	UINT zeroNum=0;
	BYTE * lpCodeLen = out;

	*out=1;

	while( in[Z_L[last]] !=0 && last>0 )last--; //找出非0串的长度
	*(char*)(++out)=*in;//DC系数不编码
	if(last==0)/*全为0*/	return ;
	in++; //移动到AC系数

	while( c <= last )
	{
		m= in[Z_L[c]]  ;

		if( m == 0 )
		{
			zeroNum++;
			c++;
		}
		else
		{
			(*lpCodeLen)++;
			if(zeroNum <4)// 1 00 xxxxx
			{
				if(m>15)m=15;
				if(m<-16)m=-16;
				m += 16;
				 *(++out) = 0x80 | (0x60  & ( zeroNum<<5 ) )|((BYTE) m  & ( 0x1F ) );
				 c++;
			}else if(zeroNum <= 15 && *in < 8 )//zeroNum <=15  0 0000 xxx
			{
				m+=4;
				*(++out) =   (zeroNum <<4) & 0x78 | (BYTE)m ;
				c++;
			}else
				*(++out) =   (zeroNum <<4) & 0x78 ;
			zeroNum=0;
		}
	}
}
typedef struct{
	char	sig[4];
	DWORD	x,y;
}CPG_H;
static void WINAPI ThreadFunc(commonThreadparam *p)
{
	UINT	width,height;
	LPBYTE	lpRGB;
	HANDLE	hf;
	width	=	p->bi.bmiHeader.biWidth  & 0xFFFFFFF8;
	height	=	p->bi.bmiHeader.biHeight & 0xFFFFFFF8;
	lpRGB	=	new BYTE[width*height*4];
	GetDIBits(p->hdc,p->hBMP,0,height,lpRGB,&(p->bi),DIB_RGB_COLORS);
	hf=CreateFile(p->pszFile,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
	p->CannotRelease=false;
	//循环进入每个8*8的小块
	BYTE Color[8][8];
	FLOAT	dct[64];
	UINT	t;
	BYTE	result[64];//压缩嘛，还会大于64B?
	char	qdct[64];
	{
		CPG_H CPG={'C','P','G','!',width,height};
		WriteFile(hf,&CPG,sizeof(CPG),&CPG.x ,0);
	}

	for(UINT i=0;i< width -8 ;i+=8)
		for(UINT j=0;j < height -8 ; j+=8)
		{//在这个格子里，循环每一个像素分别对三个基色FDCT
			UINT ii,jj;
			for(t=0;t<3;t++)//遍历三个颜色  B G R 
			{
				for(ii=0;ii<8;++ii)
					for(jj=0;jj<8;++jj)
						Color[ii][jj]= lpRGB [  width*(j+jj) + (i+ii) +t ]-128;
			//得到八个一组共八组进行DCT
				cpg_DCT(dct,Color);
				//再量化，回到char格式
				cpg_round(dct,qdct);
				cpg_RLC(qdct,result);
				//好了，写入文件吧！
				WriteFile(hf,result,*result+1,(DWORD*)result,0);
			}
		}
	CloseHandle(hf);
}
void	SaveCpjFile(LPTSTR pszFile, PBITMAPINFO pbi,HBITMAP hBMP, HDC hDC)
{
	commonThreadparam p;//=new commonThreadparam;
	lstrcpy(p.pszFile ,pszFile);
	p.hdc = hDC;
	p.hBMP = hBMP;
	p.bi = * pbi;
	CloseHandle(CreateThread(0,0,(LPTHREAD_START_ROUTINE)ThreadFunc,&p,0,0));
	while(p.CannotRelease)Sleep(0);
}
