AC_DEFUN([AC_DRAGONEGG],
[
	AC_ARG_ENABLE(dragonegg,
		AC_HELP_STRING([--enable-dragonegg],
		               [use dragonegg llvm plugin]),
		[use_dragonegg=$enableval],
		[use_dragonegg=no])

	AS_IF([test "x$enable_dragonegg" = "xyes"], [
		AC_MSG_NOTICE("using dragonegg gcc plugin")
		DRAGONEGG_FLAGS="-fplugin=/usr/lib64/llvm/dragonegg.so"
		AC_SUBST(DRAGONEGG_FLAGS)
	])
])

# vim: set ft=m4 ts=2 sw=2:
