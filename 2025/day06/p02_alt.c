#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define WIDTH 4096
#define HEIGHT 6


void transpose_matrix(void *A, void *B, size_t size, size_t rows, size_t cols);


int
main(void)
{
    char buf[HEIGHT][WIDTH] = {'\0'};
    char buf_T[WIDTH][HEIGHT] = {'\0'};
    for (size_t row = 0; NULL != fgets(buf[row], WIDTH, stdin) && row<HEIGHT; row++) {
            buf[row][strcspn(buf[row], "\n")] = '\0';
    }
    transpose_matrix(buf, buf_T, sizeof (buf[0][0]), HEIGHT, WIDTH);

    unsigned long nums[4] = {0};
    size_t num_i = 0;
    char op = '\0';
    unsigned long grand_total = 0;
    for (int row = 0; row<WIDTH; row++) {
        long val = 0;
        if (op)
            sscanf(buf_T[row], "%ld %c", &val);
        else
            sscanf(buf_T[row], "%ld %c", &val, &op);
        if (!val) {
            printf("------------------------- THUNK\n");
            unsigned long sub_total = op == '*' ? 1 : 0;
            if (op == '+')
                for (size_t i = 0; nums[i] != 0 && i < 4; i++)
                    sub_total += nums[i];
            if (op == '*')
                for (size_t i = 0; nums[i] != 0 && i < 4; i++)
                    sub_total *= nums[i];
            printf("                          %ld", sub_total);
            grand_total += sub_total;
            memset(&nums, 0, 4 * sizeof (nums[0]));
            num_i = 0;
            op = '\0';
        } else {
            nums[num_i++] = val;
            printf("%5s   ", buf_T[row]);
            printf("val: %4ld   ", val);
            printf("op: %c", op);
        }
       puts("");
    }

    printf("%ld\n", grand_total);
    return 0;
}


void
transpose_matrix(void *A, void *B, size_t size, size_t rows, size_t cols)
{
    for (size_t row = 0; row<rows; row++)
        for (size_t col = 0; col<cols; col++)
            memcpy(B + size*(rows*col + row), A + size*(row*cols + col), size);
    return;
}
