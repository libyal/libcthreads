/*
 * Queue functions
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

#if !defined( _LIBCTHREADS_QUEUE_H )
#define _LIBCTHREADS_QUEUE_H

#include <common.h>
#include <types.h>

#include "libcthreads_libcerror.h"
#include "libcthreads_queue_element.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcthreads_queue libcthreads_queue_t;

struct libcthreads_queue
{
	/* The number of elements
	 */
	int number_of_elements;

	/* The first (queue) element
	 */
	libcthreads_queue_element_t *first_element;

	/* The last (queue) element
	 */
	libcthreads_queue_element_t *last_element;
};

int libcthreads_queue_initialize(
     libcthreads_queue_t **queue,
     libcerror_error_t **error );

int libcthreads_queue_free(
     libcthreads_queue_t **queue,
     libcerror_error_t **error );

int libcthreads_queue_pop(
     libcthreads_queue_t *queue,
     intptr_t **value,
     libcerror_error_t **error );

int libcthreads_queue_push(
     libcthreads_queue_t *queue,
     intptr_t *value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

