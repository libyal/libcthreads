dnl Function to detect if libcthreads dependencies are available
AC_DEFUN([AX_LIBCTHREADS_CHECK_LOCAL],
 [AS_IF(
  [test "x$ac_cv_enable_winapi" = xno],
  [dnl Check for enabling pthread support
  AX_PTHREAD_CHECK_ENABLE
   ac_cv_libcthreads_locking=$ac_cv_pthread],
  [ac_cv_libcthreads_locking="winapi"])
 ])

