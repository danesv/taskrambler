ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

AUTH = interface/auth.c \
       credential.c \
       ldap.c \
       storage/storage.c \
       storage/hash_pw.c

AM_CFLAGS += -I../../include/

noinst_LTLIBRARIES = libauth.la

libauth_la_SOURCES = $(AUTH)
libauth_la_CFLAGS  = $(AM_CFLAGS)
