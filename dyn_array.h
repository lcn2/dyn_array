/*
 * dyn_array - dynamic array facility
 *
 * Copyright (c) 2014,2015,2022-2024 by Landon Curt Noll.  All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright, this permission notice and text
 * this comment, and the disclaimer below appear in all of the following:
 *
 *       supporting documentation
 *       source copies
 *       source works derived from this source
 *       binaries derived from this source or from derived source
 *
 * LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL LANDON CURT NOLL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * chongo (Landon Curt Noll, http://www.isthe.com/chongo/index.html) /\oo/\
 *
 * Share and enjoy! :-)
 */


#if !defined(INCLUDE_DYN_ARRAY_H)
#    define  INCLUDE_DYN_ARRAY_H


#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


/*
 * size_t MAX and MIN
 */
#if !defined(SIZE_MAX)
#define SIZE_MAX (~((size_t)0))
#endif /* SIZE_MAX */
#if !defined(SIZE_MIN)
#define SIZE_MIN ((size_t)(0))
#endif /* SIZE_MIN */


/*
 * dbg - info, debug, warning, error, and usage message facility
 */
#if defined(INTERNAL_INCLUDE)
#include "../dbg/dbg.h"
#else
#include <dbg.h>
#endif


/*
 * official version
 */
#define DYN_ARRAY_VERSION "2.3.1 2024-08-28"	/* format: major.minor YYYY-MM-DD */


/*
 * dynamic array convenience macros
 *
 * Obtain an element in a dynamic array:
 *
 *	struct dyn_array *array;
 *	double value;
 *
 *      value = dyn_array_value(array, double, i);
 *
 * Obtain the address of an element in a dynamic array:
 *
 *	struct dyn_array *array;
 *	struct json *addr;
 *
 *      addr = dyn_array_addr(array, struct json, i);
 *
 * Current element count of dynamic array:
 *
 *	struct dyn_array *array;
 *	intmax_t pos;
 *
 *	pos = dyn_array_tell(array);
 *
 * Address of the element just beyond the elements in use:
 *
 *	struct dyn_array *array;
 *	struct json_member *next;
 *
 *	next = dyn_array_beyond(array, struct json_member);
 *
 * Number of elements allocated in memory for the dynamic array:
 *
 *	struct dyn_array *array;
 *	intmax_t size;
 *
 *	size = dyn_array_alloced(array);
 *
 * Number of elements available (allocated but not in use) for the dynamic array:
 *
 *	struct dyn_array *array;
 *	intmax_t avail;
 *
 *	avail = dyn_array_avail(array);
 *
 * Rewind a dynamic array back to zero elements:
 *
 *	struct dyn_array *array;
 *
 *	dyn_array_rewind(array);
 */
#define dyn_array_value(array, type, index) (((type *)(((struct dyn_array *)(array))->data))[(index)])
#define dyn_array_addr(array, type, index) (((type *)(((struct dyn_array *)(array))->data))+(index))
#define dyn_array_tell(array) (((struct dyn_array *)(array))->count)
#define dyn_array_beyond(array, type) (dyn_array_addr(array, type, dyn_array_tell(array)))
#define dyn_array_alloced(array) (((struct dyn_array *)(array))->allocated)
#define dyn_array_avail(array) (dyn_array_alloced(array) - dyn_array_tell(array))
#define dyn_array_rewind(array) (dyn_array_seek((struct dyn_array *)(array), 0, SEEK_SET))


/*
 * dyn_array - a dynamic array of elements of the same type
 *
 * The dynamic array maintains both an allocated element count
 * and a number of elements in use.
 *
 * The actual storage allocated will be a number of elements
 * beyond the allocated element count, to serve as a guard chunk.
 *
 * If zeroize is true, then all allocated elements will be
 * zeroized when first allocated, and zeroized when dyn_array_free()
 * is called.
 */
struct dyn_array
{
    size_t elm_size;		/* Number of bytes for a single element */
    bool zeroize;		/* true ==> always zero newly allocated chunks, false ==> don't */
    intmax_t count;		/* Number of elements in use */
    intmax_t allocated;		/* Number of elements allocated (>= count) */
    intmax_t chunk;		/* Number of elements to expand by when allocating */
    void *data;			/* allocated dynamic array of identical things or NULL */
};


/*
 * global variables
 */
extern const char *const dyn_array_version;	/* library version format: major.minor YYYY-MM-DD */


/*
 * external allocation functions
 */
extern struct dyn_array *dyn_array_create(size_t elm_size, intmax_t chunk, intmax_t start_elm_count, bool zeroize);
extern bool dyn_array_append_value(struct dyn_array *array, void *value_to_add);
extern bool dyn_array_append_set(struct dyn_array *array, void *array_to_add_p, intmax_t count_of_elements_to_add);
extern bool dyn_array_concat_array(struct dyn_array *array, struct dyn_array *other);
extern bool dyn_array_seek(struct dyn_array *array, off_t offset, int whence);
extern void dyn_array_clear(struct dyn_array *array);
extern void dyn_array_free(struct dyn_array *array);


#endif		/* INCLUDE_DYN_ARRAY_H */
