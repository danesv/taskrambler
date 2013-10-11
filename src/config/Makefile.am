ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

CONFIG = config.c value.c get.c

AM_CFLAGS += -I../../include/

noinst_LTLIBRARIES = libconfig.la

libconfig_la_SOURCES = $(CONFIG)
libconfig_la_CFLAGS  = $(AM_CFLAGS)
