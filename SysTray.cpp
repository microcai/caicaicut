#include "StdAfx.h"
#include "resource.h"
#include "PrtSc.h"

static TCHAR	szTip[]=_T("小菜正在待命！");
void	StaysInsysTray()
{
	NOTIFYICONDATA n={sizeof(NOTIFYICONDATA)};
	n.hIcon=LoadIcon(hInst,_T("Icon_In_Tray"));
	n.hWnd=MainWnd;
	n.uCallbackMessage = WM_TIPS;
	n.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE;
	n.uID=GlobalFindAtom(_T("PrtSc_icon"));
	if(n.uID == 0)
		n.uID=GlobalAddAtom(_T("PrtSc_icon"));
	lstrcpy(n.szTip,szTip);
	Shell_NotifyIcon(NIM_ADD,&n);
}
void __fastcall	FlashTray(LPTSTR info,LPTSTR infoTitle /*= NULL*/,DWORD dwInfoFlags/*=0*/)
{
	NOTIFYICONDATA n={sizeof(NOTIFYICONDATA)};
	n.hWnd=MainWnd;
	n.uCallbackMessage = WM_TIPS;
	n.uFlags=NIF_MESSAGE|NIF_INFO;
	lstrcpy(n.szInfo,info);
	if(infoTitle)
		lstrcpy(n.szInfoTitle,infoTitle);
	n.uID=GlobalFindAtom(_T("PrtSc_icon"));
	n.uTimeout = 500;
	n.dwInfoFlags = dwInfoFlags;
	Shell_NotifyIcon(NIM_MODIFY,&n);
}
void	RemoveTray()
{
	NOTIFYICONDATA n={sizeof(NOTIFYICONDATA)};
	n.hWnd=MainWnd;
	n.uCallbackMessage=WM_TIPS;
	n.uFlags=NIF_MESSAGE;
	n.uID=GlobalFindAtom(_T("PrtSc_icon"));
	Shell_NotifyIcon(NIM_DELETE,&n);

	GlobalDeleteAtom((ATOM)(n.uID));
	while( 	n.uID=GlobalFindAtom(_T("PrtSc_icon")) )
		GlobalDeleteAtom((ATOM)(n.uID));
}
void	ProcessCstyleString(LPTSTR lpstr)
{
	while(*lpstr!=0)
	{
		if(*lpstr=='\\')
		{
			if( (*(lpstr+1)=='r' ))
			{
				*lpstr='\r';
				*++lpstr=0;
				lstrcat(lpstr-1,lpstr+1);		
			}else if(  (*(lpstr+1)=='n' )  )
			{
				*lpstr='\n';
				*++lpstr=0;
				lstrcat(lpstr-1,lpstr+1);			
			}
		}else 		lpstr++;
	}
}
static	LPTSTR GetTipInfo()
{
	TCHAR	section[]=TEXT("HotKeys");
	TCHAR	str[_MAX_PATH];
	LPTSTR static_text=(LPTSTR)LocalAlloc(LPTR,1024);
	
	GetPrivateProfileString(section,TEXT("PrintScreen"),TEXT("Ctrl+F12"),str,_MAX_PATH,inifile);
	_tcscpy(static_text,str);
	
	if(locale_ID!=MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT))
		lstrcat(static_text,TEXT("截全屏\r\n"));
	else
		lstrcat(static_text,TEXT("\tPrintScreen\r\n"));


	GetPrivateProfileString(section,TEXT("PrintWindow"),TEXT("Ctrl+Shift+F12"),str,_MAX_PATH,inifile);
	_tcscat(static_text,str);

	if(locale_ID!=MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT))
		lstrcat(static_text,TEXT("截活动窗口\r\n"));
	else
		lstrcat(static_text,TEXT("\tPrintActive\r\n"));


	GetPrivateProfileString(section,TEXT("CutScreen"),TEXT("Ctrl+Alt+Z"),str,_MAX_PATH,inifile);
	_tcscat(static_text,str);

	if(locale_ID!=MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT))
		lstrcat(static_text,TEXT("区域截取\r\n"));
	else
		lstrcat(static_text,TEXT("\tCutRegion\r\n"));


	GetPrivateProfileString(section,TEXT("Exit"),TEXT("Ctrl+F11"),str,_MAX_PATH,inifile);
	_tcscat(static_text,str);
	
	if(locale_ID!=MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT))
		lstrcat(static_text,TEXT("退出"));
	else
		lstrcat(static_text,TEXT("\tExit"));

	return static_text;
}


void	FlashHelpTips()
{
	LPTSTR	tip=GetTipInfo();
	if(locale_ID!=MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT))
		FlashTray(tip,_T("使用方法："),NIIF_INFO);
	else
		FlashTray(tip,_T("Usage:"),NIIF_INFO);
	LocalFree(tip);
}
void	OnRemoveTray()
{
	TCHAR section[]=TEXT("Junk Words");
	TCHAR Info[128];
	TCHAR title[128];

	LoadString(DllhInst,IDS_OnExitInfo,Info,128);
	GetPrivateProfileString(section,TEXT("OnExitInfo"),
		Info,Info,128,inifile);
	if(!lstrlen(Info))
		LoadString(DllhInst,IDS_OnExitInfo,Info,128);


	LoadString(DllhInst,IDS_OnExitTitle,title,128);
	GetPrivateProfileString(section,TEXT("OnExitTitle"),
		title,title,128,inifile);
	if(!lstrlen(title))
		LoadString(DllhInst,IDS_OnExitTitle,title,128);

	ProcessCstyleString(title);
	ProcessCstyleString(Info);
	FlashTray(Info,title);
}
void	FlashOnlyTips()
{
	TCHAR section[]=TEXT("Junk Words");
	TCHAR Info[128];
	TCHAR title[128];
	GetPrivateProfileString(section,TEXT("OnMutexTitle"),
		_T("You had ran me!"),
		title,128,inifile);
	GetPrivateProfileString(section,TEXT("OnMutex"),
		_T("Don\'t try to run multi-instances"),
		Info,128,inifile);
	FlashTray(Info,title,NIIF_ERROR);
}/*
void	TrayNewIcon(HICON	hicon)
{
	NOTIFYICONDATA n={sizeof(NOTIFYICONDATA)};
	n.hWnd=MainWnd;
	n.hIcon = hicon;
	n.uFlags=NIF_ICON;
	n.uID=GlobalFindAtom(_T("PrtSc"));
	Shell_NotifyIcon(NIM_MODIFY,&n);
}*/