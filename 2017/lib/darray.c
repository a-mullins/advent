#include <stdlib.h>
#include <string.h>
#include "darray.h"


#define MIN_CAP 16


struct darray {
    size_t cap;
    size_t len;
    size_t elem_size;
    void *buf;
};


void
darray_init(struct darray *d, size_t elem_size)
{
    d->cap = MIN_CAP;
    d->len = 0;
    d->elem_size = elem_size;
    d->buf = calloc(MIN_CAP, elem_size);
}


void *
darray_get(struct darray *d, size_t n)
{
    return d->buf + d->elem_size * n;
}

void *
darray_last(struct darray *d)
{
    // unsafe if d->len == 0;
    return d->buf + d->elem_size * (d->len - 1);
}

void
darray_push(struct darray *d, void *elem)
{
    if(d->len+1 >= d->cap) {
        d->cap <<= 1;
        // FIXME if realloc fails, we loose access to d->buf pointer
        d->buf = realloc(d->buf, d->elem_size * d->cap);
    }
    memcpy(d->buf + d->len * d->elem_size, elem, d->elem_size);
    d->len++;
}


// Completely free the array.
//
// If function-pointer `destruct` is not null, it will be called once
// with each element of the array. This is useful for free()ing
// pointers to dynamically allocated memory, for example, if the
// darray holds (char *)s to strings.
//
// If you wish to use *d again, it will need to be reinitialized.
void
darray_free(struct darray *d, void (*destruct)(void *elem)) {
    if(destruct != NULL) {
        for(size_t i = 0; i<d->len; i++) {
            (*destruct)(darray_get(d, i));
        }
    }
    d->cap = 0;
    d->len = 0;
    d->elem_size = 0;
    free(d->buf);
    d->buf = NULL;
}


void
darray_qsort(struct darray *d, int (*compar)(const void *a, const void *b))
{
    qsort(d->buf, d->len, d->elem_size, compar);
}


ssize_t
darray_in(struct darray *d, void *elem)
{
    for (size_t i = 0; i<d->len; i++) {
        if (memcmp(darray_get(d, i), elem, d->elem_size) == 0) {
            return i;
        }
    }
    return -1;
}


void *
darray_bsearch(struct darray *d,
               const void *key,
               int (*compar)(const void *a, const void *b))
{
    return bsearch(key,
                   d->buf,
                   d->len,
                   d->elem_size,
                   compar);
}
