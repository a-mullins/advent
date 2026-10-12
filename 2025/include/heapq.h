/* Copyright (c) 2026 Adam Mullins       */
/* Simple min-heap based priority queue. */
#pragma once
#include <stddef.h>
#include <sys/types.h>


struct heapq;

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
void	 		 heapq_pop(struct heapq *hq);
void	 		 heapq_free(struct heapq *hq,
				    void (*destruct)(void *elem));
