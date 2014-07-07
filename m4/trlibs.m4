AC_DEFUN([AC_TRLIBS_ROOT],
[
	AC_ARG_VAR([TRLIBS_ROOT],
	[
		The installation root of the taskrambler libraries
	])

	AS_IF([test "$TRLIBS_ROOT"],
	[
		AC_MSG_NOTICE("Using ${TRLIBS_ROOT} as installation root for libtrbase")
		CFLAGS="${CFLAGS} -I${TRLIBS_ROOT}/include"
		LDFLAGS="${LDFLAGS} -L${TRLIBS_ROOT}/lib -lgdbm -luuid"
	])
])

AC_DEFUN([AC_NEED_TRBASE],
[
	AC_CHECK_LIB([trbase], [TR_classNew], [],
	[
		AC_MSG_ERROR([Needed trbase library not found may try to set TRLIBS_ROOT])
	])
])

# vim: set ft=m4 ts=2 sw=2:
