ACLOCAL_AMFLAGS = -I m4

#bin_PROGRAMS = create_token
#create_token_SOURCES = base64.c createToken.c
#create_token_LDADD   = src/libtoken.la $(LIBOBJS)
#create_token_CFLAGS  = -Wall -I include

EXTRA_DIST = include assets certs config run

SUBDIRS = src tests docs

.PHONY: docs coverage-html

if HAVE_DOXYGEN
docs:
	-$(MAKE) -C docs $(AM_MAKEFLAGS) -k $@
endif

if HAVE_GCOV
coverage-html:
	-$(MAKE) -C tests $(AM_MAKEFLAGS) -k $@
endif
