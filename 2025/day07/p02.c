// Today's puzzle asks us to find all compossible narratives through
// the given hemn space. :-)
// https://en.wikipedia.org/wiki/Anathem
// https://anathem.dlma.com/data/Hemn-space.html
// https://en.wikipedia.org/wiki/Compossibility
#include <stdio.h>
#include <string.h>

#include "../lib/stack.h"


#define HEIGHT        256
#define WIDTH         256
#define MAX_VERTICIES 2048


typedef struct {
    long x;
    long y;
} vec2;


STACK(ssize_t);


void print_manifold();
void print_adj_list();
ssize_t get_vertex_id_by_pos(vec2 pos);
ssize_t dfs(ssize_t id, ssize_t goal);


static char tachyon_manifold[HEIGHT][WIDTH];
static size_t vertex_len = 0;
static vec2 vertex_pos[MAX_VERTICIES] = { 0 };
static ssize_t vertex_adj[MAX_VERTICIES][2];
static size_t uniq_paths = 0;
static ssize_t paths_to_goal[MAX_VERTICIES];
// vertex_adj[id][0] is left child's id,
// vertex_adj[id][1] is right child's id.


int
main(void)
{
    memset(tachyon_manifold, 0, sizeof (char) * HEIGHT * WIDTH);
    memset(vertex_adj, -1, sizeof (ssize_t) * MAX_VERTICIES * 2);
    memset(paths_to_goal, 0, sizeof (ssize_t) * MAX_VERTICIES);

    for (size_t row = 0;
         row<HEIGHT && NULL != fgets(tachyon_manifold[row], WIDTH, stdin);
         row++)
    {
        for(size_t col = 0; tachyon_manifold[row][col] != '\0'; col++) {
            char c = tachyon_manifold[row][col];
            if (c == 'S' || c == '^')
                vertex_pos[vertex_len++] =(vec2){.x = (long)col,
                                                 .y = (long)row};
        }
    }

    // handle start to next vertex as special case
    vec2 pos = vertex_pos[0];
    do {
        pos.y++;
    } while (tachyon_manifold[pos.y][pos.x] == '.');
    vertex_adj[0][0] = get_vertex_id_by_pos(pos);

    // handle rest
    for (size_t from = 1; from<vertex_len; from++) {
        for(long x_offset = -1; x_offset <= 1; x_offset += 2) {
            vec2 pos = vertex_pos[from];
            pos.x += x_offset;
            do {
                pos.y++;
            } while (tachyon_manifold[pos.y][pos.x] == '.');
            vertex_adj[from][x_offset < 0 ? 0 : 1] = get_vertex_id_by_pos(pos);
        }
    }

    // create special end vertex and connect beams that run off the end to it.
    vertex_pos[vertex_len++] = (vec2){.x = 0, .y = 0};
    for (size_t i = 1; i < vertex_len-1; i++) {
        for (size_t j = 0; j < 2; j++) {
            if (vertex_adj[i][j] == -1)
                vertex_adj[i][j] = vertex_len-1;
        }
    }

    // print_manifold();
    // for (size_t i = 0; i < vertex_len; i++)
    //     printf("id %02ld: x = %2ld, y = %2ld\n",
    //            i, vertex_pos[i].x, vertex_pos[i].y);
    // print_adj_list();

    // ... and now do a dfs traversal of the DAG to find all possible
    // paths from node 0 to node vertex_len-1?
    // ssize_t_stack to_visit;
    // ssize_t_stack_init(&to_visit);
    // ssize_t_stack_push(&to_visit, 0);
    printf("%ld\n", dfs(0, vertex_len-1));

    //printf("%ld\n", uniq_paths);
    return 0;
}


void
print_manifold()
{
    size_t len = strlen(tachyon_manifold[0]) - 1;
    printf(" ");
    for (size_t i = 0; i < len; i++)
        printf("%ld", i % 10);
    printf("\n");
    
    for (size_t i = 0; tachyon_manifold[i][0] != '\0'; i++)
        printf("%1ld%s", i % 10, tachyon_manifold[i]);
}


void
print_adj_list()
{
    for (size_t i = 0; i < vertex_len; i++)
        printf("%2ld: %2ld, %2ld\n", i, vertex_adj[i][0], vertex_adj[i][1]);
}


ssize_t
get_vertex_id_by_pos(vec2 pos)
{
    for (size_t i = 0; i<vertex_len; i++)
        if (vertex_pos[i].x == pos.x && vertex_pos[i].y == pos.y)
            return i;
    return -1;
}


ssize_t
dfs(ssize_t id, ssize_t goal)
{
    if (id == goal) {
        //puts("Found goal, ret 1.");
        return 1;
    }
    
    // If I already know how many paths there are from this vertex to the end,
    // then I don't need to explore this vertex again.
    if (paths_to_goal[id] > 0) {
        // printf("already explored %ld, %ld paths to goal\n", id, paths_to_goal[id]);
        return paths_to_goal[id];
    }//  else {
    //     // remove sentinel.
    //     paths_to_goal[id] == 0;
    // }

    //printf("id %ld\n", id);
    ssize_t left = vertex_adj[id][0];
    ssize_t right = vertex_adj[id][1];
    // Left
    if (left >= 0) {
        //printf("exploring lchild of %ld, %ld\n", id, left);
        paths_to_goal[id] += dfs(left, goal);
    }
    
    // Right
    if (right >= 0) {
        //printf("exploring rchild of %ld, %ld\n", id, right);
        paths_to_goal[id] += dfs(right, goal);
    }
    
    // Visit
    //printf("visiting %ld, returning %ld\n", id, paths_to_goal[id]);
    return paths_to_goal[id];
    // Update the paths_to_goal with the sum of all child's paths to goal?

    // If my lchild is the goal, add one path to goal.
    // Otherwise, add lchild's paths to goal to my own.
    //
    // If my rchild is the goal, add one path to goal.
    // Otherwise, add rchild's paths to goal to my own.
    //printf("visit id %ld\n", id);
    // if (id == vertex_len-1) {
    //     // ssize_t_stack backstack;
    //     // ssize_t_stack_copy(&backstack, to_visit);
    //     // while (!ssize_t_stack_empty(&backstack)) {
    //     //     printf("%02ld", ssize_t_stack_pop(&backstack));
    //     //     if(!ssize_t_stack_empty(&backstack))
    //     //         printf(" -> ");
    //     // }
    //     // puts("");
        
    // }
    // ssize_t_stack_pop(to_visit);
    //return;
}
