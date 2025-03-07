#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])

m4_define([ver_major], [0])
m4_define([ver_minor], [3])
m4_define([ver_release], [3])
m4_define([ver_build], [0])

m4_define([swars_version],
      [ver_major.ver_minor.ver_release.ver_build])

AC_INIT([Syndicate Wars Port], [swars_version], [unavowed@vexillium.org], [swars])

AC_CONFIG_SUBDIRS([bflibrary])

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIRS([m4])

AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([foreign])
ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_DEFINE([VER_MAJOR], [ver_major], [Major version number])
AC_DEFINE([VER_MINOR], [ver_minor], [Minor version number])
AC_DEFINE([VER_RELEASE], [ver_release], [Release number])
AC_DEFINE([VER_BUILD], [ver_build], [Build number])

# M4 defines __unix__ to be the empty string on UNIX-like systems.  This
# causes AC_DEFINE to fail if it is used to define __unix__.  This seems to
# fix the problem.
ifdef([__unix__], define([__unix__], [[__unix__]]))

# Set the proper prefix
AS_IF([test "$prefix" = "NONE"], [
  prefix="$ac_default_prefix"
])
AS_IF([test "$exec_prefix" = "NONE"], [
  exec_prefix="$prefix"
])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AM_PROG_AS
AM_PATH_PYTHON([3.1])
PKG_PROG_PKG_CONFIG
AC_CHECK_PROGS(WINDRES, $WINDRES "${target_alias}-windres" "${target}-windres", "windres", :)
AC_PROG_SED # `sed` is used in Makefile for packaging shared libraries on Windows

# Checks for libraries.
AC_MSG_CHECKING([if we are compiling for an i386 processor])
AC_COMPILE_IFELSE([AC_LANG_SOURCE([
  asm ("pushl \$0x0; pop %eax");
])], [
  AC_MSG_RESULT([yes])
], [
  AC_MSG_RESULT([no])
  AC_MSG_FAILURE([this Syndicate Wars port will only run on i386 processors])
])

AC_MSG_CHECKING([if C symbols need an underscore])
AC_LINK_IFELSE([AC_LANG_SOURCE([
  void test_function (void) { }
  int main (void) { asm ("call test_function"); return 0; }
])], [need_underscore=no],[need_underscore=yes])
AC_MSG_RESULT([$need_underscore])

AC_MSG_CHECKING([[if we need a __unix__ define]])
AC_COMPILE_IFELSE([AC_LANG_SOURCE([
  #ifndef __unix__
  # if defined __APPLE__ /* add other known platforms here */
  #   error should have __unix__ but don't
  # endif
  #endif
])], [AC_MSG_RESULT([no])],
[
  AC_MSG_RESULT([yes])
  AC_DEFINE([__unix__], 1,
[Define to 1 if running on a UNIX-like system where __unix__ is missing])
])

have_posix=yes
have_windows=no
have_darwin=no
stack_alignment=4

case "$host" in
  *-*-mingw*)
    have_windows=yes
    have_posix=no
    ;;
  *-*-darwin*)
    have_darwin=yes
    stack_alignment=16
    ;;
esac

AM_CONDITIONAL([TARGET_WINDOWS], [test "$have_windows" = "yes"])

# On Windows, we can optionaly include resources in the executable.
AM_CONDITIONAL([HAS_WINDRES], [test -n "${WINDRES}" -a "$have_windows" = "yes"])

# On Windows, filter out -g from CCASFLAGS.  There are too many symbols in
# swars.s to include debugging information for, which also results in a lot of
# warning messages.
AS_IF([test "$have_windows" = "yes"], [
  CCASFLAGS=`echo $CCASFLAGS | sed -e 's/\(^\|\s\)-g\>//g'`
])

AC_TYPE_OFF_T
AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_HEADERS([unistd.h])

AM_PATH_SDL([1.2.0], [], [
  AC_MSG_ERROR([SDL not found])
])

PKG_CHECK_MODULES([VORBISFILE], [vorbisfile >= 1.2.0], [], [
  AC_MSG_ERROR([vorbisfile >= 1.2.0 not found])
])

PKG_CHECK_MODULES([PNG], [libpng], [], [
  AC_MSG_ERROR([libpng not found])
])

