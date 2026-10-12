// Copyright (c) 2026 Adam Mullins
// Simple min-heap based priority queue.
#pragma once
#include <stddef.h>
#include <sys/types.h>


struct heapq_entry {
    unsigned int pri;
    void *data;
};
struct heapq;


// CREATE
// void
// heapq_init(struct heapq *d, size_t elem_size);

struct heapq *
heapq_new(size_t elem_size);

struct heapq *
heapq_copy(struct heapq *other);


// READ / QUERY
size_t
heapq_len(struct heapq *hq);

struct heapq_entry
heapq_get(struct heapq *hq, size_t i);

void *
heapq_last(struct heapq *hq);

ssize_t
heapq_in(struct heapq *hq, void *elem);

ssize_t
heapq_bsearch(struct heapq *hq,
               const void *key,
               int (*compar)(const void *hq, const void *b));


// UPDATE
void
heapq_push(struct heapq *hq, void *elem, unsigned int pri);

void
heapq_replace(struct heapq *hq, size_t i, void *new_elem);

void
heapq_swap(struct heapq *hq, size_t i, size_t j);

void
heapq_qsort(struct heapq *hq, int (*compar)(const void *a, const void *b));


// DELETE
void
heapq_del_id(struct heapq *hq, size_t i);

void
heapq_del_val(struct heapq *hq, void *elem);

void
heapq_free(struct heapq *hq, void (*destruct)(void *elem));
