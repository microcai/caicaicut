
#include "StdAfx.h"
#include <vfw.h>
#include "resource.h"
#include "PrtSc.h"


static float  __inline GetScaling(HWND hwnd)
{
	float * p;
	long	ret= GetWindowLong(hwnd,FIELD_OFFSET(ShowPicClass,scaling));
	p=(float*) &ret;
	return p[0];
}

static void	__fastcall Repaint(HWND	hWnd,RECT & updata,RECT& upbmp,HDC hdc)
{
	PAINTSTRUCT ps={0};
	HDRAWDIB	hdd;
	HBITMAP		hbmp;
	BITMAPINFOHEADER	bi={sizeof(bi)};
	void*		bits=LocalAlloc(LPTR,4096*4096*5);
	
	POINTS	bmprc;
	*((DWORD*)(&bmprc))= GetWindowLong(hWnd,FIELD_OFFSET(ShowPicClass,bmRC));

	hdd=DrawDibOpen();

	if(!hdc)
		hdc =BeginPaint(hWnd,&ps);

	hbmp=(HBITMAP)GetWindowLong(hWnd,FIELD_OFFSET(ShowPicClass,hbmp));
	
	BITMAP	bm;

	GetObject(hbmp,sizeof(bm),&bm);
	bi.biBitCount=32;
	bi.biWidth = bm.bmWidth;
	bi.biPlanes = 1;
	bi.biHeight = bm.bmHeight ;
	
	ExcludeClipRect(hdc,updata.left,updata.top,updata.right,updata.bottom);

	FillRect(hdc,&ps.rcPaint,(HBRUSH)1);
	
	SelectClipRgn(hdc,NULL);

	GetDIBits(hdc,hbmp,0,bi.biHeight,bits,(LPBITMAPINFO)&bi,DIB_RGB_COLORS);

	DrawDibDraw(hdd,hdc,updata.left,updata.top,updata.right-updata.left,updata.bottom-updata.top,
		&bi,bits,upbmp.left,upbmp.top,upbmp.right-upbmp.left,upbmp.bottom-upbmp.top,0);
	
	LocalFree(bits);

	DrawDibClose(hdd);
	
	if(ps.hdc)
		EndPaint(hWnd,&ps);
	
}

