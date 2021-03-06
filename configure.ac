# Process this file with autoconf to produce a configure script.

AC_INIT([ewl], [0.5.3.050], [enlightenment-devel@lists.sourceforge.net])
AC_PREREQ([2.52])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST

AM_INIT_AUTOMAKE(1.6 dist-bzip2)
AM_CONFIG_HEADER(ewl-config.h)

# AC_PROG_CC sets automatically CFLAGS to -g -O2
# if CFLAGS is empty and gcc is used. We just set
# a variable to know if CFLAGS is empty or not.
AC_MSG_CHECKING(whether to use default CFLAGS)
if test  "x${CFLAGS}" = "x" ; then
  _ewl_force_default_cc_options="yes"
else
   _ewl_force_default_cc_options="no"
fi
AC_MSG_RESULT([${_ewl_force_default_cc_options}])

AC_LIBTOOL_WIN32_DLL
define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
AC_PROG_LIBTOOL

# Define these at the bottom as they are not release versions, but API
# revisions numbers.
INTERFACE_CURRENT="1"
INTERFACE_REVISION="0"
INTERFACE_AGE="0"
INTERFACE_VERSION=${INTERFACE_CURRENT}:${INTERFACE_REVISION}:${INTERFACE_AGE}

AC_SUBST(INTERFACE_VERSION)

AC_DEFINE_UNQUOTED(INTERFACE_CURRENT, "$INTERFACE_CURRENT", [Interface current version number])
AC_DEFINE_UNQUOTED(INTERFACE_REVISION, "$INTERFACE_REVISION", [Interface revision number])
AC_DEFINE_UNQUOTED(INTERFACE_AGE, "$INTERFACE_AGE", [Interface age number])

# disable static lib for modules.
AC_SUBST([AM_LIBTOOLFLAGS], [--tag=disable-static])


## Check for programs
AC_ISC_POSIX
AC_PROG_CC
AC_PROG_CC_STDC
PKG_PROG_PKG_CONFIG

## Check for libraries
AC_CHECK_LIB([m], [sqrt])

case "$host_os" in
  mingw*|cegcc)
    PKG_CHECK_MODULES([EVIL], [evil])
    AC_DEFINE(HAVE_EVIL, 1, [Set to 1 if evil package is installed])
    ;;
esac

have_evas="no"
PKG_CHECK_MODULES([EVAS], [evas >= 0.9.9], [have_evas="yes"])
AM_CONDITIONAL(EWL_ENABLE_EVAS, test "x$have_evas" = "xyes")

PKG_CHECK_MODULES([ECORE], [ecore ecore-txt ecore-file])
PKG_CHECK_MODULES([ECORE_EVAS], [ecore-evas])
PKG_CHECK_MODULES([ECORE_INPUT], [ecore-input])
PKG_CHECK_MODULES([EDJE], [edje >= 0.5.0])

requirements="evas ecore edje"

# Check for libraries needed by engines
PKG_CHECK_MODULES([ECORE_X], [ecore-x >= 0.9.9], [have_ecore_x="yes"], [have_ecore_x="no"])

PKG_CHECK_MODULES([ECORE_XCB], [xcb >= 1.0 ecore-x >= 0.9.9], [have_ecore_xcb="yes"], [have_ecore_xcb="no"])

PKG_CHECK_MODULES([ECORE_WIN32], [ecore-win32 >= 0.9.9], [have_ecore_win32="yes"], [have_ecore_win32="no"])

PKG_CHECK_MODULES([ECORE_SDL], [ecore-sdl >= 0.9.9], [have_ecore_sdl="yes"], [have_ecore_sdl="no"])

PKG_CHECK_MODULES([ECORE_FB], [ecore-fb >= 0.9.9], [have_ecore_framebuffer="yes"], [have_ecore_framebuffer="no"])

# Check for optional libraries

PKG_CHECK_MODULES([EMOTION], [emotion >= 0.0.1], [have_emotion="yes"], [have_emotion="no"])
PKG_CHECK_MODULES([EFREET], [efreet >= 0.0.3 efreet-mime], [have_efreet="yes"], [have_efreet="no"])
PKG_CHECK_MODULES([EPSILON], [epsilon >= 0.3.0], [have_epsilon="yes"], [have_epsilon="no"])


## Check for header files
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h fnmatch.h grp.h langinfo.h limits.h locale.h pwd.h stddef.h])

## Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_STRUCT_TM

AC_C_CONST
AC_C___ATTRIBUTE__

# Use -Wall if we have gcc.
if test "x$GCC" = "xyes" ; then
  ewl_default_cflags="-Wall"
fi

# Cross compiling
WIN32_CFLAGS=""
case "$host_os" in
  cegcc*)
    WIN32_CFLAGS="-mwin32 -mnop-fun-dllimport"
    ;;
  mingw*)
    WIN32_CFLAGS="-mnop-fun-dllimport"
    ;;
