ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

TREE = tree.c find.c insert.c inOrderSuccessor.c delete.c walk.c \
       rotateLeft.c rotateRight.c destroy.c

noinst_LIBRARIES = libtree.a

libtree_a_SOURCES = $(TREE)
libtree_a_CFLAGS  = $(CFLAGS) -Wall -I ../../include/
