AC_DEFUN([AC_MEM_OPT],
[
	AC_ARG_ENABLE(mem_opt,
		AC_HELP_STRING([--enable-mem-opt],
		               [enable best bit tree based memory optimizations]),
		[use_mem_opt=$enableval],
		[use_mem_opt=no])

	if test "x$use_mem_opt" = "xyes"; then
	MEM_OPT_FLAGS="-DMEM_OPT"
	AC_SUBST(MEM_OPT_FLAGS)
	fi
])

# vim: set ft=m4 ts=2 sw=2:
