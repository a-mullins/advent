#include <stdio.h>
#include <string.h>


typedef struct {
    long x;
    long y;
} vec2;


static char tachyon_manifold[256][256] = {'\0'};


int split_count(vec2 start_pos);


int
main(void)
{
    for (size_t row = 0;
         row<256 && NULL != fgets(tachyon_manifold[row], 256, stdin);
         row++) { ; }

    char *start = strchr(tachyon_manifold[0], 'S');
    vec2 start_pos = {.x = 0, .y = 0};
    start_pos.x = (start - tachyon_manifold[0]) / sizeof (char);

    printf("%ld\n", split_count(start_pos));
    return 0;
}


int
split_count(vec2 start_pos)
{
    vec2 cur = start_pos;
    char c;
    while ('.' == (c = tachyon_manifold[cur.y+1][cur.x]))
        tachyon_manifold[cur.y++][cur.x] = '|';

    if (c == '^')
        return 1 + split_count((vec2){.y = cur.y, .x = cur.x-1})
                 + split_count((vec2){.y = cur.y, .x = cur.x+1});
    
    return 0;
}
