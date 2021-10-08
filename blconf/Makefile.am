
AM_CPPFLAGS = \
	-DGLIB_DISABLE_DEPRECATION_WARNINGS \
	-I$(top_srcdir) \
	-DLIBBLCONF_COMPILATION \
	-DG_LOG_DOMAIN=\"blconf\" \
	$(PLATFORM_CPPFLAGS)

lib_LTLIBRARIES = libblconf-0.la

libblconfincludedir = $(includedir)/xfce4/blconf-$(LIBBLCONF_VERSION_API)/blconf
libblconfinclude_HEADERS = \
	blconf-binding.h \
	blconf-channel.h \
	blconf-errors.h \
	blconf-types.h \
	blconf.h

libblconf_0_la_SOURCES = \
	$(libblconfinclude_HEADERS) \
	blconf-binding.c \
	blconf-cache.c \
	blconf-cache.h \
	blconf-channel.c \
	blconf-dbus-bindings.h \
	blconf-private.h \
	blconf.c \
	$(top_srcdir)/common/blconf-types.c

libblconf_0_la_CFLAGS = \
	$(GLIB_CFLAGS) \
	$(DBUS_CFLAGS) \
	$(GTHREAD_CFLAGS) \
	$(DBUS_GLIB_CFLAGS) \
	$(PLATFORM_CFLAGS)

libblconf_0_la_LDFLAGS = \
	-export-dynamic \
	-version-info $(LIBBLCONF_VERINFO) \
	-export-symbols-regex "^[^_].*" \
	-no-undefined \
	$(PLATFORM_LDFLAGS)

libblconf_0_la_LIBADD = \
	$(top_builddir)/common/libblconf-common.la \
	$(top_builddir)/common/libblconf-gvaluefuncs.la \
	$(GLIB_LIBS) \
	$(GTHREAD_LIBS) \
	$(DBUS_LIBS) \
	$(DBUS_GLIB_LIBS)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libblconf-0.pc


if MAINTAINER_MODE

BUILT_SOURCES = \
	blconf-dbus-bindings.h

blconf-dbus-bindings.h: $(top_srcdir)/common/blconf-dbus.xml Makefile
	$(AM_V_GEN) dbus-binding-tool --mode=glib-client $< > $(@).tmp \
	&& sed -e 's/^\([[:space:]]\+GValue[[:space:]]\+[^=]\+\).*;$$/\1 = { 0, };/' <$(@).tmp >$@ \
	&& rm -f $(@).tmp
# the above fixes a bug in dbus-binding-tool's output
# dbus-binding-tool doesn't initialise GValues to { 0, } properly

CLEANFILES = \
	$(BUILT_SOURCES) \
	blconf-dbus-bindings.h.tmp

endif

EXTRA_DIST = \
	abicheck.sh \
	blconf.symbols

# required for gtk-doc
dist-hook: all

if HAVE_GNUC_VISIBILITY
TESTS = abicheck.sh
endif
