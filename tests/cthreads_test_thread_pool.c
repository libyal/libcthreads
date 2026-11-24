/*
 * Library thread pool type test program
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#include <errno.h>

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

#include "../libcthreads/libcthreads_thread_pool.h"

#if defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK )

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

static int (*cthreads_test_real_pthread_cond_broadcast)(pthread_cond_t *) = NULL;
int cthreads_test_pthread_cond_broadcast_attempts_before_fail             = -1;

/* Custom pthread_cond_broadcast for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_cond_broadcast(
      pthread_cond_t *cond )
{
	int result = 0;

	if( cthreads_test_real_pthread_cond_broadcast == NULL )
	{
		cthreads_test_real_pthread_cond_broadcast = dlsym(
		                                             RTLD_NEXT,
		                                             "pthread_cond_broadcast" );

		if( cthreads_test_real_pthread_cond_broadcast == NULL )
		{
			return( EBUSY );
		}
	}
	if( cthreads_test_pthread_cond_broadcast_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_cond_broadcast_attempts_before_fail = -1;

		return( EBUSY );
	}
	else if( cthreads_test_pthread_cond_broadcast_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_cond_broadcast_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_cond_broadcast(
	          cond );

	return( result );
}

static int (*cthreads_test_real_pthread_cond_wait)(pthread_cond_t *, pthread_mutex_t *) = NULL;
int cthreads_test_pthread_cond_wait_attempts_before_fail                                = -1;

/* Custom pthread_cond_wait for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_cond_wait(
      pthread_cond_t *cond,
      pthread_mutex_t *mutex )
{
	int result = 0;

	if( cthreads_test_real_pthread_cond_wait == NULL )
	{
		cthreads_test_real_pthread_cond_wait = dlsym(
		                                        RTLD_NEXT,
		                                        "pthread_cond_wait" );

		if( cthreads_test_real_pthread_cond_wait == NULL )
		{
			return( EBUSY );
		}
	}
	if( cthreads_test_pthread_cond_wait_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_cond_wait_attempts_before_fail = -1;

		return( EBUSY );
	}
	else if( cthreads_test_pthread_cond_wait_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_cond_wait_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_cond_wait(
	          cond,
	          mutex );

	return( result );
}

#endif /* defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK ) */

libcthreads_lock_t *cthreads_test_lock  = NULL;
int cthreads_test_expected_queued_value = 0;
int cthreads_test_queued_value          = 0;
int cthreads_test_number_of_iterations  = 497;
int cthreads_test_number_of_values      = 32;

/* The thread pool callback function
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_thread_pool_callback_function(
     intptr_t *value,
     void *arguments CTHREADS_TEST_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_thread_pool_callback_function";
	int result               = 0;

	CTHREADS_TEST_UNREFERENCED_PARAMETER( arguments )

	if( value == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		goto on_error;
	}
	result = libcthreads_lock_grab(
	          cthreads_test_lock,
	          &error );

	if( result != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab lock.",
		 function );

		goto on_error;
	}
	cthreads_test_queued_value += *( (int *) value );

	result = libcthreads_lock_release(
		  cthreads_test_lock,
		  &error );

	if( result != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release lock.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stderr );

		libcerror_error_free(
		 &error );
	}
	return( -1 );
}

/* Waits until the worker thread of a thread pool is parked in the test
 * callback function.
 * The caller must hold the test lock and must have queued a value to the
 * pool that the worker thread has popped, which the worker thread cannot
 * process while the test lock is held.
 *
 * While the test lock is held, the worker thread is blocked in the lock grab
 * of the test callback function once it has popped a value. This function
 * waits until the worker thread reaches that state, so that fail-once test
 * hooks cannot be consumed by the worker thread anymore. Consuming a hook
 * would cause the pop function of the worker thread to fail and the worker
 * thread to exit, leaving the pool without a worker thread.
 */
void cthreads_test_thread_pool_wait_for_worker_parking(
      libcthreads_thread_pool_t *thread_pool )
{
	int result     = 0;
	int wait_count = 0;

	while( ( result = libcthreads_mutex_try_grab(
	          ( ( libcthreads_internal_thread_pool_t * ) thread_pool )->condition_mutex,
	          NULL ) ) != 1 && wait_count < 1000 )
	{
		sleep( 0.001 );

		wait_count++;
	}
	if( result == 1 )
	{
		libcthreads_mutex_release(
		  ( ( libcthreads_internal_thread_pool_t * ) thread_pool )->condition_mutex,
		  NULL );
	}
}

