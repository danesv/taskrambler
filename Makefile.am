ACLOCAL_AMFLAGS = -I m4

#bin_PROGRAMS = create_token
#create_token_SOURCES = base64.c createToken.c
#create_token_LDADD   = src/libtoken.la $(LIBOBJS)
#create_token_CFLAGS  = -Wall -I include

EXTRA_DIST = include certs assets

myetc_DATA = config/mime.types \
	     config/taskrambler.conf \
	     config/taskrambler.pem \
	     config/taskrambler.crt

nobase_trdata_DATA = assets/html/example.html \
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
		     assets/js/jquery.js \
		     assets/js/jquery-ui.js \
		     assets/js/serverval.js \
		     assets/js/session.js \
		     assets/js/init.js \
		     assets/other/OldNewspaperTypes.ttf \
		     assets/other/old_typewriter.ttf \
		     assets/style/taskrambler.css \
		     assets/style/common.css \
		     assets/style/jquery-ui.css \
		     assets/style/images/animated-overlay.gif \
		     assets/style/images/ui-bg_flat_0_aaaaaa_40x100.png \
		     assets/style/images/ui-bg_flat_75_ffffff_40x100.png \
		     assets/style/images/ui-bg_glass_55_fbf9ee_1x400.png \
		     assets/style/images/ui-bg_glass_65_ffffff_1x400.png \
		     assets/style/images/ui-bg_glass_75_dadada_1x400.png \
		     assets/style/images/ui-bg_glass_75_e6e6e6_1x400.png \
		     assets/style/images/ui-bg_glass_95_fef1ec_1x400.png \
		     assets/style/images/ui-bg_highlight-soft_75_cccccc_1x100.png \
		     assets/style/images/ui-icons_222222_256x240.png \
		     assets/style/images/ui-icons_2e83ff_256x240.png \
		     assets/style/images/ui-icons_454545_256x240.png \
		     assets/style/images/ui-icons_888888_256x240.png \
		     assets/style/images/ui-icons_cd0a0a_256x240.png \
		     assets/favicon.ico

SUBDIRS = src tests docs config

.PHONY: docs coverage-html

if HAVE_DOXYGEN
docs:
	-$(MAKE) -C docs $(AM_MAKEFLAGS) -k $@
endif

if HAVE_GCOV
coverage-html:
	-$(MAKE) -C tests $(AM_MAKEFLAGS) -k $@
endif

tags:
	ctags -R -V --langdef=cclass --langmap=cclass:.h --regex-cclass='/^[ \t]*CLASS\(([a-zA-Z0-9_]+)/\1/d,definition/'

loc:
	find src/ include/ -not -path testers -and -name "*.[ch]" -exec sed '/\/\*/,/\*\//d;/\/\//d' {} \; | wc -l

TR_DIRS = $(DESTDIR)/$(trdatadir)/assets/doc \
	  $(DESTDIR)/$(trdatadir)/assets/html/doc \
	  $(DESTDIR)/$(trdatadir)/assets/coverage \
	  $(DESTDIR)/$(trdatadir)/assets/html/coverage \
	  $(DESTDIR)/$(trstatedir) \
	  $(DESTDIR)/$(trdatadir)/docs/

install-data-hook:
	$(INSTALL) -d -m 755 $(TR_DIRS)
	if [ -d docs/api/ ]; then \
	  cp -Rfd docs/api/ $(DESTDIR)/$(trdatadir)/docs/; \
	fi
	if [ -d tests/coverage/ ]; then \
	  cp -Rfd tests/coverage/ $(DESTDIR)/$(trdatadir)/docs/; \
	fi
	if [ -e $(DESTDIR)/$(trdatadir)/docs/api/$(PACKAGE_VERSION)/html ]; \
	then \
	  $(LN_S) -f \
	    $(DESTDIR)/$(trdatadir)/docs/api/$(PACKAGE_VERSION)/html \
	    $(DESTDIR)/$(trdatadir)/assets/doc/$(PACKAGE_VERSION); \
	  $(LN_S) -f \
	    $(DESTDIR)/$(trdatadir)/docs/api/$(PACKAGE_VERSION)/html \
	    $(DESTDIR)/$(trdatadir)/assets/html/doc/$(PACKAGE_VERSION); \
	fi
	if [ -e $(DESTDIR)/$(trdatadir)/docs/coverage/$(PACKAGE_VERSION) ]; \
	then \
	  $(LN_S) -f \
	    $(DESTDIR)/$(trdatadir)/docs/coverage/$(PACKAGE_VERSION) \
	    $(DESTDIR)/$(trdatadir)/assets/coverage/$(PACKAGE_VERSION); \
	  $(LN_S) -f \
	    $(DESTDIR)/$(trdatadir)/docs/coverage/$(PACKAGE_VERSION) \
	    $(DESTDIR)/$(trdatadir)/assets/html/coverage/$(PACKAGE_VERSION); \
	fi

uninstall-hook:
	rm -Rf $(varlibdir) $(varrundir) $(trdatadir)
