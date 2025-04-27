#include <stdio.h>


#define OFFSETS_MAX 2048


int
main(void)
{
    short offsets[OFFSETS_MAX];
    short offsets_len = 0;

    short n;
    while (scanf("%hi", &n) != EOF && offsets_len < OFFSETS_MAX) {
        offsets[offsets_len++] = n;
    }

    size_t ip = 0; // instruction pointer
    int steps = 0;
    while (ip >= 0 && ip < offsets_len) {
        ip += offsets[ip]++;
        steps++;
    }

    printf("steps: %d\n", steps);
}
