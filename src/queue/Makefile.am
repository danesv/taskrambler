ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS += -I../../include/

noinst_LIBRARIES = libqueue.a

libqueue_a_SOURCES = queue.c get.c put.c
libqueue_a_CFLAGS  = $(AM_CFLAGS)
