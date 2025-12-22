#include <stdbool.h>
#include <stdio.h>
#include <string.h>


typedef struct {unsigned long x; unsigned long y;} vec2;
typedef struct {vec2 p0; vec2 p1;} line;


static line lines[512] = {0};
static unsigned int lines_len = 0;
static unsigned long row_min = -1; // will actually be max val
static unsigned long col_min = -1;
static unsigned long row_len = 0;
static unsigned long col_len = 0;


static unsigned long long area(vec2 v0, vec2 v1);
static bool do_lines_intersect(line l0, line l1);
static bool is_point_in_poly(vec2 p0);


int
main(void)
{
    char buf[128];
    vec2 prev, cur;
    // handle first line as a special case
    if (!fgets(buf, 128, stdin)) return 1;
    sscanf(buf, "%ld,%ld", &prev.x, &prev.y);
    while (NULL != fgets(buf, 128, stdin)) {
        sscanf(buf, "%ld,%ld", &cur.x, &cur.y);
        row_min = cur.y < row_min ? cur.y-1 : row_min;
        col_min = cur.x < col_min ? cur.x-1 : col_min;
        row_len = cur.y > row_len ? cur.y+1 : row_len;
        col_len = cur.x > col_len ? cur.x+1 : col_len;
        lines[lines_len] = (line){prev, cur};
        lines_len++;
        prev = cur;
    }
    // handle last line as a special case
    lines[lines_len++] = (line){cur, lines[0].p0};
    for (int i = 0; i < lines_len; i++) {
        printf("line %3d: %05ld,%05ld->%05ld,%05ld\n",
               i,
               lines[i].p0.x, lines[i].p0.y,
               lines[i].p1.x, lines[i].p1.y);
    }

    line l0 = {{1,1},{3,2}};
    line l1 = {{1,4},{2,-1}};
    line l2 = {{10,10},{30,20}};
    
    //printf("intersect? %d\n", do_lines_intersect(l0, l2));
    
    return 0;
}


unsigned long long
area(vec2 v0, vec2 v1)
{
    unsigned long long h = 1 + (v0.y > v1.y  ? v0.y - v1.y : v1.y - v0.y);
    unsigned long long w = 1 + (v0.x >= v1.x ? v0.x - v1.x : v1.x - v0.x);
    return h*w;
}


static
bool
do_lines_intersect(line l0, line l1)
{
    // This method is outlined in Graphics Gems III by Kirk David
    // pp 199-202 and developed by Franklin Antonio.

    return false;
}


// is the provided point inside the polygon implied by lines[]?
static
bool
is_point_in_poly(vec2 p0)
{
    
    return false;
}
