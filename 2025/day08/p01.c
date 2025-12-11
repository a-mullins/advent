#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


// The input has 1000 nodes. To make an adajency matrix, we will need to
// allocate an array of 1000 * 1000 elements. We might as well round up to
// 1024, since memory should usually be allocated in 2^n chunks anyway. If we
// use the standard bool type, that means 1024 * 1024 * 1 Byte = 1 Mebibyte,
// which is managable. On the other hand, if we do some bit packing use only a
// single bit to mark when an edge exists, we can reduce this memory to 1/8th,
// or just 128 KiB.


typedef struct {long x; long y; long z;} vec3;
typedef struct {vec3 v0; vec3 v1; float d;} threeple; // 1-ple, tuple, 3-ple.


static vec3 box[1000] = {0};
static size_t box_len = 0;
static bool connected[1000] = {false};
threeple pair[499'500] = {0}; // Combinations(1000,2) = 499,500.
static size_t pair_len = 0;
static bool adjacent[1000][1000] = {0};
static long connected_component_size[1000];


int threeplecmp(const void *a, const void*b);
inline static float dist(vec3 u, vec3 v);
inline static ssize_t get_id_by_vec3(vec3 v);
void bfs(size_t root_id, bool seen[1000]);
int revlongcmp(const void *a, const void *b);


int
main(int argc, char **argv)
{
    char line[128];
    for (/*nop*/; NULL != fgets(line, 128, stdin); box_len++) {
        vec3 v;
        sscanf(line, "%ld,%ld,%ld", &v.x, &v.y, &v.z);
        box[box_len] = v;
    }

    // for (int i = 0; i<box_len; i++)
    //     printf("%5ld %5ld %5ld\n", box[i].x, box[i].y, box[i].z);

    for (size_t i = 0; i<box_len; i++) {
        for (size_t j = i; j<box_len; j++) {
            if (i == j) continue;
            // printf("i = %3d, j = %3d, res = %3d | ", i, j, pair_len);
            pair[pair_len++] = (threeple){box[i], box[j], dist(box[i], box[j])};
            // printf("box[i] = %3d %3d %3d, box[j] = %5d %5d %5d, d = %.2f\n",
            //        box[i].x, box[i].y, box[i].z,
            //        box[j].x, box[j].y, box[j].z,
            //        dist(box[i], box[j]));
        }
    }

    qsort(pair, pair_len, sizeof (pair[0]), threeplecmp);

    puts("-=[ 1000 Closest Junction Boxes ]=-------"
         "---------------------------------------");
    for (size_t i = 0; i<1000; i++) {
        printf("pair %3d: v0 = [%5d %5d %5d], v1 = [%5d %5d %5d], d = %.2f\n",
               i,
               pair[i].v0.x, pair[i].v0.y, pair[i].v0.z,
               pair[i].v1.x, pair[i].v1.y, pair[i].v1.z,
               pair[i].d);
        ssize_t v0_id = get_id_by_vec3(pair[i].v0);
        ssize_t v1_id = get_id_by_vec3(pair[i].v1);
        printf("         v0_id: %-18ld v1_id: %ld\n\n", v0_id, v1_id);

        adjacent[v0_id][v1_id] = true;
        adjacent[v1_id][v0_id] = true;
        connected[v0_id] = true;
        connected[v1_id] = true;
    }

    puts("-=[ Adjacency Matrix ]=----------------"
         "---------------------------------------");
    puts("  01234567890123456789\n"
         " +--------------------");
    for (size_t i = 0; i < 20; i++) {
        printf("%d|", i % 10);
        for (size_t j = 0; j < 20; j++) {
            printf("%s", adjacent[i][j] ? "*" : ".");
        }
        puts("");
    }

    puts("");
    bool graph_seen[1000] = {false};
    for (size_t i = 0; i<1000; i++)
        if (/* connected[i] && */ !graph_seen[i]) {
            printf("-=[ Connected Component from %02ld ]=-----"
                   "---------------------------------------\n", i);
            bool component_seen[1000] = {false};
            bfs(i, component_seen);
            long size = 0;
            for (size_t j = 0; j < 1000; j++) {
                if (component_seen[j]) size++;
                graph_seen[j] |= component_seen[j];
            }
            connected_component_size[i] = size;
            printf("Circuit has %ld boxes.\n", size);
        }

    qsort(connected_component_size, 1000,
          sizeof (connected_component_size[0]), revlongcmp);

    for(int i = 0; i<3; i++)
        printf("%ld\n", connected_component_size[i]);
    puts("");

    printf("%ld\n", connected_component_size[0]
                    * connected_component_size[1]
                    * connected_component_size[2]);
    return 0;
}


inline static
float
dist(vec3 u, vec3 v)
{
    return sqrtf(
        powf(((float)u.x - (float)v.x), 2) +
        powf(((float)u.y - (float)v.y), 2) +
        powf(((float)u.z - (float)v.z), 2) );
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


void
bfs(size_t root_id, bool seen[1000])
{
    if (!seen) {
        fprintf(stderr, "ERR seen must not be NULL\n");
        return;
    }

    printf("%2d [%5ld %5ld %5ld]\n", root_id,
           box[root_id].x, box[root_id].y, box[root_id].z);
    seen[root_id] = true;
    for (size_t to = 0; to<1000; to++)
        if (adjacent[root_id][to])
            if (!seen[to])
                bfs(to, seen);
}


int
revlongcmp(const void *a, const void *b) {
    long n = *(int *)a;
    long m = *(int *)b;

    if(n < m) return 1;
    if(n > m) return -1;
    return 0;
}
