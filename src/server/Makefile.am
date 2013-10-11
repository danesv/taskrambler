ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

SERVER = server.c run.c close_conn.c poll.c \
	 handle_accept.c read.c write.c

AM_CFLAGS += -I../../include/

noinst_LTLIBRARIES = libserver.la

libserver_la_SOURCES = $(SERVER)
libserver_la_CFLAGS  = $(AM_CFLAGS)
