#include <stdio.h>
#include <stdlib.h>

#define MAX_OFFSETS 2048

int main(void) {
    int *offsets = calloc(MAX_OFFSETS, sizeof (int));
    int offsets_len = 0;

    char *line = NULL;
    size_t len = 0;
    while(getline(&line, &len, stdin) != -1 && offsets_len < MAX_OFFSETS) {
        offsets[offsets_len++] = atoi(line);
    }
    free(line);

    int ip = 0; // instruction pointer
    int steps = 0;
    while(0 <= ip && ip < offsets_len) {
        ip += offsets[ip]++;
        steps++;
    }
    free(offsets);

    printf("steps: %d\n", steps);
}
