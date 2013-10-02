ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

noinst_LIBRARIES = liblogger.a

liblogger_a_SOURCES = interface/i_logger.c logger.c stderr.c syslog.c
liblogger_a_CFLAGS  = $(CFLAGS) $(DRAGONEGG_FLAGS) -Wall -I ../../include/
