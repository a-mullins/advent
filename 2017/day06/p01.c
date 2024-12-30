#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "darray.h"


#define ARRAY_LEN(X) sizeof X / sizeof X[0]


typedef struct memory {
    int blocks[16];
} memory;


void
get_input(darray *history)
{
    static const char *DELIMS = " \t\r\n";

    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, stdin) <= 0) {
        fprintf(stderr, "ERR");
        exit(1);
    }

    memory m = {0};
    size_t block_cap = ARRAY_LEN(m.blocks);
    size_t block_len = 0;
    char *tok = strtok(line, DELIMS);
    do
    {
        m.blocks[block_len++] = atoi(tok);
    } while ((tok = strtok(NULL, DELIMS)) != NULL
             && block_len < block_cap);

    darray_push(history, &m);
    free(line);

    // samurai error handling: return successful, or don't return at all.
    if(block_len != block_cap) {fprintf(stderr, "ERR\n"); exit(1);}
}


int
main(void)
{
    darray history = {0};
    darray_init(&history, sizeof (memory));

    get_input(&history);

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
        bool match;
        for (size_t i = 0; i < history.len; i++) {
            match = true;
            memory *old_m = (memory *)darray_get(&history, i);
            for (size_t j = 0; j < ARRAY_LEN(m.blocks); j++) {
                if (old_m->blocks[j] != m.blocks[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                printf("old: %ld, new: %ld, diff: %ld\n",
                       i,
                       history.len,
                       history.len-i);
                goto exit;
            }
        }

        // This memory state doesn't match a previous one.
        // Push it on to the stack and try again.
        darray_push(&history, &m);
    }

exit:
    darray_free(&history);
    return 0;
}
