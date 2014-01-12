ACLOCAL_AMFLAGS = -Im4
AM_CPPFLAGS = $(glib_CFLAGS) $(cairo_CFLAGS)

bin_PROGRAMS = c16topng
c16topng_SOURCES = c16topng.c
c16topng_LDADD = $(glib_LIBS) libriley.la libriley-cairo.la

lib_LTLIBRARIES = libriley.la libriley-cairo.la
libriley_la_SOURCES = riley.c
libriley_cairo_la_SOURCES = cairo.c
libriley_cairo_la_LIBADD = libriley.la $(cairo_LIBS)
