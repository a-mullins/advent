// TODO cleanup pass
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DELIMS " \t\r\n()->,"
#define MAX_LINES 2048
#define FIELD_WIDTH 16
#define MAX_CHILDREN 8


typedef struct prog {
    char *name;
    char **children_names;
    struct prog **children;
    int weight;
} prog;


int namecmp(const void *a, const void *b);
ssize_t name2idx(char *needle);
int tree_weight(int i);
void find_unbal(int root);


// When processing the list of programs, a program might make a
// forward-reference by listing a child that hasn't yet appeared.
//
// It's possible to build up the tree in a single pass by following these
// forward references, creating the child nodes, and then ensuring we don't
// add those same nodes a second time when they are encountered again later in
// the file.
//
// However, the easier solution is to process the list twice, once to collect
// the names and weights of the programs, and a second time to record the
// children.
//
// Although it's not space-effecient for sparse trees, we will use an
// adjacency matrix. It's easy to implement, we can find the root node easily
// by finding a blank col, and I've never actually implemented one before that
// I can recall, so why not? It also seems like a very "C" solution, in that
// we'll be tracking our data in multiple plain arrays instead of building up
// complicated datastructs and doing link-chasing.
//
// 1. Parse input.
//    - build a map of indicies to names: name[name_i].
//    - build a map of indicies to weights: weight[name_i].
//
// 2. Parse input again.
//    - mark child nodes in the adjacency matrix:
//      tree[from][to] = true;
//
// 3. Find root node.
//    Scan by col, empty col means no parents.
//    That col corresponds to the root node of the tree.
static char name[MAX_LINES][FIELD_WIDTH];
static int weight[MAX_LINES];
static int names_len = 0;
static bool tree[MAX_LINES][MAX_LINES] = {false};


int main(void) {
    size_t len = 80;
    char *line = calloc(len, sizeof(char));

    // First pass, collect names.
    while(getline(&line, &len, stdin) > 0) {
        // name.
        strncpy(name[names_len++], strtok(line, DELIMS), FIELD_WIDTH-1);
    }

    if(fseek(stdin, 0L, SEEK_SET) != 0) {
        fprintf(stderr,
                "Seek failed. Please connect stdin to a regular file.\n");
        exit(1);
    }

    // Sort names so we can use bsort on it later.
    qsort(&name, names_len, FIELD_WIDTH, namecmp);

    // Second pass.
    while(getline(&line, &len, stdin) > 0) {
        char *tok = NULL;
        ssize_t parent_i = -1;

        // program name
        tok = strtok(line, DELIMS);
        parent_i = name2idx(tok);

        // weight
        tok = strtok(NULL, DELIMS);
        weight[parent_i] = atoi(tok);

        // children
        ssize_t child_i = -1;
        while((tok = strtok(NULL, DELIMS)) != NULL) {
            child_i = name2idx(tok);
            tree[parent_i][child_i] = true;
        }
    }
    free(line); line = NULL; len = 0;

    // In the adjacency-matrix representation of a tree, if a col is
    // all false, then that col corresponds to a node that has no
    // in-links, ie, no parents. Such a node must be the root of the
    // tree.
    //
    // Another way of finding root would be to see which names appear
    // in only one line of text. All of the other should appear twice,
    // once as their own entry, and again as the child of another.
    //
    // I wonder which is faster?
    // Of course, all of this is only possible with the pre-knowledge that
    // our input actually represents a valid tree.
    int root_idx = -1;
    for(int col = 0; col<MAX_LINES; col++) {
        bool rootp = true;
        for(int row = 0; row<MAX_LINES; row++) {
            if(tree[row][col] == true) {
                rootp = false;
                break;
            }
        }
        if(rootp) {
            root_idx = col;
            break;
        }
    }
    printf("root is at idx %d \"%s\"\n", root_idx, name[root_idx]);
    printf("total tree weight is %d\n", tree_weight(root_idx));

    find_unbal(root_idx);

    return 0;
}


int namecmp(const void *a, const void *b) {
    // a, b are pointers to char.
    return strcmp(a, b);
}


ssize_t name2idx(char *needle) {
    void *offset = bsearch(needle, name, names_len, FIELD_WIDTH, namecmp);
    if(offset == NULL) {
        return (ssize_t)-1;
    }

    return (ssize_t)((offset - (void *)name) / FIELD_WIDTH);
}