AM_PATH_OPENAL([], [
  AC_MSG_ERROR([OpenAL not found])
])

AC_CHECK_LIB(WildMidi, WildMidi_Init,,
  AC_MSG_ERROR([libwildmidi not found])
)
AC_SUBST(WILDMIDI_LIBS, -lWildMidi)
AC_SUBST(WILDMIDI_CFLAGS, )
AC_DEFINE(ENABLE_WILDMIDI, 1, [Define for wildmidi support])

# Arguments

AC_ARG_ENABLE([debug],
	      AS_HELP_STRING([--enable-debug=@<:@no|yes|full@:>@],
			     [enable debugging options]),
	      [use_debug=$enableval], [use_debug=no])

AC_ARG_WITH([data-path],
	    AS_HELP_STRING(
	      [--with-data-path],
	      [path to game data @<:@DATAROOTDIR/swars@:>@]),
	    [data_path=$withval], [data_path=])
AS_IF([test "$data_path" = "yes"], [
  AC_MSG_ERROR([The data path must be explicitly specified])
])
AS_IF([test -z "$data_path"], [
  eval data_path="$datadir/swars"
  eval data_path="$data_path"
])

AC_DEFINE_UNQUOTED([GAME_DATA_PATH], ["$data_path"],
		   [Define to the path where the game's data resides])

# Variables
AS_IF([test "$have_darwin" = "no"], [
  ASFILTER=""
], [
  ASFILTER="| \$(top_srcdir)/util/asfilter"
])
AC_SUBST([ASFILTER])

# Flags

AC_MSG_CHECKING([whether to enable debugging options])
AS_IF([test "$use_debug" = "yes" || test "$use_debug" = "full"], [
  AC_DEFINE([ENABLE_DEBUG], 1, [Define to 1 if compiling in debug mode])
  CPPFLAGS="$CPPFLAGS -DDEBUG"
  CFLAGS="$CFLAGS -g -O0"
  AS_IF([test "$have_windows" = "no"], [
    CCASFLAGS="$CCASFLAGS -g"
  ], [
    CCASFLAGS="$CCASFLAGS"
  ])
  AS_IF([test "$use_debug" = "full"], [
    MKWFLAGS="$MKWFLAGS -g"
  ])
], [
  AS_IF([test "$use_debug" != "no"], [
    AC_MSG_ERROR([Invalid value specified for --enable-debug])
  ])
])
AC_MSG_RESULT([$use_debug])

AS_IF([test "$have_posix" = "yes"], [
  MKWFLAGS="$MKWFLAGS -r \"\$(top_srcdir)/conf/fnmap_posix.conf\""
])
AS_IF([test "$have_windows" = "yes"], [
  MKWFLAGS="$MKWFLAGS -r \"\$(top_srcdir)/conf/fnmap_windows.conf\""
])
AS_IF([test "$need_underscore" = "yes"], [
  MKWFLAGS="$MKWFLAGS -u"
])
AS_IF([test "$stack_alignment" != "4"], [
  MKWFLAGS="$MKWFLAGS -s$stack_alignment"
])

AS_IF([test "$need_underscore" = "yes"], [
  CPPFLAGS="$CPPFLAGS -DNEED_UNDERSCORE=1"
])

CFLAGS="$CFLAGS -Wall -Wextra -Wno-unused-parameter"
CFLAGS="$CFLAGS -include \"\$(top_builddir)/src/config.h\""

CFLAGS="$CFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

CFLAGS="$CFLAGS $OPENAL_CFLAGS"
LIBS="$LIBS $OPENAL_LIBS"

CFLAGS="$CFLAGS $VORBISFILE_CFLAGS"
LIBS="$LIBS $VORBISFILE_LIBS"

CFLAGS="$CFLAGS $PNG_CFLAGS"
LIBS="$LIBS $PNG_LIBS"

CFLAGS="$CFLAGS $WILDMIDI_CFLAGS"
LIBS="$LIBS $WILDMIDI_LIBS"


AC_SUBST([MKWFLAGS])
AC_SUBST([WINDRES])

AC_CONFIG_FILES([Makefile conf/Makefile lang/Makefile src/Makefile util/Makefile])
AC_OUTPUT

echo
AC_MSG_NOTICE([data path:         $data_path])
