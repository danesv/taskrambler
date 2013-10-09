ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

CONFIG = config.c value.c get.c

AM_CFLAGS += -I../../include/

noinst_LIBRARIES = libconfig.a

libconfig_a_SOURCES = $(CONFIG)
libconfig_a_CFLAGS  = $(AM_CFLAGS)
