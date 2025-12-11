// Copyright (c) 2025 Adam Mullins
// A simple dynamic array. Also known as a `vector` in the C++ world.
#pragma once
#include <stddef.h>
#include <sys/types.h>


struct darray;


// CREATE
// void
// darray_init(struct darray *d, size_t elem_size);

struct darray *
darray_new(size_t elem_size);

struct darray *
darray_copy(struct darray *other);


// READ / QUERY
size_t
darray_len(struct darray *d);

void *
darray_get(struct darray *d, size_t i);

void *
darray_last(struct darray *d);

ssize_t
darray_in(struct darray *d, void *elem);

ssize_t
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
darray_del_id(struct darray *d, size_t i);

void
darray_del_val(struct darray *d, void *elem);

void
darray_free(struct darray *d, void (*destruct)(void *elem));
