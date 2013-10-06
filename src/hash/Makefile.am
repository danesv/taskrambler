ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

HASH = hash.c add.c get.c get_first.c delete.c each.c value.c \
       cleanup.c interface/hashable.c

noinst_LIBRARIES = libhash.a

libhash_a_SOURCES = $(HASH)
libhash_a_CFLAGS  = $(CFLAGS) $(DRAGONEGG_FLAGS) -Wall -I ../../include/
