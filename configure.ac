#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([taskrambler], [0.0.1], [Georg Hopp <georg@steffers.org>])
LT_INIT
AM_INIT_AUTOMAKE
#AM_INIT_AUTOMAKE([subdir-objects])
AM_SILENT_RULES([yes])
AC_COPYRIGHT([Copyright © 2012 Georg Hopp])
AC_REVISION([$Revision: 0.02 $])
AC_CONFIG_SRCDIR([src/taskrambler.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

m4_include([m4/gcov.m4])
AC_TDD_GCOV
AC_SUBST(COVERAGE_CFLAGS)
AC_SUBST(COVERAGE_CXXFLAGS)
AC_SUBST(COVERAGE_LDFLAGS)

PWD=$(/bin/pwd)
AC_SUBST(PWD)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_LIBTOOL
AM_PROG_CC_C_O

# # Checks for libraries.
# AC_CHECK_LIB([json], [json_object_new_object], [],
# 	     [AC_MSG_ERROR([json-c library not found], [1])])
AC_CHECK_LIB([ssl], [SSL_library_init], [],
	     [AC_MSG_ERROR([openssl not found], [1])])

# Checks for header files.
AC_CHECK_HEADERS([stdarg.h string.h stdlib.h stdio.h unistd.h syslog.h sys/types.h json/json.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
#AC_FUNC_FORK
#AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile
		 src/Makefile
		 src/asset/Makefile
		 src/auth/Makefile
		 src/cbuf/Makefile
		 src/class/Makefile
		 src/hash/Makefile
		 src/queue/Makefile
		 src/http/Makefile
		 src/logger/Makefile
		 src/server/Makefile
		 src/session/Makefile
		 src/socket/Makefile
		 src/stream/Makefile
		 src/tree/Makefile
		 tests/Makefile])
AC_OUTPUT
