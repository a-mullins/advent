#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DELIMS " \t\r\n"
#define WORDS_MAX 32


int
main(void)
{
    int valid_count = 0;

    char *words[WORDS_MAX] = {0};
    char line[128];
    while (fgets(line, 128, stdin)){
        // Read in line, split into char *words[]. We aren't
        // allocating new memory here, we are just storing indicies
        // into `line`.
        short words_len = 0;
        char *tok = strtok(line, DELIMS);
        do {
            words[words_len++] = tok;
            tok = strtok(NULL, DELIMS);
        } while (tok !=NULL && words_len<WORDS_MAX);

        // Check every token against every other token in the string.
        //
        // If every token is unique, execution falls out of the for-loop
        //   and valid_count is incremented.
        // If there are two matching tokens, then we break out with goto.
        // Faster might be sorting then checking for adjacents, which would be
        //   O(n log n + n) vs O(n^2 - n)... I think.
        for (short i = 1; i < words_len; i++) {
            for (short j = 0; j < i; j++) {
                if (!strcmp(words[i], words[j])) {
                    goto next;
                }
            }
        }
        valid_count++;

    next:
        /* nop */;
    }

    printf("%d\n", valid_count);
    return 0;
}
