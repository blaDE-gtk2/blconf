
AM_CPPFLAGS = \
	-DGLIB_DISABLE_DEPRECATION_WARNINGS \
	-I$(top_srcdir) \
	-DLIBBLCONF_COMPILATION \
	-DG_LOG_DOMAIN=\"blconf-common\" \
	$(PLATFORM_CPPFLAGS)

noinst_LTLIBRARIES = \
	libblconf-common.la \
	libblconf-gvaluefuncs.la

libblconf_common_la_SOURCES = \
	blconf-errors.c \
	blconf-marshal.c \
	blconf-marshal.h

libblconf_common_la_CFLAGS = \
	$(GLIB_CFLAGS) \
	$(PLATFORM_CFLAGS)

libblconf_common_la_LDFLAGS = \
	$(PLATFORM_LDFLAGS)

libblconf_common_la_LIBADD = \
	$(GLIB_LIBS)

libblconf_built_sources = \
	blconf-alias.h \
	blconf-aliasdef.c

libblconf_gvaluefuncs_la_SOURCES = \
	blconf-gvaluefuncs.c \
	blconf-gvaluefuncs.h

libblconf_gvaluefuncs_la_CFLAGS = \
	$(GLIB_CFLAGS) \
	$(DBUS_GLIB_CFLAGS) \
	$(PLATFORM_CFLAGS)

libblconf_gvaluefuncs_la_LDFLAGS = \
	$(PLATFORM_LDFLAGS)

libblconf_gvaluefuncs_la_LIBADD = \
	$(GLIB_LIBS) \
	$(DBUS_GLIB_LIBS)


if MAINTAINER_MODE

BUILT_SOURCES = \
	blconf-marshal.c \
	blconf-marshal.h

blconf-marshal.h: stamp-blconf-marshal.h
	@true
stamp-blconf-marshal.h: $(srcdir)/blconf-marshal.list Makefile
	$(AM_V_GEN) glib-genmarshal --prefix=_blconf_marshal $(srcdir)/blconf-marshal.list --header > blconf-marshal.h \
	&& echo timestamp > $(@F)
blconf-marshal.c: $(srcdir)/blconf-marshal.list Makefile
	$(AM_V_GEN) echo '#include "blconf-marshal.h"' > blconf-marshal.c \
	&& glib-genmarshal --prefix=_blconf_marshal $(srcdir)/blconf-marshal.list --body >> blconf-marshal.c



CLEANFILES = \
	actual-abi \
	expected-abi

DISTCLEANFILES = \
	$(libblconf_built_sources)

BUILT_SOURCES += \
	$(libblconf_built_sources)

blconf-alias.h: make-blxo-alias.pl $(top_srcdir)/blconf/blconf.symbols
	$(AM_V_GEN) $(PERL) $(srcdir)/make-blxo-alias.pl <$(top_srcdir)/blconf/blconf.symbols >blconf-alias.h

blconf-aliasdef.c: make-blxo-alias.pl $(top_srcdir)/blconf/blconf.symbols
	$(AM_V_GEN) $(PERL) $(srcdir)/make-blxo-alias.pl -def <$(top_srcdir)/blconf/blconf.symbols >blconf-aliasdef.c

endif


EXTRA_DIST = \
	make-blxo-alias.pl \
	blconf-alias.h \
	blconf-aliasdef.c \
	blconf-common-private.h \
	blconf-dbus.xml \
	blconf-marshal.list \
	blconf-types.c

# required for make distcheck
dist-hook: all
