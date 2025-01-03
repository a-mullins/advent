// A simple dynamic array. Also known as a `vector` in the C++ world.
#pragma once
#include <stddef.h>


typedef struct darray {
    size_t cap;
    size_t len;
    size_t elem_size;
    void *buf;
} darray;


// CREATE
void
darray_init(darray *d, size_t elem_size);


// READ / QUERY
void *
darray_get(darray *d, size_t n);

void *
darray_last(darray *d);

ssize_t
darray_in(darray *d, void *elem);

void *
darray_bsearch(darray *d,
               const void *key,
               int (*compar)(const void *a, const void *b));


// UPDATE
void
darray_push(darray *d, void *elem);

void
darray_qsort(darray *d, int (*compar)(const void *a, const void *b));


// DELETE
void
darray_free(darray *d, void (*destruct)(void *elem));
