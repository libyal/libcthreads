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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <errno.h>

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )
#define __USE_GNU
#include <dlfcn.h>
#undef __USE_GNU
#endif

#include "cthreads_test_libcerror.h"
#include "cthreads_test_libcthreads.h"
#include "cthreads_test_macros.h"
#include "cthreads_test_memory.h"
#include "cthreads_test_unused.h"

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )

static int (*cthreads_test_real_pthread_cond_init)(pthread_cond_t *, const pthread_condattr_t *) = NULL;
static int (*cthreads_test_real_pthread_cond_destroy)(pthread_cond_t *)                          = NULL;
static int (*cthreads_test_real_pthread_cond_broadcast)(pthread_cond_t *)                        = NULL;
static int (*cthreads_test_real_pthread_cond_signal)(pthread_cond_t *)                           = NULL;
static int (*cthreads_test_real_pthread_cond_wait)(pthread_cond_t *, pthread_mutex_t *)          = NULL;

int cthreads_test_pthread_cond_init_attempts_before_fail                                         = -1;
int cthreads_test_pthread_cond_destroy_attempts_before_fail                                      = -1;
int cthreads_test_pthread_cond_broadcast_attempts_before_fail                                    = -1;
int cthreads_test_pthread_cond_signal_attempts_before_fail                                       = -1;
int cthreads_test_pthread_cond_wait_attempts_before_fail                                         = -1;

#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) */

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )

