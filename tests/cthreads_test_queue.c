/*
 * Library queue type testing program
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "cthreads_test_libcthreads.h"
#include "cthreads_test_libcerror.h"
#include "cthreads_test_libcstring.h"

/* Tests initializing a queue
 * Make sure the value queue is referencing, is set to NULL
 * Returns 1 if successful, 0 if not or -1 on error
 */
int cthreads_test_queue_initialize(
     libcthreads_queue_t **queue,
     int expected_result )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_queue_initialize";
	int result               = 0;

	fprintf(
	 stdout,
	 "Testing initialize\t" );

	result = libcthreads_queue_initialize(
	          queue,
	          32,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create queue.",
		 function );
	}
	if( result != expected_result )
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result == -1 )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stdout );

		libcerror_error_free(
		 &error );
	}
	if( result == 1 )
	{
		if( libcthreads_queue_free(
		     queue,
		     &error ) == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free queue.",
			 function );

			libcerror_error_backtrace_fprint(
			 error,
			 stdout );

			libcerror_error_free(
			 &error );

			return( -1 );
		}
	}
	if( result != expected_result )
	{
		return( 0 );
	}
	return( 1 );
}

libcthreads_queue_t *cthreads_test_queue = NULL;
int cthreads_test_expected_queued_value  = 0;
int cthreads_test_queued_value           = 0;
int cthreads_test_number_of_iterations   = 497;
int cthreads_test_number_of_values       = 32;

/* The thread pop callback function
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_queue_pop_callback_function(
     void *arguments )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_queue_pop_callback_function";
	int *queued_value        = NULL;
	int iterator             = 0;

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
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to get value from queue.",
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
     void *arguments )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_queue_push_callback_function";
	int iterator             = 0;
	int queued_value         = 0;

	for( iterator = 0;
	     iterator < cthreads_test_number_of_iterations;
	     iterator++ )
	{
		queued_value = ( 98 * iterator ) % 45;

		if( libcthreads_queue_push(
		     cthreads_test_queue,
		     (intptr_t *) &queued_value,
		     &error ) == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to get value from queue.",
			 function );

			goto on_error;
		}
		cthreads_test_expected_queued_value += queued_value;
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

/* Tests thread queuing
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_queue_queuing(
     void )
{
	libcerror_error_t *error          = NULL;
	libcthreads_thread_t *pop_thread  = NULL;
	libcthreads_thread_t *push_thread = NULL;
	static char *function             = "cthreads_test_queue_queuing";
	int result                        = 0;

	cthreads_test_expected_queued_value = 0;
	cthreads_test_queued_value          = 0;

	if( libcthreads_queue_initialize(
	     &cthreads_test_queue,
	     cthreads_test_number_of_values,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create queue.",
		 function );

		goto on_error;
	}
	if( libcthreads_thread_create(
	     &push_thread,
	     NULL,
	     cthreads_test_queue_push_callback_function,
	     NULL,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create push thread.",
		 function );

		goto on_error;
	}
	if( libcthreads_thread_create(
	     &pop_thread,
	     NULL,
	     cthreads_test_queue_pop_callback_function,
	     NULL,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create pop thread.",
		 function );

		goto on_error;
	}
	if( libcthreads_thread_join(
	     &pop_thread,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to join pop thread.",
		 function );

		goto on_error;
	}
	if( libcthreads_thread_join(
	     &push_thread,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to join push thread.",
		 function );

		goto on_error;
	}
	if( libcthreads_queue_free(
	     &cthreads_test_queue,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free queue.",
		 function );

		goto on_error;
	}
	fprintf(
	 stdout,
	 "Testing queued value\t" );

	if( cthreads_test_queued_value != cthreads_test_expected_queued_value )
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	fprintf(
	 stdout,
	 "\n" );

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
		 NULL );
	}
	return( -1 );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcthreads_queue_t *queue = NULL;

	if( argc != 1 )
	{
		fprintf(
		 stderr,
		 "Unsupported number of arguments.\n" );

		return( EXIT_FAILURE );
	}
	/* Initialization tests
	 */
	queue = NULL;

	if( cthreads_test_queue_initialize(
	     &queue,
	     1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test initialize.\n" );

		return( EXIT_FAILURE );
	}
	queue = (libcthreads_queue_t *) 0x12345678UL;

	if( cthreads_test_queue_initialize(
	     &queue,
	     -1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test initialize.\n" );

		return( EXIT_FAILURE );
	}
	/* Test: queuing
	 */
	if( cthreads_test_queue_queuing() != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test queuing.\n" );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

