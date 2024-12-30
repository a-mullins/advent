#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "darray.h"


#define ARRAY_LEN(X) sizeof X / sizeof X[0]


typedef struct memory {
    int blocks[16];
} memory;


bool
strtomem(char *s, memory *m)
{
    size_t cap = ARRAY_LEN(m->blocks);
    size_t len = 0;
    char *tok = strtok(s, " \t\r\n");
    do
    {
        m->blocks[len++] = atoi(tok);
    } while ((tok = strtok(NULL, " \t\r\n")) != NULL
             && len < cap);

    if(len != cap) {return false;}
    else           {return true;}
}


int
main(void)
{
    int ret_code = 0;
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, stdin) <= 0) {goto err0;}

    darray history = {0};
    darray_init(&history, sizeof (memory));
    darray_push(&history, &(memory){0});

    if (!strtomem(line, darray_get(&history, 0))) {goto err1;}

    // Generate states until we find a repeat (or run out of buffer).
    while (1) {
        memory m = {0};
        memcpy(m.blocks, darray_last(&history), sizeof m.blocks);

        // Find largest block's index and size.
        int blocks = m.blocks[0];
        int max_i = 0;
        for(size_t i = 1; i < ARRAY_LEN(m.blocks); i++) {
            if (m.blocks[i] > blocks) {
                blocks = m.blocks[i];
                max_i = i;
            }
        }

        // Set its size and redistribute it to the others.
        m.blocks[max_i] = 0;
        for (size_t i = max_i; blocks > 0; blocks--) {
            i = (i+1) % (ARRAY_LEN(m.blocks));
            m.blocks[i]++;
        }

        // check if we have seen this state before
        // ie, if (m in history);
        ssize_t n = darray_in(&history, &m);
        if (n >= 0) {
            printf("old: %ld, new: %ld, diff: %ld\n",
                   n,
                   history.len,
                   history.len-n);
            goto exit;
        }

        // This memory state doesn't match a previous one.
        // Push it on to the stack and try again.
        darray_push(&history, &m);
    }

exit:
err1:
    darray_free(&history);
err0:
    free(line);
    return ret_code;
}
