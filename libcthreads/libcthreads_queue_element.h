/*
 * Queue element functions
 *
 * Copyright (c) 2012-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBCTHREADS_QUEUE_ELEMENT_H )
#define _LIBCTHREADS_QUEUE_ELEMENT_H

#include <common.h>
#include <types.h>

#include "libcthreads_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcthreads_queue_element libcthreads_queue_element_t;

struct libcthreads_queue_element
{
	/* The previous queue element
	 */
	libcthreads_queue_element_t *previous_element;

	/* The next queue element
	 */
	libcthreads_queue_element_t *next_element;

	/* The value
	 */
	intptr_t *value;
};

int libcthreads_queue_element_initialize(
     libcthreads_queue_element_t **element,
     libcerror_error_t **error );

int libcthreads_queue_element_free(
     libcthreads_queue_element_t **element,
     libcerror_error_t **error );

int libcthreads_queue_element_get_value(
     libcthreads_queue_element_t *element,
     intptr_t **value,
     libcerror_error_t **error );

int libcthreads_queue_element_set_value(
     libcthreads_queue_element_t *element,
     intptr_t *value,
     libcerror_error_t **error );

int libcthreads_queue_element_get_previous_element(
     libcthreads_queue_element_t *element,
     libcthreads_queue_element_t **previous_element,
     libcerror_error_t **error );

int libcthreads_queue_element_set_previous_element(
     libcthreads_queue_element_t *element,
     libcthreads_queue_element_t *previous_element,
     libcerror_error_t **error );

int libcthreads_queue_element_get_next_element(
     libcthreads_queue_element_t *element,
     libcthreads_queue_element_t **next_element,
     libcerror_error_t **error );

int libcthreads_queue_element_set_next_element(
     libcthreads_queue_element_t *element,
     libcthreads_queue_element_t *next_element,
     libcerror_error_t **error );

int libcthreads_queue_element_get_elements(
     libcthreads_queue_element_t *element,
     libcthreads_queue_element_t **previous_element,
     libcthreads_queue_element_t **next_element,
     libcerror_error_t **error );

int libcthreads_queue_element_set_elements(
     libcthreads_queue_element_t *element,
     libcthreads_queue_element_t *previous_element,
     libcthreads_queue_element_t *next_element,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

