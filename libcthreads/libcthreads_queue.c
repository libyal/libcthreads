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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libcthreads_libcerror.h"
#include "libcthreads_queue.h"
#include "libcthreads_queue_element.h"
#include "libcthreads_types.h"

/* Creates a queue
 * 'Make sure the value queue is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_initialize(
     libcthreads_queue_t **queue,
     libcerror_error_t **error )
{
	static char *function = "libcthreads_queue_initialize";

	if( queue == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue.",
		 function );

		return( -1 );
	}
	if( *queue != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid queue value already set.",
		 function );

		return( -1 );
	}
	*queue = memory_allocate_structure(
	         libcthreads_queue_t );

	if( *queue == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create queue.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *queue,
	     0,
	     sizeof( libcthreads_queue_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear queue.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *queue != NULL )
	{
		memory_free(
		 *queue );

		*queue = NULL;
	}
	return( -1 );
}

/* Frees a queue
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_free(
     libcthreads_queue_t **queue,
     libcerror_error_t **error )
{
	static char *function = "libcthreads_queue_free";

	if( queue == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue.",
		 function );

		return( -1 );
	}
	if( *queue != NULL )
	{
		memory_free(
		 *queue );

		*queue = NULL;
	}
	return( 1 );
}

