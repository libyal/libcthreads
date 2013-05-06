/*
 * Library repeating thread type testing program
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

int cthreads_test_locked_value = 0;

/* The repeating thread start function
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_repeating_thread_start_function(
     void *arguments )
{
	cthreads_test_locked_value += 21;

	return( 1 );
}

/* Tests creating a repeating thread
 * Make sure the value repeating_thread is referencing, is set to NULL
 * Returns 1 if successful, 0 if not or -1 on error
 */
int cthreads_test_repeating_thread_create(
     libcthreads_repeating_thread_t **repeating_thread,
     int (*start_function)(
            void *arguments ),
     int expected_result )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_repeating_thread_create";
	int result               = 0;

	fprintf(
	 stdout,
	 "Testing create\t" );

	result = libcthreads_repeating_thread_create(
	          repeating_thread,
	          NULL,
	          start_function,
	          NULL,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create repeating thread.",
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
	if( result != expected_result )
	{
		return( 0 );
	}
	return( 1 );
}

/* Tests joining a repeating thread
 * Returns 1 if successful, 0 if not or -1 on error
 */
int cthreads_test_repeating_thread_join(
     libcthreads_repeating_thread_t **repeating_thread )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_repeating_thread_join";
	int result               = 0;

	fprintf(
	 stdout,
	 "Testing join\t" );

	result = libcthreads_repeating_thread_join(
		  repeating_thread,
		  &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to join repeating thread.",
		 function );

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

		return( 0 );
	}
	return( 1 );
}

/* Tests thread lock run
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_repeating_thread_run(
     void )
{
	libcerror_error_t *error                         = NULL;
	libcthreads_repeating_thread_t *repeating_thread = NULL;
	static char *function                            = "cthreads_test_repeating_thread_run";
	int result                                       = 0;

	cthreads_test_locked_value = 31;

	if( libcthreads_repeating_thread_create(
	     &repeating_thread,
	     NULL,
	     &cthreads_test_repeating_thread_start_function,
	     NULL,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create repeating thread.",
		 function );

		goto on_error;
	}
/* TODO */
	if( libcthreads_repeating_thread_push(
	     repeating_thread,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to push repeating thread.",
		 function );

		goto on_error;
	}
	if( libcthreads_repeating_thread_join(
	     &repeating_thread,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to join repeating thread.",
		 function );

		goto on_error;
	}
	fprintf(
	 stdout,
	 "Testing locked value\t" );

	if( cthreads_test_locked_value != ( 31 + 21 ) )
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
	libcthreads_repeating_thread_t *repeating_thread = NULL;

	if( argc != 1 )
	{
		fprintf(
		 stderr,
		 "Unsupported number of arguments.\n" );

		return( EXIT_FAILURE );
	}
	/* Create tests
	 */
	repeating_thread = NULL;

	if( cthreads_test_repeating_thread_create(
	     &repeating_thread,
	     &cthreads_test_repeating_thread_start_function,
	     1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test create.\n" );

		return( EXIT_FAILURE );
	}
	if( cthreads_test_repeating_thread_join(
	     &repeating_thread ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test join.\n" );

		return( EXIT_FAILURE );
	}
	repeating_thread = NULL;

	if( cthreads_test_repeating_thread_create(
	     &repeating_thread,
	     NULL,
	     -1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test create.\n" );

		return( EXIT_FAILURE );
	}
	repeating_thread = (libcthreads_repeating_thread_t *) 0x12345678UL;

	if( cthreads_test_repeating_thread_create(
	     &repeating_thread,
	     &cthreads_test_repeating_thread_start_function,
	     -1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test create.\n" );

		return( EXIT_FAILURE );
	}
	/* Test: run
	 */
	if( cthreads_test_repeating_thread_run() != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test run.\n" );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

