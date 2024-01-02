/*
 * Library queue type test program
 *
 * Copyright (C) 2012-2024, Joachim Metz <joachim.metz@gmail.com>
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

#include <errno.h>

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )
#define __USE_GNU
#include <dlfcn.h>
#undef __USE_GNU
#endif

#include "cthreads_test_libcerror.h"
#include "cthreads_test_libcthreads.h"
#include "cthreads_test_macros.h"
#include "cthreads_test_memory.h"
#include "cthreads_test_unused.h"

#include "../libcthreads/libcthreads_queue.h"

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

static int (*cthreads_test_real_pthread_mutex_lock)(pthread_mutex_t *)   = NULL;
static int (*cthreads_test_real_pthread_mutex_unlock)(pthread_mutex_t *) = NULL;

int cthreads_test_pthread_mutex_lock_attempts_before_fail                = -1;
int cthreads_test_pthread_mutex_unlock_attempts_before_fail              = -1;

#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

libcthreads_queue_t *cthreads_test_queue = NULL;
int cthreads_test_expected_queued_value  = 0;
int cthreads_test_queued_value           = 0;
int cthreads_test_number_of_iterations   = 497;
int cthreads_test_number_of_values       = 32;

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

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

#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

/* Test element compare function
 * Returns LIBCTHREADS_COMPARE_LESS, LIBCTHREADS_COMPARE_EQUAL, LIBCTHREADS_COMPARE_GREATER if successful or -1 on error
 */
int cthreads_test_queue_value_compare_function(
     int *first_value,
     int *second_value,
     libcthreads_error_t **error )
{
	static char *function = "cthreads_test_queue_value_compare_function";

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
	if( *first_value > *second_value )
	{
		return( LIBCTHREADS_COMPARE_LESS );
	}
	else if( *first_value < *second_value )
	{
		return( LIBCTHREADS_COMPARE_GREATER );
	}
	return( LIBCTHREADS_COMPARE_EQUAL );
}

