/*
 * Thread pool functions
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

#if defined( WINAPI ) && ( WINVER >= 0x0602 )
#include <Threadpoolapiset.h>
#endif

#include "libcthreads_libcerror.h"
#include "libcthreads_thread.h"
#include "libcthreads_thread_attributes.h"
#include "libcthreads_thread_pool.h"
#include "libcthreads_types.h"

/* Creates a thread pool
 * Make sure the value thread_pool is referencing, is set to NULL
 *
 * The start_function should return 1 if successful and -1 on error
 * Returns 1 if successful or -1 on error
 */
int libcthreads_thread_pool_create(
     libcthreads_thread_pool_t **thread_pool,
     const libcthreads_thread_attributes_t *thread_attributes,
     int maximum_number_of_threads,
     int (*start_function)(
            void *arguments ),
     void *start_function_arguments,
     libcerror_error_t **error )
{
	libcthreads_internal_thread_pool_t *internal_thread_pool = NULL;
	static char *function                                    = "libcthreads_thread_pool_create";

#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	DWORD error_code                                         = 0;
#else
	size_t threads_array_size                                = 0;
#endif

	if( thread_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid thread pool.",
		 function );

		return( -1 );
	}
	if( *thread_pool != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid thread pool value already set.",
		 function );

		return( -1 );
	}
	if( maximum_number_of_threads <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid maximum number of threads value zero or less.",
		 function );

		return( -1 );
	}
#if defined( WINAPI ) && ( WINVER >= 0x0600 )
#if SIZEOF_INT > 4
	if( maximum_number_of_threads > (int) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid maximum number of threads value exceeds maximum.",
		 function );

		return( -1 );
	}
#endif
#else
#if SIZEOF_INT <= SIZEOF_SIZE_T
	if( (size_t) maximum_number_of_threads > (size_t) ( SSIZE_MAX / sizeof( libcthreads_thread_t * ) ) )
#else
	if( maximum_number_of_threads > (int) ( SSIZE_MAX / sizeof( libcthreads_thread_t * ) ) )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid maximum number of threads value exceeds maximum.",
		 function );

		return( -1 );
	}
#endif
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
	internal_thread_pool = memory_allocate_structure(
	                        libcthreads_internal_thread_pool_t );

	if( internal_thread_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create thread pool.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_thread_pool,
	     0,
	     sizeof( libcthreads_internal_thread_pool_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear thread pool.",
		 function );

		goto on_error;
	}
#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	internal_thread_pool->thread_pool = CreateThreadpool(
	                                     NULL );

	if( internal_thread_pool->thread_pool == NULL )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create thread pool.",
		 function );

		goto on_error;
	}
	SetThreadpoolThreadMaximum(
	 internal_thread_pool->thread_pool,
	 (DWORD) maximum_number_of_threads );
#else
	threads_array_size = sizeof( libcthreads_thread_t * ) * maximum_number_of_threads;

	if( threads_array_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid threads array size value out of bounds.",
		 function );

		goto on_error;
	}
	internal_thread_pool->threads_array = (libcthreads_thread_t **) memory_allocate(
	                                                                 threads_array_size );

	if( internal_thread_pool->threads_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create threads array.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_thread_pool->threads_array,
	     0,
	     threads_array_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear threads array.",
		 function );

		goto on_error;
	}
	internal_thread_pool->start_function           = start_function;
	internal_thread_pool->start_function_arguments = start_function_arguments;
#endif
	*thread_pool = (libcthreads_thread_pool_t *) internal_thread_pool;

	return( 1 );

on_error:
	if( internal_thread_pool != NULL )
	{
#if !( defined( WINAPI ) && ( WINVER >= 0x0600 ) )
		if( internal_thread_pool->threads_array != NULL )
		{
			memory_free(
			 internal_thread_pool->threads_array );
		}
#endif
		memory_free(
		 internal_thread_pool );
	}
	return( -1 );
}

