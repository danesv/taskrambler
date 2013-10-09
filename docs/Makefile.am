if HAVE_DOXYGEN

EXTRA_DIST = Doxyfile.in

.PHONY: docs

docs:
	@mkdir -p $(top_srcdir)/docs/api/$(PACKAGE_VERSION)
	$(DOXYGEN) Doxyfile

clean-local:
	@rm -rf $(top_srcdir)/docs/docs/$(PACKAGE_VERSION)
endif 
