// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2025, Day 01, Part 02
// https://adventofcode.com/2025/day/1
#include <stdio.h>


#define LEN 2048
static char line[LEN];


int
main(void)
{
    char dir = '\0';
    int dist = 0;
    int pos = 50;
    int zero_count = 0; // Not to be confused with Count Zero.
    while (fgets(line, LEN, stdin)) {
        sscanf(&line[0], "%c%d", &dir, &dist);
        // Brute. Force.
        for(; dist > 0; dist--) {
            if (dir == 'L')
                pos == 0 ? pos = 99 : pos--;
            if (dir == 'R')
                pos == 99 ? pos = 0 : pos++;
            if (pos == 0)
                zero_count++;
        }
    }
    printf("%d\n", zero_count);
    return 0;
}
