if HAVE_DOXYGEN
# directory = $(top_srcdir)/docs/man/man3/
# 
# dist_man_MANS = $(directory)/man_page_1.3 $(directory)/man_page_2.3
# $(directory)/man_page_1.3: doxyfile.stamp
# $(directory)/man_page_2.3: doxyfile.stamp

EXTRA_DIST = Doxyfile.in

.PHONY: docs

docs:
	mkdir $(PACKAGE_VERSION)
	$(DOXYGEN) Doxyfile
	echo Timestamp > doxyfile.stamp

clean-local:
	rm -rf $(top_srcdir)/docs/$(PACKAGE_VERSION)
endif 
