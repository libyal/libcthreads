/*
 * Library read/write lock type test program
 *
 * Copyright (C) 2012-2017, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <errno.h>

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )
#define __USE_GNU
#include <dlfcn.h>
#undef __USE_GNU
#endif

#include "cthreads_test_libcerror.h"
#include "cthreads_test_libcthreads.h"
#include "cthreads_test_macros.h"
#include "cthreads_test_memory.h"
#include "cthreads_test_unused.h"

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )

static int (*cthreads_test_real_pthread_rwlock_init)(pthread_rwlock_t *, const pthread_rwlockattr_t *) = NULL;
static int (*cthreads_test_real_pthread_rwlock_destroy)(pthread_rwlock_t *)                            = NULL;
static int (*cthreads_test_real_pthread_rwlock_lock)(pthread_rwlock_t *)                               = NULL;
static int (*cthreads_test_real_pthread_rwlock_unlock)(pthread_rwlock_t *)                             = NULL;

int cthreads_test_pthread_rwlock_init_attempts_before_fail                                             = -1;
int cthreads_test_pthread_rwlock_destroy_attempts_before_fail                                          = -1;
int cthreads_test_pthread_rwlock_lock_attempts_before_fail                                             = -1;
int cthreads_test_pthread_rwlock_unlock_attempts_before_fail                                           = -1;

#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) */

libcthreads_read_write_lock_t *cthreads_test_read_write_lock = NULL;
int cthreads_test_locked_value                               = 0;

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )

/* Custom pthread_rwlock_init for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_rwlock_init(
     pthread_rwlock_t *rwlock,
     const pthread_rwlockattr_t *attr )
{
	int result = 0;

	if( cthreads_test_real_pthread_rwlock_init == NULL )
	{
		cthreads_test_real_pthread_rwlock_init = dlsym(
		                                          RTLD_NEXT,
		                                          "pthread_rwlock_init" );
	}
	if( cthreads_test_pthread_rwlock_init_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_rwlock_init_attempts_before_fail = -1;

		return( EBUSY );
	}
	else if( cthreads_test_pthread_rwlock_init_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_rwlock_init_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_rwlock_init(
	          rwlock,
	          attr );

	return( result );
}

/* Custom pthread_rwlock_destroy for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_rwlock_destroy(
     pthread_rwlock_t *rwlock )
{
	int result = 0;

	if( cthreads_test_real_pthread_rwlock_destroy == NULL )
	{
		cthreads_test_real_pthread_rwlock_destroy = dlsym(
		                                             RTLD_NEXT,
		                                             "pthread_rwlock_destroy" );
	}
	if( cthreads_test_pthread_rwlock_destroy_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_rwlock_destroy_attempts_before_fail = -1;

		return( EBUSY );
	}
	else if( cthreads_test_pthread_rwlock_destroy_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_rwlock_destroy_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_rwlock_destroy(
	          rwlock );

	return( result );
}

/* Custom pthread_rwlock_lock for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_rwlock_lock(
     pthread_rwlock_t *rwlock )
{
	int result = 0;

	if( cthreads_test_real_pthread_rwlock_lock == NULL )
	{
		cthreads_test_real_pthread_rwlock_lock = dlsym(
		                                          RTLD_NEXT,
		                                          "pthread_rwlock_lock" );
	}
	if( cthreads_test_pthread_rwlock_lock_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_rwlock_lock_attempts_before_fail = -1;

		return( EBUSY );
	}
	else if( cthreads_test_pthread_rwlock_lock_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_rwlock_lock_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_rwlock_lock(
	          rwlock );

	return( result );
}

/* Custom pthread_rwlock_unlock for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_rwlock_unlock(
     pthread_rwlock_t *rwlock )
{
	int result = 0;

	if( cthreads_test_real_pthread_rwlock_unlock == NULL )
	{
		cthreads_test_real_pthread_rwlock_unlock = dlsym(
		                                            RTLD_NEXT,
		                                            "pthread_rwlock_unlock" );
	}
	if( cthreads_test_pthread_rwlock_unlock_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_rwlock_unlock_attempts_before_fail = -1;

		return( EBUSY );
	}
	else if( cthreads_test_pthread_rwlock_unlock_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_rwlock_unlock_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_rwlock_unlock(
	          rwlock );

	return( result );
}

#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) */

