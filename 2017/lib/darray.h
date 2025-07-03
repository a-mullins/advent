// Copyright (c) 2025 Adam Mullins
// A simple dynamic array. Also known as a `vector` in the C++ world.
#pragma once
#include <stddef.h>
#include <sys/types.h>


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
struct darray {
    size_t cap;
    size_t len;
    size_t elem_size;
    void *buf;
};


// CREATE
void
darray_init(struct darray *d, size_t elem_size);


// READ / QUERY
void *
darray_get(struct darray *d, size_t i);

void *
darray_last(struct darray *d);

ssize_t
darray_in(struct darray *d, void *elem);

void *
darray_bsearch(struct darray *d,
               const void *key,
               int (*compar)(const void *a, const void *b));


// UPDATE
void
darray_push(struct darray *d, void *elem);

void
darray_replace(struct darray *d, size_t i, void *new_elem);

void
darray_qsort(struct darray *d, int (*compar)(const void *a, const void *b));


// DELETE
void
darray_del(struct darray *d, size_t i);

void
darray_free(struct darray *d, void (*destruct)(void *elem));
