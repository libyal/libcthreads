/*
 * Library thread pool type test program
 *
 * Copyright (C) 2012-2018, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "cthreads_test_libcerror.h"
#include "cthreads_test_libcthreads.h"
#include "cthreads_test_macros.h"
#include "cthreads_test_memory.h"
#include "cthreads_test_unused.h"

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

/* Tests the libcthreads_thread_pool_create function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_thread_pool_create(
     void )
{
	libcerror_error_t *error               = NULL;
	libcthreads_thread_pool_t *thread_pool = NULL;
	int result                             = 0;

	/* Test libcthreads_thread_pool_create
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
	libcerror_error_t *error = NULL;
	int result               = 0;

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
	libcerror_error_t *error          = NULL;
	libcthreads_thread_t *thread_pool = NULL;
	int *queued_values                = NULL;
	static char *function             = "cthreads_test_thread_pool_push";
	int iterator                      = 0;
	int result                        = 0;

	cthreads_test_expected_queued_value = 0;
	cthreads_test_queued_value          = 0;

	queued_values = (int *) memory_allocate(
	                         sizeof( int ) * cthreads_test_number_of_iterations );

	if( queued_values == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create queued values.",
		 function );

		goto on_error;
	}
	if( libcthreads_lock_initialize(
	     &cthreads_test_lock,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create lock.",
		 function );

		goto on_error;
	}
	if( libcthreads_thread_pool_create(
	     &thread_pool,
	     NULL,
	     8,
	     cthreads_test_number_of_values,
	     (int (*)(intptr_t *, void *)) &cthreads_test_thread_pool_callback_function,
	     NULL,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create thread pool.",
		 function );

		goto on_error;
	}
	for( iterator = 0;
	     iterator < cthreads_test_number_of_iterations;
	     iterator++ )
	{
		queued_values[ iterator ] = ( 98 * iterator ) % 45;

		if( libcthreads_thread_pool_push(
		     thread_pool,
		     (intptr_t *) &( queued_values[ iterator ] ),
		     &error ) == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to push value onto queue.",
			 function );

			goto on_error;
		}
		cthreads_test_expected_queued_value += queued_values[ iterator ];
	}
	if( libcthreads_thread_pool_join(
	     &thread_pool,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to join thread pool.",
		 function );

		goto on_error;
	}
	if( libcthreads_lock_free(
	     &cthreads_test_lock,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free lock.",
		 function );

		goto on_error;
	}
	memory_free(
	 queued_values );

	queued_values = NULL;

	fprintf(
	 stdout,
	 "Testing queued value\t" );

	result = ( cthreads_test_queued_value == cthreads_test_expected_queued_value );

	if( result == 1 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	return( result );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stderr );

		libcerror_error_free(
		 &error );
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
	return( -1 );
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

	/* Test: thread_pool_push
	 */
	if( cthreads_test_thread_pool_push() != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test push.\n" );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