static void OnPaint(HWND hwnd,HDC hdc)
{
	long	flag;
	RECT	updata;
	POINT	central;
	POINTS	bmprc;
	POINTS	viewcentral;
	RECT	clientRect;
	RECT	upbmp;
	float	Scale;
	//最新得到旗标	
	flag=GetWindowLong(hwnd,FIELD_OFFSET(ShowPicClass,flag));
	
	//先得到客户区的大小
	GetClientRect(hwnd,&clientRect);
	//再得到窗口中心的坐标
	central.x = clientRect.right/2;
	central.y = clientRect.bottom/2;
	//得到图片大小
	*((DWORD*)(&bmprc))= GetWindowLong(hwnd,FIELD_OFFSET(ShowPicClass,bmRC));
	//得到更新区域
//	GetUpdateRect(hwnd,&updata,1);
//	updata.right+=5;
//	updata.left-=5;/
//	updata.top-=5;
//	updata.bottom+=5;
	GetClientRect(hwnd,&updata);

	//得到视图中心在图片上的位置
	if(flag & SHP_ZOOMAUTOSTRECH )
	{
		GetClientRect(hwnd,&updata);
		upbmp.left = 0;
		upbmp.right = bmprc.x;
		upbmp.top = 0;
		upbmp.bottom = bmprc.y;
		
		viewcentral.x = bmprc.x/2,viewcentral.y = bmprc.y/2;
		//得到更新区域对应于图片的矩形区域 --> upbmp;
		if(flag & SHP_ZOOMKEEPRATIO)
		{
		/*纵横比*/Scale = (float)viewcentral.x/viewcentral.y;
			upbmp.top  = (long)(viewcentral.y - (viewcentral.y * Scale));
			if(upbmp.top < 0 ) //很宽啊!
			{
				upbmp.bottom = (long)(viewcentral.y + viewcentral.y * Scale);
				upbmp.left = 0;
				upbmp.right = bmprc.x;
			}else
			{
				upbmp.top = 0;
				upbmp.bottom = bmprc.y;
				upbmp.right = (long)(viewcentral.x + viewcentral.x * Scale);
				upbmp.left  = (long)(viewcentral.x - viewcentral.x * Scale);
			}
		}else
		{
			// 客户区域对应于图片的矩形区域就是整张图片
			upbmp.left = upbmp.top =0;
			upbmp.bottom = bmprc.y;
			upbmp.right = bmprc.x;
		}
//		upbmp.left  =(long)(((float)updata.left)  /clientRect.right * (upbmp.right-upbmp.left));
//		upbmp.right =(long)(((float)updata.right) /clientRect.right * (upbmp.right-upbmp.left));
//		upbmp.top   =(long)(((float)updata.top )  /clientRect.bottom* (upbmp.bottom - upbmp.top));
//		upbmp.bottom=(long)(((float)updata.bottom)/clientRect.bottom* (upbmp.bottom - upbmp.top));
	}
	else 
	{

		*((DWORD*)(&viewcentral))=GetWindowLong(hwnd,FIELD_OFFSET(ShowPicClass,central));
		
		//缩放率
		Scale =1/GetScaling(hwnd);

		//依据缩放率调整

		upbmp.left = viewcentral.x - (updata.right - updata.left) * Scale / 2;
		upbmp.right = viewcentral.x + (updata.right - updata.left) * Scale / 2;

		upbmp.top = viewcentral.y -  (updata.bottom - updata.top) * Scale / 2;
		upbmp.bottom = viewcentral.y + (updata.bottom - updata.top) * Scale / 2;

		
		if(upbmp.top < 0)
		{
			updata.top += ((-upbmp.top)/Scale+0.5);
			upbmp.top =0;
		}

		if (upbmp.left < 0)
		{
			updata.left  += ((-upbmp.left)/Scale+0.5);
			upbmp.left = 0;
		}

		if(upbmp.right > bmprc.x)
		{
			updata.right -= ((upbmp.right - bmprc.x)/Scale + 0.5);
			upbmp.right = bmprc.x;
		}
		
		if(upbmp.bottom > bmprc.y)
		{
			updata.bottom -= (  (upbmp.bottom - bmprc.y)/Scale+0.5  );
			upbmp.bottom = bmprc.y;		
		}
		
	}
	//把位图里要更新的部分画到更新区域
	//rePaint(hwnd,updata,upbmp);
	Repaint(hwnd,updata,upbmp,hdc);
}

