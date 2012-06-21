# INCLUDES =
AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/multiload \
	-DG_LOG_DOMAIN=\"xfce4-multiload-plugin\" \
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\" \
	$(PLATFORM_CPPFLAGS)

#
# multiload plugin
#
plugin_LTLIBRARIES = \
	libmultiload.la

plugindir = \
	$(libdir)/xfce4/panel/plugins

libmultiload_la_SOURCES = \
	xfce4-multiload-plugin.c xfce4-multiload-plugin.h \
	xfce4-multiload-dialogs.c xfce4-multiload-dialogs.h \
	xfce4-multiload-settings.c xfce4-multiload-settings.h

libmultiload_la_CFLAGS = \
	$(GTK_CFLAGS) \
	$(XFCE4_CFLAGS)

libmultiload_la_LDFLAGS = \
       -avoid-version \
       -module \
       -no-undefined \
       -export-symbols-regex '^xfce_panel_module_(preinit|init|construct)' \
       $(PLATFORM_LDFLAGS)

libmultiload_la_LIBADD = \
	$(GTK_LIBS) \
	$(XFCE4_LIBS) \
	../multiload/multiload-core.la

#
# Desktop file
#
desktopdir =								\
	$(datadir)/xfce4/panel/plugins

desktop_DATA =								\
	multiload.desktop

@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST =								\
	multiload.desktop.in

CLEANFILES =								\
	$(desktop_DATA)

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
