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

// When processing the list of programs, a program might make a
// forward-reference by listing a child that hasn't yet appeared.
//
// It's possible to build up the tree in a single pass by following
// these forward references, creating the child nodes, and then
// ensuring we don't add those same nodes a second time when they are
// encountered again later in the file.
//
// However, the easier solution is to process the list twice, once to
// collect the names and weights of the programs, and a second time to
// record the children.
//
// Although it's not space-effecient for sparse trees, we will use an
// adjacency matrix. It's easy to implement, we can find the root node
// pretty simply by finding a blank col, and... I've never actually
// used one before that I can recall, so why not? It seems to me a
// very "C" solution, in that we'll be tracking our data in multiple
// plain arrays, instead of building up complicated datastructs and
// doing link-chasing.
//
// 1. Parse input.
//    - build a map of names to indicies: name[name_i].
//    - build a map of weights to indicies: weight[name_i].
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

    // First pass.
    while(getline(&line, &len, stdin) > 0) {
        // name.
        strncpy(name[names_len++], strtok(line, DELIMS), FIELD_WIDTH-1);
    }

    if(fseek(stdin, 0L, SEEK_SET) != 0) {
        fprintf(stderr,
                "Seek failed. Please connect stdin to a regular file.\n");
        exit(1);
    }

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

    // debug
    for(int i = 0; i<names_len; i++) {
        printf("%4d: %8s (%5d)\n", i, name[i], weight[i]);
    }

    // debug
    char *needle = "kcimi";
    printf("%s is at idx %lu\n", needle, name2idx(needle));

    // In the adjacency-matrix representation of a tree, if a col is
    // all false, then that col corresponds to a node that has no
    // in-links, ie, no parents. Such a node must be the root of the
    // tree.
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
    printf("root is at idx %d: %s\n", root_idx, name[root_idx]);

    // find unbalanced node.
    //printf("tree weight is %d\n", tree_weight(name2idx("pabgfh")));
    printf("tree weight is %d\n", tree_weight(root_idx));

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

// Since there is only one mismatched node, there will always be just
// one branch to follow, unlike DFS/BFS/A*, etc. An iterative approach
// will work just as well/be just as easy to reason about as the
// recursive one. Also, this problem is unlikely to benefit from
// parrallelization.
//
// 1. Are all of the weights of this node's children the same?
//    - If yes, then this node is the one whose weight needs to be
//      adjusted. Calculate the difference between its tree weight and
//      the target weight, then return the value.
//    - If no, examine branches of node of this node. Explore
//      whichever node is mismatched.
//
int tree_weight(int root) {
    static int target = -1; // static vars are init'd only once, ever.

    int len = 0;
    int child[MAX_CHILDREN];
    for(int to = 0; to < MAX_LINES; to++) {
        if(tree[root][to]) {
            child[len++] = to;
        }
    }

    // 1. if this node is balanced, stop.
    // 2. if this node is unbalanced, ...

    if(len > 0) {
        int cur = tree_weight(child[0]);
        for(int i=1; i<len; i++) {
            if(cur != tree_weight(child[i])) {
                printf("program %s's (id#%d) disc is unbalanced.\n",
                       name[root],
                       root);
            }
        }
    }

    // if(len > 0) {
    //     printf("node %4d (%s) has children:\n", root, name[root]);
    //     for(int i=0; i<len; i++) {
    //         printf("\t%4d (%s)\n", child[i], name[child[i]]);
    //     }
    // } else {
    //     printf("node %4d (%s) is a leaf\n", root, name[root]);
    // }

    // printf("tree_weight(%d) -> %d\n", root, sum);

    return sum;
}
