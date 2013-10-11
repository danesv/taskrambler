EXTRA_DIST = Doxyfile.in

if HAVE_DOXYGEN

.PHONY: docs

docs:
	@mkdir -p $(top_srcdir)/docs/api/$(PACKAGE_VERSION)
	$(DOXYGEN) Doxyfile

clean-local:
	@rm -rf $(top_srcdir)/docs/api/
endif 
