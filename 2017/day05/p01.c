#include <stdio.h>
#include <stdlib.h>


int
main(void)
{
    const size_t MAX_OFFSETS = 2048;
    int *offsets = calloc(MAX_OFFSETS, sizeof (int));
    size_t offsets_len = 0;

    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, stdin) != -1 && offsets_len < MAX_OFFSETS) {
        offsets[offsets_len++] = atoi(line);
    }
    free(line);

    size_t ip = 0; // instruction pointer
    int steps = 0;
    while (ip < offsets_len) {
        ip += offsets[ip]++;
        steps++;
    }
    free(offsets);

    printf("steps: %d\n", steps);
}
