ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

CB = cbuf.c read.c \
     get_line.c set_data.c get_data.c \
     addr_index.c get_free.c get_read.c get_write.c \
     inc_read.c inc_write.c is_empty.c memchr.c \
     skip_non_alpha.c is_locked.c lock.c release.c \
     empty.c

AM_CFLAGS += -I../../include/

noinst_LTLIBRARIES = libcbuf.la

libcbuf_la_SOURCES = $(CB)
libcbuf_la_CFLAGS  = $(AM_CFLAGS)
