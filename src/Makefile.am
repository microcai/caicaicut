RC=@RC@

BUILD_SOURCE = PrtSc.res

AM_CXXFLAGS = ${GTK2_CFLAGS} -I$(top_srcdir) -D_MAX_PATH=255

bin_PROGRAMS = prtsc

prtsc_SOURCES = BitMap.cpp \
		HotKey.cpp \
		ShowPic.cpp\
		WindowProc.cpp \
		PrtSc.cpp   SysTray.cpp \
		main.cpp \
		Junk.cpp \
		MainWndProc.cpp \
		PrtSc.h \
		PrtSc.rc
		
prtsc_LDADD = $(builddir)/PrtSc.res

PrtSc.res: PrtSc.rc resource.h
			$(RC) -I$(top_srcdir) $< 
