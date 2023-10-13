#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int dist(int target) {
    // square root of the bottom-right corner of target's square. eg:
    //          …  …
    // …  7  8  9 10
    // … 22 23 24 25       sqrt_corner_above of 23 is 5, of 7 is 3.
    int sqrt_corner_above = (int)ceilf(sqrtf((float)target));
    if(!(sqrt_corner_above % 2)) { // is even
        sqrt_corner_above++;
    }
    
    int      x =   (sqrt_corner_above-1)/2;
    int      y = -((sqrt_corner_above-1)/2);
    int square =   (sqrt_corner_above-1)/2;
    int  cur = sqrt_corner_above * sqrt_corner_above;

    enum directions {LEFT, RIGHT, UP, DOWN, LEN};
    enum directions dir = LEFT;
    int turn_at = cur - sqrt_corner_above + 1;

    while(cur != target && cur >= 0) {
        switch(dir) {
        case LEFT:  x--; break;
        case RIGHT: x++; break;
        case UP:    y++; break;
        case DOWN:  y--; break;
        }
        if(--cur == turn_at) {
            dir = (dir+1) % LEN;
            turn_at = cur - sqrt_corner_above + 1;
        }
    }
    
    return abs(x) + abs(y);
}

int main(void) {
    size_t buf_size = 0;
    char *buf = NULL;
    getline(&buf, &buf_size, stdin); // implicit malloc.
    int i = atoi(buf);
    free(buf);
    
    printf("%d\n", dist(i));
    return 0;
}