# Microsoft Developer Studio Project File - Name="PrtSc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PrtSc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PrtSc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PrtSc.mak" CFG="PrtSc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PrtSc - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PrtSc - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PrtSc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "安装发行"
# PROP Intermediate_Dir "obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /Gr /MD /W3 /Ox /Ot /Oa /Ow /Og /Oi /Op /Ob2 /I ".." /I "..\..\jpeg-8b" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /Fr /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# SUBTRACT MTL /Oicf
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 Vfw32.lib glu32.lib opengl32.lib kernel32.lib user32.lib gdi32.lib comctl32.lib comdlg32.lib shell32.lib vfw32.lib libpng.lib libjpeg.lib zlib.lib /subsystem:windows /pdb:none /machine:I386 /out:"../安装发行/PrtSc.exe" /libpath:"F:\lpng143\projects\visualc6\Win32_LIB_Release" /libpath:"f:\jpeg-8b"
# SUBTRACT LINK32 /nologo /map /debug /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=cd 安装发行	bind PrtSc.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "PrtSc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /vmg /GR /GX /ZI /Od /I ".." /I "..\..\jpeg-8b" /D "_WIN32" /D "_DEBUG" /D "_WINDOWS" /YX"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d MANIFEST=24
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Vfw32.lib glu32.lib opengl32.lib msvcrt.lib kernel32.lib user32.lib gdi32.lib comctl32.lib comdlg32.lib advapi32.lib shell32.lib libpngd.lib libjpeg.lib zlibd.lib libgtk-win32-2.0.dll.a /nologo /subsystem:windows /pdb:none /debug /machine:I386 /nodefaultlib /libpath:"F:\lpng143\projects\visualc6\Win32_LIB_Debug" /libpath:"..\..\jpeg-8b"

!ENDIF 

# Begin Target

# Name "PrtSc - Win32 Release"
# Name "PrtSc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\BitMap.cpp
# End Source File
# Begin Source File

SOURCE=..\src\HotKey.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Junk.cpp
# End Source File
# Begin Source File

SOURCE=..\src\main.cpp
# End Source File
# Begin Source File

SOURCE=..\src\MainWndProc.cpp
# End Source File
# Begin Source File

SOURCE=..\src\PrtSc.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ShowPic.cpp
# End Source File
# Begin Source File

SOURCE=..\StdAfx.cpp

!IF  "$(CFG)" == "PrtSc - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "PrtSc - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\SysTray.cpp
# End Source File
# Begin Source File

SOURCE=..\src\WindowProc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\resource.h
# End Source File
# Begin Source File

SOURCE=..\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\res\cursor.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor.cur
# End Source File
# Begin Source File

SOURCE=..\res\notepad.ico
# End Source File
# Begin Source File

SOURCE=.\res\notepad.ico
# End Source File
# Begin Source File

SOURCE=..\res\PrtSc.ico
# End Source File
# Begin Source File

SOURCE=.\res\PrtSc.ico
# End Source File
# Begin Source File

SOURCE=..\PrtSc.rc
# End Source File
# Begin Source File

SOURCE=..\res\tray.ico
# End Source File
# Begin Source File

SOURCE=.\res\tray.ico
# End Source File
# Begin Source File

SOURCE=..\res\tray2.ico
# End Source File
# Begin Source File

SOURCE=.\res\tray2.ico
# End Source File
# Begin Source File

SOURCE=.\res\wyn.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=..\res\manifest.xml
# End Source File
# End Target
# End Project
