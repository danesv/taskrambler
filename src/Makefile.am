ACLOCAL_AMFLAGS = -I m4

IFACE   = interface/stream_reader.c interface/logger.c \
          interface/stream_writer.c interface/http_intro.c \
          interface/subject.c interface/observer.c
SOCKET  = socket.c socket/accept.c socket/connect.c socket/listen.c
STREAM  = stream.c stream/read.c stream/write.c
SERVER  = server.c server/run.c server/close_conn.c server/poll.c \
          server/handle_accept.c server/read.c server/write.c
LOGGER  = logger.c logger/stderr.c logger/syslog.c
SESSION = session.c session/add.c session/get.c session/delete.c
UTILS   = utils/hash.c \
          utils/memory.c \
          utils/http.c \
          utils/daemonize.c \
          utils/signalHandling.c

LIBS = ./http/libhttp.a \
       ./auth/libauth.a \
       ./cbuf/libcbuf.a \
       ./class/libclass.a \
       ./hash/libhash.a

AM_CFLAGS = -Wall -I ../include/

bin_PROGRAMS = taskrambler

taskrambler_SOURCES = taskrambler.c \
		      $(IFACE) $(SOCKET) $(SERVER) $(LOGGER) \
		      $(UTILS) $(SESSION) $(STREAM)
taskrambler_CFLAGS  = -Wall -I ../include/ $(COVERAGE_CFLAGS)
taskrambler_LDADD   = $(LIBS) -lrt -lssl -lldap
taskrambler_LDFLAGS = $(COVERAGE_LDFLAGS)

SUBDIRS = auth cbuf class hash http