esac
AC_SUBST(WIN32_CFLAGS)


## Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_FUNC_STRCOLL
AC_CHECK_FUNCS([dup2 memmove memset nl_langinfo setenv setlocale sqrt strcasecmp strchr strdup strncasecmp strrchr strstr])

fnmatch_libs=""
AC_CHECK_FUNCS(fnmatch, res=yes, res=no)
if test "x$res" = "xno"; then
	AC_SEARCH_LIBS(fnmatch, fnmatch evil iberty, res=yes, res=no)
	if test "x$res" = "xno"; then
		AC_MSG_ERROR([Cannot find fnmatch() in neither libc nor libfnmatch, nor libevil, nor libiberty])
	fi
fi

AC_SUBST(fnmatch_libs)

## Set linker flags
lt_no_undefined=""
lt_enable_auto_import=""
case "$host_os" in
  mingw*|cegcc)
    lt_enable_auto_import="-Wl,--enable-auto-import"
    lt_no_undefined="-no-undefined"
    ;;
esac
AC_SUBST(lt_no_undefined)
AC_SUBST(lt_enable_auto_import)



have_locking="no"
CPPFLAGS_save=${CPPFLAGS}
LDFLAGS_save=${LDLAGS}
CPPFLAGS="${CPPFLAGS} ${EVIL_CFLAGS}"
LDFLAGS="${LDFLAGS} ${EVIL_LIBS}"
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM(
     [[
#include <stdlib.h>
#if HAVE_FCNTL_H
# include <fcntl.h>
#endif
#if HAVE_EVIL
# include <Evil.h>
#endif
     ]],
     [[
fcntl (1, F_SETLKW, NULL);
     ]]
   )
  ],
  [AC_DEFINE(HAVE_FCNTL, 1, [Define to 1 if fcntl() is available])
   have_locking="yes"]
)
CPPFLAGS=${CPPFLAGS_save}
LDFLAGS=${LDFLAGS_save}

if test "x${have_locking}" = "xno"; then
  AC_MSG_ERROR([Error: fcntl() not available.])
fi

## Engines

# Buffer engine
EWL_CHECK_ENGINE([software_buffer], [buffer], [0.9.9], ["yes"])

# X11 engines
EWL_CHECK_ENGINE([software_x11], [software X11], [0.9.9], [$have_ecore_x])

have_software_xlib="no"
have_software_xcb="no"
if test "x$have_software_x11" = "xyes" ; then
   have_software_xlib=`${PKG_CONFIG} --variable=Xlib evas-software-x11`
   if test "x$have_software_xlib" = "xyes" ; then
      AC_DEFINE(HAVE_EVAS_SOFTWARE_XLIB, 1, [Evas Software Xlib Engine Support])
   fi
   have_software_xcb=`${PKG_CONFIG} --variable=XCB evas-software-x11`
   if test "x$have_software_xcb" = "xyes" ; then
      AC_DEFINE(HAVE_EVAS_SOFTWARE_XCB, 1, [Evas Software Xlib Engine Support])
   fi
fi
AM_CONDITIONAL(EWL_ENABLE_EVAS_SOFTWARE_XLIB, [test "x$have_software_xlib" = "xyes"])
AM_CONDITIONAL(EWL_ENABLE_EVAS_SOFTWARE_XCB, [test "x$have_software_xcb" = "xyes"])

EWL_CHECK_ENGINE([software_16_x11], [software 16 bpp X11], [0.9.9], [$have_ecore_x])
EWL_CHECK_ENGINE([xrender_x11], [xrender X11], [0.9.9], [$have_ecore_x])
EWL_CHECK_ENGINE([opengl_x11], [opengl X11], [0.9.9], [$have_ecore_x])

AM_CONDITIONAL(EWL_ENABLE_X11, test "x$have_ecore_x" = "xyes" -a "x$have_software_xlib" = "xyes" -o "x$have_software_16_x11" = "xyes" -o "x$have_xrender_x11" = "xyes" -o "x$have_opengl_x11" = "xyes")
AM_CONDITIONAL(EWL_ENABLE_XCB, test "x$have_ecore_xcb" = "xyes" -a "x$have_software_xcb" = "xyes")

dnl Win32 engines
EWL_CHECK_ENGINE([software_gdi], [software GDI], [0.9.9], [$have_ecore_win32])
EWL_CHECK_ENGINE([software_ddraw], [software DirectDraw], [0.9.9], [$have_ecore_win32])
EWL_CHECK_ENGINE([opengl_glew], [opengl Glew], [0.9.9], [$have_ecore_win32])

AM_CONDITIONAL(EWL_ENABLE_WIN32, test "x$have_ecore_win32" = "xyes" -a "x$have_opengl_glew" = "xyes" -o "x$have_software_gdi" = "xyes" -o "x$have_software_ddraw" = "xyes")

