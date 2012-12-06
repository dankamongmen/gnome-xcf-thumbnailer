AC_PREREQ(2.59)

AC_INIT([gnome-xcf-thumbnailer], [1.0], [hadess@hadess.net])
AM_INIT_AUTOMAKE(1.9 dist-bzip2 no-dist-gzip check-news)
AC_CONFIG_HEADERS([config.h])

AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_SED
AM_PROG_CC_C_O
AC_FUNC_MMAP

AC_CHECK_HEADERS(inttypes.h getopt.h)

PKG_CHECK_MODULES(LIBPNG, libpng, [], [AC_MSG_FAILURE([You need libpng's development package installed])])
AC_SUBST(LIBPNG_CFLAGS)
AC_SUBST(LIBPNG_LIBS)

PKG_CHECK_MODULES(GIO, gio-2.0, [], [AC_MSG_FAILURE([You need glib 2.16 or newer installed])])

AC_PATH_PROG(GCONFTOOL, gconftool-2, no)

if test x$GCONFTOOL = "xno"; then
  AC_MSG_FAILURE([gconftool-2 is needed])
fi

AM_GCONF_SOURCE_2

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
