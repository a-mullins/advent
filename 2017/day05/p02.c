#include <stdio.h>


#define OFFSETS_MAX 2048


static int offsets[OFFSETS_MAX] = {0};
static int offsets_len = 0;


int
main(void)
{
    short n;
    while (scanf("%hi", &n) != EOF && offsets_len < OFFSETS_MAX) {
        offsets[offsets_len++] = n;
    }

    int ip = 0; // instruction pointer
    int steps = 0;
    while (ip < offsets_len) {
        ip += (offsets[ip] >= 3 ? offsets[ip]-- : offsets[ip]++);
        steps++;
    }

    printf("steps: %d\n", steps);
}
