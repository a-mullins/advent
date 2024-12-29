#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


int
charcmp(const void *p, const void *q)
{
    const char a = *(const char *)p;
    const char b = *(const char *)q;
    return (a > b) - (a < b);
}


int
main(void)
{
    const size_t WORD_LIMIT = 32;
    const char *delims = " \t\r\n";
    int valid_count = 0;

    size_t len = 0;
    char *line = NULL;
    char **words = calloc(WORD_LIMIT, sizeof (char *));
    while (getline(&line, &len, stdin) > 0) {
        memset(words, 0, WORD_LIMIT * sizeof (char *));
        size_t i = 0;
        char *tok = strtok(line, delims);
        do {
            words[i++] = tok;
            tok = strtok(NULL, delims);
        } while (tok != NULL && i<WORD_LIMIT);

        for (size_t i = 0; i<WORD_LIMIT && words[i] != NULL; i++) {
            qsort(words[i], strlen(words[i]), sizeof *words[0], charcmp);
            for (size_t j = 0; j < i; j++) {
                if (!strcmp(words[i], words[j])) {
                    goto done;
                }
            }
        }
        valid_count++;

    done:
        /* nop */
    }
    free(words);
    free(line);

    printf("%d\n", valid_count);
    return 0;
}
