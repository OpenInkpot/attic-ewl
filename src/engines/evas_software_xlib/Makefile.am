MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-I$(top_builddir)/src/lib \
-I$(top_srcdir)/src/engines/evas_software_x11 \
-I$(top_builddir)/src/engines/evas_software_x11 \
@EVAS_CFLAGS@ \
@ECORE_CFLAGS@ \
@ECORE_X_CFLAGS@

pkgdir = $(libdir)/ewl/engines

if EWL_ENABLE_EVAS_SOFTWARE_XLIB

pkg_LTLIBRARIES = evas_software_xlib.la

evas_software_xlib_la_SOURCES = \
Ewl_Engine_Evas_Software_Xlib.h \
ewl_engine_evas_software_xlib.c

evas_software_xlib_la_LIBADD = $(top_builddir)/src/lib/libewl.la @ECORE_X_LIBS@ @ECORE_LIBS@ @EVAS_LIBS@
evas_software_xlib_la_LDFLAGS = -module -version-info @INTERFACE_VERSION@
evas_software_xlib_la_DEPENDENCIES =

endif
