ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

TREE = tree.c find.c insert.c inOrderSuccessor.c delete.c walk.c \
       rotateLeft.c rotateRight.c destroy.c

AM_CFLAGS += -I../../include/

noinst_LTLIBRARIES = libtree.la

libtree_la_SOURCES = $(TREE)
libtree_la_CFLAGS  = $(AM_CFLAGS)
