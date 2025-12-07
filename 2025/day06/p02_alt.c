#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define COLS 4096
#define ROWS 8


void transpose(void *A, void *B, size_t size, size_t rows, size_t cols);


int
main(void)
{
    char buf[ROWS][COLS] = {'\0'};
    char buf_T[COLS][ROWS] = {'\0'};
    for (size_t row = 0;
         row<ROWS && NULL != fgets(buf[row], COLS, stdin);
         row++) {;}
    transpose(buf, buf_T, sizeof (buf[0][0]), ROWS, COLS);

    unsigned long nums[8] = {0};
    size_t i = 0;
    char op = '\0';
    unsigned long grand_total = 0;
    for (int row = 0; row<COLS; row++) {
        sscanf(buf_T[row], "%ld %c", &nums[i], &op);
        if (nums[i]) i++;
        else {
            unsigned long sub_total = op == '*' ? 1 : 0;
            if (op == '+')
                for (size_t i = 0; nums[i] != 0 && i < 4; i++)
                    sub_total += nums[i];
            if (op == '*')
                for (size_t i = 0; nums[i] != 0 && i < 4; i++)
                    sub_total *= nums[i];
            if (!sub_total) break;
            grand_total += sub_total;
            memset(&nums, 0, 4 * sizeof (nums[0]));
            i = 0;
            op = '\0';
        }
    }

    printf("%ld\n", grand_total);
    return 0;
}


void
transpose(void *A, void *B, size_t size, size_t rows, size_t cols)
{
    for (size_t row = 0; row<rows; row++)
        for (size_t col = 0; col<cols; col++)
            memcpy(B + size*(rows*col + row), A + size*(row*cols + col), size);
    return;
}
