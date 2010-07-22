#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([src/resource.h])
AM_INIT_AUTOMAKE([-Wall])
# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
PKG_PROG_PKG_CONFIG
# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([pow])

AC_CHECK_LIB(gdi32,CreateCompatibleDC,[],[AC_MSG_ERROR(need wine or win32 sdk)])

AC_CHECK_LIB(user32,[main],[],[AC_MSG_ERROR(need wine or win32 sdk)])

AC_CHECK_LIB(shell32,[main],[],[AC_MSG_ERROR(need wine or win32 sdk)])

AC_CHECK_LIB(comdlg32,[main],[],[AC_MSG_ERROR(need wine or win32 sdk)])

AC_CHECK_LIB(comctl32,[InitCommonControlsEx],[],[AC_MSG_ERROR(need wine or win32 sdk)])

AC_CHECK_LIB(msvfw32,[DrawDibDraw],[],[AC_MSG_ERROR(need wine or win32 sdk)])

AC_CHECK_LIB(png14,[main],[],[AC_MSG_ERROR(need libpng)])

PKG_CHECK_MODULES([GTK2],[gtk+-2.0])

AC_CONFIG_SUBDIRS(["1033" "2052"])

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])

AC_OUTPUT
