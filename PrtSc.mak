# Microsoft Developer Studio Generated NMAKE File, Based on PrtSc.dsp
!IF $(CFG)" == "
CFG=PrtSc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to PrtSc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "PrtSc - Win32 Release" && "$(CFG)" != "PrtSc - Win32 Debug"
!MESSAGE 指定的配置 "$(CFG)" 无效.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF $(OS)" == "Windows_NT
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PrtSc - Win32 Release"

OUTDIR=.\安装发行
INTDIR=.\obj
# 开始自定义宏
OutDir=.\安装发行
# 结束自定义宏

ALL : "$(OUTDIR)\PrtSc.exe"


CLEAN :
	-@erase "$(INTDIR)\BitMap.obj"
	-@erase "$(INTDIR)\HotKey.obj"
	-@erase "$(INTDIR)\Junk.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\MainWndProc.obj"
	-@erase "$(INTDIR)\PrtSc.obj"
	-@erase "$(INTDIR)\PrtSc.res"
	-@erase "$(INTDIR)\ShowPic.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SysTray.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WindowProc.obj"
	-@erase "$(OUTDIR)\PrtSc.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /G6 /Gr /MD /W3 /Ox /Ot /Oa /Ow /Og /Oi /Op /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FAs /Fa"ASM/" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\PrtSc.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PrtSc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib comctl32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib vfw32.lib /subsystem:windows /pdb:none /machine:I386 /out:"$(OUTDIR)\PrtSc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\BitMap.obj" \
	"$(INTDIR)\HotKey.obj" \
	"$(INTDIR)\Junk.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\MainWndProc.obj" \
	"$(INTDIR)\PrtSc.obj" \
	"$(INTDIR)\ShowPic.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SysTray.obj" \
	"$(INTDIR)\WindowProc.obj" \
	"$(INTDIR)\PrtSc.res"

"$(OUTDIR)\PrtSc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# 开始自定义宏
OutDir=.\安装发行
# 结束自定义宏

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\PrtSc.exe"
   cd 安装发行
	bind PrtSc.exe
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "PrtSc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# 开始自定义宏
OutDir=.\Debug
# 结束自定义宏

ALL : "$(OUTDIR)\PrtSc.exe" "$(OUTDIR)\PrtSc.pch" "$(OUTDIR)\PrtSc.bsc"


CLEAN :
	-@erase "$(INTDIR)\BitMap.obj"
	-@erase "$(INTDIR)\BitMap.sbr"
	-@erase "$(INTDIR)\HotKey.obj"
	-@erase "$(INTDIR)\HotKey.sbr"
	-@erase "$(INTDIR)\Junk.obj"
	-@erase "$(INTDIR)\Junk.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\MainWndProc.obj"
	-@erase "$(INTDIR)\MainWndProc.sbr"
	-@erase "$(INTDIR)\PrtSc.obj"
	-@erase "$(INTDIR)\PrtSc.pch"
	-@erase "$(INTDIR)\PrtSc.res"
	-@erase "$(INTDIR)\PrtSc.sbr"
	-@erase "$(INTDIR)\ShowPic.obj"
	-@erase "$(INTDIR)\ShowPic.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\SysTray.obj"
	-@erase "$(INTDIR)\SysTray.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WindowProc.obj"
	-@erase "$(INTDIR)\WindowProc.sbr"
	-@erase "$(OUTDIR)\PrtSc.bsc"
	-@erase "$(OUTDIR)\PrtSc.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G6 /Gr /MDd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\PrtSc.res" /d "_DEBUG" /d MANIFEST=24 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PrtSc.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BitMap.sbr" \
	"$(INTDIR)\HotKey.sbr" \
	"$(INTDIR)\Junk.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\MainWndProc.sbr" \
	"$(INTDIR)\PrtSc.sbr" \
	"$(INTDIR)\ShowPic.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\SysTray.sbr" \
	"$(INTDIR)\WindowProc.sbr"

"$(OUTDIR)\PrtSc.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib comctl32.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:windows /pdb:none /debug /machine:I386 /out:"$(OUTDIR)\PrtSc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\BitMap.obj" \
	"$(INTDIR)\HotKey.obj" \
	"$(INTDIR)\Junk.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\MainWndProc.obj" \
	"$(INTDIR)\PrtSc.obj" \
	"$(INTDIR)\ShowPic.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SysTray.obj" \
	"$(INTDIR)\WindowProc.obj" \
	"$(INTDIR)\PrtSc.res"

"$(OUTDIR)\PrtSc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("PrtSc.dep")
!INCLUDE "PrtSc.dep"
!ELSE 
!MESSAGE Warning: cannot find "PrtSc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "PrtSc - Win32 Release" || "$(CFG)" == "PrtSc - Win32 Debug"
SOURCE=.\BitMap.cpp

!IF  "$(CFG)" == "PrtSc - Win32 Release"


"$(INTDIR)\BitMap.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrtSc - Win32 Debug"


"$(INTDIR)\BitMap.obj"	"$(INTDIR)\BitMap.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\HotKey.cpp

!IF  "$(CFG)" == "PrtSc - Win32 Release"


"$(INTDIR)\HotKey.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrtSc - Win32 Debug"


"$(INTDIR)\HotKey.obj"	"$(INTDIR)\HotKey.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Junk.cpp

!IF  "$(CFG)" == "PrtSc - Win32 Release"


"$(INTDIR)\Junk.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrtSc - Win32 Debug"


"$(INTDIR)\Junk.obj"	"$(INTDIR)\Junk.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\main.cpp

!IF  "$(CFG)" == "PrtSc - Win32 Release"


"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrtSc - Win32 Debug"


"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MainWndProc.cpp

!IF  "$(CFG)" == "PrtSc - Win32 Release"


"$(INTDIR)\MainWndProc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrtSc - Win32 Debug"


"$(INTDIR)\MainWndProc.obj"	"$(INTDIR)\MainWndProc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PrtSc.cpp

!IF  "$(CFG)" == "PrtSc - Win32 Release"


"$(INTDIR)\PrtSc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrtSc - Win32 Debug"


"$(INTDIR)\PrtSc.obj"	"$(INTDIR)\PrtSc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ShowPic.cpp

!IF  "$(CFG)" == "PrtSc - Win32 Release"

CPP_SWITCHES=/nologo /G6 /Gr /MD /W3 /Ox /Ot /Oa /Ow /Og /Oi /Op /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FAs /Fa"ASM/" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ShowPic.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "PrtSc - Win32 Debug"

CPP_SWITCHES=/nologo /G6 /Gr /MDd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FAs /Fa"ASM/" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ShowPic.obj"	"$(INTDIR)\ShowPic.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "PrtSc - Win32 Release"

CPP_SWITCHES=/nologo /G6 /Gr /MD /W3 /Ox /Ot /Oa /Ow /Og /Oi /Op /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FAs /Fa"ASM/" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "PrtSc - Win32 Debug"

CPP_SWITCHES=/nologo /G6 /Gr /MDd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\PrtSc.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\PrtSc.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\SysTray.cpp

!IF  "$(CFG)" == "PrtSc - Win32 Release"


"$(INTDIR)\SysTray.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrtSc - Win32 Debug"


"$(INTDIR)\SysTray.obj"	"$(INTDIR)\SysTray.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\WindowProc.cpp

!IF  "$(CFG)" == "PrtSc - Win32 Release"


"$(INTDIR)\WindowProc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrtSc - Win32 Debug"


"$(INTDIR)\WindowProc.obj"	"$(INTDIR)\WindowProc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PrtSc.rc

"$(INTDIR)\PrtSc.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

