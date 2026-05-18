#ifndef ALIST_H
#define ALIST_H
#include <stddef.h>
#include <stdlib.h>


/*
 *	✓ alist_new(…);		| ✓ alist_push(…);
 *	✓ alist_copy(…);	|   alist_replace(…);
 *	✓ alist_len(…);		| ✓ alist_qsort(…);
 *	✓ alist_at(…);		|   alist_del_id(…);
 *	✓ alist_peek(…);	|   alist_del_val(…);
 *	✓ alist_pop(…);		| ✓ alist_clear(…);
 *	✓ alist_bsearch(…);	| ✓ alist_free(…);
 */


#define MIN_CAP 2


#define ALIST_DECL(name, type)						\
	struct name {							\
		size_t cap;						\
		size_t len;						\
		type *buf;						\
	}


#define ALIST_INIT(list, err)						\
	do {								\
		(list)->cap = MIN_CAP;					\
		(list)->len = 0;					\
		(list)->buf = calloc(MIN_CAP, sizeof (*((list)->buf))); \
		(err) = (list)->buf == NULL ? 1 : 0;			\
	} while (0)


#define ALIST_COPY(old, new, err)					\
	do {								\
		(new)->cap = (old)->cap;				\
		(new)->len = (old)->len;				\
		(new)->buf = calloc((new)->len, sizeof (*((new)->buf)));\
		if ((new)->buf == NULL) {				\
			(err) = 1;					\
			break;						\
		}							\
		memcpy((new)->buf, (old)->buf,				\
		    len * sizeof (*((old)->buf)));			\
		(err) = 0;						\
	} while (0)


#define ALIST_LEN(list)		(list)->len


#define ALIST_AT(list, i)	*((list)->buf + (i))


#define ALIST_PEEK(list, i)	*((list)->buf + ((list)->len - 1))


#define ALIST_POP(list, i)	*((list)->buf + ((list)->len-- - 1))


#define ALIST_PUSH(list, elem, err)					\
	do {								\
		err = 0;						\
		if ((list)->len + 1 >= (list)->cap)    {		\
			void *tmp = (list)->buf;			\
			size_t old_cap = (list)->cap;			\
			(list)->cap = (list)->cap * 3 >> 1;		\
			(list)->buf = realloc((list)->buf,		\
			    (list)->cap * sizeof (*((list)->buf)));	\
			if ((list)->buf == NULL) {			\
				(list)->buf = tmp;			\
				(list)->cap = old_cap;			\
				err = 1;				\
				break;					\
			}						\
		}							\
		*((list)->buf + (list)->len++) = elem;			\
	} while (0)


#define ALIST_FOREACH(list, var)					\
	for(size_t _i = 0;						\
	    (var) = (list)->buf + _i, _i < (list)->len;			\
	    _i++)


#define ALIST_QSORT(list, cmp)						\
	qsort((list)->buf, (list)->len, sizeof (*((list)->buf)), (cmp))


#define ALIST_BSEARCH(list, key, cmp)					\
	bsearch((key), (list)->buf, (list)->len,			\
		sizeof (*((list)->buf)), (cmp))


#define ALIST_CLEAR(list, destruct)					\
	do {								\
		if (destruct != NULL) {					\
			void (*_f)(void *) = (destruct);		\
			for(size_t _i = 0; _i < (list)->len; _i++)	\
				(*_f)( *(void **)((list)->buf + _i) );	\
		}							\
		(list)->len = 0;					\
	} while (0)


#define ALIST_FREE(list, destruct)					\
	do {								\
		ALIST_CLEAR((list), (destruct));			\
		free((list)->buf);					\
		(list)->buf = NULL;					\
		(list)->cap = 0;					\
	} while (0)


#endif
