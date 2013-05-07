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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "cthreads_test_libcthreads.h"
#include "cthreads_test_libcerror.h"
#include "cthreads_test_libcstring.h"

/* The thread pool callback function
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_thread_pool_callback_function(
     void *arguments )
{
	return( 1 );
}

/* Tests creating a thread pool
 * Make sure the value thread_pool is referencing, is set to NULL
 * Returns 1 if successful, 0 if not or -1 on error
 */
int cthreads_test_thread_pool_create(
     libcthreads_thread_pool_t **thread_pool,
     int (*callback_function)(
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

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcthreads_thread_pool_t *thread_pool = NULL;

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
	return( EXIT_SUCCESS );
}

