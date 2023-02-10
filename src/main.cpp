
#include "StdAfx.h"
#include "resource.h"
#include "PrtSc.h"
#if !defined( _WIN32)
#include <gtk/gtk.h>
#endif
/*
extern "C" {
	WINBASEAPI DWORD WINAPI RtlGetLastWin32Error();
};*/
static BOOL WINAPI InitInstance(HINSTANCE hInstance)
{
	BOOL		ret;
	WNDCLASS	wc;
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize  = sizeof(INITCOMMONCONTROLSEX);
	InitCtrls.dwICC	  = ICC_HOTKEY_CLASS ;
	InitCommonControlsEx(&InitCtrls);
	InitCommonControls();

	GetModuleFileName(hInstance,inifile,sizeof(inifile));

	LPTSTR	p=inifile + lstrlen(inifile);

	while( *p != '\\' && p!=inifile )--p;	*p=0;
	lstrcat(p,TEXT("\\Settings.ini"));

	locale_ID = GetPrivateProfileInt(_T("Others"),_T("Lang"),GetThreadLocale(),inifile);
	SetThreadLocale(locale_ID);
	while(!LondLangDll())
	{
		DWORD lasterr=GetLastError();
		MSGBOXPARAMS	MsgBoxParams;
		MsgBoxParams.cbSize=sizeof(MsgBoxParams);
		MsgBoxParams.dwContextHelpId = 0;
		MsgBoxParams.dwLanguageId = locale_ID;
		MsgBoxParams.dwStyle = MB_YESNO|MB_ICONERROR|MB_SYSTEMMODAL;
		MsgBoxParams.hInstance = hInst;
		MsgBoxParams.hwndOwner  = 0;
		MsgBoxParams.lpfnMsgBoxCallback=0;
		MsgBoxParams.lpszCaption = (LPTSTR)IDS_ERRTITLE;
		MsgBoxParams.lpszIcon = 0;
		MsgBoxParams.lpszText = (LPTSTR)IDS_ERRCAPTION2;

		if(IDYES==MessageBoxIndirect(&MsgBoxParams))
		{
			if(locale_ID==1033)locale_ID=2052;else locale_ID=1033;
			SetThreadLocale(locale_ID);
		}else exit(lasterr);
	}

	wc.cbClsExtra = 0 ;
	wc.cbWndExtra = sizeof(ShowPicClass);
	wc.hbrBackground = 0;
	wc.hCursor =LoadCursor(0,IDC_ARROW	);
	wc.hIcon = LoadIcon(hInstance,_T("PrtSc"));
	wc.hInstance = hInstance;
	wc.lpszClassName = _T("ShowPicClass");
	wc.lpszMenuName = 0;
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = ShowPicClassWndProc;
	ret=RegisterClass(&wc);
	wc.style = 0;
	wc.hbrBackground =0;

	wc.cbWndExtra = sizeof(WORD);
	wc.lpfnWndProc = MainWndProc;
	wc.lpszClassName=TEXT("##_CaiPrtSc_##MainWindow_Class");
	wc.lpszMenuName = 0;
	ret &=RegisterClass(&wc);
	wc.cbWndExtra = sizeof(DWORD);

	wc.hIcon = LoadIcon(hInstance,_T("RICHEDIT"));
	wc.lpszClassName = _T("NoteWnd");
	wc.lpfnWndProc = NoteWndProc;
	ret &= RegisterClass(&wc);

	wc.cbWndExtra = 0;
	wc.lpszClassName = _T("CutWnd");
	wc.lpfnWndProc =CutWndProc;
	ret&=RegisterClass(&wc);

	wc.cbWndExtra=sizeof(int)*2;
	wc.hbrBackground=(HBRUSH)(COLOR_WINDOW);
	wc.hCursor = LoadCursor(0,IDC_ARROW);
	wc.hIcon = LoadIcon(hInst,_T("PrtSc"));
	wc.lpfnWndProc =JunkWndProc;
	wc.lpszClassName = _T("Junk");
	return ret & RegisterClass(&wc);

}

static BOOL WINAPI IsOnlyInstance()
{
	HWND pre=FindWindow(_T("##_CaiPrtSc_##MainWindow_Class"),_T("MainWindow"));
	if(pre)
	{
		if(SendMessage(pre,WM_APP,0,0)==0)
		return 0;
	}
	return 1;
}

#if defined( _WIN32)
int WINAPI _tWinMain(HINSTANCE hInstance,HINSTANCE,LPTSTR,int)
#else
int main(int argc, char * argv[])
#endif
{
#if defined( _WIN32)
	// init us!
	hInst=hInstance;
	if(!InitInstance(hInst))return -1;
#else
	gtk_init(&argc,&argv);
#endif

	//avoid multi-instance
	if(!IsOnlyInstance())return 1;

	//CreateMainWindow

	CreateWindow(_T("##_CaiPrtSc_##MainWindow_Class"),_T("MainWindow"),
		WS_POPUP,0,0,0,0,0,0,hInst,0);
	//process cmdline
	MSG msg;
	//loop
#ifdef  _WIN32
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
#else
	gtk_main();
	return 0;
#endif

}

//全局变量
HINSTANCE	hInst,DllhInst;

HWND	MainWnd;
TCHAR	inifile[_MAX_PATH];
HICON	hicon;
LCID	locale_ID;