static void   __inline SetScaling(HWND hwnd,float Scale){SetWindowLong(hwnd,FIELD_OFFSET(ShowPicClass,scaling), *((DWORD*)&Scale));}
static DWORD	WINAPI	CreateViewPicWnd(void*bmp)
{
	SetThreadLocale(locale_ID);
	HWND	pic;
	MSG		msg;
	pic=CreateWindowEx(0,_T("ShowPicClass"),_T("截图查看(双击保存)"),
		WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		0,0,hInst,0);
	SendMessage(pic,STM_SETIMAGE,0,(LPARAM)bmp);
	SetWindowLong(pic,0,GetWindowLong(pic,0)|SHP_DELBMPONCLOSE);
	SendMessage(pic,WM_SETFLAG,0,0);
	SetScaling(pic,5.0);

	AnimateWindow(pic,500,AW_ACTIVATE|AW_VER_NEGATIVE|AW_SLIDE);
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

static int	FillRect(HDC hdc,int l,int t,int r,int b)
{
	RECT	rc;
	SetRect(&rc,l,t,r,b);
	return FillRect(hdc,&rc,(HBRUSH)1);
}

static void	OnCommand(HWND	hWnd,WPARAM wParam,LPARAM lParam)
{
	switch(wParam & 0xFFFF)
	{
	case IDM_AUTOSTRECH:
		SendMessage(hWnd,WM_SETFLAG,0,GetWindowLong(hWnd,0) ^ SHP_ZOOMAUTOSTRECH);
		break;
	case IDM_ZOOM_B:
		SetScaling(hWnd,GetScaling(hWnd)*2);
		if(GetScaling(hWnd)>20)
			SetScaling(hWnd,20);
		break;
	case IDM_ZOOM_S:
		SetScaling(hWnd,GetScaling(hWnd)/2);
		if(GetScaling(hWnd)< 0.1f)
			SetScaling(hWnd,0.1f);
		break;
	case IDM_ZOOM_ORIG:
		SetScaling(hWnd,1);
		break;
	}
	InvalidateRect(hWnd,0,0);
}
static void	OnMouseMove(HWND hwnd,UINT	message,WPARAM wParam,LPARAM lParam)
{
//	RECT	rc;
	float	s;
	static POINT	p;
	static POINTS	viewcentral;
	POINTS	t;
	switch(message)
	{
	case WM_MOUSEMOVE:
		if(wParam & MK_LBUTTON	)
		{
		s=GetScaling(hwnd);
		//将视图中心点移动这么多
		t=viewcentral;
		t.x += (p.x - (lParam & 0xFFFF))/s;
		t.y += (p.y - (lParam >> 16))/s;
		SetWindowLong(hwnd,FIELD_OFFSET(ShowPicClass,central),*((DWORD*)(&t)));		
		//设置滚动条
		

		//更新视图
		InvalidateRect(hwnd,0,0);
		}break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		break;
	case WM_LBUTTONDOWN:
		*((DWORD*)(&viewcentral))=GetWindowLong(hwnd,FIELD_OFFSET(ShowPicClass,central));
		SetCapture(hwnd);
		//得到鼠标点击地
		p.x = (lParam & 0xFFFF);
		p.y = (lParam >>16 );
	}
}

LRESULT	WINAPI	ShowPicClassWndProc(HWND hWnd,UINT	message,WPARAM wParam,LPARAM lParam)
{
	POINTS	c;
	POINTS	bmprc;
	HMENU	hmenu;
	POINT	curpos;
	switch(message)
	{
	case WM_SIZE:
		InvalidateRect(hWnd,0,0);
		break;
	case WM_HSCROLL:
	case WM_VSCROLL:
		*((DWORD*)(&c))=GetWindowLong(hWnd,FIELD_OFFSET(ShowPicClass,central));
		*((DWORD*)(&bmprc))=GetWindowLong(hWnd,FIELD_OFFSET(ShowPicClass,bmRC));
		switch(wParam & 0xFFFF)
		{
		case SB_THUMBTRACK:
			SetScrollPos(hWnd,message-WM_HSCROLL,wParam >>16,0);
			((short*)(&c))[message-WM_HSCROLL]=  ((float)(wParam >>16))/100 * ((short*)(&bmprc))[message-WM_HSCROLL];
			break;
		case 0:
			return SendMessage(hWnd,message,MAKELONG(SB_THUMBTRACK,GetScrollPos(hWnd,message-WM_HSCROLL)-1),0);
		case 1:
			return SendMessage(hWnd,message,MAKELONG(SB_THUMBTRACK,GetScrollPos(hWnd,message-WM_HSCROLL)+1),0);
		case 2:
			return SendMessage(hWnd,message,MAKELONG(SB_THUMBTRACK,GetScrollPos(hWnd,message-WM_HSCROLL)-3),0);
		case 3:
			return SendMessage(hWnd,message,MAKELONG(SB_THUMBTRACK,GetScrollPos(hWnd,message-WM_HSCROLL)+3),0);
		}
		SetWindowLong(hWnd,FIELD_OFFSET(ShowPicClass,central), ((long*)(&c))[0]);
		InvalidateRect(hWnd,0,1);
		break;
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
		OnMouseMove(hWnd,message,wParam,lParam);
		break;
	case WM_MOUSEWHEEL:
		if(  (short)HIWORD(wParam) > 0)
		{
			SetScaling(hWnd,GetScaling(hWnd)*1.1f);
			if(GetScaling(hWnd)>20)
				SetScaling(hWnd,20);
		}else{
			SetScaling(hWnd,GetScaling(hWnd)/1.1f);
			if(GetScaling(hWnd)< 0.1f)
				SetScaling(hWnd,0.1f);
		}
		InvalidateRect(hWnd,0,0);
		break;
	case WM_RBUTTONUP:
		hmenu=LoadMenu(DllhInst,_T("Viewer"));
		GetCursorPos(&curpos);
		CheckMenuItem(GetSubMenu(hmenu,0),IDM_AUTOSTRECH, (GetWindowLong(hWnd,0) & SHP_ZOOMAUTOSTRECH)? MF_CHECKED:MF_UNCHECKED	);
		TrackPopupMenu(GetSubMenu(hmenu,0),TPM_LEFTALIGN,curpos.x,curpos.y,0,hWnd,0);
		DestroyMenu(hmenu);
		break;
	case WM_LBUTTONDBLCLK:
		CloseHandle(CreateThread(0,0,(LPTHREAD_START_ROUTINE)StoreBMP,
			(LPVOID)GetWindowLong(hWnd,FIELD_OFFSET(ShowPicClass,hbmp)),0,0));
		EnableWindow(hWnd,FALSE);
		AnimateWindow(hWnd,1500,AW_HIDE|AW_BLEND);
		SendMessage(hWnd,STM_SETIMAGE,0,0);
		SendMessage(hWnd,WM_CLOSE,0,0);
		break;
	case STM_SETIMAGE:
		SetWindowLong(hWnd,4,lParam);
		if(lParam==NULL)break;
		{
			BITMAP	bmtype;
			GetObject((HBITMAP)lParam,sizeof(BITMAP),&bmtype);
			SetWindowLong(hWnd,8,MAKELONG(bmtype.bmWidth ,bmtype.bmHeight));
			c.x = bmtype.bmWidth / 2;
			c.y = bmtype.bmHeight/2;
			SetWindowLong(hWnd,FIELD_OFFSET(ShowPicClass,central), ((long*)(&c))[0]);
		}
		break;
	case WM_COMMAND:
		OnCommand(hWnd,wParam,lParam);
		break;
	case WM_PAINT:
		OnPaint(hWnd,NULL);
		break;
	case WM_PRINTCLIENT:
		OnPaint(hWnd,(HDC)wParam);
		break;
	case WM_CLOSE:
		DeleteObject((HBITMAP)GetWindowLong(hWnd,FIELD_OFFSET(ShowPicClass,hbmp)));
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		if(!( GetWindowLong(hWnd,GWL_STYLE) & WS_CHILD ))//子窗口怎么能让消息循环退出呢？
			PostQuitMessage(0);
		break;
	case WM_SETFLAG:
		if(lParam & SHP_ZOOMAUTOSTRECH)
		{
			SetWindowLong(hWnd,GWL_STYLE,GetWindowLong(hWnd,GWL_STYLE) & ~WS_HSCROLL & ~WS_VSCROLL);
			SetScrollRange(hWnd,0,0,0,1);
			SetScrollRange(hWnd,1,0,0,1);
		}
		else
		{
			SetWindowLong(hWnd,GWL_STYLE,GetWindowLong(hWnd,GWL_STYLE)|WS_HSCROLL|WS_VSCROLL);
			SetScrollPos(hWnd,SB_HORZ,50,1);
			SetScrollPos(hWnd,SB_VERT,50,1);
		}
		SetWindowLong(hWnd,FIELD_OFFSET(ShowPicClass,flag),lParam);
		break;
	case WM_CREATE:
		SetWindowLong(hWnd,FIELD_OFFSET(ShowPicClass,flag),SHP_ZOOMAUTOSTRECH);
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	if(wParam == VK_RETURN )
	{
		if(SetWindowLong(hWnd,GWL_STYLE,GetWindowLong(hWnd,GWL_STYLE)^WS_CAPTION) & WS_CAPTION)
			ShowWindow(hWnd,SW_MAXIMIZE);
		else
			ShowWindow(hWnd,SW_NORMAL);
		UpdateWindow(hWnd);
	}
	default:
		return DefWindowProc(hWnd,message,wParam,lParam);
	}
	return 0;
}
void	CreateViewPicWnd(HBITMAP bmp)
{
	CloseHandle(CreateThread(0,0,CreateViewPicWnd,bmp,0,0));
}
