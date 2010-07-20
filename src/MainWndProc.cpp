#include "StdAfx.h"
#include "resource.h"
#include "PrtSc.h"

static __inline void OnHotKey(WPARAM wParam,LPARAM)
{
	RECT	rc;
#ifdef	HOTKYE_ID_BASE

	switch(wParam)
	{
	case HOTKYE_ID_EXITPROGM:
		SendMessage(MainWnd,WM_CLOSE,0,0);
		break;
	case HOTKYE_ID_CUTREGION:
		CutRegion();
		break;
	case HOTKYE_ID_CUTACTIVE:
		CutWindow(GetForegroundWindow());
		break;
	case HOTKYE_ID_CUTSCREEN:
		GetWindowRect(GetDesktopWindow(),&rc);
		CutScreen(rc);
	}
#else
	if(wParam == HOTKYE_ID_CUTACTIVE)
		CutWindow(GetForegroundWindow());
	else if(wParam == HOTKYE_ID_CUTSCREEN)
	{
		GetWindowRect(GetDesktopWindow(),&rc);
		CutScreen(rc);
	}else if(wParam == HOTKYE_ID_EXITPROGM)
		SendMessage(MainWnd,WM_CLOSE,0,0);
	else if(wParam==HOTKYE_ID_CUTREGION)
		CutRegion();
#endif
}

static LRESULT CALLBACK OnCommand(HWND hwnd,WPARAM wParam ,LPARAM lParam)
{
	HMENU hm;
	switch(LOWORD(wParam))
	{
	case IDM_EXIT:
		SendMessage(hwnd,WM_CLOSE,0,0);
		break;
	case IDM_SETUP: //…Ë÷√
		hm=GetSubMenu(GetMenu(hwnd),0);
		EnableMenuItem(hm,IDM_SETUP,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
		DialogBox(DllhInst, (LPCTSTR)IDD_DIALOG_SETTING, hwnd, (DLGPROC)SettingsDlg);
		EnableMenuItem(hm,IDM_SETUP,MF_BYCOMMAND|MF_ENABLED);
		break;
	case IDM_HELP:
		SendMessage(hwnd,WM_FLASHHELP,0,0); 
		break;
	case IDM_DEVNOTE:
		CloseHandle(CreateThread(0,0,ViewDevNotes,0,0,0));
		break;
	case IDM_WHOAMI:
		CloseHandle(CreateThread(0,0,ViewJunk,0,0,0));//SFF_PLAINRTF
		break;
	case IDM_VIEWVKMAP:
		ShellExecute(0,_T("open"),_T("–Èƒ‚º¸¬Î∂®“Â.png"),0,0,SW_MAXIMIZE);
		break;
	case IDM_PRTSC:
		SendMessage(hwnd,WM_HOTKEY,HOTKYE_ID_CUTSCREEN,0);
		break;
	case IDM_CUTRGN:
		SendMessage(hwnd,WM_HOTKEY,HOTKYE_ID_CUTREGION,0);
		break;
	case IDM_LANG_CN:
		locale_ID = MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_DEFAULT);
		SendMessage(hwnd,WM_SETTINGCHANGE,0,0);
		break;
	case IDM_LANG_EN:
		locale_ID = MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT);
		SendMessage(hwnd,WM_SETTINGCHANGE,0,0);
		break;
	case IDM_EDIT_INI:
		ShellExecute(0,_T("open"),_T("notepad.exe"),inifile,0,SW_SHOW);
		break;
	}
	return 0;
}
static void DllLoadErr()
{
	AutoDeleteString ErrCaption,ErrTitle;

	DWORD lasterr=GetLastError();
	ErrCaption.LoadString(hInst,IDS_ERRCAPTION);
	ErrCaption.LoadString(hInst,IDS_ERRTITLE);

	MessageBoxEx(MainWnd,ErrCaption,ErrTitle,MB_ICONERROR|MB_SYSTEMMODAL,(WORD)(locale_ID & 0xFFFF));
}

LRESULT CALLBACK MainWndProc(HWND hwnd,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	POINT p;
	HMENU hm;
	switch(uMsg)
	{
	case WM_HOTKEY:
		OnHotKey(wParam,lParam);
		break;
	case WM_EXPLORER_RESTART:
		RemoveTray();
		StaysInsysTray();
		break;
	case WM_TIPS:
		switch(lParam)
		{
		case WM_MBUTTONDOWN:
			GetCursorPos(&p);
			hm=LoadMenu(DllhInst,_T("Junk"));
			TrackPopupMenu(GetSubMenu(hm,0),TPM_LEFTBUTTON|TPM_RIGHTBUTTON,p.x,p.y,0,hwnd,0);
			DestroyMenu(hm);
			break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			GetCursorPos(&p);
			hm=GetMenu(hwnd);
			hm=GetSubMenu(hm,0);
			TrackPopupMenu(hm,TPM_LEFTBUTTON|TPM_RIGHTBUTTON,p.x,p.y,0,hwnd,0);
			break;
		case NIN_BALLOONTIMEOUT:
			if(GetWindowWord(hwnd,0))
				DestroyWindow(hwnd);
		}break;
	case WM_COMMAND:
		return OnCommand(hwnd,wParam,lParam);
	case WM_FLASHHELP:
		FlashHelpTips();
		break;
	case WM_CREATE:
		MainWnd = hwnd;
		SetMenu(hwnd,LoadMenu(DllhInst,_T("MENU")));
		StaysInsysTray();
		if(RegisterHotKey(MainWnd))
			SendMessage(hwnd,WM_FLASHHELP,0,0);
		else
		{
			TCHAR	HOTKEYERRINFO[32],HOTKEYERRTITLE[32];
			LoadString(DllhInst,IDS_HOTKEYERRINFO,HOTKEYERRINFO,32);
			LoadString(DllhInst,IDS_HOTKEYERRTITLE,HOTKEYERRTITLE,32);

			FlashTray(HOTKEYERRINFO,HOTKEYERRTITLE,NIIF_ERROR);
		}
//		SetTimer(hwnd,7,800,0);
		break;
	case WM_CLOSE:
		UnRegisterHotKey(hwnd);
		OnRemoveTray();
		SetWindowWord(hwnd,0,1);
		SetTimer(hwnd,8,2400,0);
		break;
	case WM_SETTINGCHANGE:
		SetThreadLocale(locale_ID);
		DestroyMenu(GetMenu(hwnd));
		if(!LondLangDll())
		{
			DllLoadErr();
			exit(2);
		}
		SetMenu(hwnd,LoadMenu(DllhInst,_T("MENU")));
		{
			TCHAR lang[32];
			_ltot(locale_ID,lang,10);
			WritePrivateProfileString(_T("Others"),_T("Lang"),lang,inifile);
		}
		break;
	case WM_TIMER:
	/*	switch(wParam)
		{
		case 8:*/
			DestroyWindow(hwnd);
			break;
	/*	case 7:
			{
				static int	p=0;
				p ^= 0x00000001;
				if(p & 1 )
					TrayNewIcon(LoadIcon(hInst,_T("Icon_In_Tray")));
				else
					TrayNewIcon(LoadIcon(hInst,_T("Icon_In_Tray_2")));
			}
		}
		break;*/
	case WM_DESTROY:
		KillTimer(hwnd,8);
		RemoveTray();
		PostQuitMessage(0);
		break;
	case WM_APP:
		if(!GetWindowWord(hwnd,0))
			FlashOnlyTips();
		else
		{
			PostMessage(hwnd,WM_TIMER,8,0);
			return 1;
		}
		break;
	default:
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	}
	return 0;
}
