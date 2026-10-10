#ifndef PQUEUE_H
#define PQUEUE_H
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>


/*
 *	✓ pqueue_decl(…);	| ✓ pqueue_foreach(…);
 *	✓ pqueue_init(…);	| ✓ pqueue_push(…);
 *	  pqueue_copy(…);	|   pqueue_replace(…);
 *	✓ pqueue_len(…);	|   pqueue_qsort(…);
 *	  pqueue_at(…);		|   pqueue_del_id(…);
 *	  pqueue_peek(…);	|   pqueue_del_val(…);
 *	✓ pqueue_pop(…);	| ✓ pqueue_clear(…);
 *	  pqueue_bsearch(…);	| ✓ pqueue_free(…);
 *	✓ pqueue_isempty(…)
 */


#define MIN_CAP 2


#define PQUEUE_DECL(name, type)						\
	struct name {							\
		size_t cap;						\
		size_t len;						\
		size_t *pri;						\
		type *buf;						\
	}


#define PQUEUE_INIT(list, err)						\
	do {								\
		(list)->cap = MIN_CAP;					\
		(list)->len = 0;					\
		(list)->buf = calloc(MIN_CAP, sizeof (*((list)->buf))); \
		(list)->pri = calloc(MIN_CAP, sizeof (*((list)->pri))); \
		(err) = (list)->buf == NULL ? 1 : 0;			\
	} while (0)


/* #define ALIST_COPY(old, new, err)					\ */
/* 	do {								\ */
/* 		(new)->cap = (old)->cap;				\ */
/* 		(new)->len = (old)->len;				\ */
/* 		(new)->buf = calloc((new)->len, sizeof (*((new)->buf)));\ */
/* 		if ((new)->buf == NULL) {				\ */
/* 			(err) = 1;					\ */
/* 			break;						\ */
/* 		}							\ */
/* 		memcpy((new)->buf, (old)->buf,				\ */
/* 		    len * sizeof (*((old)->buf)));			\ */
/* 		(err) = 0;						\ */
/* 	} while (0) */


#define PQUEUE_LEN(list)	(list)->len


#define PQUEUE_ISEMPTY(list)	(list)->len > 0


/* #define ALIST_AT(list, i)	*((list)->buf + (i)) */


/* #define ALIST_PEEK(list, i)	*((list)->buf + ((list)->len - 1)) */


#define PQUEUE_POP(list, elem)						\
	do {								\
		size_t idx = 0;						\
		size_t min = SIZE_MAX;					\
		for (int i = 0; i < (list)->len; i++) {			\
			if (*((list)->pri + i) < min) {			\
				idx = i;				\
				min = (*((list)->pri + i));		\
			}						\
		}							\
		(list)->len--;						\
		elem = *((list)->buf + idx);				\
	} while (0)						


#define PQUEUE_PUSH(list, elem, priority, err)				\
	do {								\
		(err) = 0;						\
		if ((list)->len + 1 >= (list)->cap)    {		\
			void *tmp = (list)->buf;			\
			void *tmp_pri = (list)->pri;			\
			size_t old_cap = (list)->cap;			\
			(list)->cap = (list)->cap * 3 >> 1;		\
			(list)->buf = realloc((list)->buf,		\
			    (list)->cap * sizeof (*((list)->buf)));	\
			(list)->pri = realloc((list)->pri,		\
			    (list)->cap * sizeof (*((list)->pri)));	\
			if ((list)->buf == NULL) {			\
				(list)->buf = tmp;			\
				(list)->pri = tmp_pri;			\
				(list)->cap = old_cap;			\
				err = 1;				\
				break;					\
			}						\
		}							\
		*((list)->buf + (list)->len) = elem;			\
		*((list)->pri + (list)->len) = priority;		\
		(list)->len++;						\
	} while (0)


#define PQUEUE_FOREACH(list, var, pri_var)				\
	for(size_t _i = 0;						\
	    (var) = *((list)->buf + _i),				\
		    (pri_var) = *((list)->pri + _i),			\
	    _i < (list)->len;						\
	    _i++)


/* #define ALIST_QSORT(list, cmp)						\ */
/* 	qsort((list)->buf, (list)->len, sizeof (*((list)->buf)), (cmp)) */


/* #define ALIST_BSEARCH(list, key, cmp)					\ */
/* 	bsearch((key), (list)->buf, (list)->len,			\ */
/* 		sizeof (*((list)->buf)), (cmp)) */


#define PQUEUE_CLEAR(list, destruct)					\
	do {								\
		if (destruct != NULL) {					\
			void (*_f)(void *) = (destruct);		\
			for(size_t _i = 0; _i < (list)->len; _i++)	\
				(*_f)( *(void **)((list)->buf + _i) );	\
		}							\
		(list)->len = 0;					\
	} while (0)


#define PQUEUE_FREE(list, destruct)					\
	do {								\
		PQUEUE_CLEAR((list), (destruct));			\
		free((list)->buf);					\
		(list)->buf = NULL;					\
		(list)->cap = 0;					\
	} while (0)


#endif
