
#include "StdAfx.h"
#include <richedit.h>
#include "resource.h"
#include "PrtSc.h"
#define SETRTFFONT 0
static DWORD WINAPI	callback(DWORD dwCookie,LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	struct cookie{
		LPSTR	res;
		LPSTR	current;
		LONG	size;
		LONG	copyed;
	};
	struct cookie * ck = (struct cookie *)dwCookie;
	*pcb=0;
	if((ck->size - ck->copyed)== 0 )
	{
		if(ck->copyed)
		{
			char	note[]="\r\n\r\n\t\t\t";
			strcpy((LPSTR)pbBuff,note);
			strcat((LPSTR)pbBuff,__DATE__);//,sizeof(__DATE__));
			strcat((LPSTR)pbBuff,"   ");
			strcat((LPSTR)pbBuff,__TIME__);
			*pcb= sizeof(__DATE__) + sizeof(__TIME__) + sizeof(note);
			ck->copyed= ck->size=0;
			return 0 ;
		}else
		{
			return 1;
		}
	}
	
	cb = min(cb,ck->size - ck->copyed);

	*pcb= _tcslen((LPTSTR)ck->current);
	cb= min(cb,*pcb);

	memcpy(pbBuff,ck->current, cb );
	*pcb=cb;
	ck->copyed+= cb;
	ck->current+= cb;
#ifdef	_DEBUG
	if(*pcb !=4092 )
		return 0;
#endif
	return 0;
}
__declspec(thread)	static WNDPROC	preRchproc=0;
__declspec(thread)	static HCURSOR	hcur=0;
static LRESULT CALLBACK myRchWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		CallWindowProc(preRchproc,hwnd,uMsg,wParam,lParam);
	case WM_LBUTTONDOWN:
		SetCursor(hcur);
		break;
	default:
		return CallWindowProc(preRchproc,hwnd,uMsg,wParam,lParam);
	}
	return 0;
}
LRESULT CALLBACK NoteWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	hcur=LoadCursor(0,IDC_ARROW);
	HWND rch;
#if SETRTFFONT
	HFONT	hfont;
#endif
	switch(uMsg)
	{
	case WM_SIZE:
		rch=(HWND)GetWindowLong(hwnd,GWL_USERDATA);
		MoveWindow(rch,0,0,LOWORD(lParam),HIWORD(lParam),1);
		break;
	case WM_DESTROY:
#if	SETRTFFONT

		hfont=(HFONT)GetWindowLong(hwnd,0);
		DeleteObject(hfont);
#endif
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		{
			HRSRC	hres;

			struct{
				LPSTR	res;
				LPSTR	current;
				LONG	size;
				int		copyed;
			}cookie;

			EDITSTREAM stream;

			stream.dwError = 0;
			stream.dwCookie = (DWORD)(&cookie);
			stream.pfnCallback = callback;
			hres  =	FindResource(DllhInst, (LPTSTR)1,TEXT("TEXT"));
			cookie.size  =	SizeofResource(DllhInst,hres);
			cookie.res =	(LPSTR)LockResource(LoadResource(DllhInst,hres));
			cookie.copyed= 0;
			cookie.current= cookie.res;
			rch=CreateWindowEx(0,RICHEDIT_CLASS,_T(""),
				WS_CHILD|WS_VISIBLE|WS_VSCROLL|ES_READONLY|ES_MULTILINE,
				0,0,0,0,hwnd,(HMENU)80,0,0);
			SetWindowLong(hwnd,GWL_USERDATA,(LONG)rch);
#if	SETRTFFONT
			hfont=CreateFont(18,18,0,0,FW_NORMAL,0,0,0,GB2312_CHARSET,
				OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
				DEFAULT_PITCH|(FF_DONTCARE<<16),_T("FixedSys"));
			SetWindowLong(hwnd,0,(LONG)hfont);
			SendMessage(rch,WM_SETFONT,(WPARAM)hfont,MAKELONG(0,0));
#endif
			SendMessage(rch,EM_STREAMIN,SF_TEXT,(LPARAM)&stream);
			HideCaret(rch);
			HideCaret(rch);
		//	SetClassLong(rch,GCL_HCURSOR,(long)LoadCursor(0,IDC_CROSS	));UNALIGNED
		//	preRchproc=(WNDPROC)SetWindowLong(rch,GWL_WNDPROC,(LPARAM)myRchWndProc);
		}break;
	default:
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	}
	return 0;
}

