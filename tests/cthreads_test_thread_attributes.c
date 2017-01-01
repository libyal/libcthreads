/*
 * Library thread_attributes type test program
 *
 * Copyright (C) 2012-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include "cthreads_test_libcthreads.h"
#include "cthreads_test_macros.h"
#include "cthreads_test_memory.h"
#include "cthreads_test_unused.h"

/* Tests the libcthreads_thread_attributes_initialize function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_thread_attributes_initialize(
     void )
{
	libcerror_error_t *error                           = NULL;
	libcthreads_thread_attributes_t *thread_attributes = NULL;
	int result                                         = 0;

#if defined( HAVE_CTHREADS_TEST_MEMORY )
	int number_of_malloc_fail_tests                    = 1;
	int number_of_memset_fail_tests                    = 1;
	int test_number                                    = 0;
#endif

	/* Test regular cases
	 */
	result = libcthreads_thread_attributes_initialize(
	          &thread_attributes,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NOT_NULL(
         "thread_attributes",
         thread_attributes );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libcthreads_thread_attributes_free(
	          &thread_attributes,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "thread_attributes",
         thread_attributes );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libcthreads_thread_attributes_initialize(
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

	thread_attributes = (libcthreads_thread_attributes_t *) 0x12345678UL;

	result = libcthreads_thread_attributes_initialize(
	          &thread_attributes,
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

	thread_attributes = NULL;

#if defined( HAVE_CTHREADS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libcthreads_thread_attributes_initialize with malloc failing
		 */
		cthreads_test_malloc_attempts_before_fail = test_number;

		result = libcthreads_thread_attributes_initialize(
		          &thread_attributes,
		          &error );

		if( cthreads_test_malloc_attempts_before_fail != -1 )
		{
			cthreads_test_malloc_attempts_before_fail = -1;

			if( thread_attributes != NULL )
			{
				libcthreads_thread_attributes_free(
				 &thread_attributes,
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
			 "thread_attributes",
			 thread_attributes );

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
		/* Test libcthreads_thread_attributes_initialize with memset failing
		 */
		cthreads_test_memset_attempts_before_fail = test_number;

		result = libcthreads_thread_attributes_initialize(
		          &thread_attributes,
		          &error );

		if( cthreads_test_memset_attempts_before_fail != -1 )
		{
			cthreads_test_memset_attempts_before_fail = -1;

			if( thread_attributes != NULL )
			{
				libcthreads_thread_attributes_free(
				 &thread_attributes,
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
			 "thread_attributes",
			 thread_attributes );

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
	if( thread_attributes != NULL )
	{
		libcthreads_thread_attributes_free(
		 &thread_attributes,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_thread_attributes_free function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_thread_attributes_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libcthreads_thread_attributes_free(
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
	 "libcthreads_thread_attributes_initialize",
	 cthreads_test_thread_attributes_initialize );

	CTHREADS_TEST_RUN(
	 "libcthreads_thread_attributes_free",
	 cthreads_test_thread_attributes_free );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

