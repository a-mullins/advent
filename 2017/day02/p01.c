#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    /* const int diff_size = 4096; */
    /* int diffs[diff_size]; */
    /* memset(&diffs, -1, diff_size * sizeof (int)); */

    int sum = 0;
    // for each line of file
    while(true) {
        size_t buf_size = 0;
        char *buf = NULL;
        if(getline(&buf, &buf_size, stdin) <= 0) { // implicit malloc()
            break;
        }

        char delims[] = " \r\n\t";
        char *tok = NULL;
        int max = INT_MIN;
        int min = INT_MAX;
        // for each token of line
        tok = strtok(buf, delims);
        do {
            int cur = atoi(tok);
            if(cur > max) { max = cur; }
            if(cur < min) { min = cur; }
            tok = strtok(NULL, delims);
        } while(tok != NULL);

        sum += max - min;

        free(buf);
    }

    printf("%d\n", sum);
    return EXIT_SUCCESS;
}
