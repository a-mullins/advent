#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned long find_rmost_col(char lines[5][4096]);
unsigned long solve_rmost_problem(char lines[5][4096], size_t *new_last);


int
main(void)
{
    unsigned long total = 0;
    char lines[5][4096] = {'\0'};
    for (size_t row = 0;
         row < 5 && fgets(&lines[row][0], 4096, stdin);
         row++)
            lines[row][strcspn(lines[row], "\n")] = '\0';

    size_t rmost_col = find_rmost_col(lines);
    while (rmost_col > 0)
        total += solve_rmost_problem(lines, &rmost_col);;
    printf("%ld\n", total);
    return 0;
}


unsigned long
find_rmost_col(char lines[5][4096])
{
    size_t rmost_col = 0;
    for(size_t row = 0; row < 5; row++) {
        size_t len = strlen(lines[row]);
        rmost_col = len-1 > rmost_col ? len-1 : rmost_col;
    }
    return rmost_col;
}


unsigned long
solve_rmost_problem(char lines[5][4096], size_t *rmost_col)
{
    size_t col = rmost_col ? *rmost_col : find_rmost_col(lines);
    unsigned long nums[5] = {0};
    char op = '\0';
    for (size_t i = 0; !op; i++) {
        char s[8] = {'\0'};
        for(size_t row = 0; row < 5; row++) {
            char c = lines[row][col];
            if (c == '*' || c == '+')
                op = c;
            else
                s[row] = c;
        }
        nums[i] = atol(s);
        if (col > 0) --col;
    }
    if (rmost_col)
        *rmost_col = col == 0 ? 0 : col-1;

    // Choose multiplicative or additive identity.
    unsigned long total = op == '*' ? 1 : 0;
    if (op == '+')
        for (size_t i = 0; nums[i] != 0 && i < 4; i++)
            total += nums[i];
    if (op == '*')
        for (size_t i = 0; nums[i] != 0 && i < 4; i++)
            total *= nums[i];
    return total;
}
