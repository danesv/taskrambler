ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS += -I../../include/

noinst_LIBRARIES = libauth.a

libauth_a_SOURCES = interface/auth.c \
		    credential.c \
		    ldap.c \
		    storage/storage.c \
		    storage/hash_pw.c
libauth_a_CFLAGS  = $(AM_CFLAGS)
