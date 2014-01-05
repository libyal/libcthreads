/*
 * Library mutex type testing program
 *
 * Copyright (c) 2012-2014, Joachim Metz <joachim.metz@gmail.com>
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
#include "cthreads_test_unused.h"

/* Tests initializing a mutex
 * Make sure the value mutex is referencing, is set to NULL
 * Returns 1 if successful, 0 if not or -1 on error
 */
int cthreads_test_mutex_initialize(
     libcthreads_mutex_t **mutex,
     int expected_result )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_mutex_initialize";
	int result               = 0;

	fprintf(
	 stdout,
	 "Testing initialize\t" );

	result = libcthreads_mutex_initialize(
	          mutex,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create mutex.",
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
		if( libcthreads_mutex_free(
		     mutex,
		     &error ) == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free mutex.",
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

libcthreads_mutex_t *cthreads_test_mutex = NULL;
int cthreads_test_mutexed_value          = 0;

/* The thread1 callback function
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_mutex_callback_function1(
     void *arguments CTHREADS_TEST_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_mutex_callback_function1";
	int result               = 0;

	CTHREADS_TEST_UNREFERENCED_PARAMETER( arguments )

	result = libcthreads_mutex_grab(
	          cthreads_test_mutex,
	          &error );

	if( result != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab mutex.",
		 function );

		goto on_error;
	}
	cthreads_test_mutexed_value += 19;

	result = libcthreads_mutex_release(
		  cthreads_test_mutex,
		  &error );

	if( result != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release mutex.",
		 function );

		goto on_error;
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

/* The thread2 callback function
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_mutex_callback_function2(
     void *arguments CTHREADS_TEST_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_mutex_callback_function2";
	int result               = 0;

	CTHREADS_TEST_UNREFERENCED_PARAMETER( arguments )

	result = libcthreads_mutex_grab(
	          cthreads_test_mutex,
	          &error );

	if( result != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab mutex.",
		 function );

		goto on_error;
	}
	cthreads_test_mutexed_value += 38;

	result = libcthreads_mutex_release(
		  cthreads_test_mutex,
		  &error );

	if( result != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release mutex.",
		 function );

		goto on_error;
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

/* Tests thread mutex locking
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_mutex_locking(
     void )
{
	libcerror_error_t *error      = NULL;
	libcthreads_thread_t *thread1 = NULL;
	libcthreads_thread_t *thread2 = NULL;
	static char *function         = "cthreads_test_mutex_locking";
	int result                    = 0;

	if( libcthreads_mutex_initialize(
	     &cthreads_test_mutex,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create mutex.",
		 function );

		goto on_error;
	}
	fprintf(
	 stdout,
	 "Testing grab\t" );

	result = libcthreads_mutex_grab(
	          cthreads_test_mutex,
	          &error );

	if( result != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab mutex.",
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

	if( result == 1 )
	{
		if( libcthreads_thread_create(
		     &thread1,
		     NULL,
		     cthreads_test_mutex_callback_function1,
		     NULL,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create thread1.",
			 function );

			goto on_error;
		}
		if( libcthreads_thread_create(
		     &thread2,
		     NULL,
		     cthreads_test_mutex_callback_function2,
		     NULL,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create thread2.",
			 function );

			goto on_error;
		}
		cthreads_test_mutexed_value = 46;

		fprintf(
		 stdout,
		 "Testing release\t" );

		result = libcthreads_mutex_release(
			  cthreads_test_mutex,
			  &error );

		if( result != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to release mutex.",
			 function );
		}
		if( result != 1 )
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
	}
	if( result == -1 )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stdout );

		libcerror_error_free(
		 &error );
	}
	if( libcthreads_thread_join(
	     &thread2,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to join thread2.",
		 function );

		goto on_error;
	}
	if( libcthreads_thread_join(
	     &thread1,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to join thread1.",
		 function );

		goto on_error;
	}
	if( libcthreads_mutex_free(
	     &cthreads_test_mutex,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free mutex.",
		 function );

		goto on_error;
	}
	fprintf(
	 stdout,
	 "Testing mutexed value\t" );

	if( cthreads_test_mutexed_value != ( 46 + 19 + 38 ) )
	{
		fprintf(
		 stdout,
		 "(FAIL)" );

		result = 0;
	}
	else
	{
		fprintf(
		 stdout,
		 "(PASS)" );

		result = 1;
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
		 stdout );

		libcerror_error_free(
		 &error );
	}
	if( thread2 != NULL )
	{
		libcthreads_thread_join(
		 &thread2,
		 NULL );
	}
	if( thread1 != NULL )
	{
		libcthreads_thread_join(
		 &thread1,
		 NULL );
	}
	if( cthreads_test_mutex != NULL )
	{
		libcthreads_mutex_release(
		 cthreads_test_mutex,
		 NULL );
		libcthreads_mutex_free(
		 &cthreads_test_mutex,
		 NULL );
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
	libcthreads_mutex_t *mutex = NULL;

	CTHREADS_TEST_UNREFERENCED_PARAMETER( argv )

	if( argc != 1 )
	{
		fprintf(
		 stderr,
		 "Unsupported number of arguments.\n" );

		return( EXIT_FAILURE );
	}
	/* Initialization tests
	 */
	mutex = NULL;

	if( cthreads_test_mutex_initialize(
	     &mutex,
	     1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test initialize.\n" );

		return( EXIT_FAILURE );
	}
	mutex = (libcthreads_mutex_t *) 0x12345678UL;

	if( cthreads_test_mutex_initialize(
	     &mutex,
	     -1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test initialize.\n" );

		return( EXIT_FAILURE );
	}
	/* Test: locking
	 */
	if( cthreads_test_mutex_locking() != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test locking.\n" );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