/* The thread pop callback function
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_queue_pop_callback_function(
     void *arguments CTHREADS_TEST_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_queue_pop_callback_function";
	int *queued_value        = NULL;
	int iterator             = 0;

	CTHREADS_TEST_UNREFERENCED_PARAMETER( arguments )

	for( iterator = 0;
	     iterator < cthreads_test_number_of_iterations;
	     iterator++ )
	{
		if( libcthreads_queue_pop(
		     cthreads_test_queue,
		     (intptr_t **) &queued_value,
		     &error ) == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
			 "%s: unable to pop value off queue.",
			 function );

			goto on_error;
		}
		cthreads_test_queued_value += *queued_value;
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

/* The thread push callback function
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_queue_push_callback_function(
     int *queued_values )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_queue_push_callback_function";
	int iterator             = 0;

	for( iterator = 0;
	     iterator < cthreads_test_number_of_iterations;
	     iterator++ )
	{
		queued_values[ iterator ] = ( 98 * iterator ) % 45;

		if( libcthreads_queue_push(
		     cthreads_test_queue,
		     (intptr_t *) &( queued_values[ iterator ] ),
		     &error ) == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to get value from queue.",
			 function );

			goto on_error;
		}
		cthreads_test_expected_queued_value += queued_values[ iterator ];
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

/* Tests the libcthreads_queue_initialize function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_queue_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libcthreads_queue_t *queue      = NULL;
	int result                      = 0;

#if defined( HAVE_CTHREADS_TEST_MEMORY )
	int number_of_malloc_fail_tests = 5;
	int number_of_memset_fail_tests = 2;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libcthreads_queue_initialize(
	          &queue,
	          0,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "queue",
	 queue );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_queue_free(
	          &queue,
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "queue",
	 queue );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libcthreads_queue_initialize(
	          NULL,
	          0,
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

	result = libcthreads_queue_initialize(
	          &queue,
	          (int) SSIZE_MAX,
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

	result = libcthreads_queue_initialize(
	          &queue,
	          -1,
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

	queue = (libcthreads_queue_t *) 0x12345678UL;

	result = libcthreads_queue_initialize(
	          &queue,
	          0,
	          &error );

	queue = NULL;

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

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libcthreads_queue_initialize with malloc failing
		 */
		cthreads_test_malloc_attempts_before_fail = test_number;

		result = libcthreads_queue_initialize(
		          &queue,
		          0,
		          &error );

		if( cthreads_test_malloc_attempts_before_fail != -1 )
		{
			cthreads_test_malloc_attempts_before_fail = -1;

			if( queue != NULL )
			{
				libcthreads_queue_free(
				 &queue,
				 NULL,
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
			 "queue",
			 queue );

			CTHREADS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libcthreads_queue_initialize with memset failing
		 */
		cthreads_test_memset_attempts_before_fail = test_number;

		result = libcthreads_queue_initialize(
		          &queue,
		          0,
		          &error );

		if( cthreads_test_memset_attempts_before_fail != -1 )
		{
			cthreads_test_memset_attempts_before_fail = -1;

			if( queue != NULL )
			{
				libcthreads_queue_free(
				 &queue,
				 NULL,
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
			 "queue",
			 queue );

			CTHREADS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_CTHREADS_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( queue != NULL )
	{
		libcthreads_queue_free(
		 &queue,
		 NULL,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_queue_free function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_queue_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libcthreads_queue_free(
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libcthreads_queue_empty function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_queue_empty(
     void )
{
	libcerror_error_t *error   = NULL;
	libcthreads_queue_t *queue = NULL;
	intptr_t **values_array    = NULL;
	int result                 = 0;

	/* Initialize test
	 */
	result = libcthreads_queue_initialize(
	          &queue,
	          10,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "queue",
	 queue );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libcthreads_queue_empty(
	          queue,
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
	result = libcthreads_queue_empty(
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

	values_array = ( (libcthreads_internal_queue_t *) queue )->values_array;

	( (libcthreads_internal_queue_t *) queue )->values_array = NULL;

	result = libcthreads_queue_empty(
	          queue,
	          &error );

	( (libcthreads_internal_queue_t *) queue )->values_array = values_array;

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

	/* Test libcthreads_queue_empty with pthread_mutex_lock failing in libcthreads_mutex_grab
	 */
	cthreads_test_pthread_mutex_lock_attempts_before_fail = 0;

	result = libcthreads_queue_empty(
	          queue,
	          &error );

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
/* TODO test libcthreads_queue_empty with libcthreads_condition_wait failing */

	/* Test libcthreads_queue_empty with pthread_mutex_unlock failing in libcthreads_mutex_release
	 */
	cthreads_test_pthread_mutex_unlock_attempts_before_fail = 0;

	result = libcthreads_queue_empty(
	          queue,
	          &error );

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

		/* Manually release the mutex otherwise libcthreads_queue_free will fail
		 */
		result = libcthreads_mutex_release(
		          ( (libcthreads_internal_queue_t *) queue )->condition_mutex,
		          &error );

		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

	/* Clean up
	 */
	result = libcthreads_queue_free(
	          &queue,
	          NULL,
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
	if( queue != NULL )
	{
		libcthreads_queue_free(
		 &queue,
		 NULL,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_queue_try_pop function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_queue_try_pop(
     void )
{
	libcerror_error_t *error   = NULL;
	libcthreads_queue_t *queue = NULL;
	intptr_t *value            = NULL;
	intptr_t **values_array    = NULL;
	int queued_value1          = 1;
	int queued_value2          = 2;
	int result                 = 0;

	/* Initialize test
	 */
	result = libcthreads_queue_initialize(
	          &queue,
	          10,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "queue",
	 queue );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libcthreads_queue_try_pop(
	          queue,
	          &value,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libcthreads_queue_push(
	          queue,
	          (intptr_t *) &queued_value1,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_queue_push(
	          queue,
	          (intptr_t *) &queued_value2,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libcthreads_queue_try_pop(
	          queue,
	          &value,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "value",
	 value );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libcthreads_queue_try_pop(
	          NULL,
	          &value,
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

	values_array = ( (libcthreads_internal_queue_t *) queue )->values_array;

	( (libcthreads_internal_queue_t *) queue )->values_array = NULL;

	result = libcthreads_queue_try_pop(
	          queue,
	          &value,
	          &error );

	( (libcthreads_internal_queue_t *) queue )->values_array = values_array;

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libcthreads_queue_try_pop(
	          queue,
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

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

	/* Test libcthreads_queue_try_pop with pthread_mutex_lock failing in libcthreads_mutex_grab
	 */
	cthreads_test_pthread_mutex_lock_attempts_before_fail = 0;

	result = libcthreads_queue_try_pop(
	          queue,
	          &value,
	          &error );

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
/* TODO test libcthreads_queue_try_pop with libcthreads_condition_broadcast failing */

	/* Test libcthreads_queue_try_pop with pthread_mutex_unlock failing in libcthreads_mutex_release
	 */
	cthreads_test_pthread_mutex_unlock_attempts_before_fail = 0;

	result = libcthreads_queue_try_pop(
	          queue,
	          &value,
	          &error );

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

		/* Manually release the mutex otherwise libcthreads_queue_free will fail
		 */
		result = libcthreads_mutex_release(
		          ( (libcthreads_internal_queue_t *) queue )->condition_mutex,
		          &error );

		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

	/* Clean up
	 */
	result = libcthreads_queue_free(
	          &queue,
	          NULL,
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
	if( queue != NULL )
	{
		libcthreads_queue_free(
		 &queue,
		 NULL,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_queue_pop function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_queue_pop(
     void )
{
	libcerror_error_t *error   = NULL;
	libcthreads_queue_t *queue = NULL;
	intptr_t *value            = NULL;
	intptr_t **values_array    = NULL;
	int queued_value1          = 1;
	int queued_value2          = 2;
	int result                 = 0;

	/* Initialize test
	 */
	result = libcthreads_queue_initialize(
	          &queue,
	          10,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "queue",
	 queue );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_queue_push(
	          queue,
	          (intptr_t *) &queued_value1,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_queue_push(
	          queue,
	          (intptr_t *) &queued_value2,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libcthreads_queue_pop(
	          queue,
	          &value,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "value",
	 value );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libcthreads_queue_pop(
	          NULL,
	          &value,
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

	values_array = ( (libcthreads_internal_queue_t *) queue )->values_array;

	( (libcthreads_internal_queue_t *) queue )->values_array = NULL;

	result = libcthreads_queue_pop(
	          queue,
	          &value,
	          &error );

	( (libcthreads_internal_queue_t *) queue )->values_array = values_array;

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libcthreads_queue_pop(
	          queue,
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

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

	/* Test libcthreads_queue_pop with pthread_mutex_lock failing in libcthreads_mutex_grab
	 */
	cthreads_test_pthread_mutex_lock_attempts_before_fail = 0;

	result = libcthreads_queue_pop(
	          queue,
	          &value,
	          &error );

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
/* TODO test libcthreads_queue_pop with libcthreads_condition_wait failing */
/* TODO test libcthreads_queue_pop with libcthreads_condition_broadcast failing */

	/* Test libcthreads_queue_pop with pthread_mutex_unlock failing in libcthreads_mutex_release
	 */
	cthreads_test_pthread_mutex_unlock_attempts_before_fail = 0;

	result = libcthreads_queue_pop(
	          queue,
	          &value,
	          &error );

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

		/* Manually release the mutex otherwise libcthreads_queue_free will fail
		 */
		result = libcthreads_mutex_release(
		          ( (libcthreads_internal_queue_t *) queue )->condition_mutex,
		          &error );

		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

	/* Clean up
	 */
	result = libcthreads_queue_free(
	          &queue,
	          NULL,
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
	if( queue != NULL )
	{
		libcthreads_queue_free(
		 &queue,
		 NULL,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_queue_try_push function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_queue_try_push(
     void )
{
	libcerror_error_t *error       = NULL;
	libcthreads_queue_t *queue     = NULL;
	intptr_t **values_array        = NULL;
	int allocated_number_of_values = 0;
	int queued_value1              = 1;
	int queued_value2              = 2;
	int result                     = 0;

	/* Initialize test
	 */
	result = libcthreads_queue_initialize(
	          &queue,
	          10,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "queue",
	 queue );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_queue_push(
	          queue,
	          (intptr_t *) &queued_value1,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libcthreads_queue_try_push(
	          queue,
	          (intptr_t *) &queued_value1,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	allocated_number_of_values = ( (libcthreads_internal_queue_t *) queue )->allocated_number_of_values;

	( (libcthreads_internal_queue_t *) queue )->allocated_number_of_values = ( (libcthreads_internal_queue_t *) queue )->number_of_values;

	result = libcthreads_queue_try_push(
	          queue,
	          (intptr_t *) &queued_value2,
	          &error );

	( (libcthreads_internal_queue_t *) queue )->allocated_number_of_values = allocated_number_of_values;

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libcthreads_queue_try_push(
	          queue,
	          (intptr_t *) &queued_value2,
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
	result = libcthreads_queue_try_push(
	          NULL,
	          (intptr_t *) &queued_value2,
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

	values_array = ( (libcthreads_internal_queue_t *) queue )->values_array;

	( (libcthreads_internal_queue_t *) queue )->values_array = NULL;

	result = libcthreads_queue_try_push(
	          queue,
	          (intptr_t *) &queued_value2,
	          &error );

	( (libcthreads_internal_queue_t *) queue )->values_array = values_array;

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libcthreads_queue_try_push(
	          queue,
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

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

	/* Test libcthreads_queue_try_push with pthread_mutex_lock failing in libcthreads_mutex_grab
	 */
	cthreads_test_pthread_mutex_lock_attempts_before_fail = 0;

	result = libcthreads_queue_try_push(
	          queue,
	          (intptr_t *) &queued_value2,
	          &error );

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
/* TODO test libcthreads_queue_try_push with libcthreads_condition_broadcast failing */

	/* Test libcthreads_queue_try_push with pthread_mutex_unlock failing in libcthreads_mutex_release
	 */
	cthreads_test_pthread_mutex_unlock_attempts_before_fail = 0;

	result = libcthreads_queue_try_push(
	          queue,
	          (intptr_t *) &queued_value2,
	          &error );

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

		/* Manually release the mutex otherwise libcthreads_queue_free will fail
		 */
		result = libcthreads_mutex_release(
		          ( (libcthreads_internal_queue_t *) queue )->condition_mutex,
		          &error );

		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

	/* Clean up
	 */
	result = libcthreads_queue_free(
	          &queue,
	          NULL,
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
	if( queue != NULL )
	{
		libcthreads_queue_free(
		 &queue,
		 NULL,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_queue_push function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_queue_push(
     void )
{
	libcerror_error_t *error   = NULL;
	libcthreads_queue_t *queue = NULL;
	intptr_t **values_array    = NULL;
	int queued_value1          = 1;
	int queued_value2          = 2;
	int result                 = 0;

	/* Initialize test
	 */
	result = libcthreads_queue_initialize(
	          &queue,
	          10,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "queue",
	 queue );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_queue_push(
	          queue,
	          (intptr_t *) &queued_value1,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libcthreads_queue_push(
	          queue,
	          (intptr_t *) &queued_value2,
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
	result = libcthreads_queue_push(
	          NULL,
	          (intptr_t *) &queued_value2,
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

	values_array = ( (libcthreads_internal_queue_t *) queue )->values_array;

	( (libcthreads_internal_queue_t *) queue )->values_array = NULL;

	result = libcthreads_queue_push(
	          queue,
	          (intptr_t *) &queued_value2,
	          &error );

	( (libcthreads_internal_queue_t *) queue )->values_array = values_array;

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libcthreads_queue_push(
	          queue,
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

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

	/* Test libcthreads_queue_push with pthread_mutex_lock failing in libcthreads_mutex_grab
	 */
	cthreads_test_pthread_mutex_lock_attempts_before_fail = 0;

	result = libcthreads_queue_push(
	          queue,
	          (intptr_t *) &queued_value2,
	          &error );

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
/* TODO test libcthreads_queue_push with libcthreads_condition_wait failing */
/* TODO test libcthreads_queue_push with libcthreads_condition_broadcast failing */

	/* Test libcthreads_queue_push with pthread_mutex_unlock failing in libcthreads_mutex_release
	 */
	cthreads_test_pthread_mutex_unlock_attempts_before_fail = 0;

	result = libcthreads_queue_push(
	          queue,
	          (intptr_t *) &queued_value2,
	          &error );

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

		/* Manually release the mutex otherwise libcthreads_queue_free will fail
		 */
		result = libcthreads_mutex_release(
		          ( (libcthreads_internal_queue_t *) queue )->condition_mutex,
		          &error );

		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

	/* Clean up
	 */
	result = libcthreads_queue_free(
	          &queue,
	          NULL,
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
	if( queue != NULL )
	{
		libcthreads_queue_free(
		 &queue,
		 NULL,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_queue_push_sorted function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_queue_push_sorted(
     void )
{
	libcerror_error_t *error   = NULL;
	libcthreads_queue_t *queue = NULL;
	intptr_t **values_array    = NULL;
	int queued_value1          = 1;
	int queued_value2          = 2;
	int result                 = 0;

	/* Initialize test
	 */
	result = libcthreads_queue_initialize(
	          &queue,
	          10,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "queue",
	 queue );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_queue_push(
	          queue,
	          (intptr_t *) &queued_value1,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libcthreads_queue_push_sorted(
	          queue,
	          (intptr_t *) &queued_value2,
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_queue_value_compare_function,
	          0,
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
	result = libcthreads_queue_push_sorted(
	          NULL,
	          (intptr_t *) &queued_value2,
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_queue_value_compare_function,
	          0,
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

	values_array = ( (libcthreads_internal_queue_t *) queue )->values_array;

	( (libcthreads_internal_queue_t *) queue )->values_array = NULL;

	result = libcthreads_queue_push_sorted(
	          queue,
	          (intptr_t *) &queued_value2,
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_queue_value_compare_function,
	          0,
	          &error );

	( (libcthreads_internal_queue_t *) queue )->values_array = values_array;

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libcthreads_queue_push_sorted(
	          queue,
	          NULL,
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_queue_value_compare_function,
	          0,
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

	result = libcthreads_queue_push_sorted(
	          queue,
	          (intptr_t *) &queued_value2,
	          NULL,
	          0,
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

	result = libcthreads_queue_push_sorted(
	          queue,
	          (intptr_t *) &queued_value2,
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_queue_value_compare_function,
	          0xff,
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

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

	/* Test libcthreads_queue_push_sorted with pthread_mutex_lock failing in libcthreads_mutex_grab
	 */
	cthreads_test_pthread_mutex_lock_attempts_before_fail = 0;

	result = libcthreads_queue_push_sorted(
	          queue,
	          (intptr_t *) &queued_value2,
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_queue_value_compare_function,
	          0,
	          &error );

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
/* TODO test libcthreads_queue_push_sorted with libcthreads_condition_wait failing */
/* TODO test libcthreads_queue_push_sorted with libcthreads_condition_broadcast failing */

	/* Test libcthreads_queue_push_sorted with pthread_mutex_unlock failing in libcthreads_mutex_release
	 */
	cthreads_test_pthread_mutex_unlock_attempts_before_fail = 0;

	result = libcthreads_queue_push_sorted(
	          queue,
	          (intptr_t *) &queued_value2,
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &cthreads_test_queue_value_compare_function,
	          0,
	          &error );

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

		/* Manually release the mutex otherwise libcthreads_queue_free will fail
		 */
		result = libcthreads_mutex_release(
		          ( (libcthreads_internal_queue_t *) queue )->condition_mutex,
		          &error );

		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

	/* Clean up
	 */
	result = libcthreads_queue_free(
	          &queue,
	          NULL,
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
	if( queue != NULL )
	{
		libcthreads_queue_free(
		 &queue,
		 NULL,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_queue_push and libcthreads_queue_pop functions in separate threads
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_queue_push_pop_threaded(
     void )
{
	libcerror_error_t *error          = NULL;
	libcthreads_thread_t *pop_thread  = NULL;
	libcthreads_thread_t *push_thread = NULL;
	int *queued_values                = NULL;
	int result                        = 0;

	/* Initialize test
	 */
	cthreads_test_expected_queued_value = 0;
	cthreads_test_queued_value          = 0;

	queued_values = (int *) memory_allocate(
	                         sizeof( int ) * cthreads_test_number_of_iterations );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "queued_values",
	 queued_values );

	result = libcthreads_queue_initialize(
	          &cthreads_test_queue,
	          cthreads_test_number_of_values,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_thread_create(
	          &push_thread,
	          NULL,
	          (int (*)(void *)) &cthreads_test_queue_push_callback_function,
	          queued_values,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_thread_create(
	          &pop_thread,
	          NULL,
	          (int (*)(void *)) &cthreads_test_queue_pop_callback_function,
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_thread_join(
	          &pop_thread,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_thread_join(
	          &push_thread,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_queue_empty(
	          cthreads_test_queue,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_queue_free(
	          &cthreads_test_queue,
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	memory_free(
	 queued_values );

	queued_values = NULL;

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "cthreads_test_queued_value",
	 cthreads_test_queued_value,
	 cthreads_test_expected_queued_value );

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
	if( pop_thread != NULL )
	{
		libcthreads_thread_join(
		 &pop_thread,
		 NULL );
	}
	if( push_thread != NULL )
	{
		libcthreads_thread_join(
		 &push_thread,
		 NULL );
	}
	if( cthreads_test_queue != NULL )
	{
		libcthreads_queue_free(
		 &cthreads_test_queue,
		 NULL,
		 NULL );
	}
	if( queued_values != NULL )
	{
		memory_free(
		 queued_values );
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
	 "libcthreads_queue_initialize",
	 cthreads_test_queue_initialize );

	CTHREADS_TEST_RUN(
	 "libcthreads_queue_free",
	 cthreads_test_queue_free );

	CTHREADS_TEST_RUN(
	 "libcthreads_queue_empty",
	 cthreads_test_queue_empty );

	CTHREADS_TEST_RUN(
	 "libcthreads_queue_try_pop",
	 cthreads_test_queue_try_pop );

	CTHREADS_TEST_RUN(
	 "libcthreads_queue_pop",
	 cthreads_test_queue_pop );

	CTHREADS_TEST_RUN(
	 "libcthreads_queue_try_push",
	 cthreads_test_queue_try_push );

	CTHREADS_TEST_RUN(
	 "libcthreads_queue_push",
	 cthreads_test_queue_push );

	CTHREADS_TEST_RUN(
	 "libcthreads_queue_push_sorted",
	 cthreads_test_queue_push_sorted );

	CTHREADS_TEST_RUN(
	 "cthreads_test_queue_push_pop_threaded",
	 cthreads_test_queue_push_pop_threaded );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

