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
static int		 is_heapq(struct heapq *hq);                /* TODO */
static int		 lchild_i(idx);                             /* TODO */
static int		 rchild_i(idx);                             /* TODO */
static int		 parent_i(idx);                             /* TODO */

/* UPDATE */
void			 heapq_push(struct heapq *hq,
				    void *elem, unsigned int pri);
static int		 heapq_swap(struct heapq *hq, size_t i, size_t j);
static void		 perc_up(struct heapq *hq, size_t i);       /* TODO */
static void		 sink_down(struct heapq *hq, size_t i);     /* TODO */

/* DELETE */
void			 heapq_pop(struct heapq *hq);               /* TODO */
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
	/* TODO Heapify after appending a new element. */
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


static void
perc_up(struct heapq *hq, size_t i)
{
        /* //console.log(`_perc_up(${idx})`); */
        /* // base case. */
        /* if(idx === 0) { */
        /*     //console.log("base-case"); */
        /*     return; */
        /* } */
        /* else { */
        /*     // so we don't have to write `this.` everywhere... */
        /*     const elem = this.elem_at(idx); */
        /*     const parent_i = this.parent_i(idx); */
        /*     const parent_elem = this.elem_at(parent_i); */

        /*     // console.log(`parent_elem.priority > elem.priority: `+ */
        /*     //             `${parent_elem.pri > elem.pri}`); */
        /*     if(parent_elem.pri > elem.pri) { */
        /*         // if parent's priority is higher, swap parent and this child */
        /*         // console.log("parent's priority is higher than child's"); */
        /*         // console.log(this.array); */
        /*         // put this this.element in parent's index, */
        /*         this.array[parent_i] = elem; */
        /*         // put parent in this this.element's indx. */
        /*         this.array[idx] = parent_elem; */
        /*         //console.log(this.array); */
        /*         // now perc_up from new location */
        /*         this._perc_up(parent_i); */
        /*     } else { */
        /*         // this element's priority is higher than its parent's, it can */
        /*         // stay put. */
        /*         //console.log("parent's priority is lower than child's"); */
        /*         return; */
        /*     } */
        /* } */
}


static void
sink_down(struct *hq, size_t i)
{
        /* //console.log(`_sink_down(${idx})`); */
        /* //console.log(`this.elem_at(idx): ${this.elem_at(idx)}`); */
        /* const elem = this.elem_at(idx) || {pri:-Number.MAX_SAFE_INTEGER}; */
        /* const lchild_i = this.lchild_i(idx); */
        /* const rchild_i = this.rchild_i(idx); */
        /* const lchild = this.elem_at(lchild_i) || {pri:Number.MAX_SAFE_INTEGER}; */
        /* const rchild = this.elem_at(rchild_i) || {pri:Number.MAX_SAFE_INTEGER}; */
        /* const smallest_i = rchild.pri < lchild.pri ? rchild_i : lchild_i; */
        /* const smallest = this.elem_at(smallest_i); */
        /* //console.log(`\tparent{ ${elem.pri} } =>`); */
        /* //console.log("\t\t"+lchild.pri); */
        /* //console.log("\t\t"+rchild.pri); */

        /* // done when both children are >= this priority, or */
        /* // there are no children and we are a leaf. */
        /* if(lchild.pri >= elem.pri && rchild.pri >= elem.pri) { */
        /*     //console.log("\tbase-case"); */
        /*     return; */
        /* } */
        /* // otherwise, swap this node with the smallest of its children & repeat. */
        /* //console.log("\tidx, smallest_i, smallest.pri, elem.pri"); */
        /* //console.log(`\t${idx}, ${smallest_i}, ${smallest.pri}, ${elem.pri}`); */
        /* //console.log(`this.array[${idx}] = ${this.elem_at(smallest_i)};`); */
        /* this.array[smallest_i] = elem; */
        /* this.array[idx] = smallest; */
        /* this._sink_down(smallest_i); */
        /* // if(lchild.pri < elem.pri) { */
        /* //     //console.log("lchild.pri < elem.pri"); */
        /* //     //console.log(`${lchild.pri} < ${elem.pri}`); */
        /* //     // swap lchild with this element */
        /* //     this.array[lchild_i] = elem; */
        /* //     this.array[idx] = lchild; */
        /* //     this._sink_down(lchild_i); */
        /* // } else if(rchild.pri < elem.pri) { */
        /* //     // swap rchild with this element */
        /* //     this.array[rchild_i] = elem; */
        /* //     this.array[idx] = rchild; */
        /* //     this._sink_down(rchild_i); */
        /* // } */
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
