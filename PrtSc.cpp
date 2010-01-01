
#include "StdAfx.h"
#include "resource.h"
#include "PrtSc.h"

DWORD WINAPI ViewDevNotes(LPVOID)
{
	HMODULE	hmod;
	HWND	main;
	MSG		msg;

	hmod=LoadLibrary(_T("RICHED32.DLL"));
	SetThreadLocale(locale_ID);
	AutoDeleteString lpWindowName;
	lpWindowName.LoadString(DllhInst,IDS_DEVNOTECAPTION);
	
	lstrcat(lpWindowName,_T(__DATE__));
	main=CreateWindowEx(0,TEXT("NoteWnd"),lpWindowName,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,0,(HMENU)0,hInst,0);

	ShowWindow(main,SW_SHOW);
	UpdateWindow(main);

	while(GetMessage(&msg,0,0,0))//,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	FreeLibrary(hmod);
	return msg.lParam ;
}


void CutRegion()
{
	RECT rc;
	AutoDeleteString	lpWndTxt;

	GetWindowRect(GetDesktopWindow(),&rc);

	lpWndTxt.LoadString(DllhInst,IDS_CURWNDCAPTION);
	HWND hwnd=CreateWindowEx(WS_EX_TRANSPARENT,_T("CutWnd"),lpWndTxt,WS_POPUP,
		0,0,rc.right,rc.bottom,0,0,0,0);

	SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOREDRAW);

	ShowWindow(hwnd,SW_MAXIMIZE);
	UpdateWindow(hwnd);
}
static void	BmpToClip(HBITMAP hbitmap)
{
	int tryed=0;
	while(OpenClipboard(MainWnd)==FALSE&&tryed<100)
	{
		tryed++;
		Sleep(10);
	}
	if(tryed<100){
		EmptyClipboard();		
		SetClipboardData(CF_BITMAP,hbitmap);
		CloseClipboard();
	}else{MessageBox(MainWnd,TEXT("剪切板超时"),TEXT("妮子, :("),0);}
}
static UINT APIENTRY OFNHookProc(HWND hdlg,UINT message,WPARAM wParam,LPARAM lParam);
static BOOL	GetFileName(LPTSTR filename)
{
	AutoDeleteString lpstrTitle;
	
	lpstrTitle.LoadString(DllhInst,IDS_GTFLNMTITLE);
	
	OPENFILENAME file={sizeof(OPENFILENAME)};
	file.hInstance=hInst;
	file.hwndOwner =MainWnd;
	file.lpstrDefExt=TEXT("bmp");
	file.lpstrFilter=TEXT("位图文件\0*.bmp\0\0JPEG格式(*.jpg)\0*.jpg\0CPG文件(*.CPG)\0*.cpg\0");
	file.lpstrTitle= lpstrTitle;//TEXT("妮子，保存吧，^_^(取消写入剪切板,双击放大查看)");
	file.nMaxFile=_MAX_PATH;
	file.lpstrFile=filename;
	file.lpfnHook = OFNHookProc;
	file.Flags=OFN_EXPLORER|OFN_ENABLEHOOK|OFN_HIDEREADONLY|OFN_SHAREAWARE
		|OFN_ENABLETEMPLATE|OFN_ENABLEINCLUDENOTIFY;
	file.lpTemplateName = (LPTSTR)_T("tmplate");
	return GetSaveFileName(&file);
}
__declspec(thread)	static	HBITMAP	bmp;
static UINT APIENTRY OFNHookProc(HWND hdlg,UINT message,WPARAM,LPARAM)
{
	RECT	rc;
	//LPOFNOTIFY	
	switch(message)
	{
	case WM_LBUTTONDBLCLK:
		SendDlgItemMessage(hdlg,56,STM_SETIMAGE,0,0);
		{
			HBITMAP	tmp=bmp;
			bmp=0;
			EndDialog(GetParent(hdlg),0);
			CreateViewPicWnd(tmp);
		}
		break;
	case WM_INITDIALOG:
		GetClientRect(hdlg,&rc);
		hdlg=CreateWindowEx(0,_T("ShowPicClass"),_T(""),WS_DISABLED|WS_CHILD|WS_VISIBLE,0,0,0,0,hdlg,(HMENU)56,hInst,0);
		SendMessage(hdlg,STM_SETIMAGE,0,(LPARAM)bmp);
		MoveWindow(hdlg,0,0,rc.right,rc.bottom,0);
	}
	return 0;
}

DWORD	WINAPI	StoreBMP(HBITMAP	hbmp)
{
	SetThreadLocale(locale_ID);
	LPTSTR	filename;
	filename=new TCHAR[_MAX_PATH];
	filename[0]=0;
	bmp=hbmp;

	if(!GetFileName(filename))
	{
		delete[] filename;
		if(bmp)
		{
			BmpToClip(hbmp);
			AutoDeleteString HasToClipbord;
			HasToClipbord.LoadString(DllhInst,IDS_TOCLIPBOARD);
			FlashTray(HasToClipbord,0);//已放入剪切板
		}
		return 0;
	}
	SaveBmpToFile(hbmp,filename,FT_BMP);
	delete[]filename;
	return 0;
}
void	CutScreen(RECT rc)
{
	HDC		hdc;
	HDC		mdc;
	HBITMAP	hbmp;
	hdc=CreateDC(_T("DISPLAY"),0,0,0);
	mdc=CreateCompatibleDC(hdc);
	hbmp=CreateCompatibleBitmap(hdc,rc.right - rc.left , rc.bottom - rc.top);
	DeleteObject(SelectObject(mdc,hbmp));
	BitBlt(mdc,0,0,rc.right - rc.left , rc.bottom - rc.top,hdc,rc.left,rc.top,SRCCOPY);
	DeleteDC(hdc);
	DeleteDC(mdc);
	CloseHandle(CreateThread(0,0,(LPTHREAD_START_ROUTINE)StoreBMP,hbmp,0,0));
}
void	CutWindow(HWND hwnd)
{
	HDC		hdc;
	HDC		mdc;
	RECT	rc;
	HBITMAP	hbmp;
	hdc=GetWindowDC(hwnd);
	GetWindowRect(hwnd,&rc);
	rc.right -= rc.left;
	rc.bottom-=rc.top;
	rc.top=rc.left=0;
	mdc=CreateCompatibleDC(hdc);
	hbmp=CreateCompatibleBitmap(hdc,rc.right, rc.bottom);
	DeleteObject(SelectObject(mdc,hbmp));
	BitBlt(mdc,0,0,rc.right , rc.bottom ,hdc,0,0,SRCCOPY);
	DeleteDC(hdc);
	DeleteDC(mdc);
	CloseHandle(CreateThread(0,0,(LPTHREAD_START_ROUTINE)StoreBMP,hbmp,0,0));
}

BOOL  WINAPI	LondLangDll()
{
	TCHAR langlib[_MAX_PATH];

	if(DllhInst)
		FreeLibrary(DllhInst);
	_ltot(locale_ID,langlib,10);
	lstrcat(langlib,_T("\\lang.dll"));
	DllhInst = LoadLibraryEx(langlib,0,LOAD_LIBRARY_AS_DATAFILE);
	return DllhInst!=NULL;
}

AutoDeleteString::AutoDeleteString(int len)
{
	m_len = len;
	m_str = NEWSTR(len);
}
AutoDeleteString::~AutoDeleteString(){DELSTR(m_str);}
void AutoDeleteString::LoadString(HINSTANCE hinst,UINT id)
{
	::LoadString(hinst,id,m_str,m_len);
}