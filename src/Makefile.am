ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects


TRUTILS = utils/http.c \
	  utils/daemonize.c \
	  utils/signalHandling.c \
	  utils/mime_type.c

TRHTTPSERVER = http/libhttp.la \
	       asset/libasset.la \
	       server/libserver.la

TR = ./application/.libs/libapplication.a \
     ./user/.libs/libuser.a \
     ./auth/.libs/libauth.a \
     ./storage/.libs/libstorage.a \
     ./session/.libs/libsession.a \
     ./config/.libs/libconfig.a \
     ./router/.libs/librouter.a

TRLIBS = -ltrbase -ltrhashing -ltrio -ltrdata -ltrutils -ltrhttpserver
USEDLIBS = -lrt -lssl -lcrypto -lldap -lgdbm -luuid

AM_CFLAGS += -I../include/

bin_PROGRAMS = taskrambler
lib_LTLIBRARIES = libtrutils.la libtrhttpserver.la

libtrutils_la_SOURCES = $(TRUTILS)
libtrutils_la_CFLAGS  = $(AM_CFLAGS)
libtrutils_la_LIBADD  =

libtrhttpserver_la_SOURCES =
libtrhttpserver_la_CFLAGS  = $(AM_CFLAGS)
libtrhttpserver_la_LIBADD  = $(TRHTTPSERVER)

taskrambler_SOURCES = taskrambler.c $(IFACE) $(UTILS)
taskrambler_CFLAGS  = $(AM_CFLAGS)
taskrambler_LDADD   = $(TRLIBS) $(TR) $(USEDLIBS)
taskrambler_LDFLAGS = -Wl,--export-dynamic \
		      -Wl,--whole-archive,./application/.libs/libapplication.a,--no-whole-archive

SUBDIRS = asset auth http \
	  server session application \
	  storage user config router
