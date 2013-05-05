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

#if !defined( _LIBCTHREADS_THREAD_POOL_H )
#define _LIBCTHREADS_THREAD_POOL_H

#include <common.h>
#include <types.h>

#if defined( WINAPI ) && ( WINVER >= 0x0602 )
#include <Threadpoolapiset.h>
#endif

#include "libcthreads_extern.h"
#include "libcthreads_libcerror.h"
#include "libcthreads_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcthreads_internal_thread_pool libcthreads_internal_thread_pool_t;

struct libcthreads_internal_thread_pool
{
#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	/* The thread pool
	 */
	TP_POOL *thread_pool;

#else
	/* The maximum number of threads in the pool
	 */
	int maximum_number_of_threads;

	/* The threads array
	 */
	libcthreads_thread_t **threads_array;

	/* The start function
	 */
	int (*start_function)(
	       void *arguments );

	/* The start function arguments
	 */
	void *start_function_arguments;
#endif
};

LIBCTHREADS_EXTERN \
int libcthreads_thread_pool_create(
     libcthreads_thread_pool_t **thread_pool,
     const libcthreads_thread_attributes_t *thread_attributes,
     int maximum_number_of_threads,
     int (*start_function)(
            void *arguments ),
     void *start_function_arguments,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

