#include <stdio.h>
#include <string.h>

#include "../lib/stack.h"


#define HEIGHT 256
#define WIDTH  256


typedef struct {
    long x;
    long y;
} vec2;


STACK(vec2);


void print_manifold(char manifold[HEIGHT][WIDTH]);


int
main(void)
{
    char tachyon_manifold[HEIGHT][WIDTH] = {'\0'};
    vec2_stack tachyon_beams;
    vec2_stack_init(&tachyon_beams);
    long split_count = 0;

    for (size_t row = 0;
         row<HEIGHT && NULL != fgets(tachyon_manifold[row], WIDTH, stdin);
         row++) { ; }

    char *start;
    if (NULL == (start = strchr(tachyon_manifold[0], 'S'))) {
        fprintf(stderr, "ERR invalid input.\n");
        return 1;
    } else {
        vec2 start_pos = {.x = 0, .y = 0};
        start_pos.x = (start - tachyon_manifold[0]) / sizeof (char);
        vec2_stack_push(&tachyon_beams, start_pos);
    }

    //print_manifold(tachyon_manifold);
    
    while (!vec2_stack_empty(&tachyon_beams)) {
        vec2 cur = vec2_stack_pop(&tachyon_beams);
        //printf("x: %ld, y: %ld\n", cur.x, cur.y);

        while (true) {
            // Visit this beam position by marking it.
            tachyon_manifold[cur.y][cur.x] = '|';
            // print_manifold(tachyon_manifold);
            // puts("");

            // Decide what to do next based on what the beam will
            // interact with next.
            char c = tachyon_manifold[cur.y+1][cur.x];
            if (c == '.') {
                cur.y++;
            } else if (c == '^') {
                split_count++;
                if (cur.x-1 >= 0
                    && tachyon_manifold[cur.y+1][cur.x-1] == '.')
                    vec2_stack_push(&tachyon_beams,
                                    (vec2){.x = cur.x-1, .y = cur.y+1});
                if (cur.x+1 <= WIDTH
                    && tachyon_manifold[cur.y+1][cur.x+1] == '.')
                    vec2_stack_push(&tachyon_beams,
                                    (vec2){.x = cur.x+1, .y = cur.y+1});
                break;
            } else {
                break;
            }
        }
    }

    vec2_stack_free(&tachyon_beams);
    printf("%ld\n", split_count);
    return 0;
}


void
print_manifold(char manifold[HEIGHT][WIDTH])
{
    for(size_t i = 0; manifold[i][0] != '\0'; i++)
        puts(manifold[i]);
}
