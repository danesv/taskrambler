AC_DEFUN([AC_MEM_OPT],
[
	AC_ARG_ENABLE(mem_opt,
		AC_HELP_STRING([--disable-mem-opt],
		               [disable best bit tree based memory optimizations]))

	AS_IF([test "x$enable_mem_opt" != "xno"], [
		AC_MSG_NOTICE("Using best fit tree based memory optimizations")
		MEM_OPT_FLAGS="-DMEM_OPT"
		AC_SUBST(MEM_OPT_FLAGS)
	])
])

# vim: set ft=m4 ts=2 sw=2:
