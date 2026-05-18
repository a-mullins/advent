Attempted to convert my dynamic array implementation to header-only and type-checking in the style of sys/queue.h. However, I noticed a quirk straight away. Consider:

    #define ALIST_HEAD(name, type)               \
        struct name {                            \
            size_t cap;                          \
            size_t len;                          \
            type *buf;                           \
        };

    #define ALIST_INIT(head) do {               \
            (head)->cap = 2;                    \
            (head)->len = 0;                    \
            buf = calloc(sizeof (*buf), 2);     \
            if (buf == NULL) {                  \
                /* What do we do? */            \
            }                                   \
        } while (0)                             \

A memory buffer must be allocated to store data in the list, but the *alloc family of functions might fail, returning NULL. How is can the macro indicate to 'calling' code that an error has occured? ALIST\_INIT isn't a function, so there is no return value. A do-while block isn't a valid rvalue, and neither is a compound statement, so err = ALIST\_INIT won't work.

sys/queue.h avoids this problem by never dynamically allocating memory in the macro; it's the callers resposibility to do so, like:

    if ((n1 = malloc(sizeof (...)) == NULL )
        /* handle errors */
    SLIST_INSERT_HEAD(&head, n1, entries);

But that's not possible with a dynamic array, because the data must be copied into the array, not pointed at. The array can't avoid managing a buffer, because that it is just a big buffer with some metadata.

Maybe error signaling could be done by requiring 'callers' to specify an error variable:

    #define ALIST_INIT(head, err) do {          \
            (head)->cap = 2;                    \
            (head)->len = 0;                    \
            buf = calloc(sizeof (*buf), 2);     \
			#if err                             \
			err = buf == NULL ? 1 : 0;          \
			#endif                              \
        } while (0)                             \

But that seems a little kludgey and more-or-less every macro would need it.
