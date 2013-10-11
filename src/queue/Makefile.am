ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS += -I../../include/

noinst_LTLIBRARIES = libqueue.la

libqueue_la_SOURCES = queue.c get.c put.c
libqueue_la_CFLAGS  = $(AM_CFLAGS)
