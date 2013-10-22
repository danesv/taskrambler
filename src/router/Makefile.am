ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

ROUTER = router.c route.c

AM_CFLAGS += -I../../include/

noinst_LTLIBRARIES = librouter.la

librouter_la_SOURCES = $(ROUTER)
librouter_la_CFLAGS  = $(AM_CFLAGS)
