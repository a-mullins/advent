// Copyright (c) 2025 Adam Mullins
//
// Alternative Solution for Advent of Code 2017, Day 04, Part 1
//   https://adventofcode.com/2017/day/4
//
// In the other solution, every word in a line's uniqueness is checked
// by comparing it to every other word in that line. That is something
// like O(n^2 - n), I think.
//
// By comparison, this solution sorts the list by using stdlib's qsort
// and then comparing adjacents for O(n log n + n).
//
// Of course when n = 512, as it does with the provided input, the
// difference is likely so negligible that it's difficult to even
// measure empirically with /usr/bin/time.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DELIMS " \t\r\n"
#define WORDS_MAX 32


int
wordcmp(const void *p, const void *q);


int
main(void)
{
    int valid_count = 0;

    char *words[WORDS_MAX] = {0};
    char line[128];
    while (fgets(line, 128, stdin)){
        short words_len = 0;
        char *tok = strtok(line, DELIMS);
        do {
            words[words_len++] = tok;
            tok = strtok(NULL, DELIMS);
        } while (tok !=NULL && words_len<WORDS_MAX);

        qsort(words, (size_t)words_len, sizeof (*words), wordcmp);

        for (short i = 1; i < words_len; i++) {
            if (!strcmp(words[i], words[i-1])) {
                goto next_line;
            }
        }
        valid_count++;

    next_line:
        /* nop */
    }

    printf("%d\n", valid_count);
    return 0;
}


int
wordcmp(const void *p, const void *q)
{
    const char *a = *(const char **)p;
    const char *b = *(const char **)q;
    return strcmp(a, b);
}
