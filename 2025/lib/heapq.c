// Copyright (c) 2025 Adam Mullins
// TODO check *allocs for errors.
#include <stdlib.h>
#include <string.h>
#include "heapq.h"


#define MIN_CAP 2


struct heapq {
    size_t cap;
    size_t len;
    size_t elem_size;
    void *buf;
};


static
void
heapq_init(struct heapq *hq, size_t elem_size)
{
    hq->cap = MIN_CAP;
    hq->len = 0;
    hq->elem_size = sizeof (unsigned int) + elem_size;
    hq->buf = malloc(MIN_CAP * elem_size);
}

struct heapq *
heapq_new(size_t elem_size)
{
    struct heapq *p = malloc(sizeof (struct heapq));
    heapq_init(p, elem_size);
    return p;
}

/* struct heapq * */
/* heapq_copy(struct heapq *other) { */
/*     struct heapq *hq = malloc(sizeof (struct heapq)); */
/*     a->cap = other->cap; */
/*     a->len = other->len; */
/*     a->elem_size = other->elem_size; */
/*     a->buf = calloc(a->cap, a->elem_size); */
/*     memcpy(a->buf, other->buf, a->len * a->elem_size); */
/*     return a; */
/* } */

static void
heapq_grow(struct heapq *hq)
{
    hq->cap = hq->cap * 3 >> 1; // (3/2) * hq->cap
    // FIXME if realloc fails, we loose access to hq->buf pointer
    hq->buf = realloc(hq->buf, hq->elem_size * hq->cap);
    return;
}

size_t
heapq_len(struct heapq *hq)
{
    return hq->len;
}

struct heapq_entry
heapq_get(struct heapq *hq, size_t i)
{
    struct heapq_entry hqe;
    hqe.pri = *(unsigned int *)(hq->buf + i * hq->elem_size);
    hqe.data = (hq->buf + i * hq->elem_size) + sizeof (unsigned int);
    return hqe;	    
}

/* void * */
/* heapq_last(struct heapq *hq) */
/* { */
/*     if (a->len == 0) */
/*         return NULL; */
/*     else */
/*         return a->buf + a->elem_size * (a->len - 1); */
/* } */


void
heapq_push(struct heapq *hq, void *elem, unsigned int pri)
{
    if (hq->len+1 >= hq->cap)
        heapq_grow(hq);
    /* Copy priority. */
    memcpy(hq->buf + hq->len * hq->elem_size, &pri, sizeof (unsigned int));
    /* Copy data. */
    memcpy((hq->buf + hq->len * hq->elem_size) + sizeof (unsigned int),
	   elem,
	   hq->elem_size);
    hq->len++;
}


/* void */
/* heapq_replace(struct heapq *hq, size_t i, void *new_elem) */
/* { */
/*     memcpy(a->buf + i * a->elem_size, new_elem, a->elem_size); */
/*     return; */
/* } */


/* void */
/* heapq_swap(struct heapq *hq, size_t i, size_t j) */
/* { */
/*     void *tmp = malloc(a->elem_size); */
    
/*     /\* tmp = i; *\/ */
/*     memcpy(tmp, heapq_get(a, i), a->elem_size); */
/*     /\* i = j *\/ */
/*     memcpy(heapq_get(a, i), heapq_get(a, j), a->elem_size); */
/*     /\* j = tmp *\/ */
/*     memcpy(heapq_get(a, j), tmp, a->elem_size); */
    
/*     free(tmp); */
/* } */


/* void */
/* heapq_del_id(struct heapq *hq, size_t i) */
/* { */
/*     if (i >= a->len ) */
/*         return; */
/*     if (i == a->len - 1) */
/*         memset(a->buf + i * a->elem_size, 0, a->elem_size); */
/*     else { */
/*         memmove(a->buf + i       * a->elem_size, */
/*                 a->buf + (i+1)   * a->elem_size, */
/*                 (a->len - (i+1)) * a->elem_size); */
/*     } */
/*     a->len--; */
/*     return; */
/* } */


/* void */
/* heapq_del_val(struct heapq *hq, void *elem) */
/* { */
/*     heapq_del_id(a, heapq_in(a, elem)); */
/*     return; */
/* } */


// Completely free the array.
//
// If function-pointer `destruct` is not null, it will be called once
// with each element of the array. This is useful for free()ing
// pointers to dynamically allocated memory, for example, if the
// heapq holds (char *)s to malloc'd strings.
//
// If you wish to use *hq again, it will need to be reinitialized.
void
heapq_free(struct heapq *hq, void (*destruct)(void *elem)) {
    if(destruct != NULL)
        for(size_t i = 0; i<hq->len; i++)
            (*destruct)((hq->buf + i * hq->elem_size) + sizeof (unsigned int));
    hq->cap = 0;
    hq->len = 0;
    hq->elem_size = 0;
    free(hq->buf);
    hq->buf = NULL;
}


/* ssize_t */
/* heapq_in(struct heapq *hq, void *elem) */
/* { */
/*     for (size_t i = 0; i<a->len; i++) */
/*         if (memcmp(heapq_get(a, i), elem, a->elem_size) == 0) */
/*             return i; */
/*     return -1; */
/* } */


/* void */
/* heapq_qsort(struct heapq *hq, int (*compar)(const void *a, const void *b)) */
/* { */
/*     qsort(a->buf, a->len, a->elem_size, compar); */
/* } */


/* ssize_t */
/* heapq_bsearch(struct heapq *hq, */
/*                const void *key, */
/*                int (*compar)(const void *a, const void *b)) */
/* { */
/*     void *key_p = bsearch(key, */
/*                    a->buf, */
/*                    a->len, */
/*                    a->elem_size, */
/*                    compar); */
/*     if (key_p) */
/*         return (ssize_t)((key_p - a->buf) / a->elem_size); */
/*     else */
/*         return -1; */
/* } */
