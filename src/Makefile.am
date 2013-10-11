ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects


TRUTILS = utils/hash.c \
	  utils/memory.c \
	  utils/http.c \
	  utils/daemonize.c \
	  utils/signalHandling.c \
	  utils/mime_type.c

TRBASESRC = interface/subject.c \
	    interface/observer.c

TRBASELIBS = class/libclass.la \
	     cbuf/libcbuf.la \
	     hash/libhash.la \
	     queue/libqueue.la \
	     logger/liblogger.la \
	     tree/libtree.la \
	     stream/libstream.la

TRHTTPSERVER = http/libhttp.la \
	       asset/libasset.la \
	       server/libserver.la \
	       socket/libsocket.la

TR = ./application/.libs/libapplication.a \
     ./user/.libs/libuser.a \
     ./storage/.libs/libstorage.a \
     ./auth/.libs/libauth.a \
     ./session/.libs/libsession.a \
     ./config/.libs/libconfig.a

TRLIBS = -ltrutils -ltrhttpserver -ltrbase
USEDLIBS = -lrt -lssl -lldap -lgdbm -luuid

AM_CFLAGS += -I../include/

bin_PROGRAMS = taskrambler
lib_LTLIBRARIES = libtrutils.la libtrbase.la libtrhttpserver.la

libtrutils_la_SOURCES = $(TRUTILS)
libtrutils_la_CFLAGS  = $(AM_CFLAGS)
libtrutils_la_LIBADD  =

libtrbase_la_SOURCES = $(TRBASESRC)
libtrbase_la_CFLAGS  = $(AM_CFLAGS)
libtrbase_la_LIBADD  = $(TRBASELIBS)

libtrhttpserver_la_SOURCES =
libtrhttpserver_la_CFLAGS  = $(AM_CFLAGS)
libtrhttpserver_la_LIBADD  = $(TRHTTPSERVER)

taskrambler_SOURCES = taskrambler.c $(IFACE) $(UTILS)
taskrambler_CFLAGS  = $(AM_CFLAGS)
taskrambler_LDADD   = $(TRLIBS) $(USEDLIBS) $(TR)

SUBDIRS = asset auth cbuf class hash queue http \
	  logger server session socket stream tree application \
	  storage user config
