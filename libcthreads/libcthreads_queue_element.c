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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libcthreads_libcerror.h"
#include "libcthreads_queue_element.h"
#include "libcthreads_types.h"

/* Creates a queue element
 * 'Make sure the value element is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_element_initialize(
     libcthreads_queue_element_t **element,
     libcerror_error_t **error )
{
	static char *function = "libcthreads_queue_element_initialize";

	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue element.",
		 function );

		return( -1 );
	}
	if( *element != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid element value already set.",
		 function );

		return( -1 );
	}
	*element = memory_allocate_structure(
	            libcthreads_queue_element_t );

	if( *element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create queue element.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *element,
	     0,
	     sizeof( libcthreads_queue_element_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear queue element.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *element != NULL )
	{
		memory_free(
		 *element );

		*element = NULL;
	}
	return( -1 );
}

/* Frees a queue element
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_element_free(
     libcthreads_queue_element_t **element,
     libcerror_error_t **error )
{
	static char *function = "libcthreads_queue_element_free";
	int result            = 1;

	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue element.",
		 function );

		return( -1 );
	}
	if( *element != NULL )
	{
		if( ( ( *element )->previous_element != NULL )
		 || ( ( *element )->next_element != NULL ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: queue element part of a queue.",
			 function );

			return( -1 );
		}
		/* The value is freed elsewhere
		 */
		memory_free(
		 *element );

		*element = NULL;
	}
	return( result );
}

/* Retrieves the value from the queue element
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_element_get_value(
     libcthreads_queue_element_t *element,
     intptr_t **value,
     libcerror_error_t **error )
{
	static char *function = "libcthreads_queue_element_get_value";

	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue element.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	*value = element->value;

	return( 1 );
}

/* Sets the value in the queue element
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_element_set_value(
     libcthreads_queue_element_t *element,
     intptr_t *value,
     libcerror_error_t **error )
{
	static char *function = "libcthreads_queue_element_set_value";

	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue element.",
		 function );

		return( -1 );
	}
	element->value = value;

	return( 1 );
}

/* Retrieves the previous element from the queue element
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_element_get_previous_element(
     libcthreads_queue_element_t *element,
     libcthreads_queue_element_t **previous_element,
     libcerror_error_t **error )
{
	static char *function = "libcthreads_queue_element_get_previous_element";

	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue element.",
		 function );

		return( -1 );
	}
	if( previous_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid previous element.",
		 function );

		return( -1 );
	}
	*previous_element = element->previous_element;

	return( 1 );
}

/* Sets the previous element in the queue element
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_element_set_previous_element(
     libcthreads_queue_element_t *element,
     libcthreads_queue_element_t *previous_element,
     libcerror_error_t **error )
{
	static char *function = "libcthreads_queue_element_set_previous_element";

	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue element.",
		 function );

		return( -1 );
	}
	element->previous_element = previous_element;

	return( 1 );
}

/* Retrieves the next element from the queue element
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_element_get_next_element(
     libcthreads_queue_element_t *element,
     libcthreads_queue_element_t **next_element,
     libcerror_error_t **error )
{
	static char *function = "libcthreads_queue_element_get_next_element";

	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue element.",
		 function );

		return( -1 );
	}
	if( next_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid next element.",
		 function );

		return( -1 );
	}
	*next_element = element->next_element;

	return( 1 );
}

/* Sets the next element in the queue element
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_element_set_next_element(
     libcthreads_queue_element_t *element,
     libcthreads_queue_element_t *next_element,
     libcerror_error_t **error )
{
	static char *function = "libcthreads_queue_element_set_next_element";

	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue element.",
		 function );

		return( -1 );
	}
	element->next_element = next_element;

	return( 1 );
}

/* Retrieves the previous and next element from the queue element
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_element_get_elements(
     libcthreads_queue_element_t *element,
     libcthreads_queue_element_t **previous_element,
     libcthreads_queue_element_t **next_element,
     libcerror_error_t **error )
{
	static char *function = "libcthreads_queue_element_get_elements";

	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue element.",
		 function );

		return( -1 );
	}
	if( previous_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid previous element.",
		 function );

		return( -1 );
	}
	if( next_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid next element.",
		 function );

		return( -1 );
	}
	*previous_element = element->previous_element;
	*next_element     = element->next_element;

	return( 1 );
}

/* Sets the previous and next element in the queue element
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_element_set_elements(
     libcthreads_queue_element_t *element,
     libcthreads_queue_element_t *previous_element,
     libcthreads_queue_element_t *next_element,
     libcerror_error_t **error )
{
	static char *function = "libcthreads_queue_element_set_elements";

	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue element.",
		 function );

		return( -1 );
	}
	element->previous_element = previous_element;
	element->next_element     = next_element;

	return( 1 );
}

