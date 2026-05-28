dnl Checks for required headers and functions
dnl
dnl Version: 20240513

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

dnl Function to check if pthread_rwlock_unlock can be hooked for testing
AC_DEFUN([AX_TESTS_CHECK_PTHREAD_RWLOCK_UNLOCK_HOOK],
  [AC_MSG_CHECKING([whether pthread_rwlock_unlock can be hooked for testing])

  SAVE_LIBS="$LIBS"
  LIBS="-ldl -lpthread $LIBS"

  AC_RUN_IFELSE(
    [AC_LANG_PROGRAM(
      [[#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#define __USE_GNU
#include <dlfcn.h>
#undef __USE_GNU]],
      [[static int (*hooked_pthread_rwlock_unlock)(pthread_rwlock_t *) = NULL;
hooked_pthread_rwlock_unlock = dlsym( RTLD_NEXT, "pthread_rwlock_unlock" );
if( hooked_pthread_rwlock_unlock == NULL ) { return 1; }
pthread_rwlock_t rwlock;
if( pthread_rwlock_init( &rwlock, NULL ) != 0 ) { return 2; }
pthread_rwlock_wrlock( &rwlock );
if( hooked_pthread_rwlock_unlock( &rwlock ) != 0 ) { return 3; }
if( pthread_rwlock_destroy( &rwlock ) != 0 ) { return 4; }]] )],
      [ac_cv_have_pthread_rwlock_unlock_hook=yes],
      [ac_cv_have_pthread_rwlock_unlock_hook=no],
      [ac_cv_have_pthread_rwlock_unlock_hook=undetermined])

  LIBS="$SAVE_LIBS"

  AC_MSG_RESULT(
    [$ac_cv_have_pthread_rwlock_unlock_hook])

  AS_IF(
    [test "x$ac_cv_have_pthread_rwlock_unlock_hook" = xyes],
    [AC_DEFINE(
      [HAVE_PTHREAD_RWLOCK_UNLOCK_HOOK],
      [1],
      [Define to 1 whether pthread_rwlock_unlock can be hooked for testing.])
    ])
  ])

