ACLOCAL_AMFLAGS = -I m4

SERVER = server.c run.c close_conn.c poll.c \
	 handle_accept.c read.c write.c

noinst_LIBRARIES = libserver.a

libserver_a_SOURCES = $(SERVER)
libserver_a_CFLAGS  = $(CFLAGS) -Wall -I ../../include/
