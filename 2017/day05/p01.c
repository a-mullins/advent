#include <stdio.h>  // getline, &c
#include <stdlib.h> // atoi, free

#define MAX_JUMPS 2048

int main(void) {
    int jumps[MAX_JUMPS] = { 0 };
    int jumps_len = 0;

    char *buf = NULL;
    size_t buf_size = 0;
    while(getline(&buf, &buf_size, stdin) != -1 && jumps_len < MAX_JUMPS) {
        jumps[jumps_len++] = atoi(buf);
    }
    free(buf);

    int ip = 0; // instruction pointer
    int steps = 0;
    while(0 <= ip && ip < jumps_len) {
        ip += jumps[ip]++;
        steps++;
    }

    printf("steps: %d\n", steps);
}
