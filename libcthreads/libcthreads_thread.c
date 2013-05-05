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

#if defined( WINAPI ) && ( WINVER >= 0x0602 ) && !defined( USE_CRT_FUNCTIONS )
#include <Processthreadsapi.h>
#include <Synchapi.h>
#endif

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
#include <pthread.h>
#endif

#include "libcthreads_libcerror.h"
#include "libcthreads_thread.h"
#include "libcthreads_thread_attributes.h"
#include "libcthreads_types.h"

typedef int (*libcthreads_thread_start_function_t)( void *arguments );

#if defined( WINAPI )
/* Start function helper function for WINAPI
 * Returns 0 if successful or 1 on error
 */
DWORD WINAPI libcthreads_thread_start_function_helper(
              void *arguments )
{
	libcthreads_thread_start_function_t start_function = NULL;
	void *start_function_arguments                     = NULL;
	int result                                         = 0;

	if( arguments == NULL )
	{
		return( 1 );
	}
	start_function = (libcthreads_thread_start_function_t) *( (intptr_t *) arguments );

	if( start_function == NULL )
	{
		return( 1 );
	}
	arguments++;

	start_function_arguments = (void *) *( (intptr_t *) arguments );
	arguments++;

	if( *( (intptr_t *) arguments ) != (intptr_t) NULL )
	{
		return( 1 );
	}
	result = start_function(
	          start_function_arguments );

	if( result != 1 )
	{
		return( 1 );
	}
	return( 0 );
}

#elif defined( HAVE_PTHREAD_H )
/* Start function helper function for pthread
 * Returns a pointer to the start function result if successful or NULL on error
 */
void *libcthreads_thread_start_function_helper(
       void *arguments )
{
	libcthreads_thread_start_function_t start_function = NULL;
	void *start_function_arguments                     = NULL;
	int *start_function_result                         = NULL;
	int result                                         = 0;

	if( arguments == NULL )
	{
		return( NULL );
	}
	start_function = (libcthreads_thread_start_function_t) *( (intptr_t *) arguments );

	if( start_function == NULL )
	{
		return( NULL );
	}
	arguments++;

	start_function_arguments = (void *) *( (intptr_t *) arguments );
	arguments++;

	start_function_result = (int *) *( (intptr_t *) arguments );

	if( start_function_result == NULL )
	{
		return( NULL );
	}
	arguments++;

	if( *( (intptr_t *) arguments ) != (intptr_t) NULL )
	{
		return( NULL );
	}
	result = start_function(
	          start_function_arguments );

	*start_function_result = result;

	return( (void *) start_function_result );
}

#endif

/* Creates a thread
 * Make sure the value thread is referencing, is set to NULL
 *
 * The start_function should return 1 if successful and -1 on error
 * Returns 1 if successful or -1 on error
 */
int libcthreads_thread_create(
     libcthreads_thread_t **thread,
     const libcthreads_thread_attributes_t *thread_attributes,
     int (*start_function)(
            void *arguments ),
     void *start_function_arguments,
     libcerror_error_t **error )
{
	void *start_function_helper_arguments[ 4 ];

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
	start_function_helper_arguments[ 0 ] = (void *) start_function;
	start_function_helper_arguments[ 1 ] = start_function_arguments;
	start_function_helper_arguments[ 2 ] = NULL;

#if defined( WINAPI )
	if( thread_attributes != NULL )
	{
		security_attributes = &( ( (libcthreads_internal_thread_attributes_t *) thread_attributes )->security_attributes );
	}
	internal_thread->thread_handle = CreateThread(
	                                  security_attributes,
	                                  0, /* stack size */
	                                  (PTHREAD_START_ROUTINE) &libcthreads_thread_start_function_helper,
	                                  (void *) start_function_helper_arguments,
	                                  0, /* creation flags */
	                                  &( internal_thread->thread_identifier ) );

	if( internal_thread->thread_handle == NULL )
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
	start_function_helper_arguments[ 2 ] = (void *) &( internal_thread->start_function_result );
	start_function_helper_arguments[ 3 ] = NULL;

	if( thread_attributes != NULL )
	{
		attributes = &( ( (libcthreads_internal_thread_attributes_t *) thread_attributes )->attributes );
	}
	result = pthread_create(
		  &( internal_thread->thread ),
	          attributes,
	          (void* (*)(void *)) &libcthreads_thread_start_function_helper,
	          start_function_helper_arguments );

	if( result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 result,
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

/* Exits the current thread
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

/* Joins the current with a specified thread
 * Returns 1 if successful or -1 on error
 */
int libcthreads_thread_join(
     libcthreads_thread_t *thread,
     libcerror_error_t **error )
{
	libcthreads_internal_thread_t *internal_thread = NULL;
	static char *function                          = "libcthreads_thread_join";

#if defined( WINAPI )
	DWORD error_code                               = 0;
	DWORD wait_status                              = 0;

#elif defined( HAVE_PTHREAD_H )
	int *thread_return_value                       = 0;
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
	internal_thread = (libcthreads_internal_thread_t *) thread;

#if defined( WINAPI )
	wait_status = WaitForSingleObject(
	               internal_thread->thread_handle,
	               INFINITE );

	if( wait_status == WAIT_FAILED )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: wait for thread failed.",
		 function );

		return( -1 );
	}
#elif defined( HAVE_PTHREAD_H )
	result = pthread_join(
	          internal_thread->thread,
	          (void **) &thread_return_value );

	if( result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to join thread.",
		 function );

		return( -1 );
	}
	if( ( thread_return_value == NULL )
	 || ( thread_return_value != &( internal_thread->start_function_result ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: invalid thread return value.",
		 function );

		return( -1 );
	}
	if( *thread_return_value != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: thread returned an error status.",
		 function,
		 *thread_return_value );

		return( -1 );
	}
#endif
	return( 1 );
}

