ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS += -I../../include/

noinst_LIBRARIES = libsocket.a

libsocket_a_SOURCES = socket.c accept.c connect.c listen.c nonblock.c
libsocket_a_CFLAGS  = $(AM_CFLAGS)
