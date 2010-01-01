#include "StdAfx.h"
#include <ole.h>
#include <ole2.h>

#include <richedit.h>
#include <richole.h>
#include "resource.h"
#include "PrtSc.h"
DWORD WINAPI	ViewJunk(void*)
{
	HWND m;
	MSG msg;
	SetThreadLocale(locale_ID);
	{
		AutoDeleteString lpWindowName;

		lpWindowName.LoadString(DllhInst,IDS_JUNKCAPTION);
		m=CreateWindowEx(0,_T("Junk"),lpWindowName,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,0,0,hInst,0);
	}
	ShowWindow(m,SW_SHOW);
	UpdateWindow(m);
	__try{

		while(GetMessage(&msg,0,0,0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}__except(EXCEPTION_EXECUTE_HANDLER)
	{
	}
	return msg.lParam;
}

LRESULT	CALLBACK	JunkWndProc(HWND hwnd,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	HDC			hdc,hmdc;
	HWND		child;
	PAINTSTRUCT	ps;
	HBITMAP		hbmp;
	RECT		rc;
	LPTSTR		hello_jenee;
	switch(uMsg)
	{
	case WM_SIZE:
		child=(HWND)GetWindowLong(hwnd,4);
		MoveWindow(child,5,135,LOWORD(lParam)-10,HIWORD(lParam)-140,1);
		break;
	case WM_PAINT:
		hbmp=(HBITMAP)GetWindowLong(hwnd,0);
		hdc=BeginPaint(hwnd,&ps);
		hmdc=CreateCompatibleDC(hdc);
		DeleteObject(SelectObject(hmdc,hbmp));
		BitBlt(hdc,7,10,105,127,hmdc,0,0,SRCCOPY);
		DeleteDC(hmdc);

		MoveToEx(hdc,75,105,0);
		LineTo(hdc,130,115);
		MoveToEx(hdc,75,105,0);
		LineTo(hdc,133,125);
		rc.left=131;
		rc.right = 131+125;
		rc.top=20;
		rc.bottom = 130;
		RoundRect(hdc,rc.left,rc.top,rc.right,rc.bottom,25,25);
		hello_jenee = NEWSTR(1024);
		LoadString(DllhInst,IDS_JENEE,hello_jenee,1024);
		ExtTextOut(hdc,140,25,ETO_CLIPPED,&rc,hello_jenee,lstrlen(hello_jenee),0);
		DELSTR(hello_jenee);
		EndPaint(hwnd,&ps);
		break;
	case	WM_DESTROY:
		DeleteObject((HGDIOBJ)GetWindowLong(hwnd,0));
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		hbmp=LoadBitmap(DllhInst,_T("wyn"));
		SetWindowLong(hwnd,0,(long)hbmp);
		hello_jenee = (LPTSTR)(LPSTR)LockResource(LoadResource(DllhInst,FindResource(DllhInst,(LPTSTR)2,_T("TEXT"))));
		child = CreateWindowExA(/*WS_EX_TRANSPARENT*/0,"EDIT",(LPSTR)hello_jenee,
			WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_READONLY,
			20,200,500,500,hwnd,0,hInst,0);
		SetWindowLong(hwnd,4,(long)child);
		
	default:
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	}
	return 0;
}
