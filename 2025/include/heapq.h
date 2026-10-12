/* Copyright (c) 2026 Adam Mullins       */
/* Simple min-heap based priority queue. */
#pragma once
#include <stddef.h>
#include <sys/types.h>


struct heapq;

/* CREATE */
const struct heapq 	*heapq_new(size_t elem_size);

/* READ / QUERY */
size_t			 heapq_len(struct heapq *hq);
int			 heapq_empty(struct heapq *hq);
const void		*heapq_peek(struct heapq *hq);

/* UPDATE */
void			 heapq_push(struct heapq *hq,
				    void *elem, unsigned int pri);

/* DELETE */
void	 		 heapq_pop(struct heapq *hq);
void	 		 heapq_free(struct heapq *hq,
				    void (*destruct)(void *elem));
