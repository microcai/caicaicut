
#if !defined(AFX_PRTSC_H__INCLUDED_)
#define AFX_PRTSC_H__INCLUDED_

#define WM_TIPS			(WM_USER+50)
#define	WM_SETFLAG		(WM_USER+100)
#define	WM_FLASHHELP	(WM_APP+1)
#define	WM_EXPLORER_RESTART		0xC0C6

#define	HOTKYE_ID_BASE			0xAFFF
#define	HOTKYE_ID_CUTSCREEN		(0xCFFF+50)
#define	HOTKYE_ID_CUTACTIVE		(0xCFFF+51)
#define	HOTKYE_ID_CUTREGION		(0xCFFF+52)
#define	HOTKYE_ID_EXITPROGM		(0xCFFF+53)

//该标志告诉我要窗口撤销时关闭位图
#define	SHP_DELBMPONCLOSE		0x00000001
//自动伸展至可见窗口区域
#define	SHP_ZOOMAUTOSTRECH		0x00000002
//保存纵横比
#define	SHP_ZOOMKEEPRATIO		0x00000004

typedef BOOL (*InitFunc)(HINSTANCE);//FARPROC
//struct define
typedef struct{
	DWORD	flag;
	HBITMAP	hbmp;
	DWORD	bmRC;
	float	scaling;
	POINTS	central;
}ShowPicClass;

enum ImageFileType{
	FT_BMP,
	FT_PNG,
	FT_JPEG
};


//WindowProcs

LRESULT CALLBACK NoteWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK CutWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK MainWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
LRESULT	WINAPI	 ShowPicClassWndProc(HWND hWnd,UINT	message,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK SettingsDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT	CALLBACK JunkWndProc(HWND hwnd,UINT uMsg,WPARAM wParam ,LPARAM lParam);

//HotKey
BOOL __fastcall	RegisterHotKey(HWND);
BOOL __fastcall	UnRegisterHotKey(HWND);
void __fastcall	LoadHotKeyCFG(HWND);
BOOL __fastcall	SaveHotKeyCfg(HWND);

//PrintFunctions
void	CutRegion();
void	CutWindow(HWND hwnd);
void	CutScreen(RECT rc);
BOOL	SaveBmpToFile(HBITMAP,LPTSTR,ImageFileType);

//SysTray Funcs
void	StaysInsysTray();
void	RemoveTray();
void __fastcall	FlashTray(LPTSTR info,LPTSTR infoTitle = NULL,DWORD =0);
void	FlashHelpTips();
void	OnRemoveTray();
void 	FlashOnlyTips();
//void	TrayNewIcon(HICON	hicon);
//Other Function

DWORD WINAPI	ViewDevNotes(LPVOID);
DWORD WINAPI	ViewJunk(void*);
BOOL  WINAPI	LondLangDll();




void	CreateViewPicWnd(HBITMAP bmp);

DWORD	WINAPI	StoreBMP(HBITMAP	hbmp);

#define	NEWSTR(numberOfChar)	(LPTSTR)LocalAlloc(LPTR,numberOfChar*sizeof(TCHAR))

#define	DELSTR(lpstr)	LocalFree(lpstr)

//Helper Class
class AutoDeleteString
{
	LPTSTR	m_str;
	int		m_len;
public:
	operator LPTSTR(){return m_str;};
	operator int(){return m_len;};
	AutoDeleteString(int len=512);
	~AutoDeleteString();
	void LoadString(HINSTANCE,UINT);
};

//Global var
extern	TCHAR	inifile[_MAX_PATH];

extern	HINSTANCE	hInst;
extern	HINSTANCE	DllhInst;
extern	HWND	MainWnd;
extern	HICON	hicon;
extern	LCID	locale_ID;
#endif // !defined(AFX_PRTSC_H__INCLUDED_)
