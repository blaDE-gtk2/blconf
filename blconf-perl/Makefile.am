@SET_MAKE@

SUBDIRS = \
	xs

blconf_perl_files = \
	copyright.pod \
	doctypes \
	blconfperl.h \
	blconfperl.maps \
	xs_files

EXTRA_DIST = \
	$(blconf_perl_files)

if BUILD_PERL_BINDINGS
Makefile.perl: Makefile.PL xs_files blconfperl.maps Makefile
	if test "$(top_srcdir)" != "$(top_builddir)"; then \
	    for f in $(blconf_perl_files); do \
	        test -f $$f || $(LN_S) $(srcdir)/$$f $$f; \
	    done; \
	fi
	top_builddir="$(top_srcdir)" $(PERL) Makefile.PL PREFIX="$(prefix)" $(PERL_MAKE_OPTIONS)

all-local: Makefile.perl
	$(MAKE) -f Makefile.perl

install-exec-local:
	$(MAKE) -f Makefile.perl install DESTDIR=$(DESTDIR)

distclean-local:
	$(MAKE) -f Makefile.perl realclean
endif
