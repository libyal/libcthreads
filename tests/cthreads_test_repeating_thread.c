/*
 * Library repeating_thread type test program
 *
 * Copyright (C) 2012-2026, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include <errno.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ )
#define __USE_GNU
#include <dlfcn.h>
#undef __USE_GNU
#endif

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )
#define HAVE_CTHREADS_TEST_FUNCTION_HOOK	1
#endif

#include "cthreads_test_libcerror.h"
#include "cthreads_test_libcthreads.h"
#include "cthreads_test_macros.h"
#include "cthreads_test_memory.h"
#include "cthreads_test_unused.h"

#include "../libcthreads/libcthreads_repeating_thread.h"

LIBCTHREADS_EXTERN \
int libcthreads_repeating_thread_create(
     libcthreads_repeating_thread_t **repeating_thread,
     const libcthreads_thread_attributes_t *thread_attributes,
     int (*callback_function)(
            void *arguments ),
     void *callback_function_arguments,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_repeating_thread_push(
     libcthreads_repeating_thread_t *repeating_thread,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_repeating_thread_join(
     libcthreads_repeating_thread_t **repeating_thread,
     libcerror_error_t **error );

#if defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK )

static int (*cthreads_test_real_pthread_thread_create)(pthread_t *, const pthread_attr_t *, void *(*)(void *), void *) = NULL;
int cthreads_test_pthread_thread_create_attempts_before_fail                                                           = -1;
int cthreads_test_real_pthread_thread_create_function_return_value                                                     = EBUSY;

/* Custom pthread_thread_create for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_thread_create(
     pthread_t *thread,
     const pthread_attr_t *attr,
     void *(*start_routine)(void *),
     void **arg )
{
	int result = 0;

	if( cthreads_test_real_pthread_thread_create == NULL )
	{
		cthreads_test_real_pthread_thread_create = dlsym(
		                                            RTLD_NEXT,
		                                            "pthread_thread_create" );

		if( cthreads_test_real_pthread_thread_create == NULL )
		{
			return( EBUSY );
		}
	}
	if( cthreads_test_pthread_thread_create_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_thread_create_attempts_before_fail = -1;

		return( cthreads_test_real_pthread_thread_create_function_return_value );
	}
	else if( cthreads_test_pthread_thread_create_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_thread_create_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_thread_create(
	          thread,
	          attr,
	          start_routine,
	          arg );

	return( result );
}

static int (*cthreads_test_real_pthread_thread_join)(pthread_t, void **) = NULL;
int cthreads_test_pthread_thread_join_attempts_before_fail               = -1;
int cthreads_test_real_pthread_thread_join_function_return_value         = EBUSY;

/* Custom pthread_thread_join for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_thread_join(
     pthread_t thread,
     void **retval )
{
	int result = 0;

	if( cthreads_test_real_pthread_thread_join == NULL )
	{
		cthreads_test_real_pthread_thread_join = dlsym(
		                                          RTLD_NEXT,
		                                          "pthread_thread_join" );

		if( cthreads_test_real_pthread_thread_join == NULL )
		{
			return( EBUSY );
		}
	}
	if( cthreads_test_pthread_thread_join_attempts_before_fail == 0 )
	{
		/* Join the thread otherwise it can enter a nondeterministic state
		 */
		cthreads_test_real_pthread_thread_join(
		 thread,
		 retval );

		cthreads_test_pthread_thread_join_attempts_before_fail = -1;

		return( cthreads_test_real_pthread_thread_join_function_return_value );
	}
	else if( cthreads_test_pthread_thread_join_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_thread_join_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_thread_join(
	          thread,
	          retval );

	return( result );
}


static int (*cthreads_test_real_pthread_mutex_lock)(pthread_mutex_t *) = NULL;
int cthreads_test_pthread_mutex_lock_attempts_before_fail              = -1;