/* Custom pthread_cond_init for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_cond_init(
     pthread_cond_t *cond,
     const pthread_condattr_t *attr )
{
	int result = 0;

	if( cthreads_test_real_pthread_cond_init == NULL )
	{
		cthreads_test_real_pthread_cond_init = dlsym(
		                                        RTLD_NEXT,
		                                        "pthread_cond_init" );
	}
	if( cthreads_test_pthread_cond_init_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_cond_init_attempts_before_fail = -1;

		return( EBUSY );
	}
	else if( cthreads_test_pthread_cond_init_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_cond_init_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_cond_init(
	          cond,
	          attr );

	return( result );
}

/* Custom pthread_cond_destroy for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_cond_destroy(
     pthread_cond_t *cond )
{
	int result = 0;

	if( cthreads_test_real_pthread_cond_destroy == NULL )
	{
		cthreads_test_real_pthread_cond_destroy = dlsym(
		                                           RTLD_NEXT,
		                                           "pthread_cond_destroy" );
	}
	if( cthreads_test_pthread_cond_destroy_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_cond_destroy_attempts_before_fail = -1;

		return( EBUSY );
	}
	else if( cthreads_test_pthread_cond_destroy_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_cond_destroy_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_cond_destroy(
	          cond );

	return( result );
}

/* Custom pthread_cond_broadcast for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_cond_broadcast(
     pthread_cond_t *cond )
{
	int result = 0;

	if( cthreads_test_real_pthread_cond_broadcast == NULL )
	{
		cthreads_test_real_pthread_cond_broadcast = dlsym(
		                                             RTLD_NEXT,
		                                             "pthread_cond_broadcast" );
	}
	if( cthreads_test_pthread_cond_broadcast_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_cond_broadcast_attempts_before_fail = -1;

		return( EBUSY );
	}
	else if( cthreads_test_pthread_cond_broadcast_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_cond_broadcast_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_cond_broadcast(
	          cond );

	return( result );
}

/* Custom pthread_cond_signal for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_cond_signal(
     pthread_cond_t *cond )
{
	int result = 0;

	if( cthreads_test_real_pthread_cond_signal == NULL )
	{
		cthreads_test_real_pthread_cond_signal = dlsym(
		                                          RTLD_NEXT,
		                                          "pthread_cond_signal" );
	}
	if( cthreads_test_pthread_cond_signal_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_cond_signal_attempts_before_fail = -1;

		return( EBUSY );
	}
	else if( cthreads_test_pthread_cond_signal_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_cond_signal_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_cond_signal(
	          cond );

	return( result );
}

/* Custom pthread_cond_wait for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_cond_wait(
     pthread_cond_t *cond,
     pthread_mutex_t *mutex )
{
	int result = 0;

	if( cthreads_test_real_pthread_cond_wait == NULL )
	{
		cthreads_test_real_pthread_cond_wait = dlsym(
		                                        RTLD_NEXT,
		                                        "pthread_cond_wait" );
	}
	if( cthreads_test_pthread_cond_wait_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_cond_wait_attempts_before_fail = -1;

		return( EBUSY );
	}
	else if( cthreads_test_pthread_cond_wait_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_cond_wait_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_cond_wait(
	          cond,
	          mutex );

	return( result );
}

#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) */

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

		if( condition != NULL )
		{
			libcthreads_condition_free(
			 &condition,
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

		if( condition != NULL )
		{
			libcthreads_condition_free(
			 &condition,
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
		 "condition",
		 condition );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_CTHREADS_TEST_MEMORY ) */

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )

	/* Test libcthreads_condition_initialize with pthread_cond_init failing
	 */
	cthreads_test_pthread_cond_init_attempts_before_fail = 0;

	result = libcthreads_condition_initialize(
	          &condition,
	          &error );

	if( cthreads_test_pthread_cond_init_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_cond_init_attempts_before_fail = -1;

		if( condition != NULL )
		{
			libcthreads_condition_free(
			 &condition,
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
		 "condition",
		 condition );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) */

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
	libcerror_error_t *error           = NULL;
	int result                         = 0;

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )
	libcthreads_condition_t *condition = NULL;
#endif

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

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )

	/* Initialize test
	 */
	result = libcthreads_condition_initialize(
	          &condition,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test libcthreads_condition_free with pthread_cond_destroy failing
	 */
	cthreads_test_pthread_cond_destroy_attempts_before_fail = 0;

	result = libcthreads_condition_free(
	          &condition,
	          &error );

	if( cthreads_test_pthread_cond_destroy_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_cond_destroy_attempts_before_fail = -1;

		if( condition != NULL )
		{
			libcthreads_condition_free(
			 &condition,
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
		 "condition",
		 condition );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )
	if( condition != NULL )
	{
		libcthreads_condition_free(
		 &condition,
		 NULL );
	}
#endif
	return( 0 );
}

/* Tests the libcthreads_condition_broadcast function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_condition_broadcast(
     void )
{
	libcerror_error_t *error           = NULL;
	libcthreads_condition_t *condition = NULL;
	int result                         = 0;

	/* Initialize test
	 */
	result = libcthreads_condition_initialize(
	          &condition,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test broadcast
	 */
	result = libcthreads_condition_broadcast(
	          condition,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libcthreads_condition_broadcast(
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

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )

	/* Test libcthreads_condition_broadcast with pthread_cond_broadcast failing
	 */
	cthreads_test_pthread_cond_broadcast_attempts_before_fail = 0;

	result = libcthreads_condition_broadcast(
	          condition,
	          &error );

	if( cthreads_test_pthread_cond_broadcast_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_cond_broadcast_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) */

	/* Clean up
	 */
	result = libcthreads_condition_free(
	          &condition,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

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

/* Tests the libcthreads_condition_signal function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_condition_signal(
     void )
{
	libcerror_error_t *error           = NULL;
	libcthreads_condition_t *condition = NULL;
	int result                         = 0;

	/* Initialize test
	 */
	result = libcthreads_condition_initialize(
	          &condition,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test signal
	 */
	result = libcthreads_condition_signal(
	          condition,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libcthreads_condition_signal(
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

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )

	/* Test libcthreads_condition_signal with pthread_cond_signal failing
	 */
	cthreads_test_pthread_cond_signal_attempts_before_fail = 0;

	result = libcthreads_condition_signal(
	          condition,
	          &error );

	if( cthreads_test_pthread_cond_signal_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_cond_signal_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) */

	/* Clean up
	 */
	result = libcthreads_condition_free(
	          &condition,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

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

/* Tests the libcthreads_condition_wait function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_condition_wait(
     void )
{
	libcerror_error_t *error           = NULL;
	libcthreads_condition_t *condition = NULL;
	libcthreads_mutex_t *mutex         = NULL;
	int result                         = 0;

	/* Initialize test
	 */
	result = libcthreads_condition_initialize(
	          &condition,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libcthreads_mutex_initialize(
	          &mutex,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CTHREADS_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libcthreads_condition_wait(
	          NULL,
	          mutex,
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

	result = libcthreads_condition_wait(
	          condition,
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

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ )

	/* Test libcthreads_condition_wait with pthread_cond_wait failing
	 */
	cthreads_test_pthread_cond_wait_attempts_before_fail = 0;

	result = libcthreads_condition_wait(
	          condition,
	          mutex,
	          &error );

	if( cthreads_test_pthread_cond_wait_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_cond_wait_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) */

	/* Clean up
	 */
	result = libcthreads_mutex_free(
	          &mutex,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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
         "error",
         error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( mutex != NULL )
	{
		libcthreads_mutex_free(
		 &mutex,
		 NULL );
	}
	if( condition != NULL )
	{
		libcthreads_condition_free(
		 &condition,
		 NULL );
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
	 "libcthreads_condition_initialize",
	 cthreads_test_condition_initialize );

	CTHREADS_TEST_RUN(
	 "libcthreads_condition_free",
	 cthreads_test_condition_free );

	CTHREADS_TEST_RUN(
	 "libcthreads_condition_broadcast",
	 cthreads_test_condition_broadcast );

	CTHREADS_TEST_RUN(
	 "libcthreads_condition_signal",
	 cthreads_test_condition_signal );

	CTHREADS_TEST_RUN(
	 "libcthreads_condition_wait",
	 cthreads_test_condition_wait );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

