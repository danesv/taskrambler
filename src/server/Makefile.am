ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

SERVER = server.c run.c close_conn.c poll.c \
	 handle_accept.c read.c write.c

AM_CFLAGS += -I../../include/

noinst_LIBRARIES = libserver.a

libserver_a_SOURCES = $(SERVER)
libserver_a_CFLAGS  = $(AM_CFLAGS)
