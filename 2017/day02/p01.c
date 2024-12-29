#include <limits.h> // INT_MIN, INT_MAX
#include <stddef.h> // NULL
#include <stdio.h>  // getline(), printf(), stdin
#include <stdlib.h> // atoi(), free()
#include <string.h> // strtok()


int
main(void)
{
    const char DELIMS[] = " \r\n\t";
    int sum = 0;

    size_t len = 0;
    char *line = NULL;
    while (getline(&line, &len, stdin) > 0) {
        char *tok = NULL;
        int max = INT_MIN;
        int min = INT_MAX;
        // for each token of line
        tok = strtok(line, DELIMS);
        do {
            int cur = atoi(tok);
            if (cur > max) {max = cur;}
            if (cur < min) {min = cur;}
            tok = strtok(NULL, DELIMS);
        } while (tok != NULL);

        sum += max - min;
    }
    free(line);

    printf("%d\n", sum);
    return 0;
}
