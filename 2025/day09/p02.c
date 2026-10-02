#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


typedef struct {long x; long y;} point;


/*
 * Long is guaranteed to be at least 32 bits which is enough for this problem.
 *
 * These arrays act like a primative hash map, from an index to the original x
 * or y coordinate. Since, even though the points are scaled, their spatial
 * relationships to one another are presereved, we should be to carry out
 * operations on the indicies rather than the original coordinates.
 */
static long x_map[512] = {0};
static long y_map[512] = {0};
static int x_map_len = 0;
static int y_map_len = 0;

static point points[512] = {0};
static int point_len = 0;


int longcmp(const void *m, const void *n);
void uniq(long *, int *);
long long area(point, point);
point comptoreal(point);
int is_left(point, point, point);
int on_segment(point p, point q, point r);
int is_point_inside(point);
int all_points_inside(point, point);


int
main(void)
{
	if(fseek(stdin, 0, SEEK_SET) != 0) {
		fprintf(stderr, "ERR stdin not seekable\n");
		return 1;
	}

	char buf[128];
	while (NULL != fgets(buf, 128, stdin)) {
		long x, y;
		sscanf(buf, "%ld,%ld", &x, &y);
		x_map[x_map_len++] = x;
		y_map[y_map_len++] = y;

		if (x_map_len == 512) {
			fprintf(stderr, "ERR too much input\n");
			return 1;
		}
	}

	qsort(&x_map, x_map_len, sizeof (long), longcmp);
	qsort(&y_map, y_map_len, sizeof (long), longcmp);
	uniq(x_map, &x_map_len);
	uniq(y_map, &y_map_len);

	rewind(stdin);
	while (NULL != fgets(buf, 128, stdin)) {
		long true_x, true_y;
		long comp_x, comp_y;
		long *needle;
		sscanf(buf, "%ld,%ld", &true_x, &true_y);

		needle = (long *)bsearch(&true_x, x_map, x_map_len, sizeof (long), longcmp);
		comp_x = (size_t)(needle - x_map);

		needle = (long *)bsearch(&true_y, y_map, y_map_len, sizeof (long), longcmp);
		comp_y = (size_t)(needle - y_map);

		points[point_len++] = (point){comp_x, comp_y};
	}
	points[point_len++] = points[0];

	long long A = 0;
	for (int i = 0; i < point_len; i++) {
		for (int j = i+1; j < point_len; j++) {
			long long a = area(comptoreal(points[i]),
					   comptoreal(points[j]));
			if (a > A && all_points_inside(points[i], points[j]))
				A = a;
		}
	}

	printf("%lld\n", A);
	return 0;
}


int
longcmp(const void *p, const void *q)
{
	long n = *(long *)p;
	long m = *(long *)q;
	if (n < m) return -1;
	if (n > m) return  1;
	else       return  0;
}


void
uniq(long *base, int *n)
{
	int base_len = *n;

	long *buf = calloc(*n, sizeof (long));
	*buf = *base;
	int buf_len = 1;

	long cur = *base;
	for (int i = 1; i < base_len; i++) {
		if (*(base + i) != cur) {
			cur = *(base + i);
			*(buf + buf_len++) = *(base + i);
		}
	}

	memcpy(base, buf, sizeof (long) * buf_len);
	*n = buf_len;
	return;
}


long long
area(point p, point q)
{
    long long h = 1 + (p.y >  q.y ? p.y - q.y : q.y - p.y);
    long long w = 1 + (p.x >= q.x ? p.x - q.x : q.x - p.x);
    return h*w;
}


point
comptoreal(point p)
{
	return (point){x_map[p.x], y_map[p.y]};
}


/*
 * > 0 if r is left of the line through p and q
 * = 0 if r is colinear with pq
 * < 0 if r is right of the line
 */
int
is_left(point p, point q, point r)
{
    return (q.x - p.x) * (r.y - p.y)
	- (r.x -  p.x) * (q.y - p.y);
}


/* Is point r on line segment pq? */
int
on_segment(point p, point q, point r)
{
    long crossproduct = (r.y - p.y) * (q.x - p.x) - (r.x - p.x) * (q.y - p.y);
    
    //compare versus epsilon for floating point values, or != 0 if using integers
    if (crossproduct != 0)
	return 0;
    /* if (fabs(crossproduct) > FLT_EPSILON) */
    /* 	return 0; */

    long dotproduct = (r.x - p.x) * (q.x - p.x) + (r.y - p.y)*(q.y - p.y);
    if (dotproduct < 0)
        return 0;

    long squaredlengthba = (q.x - p.x)*(q.x - p.x) + (q.y - p.y)*(q.y - p.y);
    if (dotproduct > squaredlengthba)
	return 0;

    return 1;
}


// return the winding number, which is only 0 if p is outside the poly
int
is_point_inside(point p)
{
    int wn = 0;    // the  winding number counter

    // loop through all edges of the polygon
    for (int i=0; i<point_len-1; i++) {  // edge from V[i] to  V[i+1]
	if (on_segment(points[i], points[i+1], p) == 1)
	    return 1;
	
        if (points[i].y <= p.y) {          // start y <= P.y
            if (points[i+1].y  > p.y)      // an upward crossing
                 if (is_left( points[i], points[i+1], p) > 0)  // P left of  edge
                     ++wn;            // have  a valid up intersect
        }
        else {                        // start y > P.y (no test needed)
            if (points[i+1].y  <= p.y)     // a downward crossing
                 if (is_left( points[i], points[i+1], p) < 0)  // P right of  edge
                     --wn;            // have  a valid down intersect
        }
    }
    return wn;
}


int
all_points_inside(point p, point q)
{
    long min_x = MIN(p.x, q.x);
    long min_y = MIN(p.y, q.y);
    long max_x = MAX(p.x, q.x);
    long max_y = MAX(p.y, q.y);

    // First, check the extrema.
    if (is_point_inside((point){min_x, min_y}) == 0)
	return 0;
    if (is_point_inside((point){max_x, min_y}) == 0)
	return 0;
    if (is_point_inside((point){min_x, max_y}) == 0)
	return 0;
    if (is_point_inside((point){max_x, max_y}) == 0)
	return 0;

    for(int x = min_x; x <= max_x; x++)
	for(int y = min_y; y <= max_y; y++)
	    if (is_point_inside((point){x, y}) == 0)
		return 0;

    return 1;
}
