AC_PREREQ([2.69])

AC_INIT(
 [libcthreads],
 [20231025],
 [joachim.metz@gmail.com])

AC_CONFIG_SRCDIR(
 [include/libcthreads.h.in])

AM_INIT_AUTOMAKE([gnu 1.6 tar-ustar])
AM_EXTRA_RECURSIVE_TARGETS([sources splint])

AC_CONFIG_MACRO_DIR([m4])

dnl Check for host type
AC_CANONICAL_HOST

dnl Check for libtool DLL support
LT_INIT([win32-dll])

dnl Checks for programs
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_PROG_MAKE_SET
AC_PROG_INSTALL

dnl Check for libtool
AC_SUBST(LIBTOOL_DEPS)

dnl Check for pkg-config
AC_PATH_PROG(PKGCONFIG,[pkg-config])

dnl Support of internationalization (i18n)
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19])

dnl Check for compiler language support
AC_C_CONST
AC_C_VOLATILE

dnl Check for large file support
AC_SYS_LARGEFILE

dnl Check if shared library support should be disabled
AX_COMMON_CHECK_DISABLE_SHARED_LIBS

dnl Check if WINAPI support should be enabled
AX_COMMON_CHECK_ENABLE_WINAPI

dnl Check for type definitions
AX_TYPES_CHECK_LOCAL

dnl Check if common required headers and functions are available
AX_COMMON_CHECK_LOCAL

dnl Check if libcerror or required headers and functions are available
AX_LIBCERROR_CHECK_ENABLE

dnl Check if libcthreads required headers and functions are available
AX_LIBCTHREADS_CHECK_LOCAL

dnl Check if DLL support is needed
AX_LIBCTHREADS_CHECK_DLL_SUPPORT

dnl Check if tests required headers and functions are available
AX_TESTS_CHECK_LOCAL

dnl Set additional compiler flags
CFLAGS="$CFLAGS -Wall";

dnl Check if requires and build requires should be set in spec file
AS_IF(
 [test "x$ac_cv_libcerror" = xyes],
 [AC_SUBST(
  [libcthreads_spec_requires],
  [Requires:])
 ])

dnl Set the date for the dpkg files
AC_SUBST(
 [DPKG_DATE],
 [`date -R 2> /dev/null`])

dnl Set the date for the spec file
AC_SUBST(
 [SPEC_DATE],
 [`date +"%a %b %e %Y" 2> /dev/null`])

dnl Generate Makefiles
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([include/Makefile])
AC_CONFIG_FILES([common/Makefile])
AC_CONFIG_FILES([libcerror/Makefile])
AC_CONFIG_FILES([libcthreads/Makefile])
AC_CONFIG_FILES([po/Makefile.in])
AC_CONFIG_FILES([po/Makevars])
AC_CONFIG_FILES([manuals/Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([msvscpp/Makefile])
dnl Generate header files
AC_CONFIG_FILES([include/libcthreads.h])
AC_CONFIG_FILES([include/libcthreads/definitions.h])
AC_CONFIG_FILES([include/libcthreads/features.h])
AC_CONFIG_FILES([include/libcthreads/types.h])
AC_CONFIG_FILES([libcthreads/libcthreads_definitions.h])
dnl Generate distribution specific files
AC_CONFIG_FILES([common/types.h])
AC_CONFIG_FILES([dpkg/changelog])
AC_CONFIG_FILES([libcthreads/libcthreads.rc])
AC_CONFIG_FILES([libcthreads.pc])
AC_CONFIG_FILES([libcthreads.spec])
dnl Generate a source configuration file
AC_CONFIG_HEADERS([common/config.h])

AC_OUTPUT

dnl Print a summary
AC_MSG_NOTICE([
Building:
   libcerror support:       $ac_cv_libcerror

Features:
   Multi-threading support: $ac_cv_libcthreads_multi_threading
]);