/* The thread1 callback function
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_read_write_lock_callback_function1(
     void *arguments CTHREADS_TEST_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_read_write_lock_callback_function1";
	int result               = 0;

	CTHREADS_TEST_UNREFERENCED_PARAMETER( arguments )

	result = libcthreads_read_write_lock_grab_for_write(
	          cthreads_test_read_write_lock,
	          &error );

	if( result != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock.",
		 function );

		goto on_error;
	}
	cthreads_test_locked_value += 19;

	result = libcthreads_read_write_lock_release_for_write(
		  cthreads_test_read_write_lock,
		  &error );

	if( result != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stdout );

		libcerror_error_free(
		 &error );
	}
	return( -1 );
}

/* The thread2 callback function
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_read_write_lock_callback_function2(
     void *arguments CTHREADS_TEST_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_read_write_lock_callback_function2";
	int result               = 0;

	CTHREADS_TEST_UNREFERENCED_PARAMETER( arguments )

	result = libcthreads_read_write_lock_grab_for_write(
	          cthreads_test_read_write_lock,
	          &error );

	if( result != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock.",
		 function );

		goto on_error;
	}
	cthreads_test_locked_value += 38;

	result = libcthreads_read_write_lock_release_for_write(
		  cthreads_test_read_write_lock,
		  &error );

	if( result != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stdout );

		libcerror_error_free(
		 &error );
	}
	return( -1 );
}

/* Tests the libcthreads_read_write_lock_initialize function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_read_write_lock_initialize(
     void )
{
	libcerror_error_t *error                       = NULL;
	libcthreads_read_write_lock_t *read_write_lock = NULL;
	int result                                     = 0;

	/* Test libcthreads_read_write_lock_initialize
	 */
	result = libcthreads_read_write_lock_initialize(
	          &read_write_lock,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NOT_NULL(
         "read_write_lock",
         read_write_lock );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libcthreads_read_write_lock_free(
	          &read_write_lock,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "read_write_lock",
         read_write_lock );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libcthreads_read_write_lock_initialize(
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        CTHREADS_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	read_write_lock = (libcthreads_read_write_lock_t *) 0x12345678UL;

	result = libcthreads_read_write_lock_initialize(
	          &read_write_lock,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        CTHREADS_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	read_write_lock = NULL;

#if defined( HAVE_CTHREADS_TEST_MEMORY )

	/* Test libcthreads_read_write_lock_initialize with malloc failing
	 */
	cthreads_test_malloc_attempts_before_fail = 0;

	result = libcthreads_read_write_lock_initialize(
	          &read_write_lock,
	          &error );

	if( cthreads_test_malloc_attempts_before_fail != -1 )
	{
		cthreads_test_malloc_attempts_before_fail = -1;

		if( read_write_lock != NULL )
		{
			libcthreads_read_write_lock_free(
			 &read_write_lock,
			 NULL );
		}
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "read_write_lock",
		 read_write_lock );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libcthreads_read_write_lock_initialize with memset failing
	 */
	cthreads_test_memset_attempts_before_fail = 0;

	result = libcthreads_read_write_lock_initialize(
	          &read_write_lock,
	          &error );

	if( cthreads_test_memset_attempts_before_fail != -1 )
	{
		cthreads_test_memset_attempts_before_fail = -1;

		if( read_write_lock != NULL )
		{
			libcthreads_read_write_lock_free(
			 &read_write_lock,
			 NULL );
		}
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "read_write_lock",
		 read_write_lock );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_CTHREADS_TEST_MEMORY ) */

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )

	/* Test libcthreads_read_write_lock_initialize with pthread_rwlock_init failing
	 */
	cthreads_test_pthread_rwlock_init_attempts_before_fail = 0;

	result = libcthreads_read_write_lock_initialize(
	          &read_write_lock,
	          &error );

	if( cthreads_test_pthread_rwlock_init_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_rwlock_init_attempts_before_fail = -1;

		if( read_write_lock != NULL )
		{
			libcthreads_read_write_lock_free(
			 &read_write_lock,
			 NULL );
		}
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "read_write_lock",
		 read_write_lock );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( read_write_lock != NULL )
	{
		libcthreads_read_write_lock_free(
		 &read_write_lock,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_read_write_lock_free function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_read_write_lock_free(
     void )
{
	libcerror_error_t *error                       = NULL;
	int result                                     = 0;

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )
	libcthreads_read_write_lock_t *read_write_lock = NULL;
#endif

	/* Test error cases
	 */
	result = libcthreads_read_write_lock_free(
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        CTHREADS_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )

	/* Initialize test
	 */
	result = libcthreads_read_write_lock_initialize(
	          &read_write_lock,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test libcthreads_read_write_lock_free with pthread_rwlock_destroy failing
	 */
	cthreads_test_pthread_rwlock_destroy_attempts_before_fail = 0;

	result = libcthreads_read_write_lock_free(
	          &read_write_lock,
	          &error );

	if( cthreads_test_pthread_rwlock_destroy_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_rwlock_destroy_attempts_before_fail = -1;

		if( read_write_lock != NULL )
		{
			libcthreads_read_write_lock_free(
			 &read_write_lock,
			 NULL );
		}
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "read_write_lock",
		 read_write_lock );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )
	if( read_write_lock != NULL )
	{
		libcthreads_read_write_lock_free(
		 &read_write_lock,
		 NULL );
	}
#endif
	return( 0 );
}

/* Tests the libcthreads_read_write_lock_grab_for_read function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_read_write_lock_grab_for_read(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

/* TODO: add tests */

	/* Test error cases
	 */
	result = libcthreads_read_write_lock_grab_for_read(
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        CTHREADS_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libcthreads_read_write_lock_grab_for_write function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_read_write_lock_grab_for_write(
     void )
{
	libcerror_error_t *error      = NULL;
	libcthreads_thread_t *thread1 = NULL;
	libcthreads_thread_t *thread2 = NULL;
	int result                    = 0;

	/* Initialize test
	 */
	result = libcthreads_read_write_lock_initialize(
	          &cthreads_test_read_write_lock,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test grab
	 */
	result = libcthreads_read_write_lock_grab_for_write(
	          cthreads_test_read_write_lock,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Initialize test
	 */
	result = libcthreads_thread_create(
	          &thread1,
	          NULL,
	          cthreads_test_read_write_lock_callback_function1,
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libcthreads_thread_create(
	          &thread2,
	          NULL,
	          cthreads_test_read_write_lock_callback_function2,
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	cthreads_test_locked_value = 46;

	/* Test release
	 */
	result = libcthreads_read_write_lock_release_for_write(
		  cthreads_test_read_write_lock,
		  &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Clean up
	 */
	result = libcthreads_thread_join(
	          &thread2,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libcthreads_thread_join(
	          &thread1,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "cthreads_test_locked_value",
	 cthreads_test_locked_value,
	 46 + 19 + 38 );

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )

	/* Test libcthreads_read_write_lock_grab with pthread_rwlock_lock failing
	 */
	cthreads_test_pthread_rwlock_lock_attempts_before_fail = 0;

	result = libcthreads_read_write_lock_grab_for_write(
	          cthreads_test_read_write_lock,
	          &error );

	if( cthreads_test_pthread_rwlock_lock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_rwlock_lock_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) */

	/* Clean up
	 */
	result = libcthreads_read_write_lock_free(
	          &cthreads_test_read_write_lock,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );


	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stdout );

		libcerror_error_free(
		 &error );
	}
	if( thread2 != NULL )
	{
		libcthreads_thread_join(
		 &thread2,
		 NULL );
	}
	if( thread1 != NULL )
	{
		libcthreads_thread_join(
		 &thread1,
		 NULL );
	}
	if( cthreads_test_read_write_lock != NULL )
	{
		libcthreads_read_write_lock_release_for_write(
		 cthreads_test_read_write_lock,
		 NULL );
		libcthreads_read_write_lock_free(
		 &cthreads_test_read_write_lock,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_read_write_lock_release_for_read function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_read_write_lock_release_for_read(
     void )
{
	libcerror_error_t *error                       = NULL;
	libcthreads_read_write_lock_t *read_write_lock = NULL;
	int result                                     = 0;

	/* Initialize test
	 */
	result = libcthreads_read_write_lock_initialize(
	          &read_write_lock,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libcthreads_read_write_lock_release_for_read(
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        CTHREADS_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )

	/* Test libcthreads_read_write_lock_release_for_read with pthread_rwlock_unlock failing
	 */
	cthreads_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libcthreads_read_write_lock_release_for_read(
	          read_write_lock,
	          &error );

	if( cthreads_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_rwlock_unlock_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) */

	/* Clean up
	 */
	result = libcthreads_read_write_lock_free(
	          &read_write_lock,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( read_write_lock != NULL )
	{
		libcthreads_read_write_lock_free(
		 &read_write_lock,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_read_write_lock_release_for_write function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_read_write_lock_release_for_write(
     void )
{
	libcerror_error_t *error                       = NULL;
	libcthreads_read_write_lock_t *read_write_lock = NULL;
	int result                                     = 0;

	/* Initialize test
	 */
	result = libcthreads_read_write_lock_initialize(
	          &read_write_lock,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libcthreads_read_write_lock_release_for_write(
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        CTHREADS_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )

	/* Test libcthreads_read_write_lock_release_for_write with pthread_rwlock_unlock failing
	 */
	cthreads_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libcthreads_read_write_lock_release_for_write(
	          read_write_lock,
	          &error );

	if( cthreads_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_rwlock_unlock_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) */

	/* Clean up
	 */
	result = libcthreads_read_write_lock_free(
	          &read_write_lock,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( read_write_lock != NULL )
	{
		libcthreads_read_write_lock_free(
		 &read_write_lock,
		 NULL );
	}
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc CTHREADS_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] CTHREADS_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc CTHREADS_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] CTHREADS_TEST_ATTRIBUTE_UNUSED )
#endif
{
	CTHREADS_TEST_UNREFERENCED_PARAMETER( argc )
	CTHREADS_TEST_UNREFERENCED_PARAMETER( argv )

	CTHREADS_TEST_RUN(
	 "libcthreads_read_write_lock_initialize",
	 cthreads_test_read_write_lock_initialize );

	CTHREADS_TEST_RUN(
	 "libcthreads_read_write_lock_free",
	 cthreads_test_read_write_lock_free );

	CTHREADS_TEST_RUN(
	 "libcthreads_read_write_lock_grab_for_read",
	 cthreads_test_read_write_lock_grab_for_read );

	CTHREADS_TEST_RUN(
	 "libcthreads_read_write_lock_grab_for_write",
	 cthreads_test_read_write_lock_grab_for_write );

	CTHREADS_TEST_RUN(
	 "libcthreads_read_write_lock_release_for_read",
	 cthreads_test_read_write_lock_release_for_read );

	CTHREADS_TEST_RUN(
	 "libcthreads_read_write_lock_release_for_write",
	 cthreads_test_read_write_lock_release_for_write );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

