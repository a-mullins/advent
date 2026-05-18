// Copyright (c) 2025 Adam Mullins
#pragma once
#include <stdbool.h>
#include <stdlib.h>

#define STACK_MIN_CAP 8

#define stack_struct(T)                         \
    typedef struct T##_stack {                  \
        T *buf;                                 \
        size_t cap;                             \
        size_t len;                             \
    } T##_stack;


#define stack_init(T)                           \
    void                                        \
    T##_stack_init(T##_stack *s)                \
    {                                           \
        s->cap = STACK_MIN_CAP;                 \
        s->buf = calloc(s->cap, sizeof (T));    \
        s->len = 0;                             \
    }


#define stack_empty(T)                          \
    bool                                        \
    T##_stack_empty(T##_stack *s)               \
    {                                           \
        return (s->len == 0);                   \
    }


#define stack_pop(T)                            \
    T                                           \
    T##_stack_pop(T##_stack *s)                 \
    {                                           \
        return *(s->buf + s->len-- - 1);        \
    }


#define stack_peek(T)                           \
    T                                           \
    T##_stack_peek(T##_stack *s)                \
    {                                           \
        return *(s->buf + s->len - 1);          \
    }


#define stack_push(T)                                           \
    void                                                        \
    T##_stack_push(T##_stack *s, T elem) {                      \
        if (s->len == s->cap) {                                 \
            s->cap <<= 1;                                       \
            s->buf = realloc(s->buf, sizeof (T) * s->cap);      \
        }                                                       \
        s->buf[s->len++] = elem;                                \
    }


#define stack_free(T)                           \
    void                                        \
    T##_stack_free(T##_stack *s)                \
    {                                           \
        free(s->buf);                           \
        s->cap = 0;                             \
        s->len = 0;                             \
    }


#define stack_copy(T)                                           \
    void                                                        \
    T##_stack_copy(T##_stack *dest, T##_stack *src)             \
    {                                                           \
        dest->cap = src->cap;                                   \
        dest->buf = calloc(dest->cap, sizeof (T));              \
        dest->len = src->len;                                   \
        memcpy(dest->buf, src->buf, src->len * sizeof (T));     \
    }


#define STACK(T)                                \
    stack_struct(T)                             \
    stack_empty(T)                              \
    stack_init(T)                               \
    stack_pop(T)                                \
    stack_peek(T)                               \
    stack_push(T)                               \
    stack_copy(T)                               \
    stack_free(T)
