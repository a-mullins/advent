#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DELIMS " \r\n\t"
#define MAX_DIGITS 32


int
main(void)
{
    int sum = 0;

    char line[512];
    while (fgets(line, 512, stdin)) {
        char *tok = NULL;
        short digits[MAX_DIGITS];
        short len = 0;

        // for each token of line
        tok = strtok(line, DELIMS);
        do {
            digits[len++] = (short)atoi(tok);
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

    printf("%d\n", sum);
    return 0;
}
