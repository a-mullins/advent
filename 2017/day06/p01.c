#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BANKS_MAX 16


typedef struct {
    short banks[BANKS_MAX];
} memory;


void
strtomem(char *s, memory *m);


int
main(void)
{
    char line[64];
    fgets(line, 64, stdin);

    size_t history_cap = 256;
    size_t history_len = 0;
    memory *history = calloc(history_cap, sizeof (memory));

    strtomem(line, &history[0]);
    history_len++;

    // Generate states until we find a repeat (or run out of buffer).
    while (1) {
        memory m = {0};
        memcpy(&m, &history[history_len-1], sizeof (memory));

        // Find largest block's index and size.
        int blocks = m.banks[0];
        int max_i = 0;
        for(size_t i = 1; i < BANKS_MAX; i++) {
            if (m.banks[i] > blocks) {
                blocks = m.banks[i];
                max_i = i;
            }
        }

        // Set its size and redistribute it to the others.
        m.banks[max_i] = 0;
        for (size_t i = max_i; blocks > 0; blocks--) {
            i = (i+1) % BANKS_MAX;
            m.banks[i]++;
        }

        // check if we have seen this state before
        // ie, if (m in history);
        for(size_t i = 0; i < history_len; i++) {
            if (!memcmp(&m, &history[i], sizeof (memory))) {
                printf("old: %ld, new: %ld, diff: %ld\n",
                       i,
                       history_len,
                       history_len-i);
                goto exit;
            }
        }

        // This memory state doesn't match a previous one.
        // Push it on the stack and try again.
        if (history_len == history_cap) {
            history_cap <<= 1;
            // fprintf(stderr,
            //         "> history deepens.\n  (%d elem, %d bytes)\n",
            //         history_cap, history_cap * sizeof (memory));
            // NB realloc could fail and return NULL,
            //    but we choose to ignore error handling.
            history = realloc(history, history_cap * sizeof (memory));
        }
        memcpy(&history[history_len++], &m, sizeof (memory));
    }

exit:
    return 0;
}


void
strtomem(char *s, memory *m)
{
    short i = 0;
    char *tok = strtok(s, " \t\r\n");
    do
    {
        m->banks[i++] = atoi(tok);
    } while ((tok = strtok(NULL, " \t\r\n")) != NULL
             && i < BANKS_MAX);
}
