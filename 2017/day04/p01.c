#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_LIMIT 32

int main(void) {
    char *delims = " \t\r\n";
    int valid_count = 0;

    size_t len = 0;
    char *line = NULL;
    char **words = calloc(WORD_LIMIT, sizeof (char *));
    while(getline(&line, &len, stdin) > 0){
        // Read in line, split into char *words[]. We aren't
        // allocating new memory here, we are just storing indicies
        // into `line`.
        memset(words, 0, WORD_LIMIT * sizeof (char *));
        int i = 0;
        char *tok = strtok(line, delims);
        do {
            words[i++] = tok;
            tok = strtok(NULL, delims);
        } while(tok !=NULL && i<WORD_LIMIT);

        // Check every token against every other token in the string.
        // if every token is unique, execution falls out of the
        //   for() and valid_count is incremented.
        // if there are two matching tokens, then we "break" by goto.
        //   C does not have multi-level breaks.
        for(int i = 0; i<WORD_LIMIT && words[i] != NULL; i++) {
            for(int j = 0; j < i; j++) {
                if(!strcmp(words[i], words[j])) {
                    goto next;
                }
            }
        }
        valid_count++;

    next:
        /* nop */
    }
    free(line);

    printf("%d\n", valid_count);
    return 0;
}
