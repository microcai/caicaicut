// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _WIN32_WINNT 0x500
#define	_WIN32_IE 0x601

// Windows Header Files:
#include <windows.h>

#include <tchar.h>
#include <shellapi.h>

#include <stdlib.h>
#include <commctrl.h>
#include <commdlg.h>

#ifndef NIN_BALLOONTIMEOUT
#define NIN_BALLOONTIMEOUT (WM_USER + 4)
#endif

#ifndef NIIF_ERROR
#define NIIF_ERROR  0x00000003
#endif

#ifndef NIIF_INFO
#define  NIIF_INFO  0x1
#endif

#ifndef NIF_INFO
#define NIF_INFO		  0x00000010
#endif

typedef struct {
		DWORD cbSize;
		HWND hWnd;
		UINT uID;
		UINT uFlags;
		UINT uCallbackMessage;
		HICON hIcon;
		TCHAR szTip[128];
		DWORD dwState;
		DWORD dwStateMask;
		TCHAR szInfo[256];
		union { UINT uTimeout;UINT uVersion; }; 
		TCHAR szInfoTitle[64];
		DWORD dwInfoFlags;
		GUID guidItem;
} IE6_NOTIFYICONDATA;

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
