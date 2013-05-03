/*
 * Thread functions
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
#include "libcthreads_thread.h"
#include "libcthreads_thread_attributes.h"
#include "libcthreads_types.h"

/* Creates a thread
 * Make sure the value thread is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcthreads_thread_create(
     libcthreads_thread_t **thread,
     const libcthreads_thread_attributes_t *thread_attributes,
     int (*start_function)(
            void *arguments),
     void *start_function_arguments,
     libcerror_error_t **error )
{
	libcthreads_internal_thread_t *internal_thread = NULL;
	static char *function                          = "libcthreads_thread_create";

#if defined( WINAPI )
	SECURITY_ATTRIBUTES *security_attributes       = NULL;
	DWORD error_code                               = 0;

#elif defined( HAVE_PTHREAD_H )
	pthread_attr_t *attributes                     = NULL;
	int result                                     = 0;
#endif

	if( thread == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid thread.",
		 function );

		return( -1 );
	}
	if( *thread != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid thread value already set.",
		 function );

		return( -1 );
	}
	if( start_function == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid start function.",
		 function );

		return( -1 );
	}
	internal_thread = memory_allocate_structure(
	                   libcthreads_internal_thread_t );

	if( internal_thread == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create thread.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_thread,
	     0,
	     sizeof( libcthreads_internal_thread_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear thread.",
		 function );

		goto on_error;
	}
#if defined( WINAPI )
	if( thread_attributes != NULL )
	{
		security_attributes = &( ( (libcthreads_internal_thread_attributes_t *) thread_attributes )->security_attributes );
	}
	internal_thread->thread_handle = CreateThread(
	                                  security_attributes,
	                                  0, /* stack size */
	                                  (PTHREAD_START_ROUTINE) start_function,
	                                  start_function_arguments,
	                                  0, /* creation flags */
	                                  &( internal_thread->thread_identifier ) );

	if(  internal_thread->thread_handle == NULL )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create thread handle.",
		 function );

		goto on_error;
	}
#elif defined( HAVE_PTHREAD_H )
	if( thread_attributes != NULL )
	{
		attributes = &( ( (libcthreads_internal_thread_attributes_t *) thread_attributes )->attributes );
	}
	result = pthread_create(
		  &( internal_thread->thread ),
	          attributes,
	          (void* (*)(void *)) start_function,
	          start_function_arguments );

	if( result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 errno,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create thread.",
		 function );

		goto on_error;
	}
#endif
	*thread = (libcthreads_thread_t *) internal_thread;

	return( 1 );

on_error:
	if( internal_thread != NULL )
	{
		memory_free(
		 internal_thread );
	}
	return( -1 );
}

/* Exits a thread
 * Call this function within the thread
 */
void libcthreads_thread_exit(
      void )
{
#if defined( WINAPI )
	ExitThread(
	 0 );

#elif defined( HAVE_PTHREAD_H )
	pthread_exit(
	 NULL );
#endif
}

