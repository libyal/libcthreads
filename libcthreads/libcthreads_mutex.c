/*
 * Mutex functions
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

#include "libcthreads_libcerror.h"
#include "libcthreads_mutex.h"
#include "libcthreads_types.h"

/* Creates a mutex
 * Make sure the value mutex is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcthreads_mutex_initialize(
     libcthreads_mutex_t **mutex,
     libcerror_error_t **error )
{
	libcthreads_internal_mutex_t *internal_mutex = NULL;
	static char *function                        = "libcthreads_mutex_initialize";

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                           = 0;
#endif

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
	if( *mutex != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid mutex value already set.",
		 function );

		return( -1 );
	}
	internal_mutex = memory_allocate_structure(
	                  libcthreads_internal_mutex_t );

	if( internal_mutex == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create mutex.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_mutex,
	     0,
	     sizeof( libcthreads_internal_mutex_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear mutex.",
		 function );

		goto on_error;
	}
#if defined( WINAPI )
	InitializeCriticalSection(
	 &( internal_mutex->critical_section ) );

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_mutex_init(
	                  &( internal_mutex->mutex ),
	                  NULL );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize mutex.",
		 function );

		goto on_error;
	}
#endif
	*mutex = (libcthreads_mutex_t *) internal_mutex;

	return( 1 );

on_error:
	if( internal_mutex != NULL )
	{
		memory_free(
		 internal_mutex );
	}
	return( -1 );
}

/* Frees a mutex
 * Returns 1 if successful or -1 on error
 */
int libcthreads_mutex_free(
     libcthreads_mutex_t **mutex,
     libcerror_error_t **error )
{
	libcthreads_internal_mutex_t *internal_mutex = NULL;
	static char *function                        = "libcthreads_mutex_free";
	int result                                   = 1;

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                           = 0;
#endif

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
	if( *mutex != NULL )
	{
		internal_mutex = (libcthreads_internal_mutex_t *) *mutex;
		*mutex         = NULL;

#if defined( WINAPI )
		DeleteCriticalSection(
		 &( internal_mutex->critical_section ) );

#elif defined( HAVE_PTHREAD_H )
		pthread_result = pthread_mutex_destroy(
		                  &( internal_mutex->mutex ) );

		if( pthread_result != 0 )
		{
			switch( pthread_result )
			{
				case EBUSY:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to destroy mutex with error: Resource busy.",
					 function );

					break;

				default:
					libcerror_system_set_error(
					 error,
					 pthread_result,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to destroy mutex.",
					 function );

					break;
			}
			result = -1;
		}
#endif
		memory_free(
		 internal_mutex );
	}
	return( result );
}

/* Grabs a mutex
 * Returns 1 if successful or -1 on error
 */
int libcthreads_mutex_grab(
     libcthreads_mutex_t *mutex,
     libcerror_error_t **error )
{
	libcthreads_internal_mutex_t *internal_mutex = NULL;
	static char *function                        = "libcthreads_mutex_grab";

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                           = 0;
#endif

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
	EnterCriticalSection(
	 &( internal_mutex->critical_section ) );

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_mutex_lock(
	                  &( internal_mutex->mutex ) );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to lock mutex.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Releases a mutex
 * Returns 1 if successful or -1 on error
 */
int libcthreads_mutex_release(
     libcthreads_mutex_t *mutex,
     libcerror_error_t **error )
{
	libcthreads_internal_mutex_t *internal_mutex = NULL;
	static char *function                        = "libcthreads_mutex_release";

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                           = 0;
#endif

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
	LeaveCriticalSection(
	 &( internal_mutex->critical_section ) );

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_mutex_unlock(
	                  &( internal_mutex->mutex ) );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to unlock mutex.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

