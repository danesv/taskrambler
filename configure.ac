#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([taskrambler],
        [m4_esyscmd_s([git describe --always --abbrev=0])],
        [Georg Hopp <georg@steffers.org>])
LT_INIT
AM_INIT_AUTOMAKE
#AM_INIT_AUTOMAKE([subdir-objects])
#AM_SILENT_RULES([yes])
AC_COPYRIGHT([Copyright © 2013 Georg Hopp])
AC_REVISION([m4_esyscmd_s([git describe --always])])
AC_CONFIG_SRCDIR([src/taskrambler.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

m4_include([m4/gcov.m4])
AC_TDD_GCOV

m4_include([m4/memopt.m4])
AC_MEM_OPT

PWD=$(/bin/pwd)
AC_SUBST(PWD)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_LIBTOOL
AM_PROG_CC_C_O

# Doxygen
AC_CHECK_PROGS([DOXYGEN], [doxygen])
if test -z "$DOXYGEN";
   then AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])
fi

AM_CONDITIONAL([HAVE_DOXYGEN],
               [test -n "$DOXYGEN"])
AM_COND_IF([HAVE_DOXYGEN],
	   [AC_CONFIG_FILES([docs/Doxyfile])])

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

myetcdir=${sysconfdir}/taskrambler
varlibdir=${localstatedir}/lib/taskrambler
varrundir=${localstatedir}/run/taskrambler
trdatadir=${datadir}/taskrambler
trlibdir=${libdir}/taskrambler
AC_SUBST(myetcdir)
AC_SUBST(varlibdir)
AC_SUBST(varrundir)
AC_SUBST(trdatadir)
AC_SUBST(trlibdir)

AM_CFLAGS="${AM_CFLAGS} ${DRAGONEGG_FLAGS}"
AM_CFLAGS="${AM_CFLAGS} ${MEM_OPT_FLAGS}"
AM_CFLAGS="${AM_CFLAGS} -DPWD=\\\"${PWD}\\\""
AM_CFLAGS="${AM_CFLAGS} -DCONFIGDIR=\\\"${myetcdir}\\\""
AM_CFLAGS="${AM_CFLAGS} ${CFLAGS}"
AC_SUBST(AM_CFLAGS)

AC_CONFIG_FILES([config/taskrambler.conf
		 assets/html/_documentation.html])

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
		 src/application/Makefile
		 src/storage/Makefile
		 src/user/Makefile
		 src/config/Makefile
		 docs/Makefile
		 config/Makefile
		 tests/Makefile])
AC_OUTPUT
