/*
 * Condition functions
 *
 * Copyright (C) 2012-2013, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include <errno.h>

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
#include <pthread.h>
#endif

#include "libcthreads_condition.h"
#include "libcthreads_libcerror.h"
#include "libcthreads_mutex.h"
#include "libcthreads_types.h"

/* Creates a condition
 * Make sure the value condition is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcthreads_condition_initialize(
     libcthreads_condition_t **condition,
     libcerror_error_t **error )
{
	libcthreads_internal_condition_t *internal_condition = NULL;
	static char *function                                = "libcthreads_condition_initialize";

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                                   = 0;
#endif

	if( condition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid condition.",
		 function );

		return( -1 );
	}
	if( *condition != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid condition value already set.",
		 function );

		return( -1 );
	}
	internal_condition = memory_allocate_structure(
	                      libcthreads_internal_condition_t );

	if( internal_condition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create condition.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_condition,
	     0,
	     sizeof( libcthreads_internal_condition_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear condition.",
		 function );

		goto on_error;
	}
#if defined( WINAPI )
/* TODO
	internal_condition->condition_handle = CreateMutex(
	                                        NULL,
	                                        FALSE,
	                                        NULL );

	if( internal_read_write_lock->condition_handle == NULL )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize condition handle.",
		 function );

		goto on_error;
	}
*/

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_cond_init(
		          &( internal_condition->condition ),
	                  NULL );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize condition.",
		 function );

		goto on_error;
	}
#endif
	*condition = (libcthreads_condition_t *) internal_condition;

	return( 1 );

on_error:
	if( internal_condition != NULL )
	{
		memory_free(
		 internal_condition );
	}
	return( -1 );
}

/* Frees a condition
 * Returns 1 if successful or -1 on error
 */
int libcthreads_condition_free(
     libcthreads_condition_t **condition,
     libcerror_error_t **error )
{
	libcthreads_internal_condition_t *internal_condition = NULL;
	static char *function                                = "libcthreads_condition_free";
	int result                                           = 1;

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                                   = 0;
#endif

	if( condition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid condition.",
		 function );

		return( -1 );
	}
	if( *condition != NULL )
	{
		internal_condition = (libcthreads_internal_condition_t *) *condition;
		*condition         = NULL;

#if defined( WINAPI )
/* TODO
		if( CloseHandle(
		     internal_read_write_lock->condition_handle ) == 0 )
		{
			error_code = GetLastError();

			libcerror_system_set_error(
			 error,
			 error_code,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free condition handle.",
			 function );

			result = -1;
		}
*/

#elif defined( HAVE_PTHREAD_H )
		pthread_result = pthread_cond_destroy(
		                  &( internal_condition->condition ) );

		if( pthread_result != 0 )
		{
			switch( pthread_result )
			{
				case EBUSY:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to destroy condition with error: Resource busy.",
					 function );

					break;

				default:
					libcerror_system_set_error(
					 error,
					 pthread_result,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to destroy condition.",
					 function );

					break;
			}
			result = -1;
		}
#endif
		memory_free(
		 internal_condition );
	}
	return( result );
}

/* Signals a condition
 * Returns 1 if successful or -1 on error
 */
int libcthreads_condition_signal(
     libcthreads_condition_t *condition,
     libcerror_error_t **error )
{
	libcthreads_internal_condition_t *internal_condition = NULL;
	static char *function                                = "libcthreads_condition_signal";

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                                   = 0;
#endif

	if( condition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid condition.",
		 function );

		return( -1 );
	}
	internal_condition = (libcthreads_internal_condition_t *) condition;

#if defined( WINAPI )
/* TODO
	wait_status = WaitForSingleObject(
	               internal_read_write_lock->condition_handle,
	               INFINITE );

	if( wait_status == WAIT_FAILED )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: wait for condition handle failed.",
		 function );

		return( -1 );
	}
*/

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_cond_signal(
	                  &( internal_condition->condition ) );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to signal condition.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Waits for a condition
 * Returns 1 if successful or -1 on error
 */
int libcthreads_condition_wait(
     libcthreads_condition_t *condition,
     libcthreads_mutex_t *mutex,
     libcerror_error_t **error )
{
	libcthreads_internal_condition_t *internal_condition = NULL;
	libcthreads_internal_mutex_t *internal_mutex         = NULL;
	static char *function                                = "libcthreads_condition_wait";

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                                   = 0;
#endif

	if( condition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid condition.",
		 function );

		return( -1 );
	}
	internal_condition = (libcthreads_internal_condition_t *) condition;

	if( mutex == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mutex.",
		 function );

		return( -1 );
	}
	internal_mutex = (libcthreads_internal_mutex_t *) mutex;

#if defined( WINAPI )
/* TODO
	wait_status = WaitForSingleObject(
	               internal_read_write_lock->condition_handle,
	               INFINITE );

	if( wait_status == WAIT_FAILED )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: wait for condition handle failed.",
		 function );

		return( -1 );
	}
*/

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_cond_wait(
	                  &( internal_condition->condition ),
	                  &( internal_mutex->mutex ) );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to wait for condition.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