# SDL engine
EWL_CHECK_ENGINE([software_sdl], [software SDL], [0.9.9], [$have_ecore_sdl])

dnl Frame Buffer engine
EWL_CHECK_ENGINE([fb], [framebuffer], [0.9.9], [$have_ecore_framebuffer])

# Set default engine
case "$host_os" in
     mingw*)
        ewl_evas_engine_default="evas_software_gdi"
        ;;
     *)
        ewl_evas_engine_default="evas_software_xcb"
        ;;
esac
AC_SUBST(ewl_evas_engine_default)


## Optional components

if test "x${have_emotion}" = "xyes" ; then
  ewl_media_include="#include <ewl_media.h>"
  requirements="$requirements emotion"
  AC_DEFINE(BUILD_EMOTION_SUPPORT, 1, [Enable Emotion Support for Ewl_Media])
fi
AC_SUBST(ewl_media_include)

if test "x${have_efreet}" = "xyes" ; then
  requirements="$requirements efreet"
  AC_DEFINE(BUILD_EFREET_SUPPORT, 1, [Enable Efreet Support])
fi

if test "x${have_epsilon}" = "xyes" ; then
  requirements="$requirements epsilon"
  AC_DEFINE(BUILD_EPSILON_SUPPORT, 1, [Enable Epsilon Support for Ewl_Image])
fi


## Arguments

# Debug
AC_ARG_ENABLE(debug,
	[AC_HELP_STRING([--enable-debug],[enable debug support])],
	[enable_debug=$enableval], [enable_debug="yes"])
AC_MSG_CHECKING([whether to build in debug mode])
AC_MSG_RESULT(${enable_debug})

if test "x${enable_debug}" = "xyes" ; then
  AC_DEFINE(EWL_ENABLE_DEBUG, 1, [Enable debug support])
else
  AC_DEFINE(EWL_ENABLE_DEBUG, 0, [Enable debug support])
  enable_debug="no"
fi

# Tests
AC_ARG_ENABLE(tests,
	[AC_HELP_STRING([--disable-tests],[disable the build of tests])],
	[enable_tests=$enableval], [enable_tests="yes"])
AC_MSG_CHECKING([whether to build test program])
AC_MSG_RESULT(${enable_tests})

AM_CONDITIONAL(EWL_TESTS_BUILD, test "x${enable_tests}" = "xyes")

# Coverage
AC_ARG_ENABLE(gcov,
  AC_HELP_STRING([--enable-gcov],
  [compile with coverage profiling instrumentation]),
  [enable_gcov=$enableval],
  [enable_gcov="no"]
)
AC_MSG_CHECKING([whether to use profiling instrumentation])
AC_MSG_RESULT(${enable_gcov})

if test "x${enable_gcov}" = "xyes" ; then
  AC_CHECK_PROG(have_lcov, [lcov], [yes], [no])

  if test "x${have_lcov}" = "xyes" ; then
    GCOV_CFLAGS="-fprofile-arcs -ftest-coverage"
    GCOV_LIBS="-lgcov"
  else
    AC_MSG_WARN([lcov is not found, disable profiling instrumentation])
    enable_gcov="no"
  fi
fi
AC_SUBST(GCOV_CFLAGS)
AC_SUBST(GCOV_LIBS)

AM_CONDITIONAL(EWL_ENABLE_GCOV, test "x${enable_gcov}" = "xyes")

# we set now CFLAGS correctly
if test "x$GCC" = "xyes" ; then
  if test "x${enable_debug}" = "xyes" -o "x${enable_gcov}" = "xyes"; then
    ewl_default_cflags="${ewl_default_cflags} -g -O0"
  else
    ewl_default_cflags="${ewl_default_cflags} -O2"
  fi
fi

if test "x${_ewl_force_default_cc_options}" = "xyes" ; then
  CFLAGS="${ewl_default_cflags}"
fi

AC_SUBST(requirements)

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.15])

