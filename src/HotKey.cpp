
#include "StdAfx.h"

#include "PrtSc.h"
#include "resource.h"

static DWORD strHex(LPTSTR szHex)
{
	if(*szHex=='0')
	{
		if(*(szHex+1)=='x'||*(szHex+1)=='X')
		{
			szHex+=2;
			goto _strHex;
		}
	}else
	{
		int len=lstrlen(szHex)-1;
		if((*(szHex+len)=='H')||(*(szHex+len)=='h'))
		{
			goto _strHex;
		}else return 0;
	}
_strHex:
	DWORD hex=0,ch;
	while( *szHex!=0 )
	{
		ch=*szHex;
		if(ch>='0'&&ch<='9')
			ch-='0';
		else if(ch>='a'&&ch<='f')
			ch-='a'-10;
		else if(ch>='A'&&ch<='F')
			ch-='A'-10;
		else if(ch=='h'||ch=='H')
			break;
		else return 0;
		hex<<=4;
		hex|=ch;
		++szHex;
	}	
	return hex;
}

static WORD modaltshjift(WORD m)
{
	WORD a,s;
	a=s=m;
	a &= HOTKEYF_ALT;
	s &= HOTKEYF_SHIFT;
	m &=HOTKEYF_CONTROL;
	a >>=2;
	s <<=2;
	return a|s|m;
}

#define DEFVK(VKID)	{ _T(VKID),0 },
#define DEFVKALIAS(VKID,alias)	{ _T(VKID), _T(alias) },
#define	UNDEFVK 	{0,0},
#define	DEFVKA2Z(k)		{0 , _T(k) },

