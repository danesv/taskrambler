ACLOCAL_AMFLAGS = -I m4

STREAM = stream.c read.c write.c
IFACE  = interface/stream_reader.c \
	 interface/stream_writer.c

noinst_LIBRARIES = libstream.a

libstream_a_SOURCES = $(STREAM) $(IFACE)
libstream_a_CFLAGS  = -Wall -I ../../include/
