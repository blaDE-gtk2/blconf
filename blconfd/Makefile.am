
AM_CPPFLAGS = \
	-DGLIB_DISABLE_DEPRECATION_WARNINGS \
	-I$(top_srcdir) \
	-DG_LOG_DOMAIN=\"blconfd\" \
	-DLIBBLCONF_COMPILATION \
	-DBINDIR=\"$(bindir)\" \
	-DLOCALEDIR=\"$(localedir)\" \
	$(PLATFORM_CPPFLAGS)

blconfddir = $(HELPER_PATH_PREFIX)/xfce4/blconf
blconfd_PROGRAMS = blconfd

blconf_backend_sources =

if BUILD_BLCONF_BACKEND_PERCHANNEL_XML
blconf_backend_sources += \
	blconf-backend-perchannel-xml.c \
	blconf-backend-perchannel-xml.h
endif

blconfd_SOURCES = \
	main.c \
	blconf-backend-factory.c \
	blconf-backend-factory.h \
	blconf-backend.c \
	blconf-backend.h \
	blconf-daemon.c \
	blconf-daemon.h \
	blconf-dbus-server.h \
	blconf-locking-utils.c \
	blconf-locking-utils.h \
	$(blconf_backend_sources) \
	$(top_srcdir)/common/blconf-types.c

blconfd_CFLAGS = \
	$(GLIB_CFLAGS) \
	$(GTHREAD_CFLAGS) \
	$(DBUS_CFLAGS) \
	$(DBUS_GLIB_CFLAGS) \
	$(LIBBLADEUTIL_CFLAGS) \
	$(PLATFORM_CFLAGS)

blconfd_LDFLAGS = \
	$(PLATFORM_LDFLAGS)

if ENABLE_PROFILING
blconfd_CFLAGS += -pg
endif

blconfd_LDADD = \
	$(top_builddir)/common/libblconf-common.la \
	$(top_builddir)/common/libblconf-gvaluefuncs.la \
	$(GLIB_LIBS) \
	$(GTHREAD_LIBS) \
	$(DBUS_LIBS) \
	$(DBUS_GLIB_LIBS) \
	$(LIBBLADEUTIL_LIBS)

servicedir = $(datadir)/dbus-1/services
service_in_files = org.blade.Blconf.service.in
service_DATA = $(service_in_files:.service.in=.service)

%.service: %.service.in
	$(AM_V_GEN) sed -e "s,\@HELPERDIR\@,$(HELPER_PATH_PREFIX),g" < $< > $@

CLEANFILES = \
	$(service_DATA)

EXTRA_DIST = \
	blconf-backend-perchannel-xml.c \
	blconf-backend-perchannel-xml.h \
	$(service_in_files)


if MAINTAINER_MODE

blconf_built_sources = \
	blconf-dbus-server.h

DISTCLEANFILES = \
	$(blconf_built_sources)

BUILT_SOURCES = \
	$(blconf_built_sources)

blconf-dbus-server.h: $(top_srcdir)/common/blconf-dbus.xml
	$(AM_V_GEN) dbus-binding-tool --mode=glib-server --prefix=blconf $< > $@

endif

# required for gtk-doc
dist-hook: all