struct {
	LPTSTR	VKstr;
	LPTSTR	alias;
}vkmap[]=
{
	DEFVK		("VK_LBUTTON")//01
	DEFVK		("VK_RBUTTON")//02
	DEFVK		("VK_CANCEL")//03
	DEFVK		("VK_MBUTTON")//04
	UNDEFVK UNDEFVK  UNDEFVK //05-07
	DEFVKALIAS	("VK_BACK","BackSpace")//08
	DEFVK		("VK_TAB")//09
	UNDEFVK UNDEFVK //0a - 0b
	DEFVK		("VK_CLEAR") //0c
	DEFVK		("VK_RETURN") //0d
	UNDEFVK UNDEFVK  //0e - 0f
	DEFVKALIAS	("VK_SHIFT","Shift") //10
	DEFVKALIAS	("VK_CONTROL","Ctrl")//11
	DEFVKALIAS	("VK_MENU","Alt")//12
	DEFVK		("VK_PAUSE"	)//13
	DEFVKALIAS	("VK_CAPITAL","CapsLock") //14
	UNDEFVK UNDEFVK UNDEFVK UNDEFVK UNDEFVK UNDEFVK //15-1A
	DEFVKALIAS	("VK_ESCAPE","Esc")  //1b
	UNDEFVK UNDEFVK UNDEFVK UNDEFVK //1c - 1f
	DEFVK		("VK_SPACE")//20
	DEFVKALIAS	("VK_PRIOR","PageUp")//21
	DEFVKALIAS	("VK_NEXT","PageDown")//22
	DEFVKALIAS	("VK_END","End")//23
	DEFVKALIAS	("VK_HOME","Home")//24
	DEFVK		("VK_LEFT")//25
	DEFVK		("VK_UP")//26
	DEFVK		("VK_RIGHT")//27
	DEFVK		("VK_DOWN")//28
	DEFVK		("VK_SELECT")//29
	UNDEFVK	//2a
	DEFVK		("VK_EXECUTE")//2b
	DEFVKALIAS	("VK_SNAPSHOT","PrintScreen")//2c
	DEFVKALIAS	("VK_INSERT","Ins")//2d
	DEFVKALIAS	("VK_DELETE","Del")//2e
	DEFVKALIAS	("VK_HELP","Help")//2f
	//30 - 39 num key
	DEFVKALIAS	("VK_0","0")
	DEFVKALIAS	("VK_1","1")
	DEFVKALIAS	("VK_2","2")
	DEFVKALIAS	("VK_3","3")
	DEFVKALIAS	("VK_4","4")
	DEFVKALIAS	("VK_5","5")
	DEFVKALIAS	("VK_6","6")
	DEFVKALIAS	("VK_7","7")
	DEFVKALIAS	("VK_8","8")
	DEFVKALIAS	("VK_9","9")
	UNDEFVK UNDEFVK UNDEFVK UNDEFVK UNDEFVK UNDEFVK UNDEFVK //3A-40	
	//41 - 5A  A to Z key
	DEFVKA2Z	("A")
	DEFVKA2Z	("B") 
	DEFVKA2Z	("C") 
	DEFVKA2Z	("D") 
	DEFVKA2Z	("E") 
	DEFVKA2Z	("F") 
	DEFVKA2Z	("G") 
	DEFVKA2Z	("H") 
	DEFVKA2Z	("I") 
	DEFVKA2Z	("J") 
	DEFVKA2Z	("K") 
	DEFVKA2Z	("L") 
	DEFVKA2Z	("M") 
	DEFVKA2Z	("N") 
	DEFVKA2Z	("O") 
	DEFVKA2Z	("P") 
	DEFVKA2Z	("Q") 
	DEFVKA2Z	("R") 
	DEFVKA2Z	("S") 
	DEFVKA2Z	("T") 
	DEFVKA2Z	("U") 
	DEFVKA2Z	("V") 
	DEFVKA2Z	("W") 
	DEFVKA2Z	("X") 
	DEFVKA2Z	("Y") 
	DEFVKA2Z	("Z") 
	DEFVKALIAS	("VK_LWIN","Win")//5B	
	DEFVKALIAS	("VK_RWIN","Win")//5c
	DEFVK		("VK_APPS") //5d
	UNDEFVK UNDEFVK //5E-5F	
	DEFVKALIAS	("VK_NUMPAD0","0") //60
	DEFVKALIAS	("VK_NUMPAD1","1")//61
	DEFVKALIAS	("VK_NUMPAD2","2")//62
	DEFVKALIAS	("VK_NUMPAD3","3")//63
	DEFVKALIAS	("VK_NUMPAD4","4")//64
	DEFVKALIAS	("VK_NUMPAD5","5")//65
	DEFVKALIAS	("VK_NUMPAD6","6")//66
	DEFVKALIAS	("VK_NUMPAD7","7")//67
	DEFVKALIAS	("VK_NUMPAD8","8")//68
	DEFVKALIAS	("VK_NUMPAD9","9")//69
	DEFVK		("VK_MULTIPLY")//6a
	DEFVK		("VK_ADD")//6b
	DEFVK		("VK_SEPARATOR")	//6c
	DEFVK		("VK_SUBTRACT")//6d
	DEFVK		("VK_DECIMAL")//6e
	DEFVK		("VK_DIVIDE")//6f
	DEFVKALIAS	("VK_F1","F1")	//70
	DEFVKALIAS	("VK_F2","F2") 
	DEFVKALIAS	("VK_F3","F3") 
	DEFVKALIAS	("VK_F4","F4") 
	DEFVKALIAS	("VK_F5","F5") 
	DEFVKALIAS	("VK_F6","F6") 
	DEFVKALIAS	("VK_F7","F7") 
	DEFVKALIAS	("VK_F8","F8") 
	DEFVKALIAS	("VK_F9","F9") 
	DEFVKALIAS	("VK_F10","F10") //79
	DEFVKALIAS	("VK_F11","F11") //7A
	DEFVKALIAS	("VK_F12","F12") //7B
	DEFVKALIAS	("VK_F13","F13") 
	DEFVKALIAS	("VK_F14","F14") 
	DEFVKALIAS	("VK_F15","F15") 
	DEFVKALIAS	("VK_F16"	,"F16") 
	DEFVKALIAS	("VK_F17"	,"F17") 
	DEFVKALIAS	("VK_F18"	,"F18") 
	DEFVKALIAS	("VK_F19"	,"F19") 
	DEFVKALIAS	("VK_F20"	,"F20") 
	DEFVKALIAS	("VK_F21"	,"F21") 
	DEFVKALIAS	("VK_F22"	,"F22") 
	DEFVKALIAS	("VK_F23"	,"F23") 
	DEFVKALIAS	("VK_F24","F24") //87
	UNDEFVK UNDEFVK UNDEFVK UNDEFVK UNDEFVK UNDEFVK UNDEFVK UNDEFVK //88-8F	
	DEFVKALIAS	("VK_NUMLOCK", "NumLock") //90
	DEFVKALIAS	("VK_SCROLL","ScrollLock") //91
}; // 0x91
static __inline int	VKFormStr(LPTSTR str)
{
	int	index=0;
	for(;index < 0x91 ; index++ )
	{
		if(vkmap[index].alias && lstrcmpi( str , vkmap[index].alias)==0)
			return index +1;
		if(vkmap[index].VKstr && lstrcmpi( str ,vkmap[index].VKstr )==0)
			return index +1;
	}
	return 0;
}
static ULONG	Hotkey(LPTSTR hotkey)
{
	UINT	fuModifiers=0,uVirtKey=0;
	LPTSTR tok=_tcstok(hotkey,_T("+"));
	while(tok!=NULL)
	{
		if(lstrcmpi(tok,TEXT("Ctrl"))==0)
			fuModifiers|=MOD_CONTROL;
		else if(lstrcmpi(tok,TEXT("Alt"))==0)
			fuModifiers|=MOD_ALT;
		else if(lstrcmpi(tok,TEXT("Shift"))==0)
			fuModifiers|=MOD_SHIFT;
		else if(lstrcmpi(tok,TEXT("Win"))==0)
			fuModifiers|=MOD_WIN;
		else if( ( uVirtKey = VKFormStr(tok))==0)
		{
			*(tok+1)='0';
			*(tok+2)='x';
			uVirtKey=::strHex(tok+1);
			if(uVirtKey==0x5B||uVirtKey==0x5C)fuModifiers|=MOD_WIN;
		}
		tok=_tcstok(NULL,_T("+"));
	}
	return MAKELONG(fuModifiers,uVirtKey);
}

