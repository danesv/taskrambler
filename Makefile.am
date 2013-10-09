ACLOCAL_AMFLAGS = -I m4

#bin_PROGRAMS = create_token
#create_token_SOURCES = base64.c createToken.c
#create_token_LDADD   = src/libtoken.la $(LIBOBJS)
#create_token_CFLAGS  = -Wall -I include

EXTRA_DIST = include certs assets config run

myetc_DATA = config/mime.types config/taskrambler.conf

nobase_varlib_DATA = assets/html/example.html \
		     assets/html/_footer.html \
		     assets/html/_login.html \
		     assets/html/_menu.html \
		     assets/html/_signup.html \
		     assets/html/_statusline.html \
		     assets/html/_title.html \
		     assets/html/_author.html \
		     assets/html/_main.html \
		     assets/html/author.html \
		     assets/html/documentation.html \
		     assets/html/download.html \
		     assets/html/layout.html \
		     assets/html/main.html \
		     assets/html/_documentation.html \
		     assets/html/_download.html \
		     assets/image/waldschrat.jpg \
		     assets/image/fav128.png \
		     assets/image/fav16.png \
		     assets/image/fav256.png \
		     assets/image/fav32.png \
		     assets/image/fav64.png \
		     assets/image/favicon.ico \
		     assets/image/rambler-bg.jpg \
		     assets/image/rambler-border-b.jpg \
		     assets/image/rambler-border-bl.jpg \
		     assets/image/rambler-border-br.jpg \
		     assets/image/rambler-border-l.jpg \
		     assets/image/rambler-border-r.jpg \
		     assets/image/rambler-border-t.jpg \
		     assets/image/rambler-border-tl.jpg \
		     assets/image/rambler-border-tr.jpg \
		     assets/image/rambler-logo-small.jpg \
		     assets/image/rambler-logo2-small.jpg \
		     assets/js/jquery-1.7.1.js \
		     assets/js/jquery-1.7.1.min.js \
		     assets/js/jquery.js \
		     assets/js/serverval.js \
		     assets/js/session.js \
		     assets/js/init.js \
		     assets/other/OldNewspaperTypes.ttf \
		     assets/other/old_typewriter.ttf \
		     assets/style/taskrambler.css \
		     assets/style/common.css \
		     assets/favicon.ico

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

install-data-hook:
	mkdir -p $(varlibdir)/assets/doc $(varlibdir)/assets/html/doc $(varrundir)
	$(LN_S) -f $(trdatadir)/docs/api/$(PACKAGE_VERSION)/html $(varlibdir)/assets/doc/$(PACKAGE_VERSION)
	$(LN_S) -f $(trdatadir)/docs/api/$(PACKAGE_VERSION)/html $(varlibdir)/assets/html/doc/$(PACKAGE_VERSION)
