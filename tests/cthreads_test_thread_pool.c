/*
 * Library thread pool type testing program
 *
 * Copyright (c) 2012-2013, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "cthreads_test_libcthreads.h"
#include "cthreads_test_libcerror.h"
#include "cthreads_test_libcstring.h"
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
	cthreads_test_queued_value += *value;

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

/* Tests creating a thread pool
 * Make sure the value thread_pool is referencing, is set to NULL
 * Returns 1 if successful, 0 if not or -1 on error
 */
int cthreads_test_thread_pool_create(
     libcthreads_thread_pool_t **thread_pool,
     int (*callback_function)(
            intptr_t *value,
            void *arguments ),
     int expected_result )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_thread_pool_create";
	int result               = 0;

	fprintf(
	 stdout,
	 "Testing create\t" );

	result = libcthreads_thread_pool_create(
	          thread_pool,
	          NULL,
	          8,
	          32,
	          callback_function,
	          NULL,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create thread pool.",
		 function );
	}
	result = ( expected_result == result );

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

	if( error != NULL )
	{
		if( result != 1 )
		{
			libcerror_error_backtrace_fprint(
			 error,
			 stderr );
		}
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Tests joining a thread pool
 * Returns 1 if successful, 0 if not or -1 on error
 */
int cthreads_test_thread_pool_join(
     libcthreads_thread_pool_t **thread_pool )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_thread_pool_join";
	int result               = 0;

	fprintf(
	 stdout,
	 "Testing join\t" );

	result = libcthreads_thread_pool_join(
		  thread_pool,
		  &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to join thread pool.",
		 function );
	}
	result = ( result == 1 );

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

	if( error != NULL )
	{
		if( result != 1 )
		{
			libcerror_error_backtrace_fprint(
			 error,
			 stderr );
		}
		libcerror_error_free(
		 &error );
	}
	return( result );
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
	     &cthreads_test_thread_pool_callback_function,
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

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] CTHREADS_TEST_ATTRIBUTE_UNUSED )
#else
int main( int argc, char * const argv[] CTHREADS_TEST_ATTRIBUTE_UNUSED )
#endif
{
	libcthreads_thread_pool_t *thread_pool = NULL;

	CTHREADS_TEST_UNREFERENCED_PARAMETER( argv )

	if( argc != 1 )
	{
		fprintf(
		 stderr,
		 "Unsupported number of arguments.\n" );

		return( EXIT_FAILURE );
	}
	/* Create tests
	 */
	thread_pool = NULL;

	if( cthreads_test_thread_pool_create(
	     &thread_pool,
	     &cthreads_test_thread_pool_callback_function,
	     1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test create.\n" );

		return( EXIT_FAILURE );
	}
	if( cthreads_test_thread_pool_join(
	     &thread_pool ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test join.\n" );

		return( EXIT_FAILURE );
	}
	thread_pool = NULL;

	if( cthreads_test_thread_pool_create(
	     &thread_pool,
	     NULL,
	     -1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test create.\n" );

		return( EXIT_FAILURE );
	}
	thread_pool = (libcthreads_thread_pool_t *) 0x12345678UL;

	if( cthreads_test_thread_pool_create(
	     &thread_pool,
	     &cthreads_test_thread_pool_callback_function,
	     -1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test create.\n" );

		return( EXIT_FAILURE );
	}
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
}

