
AM_CPPFLAGS = \
	-DGLIB_DISABLE_DEPRECATION_WARNINGS \
	-I$(top_srcdir) \
	-DG_LOG_DOMAIN=\"blconf-query\" \
	-DDATADIR=\"$(datadir)\" \
	-DSRCDIR=\"$(top_srcdir)\" \
	-DLOCALEDIR=\"$(localedir)\" \
	$(PLATFORM_CPPFLAGS)

bin_PROGRAMS = blconf-query

blconf_query_SOURCES = \
	main.c

blconf_query_CFLAGS = \
	$(GLIB_CFLAGS) \
	$(LIBBLADEUTIL_CFLAGS) \
	$(DBUS_GLIB_CFLAGS) \
	$(PLATFORM_CFLAGS)

blconf_query_LDFLAGS = \
	$(PLATFORM_LDFLAGS)

blconf_query_LDADD = \
	$(top_builddir)/common/libblconf-gvaluefuncs.la \
	$(top_builddir)/blconf/libblconf-0.la \
	$(GLIB_LIBS) \
	$(LIBBLADEUTIL_LIBS) \
	$(DBUS_GLIB_LIBS)