// return the total weight of the sub-tree with root `root`
int tree_weight(int root) {
    static int total_weight[MAX_LINES];

    if(total_weight[root] > 0) {
        return total_weight[root];
    }

    int child_len = 0;
    int child[MAX_CHILDREN];
    for(int to = 0; to < MAX_LINES; to++) {
        if(tree[root][to]) {
            child[child_len++] = to;
        }
    }

    total_weight[root] = weight[root];
    for(int i = 0; i < child_len; i++) {
        total_weight[root] += tree_weight(child[i]);
    }

    return total_weight[root];
}


// Since there is only one mismatched node, there will always be just
// one branch to follow so a simple iterative and greedy approach will
// work just as well as a complicated one like (D|B)FS/A*, etc. Also,
// this problem won't benefit from multiprocessing.
//
// 1. Calculate the target weight. The target weight is the weight
//    that the majority of this node's children share. Store.
// 2. Find the mismatched node. Label this one `target`.
// 3. Examine target's children.
//    a. If target's children are matched, then target is the node
//       that needs adjusting. Return the difference between its
//       weight and the majority.
//    b. Else, visit target and repeat from step 1.
void find_unbal(int root) {
    int target_w = -1;   // target weight
    int target_i = root;

    // find children of target.
    int len = 0;
    int child[MAX_CHILDREN];
    for(int to = 0; to < MAX_LINES; to++) {
        if(tree[target_i][to]) {
            child[len++] = to;
        }
    }

    if(len > 0) {
        printf("  node %d \"%s\" has children:\n", target_i, name[target_i]);
    } else {
        printf("  node %d \"%s\" is a leaf.\n", target_i, name[target_i]);
    }

    // find the minimum and maximum weights of the child sub-trees.
    int max = INT_MIN;
    int min = INT_MAX;
    for(int i = 0; i<len; i++) {
        printf("    %4d \"%7s\" (%5d)\n", child[i], name[child[i]],
               tree_weight(child[i]));

        if(tree_weight(child[i]) < min) {
            min = tree_weight(child[i]);
        }
        if(tree_weight(child[i]) > max) {
            max = tree_weight(child[i]);
        }
    }

    if(len == 0 || max == min) {
        fprintf(stderr, "!!! Something has gone wrong, aborting!\n");
        exit(1);
    }

    int min_c = 0;
    int max_c = 0;
    int min_i = -1;
    int max_i = -1;
    for(int i = 0; i<len; i++) {
        if(tree_weight(child[i]) == min) {
            min_i = child[i];
            min_c++;
        }
        if(tree_weight(child[i]) == max) {
            max_i = child[i];
            max_c++;
        }
    }
    int next_i;
    if(max_c < min_c) {
        next_i = max_i;
        target_w = min;
    }
    else {
        next_i = min_i;
        target_w = max;
    }

    //printf("    %d child sub-trees have min-weight %d\n", min_c, min);
    //printf("    %d child sub-trees have max-weight %d\n", max_c, max);
    //printf("    next sub-tree to check is %d\n", next_i);
    //printf("tree_weight(#%d) -> %d\n", target_i, sum);

    // examine child node's children:
    bool next_unbal_p = true;
    {
        int len = 0;
        int child[MAX_CHILDREN];
        for(int to = 0; to < MAX_LINES; to++) {
            if(tree[next_i][to]) {
                child[len++] = to;
            }
        }

        int max = INT_MIN;
        int min = INT_MAX;
        // printf("      node %d \"%s\" has children:\n", next_i, name[next_i]);
        for(int i = 0; i<len; i++) {
            // printf("        %4d \"%7s\" (%5d)\n", child[i], name[child[i]],
            //        tree_weight(child[i]));

            if(tree_weight(child[i]) < min) {
                min = tree_weight(child[i]);
            }
            if(tree_weight(child[i]) > max) {
                max = tree_weight(child[i]);
            }
        }
        if(max == min) {next_unbal_p = false;}
    }

    if(next_unbal_p) {
        // printf("visiting %d, children unbalanced\n", next_i);
        find_unbal(next_i);
    } else {
        // printf("target node's children are balanced, need to modify weight by "
        //        "%d units\n", target_w - tree_weight(next_i));
        printf("node %d \"%s\" weighs %d, but should weigh %d\n",
               next_i, name[next_i], weight[next_i],
               weight[next_i] + (target_w - tree_weight(next_i))
            );
    }
    return;
}