static BOOL	LoadUserDefinedIcon()//false if 内置
{
	TCHAR	str[_MAX_PATH];
	GetPrivateProfileString(TEXT("Others"),TEXT("Cursor"),TEXT(""),str,_MAX_PATH,inifile);
	if(lstrlen(str))
	{
		hicon=LoadCursorFromFile(str);
		return true;
	}else hicon=LoadCursor(hInst,(LPTSTR)IDC_CURSOR_CUT);
	return false;
}

LRESULT CALLBACK CutWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

	static	HDC		hdc=0;
	static POINT downpt={0,0};
	static RECT	lastRC={0};
	int		t;
	RECT	rc;
	POINT	pt;

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		if(wParam & MK_LBUTTON	)
		{
			pt.x  = LOWORD(lParam);  // horizontal position of cursor 
			pt.y = HIWORD(lParam);  // vertical position of cursor 
			rc.left= downpt.x;
			rc.top = downpt.y;
			switch(wParam)
			{
			case MK_LBUTTON:
				rc.right = pt.x;
				rc.bottom= pt.y;
				break;
			case MK_SHIFT|MK_LBUTTON:
				t = min( abs(pt.x - rc.left),abs(pt.y - rc.top) );
				if(pt.y > downpt.y )
					rc.bottom = rc.top +t;
				else
					rc.bottom = rc.top -t;
				if(pt.x > downpt.x)
					rc.right =rc.left+t;
				else
					rc.right = rc.left -t;
				break;
			case MK_SHIFT|MK_CONTROL|MK_LBUTTON:
				t = min( abs(pt.x - rc.left),abs(pt.y - rc.top) );
				pt.x = rc.left + t;
				pt.y = rc.top + t;
			case MK_CONTROL|MK_LBUTTON:
				if(pt.y > downpt.y )
				{
					rc.top = downpt.y + downpt.y - pt.y;
					rc.bottom=pt.y;
				}else
				{
					rc.bottom = downpt.y + downpt.y - pt.y;
					rc.top=pt.y;
				}
				if( pt.x > downpt.x  )
				{
					rc.left = downpt.x + downpt.x - pt.x;
					rc.right= pt.x;
				}else
				{
					rc.right= downpt.x + downpt.x - pt.x;
					rc.left = pt.x;
				}break;
		//	case MK_XBUTTON1:
				rc.bottom = pt.y;
				break;
			default:
				return 0;
			}
			if(rc.top > rc.bottom )
			{
				rc.bottom^=rc.top;
				rc.top^=rc.bottom;
				rc.bottom^=rc.top;
			}
			if(rc.left > rc.right )
			{
				rc.left^=rc.right;
				rc.right^=rc.left;
				rc.left^=rc.right;
			}
			DrawFocusRect(hdc,&lastRC);
			CopyRect(&lastRC,&rc);
			DrawFocusRect(hdc,&lastRC);
		}
		SetCursor(hicon);
		break;
	case WM_LBUTTONUP:
		{
			POINT x,y;
			
			ReleaseCapture();
			DrawFocusRect(hdc,&lastRC);
			ReleaseDC(hwnd,hdc);

			x.x = lastRC.left;
			x.y = lastRC.top;
			ClientToScreen(hwnd,&x);
			y.x = lastRC.right;
			y.y = lastRC.bottom;
			ClientToScreen(hwnd,&y);
			DestroyWindow(hwnd);
			rc.top = x.y;
			rc.left= x.x;
			rc.right=y.x;
			rc.bottom=y.y;			
			CutScreen(rc);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			downpt.x= LOWORD(lParam);  // horizontal position of cursor 
			downpt.y = HIWORD(lParam);  // vertical position of cursor 

			SetRect(&lastRC,downpt.x,downpt.y,downpt.x,downpt.y);
			CopyRect(&rc,&lastRC);

			SetCapture(hwnd);
			hdc=GetDC(hwnd);
			DrawFocusRect(hdc,&rc);
		}break;
	case WM_NCLBUTTONDOWN:
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_SHIFT:
		case VK_CONTROL:
		case VK_MENU:
			break;
		default:
			DestroyWindow(hwnd);
		}break;
	case WM_PAINT:
		ValidateRect(hwnd,NULL);		
		break;
	case WM_CREATE:
		if(hicon==0)	LoadUserDefinedIcon();
	default:
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	}
	return 0;
}
__inline void SetUserDefinedIcon(LPTSTR file)
{
	WritePrivateProfileString(TEXT("Others"),TEXT("Cursor"),file,inifile);
}
static __inline BOOL GetCursorFile(HWND hDlg,LPTSTR	file)
{
	OPENFILENAME opfn={sizeof(OPENFILENAME)};
	TCHAR	InitialDir[22];
	opfn.lpstrInitialDir=InitialDir;
	GetWindowsDirectory(InitialDir,sizeof(InitialDir));
	_tcscat(InitialDir,_T("\\Cursors"));
	opfn.hInstance=hInst;
	opfn.hwndOwner =hDlg;
	opfn.lpstrDefExt=TEXT("cur");
	opfn.lpstrFilter=TEXT("光标文件(*.cur;*.ani)\0*.cur;*.ani\0");
	opfn.lpstrTitle=TEXT("妮子，选择光标文件");	
	opfn.nMaxFile=_MAX_PATH;
	opfn.lpstrFile=file;
	opfn.Flags=OFN_NOREADONLYRETURN|OFN_OVERWRITEPROMPT|OFN_READONLY|OFN_EXPLORER;
	return GetOpenFileName(&opfn);
}

