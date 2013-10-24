ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

UUID = uuid.c version1.c version3.c version5.c _format3or5.c \
       parse.c unparse.c compare.c

AM_CFLAGS += -I../../include/

noinst_LTLIBRARIES = libuuid.la

libuuid_la_SOURCES = $(UUID)
libuuid_la_CFLAGS  = $(AM_CFLAGS)