/* Tests the libcthreads_thread_pool_create function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_thread_pool_create(
     void )
{
	libcerror_error_t *error               = NULL;
	libcthreads_thread_pool_t *thread_pool = NULL;
	int result                             = 0;

	/* Test regular cases
	 */
	result = libcthreads_thread_pool_create(
	          &thread_pool,
	          NULL,
	          8,
	          32,
	          &cthreads_test_thread_pool_callback_function,
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "thread_pool",
	 thread_pool );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_thread_pool_join(
	          &thread_pool,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "thread_pool",
	 thread_pool );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libcthreads_thread_pool_create(
	          NULL,
	          NULL,
	          8,
	          32,
	          &cthreads_test_thread_pool_callback_function,
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

	thread_pool = (libcthreads_thread_pool_t *) 0x12345678UL;

	result = libcthreads_thread_pool_create(
	          &thread_pool,
	          NULL,
	          8,
	          32,
	          &cthreads_test_thread_pool_callback_function,
	          NULL,
	          &error );

	thread_pool = NULL;

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );



	/* Test libcththreads_thread_pool_create with an invalid number of threads value
	 */
	result = libcthreads_thread_pool_create(
	          &thread_pool,
	          NULL,
	          0,
	          32,
	          &cthreads_test_thread_pool_callback_function,
	          NULL,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );
	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );
	libcerror_error_free(
	 &error );

	/* Test libcththreads_thread_pool_create with an invalid maximum number of values value
	 */
	result = libcthreads_thread_pool_create(
	          &thread_pool,
	          NULL,
	          8,
	          0,
	          &cthreads_test_thread_pool_callback_function,
	          NULL,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );
	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );
	libcerror_error_free(
	 &error );

	/* Test libcththreads_thread_pool_create with an invalid callback function
	 */
	result = libcthreads_thread_pool_create(
	          &thread_pool,
	          NULL,
	          8,
	          32,
	          NULL,
	          NULL,
	          &error
	);
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
#if defined( OPTIMIZATION_DISABLED )

	/* This test is conditionally enabled because implementations of
	 * pthread_create will use a non-optimized malloc and memset and
	 * will still return success.
	 */

	/* Test libcthreads_thread_pool_create with malloc failing
	 */
	cthreads_test_malloc_attempts_before_fail = 0;

	result = libcthreads_thread_pool_create(
	          &thread_pool,
	          NULL,
	          8,
	          32,
	          &cthreads_test_thread_pool_callback_function,
	          NULL,
	          &error );

	if( cthreads_test_malloc_attempts_before_fail != -1 )
	{
		cthreads_test_malloc_attempts_before_fail = -1;

		if( thread_pool != NULL )
		{
			libcthreads_thread_pool_join(
			 &thread_pool,
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
		 "thread_pool",
		 thread_pool );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libcthreads_thread_pool_create with memset failing
	 */
	cthreads_test_memset_attempts_before_fail = 0;

	result = libcthreads_thread_pool_create(
	          &thread_pool,
	          NULL,
	          8,
	          32,
	          &cthreads_test_thread_pool_callback_function,
	          NULL,
	          &error );

	if( cthreads_test_memset_attempts_before_fail != -1 )
	{
		cthreads_test_memset_attempts_before_fail = -1;

		if( thread_pool != NULL )
		{
			libcthreads_thread_pool_join(
			 &thread_pool,
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
		 "thread_pool",
		 thread_pool );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_CTHREADS_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( thread_pool != NULL )
	{
		libcthreads_thread_pool_join(
		 &thread_pool,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_thread_pool_join function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_thread_pool_join(
     void )
{
	libcerror_error_t *error               = NULL;
	libcthreads_thread_pool_t *thread_pool = NULL;
	int result                             = 0;


	/* Test error cases
	 */
	result = libcthreads_thread_pool_join(
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

	/* Test libcththreads_thread_pool_join with a missing thread pool value
	 */
	result = libcthreads_thread_pool_join(
	          &thread_pool,
	          &error
	);

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

/* Tests thread pool push
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_thread_pool_push(
     void )
{
	libcerror_error_t *error                   = NULL;
	libcthreads_thread_t *thread_pool          = NULL;
	int *queued_values                         = NULL;
	int *fill_values                           = NULL;
	int *dedicated_values                      = NULL;
	intptr_t **values_array                    = NULL;
	libcthreads_thread_pool_t *pool_dedicated1 = NULL;
	libcthreads_thread_pool_t *pool_dedicated2 = NULL;
#if defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK )
	int fill_index                             = 0;
#endif
	int gate_held                              = 0;
	int iterator                               = 0;
	int result                                 = 0;


	cthreads_test_expected_queued_value = 0;
	cthreads_test_queued_value          = 0;

	queued_values = (int *) memory_allocate(
	                         sizeof( int ) * cthreads_test_number_of_iterations );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "queued_values",
	 queued_values );

	result = libcthreads_lock_initialize(
	          &cthreads_test_lock,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "cthreads_test_lock",
	 cthreads_test_lock );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_thread_pool_create(
	          &thread_pool,
	          NULL,
	          8,
	          cthreads_test_number_of_values,
	          &cthreads_test_thread_pool_callback_function,
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "thread_pool",
	 thread_pool );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	for( iterator = 0;
	     iterator < cthreads_test_number_of_iterations;
	     iterator++ )
	{
		queued_values[ iterator ] = ( 98 * iterator ) % 45;

		result = libcthreads_thread_pool_push(
		          thread_pool,
		          (intptr_t *) &( queued_values[ iterator ] ),
		          &error );

		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		cthreads_test_expected_queued_value += queued_values[ iterator ];
	}
	result = libcthreads_thread_pool_join(
	          &thread_pool,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "thread_pool",
	 thread_pool );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libcthreads_thread_pool_push(
	          NULL,
	          (intptr_t *) &( queued_values[ 0 ] ),
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );
	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );
	libcerror_error_free(
	 &error );


	pool_dedicated1 = NULL;

	result = libcthreads_thread_pool_create(
	          &pool_dedicated1,
	          NULL,
	          1,
	          16,
	          &cthreads_test_thread_pool_callback_function,
	          NULL,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );
	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "pool_dedicated1",
	 pool_dedicated1 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	dedicated_values = (int *) memory_allocate(
	                            sizeof( int ) * 4 );
	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "dedicated_values",
	 dedicated_values );

	dedicated_values[ 0 ] = 9801;
	dedicated_values[ 1 ] = 9802;
	dedicated_values[ 2 ] = 9803;
	dedicated_values[ 3 ] = 9807;

	/* Grab the lock so that the worker thread cannot process the queued values
	 */
	result = libcthreads_lock_grab(
	          cthreads_test_lock,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	gate_held = 1;

	result = libcthreads_thread_pool_push(
	          pool_dedicated1,
	          (intptr_t *) &( dedicated_values[ 0 ] ),
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	cthreads_test_expected_queued_value += dedicated_values[ 0 ];

	/* Wait for the worker thread to pop the value, it then parks in the
	 * callback waiting on the gate held by this thread. Having the worker
	 * thread parked in the callback function also protects the modifications
	 * of the pool values array below, otherwise a race with the running
	 * worker thread could cause it to exit and leave the pool without a worker.
	 */
	while( ( ( ( libcthreads_internal_thread_pool_t * ) pool_dedicated1 ) )->number_of_values != 0 )
	{
		sleep( 0.01 );
	}
	cthreads_test_thread_pool_wait_for_worker_parking( pool_dedicated1 );

	values_array = ( ( libcthreads_internal_thread_pool_t * ) pool_dedicated1 )->values_array;

	( ( libcthreads_internal_thread_pool_t * ) pool_dedicated1 )->values_array = NULL;

	result = libcthreads_thread_pool_push(
	          pool_dedicated1,
	          (intptr_t *) &( queued_values[ 0 ] ),
	          &error
	);

	( ( libcthreads_internal_thread_pool_t * ) pool_dedicated1 )->values_array = values_array;

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );
	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );
	libcerror_error_free(
	 &error );


	result = libcthreads_thread_pool_push(
	          pool_dedicated1,
	          NULL,
	          &error
	);
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

	fill_values = (int *) memory_allocate(
	                   sizeof( int ) * 16 );
	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "fill_values",
	 fill_values );

	while( ( ( ( libcthreads_internal_thread_pool_t * ) pool_dedicated1 ) )->number_of_values < ( ( ( libcthreads_internal_thread_pool_t * ) pool_dedicated1 ) )->allocated_number_of_values && fill_index < 16 )
	{
		fill_values[ fill_index ] = 8000 + fill_index;

		result = libcthreads_thread_pool_push(
		          pool_dedicated1,
		          (intptr_t *) &( fill_values[ fill_index ] ),
		          &error
		);
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );
		CTHREADS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		cthreads_test_expected_queued_value += fill_values[ fill_index ];

		fill_index++;
	}

	/* Test libcthreads_thread_pool_push with pthread_mutex_lock failing in libcthreads_mutex_grab
	 */
	cthreads_test_pthread_mutex_lock_attempts_before_fail = 0;

	result = libcthreads_thread_pool_push(
	          pool_dedicated1,
	          (intptr_t *) &( dedicated_values[ 1 ] ),
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "cthreads_test_pthread_mutex_lock_attempts_before_fail",
	 cthreads_test_pthread_mutex_lock_attempts_before_fail,
	 -1 );

	if( cthreads_test_pthread_mutex_lock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_lock_attempts_before_fail = -1;
		cthreads_test_expected_queued_value += dedicated_values[ 1 ];
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

	/* Test libcthreads_thread_pool_push with pthread_cond_wait failing in libcthreads_condition_wait
	 */
	cthreads_test_pthread_cond_wait_attempts_before_fail = 0;

	result = libcthreads_thread_pool_push(
	          pool_dedicated1,
	          (intptr_t *) &( dedicated_values[ 2 ] ),
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "cthreads_test_pthread_cond_wait_attempts_before_fail",
	 cthreads_test_pthread_cond_wait_attempts_before_fail,
	 -1 );

	if( cthreads_test_pthread_cond_wait_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_cond_wait_attempts_before_fail = -1;
		cthreads_test_expected_queued_value += dedicated_values[ 2 ];
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

#endif /* defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK ) */

	/* Release the lock so that the worker thread can process the queued values
	 */
	result = libcthreads_lock_release(
	          cthreads_test_lock,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	gate_held = 0;

	result = libcthreads_thread_pool_join(
	          &pool_dedicated1,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "pool_dedicated1",
	 pool_dedicated1 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#if defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK )

	pool_dedicated2 = NULL;

	result = libcthreads_thread_pool_create(
	          &pool_dedicated2,
	          NULL,
	          1,
	          16,
	          &cthreads_test_thread_pool_callback_function,
	          NULL,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );
	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "pool_dedicated2",
	 pool_dedicated2 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Grab the lock so that the worker thread cannot process the queued values
	 */
	result = libcthreads_lock_grab(
	          cthreads_test_lock,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	gate_held = 1;

	result = libcthreads_thread_pool_push(
	          pool_dedicated2,
	          (intptr_t *) &( dedicated_values[ 3 ] ),
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	cthreads_test_expected_queued_value += dedicated_values[ 3 ];

	/* Wait for the worker thread to pop the value, it then parks in the
	 * callback waiting on the gate held by this thread, so that the fail-once
	 * test hooks below cannot be consumed by the worker thread anymore
	 */
	while( ( ( ( libcthreads_internal_thread_pool_t * ) pool_dedicated2 ) )->number_of_values != 0 )
	{
		sleep( 0.01 );
	}
	cthreads_test_thread_pool_wait_for_worker_parking( pool_dedicated2 );

	/* Test libcthreads_thread_pool_push with pthread_cond_broadcast failing in libcthreads_condition_broadcast
	 */
	cthreads_test_pthread_cond_broadcast_attempts_before_fail = 0;

	result = libcthreads_thread_pool_push(
	          pool_dedicated2,
	          (intptr_t *) &( dedicated_values[ 0 ] ),
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "cthreads_test_pthread_cond_broadcast_attempts_before_fail",
	 cthreads_test_pthread_cond_broadcast_attempts_before_fail,
	 -1 );

	if( cthreads_test_pthread_cond_broadcast_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_cond_broadcast_attempts_before_fail = -1;
		cthreads_test_expected_queued_value += dedicated_values[ 0 ];
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

		cthreads_test_expected_queued_value += dedicated_values[ 0 ];
	}

	/* Test libcthreads_thread_pool_push with pthread_mutex_unlock failing in libcthreads_mutex_release
	 */
	cthreads_test_pthread_mutex_unlock_attempts_before_fail = 0;

	result = libcthreads_thread_pool_push(
	          pool_dedicated2,
	          (intptr_t *) &( dedicated_values[ 1 ] ),
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "cthreads_test_pthread_mutex_unlock_attempts_before_fail",
	 cthreads_test_pthread_mutex_unlock_attempts_before_fail,
	 -1 );

	if( cthreads_test_pthread_mutex_unlock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_unlock_attempts_before_fail = -1;
		cthreads_test_expected_queued_value += dedicated_values[ 1 ];
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

		cthreads_test_expected_queued_value += dedicated_values[ 1 ];

		/* Manually release the mutex otherwise libcthreads_thread_pool_join will fail
		 */
		result = libcthreads_mutex_release(
		          ( ( libcthreads_internal_thread_pool_t * ) pool_dedicated2 )->condition_mutex,
		          &error );

		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}

	/* Release the lock so that the worker thread can process the queued values
	 */
	result = libcthreads_lock_release(
	          cthreads_test_lock,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	gate_held = 0;

	result = libcthreads_thread_pool_join(
	          &pool_dedicated2,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "pool_dedicated2",
	 pool_dedicated2 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK ) */
	result = ( cthreads_test_queued_value == cthreads_test_expected_queued_value );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	/* Clean up
	 */
	result = libcthreads_lock_free(
	          &cthreads_test_lock,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "cthreads_test_lock",
	 cthreads_test_lock );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	memory_free(
	 fill_values );

	fill_values = NULL;

	memory_free(
	 dedicated_values );

	dedicated_values = NULL;

	memory_free(
	 queued_values );

	queued_values = NULL;

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stderr );

		libcerror_error_free(
		 &error );
	}
	if( gate_held )
	{
		libcthreads_lock_release(
		 cthreads_test_lock,
		 NULL );

		gate_held = 0;
	}
	if( pool_dedicated1 != NULL )
	{
		libcthreads_thread_pool_join(
		 &pool_dedicated1,
		 NULL );
	}
	if( pool_dedicated2 != NULL )
	{
		libcthreads_thread_pool_join(
		 &pool_dedicated2,
		 NULL );
	}
	if( thread_pool != NULL )
	{
		libcthreads_thread_pool_join(
		 &thread_pool,
		 NULL );
	}
	if( cthreads_test_lock != NULL )
	{
		libcthreads_lock_release(
		 cthreads_test_lock,
		 NULL );
		libcthreads_lock_free(
		 &cthreads_test_lock,
		 NULL );
	}
	if( queued_values != NULL )
	{
		memory_free(
		 queued_values );
	}
	if( fill_values != NULL )
	{
		memory_free(
		 fill_values );
	}
	if( dedicated_values != NULL )
	{
		memory_free(
		 dedicated_values );
	}
	return( -1 );
}

/* The thread pool value compare function
 * Returns LIBCTHREADS_COMPARE_LESS, LIBCTHREADS_COMPARE_EQUAL or LIBCTHREADS_COMPARE_GREATER if successful or -1 on error
 */
int cthreads_test_thread_pool_value_compare_function(
      int *first_value,
      int *second_value,
      libcthreads_error_t **error )
{
	static char *function = "cthreads_test_thread_pool_value_compare_function";

	if( first_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first value.",
		 function );

		return( -1 );
	}
	if( second_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second value.",
		 function );

		return( -1 );
	}
	if( *first_value < *second_value )
	{
		return( LIBCTHREADS_COMPARE_LESS );
	}
	else if( *first_value > *second_value )
	{
		return( LIBCTHREADS_COMPARE_GREATER );
	}
	return( LIBCTHREADS_COMPARE_EQUAL );
}

/* The thread pool value compare function that returns -1
 * Returns -1 on error
 */
int cthreads_test_thread_pool_error_value_compare_function(
      int *first_value CTHREADS_TEST_ATTRIBUTE_UNUSED,
      int *second_value CTHREADS_TEST_ATTRIBUTE_UNUSED,
      libcthreads_error_t **error CTHREADS_TEST_ATTRIBUTE_UNUSED )
{
	CTHREADS_TEST_UNREFERENCED_PARAMETER( first_value )
	CTHREADS_TEST_UNREFERENCED_PARAMETER( second_value )
	CTHREADS_TEST_UNREFERENCED_PARAMETER( error )

	return( -1 );
}

/* The thread pool value compare function that returns an unsupported value
 * Returns an unsupported value
 */
int cthreads_test_thread_pool_unsupported_value_compare_function(
      int *first_value CTHREADS_TEST_ATTRIBUTE_UNUSED,
      int *second_value CTHREADS_TEST_ATTRIBUTE_UNUSED,
      libcthreads_error_t **error CTHREADS_TEST_ATTRIBUTE_UNUSED )
{
	CTHREADS_TEST_UNREFERENCED_PARAMETER( first_value )
	CTHREADS_TEST_UNREFERENCED_PARAMETER( second_value )
	CTHREADS_TEST_UNREFERENCED_PARAMETER( error )

	return( 42 );
}


/* Tests the libcthreads_thread_pool_push_sorted function

 * Returns 1 if successful or 0 if not

 */

int cthreads_test_thread_pool_push_sorted(

      void )

{

	libcerror_error_t *error                = NULL;
	libcthreads_thread_pool_t *thread_pool1 = NULL;
	libcthreads_thread_pool_t *thread_pool2 = NULL;
	intptr_t **values_array                 = NULL;
	int *sorted_values                      = NULL;
	int *fill_values                        = NULL;
	int fill_index                          = 0;
	int gate_held                           = 0;
	int result                              = 0;

	/* Initialize test

	 */

	if( cthreads_test_lock == NULL )

	{

		result = libcthreads_lock_initialize(
		          &cthreads_test_lock,
		          &error
		);

		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "cthreads_test_lock",
		 cthreads_test_lock );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

	}

	thread_pool1 = NULL;

	result = libcthreads_thread_pool_create(
	          &thread_pool1,
	          NULL,
	          1,
	          16,
	          &cthreads_test_thread_pool_callback_function,
	          NULL,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "thread_pool1",
	 thread_pool1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	sorted_values = (int *) memory_allocate(

	                        sizeof( int ) * 4 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "sorted_values",
	 sorted_values );

	sorted_values[ 0 ] = 100;

	sorted_values[ 1 ] = 200;

	sorted_values[ 2 ] = 300;

	sorted_values[ 3 ] = 400;

	/* Grab the lock so that the worker thread cannot process the queued values

	 */

	result = libcthreads_lock_grab(
	          cthreads_test_lock,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	gate_held = 1;

	/* Test regular cases

	 */

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool1,
	          (intptr_t *) &( sorted_values[ 0 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
	          0,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
	cthreads_test_expected_queued_value += sorted_values[ 0 ];

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool1,
	          (intptr_t *) &( sorted_values[ 1 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
	          LIBCTHREADS_SORT_FLAG_UNIQUE_VALUES,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
	cthreads_test_expected_queued_value += sorted_values[ 1 ];

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool1,
	          (intptr_t *) &( sorted_values[ 2 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
	          LIBCTHREADS_SORT_FLAG_UNIQUE_VALUES,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
	cthreads_test_expected_queued_value += sorted_values[ 2 ];

	/* Test with a duplicate value and LIBCTHREADS_SORT_FLAG_UNIQUE_VALUES set

	 */

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool1,
	          (intptr_t *) &( sorted_values[ 2 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
	          LIBCTHREADS_SORT_FLAG_UNIQUE_VALUES,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test with a duplicate value and LIBCTHREADS_SORT_FLAG_NON_UNIQUE_VALUES set

	 */

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool1,
	          (intptr_t *) &( sorted_values[ 2 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
	          0,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
	cthreads_test_expected_queued_value += sorted_values[ 2 ];

	/* Wait for the worker thread to pop the first value, it then parks in the

	 * callback waiting on the gate held by this thread, so that the fill loop

	 * below fills the queue to capacity deterministically

	 */

	while( ( ( ( libcthreads_internal_thread_pool_t * ) thread_pool1 ) )->number_of_values != 3 )

	{

		sleep( 0.01 );

	}

	/* Fill the queue

	 */

	fill_values = (int *) memory_allocate(

	                   sizeof( int ) * 16 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "fill_values",
	 fill_values );

	while( ( ( ( libcthreads_internal_thread_pool_t * ) thread_pool1 ) )->number_of_values < ( ( ( libcthreads_internal_thread_pool_t * ) thread_pool1 ) )->allocated_number_of_values && fill_index < 16 )

	{

		fill_values[ fill_index ] = 9000 + fill_index;

		result = libcthreads_thread_pool_push_sorted(
		          thread_pool1,
		          (intptr_t *) &( fill_values[ fill_index ] ),
		          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
		          LIBCTHREADS_SORT_FLAG_UNIQUE_VALUES,
		          &error
		);
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );
		CTHREADS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
		cthreads_test_expected_queued_value += fill_values[ fill_index ];

		fill_index++;

	}

	/* Test error cases

	 */

	/* Test libcthreads_thread_pool_push_sorted with an invalid thread pool
	 */
	result = libcthreads_thread_pool_push_sorted(
	          NULL,
	          (intptr_t *) &( sorted_values[ 0 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
	          0,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );
	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );
	libcerror_error_free(
	 &error );

	values_array = ( ( libcthreads_internal_thread_pool_t * ) thread_pool1 )->values_array;

	( ( libcthreads_internal_thread_pool_t * ) thread_pool1 )->values_array = NULL;

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool1,
	          (intptr_t *) &( sorted_values[ 0 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
	          0,
	          &error
	);

	( ( libcthreads_internal_thread_pool_t * ) thread_pool1 )->values_array = values_array;

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool1,
	          NULL,
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
	          0,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );
	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );
	libcerror_error_free(
	 &error );

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool1,
	          (intptr_t *) &( sorted_values[ 0 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) NULL,
	          0,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );
	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );
	libcerror_error_free(
	 &error );

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool1,
	          (intptr_t *) &( sorted_values[ 0 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
	          0xff,
	          &error
	);
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

	/* Test libcthreads_thread_pool_push_sorted with pthread_mutex_lock failing in libcthreads_mutex_grab
	 */
	cthreads_test_pthread_mutex_lock_attempts_before_fail = 0;

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool1,
	          (intptr_t *) &( sorted_values[ 3 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
	          0,
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

	/* Test libcthreads_thread_pool_push_sorted with pthread_cond_wait failing in libcthreads_condition_wait
	 */
	cthreads_test_pthread_cond_wait_attempts_before_fail = 0;

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool1,
	          (intptr_t *) &( sorted_values[ 3 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
	          0,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "cthreads_test_pthread_cond_wait_attempts_before_fail",
	 cthreads_test_pthread_cond_wait_attempts_before_fail,
	 -1 );

	if( cthreads_test_pthread_cond_wait_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_cond_wait_attempts_before_fail = -1;
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

#endif /* defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK ) */

	/* Release the lock so that the worker thread can process the queued values

	 */

	result = libcthreads_lock_release(
	          cthreads_test_lock,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	gate_held = 0;

	result = libcthreads_thread_pool_join(
	          &thread_pool1,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "thread_pool1",
	 thread_pool1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	thread_pool2 = NULL;

	result = libcthreads_thread_pool_create(
	          &thread_pool2,
	          NULL,
	          1,
	          16,
	          &cthreads_test_thread_pool_callback_function,
	          NULL,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "thread_pool2",
	 thread_pool2 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Grab the lock so that the worker thread cannot process the queued values

	 */

	result = libcthreads_lock_grab(
	          cthreads_test_lock,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	gate_held = 1;

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool2,
	          (intptr_t *) &( sorted_values[ 0 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
	          0,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );
	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
	cthreads_test_expected_queued_value += sorted_values[ 0 ];

	/* Queue a second value so that at least one value remains in the pool queue.
	 * The worker thread may have popped the first value already, in which case it is
	 * blocked in the test callback function while the test lock is held. The value
	 * compare function is only invoked by libcthreads_thread_pool_push_sorted when
	 * the pool queue is not empty, otherwise the error injection test cases below
	 * will not run the value compare function and will return 1 instead of -1.
	 */
	result = libcthreads_thread_pool_push_sorted(
	          thread_pool2,
	          (intptr_t *) &( sorted_values[ 1 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
	          0,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	cthreads_test_expected_queued_value += sorted_values[ 1 ];

	/* Wait for the worker thread to pop one of the two queued values, it then
	 * parks in the callback waiting on the gate held by this thread, so that
	 * the fail-once test hooks below cannot be consumed by the worker thread
	 * anymore, and at least one value remains in the pool queue
	 */
	while( ( ( ( libcthreads_internal_thread_pool_t * ) thread_pool2 ) )->number_of_values != 1 )
	{
		sleep( 0.01 );
	}
	cthreads_test_thread_pool_wait_for_worker_parking( thread_pool2 );

	/* Test libcthreads_thread_pool_push_sorted with an error return value from the value compare function

	 */

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool2,
	          (intptr_t *) &( sorted_values[ 2 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_error_value_compare_function,
	          0,
	          &error
	);
	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );
	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );
	libcerror_error_free(
	 &error );

	/* Test libcthreads_thread_pool_push_sorted with an unsupported return value from the value compare function

	 */

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool2,
	          (intptr_t *) &( sorted_values[ 3 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_unsupported_value_compare_function,
	          0,
	          &error
	);
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

	/* Test libcthreads_thread_pool_push_sorted with pthread_cond_broadcast failing in libcthreads_condition_broadcast
	 */
	cthreads_test_pthread_cond_broadcast_attempts_before_fail = 0;

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool2,
	          (intptr_t *) &( sorted_values[ 3 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
	          0,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "cthreads_test_pthread_cond_broadcast_attempts_before_fail",
	 cthreads_test_pthread_cond_broadcast_attempts_before_fail,
	 -1 );

	if( cthreads_test_pthread_cond_broadcast_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_cond_broadcast_attempts_before_fail = -1;
		cthreads_test_expected_queued_value += sorted_values[ 3 ];
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

		cthreads_test_expected_queued_value += sorted_values[ 3 ];
	}

	/* Test libcthreads_thread_pool_push_sorted with pthread_mutex_unlock failing in libcthreads_mutex_release
	 */
	cthreads_test_pthread_mutex_unlock_attempts_before_fail = 0;

	result = libcthreads_thread_pool_push_sorted(
	          thread_pool2,
	          (intptr_t *) &( sorted_values[ 3 ] ),
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_thread_pool_value_compare_function,
	          LIBCTHREADS_SORT_FLAG_NON_UNIQUE_VALUES,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "cthreads_test_pthread_mutex_unlock_attempts_before_fail",
	 cthreads_test_pthread_mutex_unlock_attempts_before_fail,
	 -1 );

	if( cthreads_test_pthread_mutex_unlock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_unlock_attempts_before_fail = -1;
		cthreads_test_expected_queued_value += sorted_values[ 3 ];
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

		cthreads_test_expected_queued_value += sorted_values[ 3 ];

		/* Manually release the mutex otherwise libcthreads_thread_pool_join will fail
		 */
		result = libcthreads_mutex_release(
		          ( ( libcthreads_internal_thread_pool_t * ) thread_pool2 )->condition_mutex,
		          &error );

		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}

#endif /* defined( HAVE_CTHREADS_TEST_FUNCTION_HOOK ) */

	/* Release the lock so that the worker thread can process the queued values

	 */

	result = libcthreads_lock_release(
	          cthreads_test_lock,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	gate_held = 0;

	result = libcthreads_thread_pool_join(
	          &thread_pool2,
	          &error
	);

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "thread_pool2",
	 thread_pool2 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up

	 */

	memory_free(

	 fill_values );

	fill_values = NULL;

	memory_free(

	 sorted_values );

	sorted_values = NULL;

	result = libcthreads_lock_free(

	          &cthreads_test_lock,

	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "cthreads_test_lock",
	 cthreads_test_lock );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:

	if( error != NULL )

	{

		libcerror_error_backtrace_fprint(

		 error,

		 stderr );

		libcerror_error_free(

		 &error );

	}

	if( gate_held )

	{

		libcthreads_lock_release(

		 cthreads_test_lock,

		 NULL );

		gate_held = 0;

	}

	if( thread_pool1 != NULL )

	{

		libcthreads_thread_pool_join(

		 &thread_pool1,

		 NULL );

	}

	if( thread_pool2 != NULL )

	{

		libcthreads_thread_pool_join(

		 &thread_pool2,

		 NULL );

	}

	if( fill_values != NULL )

	{

		memory_free(

		 fill_values );

	}

	if( sorted_values != NULL )

	{

		memory_free(

		 sorted_values );

	}

	if( cthreads_test_lock != NULL )

	{

		libcthreads_lock_free(

		 &cthreads_test_lock,

		 NULL );

	}

	return( 0 );

}

/* Tests the libcthreads_thread_join function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_thread_join(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libcthreads_thread_join(
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
	 "libcthreads_thread_pool_create",
	 cthreads_test_thread_pool_create );

	CTHREADS_TEST_RUN(
	 "libcthreads_thread_pool_join",
	 cthreads_test_thread_pool_join );

	CTHREADS_TEST_RUN(
	 "libcthreads_thread_pool_push",
	 cthreads_test_thread_pool_push );

	CTHREADS_TEST_RUN(
	 "libcthreads_thread_pool_push_sorted",
	 cthreads_test_thread_pool_push_sorted );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