static LPTSTR VK2STR(WORD vk)
{
	LPTSTR astr=(LPTSTR)LocalAlloc(LPTR,256);
	UINT Mod,VirtKey;
	Mod=HIBYTE(vk);
	VirtKey=LOBYTE(vk);
	if(Mod==(Mod|MOD_CONTROL))
		lstrcat(astr,TEXT("Ctrl+"));
	if(Mod==(Mod|MOD_SHIFT))
		lstrcat(astr,TEXT("Alt+"));
	if(Mod==(Mod|MOD_ALT))
		lstrcat(astr,TEXT("Shift+"));
	if(vkmap[VirtKey-1].alias)
		lstrcat(astr,vkmap[VirtKey-1].alias);
	else
		lstrcat(astr,vkmap[VirtKey-1].VKstr);
	return astr;
}
static __inline LPTSTR GetHotkeyCfgStr(LPCTSTR lpKeyName,LPCTSTR lpDefault,LPTSTR lpReturnedString)
{
	TCHAR	section[]=_T("HotKeys");
	GetPrivateProfileString(section,lpKeyName,lpDefault,lpReturnedString,256,inifile);
	return lpReturnedString;
}

void __fastcall	LoadHotKeyCFG(HWND	hWnd)
{
	DWORD	vk;
	WORD	vkl,vkh;
	LPTSTR	str=new TCHAR[256];

	vk=Hotkey(GetHotkeyCfgStr(TEXT("PrintScreen"),TEXT("Ctrl+F12"),str));
	vkl=modaltshjift(LOWORD(vk)); //mod
	vkh=HIWORD(vk); //vk
	SendDlgItemMessage(hWnd,IDC_HOTKEY1,HKM_SETHOTKEY,MAKEWORD(vkh,vkl),0);
	
	vk=Hotkey(GetHotkeyCfgStr(TEXT("PrintWindow"),TEXT("Ctrl+Shift+F12"),str));
	vkl=modaltshjift(LOWORD(vk)); //mod
	vkh=HIWORD(vk); //vk
	SendDlgItemMessage(hWnd,IDC_HOTKEY2,HKM_SETHOTKEY,MAKEWORD(vkh,vkl),0);
	
	GetHotkeyCfgStr(TEXT("CutScreen"),TEXT("Ctrl+Alt+Z"),str);
	vk=Hotkey(str);
	vkl=modaltshjift(LOWORD(vk)); //mod
	vkh=HIWORD(vk); //vk
	SendDlgItemMessage(hWnd,IDC_HOTKEY3,HKM_SETHOTKEY,MAKEWORD(vkh,vkl),0);
	
	GetHotkeyCfgStr(TEXT("Exit"),TEXT("Ctrl+F11"),str);
	vk=Hotkey(str);
	vkl=modaltshjift(LOWORD(vk)); //mod
	vkh=HIWORD(vk); //vk
	SendDlgItemMessage(hWnd,IDC_HOTKEY4,HKM_SETHOTKEY,MAKEWORD(vkh,vkl),0);
	delete[]str;
}
BOOL __fastcall	SaveHotKeyCfg(HWND hwnd)
{
	TCHAR	section[]=_T("HotKeys");
	LPTSTR	vkstr;
	WORD	vk[4];
	vk[0]=(SHORT)SendDlgItemMessage(hwnd,IDC_HOTKEY1,HKM_GETHOTKEY,0,0);
	vk[1]=(SHORT)SendDlgItemMessage(hwnd,IDC_HOTKEY2,HKM_GETHOTKEY,0,0);
	vk[2]=(SHORT)SendDlgItemMessage(hwnd,IDC_HOTKEY3,HKM_GETHOTKEY,0,0);
	vk[3]=(SHORT)SendDlgItemMessage(hwnd,IDC_HOTKEY4,HKM_GETHOTKEY,0,0);
	vkstr=VK2STR(vk[0]);

	if(vk[0]!=0)WritePrivateProfileString(section,TEXT("PrintScreen"),vkstr,inifile);
	LocalFree(vkstr);

	vkstr=VK2STR(vk[1]);
	if(vk[1]!=0)WritePrivateProfileString(section,TEXT("PrintWindow"),vkstr,inifile);
	LocalFree(vkstr);

	vkstr=VK2STR(vk[2]);
	if(vk[2]!=0)WritePrivateProfileString(section,TEXT("CutScreen"),vkstr,inifile);
	LocalFree(vkstr);

	vkstr=VK2STR(vk[3]);
	if(vk[3]!=0)WritePrivateProfileString(section,TEXT("Exit"),vkstr,inifile);
	LocalFree(vkstr);
	return 0;
}
#ifndef HOTKYE_ID_BASE
void	InitHotKeyID()
{
	HOTKYE_ID_CUTSCREEN=GlobalAddAtom("PrintScreen");
	HOTKYE_ID_CUTACTIVE=GlobalAddAtom("PrintWindow");
	HOTKYE_ID_CUTREGION=GlobalAddAtom("CutScreen");
	HOTKYE_ID_EXITPROGM=GlobalAddAtom("Exit");
}
#endif
BOOL __fastcall	RegisterHotKey(HWND hwnd)
{
	ULONG	vk;
	BOOL	bOK;
	LPTSTR	retStr=new TCHAR[256];
	GetHotkeyCfgStr(TEXT("PrintScreen"),TEXT("Ctrl+F12"),retStr);
	vk=Hotkey(retStr);
	bOK = RegisterHotKey(hwnd,HOTKYE_ID_CUTSCREEN,LOWORD(vk),HIWORD(vk));
	
	GetHotkeyCfgStr(TEXT("PrintWindow"),TEXT("Ctrl+Shift+F12"),retStr);
	vk=Hotkey(retStr);
	bOK = bOK && RegisterHotKey(hwnd,HOTKYE_ID_CUTACTIVE,LOWORD(vk),HIWORD(vk));

	GetHotkeyCfgStr(TEXT("CutScreen"),TEXT("Ctrl+Alt+Z"),retStr);
	vk=Hotkey(retStr);
	bOK = bOK && RegisterHotKey(hwnd,HOTKYE_ID_CUTREGION,LOWORD(vk),HIWORD(vk));

	GetHotkeyCfgStr(TEXT("Exit"),TEXT("Ctrl+Q"),retStr);
	vk=Hotkey(retStr);
	bOK = bOK && RegisterHotKey(hwnd,HOTKYE_ID_EXITPROGM,LOWORD(vk),HIWORD(vk));
	delete[] retStr;
	return bOK;
}
BOOL __fastcall	UnRegisterHotKey(HWND hwnd)
{
	register BOOL	ret;
	ret = UnregisterHotKey(hwnd,HOTKYE_ID_CUTSCREEN) ;
	ret &=UnregisterHotKey(hwnd,HOTKYE_ID_CUTACTIVE) ;
	ret &=UnregisterHotKey(hwnd,HOTKYE_ID_CUTREGION) ;
	ret &=UnregisterHotKey(hwnd,HOTKYE_ID_EXITPROGM) ;
	return ret;
}