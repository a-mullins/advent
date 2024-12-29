#include <stddef.h> // NULL
#include <stdio.h>  // getline(), printf(), stdin
#include <stdlib.h> // atoi(), free()
#include <string.h> // strotk()


int
main(void)
{
    const char DELIMS[] = " \r\n\t";
    const char MAX_DIGITS = 32;
    int sum = 0;

    size_t len = 0;
    char *line = NULL;
    // for each line of input
    while (getline(&line, &len, stdin) > 0) {
        char *tok = NULL;
        int digits[MAX_DIGITS];
        int len = 0;

        // for each token of line
        tok = strtok(line, DELIMS);
        do {
            digits[len++] = atoi(tok);
            tok = strtok(NULL, DELIMS);
        } while (tok != NULL && len < MAX_DIGITS);

        // "if j divides i evenly" translates to
        // if (digits[i] % digits[j] == 0)
        // also note that we do not have to check if j > i in
        // sum += i / j because if i % j == 0, then i>=j.
        for (int i = 0; i<len; i++) {
            for (int j = 0; j<len; j++) {
                if (i == j) {continue;}
                if (digits[i] % digits[j] == 0) {
                    sum += digits[i] / digits[j];
                }
            }
        }
    }
    free(line);

    printf("%d\n", sum);
    return 0;
}
