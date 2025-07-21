// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2017, Day 04, Part 1
//   https://adventofcode.com/2017/day/4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DELIMS " \t\r\n"
#define WORDS_MAX 32


int
main(void)
{
    int valid_count = 0;

    char *words[WORDS_MAX] = {0};
    char line[128];
    while (fgets(line, 128, stdin)){
        // Read in line, split into char *words[]. No new memory is
        // malloc'd here, we are only storing pointers into `line`.
        short words_len = 0;
        char *tok = strtok(line, DELIMS);
        do {
            words[words_len++] = tok;
            tok = strtok(NULL, DELIMS);
        } while (tok != NULL && words_len < WORDS_MAX);

        // Check every token against every other token in the string.
        //
        // If every token is unique, execution falls out of the for-loop
        //   and valid_count is incremented.
        // If there are two matching tokens, then we break out with goto.
        // Faster might be sorting then checking for adjacents, which would be
        //   O(n log n + n) vs O(n^2 - n)... I think.
        for (short i = 1; i < words_len; i++) {
            for (short j = 0; j < i; j++) {
                if (!strcmp(words[i], words[j])) {
                    goto not_valid;
                }
            }
        }
        valid_count++;

    not_valid:
        ; /* nop */
    }

    printf("%d\n", valid_count);
    return 0;
}
