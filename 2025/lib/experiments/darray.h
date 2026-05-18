#idndef DARRAY_H
#define DARRAY_H
#include <stdlib.h>
#include <string.h>


#define MIN_CAP 8


#define DARR_STRUCT(T)                          \
    struct T##_darr {                           \
        T *buf;                                 \
        size_t cap;                             \
        size_t len;                             \
    };                                          \


#define DARR_INIT(T)                            \
    static void                                 \
    T##_darr_init(struct T##_darr *d)           \
    {                                           \
        d->cap = MIN_CAP;                       \
        d->len = 0;                             \
        d->buf = calloc(MIN_CAP, sizeof (T));   \
    }                                           \


#define DARR_LEN(T)                             \
    static inline                               \
    size_t                                      \
    T##_darr_len(struct T##_darr *d)            \
    {                                           \
        return d->len;                          \
    }                                           \


#define DARR_EMPTY(T)                           \
    static inline                               \
    bool                                        \
    T##_darr_empty(struct T##_darr *d)          \
    {                                           \
        return d->len == 0;                     \
    }                                           \


#define DARR_GET(T)                             \
    static inline                               \
    T                                           \
    T##_darr_get(struct T##_darr *d, size_t i)  \
    {                                           \
        return d->buf[i];                       \
    }                                           \


#define DARR_LAST(T)                            \
    static inline                               \
    T                                           \
    T##_darr_last(struct T##_darr *d)           \
    {                                           \
        return *(d->buf + (d->len - 1));        \
    }


// FIXME if realloc fails, access to d->buf pointer is lost
#define DARR_PUSH(T)                                            \
    static                                                      \
    void                                                        \
    T##_darr_push(struct T##_darr *d, T elem)                   \
    {                                                           \
        if(d->len+1 >= d->cap) {                                \
            d->cap <<= 1;                                       \
            d->buf = realloc(d->buf, sizeof (T) * d->cap);      \
        }                                                       \
        *(d->buf + d->len++) = elem;                            \
    }


#define DARR_POP(T)                             \
    static inline                               \
    T                                           \
    T##_darr_pop(struct T##_darr *d) {          \
        return *(d->buf + (--d->len));          \
    }                                           \


#define DARR_REPLACE(T)                                 \
    static inline                                       \
    void                                                \
    T##_darr_replace(struct T##_darr *d, size_t i, T new_elem) \
    {                                                   \
        *(d->buf + i) = new_elem;                       \
        return;                                         \
    }                                                   \


#define DARR_DEL(T)                                     \
    static void                                         \
    T##_darr_del(struct T##_darr *d, size_t i)          \
    {                                                   \
        if (i >= d->len) { return; }                    \
        if (i == d->len - 1) {                          \
            memset(d->buf + i, 0, sizeof (T));          \
            d->len--;                                   \
        } else {                                        \
            memmove(d->buf + i, d->buf + i + 1,         \
                    (d->len - (i+1)) * sizeof (T));     \
            d->len--;                                   \
        }                                               \
        return;                                         \
    }


#define DARR_FREE(T)                            \
    static inline                               \
    void                                        \
    T##_darr_free(struct T##_darr *d) {         \
        d->cap = 0;                             \
        d->len = 0;                             \
        free(d->buf);                           \
        d->buf = NULL;                          \
    }


#define DARR_REVERSE(T)                                 \
    static                                              \
    void                                                \
    T##_darr_reverse(struct T##_darr *d)                \
    {                                                   \
        T *tmp_buf = calloc(d->len, sizeof (T));        \
        for (size_t i = 0; i < d->len; i++) {           \
            tmp_buf[d->len - 1 - i] = d->buf[i];        \
        }                                               \
        memcpy(d->buf, tmp_buf, d->len * sizeof (T));   \
        free(tmp_buf);                                  \
        return;                                         \
    }                                                   \


#define DARR_IN(T)                                      \
    static                                              \
    ssize_t                                             \
    T##_darr_in(struct T##_darr *d, T elem)             \
    {                                                   \
        for (size_t i = 0; i<d->len; i++)               \
            if (!memcmp(d->buf + i, &elem, sizeof (T))) \
                return (ssize_t)i;                      \
        return -1;                                      \
    }


#define DARR_QSORT(T)                                           \
    static inline                                               \
    void                                                        \
    T##_darr_qsort(struct T##_darr *d,                          \
                   int (*compar)(const void *a, const void *b)) \
    {                                                           \
        qsort(d->buf, d->len, sizeof (T), compar);              \
    }


#define DARR_BSEARCH(T)                                         \
    ssize_t                                                     \
    T##_darr_bsearch(struct T##_darr *d,                        \
                   T key,                                       \
                   int (*compar)(const void *a, const void *b)) \
    {                                                           \
        T *key_p = (T *)bsearch(&key,                           \
                                d->buf,                         \
                                d->len,                         \
                                sizeof (T),                     \
                                compar);                        \
        if (key_p)                                              \
            return (ssize_t)(key_p - d->buf);                   \
        else                                                    \
            return -1;                                          \
    }

#define DARR(T)                                 \
    DARR_STRUCT(T)                              \
    DARR_INIT(T)                                \
    DARR_LEN(T)                                 \
    DARR_EMPTY(T)                               \
    DARR_GET(T)                                 \
    DARR_LAST(T)                                \
    DARR_PUSH(T)                                \
    DARR_POP(T)                                 \
    DARR_REPLACE(T)                             \
    DARR_DEL(T)                                 \
    DARR_FREE(T)                                \
    DARR_REVERSE(T)                             \
    DARR_IN(T)                                  \
    DARR_QSORT(T)                               \
    DARR_BSEARCH(T)

#endif
