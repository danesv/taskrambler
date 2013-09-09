ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

STREAM = stream.c read.c write.c
IFACE  = interface/reader.c \
	 interface/writer.c

noinst_LIBRARIES = libstream.a

libstream_a_SOURCES = $(STREAM) $(IFACE)
libstream_a_CFLAGS  = $(CFLAGS) -Wall -I ../../include/
