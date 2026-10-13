/* Copyright (c) 2026 Adam Mullins */
#include <limits.h>
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


/* -=[ FORWARD DECL ]=----------------------------------------------------- */
/* CREATE */
/* static */ 	void		 heapq_init(struct heapq *d, size_t elem_size);
struct heapq 	*heapq_new(size_t elem_size);

/* READ / QUERY */
/* static */ void		 heapq_grow(struct heapq *hq);
size_t			 heapq_len(struct heapq *hq);
int			 heapq_empty(struct heapq *hq);
/* static */ void		*heapq_get(struct heapq *hq, size_t i);
/* static */ void		*heapq_get_entry(struct heapq *hq, size_t i);
void		*heapq_peek(struct heapq *hq);
int		 heapq_in(struct heapq *hq, void *elem);
/* static */ unsigned int		 heapq_pri(struct heapq *hq, size_t i);
/* static */ int		 is_heap(struct heapq *hq);
/* static */ size_t		 lchild_i(size_t i);
/* static */ size_t		 rchild_i(size_t i);
/* static */ size_t		 parent_i(size_t i);

/* UPDATE */
void			 heapq_push(struct heapq *hq,
				    void *elem, unsigned int pri);
/* static */ int		 heapq_swap(struct heapq *hq, size_t i, size_t j);
/* static */ void		 perc_up(struct heapq *hq, size_t i);
/* static */ void		 sink_down(struct heapq *hq, size_t i);

/* DELETE */
void			 heapq_pop(struct heapq *hq);
void			 heapq_free(struct heapq *hq,
				    void (*destruct)(void *elem));


/* -=[ IMPLEMENTATION ]=--------------------------------------------------- */
/* static */
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


/* static */
void
heapq_grow(struct heapq *hq)
{
	hq->cap = hq->cap * 3 >> 1; // (3/2) * hq->cap
	/* FIXME If realloc fails, access to hq->buf pointer is lost. */
	hq->buf = realloc(hq->buf, hq->elem_size * hq->cap);
	return;
}


size_t
heapq_len(struct heapq *hq)
{
	return hq->len;
}


int
heapq_empty(struct heapq *hq)
{
	return hq->len == 0;
}


/* TODO refactor to get_data or something */
/* static */
void *
heapq_get(struct heapq *hq, size_t i)
{
	if (i < hq->len)
		return (hq->buf + i * hq->elem_size) + sizeof (unsigned int);
	else
		return NULL;
}


/*
 * Returns a pointer to the start of the whole {priority, data} entry.
 * This is actually the same address as what heapq_pri() returns, but this name
 * communicates specific intent.
 * TODO refactor to get_whole_record or something.
 */
/* static */
void *
heapq_get_entry(struct heapq *hq, size_t i)
{
	if (i < hq->len)
		return (hq->buf + i * hq->elem_size);
	else
		return NULL;
}


void *
heapq_peek(struct heapq *hq)
{
	if (hq->len == 0)
		return NULL;
	else
		return heapq_get(hq, 0);
}


int
heapq_in(struct heapq *hq, void *needle)
{
	void *hay;
	for (size_t i = 0; i < heapq_len(hq); i++) {
		void *hay = heapq_get(hq, i);
		int res;
		res = memcmp(hay,
			       needle,
			       hq->elem_size - sizeof (unsigned int));
		if (res == 0)
			return 1;
	}
	return 0;
}


/* TODO Refactor to heapq_get_pri */
/* static */
unsigned int
heapq_pri(struct heapq *hq, size_t i)
{
	if (i < hq->len)
		return *(unsigned int *)(hq->buf + i * hq->elem_size);
	return -1;
}


/* static */ int
is_heap(struct heapq *hq)
{
	int is_heap = 1;
	for(int i = 0; i < heapq_len(hq); i++) {
		unsigned int parent_pri;
		unsigned int lchild_pri;
		unsigned int rchild_pri;

		parent_pri = i < heapq_len(hq) ? heapq_pri(hq, i) : UINT_MAX;
		lchild_pri = lchild_i(i) < heapq_len(hq) ?
			heapq_pri(hq, lchild_i(i)) : UINT_MAX;
		rchild_pri = rchild_i(i) < heapq_len(hq) ?
			heapq_pri(hq, rchild_i(i)) : UINT_MAX;

		if (parent_pri > lchild_pri || parent_pri > rchild_pri) {
			is_heap = 0;
			break;
		}
	}
	return is_heap;
}


