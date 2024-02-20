#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int sum = 0;

    char delims[] = " \r\n\t";
    size_t buf_size = 0;
    char *buf = NULL;
    while(getline(&buf, &buf_size, stdin) > 0) {
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
    }
    free(buf);

    printf("%d\n", sum);
    return 0;
}
