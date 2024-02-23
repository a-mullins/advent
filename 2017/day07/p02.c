#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIMS " \t\r\n()->,"
#define MAX_LINES 2048

typedef struct prog {
    char *name;
    char **children_names;
    struct prog **children;
    int weight;
} prog;

int namecmp(const void *a, const void *b);
int prognamecmp(const void *a, const void *b);

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
//    a. build a map of names to indicies: name[name_i].
//    b. build a map of weights to indicies: weight[name_i].
//
// 2. Parse input again.
//    c. mark child nodes in the adjacency matrix:
//       tree[from][to] = true;
//
// 3. Find root node: scan by col, empty col means no parents.
//    That col corresponds to the root node of the tree.
int main(void) {
    prog *progs = calloc(MAX_LINES, sizeof(prog));
    memset(progs, 0, MAX_LINES * sizeof(prog));
    char **names = calloc(MAX_LINES, sizeof(char *));
    memset(names, 0, MAX_LINES * sizeof(char *));
    int progs_len = 0;
    int names_len = 0;

    // progs[0].name = "foo";
    // progs[1].name = "bar";
    // progs[0].children = calloc(2, sizeof(prog *));
    // memset(progs[0].children, 0, 2 * sizeof(prog *));
    // progs[0].children[0] = &progs[1];

    char **children = calloc(MAX_LINES, sizeof(char *));
    memset(children, 0, MAX_LINES * sizeof(char *));
    int children_len = 0;

    size_t len = 80;
    char *line = calloc(len, sizeof(char));
    while(getline(&line, &len, stdin) > 0) {
        char *tok = strtok(line, DELIMS);
        // program name
        progs[progs_len].name = calloc(strlen(tok) + 1, sizeof(char));
        strcpy(progs[progs_len].name, tok);

        names[names_len] = calloc(strlen(tok) + 1, sizeof(char));
        strcpy(names[names_len], tok);
        // discard first field, which is weight
        progs[progs_len].weight = atoi(strtok(NULL, DELIMS));

        // collect names of children.
        // We don't need to worry about repeated strings, because
        // a child can only have one parent.
        // ie, no child will show up in two different lines.
        while((tok = strtok(NULL, DELIMS)) != NULL) {
            children[children_len] = calloc(strlen(tok) + 1, sizeof(char));
            strcpy(children[children_len], tok);
            children_len++;
        }
        //names_len++;
        progs_len++;
    }
    free(line); line = NULL; len = 0;

    //printf("%d\n", prognamecmp(progs, progs + 1));

    // For every program, check if it is a child.
    // If it is not a child of any node, then it must be the root.
    qsort(progs, progs_len, sizeof(prog), prognamecmp);
    // qsort(children, children_len, sizeof(*children), namecmp);
    // for(int i = 0; i<MAX_LINES && names[i] != NULL; i++) {
    //     char **result = bsearch(
    //         &names[i], children, children_len,
    //         sizeof(*children), namecmp);
    //     if(result == NULL) {
    //         printf("%s\n", names[i]);
    //     }
    // }
    for(int i = 0; i<progs_len; i++) {
        printf("%s\n", progs[i].name);
    }

    // for(int i = 0; children[i] != NULL; i++) {free(children[i]);}
    // free(children);
    // for(int i = 0; names[i] != NULL; i++) {free(names[i]);}
    // free(names);
    // free(progs);
    return 0;
}

int namecmp(const void *a, const void *b) {
    // a will be a pointer-to-pointer-to-char
    const char *p = *(const char **)a;
    const char *q = *(const char **)b;
    return strcmp(p, q);
}

int prognamecmp(const void *a, const void *b) {
    // a, b will be pointers to struct prog
    const prog *p = (const prog *)a;
    const prog *q = (const prog *)b;
    return strcmp(p->name, q->name);
}
