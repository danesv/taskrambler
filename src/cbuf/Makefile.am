ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

CB = cbuf.c read.c \
     get_line.c set_data.c get_data.c \
     addr_index.c get_free.c get_read.c get_write.c \
     inc_read.c inc_write.c is_empty.c memchr.c \
     skip_non_alpha.c is_locked.c lock.c release.c \
     empty.c

noinst_LIBRARIES = libcbuf.a

libcbuf_a_SOURCES = $(CB)
libcbuf_a_CFLAGS  = $(CFLAGS) -Wall -I ../../include/
