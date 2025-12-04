#include <stdio.h>
#include <string.h>


#define WIDTH  140
#define HEIGHT 140


bool is_accessible(size_t width, char grid[][width], size_t x, size_t y);


int
main(void)
{
    char grid[HEIGHT+2][WIDTH+2];
    for(size_t y = 0; y < HEIGHT+2; y++)
        for(size_t x = 0; x < WIDTH+2; x++)
            grid[y][x] = '.';
    // Why doesn't this next line work instead?
    // Is it because '.' is promoted to int?
    // memset(grid, '.', sizeof (grid));
    char line[256] = {'\0'};
    size_t y = 0;
    while (NULL != fgets(line, 256, stdin)) {
        y++;
        line[strcspn(line, "\n")] = '\0';
        strncpy(&grid[y][1], line, strlen(line));
    }
    // for(size_t y = 0; y < HEIGHT+2; y++) {
    //     for(size_t x = 0; x < WIDTH+2; x++)
    //         printf("%c", grid[y][x]);
    //     puts("");
    // }
    int num_removed = 0;
    //printf("%s\n", is_accessible(WIDTH+2, grid, 1, 8) ? "true" : "false");
    bool removed_rolls = false;
    char new_grid[HEIGHT+2][WIDTH+2];
    memcpy(new_grid, grid, sizeof (grid));
    size_t generation = 0;
    do {
        printf("-=[Generation %2d]=-----------------------------------------------------------\n",
               generation++);
        
        removed_rolls = false;
        for(size_t y = 1; y < HEIGHT+1; y++) {
            for(size_t x = 1; x < WIDTH+1; x++) {
                if (is_accessible(WIDTH+2, grid, y, x)){
                    num_removed++;
                    new_grid[y][x] = '.';
                    removed_rolls = true;
                    printf("x");
                } else {
                    printf("%c", grid[y][x]);
                }
            }
            puts("");
        }
        // overwrite grid with new_grid.
        memcpy(grid, new_grid, sizeof (grid));
        puts("");
    } while(removed_rolls == true);
    //is_accessible(WIDTH+2, grid, 2, 1);
    fprintf(stderr, "%d\n", num_removed);
    return 0;
}


bool
is_accessible(size_t width, char grid[][width], size_t y, size_t x)
{
    if (grid[y][x] != '@')
        return false;
    size_t y_to_check = y;
    size_t x_to_check = x;
    short neighbors = 0;
    for(size_t y = y_to_check-1; y < y_to_check+2; y++) {
        for(size_t x = x_to_check-1; x < x_to_check+2; x++) {
            //printf("checking grid[%d][%d] -> %c\n", y, x, grid[y][x]);
            if (y == y_to_check && x == x_to_check) {
                //printf("Skipping this square...\n");
                continue;
            }
            if (grid[y][x] == '@') {
                neighbors++;
            }
            if(neighbors >= 4) {
                return false;
            }
        }
    }
    return true;
}
