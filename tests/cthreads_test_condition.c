/*
 * Library condition type testing program
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "cthreads_test_libcerror.h"
#include "cthreads_test_libcstring.h"
#include "cthreads_test_libcthreads.h"
#include "cthreads_test_macros.h"
#include "cthreads_test_memory.h"
#include "cthreads_test_unused.h"

/* Tests the libcthreads_condition_initialize function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_condition_initialize(
     void )
{
	libcerror_error_t *error           = NULL;
	libcthreads_condition_t *condition = NULL;
	int result                         = 0;

	/* Test libcthreads_condition_initialize
	 */
	result = libcthreads_condition_initialize(
	          &condition,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NOT_NULL(
         "condition",
         condition );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libcthreads_condition_free(
	          &condition,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "condition",
         condition );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libcthreads_condition_initialize(
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

	condition = (libcthreads_condition_t *) 0x12345678UL;

	result = libcthreads_condition_initialize(
	          &condition,
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

	condition = NULL;

#if defined( HAVE_CTHREADS_TEST_MEMORY )

	/* Test libcthreads_condition_initialize with malloc failing
	 */
	cthreads_test_malloc_attempts_before_fail = 0;

	result = libcthreads_condition_initialize(
	          &condition,
	          &error );

	if( cthreads_test_malloc_attempts_before_fail != -1 )
	{
		cthreads_test_malloc_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "condition",
		 condition );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libcthreads_condition_initialize with memset failing
	 */
	cthreads_test_memset_attempts_before_fail = 0;

	result = libcthreads_condition_initialize(
	          &condition,
	          &error );

	if( cthreads_test_memset_attempts_before_fail != -1 )
	{
		cthreads_test_memset_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "condition",
		 condition );

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
	if( condition != NULL )
	{
		libcthreads_condition_free(
		 &condition,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_condition_free function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_condition_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libcthreads_condition_free(
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
	 "libcthreads_condition_initialize",
	 cthreads_test_condition_initialize );

	CTHREADS_TEST_RUN(
	 "libcthreads_condition_free",
	 cthreads_test_condition_free );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

