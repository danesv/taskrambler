ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS += -I../../include/

noinst_LIBRARIES = libuser.a

libuser_a_SOURCES = user.c load.c save.c
libuser_a_CFLAGS  = $(AM_CFLAGS)
