ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

HASH = hash.c add.c get.c get_first.c delete.c each.c value.c \
       cleanup.c interface/hashable.c

AM_CFLAGS += -I../../include/

noinst_LTLIBRARIES = libhash.la

libhash_la_SOURCES = $(HASH)
libhash_la_CFLAGS  = $(AM_CFLAGS)
