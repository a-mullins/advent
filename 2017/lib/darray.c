// Copyright (c) 2025 Adam Mullins
#include <stdlib.h>
#include <string.h>
#include "darray.h"


#define MIN_CAP 2


struct darray {
    size_t cap;
    size_t len;
    size_t elem_size;
    void *buf;
};


static
void
darray_init(struct darray *d, size_t elem_size)
{
    d->cap = MIN_CAP;
    d->len = 0;
    d->elem_size = elem_size;
    d->buf = malloc(MIN_CAP * elem_size);
}

struct darray *
darray_new(size_t elem_size)
{
    struct darray *p = malloc(sizeof (struct darray));
    darray_init(p, elem_size);
    return p;
}

struct darray *
darray_copy(struct darray *other) {
    struct darray *d = malloc(sizeof (struct darray));
    d->cap = other->cap;
    d->len = other->len;
    d->elem_size = other->elem_size;
    d->buf = calloc(d->cap, d->elem_size);
    memcpy(d->buf, other->buf, d->len * d->elem_size);
    return d;
}

static void
darray_grow(struct darray *d)
{
    d->cap = d->cap * 3 >> 1; // (3/2) * d->cap
    // FIXME if realloc fails, we loose access to d->buf pointer
    d->buf = realloc(d->buf, d->elem_size * d->cap);
    return;
}

size_t
darray_len(struct darray *d)
{
    return d->len;
}

void *
darray_get(struct darray *d, size_t i)
{
    return d->buf + i * d->elem_size;
}


void *
darray_last(struct darray *d)
{
    if (d->len == 0)
        return NULL;
    else
        return d->buf + d->elem_size * (d->len - 1);
}


void
darray_push(struct darray *d, void *elem)
{
    if (d->len+1 >= d->cap)
        darray_grow(d);
    memcpy(d->buf + d->len * d->elem_size, elem, d->elem_size);
    d->len++;
}


void
darray_replace(struct darray *d, size_t i, void *new_elem)
{
    memcpy(d->buf + i * d->elem_size, new_elem, d->elem_size);
    return;
}


void
darray_del_id(struct darray *d, size_t i)
{
    if (i >= d->len )
        return;
    if (i == d->len - 1)
        memset(d->buf + i * d->elem_size, 0, d->elem_size);
    else {
        memmove(d->buf + i       * d->elem_size,
                d->buf + (i+1)   * d->elem_size,
                (d->len - (i+1)) * d->elem_size);
    }
    d->len--;
    return;
}


void
darray_del_val(struct darray *d, void *elem)
{
    darray_del_id(d, darray_in(d, elem));
    return;
}


// Completely free the array.
//
// If function-pointer `destruct` is not null, it will be called once
// with each element of the array. This is useful for free()ing
// pointers to dynamically allocated memory, for example, if the
// darray holds (char *)s to malloc'd strings.
//
// If you wish to use *d again, it will need to be reinitialized.
void
darray_free(struct darray *d, void (*destruct)(void *elem)) {
    if(destruct != NULL)
        for(size_t i = 0; i<d->len; i++)
            (*destruct)(d->buf + i * d->elem_size);
    d->cap = 0;
    d->len = 0;
    d->elem_size = 0;
    free(d->buf);
    d->buf = NULL;
}


ssize_t
darray_in(struct darray *d, void *elem)
{
    for (size_t i = 0; i<d->len; i++)
        if (memcmp(darray_get(d, i), elem, d->elem_size) == 0)
            return i;
    return -1;
}


void
darray_qsort(struct darray *d, int (*compar)(const void *a, const void *b))
{
    qsort(d->buf, d->len, d->elem_size, compar);
}


ssize_t
darray_bsearch(struct darray *d,
               const void *key,
               int (*compar)(const void *a, const void *b))
{
    void *key_p = bsearch(key,
                   d->buf,
                   d->len,
                   d->elem_size,
                   compar);
    if (key_p)
        return (ssize_t)((key_p - d->buf) / d->elem_size);
    else
        return -1;
}