/* Custom pthread_mutex_lock for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_mutex_lock(
     pthread_mutex_t *mutex )
{
	int result = 0;

	if( cthreads_test_real_pthread_mutex_lock == NULL )
	{
		cthreads_test_real_pthread_mutex_lock = dlsym(
		                                         RTLD_NEXT,
		                                         "pthread_mutex_lock" );

		if( cthreads_test_real_pthread_mutex_lock == NULL )
		{
			return( EBUSY );
		}
	}
	if( cthreads_test_pthread_mutex_lock_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_mutex_lock_attempts_before_fail = -1;

		return( EBUSY );
	}
	else if( cthreads_test_pthread_mutex_lock_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_mutex_lock_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_mutex_lock(
	          mutex );

	return( result );
}

static int (*cthreads_test_real_pthread_mutex_unlock)(pthread_mutex_t *) = NULL;
int cthreads_test_pthread_mutex_unlock_attempts_before_fail              = -1;

/* Custom pthread_mutex_unlock for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_mutex_unlock(
     pthread_mutex_t *mutex )
{
	int result = 0;

	if( cthreads_test_real_pthread_mutex_unlock == NULL )
	{
		cthreads_test_real_pthread_mutex_unlock = dlsym(
		                                           RTLD_NEXT,
		                                           "pthread_mutex_unlock" );

		if( cthreads_test_real_pthread_mutex_unlock == NULL )
		{
			return( EBUSY );
		}
	}
	if( cthreads_test_pthread_mutex_unlock_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_mutex_unlock_attempts_before_fail = -1;

		return( EBUSY );
	}
	else if( cthreads_test_pthread_mutex_unlock_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_mutex_unlock_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_mutex_unlock(
	          mutex );

	return( result );
}

static int (*cthreads_test_real_pthread_cond_signal)(pthread_cond_t *) = NULL;
int cthreads_test_pthread_cond_signal_attempts_before_fail             = -1;

/* Custom pthread_cond_signal for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_cond_signal(
     pthread_cond_t *cond )
{
	int result = 0;

	if( cthreads_test_real_pthread_cond_signal == NULL )
	{
		cthreads_test_real_pthread_cond_signal = dlsym(
		                                          RTLD_NEXT,
		                                          "pthread_cond_signal" );

		if( cthreads_test_real_pthread_cond_signal == NULL )
		{
			return( EBUSY );
		}
	}
	if( cthreads_test_pthread_cond_signal_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_cond_signal_attempts_before_fail = -1;

		return( EBUSY );
	}
	else if( cthreads_test_pthread_cond_signal_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_cond_signal_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_cond_signal(
	          cond );

	return( result );
}

#endif /* defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK ) */

int cthreads_test_locked_value = 0;

