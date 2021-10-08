
blconf_perl_xs_files = \
	Blconf.xs \
	BlconfBinding.xs \
	BlconfChannel.xs

EXTRA_DIST = \
	$(blconf_perl_xs_files)

if BUILD_PERL_BINDINGS
all-local:
	if test "$(top_srcdir)" != "$(top_builddir)"; then \
	    for f in $(blconf_perl_xs_files); do \
	        test -f $$f || $(LN_S) $(srcdir)/$$f $$f; \
	    done; \
	fi
endif
