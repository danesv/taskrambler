ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS += -I../../include/

noinst_LTLIBRARIES = liblogger.la

liblogger_la_SOURCES = interface/i_logger.c logger.c stderr.c syslog.c
liblogger_la_CFLAGS  = $(AM_CFLAGS)
