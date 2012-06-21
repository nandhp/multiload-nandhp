AC_PREREQ(2.50)
AC_INIT([xfce4-multiload-plugin], [0.1], [nandhp@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.8 -Wall -Werror dist-bzip2])
# AM_CONFIG_HEADER([config.h])
AM_MAINTAINER_MODE()

LT_PREREQ([2.2.6])
LT_INIT([disable-static])

AC_PROG_CC()
# Compiling sources with per-target flags requires AM_PROG_CC_C_O
AM_PROG_CC_C_O()
AC_PROG_LD()
AC_PROG_INSTALL()
AC_PROG_LIBTOOL()
AC_PROG_INTLTOOL(, [no-xml])

# Environment checks from autoscan
# AC_HEADER_STDC()
# AC_CHECK_HEADERS([stdlib.h unistd.h locale.h stdio.h errno.h time.h string.h \
#                   math.h sys/types.h sys/wait.h memory.h signal.h sys/prctl.h \
#                   libintl.h])
# AC_CHECK_FUNCS([bind_textdomain_codeset])
AC_CHECK_HEADERS([fcntl.h libintl.h string.h unistd.h])
AC_CHECK_FUNCS([memset rint strchr])
AC_TYPE_SIZE_T()

# Misc checks
AC_MSG_CHECKING(whether compiler understands -Wall)
old_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS -Wall"
AC_TRY_COMPILE([],[],
  AC_MSG_RESULT(yes),
    AC_MSG_RESULT(no)
      CFLAGS="$old_CFLAGS")

# I18n
rm -f $srcdir/po/LINGUAS
for po_file in `ls $srcdir/po/*.po | sort`;
do
    lang=`echo "$po_file" | sed "s|.*/po/\(.*\)\.po|\1|g"`
    echo $lang >> $srcdir/po/LINGUAS
done

GETTEXT_PACKAGE=lxpanel
#AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION(0.14.1)
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package.])

AM_GLIB_GNU_GETTEXT

# Dependencies
AM_PATH_GTK_2_0([2.10.0],,AC_MSG_ERROR([Gtk+ 2.10.0 or higher required.]))
PKG_CHECK_MODULES(LIBGTOP, [libgtop-2.0 >= 2.11.92])
AC_SUBST(LIBGTOP_CFLAGS)
AC_SUBST(LIBGTOP_LIBS)

# Xfce
AC_ARG_WITH([xfce4], AS_HELP_STRING([--with-xfce4], [Build plugin for xfce4-panel]),
            [], [with_xfce4=check])
AS_IF([test "x$with_xfce4" != "xno"], [
    pkg_modules="libxfce4ui-1 >= 4.6.0 \
                 libxfce4util-1.0 >= 4.6.0
                 libxfce4panel-1.0 >= 4.6.0"
    PKG_CHECK_MODULES(XFCE4, [$pkg_modules],
                      [
                        AC_DEFINE([HAVE_XFCE4], [1], [Support for xfce4-panel])
                        with_xfce4=yes
                      ],[
                        if test "x$with_xfce4" != xcheck; then
                          AC_MSG_FAILURE(
                           [--with-xfce4 was given, but test failed])
                        fi
                        with_xfce4=no
                      ])
])
AM_CONDITIONAL([HAVE_XFCE4], [test x$with_xfce4 = xyes])
AC_SUBST(XFCE4_CFLAGS)
AC_SUBST(XFCE4_LIBS)

# lxpanel
AC_ARG_WITH([lxpanel], AS_HELP_STRING([--with-lxpanel], [Build plugin for lxpanel]),
            [], [with_lxpanel=check])
AS_IF([test "x$with_lxpanel" != "xno"], [
    pkg_modules="lxpanel"
    PKG_CHECK_MODULES(LXPANEL, [$pkg_modules],
                      [
                        AC_DEFINE([HAVE_LXPANEL], [1], [Support for lxpanel])
                        with_lxpanel=yes
                      ],[
                        if test "x$with_lxpanel" != xcheck; then
                          AC_MSG_FAILURE(
                           [--with-lxpanel was given, but test failed])
                        fi
                        with_lxpanel=no
                      ])
])
AM_CONDITIONAL(HAVE_LXPANEL, test x$with_lxpanel = xyes)
AC_SUBST(LXPANEL_CFLAGS)
AC_SUBST(LXPANEL_LIBS)

# Output files
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    Makefile
    multiload/Makefile
    xfce4/Makefile
    po/Makefile.in
])
AC_OUTPUT

echo
echo "Build Configuration:"
echo
echo "* xfce4-panel :    $with_xfce4"
echo
echo "* lxpanel :        $with_lxpanel"
echo