AC_OUTPUT([
Makefile
ewl.spec
ewl.pc
po/Makefile.in
src/Makefile
src/bin/Makefile
src/bin/tests/Makefile
src/bin/tests/border/Makefile
src/bin/tests/box/Makefile
src/bin/tests/button/Makefile
src/bin/tests/calendar/Makefile
src/bin/tests/callback/Makefile
src/bin/tests/check/Makefile
src/bin/tests/colordialog/Makefile
src/bin/tests/colorpicker/Makefile
src/bin/tests/combo/Makefile
src/bin/tests/config/Makefile
src/bin/tests/container/Makefile
src/bin/tests/datepicker/Makefile
src/bin/tests/dialog/Makefile
src/bin/tests/dnd_snoop/Makefile
src/bin/tests/engine/Makefile
src/bin/tests/entry/Makefile
src/bin/tests/filedialog/Makefile
src/bin/tests/filepicker/Makefile
src/bin/tests/floater/Makefile
src/bin/tests/freebox/Makefile
src/bin/tests/freebox_mvc/Makefile
src/bin/tests/fullscreen/Makefile
src/bin/tests/grid/Makefile
src/bin/tests/histogram/Makefile
src/bin/tests/icon/Makefile
src/bin/tests/icon_theme/Makefile
src/bin/tests/image/Makefile
src/bin/tests/image_thumbnail/Makefile
src/bin/tests/io_manager/Makefile
src/bin/tests/kinetic_scrollpane/Makefile
src/bin/tests/label/Makefile
src/bin/tests/layer/Makefile
src/bin/tests/list/Makefile
src/bin/tests/list2/Makefile
src/bin/tests/media/Makefile
src/bin/tests/menu/Makefile
src/bin/tests/menubar/Makefile
src/bin/tests/modal/Makefile
src/bin/tests/mvc/Makefile
src/bin/tests/notebook/Makefile
src/bin/tests/object/Makefile
src/bin/tests/range/Makefile
src/bin/tests/reparent/Makefile
src/bin/tests/paned/Makefile
src/bin/tests/password/Makefile
src/bin/tests/pointer/Makefile
src/bin/tests/progressbar/Makefile
src/bin/tests/puzzle/Makefile
src/bin/tests/scrollbar/Makefile
src/bin/tests/scrollpane/Makefile
src/bin/tests/seeker/Makefile
src/bin/tests/shadow/Makefile
src/bin/tests/spinner/Makefile
src/bin/tests/statusbar/Makefile
src/bin/tests/table/Makefile
src/bin/tests/text/Makefile
src/bin/tests/text_format/Makefile
src/bin/tests/text_editor/Makefile
src/bin/tests/theme/Makefile
src/bin/tests/toolbar/Makefile
src/bin/tests/tooltip/Makefile
src/bin/tests/tree/Makefile
src/bin/tests/widget/Makefile
src/bin/tests/window/Makefile
src/lib/Makefile
src/engines/Makefile
src/plugins/Makefile
src/engines/x11/Makefile
src/engines/xcb/Makefile
src/engines/evas/Makefile
src/engines/evas_software_xlib/Makefile
src/engines/evas_software_xcb/Makefile
src/engines/evas_software_sdl/Makefile
src/engines/evas_software_16_x11/Makefile
src/engines/evas_xrender_x11/Makefile
src/engines/evas_gl_x11/Makefile
src/engines/evas_fb/Makefile
src/engines/evas_buffer/Makefile
src/engines/win32/Makefile
src/engines/evas_software_gdi/Makefile
src/engines/evas_gl_glew/Makefile
src/engines/evas_software_ddraw/Makefile
data/Makefile
data/config/Makefile
data/config/ewl.cfg
data/images/Makefile
data/themes/Makefile
data/themes/b_and_w/Makefile
data/themes/b_and_w/images/Makefile
data/themes/b_and_w/groups/Makefile
data/themes/e17/Makefile
data/themes/e17/fonts/Makefile
data/themes/e17/images/Makefile
data/themes/e17/groups/Makefile
data/themes/e17/groups/inc/Makefile
data/themes/ewl_embed_test/Makefile
data/themes/ewl_embed_test/images/Makefile
tools/Makefile
tools/ewl_config/Makefile
doc/Makefile
doc/tutorials/Makefile
doc/tutorials/widgets/Makefile
])

echo
echo "${PACKAGE} ${VERSION}"
echo
echo "Engines:"
echo
echo "  Buffer...................................: ${have_software_buffer}"
echo "  Software X11.............................: ${have_software_x11}"
echo "  XRender X11..............................: ${have_xrender_x11}"
echo "  OpenGL X11...............................: ${have_opengl_x11}"
echo "  Software XCB.............................: ${have_software_xcb}"
echo "  Software GDI.............................: ${have_software_gdi}"
echo "  Software DirectDraw......................: ${have_software_ddraw}"
echo "  OpenGL Glew..............................: ${have_opengl_glew}"
echo "  SDL......................................: ${have_software_sdl}"
echo "  Framebuffer..............................: ${have_fb}"
echo "  Software 16 X11..........................: ${have_software_16_x11}"
echo
echo "Optional Components:"
echo
echo "  Emotion Support..........................: ${have_emotion}"
echo "  Efreet Support...........................: ${have_efreet}"
echo "  Epsilon Support..........................: ${have_epsilon}"
echo
echo "Debug Support..............................: ${enable_debug}"
echo
echo "Coverage Support...........................: ${enable_gcov}"
echo
echo "Build tests................................: ${enable_tests}"
echo
echo "Now type 'make' ('gmake' on some systems) to compile ${PACKAGE}."
echo
