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

#if !defined( _LIBCTHREADS_MUTEX_H )
#define _LIBCTHREADS_MUTEX_H

#include <common.h>
#include <types.h>

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
#include <pthread.h>
#endif

#include "libcthreads_extern.h"
#include "libcthreads_libcerror.h"
#include "libcthreads_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcthreads_internal_mutex libcthreads_internal_mutex_t;

struct libcthreads_internal_mutex
{
#if defined( WINAPI )
	/* The mutex handle
	 */
	HANDLE mutex;

#elif defined( HAVE_PTHREAD_H )
	/* The mutex
	 */
	pthread_mutex_t mutex;

#else
#error Missing mutex type
#endif
};

LIBCTHREADS_EXTERN \
int libcthreads_mutex_initialize(
     libcthreads_mutex_t **mutex,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_mutex_free(
     libcthreads_mutex_t **mutex,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_mutex_grab(
     const libcthreads_mutex_t *mutex,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_mutex_release(
     const libcthreads_mutex_t *mutex,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

