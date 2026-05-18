// Copyright (c) 2025 Adam Mullins
// A simple dynamic array. Also known as a `vector` in the C++ world.
#pragma once
#include <stddef.h>
#include <sys/types.h>


struct alist;


// CREATE
// void
// alist_init(struct alist *d, size_t elem_size);

struct alist *
alist_new(size_t elem_size);

struct alist *
alist_copy(struct alist *other);


// READ / QUERY
size_t
alist_len(struct alist *a);

void *
alist_get(struct alist *a, size_t i);

void *
alist_last(struct alist *a);

ssize_t
alist_in(struct alist *a, void *elem);

ssize_t
alist_bsearch(struct alist *a,
               const void *key,
               int (*compar)(const void *a, const void *b));


// UPDATE
void
alist_push(struct alist *a, void *elem);

void
alist_replace(struct alist *a, size_t i, void *new_elem);

void
alist_qsort(struct alist *a, int (*compar)(const void *a, const void *b));


// DELETE
void
alist_del_id(struct alist *a, size_t i);

void
alist_del_val(struct alist *a, void *elem);

void
alist_free(struct alist *a, void (*destruct)(void *elem));
