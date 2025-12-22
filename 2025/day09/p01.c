#include <stdio.h>


typedef struct {unsigned long x; unsigned long y;} vec2;


vec2 tile[512] = {0};
unsigned int tile_len = 0;


static unsigned long long area(vec2 v0, vec2 v1);


int
main(void)
{
    char line[128];
    while (tile_len<512 && NULL != fgets(line, 128, stdin)) {
        vec2 v;
        sscanf(line, "%ld,%ld", &v.x, &v.y);
        tile[tile_len++] = v;
    }

    unsigned long long A = 0;
    for (unsigned int i = 0; i<tile_len; i++)
        for (unsigned int j = i; j<tile_len; j++) {
            if (i == j) continue;
            A = A > area(tile[i], tile[j]) ? A : area(tile[i], tile[j]);
        }

    printf("%lld\n", A);
    return 0;
}


static
unsigned long long
area(vec2 v0, vec2 v1)
{
    unsigned long long h = 1 + (v0.y > v1.y  ? v0.y - v1.y : v1.y - v0.y);
    unsigned long long w = 1 + (v0.x >= v1.x ? v0.x - v1.x : v1.x - v0.x);
    return h*w;
}
