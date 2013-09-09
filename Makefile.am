ACLOCAL_AMFLAGS = -I m4

#bin_PROGRAMS = create_token
#create_token_SOURCES = base64.c createToken.c
#create_token_LDADD   = src/libtoken.la $(LIBOBJS)
#create_token_CFLAGS  = -Wall -I include

EXTRA_DIST = include assets certs config

SUBDIRS = src tests

include $(top_srcdir)/Makefile.am.coverage
