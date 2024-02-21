#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    size_t len = 0;
    char *line = NULL;
    getline(&line, &len, stdin); // implicit malloc.
    int input = atoi(line);
    free(line);

    int rows = (int)ceil(sqrt(input));
    int cols = rows;
    int row = rows / 2;
    int col = cols / 2;
    int arr[rows][cols];
    memset(&arr, 0, sizeof(int) * rows * cols);
    arr[row][col] = 1;

    enum directions {RIGHT, UP, LEFT, DOWN, LEN};
    enum directions dir = RIGHT;
    bool change_dir = false;

    while((2 <= row && row <= rows-2) && (2 <= col && col <= cols-2)) {
        change_dir = false;
        // Go to open space in our current dir of travel.
        // Also, plan our next move. If we can turn counterclockwise, do so.
        switch(dir) {
        case RIGHT:
            col++;
            if(arr[row-1][col] == 0) { change_dir = true; }
            break;
        case UP:
            row--;
            if(arr[row][col-1] == 0) { change_dir = true; }
            break;
        case LEFT:
            col--;
            if(arr[row+1][col] == 0) { change_dir = true; }
            break;
        case DOWN:
            row++;
            if(arr[row][col+1] == 0) { change_dir = true; }
            break;
        case LEN:
            exit(1);
            break;
        }
        if(change_dir) { dir = (dir+1) % LEN; }

        // Mark this space with the sum of its neighbors.
        int acc = 0;
        for(int i = row-1; i <= row+1; i++) {
            for(int j = col-1; j <= col+1; j++) {
                if(i == row && j == col) { continue; }
                acc += arr[i][j];
            }
        }
        arr[row][col] = acc;

        // if this spot is our target, we are done.
        if(arr[row][col] > input) {
            printf("%d\n", arr[row][col]);
            break;
        }
    }

    return 0;
}
