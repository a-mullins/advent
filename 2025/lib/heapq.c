/* Copyright (c) 2026 Adam Mullins */
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
static 	void		 heapq_init(struct heapq *d, size_t elem_size);
const struct heapq 	*heapq_new(size_t elem_size);

/* READ / QUERY */
static void		 heapq_grow(struct heapq *hq);
size_t			 heapq_len(struct heapq *hq);
int			 heapq_empty(struct heapq *hq);
static const void	*heapq_get(struct heapq *hq, size_t i);
const void		*heapq_peek(struct heapq *hq);
static unsigned int	 heapq_pri(struct heapq *hq, size_t i);

/* UPDATE */
void			 heapq_push(struct heapq *hq,
				    void *elem, unsigned int pri);
static int		 heapq_swap(struct heapq *hq, size_t i, size_t j);

/* DELETE */
void			 heapq_pop(struct heapq *hq);
void			 heapq_free(struct heapq *hq,
				    void (*destruct)(void *elem));


/* -=[ IMPLEMENTATION ]=--------------------------------------------------- */
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


static
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


static
const void *
heapq_get(struct heapq *hq, size_t i)
{
	return (hq->buf + i * hq->elem_size) + sizeof (unsigned int);
}


const void *
heapq_peek(struct heapq *hq)
{
	if (a->len == 0)
		return NULL;
	else
		return heapq_get(hq, hq->len - 1);
}


static
unsigned int
heapq_pri(struct heapq *hq, size_t i)
{
	return *(unsigned int *)hq->buf + i * hq->elem_size;
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
}


static
int
heapq_swap(struct heapq *hq, size_t i, size_t j)
{
	void *tmp = malloc(a->elem_size);
	if (tmp == NULL)
		return 1;

	/* tmp = i; */
	memcpy(tmp, heapq_get(a, i), a->elem_size);
	/* i = j */
	memcpy(heapq_get(a, i), heapq_get(a, j), a->elem_size);
	/* j = tmp */
	memcpy(heapq_get(a, j), tmp, a->elem_size);

	free(tmp);
	return 0;
}


/*
 * Completely free the array.
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
