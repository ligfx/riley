AC_INIT([riley], [0])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall -Werror])
AM_SILENT_RULES([yes])
AM_PROG_AR
LT_INIT
AC_PROG_LIBTOOL
AC_CONFIG_FILES([Makefile])

PKG_CHECK_MODULES([cairo], [cairo])
PKG_CHECK_MODULES([glib], [glib-2.0])

AC_OUTPUT

cat <<EOF

----- summary -----
package: $PACKAGE_NAME
version: $PACKAGE_VERSION
prefix: $prefix
compiler: $CC $CFLAGS $CPPFLAGS
linker: $LD $LDFLAGS $LIBS
glib: $glib_CFLAGS $glib_LDFLAGS
cairo: $cairo_CFLAGS $cairo_LDFLAGS
make: ${MAKE-make}

EOF
