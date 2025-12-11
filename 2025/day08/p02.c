#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {long x; long y; long z;} vec3;
typedef struct {vec3 v0; vec3 v1; float d;} threeple; // 1-ple, tuple, 3-ple.


static vec3 box[1000] = {0};
static size_t box_len = 0;
threeple pair[499'500] = {0}; // Combinations(1000,2) = 499,500.
static size_t pair_len = 0;
static bool connected[1000] = {false};


int threeplecmp(const void *a, const void*b);
inline static float dist(vec3 u, vec3 v);
inline static ssize_t get_id_by_vec3(vec3 v);


int
main(int argc, char **argv)
{
    char line[128];
    while (box_len<1000 && NULL != fgets(line, 128, stdin)) {
        vec3 v;
        sscanf(line, "%ld,%ld,%ld", &v.x, &v.y, &v.z);
        box[box_len++] = v;
    }

    // Calculate distances for every possible pair of boxes.
    for (size_t i = 0; i<box_len; i++)
        for (size_t j = i; j<box_len; j++) {
            if (i == j) continue;
            pair[pair_len++] = (threeple){box[i], box[j], dist(box[i], box[j])};
        }

    // Sort by distance.
    qsort(pair, pair_len, sizeof (pair[0]), threeplecmp);

    // For every pair of junction boxes, from closest to most distant...
    for (size_t i = 0; i<pair_len; i++) {
        // ...connect the pair...
        ssize_t v0_id = get_id_by_vec3(pair[i].v0);
        ssize_t v1_id = get_id_by_vec3(pair[i].v1);
        connected[v0_id] = true;
        connected[v1_id] = true;

        // ...determine if all boxes are connected...
        bool all_connected = true;
        for (size_t i = 0; i < box_len; i++)
            all_connected &= connected[i];

        // ...and if so, report product of the x coords
        // of the last connected pair.
        if (all_connected) {
            printf("%ld\n", box[v0_id].x * box[v1_id].x);
            break;
        }
    }

    return 0;
}


inline static
float
dist(vec3 u, vec3 v)
{
    return sqrtf(powf(((float)u.x - (float)v.x), 2)+
                 powf(((float)u.y - (float)v.y), 2)+
                 powf(((float)u.z - (float)v.z), 2));
}


int
threeplecmp(const void *a, const void*b)
{
    threeple *A = (threeple *)a;
    threeple *B = (threeple *)b;

    if (A->d < B->d)
        return -1;
    if (A->d > B->d)
        return 1;
    return 0;
}


inline static
ssize_t
get_id_by_vec3(vec3 v)
{
    for (size_t i = 0; i < box_len; i++)
        if (box[i].x == v.x && box[i].y == v.y && box[i].z == v.z)
            return i;
    return -1;
}
