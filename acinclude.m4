dnl Checks for required headers and functions
dnl
dnl Version: 20190308

dnl Function to detect if libcthreads dependencies are available
AC_DEFUN([AX_LIBCTHREADS_CHECK_LOCAL],
  [AS_IF(
    [test "x$ac_cv_enable_winapi" = xno],
    [dnl Check for enabling pthread support
    AX_PTHREAD_CHECK_ENABLE
      ac_cv_libcthreads_multi_threading=$ac_cv_pthread],
    [ac_cv_libcthreads_multi_threading="winapi"])
  ])

dnl Function to check if DLL support is needed
AC_DEFUN([AX_LIBCTHREADS_CHECK_DLL_SUPPORT],
  [AS_IF(
    [test "x$enable_shared" = xyes],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw* | *msys*],
      [AC_DEFINE(
        [HAVE_DLLMAIN],
        [1],
        [Define to 1 to enable the DllMain function.])
      AC_SUBST(
        [HAVE_DLLMAIN],
        [1])

      AC_SUBST(
        [LIBCTHREADS_DLL_EXPORT],
        ["-DLIBCTHREADS_DLL_EXPORT"])

      AC_SUBST(
        [LIBCTHREADS_DLL_IMPORT],
        ["-DLIBCTHREADS_DLL_IMPORT"])
      ])
    ])
  ])

