ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

STREAM = stream.c read.c write.c
IFACE  = interface/reader.c \
	 interface/writer.c

AM_CFLAGS += -I../../include/

noinst_LTLIBRARIES = libstream.la

libstream_la_SOURCES = $(STREAM) $(IFACE)
libstream_la_CFLAGS  = $(AM_CFLAGS)
