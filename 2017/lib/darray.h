// A simple dynamic array. Also known as a `vector` in the C++ world.
#pragma once
#include <stddef.h>


// NOTE letting outside code access buf directly can potentially
// cause. For instance, code might be tempted to save a pointer to a
// part of buf for quick reference or to implement a tree. But the buf
// might change underneath, eg, because of a sorting operation. Then
// the pointer becomes invalid, pointing to an unexpected member.
//
// Still, allowing direct access to buf can be expedient.
//
// Perhaps it can be changed by implementing getters/setters and
// providing wrappers for qsort/bsearch/etc.
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
