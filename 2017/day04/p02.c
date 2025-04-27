#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define DELIMS " \t\r\n"
#define WORDS_MAX 32


int
charcmp(const void *p, const void *q);


int
main(void)
{
    int valid_count = 0;

    char *words[WORDS_MAX] = {0};
    char line[128];
    while (fgets(line, 128, stdin)) {
        short words_len = 0;
        char *tok = strtok(line, DELIMS);
        do {
            words[words_len++] = tok;
            tok = strtok(NULL, DELIMS);
        } while (tok != NULL && words_len<WORDS_MAX);

        for (short i = 0; i < words_len; i++) {
            qsort(words[i], strlen(words[i]), sizeof *words[0], charcmp);
            for (short j = 0; j < i; j++) {
                if (!strcmp(words[i], words[j])) {
                    goto done;
                }
            }
        }
        valid_count++;

    done:
        /* nop */
    }

    printf("%d\n", valid_count);
    return 0;
}


int
charcmp(const void *p, const void *q)
{
    const char a = *(const char *)p;
    const char b = *(const char *)q;
    return (a > b) - (a < b);
}
