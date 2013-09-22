ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

IFACE = interface/subject.c \
	interface/observer.c

UTILS = utils/hash.c \
	utils/memory.c \
	utils/http.c \
	utils/daemonize.c \
	utils/signalHandling.c \
	utils/mime_type.c

LIBS = ./application/libapplication.a \
       ./user/libuser.a \
       ./storage/libstorage.a \
       ./http/libhttp.a \
       ./auth/libauth.a \
       ./cbuf/libcbuf.a \
       ./class/libclass.a \
       ./asset/libasset.a \
       ./hash/libhash.a \
       ./queue/libqueue.a \
       ./logger/liblogger.a \
       ./server/libserver.a \
       ./session/libsession.a \
       ./socket/libsocket.a \
       ./stream/libstream.a \
       ./tree/libtree.a

AM_CFLAGS = -Wall -I ../include/

bin_PROGRAMS = taskrambler

taskrambler_SOURCES = taskrambler.c $(IFACE) $(UTILS)
taskrambler_CFLAGS  = $(CFLAGS) -Wall -DPWD=\"$(PWD)\" -I../include/ # $(COVERAGE_CFLAGS)
taskrambler_LDADD   = $(LIBS) -lrt -lssl -lldap -lgdbm -luuid
#taskrambler_LDFLAGS = $(COVERAGE_LDFLAGS)

SUBDIRS = asset auth cbuf class hash queue http \
	  logger server session socket stream tree application \
	  storage user