/* static */ size_t
lchild_i(size_t i)
{
	return 2 * i + 1;
}


/* static */ size_t
rchild_i(size_t i)
{
	return 2 * i + 2;
}

/* parent_i(0) returns nonsense because size_t underflows, so don't that. */
/* static */ size_t
parent_i(size_t i)
{
	return (i - 1) >> 1;
}


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
	/* Heapify. */
	perc_up(hq, hq->len - 1);
}


/* static */
int
heapq_swap(struct heapq *hq, size_t i, size_t j)
{
	void *tmp = malloc(hq->elem_size);
	if (tmp == NULL)
		return 1;

	/* tmp = i; */
	memcpy(tmp, heapq_get_entry(hq, i), hq->elem_size);
	/* i = j */
	memcpy(heapq_get_entry(hq, i), heapq_get_entry(hq, j), hq->elem_size);
	/* j = tmp */
	memcpy(heapq_get_entry(hq, j), tmp, hq->elem_size);

	free(tmp);
	return 0;
}


/* static */ void
perc_up(struct heapq *hq, size_t idx)
{
	/* if(i == 0) */
	/* 	return; */
	if (!(idx > 0 && idx < hq->len))
		return;

	size_t		parent_idx   = parent_i(idx);
	unsigned int	cur_priority = heapq_pri(hq, idx);
	unsigned int 	parent_pri   = heapq_pri(hq, parent_idx);

	if (cur_priority < parent_pri) {
		heapq_swap(hq, idx, parent_idx);
                perc_up(hq, parent_idx);
	}
}


/* static */ void
sink_down(struct heapq *hq, size_t idx)
{
	/* printf("Sinking down index %zu\n", idx); */
	/* if (!(idx >= 0 && idx < hq->len)) { */
	/* 	printf("Out of bounds.\n"); */
	/* 	return; */
	/* } */
	
	unsigned int	current_pri = heapq_pri(hq, idx);
        size_t		lchild_idx = lchild_i(idx);
        size_t		rchild_idx = rchild_i(idx);
	unsigned int	lchild_pri = lchild_i(idx) < heapq_len(hq) ?
		heapq_pri(hq, lchild_i(idx)) : UINT_MAX;
	unsigned int	rchild_pri = rchild_i(idx) < heapq_len(hq) ?
		heapq_pri(hq, rchild_i(idx)) : UINT_MAX;
        size_t		min_pri_child_idx = rchild_pri < lchild_pri ?
		rchild_idx : lchild_idx;

	/*printf("current id %zu pri %u data %c\n\t",
	       idx, current_pri, *(char *)heapq_get(hq, idx));
	if (lchild_pri != UINT_MAX)
		printf("lchild  id %zu pri %3u data %c\n\t",
		       lchild_idx, lchild_pri, *(char *)heapq_get(hq, lchild_idx));
	if (rchild_pri != UINT_MAX)
		printf("rchild  id %zu pri %3u data %c\n",
		rchild_idx, rchild_pri, *(char *)heapq_get(hq, rchild_idx));*/
	
        // done when both children are >= this priority, or
        // there are no children and we are a leaf.
        if (lchild_pri >= current_pri && rchild_pri >= current_pri)
            return;

        // otherwise, swap this node with the smallest of its children & repeat.
	heapq_swap(hq, idx, min_pri_child_idx);
        sink_down(hq, min_pri_child_idx);
}


void
heapq_pop(struct heapq *hq) {
	if (hq->len == 0)
		return;
	/* Move the last element to the first, then heapify down. */
	memcpy(hq->buf,
	       hq->buf + (hq->len - 1) * hq->elem_size,
	       hq->elem_size);
	hq->len--;
	sink_down(hq, 0);
}


/*
 * Completely free the queue.
 *
 * If function-pointer `destruct` is not null, it will be called once
 * with each element of the heap. This is useful for free()ing
 * pointers to dynamically allocated memory, for example, if the
 * heap holds (char *)s to malloc'd strings.
 *
 * *hq will need to be reinitialized if it's to be used again.
 */
void
heapq_free(struct heapq *hq, void (*destruct)(void *elem)) {
	if(destruct != NULL)
		for(size_t i = 0; i<hq->len; i++)
			(*destruct)((hq->buf + i * hq->elem_size) \
				    + sizeof (unsigned int));
	hq->cap = 0;
	hq->len = 0;
	hq->elem_size = 0;
	free(hq->buf);
	hq->buf = NULL;
}
