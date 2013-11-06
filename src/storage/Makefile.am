ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS += -I../../include/

noinst_LTLIBRARIES = libstorage.la

libstorage_la_SOURCES = storage.c get.c put.c update.c delete.c
libstorage_la_CFLAGS  = $(AM_CFLAGS)
