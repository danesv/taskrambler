ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS += -I../../include/

noinst_LTLIBRARIES = libsocket.la

libsocket_la_SOURCES = socket.c accept.c connect.c listen.c nonblock.c
libsocket_la_CFLAGS  = $(AM_CFLAGS)
