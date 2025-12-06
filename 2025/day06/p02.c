#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned long find_rmost_col(char lines[4][4096]);
unsigned long solve_rmost_problem(char lines[4][4096], size_t *new_last);


int
main(void)
{
    unsigned long total = 0;
    char lines[5][4096] = {'\0'};
    // Ordering of conditionals in this for loop is important because
    // of conditional short-circuiting.
    for (size_t row = 0; row < 5; row++)
        if (!fgets(&lines[row][0], 4096, stdin))
            break;
        else
            lines[row][strcspn(lines[row], "\n")] = '\0';

    size_t rmost_col = find_rmost_col(lines);
    while (rmost_col > 0) {
        unsigned long res = solve_rmost_problem(lines, &rmost_col);
        total += res;
        printf("result: %ld\n", res);
    }
    puts("");
    printf("%ld\n", total);
    return 0;
}


unsigned long
find_rmost_col(char lines[4][4096])
{
    size_t rmost_col = 0;
    for(size_t row = 0; row < 4; row++) {
        size_t len = strlen(lines[row]);
        // while (len > 0 && (lines[row][len-1] == ' ' || lines[row][len-1] == '\0'))
        //     len--;
        rmost_col = len-1 > rmost_col ? len-1 : rmost_col;
    }
    return rmost_col;
}


unsigned long
solve_rmost_problem(char lines[4][4096], size_t *rmost_col)
{
    size_t col = 0;
    if (rmost_col)
        col = *rmost_col;
    else
        col = find_rmost_col(lines);
    
    unsigned long nums[4] = {0};
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
        printf("%ld\n", nums[i]);
        if (col > 0) --col;
    }
    if (rmost_col)
        //*rmost_col = col == (unsigned long)0 - 1 ? 0 : col-1;
        *rmost_col = col == 0 ? 0 : col-1;

    unsigned long total = op == '*' ? 1 : 0;
    if (op == '+')
        for (size_t i = 0; nums[i] != 0 && i < 4; i++)
            total += nums[i];
    if (op == '*')
        for (size_t i = 0; nums[i] != 0 && i < 4; i++)
            total *= nums[i];
    return total;
}
