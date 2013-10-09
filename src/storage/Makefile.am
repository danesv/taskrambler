ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS += -I../../include/

noinst_LIBRARIES = libstorage.a

libstorage_a_SOURCES = storage.c get.c put.c update.c
libstorage_a_CFLAGS  = $(AM_CFLAGS)
