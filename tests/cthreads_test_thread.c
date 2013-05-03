/*
 * Library thread type testing program
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

/* The thread start function
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_thread_start_function(
     void *arguments )
{
	libcthreads_thread_exit();

	return( 1 );
}

/* Tests creating a thread
 * Make sure the value thread is referencing, is set to NULL
 * Returns 1 if successful, 0 if not or -1 on error
 */
int cthreads_test_thread_create(
     libcthreads_thread_t **thread,
     int (*start_function)(
            void *arguments ),
     int expected_result )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_thread_create";
	int result               = 0;

	fprintf(
	 stdout,
	 "Testing create\t" );

	result = libcthreads_thread_create(
	          thread,
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
		 "%s: unable to create thread.",
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

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcthreads_thread_t *thread = NULL;

	if( argc != 1 )
	{
		fprintf(
		 stderr,
		 "Unsupported number of arguments.\n" );

		return( EXIT_FAILURE );
	}
	/* Create tests
	 */
	thread = NULL;

	if( cthreads_test_thread_create(
	     &thread,
	     &cthreads_test_thread_start_function,
	     1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test create.\n" );

		return( EXIT_FAILURE );
	}
	thread = NULL;

	if( cthreads_test_thread_create(
	     &thread,
	     NULL,
	     -1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test create.\n" );

		return( EXIT_FAILURE );
	}
	thread = (libcthreads_thread_t *) 0x12345678UL;

	if( cthreads_test_thread_create(
	     &thread,
	     &cthreads_test_thread_start_function,
	     -1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test create.\n" );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