LRESULT CALLBACK SettingsDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC	dc;
	switch (message)
	{
	case WM_INITDIALOG:
		LoadHotKeyCFG(hDlg);
		if(LoadUserDefinedIcon())
			SendDlgItemMessage(hDlg,IDC_CHECK1,BM_SETCHECK,BST_CHECKED,0);
		break;
	case WM_PAINT:
		dc=GetDC(hDlg);
		DrawIcon(dc,345,170,hicon);
		ReleaseDC(hDlg,dc);
		//ValidateRect(hDlg,0);
		return 0;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			SaveHotKeyCfg(hDlg);
			UnRegisterHotKey(MainWnd);
			RegisterHotKey(MainWnd);
			SendMessage(MainWnd,WM_FLASHHELP,0,0);
		case IDCANCEL://LOWORD
			EndDialog(hDlg,wParam);
			break;
		case IDC_CHECK1:
			if(BN_CLICKED==HIWORD(wParam))
			{
				if(SendDlgItemMessage(hDlg,IDC_CHECK1,BM_GETCHECK,0,0)==BST_CHECKED)
				{
					LPTSTR	filename = NEWSTR(_MAX_PATH);
					*filename=0;
					if(GetCursorFile(hDlg,filename))
						SetUserDefinedIcon(filename);
					else
					{
						SetUserDefinedIcon(_T(""));
						SendDlgItemMessage(hDlg,IDC_CHECK1,BM_SETCHECK,BST_UNCHECKED,0);
					}
					DELSTR(filename);
				}else SetUserDefinedIcon(_T(""));
				LoadUserDefinedIcon();
			}
			InvalidateRect(hDlg,0,1);
			SendMessage(hDlg,WM_PAINT,0,0);
			SettingsDlg(hDlg,WM_PAINT,0,0);
		}
		break;
	case WM_LBUTTONDOWN:
		//SetWindowLong(hDlg,DWL_MSGRESULT,HTCAPTION);
		SendMessage(hDlg,WM_NCLBUTTONDOWN,HTCAPTION,0);
		break;
	default:
		return FALSE;
	}
    return TRUE;
}