/* The repeating callback function
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_repeating_thread_callback_function(
      void *arguments CTHREADS_TEST_ATTRIBUTE_UNUSED )
{
	CTHREADS_TEST_UNREFERENCED_PARAMETER( arguments )

	cthreads_test_locked_value += 1;

	return( 1 );
}

/* The repeating callback function that returns -1
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_repeating_thread_error_callback_function(
      void *arguments CTHREADS_TEST_ATTRIBUTE_UNUSED )
{
	CTHREADS_TEST_UNREFERENCED_PARAMETER( arguments )

	return( -1 );
}

/* Tests the libcthreads_repeating_thread_create function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_repeating_thread_create(
      void )
{
	libcerror_error_t *error                         = NULL;
	libcthreads_repeating_thread_t *repeating_thread = NULL;
	int result                                       = 0;

	/* Test regular cases
	 */
	result = libcthreads_repeating_thread_create(
	          &repeating_thread,
	          NULL,
	          &cthreads_test_repeating_thread_callback_function,
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "repeating_thread",
	 repeating_thread );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_repeating_thread_join(
	          &repeating_thread,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "repeating_thread",
	 repeating_thread );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libcthreads_repeating_thread_create(
	          NULL,
	          NULL,
	          &cthreads_test_repeating_thread_callback_function,
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

	result = libcthreads_repeating_thread_create(
	          &repeating_thread,
	          NULL,
	          NULL,
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

	repeating_thread = (libcthreads_repeating_thread_t *) 0x12345678UL;

	result = libcthreads_repeating_thread_create(
	          &repeating_thread,
	          NULL,
	          &cthreads_test_repeating_thread_callback_function,
	          NULL,
	          &error );

	repeating_thread = NULL;

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_CTHREADS_TEST_MEMORY )

	/* Test libcthreads_repeating_thread_create with malloc failing
	 */
	cthreads_test_malloc_attempts_before_fail = 0;

	result = libcthreads_repeating_thread_create(
	          &repeating_thread,
	          NULL,
	          &cthreads_test_repeating_thread_callback_function,
	          NULL,
	          &error );

	if( cthreads_test_malloc_attempts_before_fail != -1 )
	{
		cthreads_test_malloc_attempts_before_fail = -1;

		if( repeating_thread != NULL )
		{
			libcthreads_repeating_thread_join(
			 &repeating_thread,
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
		 "repeating_thread",
		 repeating_thread );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}

#if defined( OPTIMIZATION_DISABLED )

	/* This test is conditionally enabled because implementations of
	 * pthread_create will use a non-optimized memset and will return
	 * success.
	 */

	/* Test libcthreads_repeating_thread_create with memset failing
	 */
	cthreads_test_memset_attempts_before_fail = 0;

	result = libcthreads_repeating_thread_create(
	          &repeating_thread,
	          NULL,
	          &cthreads_test_repeating_thread_callback_function,
	          NULL,
	          &error );

	if( cthreads_test_memset_attempts_before_fail != -1 )
	{
		cthreads_test_memset_attempts_before_fail = -1;

		if( repeating_thread != NULL )
		{
			libcthreads_repeating_thread_join(
			 &repeating_thread,
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
		 "repeating_thread",
		 repeating_thread );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_CTHREADS_TEST_MEMORY ) */

#if defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK )

	/* Test libcthreads_repeating_thread_create with pthread_create returning EAGAIN
	 */
	cthreads_test_pthread_thread_create_attempts_before_fail       = 0;
	cthreads_test_real_pthread_thread_create_function_return_value = EAGAIN;

	result = libcthreads_repeating_thread_create(
	          &repeating_thread,
	          NULL,
	          &cthreads_test_repeating_thread_callback_function,
	          NULL,
	          &error );

	if( cthreads_test_pthread_thread_create_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_thread_create_attempts_before_fail = -1;

		if( repeating_thread != NULL )
		{
			libcthreads_repeating_thread_join(
			 &repeating_thread,
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
		 "repeating_thread",
		 repeating_thread );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}

	/* Test libcthreads_repeating_thread_create with pthread_create returning EBUSY
	 */
	cthreads_test_pthread_thread_create_attempts_before_fail       = 0;
	cthreads_test_real_pthread_thread_create_function_return_value = EBUSY;

	result = libcthreads_repeating_thread_create(
	          &repeating_thread,
	          NULL,
	          &cthreads_test_repeating_thread_callback_function,
	          NULL,
	          &error );

	if( cthreads_test_pthread_thread_create_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_thread_create_attempts_before_fail = -1;

		if( repeating_thread != NULL )
		{
			libcthreads_repeating_thread_join(
			 &repeating_thread,
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
		 "repeating_thread",
		 repeating_thread );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( repeating_thread != NULL )
	{
		libcthreads_repeating_thread_join(
		 &repeating_thread,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_repeating_thread_push function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_repeating_thread_push(
      void )
{
	libcerror_error_t *error                          = NULL;
	libcthreads_repeating_thread_t *repeating_thread  = NULL;
	libcthreads_repeating_thread_t *repeating_thread2 = NULL;
	int result                                        = 0;

	/* Test regular cases
	 */
	cthreads_test_locked_value = 0;

	result = libcthreads_repeating_thread_create(
	          &repeating_thread,
	          NULL,
	          &cthreads_test_repeating_thread_callback_function,
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "repeating_thread",
	 repeating_thread );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_repeating_thread_push(
	          repeating_thread,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_repeating_thread_push(
	          repeating_thread,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_repeating_thread_join(
	          &repeating_thread,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "repeating_thread",
	 repeating_thread );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

        CTHREADS_TEST_ASSERT_EQUAL_INT(
         "cthreads_test_locked_value",
         cthreads_test_locked_value,
         2 );

	/* Test error cases
	 */
	result = libcthreads_repeating_thread_push(
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

#if defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK )

	/* Create a dedicated thread so the push error paths do not interfere
	 * with the test thread created above
	 */
	result = libcthreads_repeating_thread_create(
	          &repeating_thread2,
	          NULL,
	          &cthreads_test_repeating_thread_callback_function,
	          NULL,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "repeating_thread2",
	 repeating_thread2 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test libcthreads_repeating_thread_push with pthread_mutex_lock failing in libcthreads_mutex_grab */
	cthreads_test_pthread_mutex_lock_attempts_before_fail = 0;

	result = libcthreads_repeating_thread_push(
	          repeating_thread2,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "cthreads_test_pthread_mutex_lock_attempts_before_fail",
	 cthreads_test_pthread_mutex_lock_attempts_before_fail,
	 -1 );

	if( cthreads_test_pthread_mutex_lock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_lock_attempts_before_fail = -1;
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

	/* Test libcthreads_repeating_thread_push with pthread_cond_signal failing in libcthreads_condition_signal */
	cthreads_test_pthread_cond_signal_attempts_before_fail = 0;

	result = libcthreads_repeating_thread_push(
	          repeating_thread2,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "cthreads_test_pthread_cond_signal_attempts_before_fail",
	 cthreads_test_pthread_cond_signal_attempts_before_fail,
	 -1 );

	if( cthreads_test_pthread_cond_signal_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_cond_signal_attempts_before_fail = -1;
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

	/* Test libcthreads_repeating_thread_push with pthread_mutex_unlock failing in libcthreads_mutex_release */
	cthreads_test_pthread_mutex_unlock_attempts_before_fail = 0;

	result = libcthreads_repeating_thread_push(
	          repeating_thread2,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "cthreads_test_pthread_mutex_unlock_attempts_before_fail",
	 cthreads_test_pthread_mutex_unlock_attempts_before_fail,
	 -1 );

	if( cthreads_test_pthread_mutex_unlock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_unlock_attempts_before_fail = -1;
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

		/* Manually release the mutex otherwise a worker thread will block on the held mutex
		 */
		result = libcthreads_mutex_release(
		           ( ( libcthreads_internal_repeating_thread_t * ) repeating_thread2 )->condition_mutex,
		          &error );

		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}

	/* Join the dedicated thread so its worker exits cleanly
	 */
	result = libcthreads_repeating_thread_join(
	          &repeating_thread2,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "repeating_thread2",
	 repeating_thread2 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( repeating_thread != NULL )
	{
		libcthreads_repeating_thread_join(
		 &repeating_thread,
		 NULL );
	}
	if( repeating_thread2 != NULL )
	{
		libcthreads_repeating_thread_join(
		 &repeating_thread2,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_repeating_thread_join function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_repeating_thread_join(
      void )
{
	libcerror_error_t *error                         = NULL;
	libcthreads_repeating_thread_t *repeating_thread = NULL;
	int result                                       = 0;

	/* Test error cases
	 */
	result = libcthreads_repeating_thread_join(
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

	repeating_thread = NULL;

	result = libcthreads_repeating_thread_join(
	          &repeating_thread,
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

#if defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK )

	/* Initialize test
	 */
	result = libcthreads_repeating_thread_create(
	          &repeating_thread,
	          NULL,
	          &cthreads_test_repeating_thread_callback_function,
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test libcthreads_repeating_thread_join with pthread_join returning EDEADLK
	 */
	cthreads_test_pthread_thread_join_attempts_before_fail       = 0;
	cthreads_test_real_pthread_thread_join_function_return_value = EDEADLK;

	result = libcthreads_repeating_thread_join(
	          &repeating_thread,
	          &error );

	if( cthreads_test_pthread_thread_join_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_thread_join_attempts_before_fail = -1;

		if( repeating_thread != NULL )
		{
			libcthreads_repeating_thread_join(
			 &repeating_thread,
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
		 "repeating_thread",
		 repeating_thread );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Initialize test
	 */
	result = libcthreads_repeating_thread_create(
	          &repeating_thread,
	          NULL,
	          &cthreads_test_repeating_thread_callback_function,
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test libcthreads_repeating_thread_join with pthread_join returning EBUSY
	 */
	cthreads_test_pthread_thread_join_attempts_before_fail       = 0;
	cthreads_test_real_pthread_thread_join_function_return_value = EBUSY;

	result = libcthreads_repeating_thread_join(
	          &repeating_thread,
	          &error );

	if( cthreads_test_pthread_thread_join_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_thread_join_attempts_before_fail = -1;

		if( repeating_thread != NULL )
		{
			libcthreads_repeating_thread_join(
			 &repeating_thread,
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
		 "repeating_thread",
		 repeating_thread );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK ) */

	/* The following test is flaky when using the WINAPI functions
	 */
#if !defined( WINAPI )

	/* Initialize test
	 */
	result = libcthreads_repeating_thread_create(
	          &repeating_thread,
	          NULL,
	          &cthreads_test_repeating_thread_error_callback_function,
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_repeating_thread_push(
	          repeating_thread,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Allow the callback function to run
	 */
	sleep( 0.3 );

	/* Test libcthreads_repeating_thread_join with the callback function returning -1
	 */
	result = libcthreads_repeating_thread_join(
	          &repeating_thread,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "repeating_thread",
	 repeating_thread );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#endif /* !defined( WINAPI ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( repeating_thread != NULL )
	{
		libcthreads_repeating_thread_join(
		 &repeating_thread,
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
	 "libcthreads_repeating_thread_create",
	 cthreads_test_repeating_thread_create );

	CTHREADS_TEST_RUN(
	 "libcthreads_repeating_thread_push",
	 cthreads_test_repeating_thread_push );

	CTHREADS_TEST_RUN(
	 "libcthreads_repeating_thread_join",
	 cthreads_test_repeating_thread_join );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}
