#include <stdio.h>
#include <string.h>

#include "../lib/darray.h"


#define HEIGHT 256
#define WIDTH  256


struct pos {
    long x;
    long y;
};


int
main(void)
{
    struct darray tachyon_beams;
    darray_init(&tachyon_beams, sizeof (struct pos));
    char tachyon_manifold[HEIGHT][WIDTH] = {'\0'};

    for (size_t row = 0;
         row<HEIGHT && NULL != fgets(tachyon_manifold[row], WIDTH, stdin);
         row++) { ; }

    char *start;
    if (NULL == (start = strchr(tachyon_manifold[0], 'S'))) {
        fprintf(stderr, "ERR invalid input.\n");
        return 1;
    } else {
        struct pos start_pos = {.x = 0, .y = 0};
        start_pos.x = (start - tachyon_manifold[0]) / sizeof (char);
        darray_push(&tachyon_beams, &start_pos);
    }

    // start_pos.x = 99;
    // start_pos.y = 99;
    // darray_push(&tachyon_beams, &start_pos);

    // start_pos.x = 53;
    // start_pos.y = 2;
    // darray_push(&tachyon_beams, &start_pos);

    // start_pos.x = 0;
    // start_pos.y = 0;
    // ssize_t i = darray_in(&tachyon_beams, &start_pos);
    // if (i >= 0) {
    //     struct pos *res = (struct pos *)darray_get(&tachyon_beams, i);
    //     printf("x: %d, y: %d\n", res->x, res->y);
    // }

    while (tachyon_beams.len) {
        struct pos *res = (struct pos *)darray_last(&tachyon_beams);
        printf("x: %d, y: %d\n", res->x, res->y);
        darray_del(&tachyon_beams, tachyon_beams.len-1);
    }
    
    return 0;
}
