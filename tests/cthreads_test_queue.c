/*
 * Library queue type testing program
 *
 * Copyright (C) 2012-2016, Joachim Metz <joachim.metz@gmail.com>
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

#include "cthreads_test_libcerror.h"
#include "cthreads_test_libcstring.h"
#include "cthreads_test_libcthreads.h"
#include "cthreads_test_macros.h"
#include "cthreads_test_memory.h"
#include "cthreads_test_unused.h"

libcthreads_queue_t *cthreads_test_queue = NULL;
int cthreads_test_expected_queued_value  = 0;
int cthreads_test_queued_value           = 0;
int cthreads_test_number_of_iterations   = 497;
int cthreads_test_number_of_values       = 32;

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
	libcerror_error_t *error   = NULL;
	libcthreads_queue_t *queue = NULL;
	int result                 = 0;

	/* Test libcthreads_queue_initialize
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

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        CTHREADS_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	queue = NULL;

#if defined( HAVE_CTHREADS_TEST_MEMORY )

	/* Test libcthreads_queue_initialize with malloc failing
	 */
	cthreads_test_malloc_attempts_before_fail = 0;

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
	cthreads_test_malloc_attempts_before_fail = 1;

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
	/* Test libcthreads_queue_initialize with memset failing
	 */
	cthreads_test_memset_attempts_before_fail = 0;

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
	cthreads_test_memset_attempts_before_fail = 1;

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
	libcerror_error_t *error = NULL;
	int result               = 0;

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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libcthreads_queue_try_pop function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_queue_try_pop(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libcthreads_queue_try_pop(
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

/* Tests the libcthreads_queue_pop function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_queue_pop(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libcthreads_queue_pop(
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

/* Tests the libcthreads_push function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_queue_push(
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

/* Tests the libcthreads_queue_push_sorted function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_queue_push_sorted(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libcthreads_queue_push_sorted(
	          NULL,
	          NULL,
	          NULL,
	          LIBCTHREADS_SORT_FLAG_NON_UNIQUE_VALUES,
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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
	 "libcthreads_queue_push",
	 cthreads_test_queue_push );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

