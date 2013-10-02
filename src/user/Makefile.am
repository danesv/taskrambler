ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

noinst_LIBRARIES = libuser.a

libuser_a_SOURCES = user.c load.c save.c
libuser_a_CFLAGS  = $(CFLAGS) $(DRAGONEGG_FLAGS) -Wall -I ../../include/
