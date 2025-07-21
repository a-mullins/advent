// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2017, Day 02, Part 1
//   https://adventofcode.com/2017/day/2
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DELIMS " \r\n\t"


int
main(void)
{
    int sum = 0;

    char line[512];
    while (fgets(line, 512, stdin)) {
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

    printf("%d\n", sum);

    return 0;
}
