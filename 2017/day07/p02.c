// TODO cleanup pass
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../darray.h"


#define LABEL_CAP 16


// -=[ TREE STUFF ]=-----------------------------------------------------------
typedef struct tree {
    char label[LABEL_CAP];
    // These two could be combined in our application,
    // but we will just leave them seperate.
    unsigned int vert_weight;
    unsigned int tree_weight;
    darray children;
} tree;


void
tree_init(tree *t)
{
    memset(t->label, 0, LABEL_CAP * sizeof (char));
    t->vert_weight = 0;
    t->tree_weight = 0;
    darray_init(&(t->children), sizeof (tree *));
}


void
tree_push_child(tree *parent, tree *child)
{
    // push a pointer to the child, not the child itself.
    darray_push(&parent->children, &child);
}


tree *
tree_get_child(tree *t, size_t i) {
    return *(tree**)darray_get(&t->children, i);
}


tree *
tree_dfs(tree *t, bool (*pred)(tree *t))
{
    // Visit all child nodes...
    for (size_t i = 0; i < t->children.len; i++) {
        if(tree_dfs(tree_get_child(t, i), pred) != NULL) {
            return tree_get_child(t, i);
        }
    }
    // ...then visit self.
    return (*pred)(t) == false ? NULL : t;
}


bool
print_tree(tree *t) {
  printf("%s\n", t->label);
  return false;
}


tree *
tree_bfs(tree *t, bool (*pred)(tree *t))
{
    // Visit self...
    if((*pred)(t)) {return t;}
    // ... then visit all child nodes.
    for (size_t i = 0; i < t->children.len; i++) {
        if(tree_bfs(tree_get_child(t, i), pred) != NULL) {
            return tree_get_child(t, i);
        }
    }
    return NULL;
}


// void
// tree_free(tree *t)
// {
//     TODO: dfs starting at *t, freeing nodes as we go.
//     darray_free(&t->children, NULL);
//     free(t);
// }
// -=[ END TREE STUFF ]=-------------------------------------------------------

int
labelcmp(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}


void
lines_destruct(void *a) {
    char *p = *(char**)a;
    free(p);
}


void
find_root_label(darray *lines, char *s) {
    const char *DELIMS = " \t\r\n()->,";

    // Create list of all nodes which are children.
    darray children;
    darray_init(&children, sizeof (char *));
    for (size_t i = 0; i < lines->len; i++) {
        size_t line_len = strlen(*(char **)darray_get(lines, i));
        char *line = calloc(line_len+1, sizeof (char));
        strcpy(line, *(char **)darray_get(lines, i));

        // First field is name, second is weight. Discard them.
        strtok(line, DELIMS);
        strtok(NULL, DELIMS);

        // Collect names of child nodes.
        // We don't need to worry about repeated strings because
        // a child can only have one parent;
        // ie, no child will show up in two different lines.
        char *tok = NULL;
        while ((tok = strtok(NULL, DELIMS)) != NULL) {
            char *buf = calloc(strlen(tok) + 1, sizeof (char));
            strcpy(buf, tok);
            darray_push(&children, &buf);
        }
        free(line);
    }

    darray_qsort(&children, labelcmp);

    // For every program name, check if it is a child.
    for(size_t i = 0; i < lines->len; i++) {
        size_t line_len = strlen(*(char **)darray_get(lines, i));
        char *line = calloc(line_len+1, sizeof (char));
        strcpy(line, *(char **)darray_get(lines, i));

        size_t label_len = strcspn(line, " ");
        line[label_len] = '\0';

        //strncpy(label, line, label_len);

        //void *result = darray_bsearch(&children, &label, labelcmp);
        void *result = darray_bsearch(&children, &line, labelcmp);
        // If the name is If it is not a child of any node, then it must be the root.
        if (result == NULL) {
            strcpy(s, line);
        }
        free(line);
    }

    darray_free(&children, lines_destruct);
    return;
}


int
main(void)
{
    darray lines = {0};

    darray_init(&lines, sizeof (char *));

    size_t len = 0;
    ssize_t nread = 0;
    char *line = NULL;
    while ((nread = getline(&line, &len, stdin)) >= 0) {
        char *buf = calloc((size_t)(nread + 1), sizeof (char));
        strcpy(buf, line);
        darray_push(&lines, &buf);
    }
    len = 0; nread = 0; free(line);

    darray_qsort(&lines, labelcmp);

    char *s = calloc(16, sizeof (char));
    find_root_label(&lines, s);
    printf("%s\n", s);
    free(s);

    // print all lines
    // for(size_t i = 0; i < lines.len; i++) {
    //     printf("%s", *(char **)darray_get(&lines, i));
    // }
    tree *t0 = malloc(sizeof (tree));
    tree *t1 = malloc(sizeof (tree));
    tree *t2 = malloc(sizeof (tree));
    tree_init(t0);
    strcpy((char*)&t0->label, "root");
    tree_init(t1);
    strcpy((char*)&t1->label, "child1");
    tree_init(t2);
    strcpy((char*)&t2->label, "child2");

    tree *t3 = malloc(sizeof (tree));
    tree_init(t3);
    strcpy((char*)&t3->label, "child3");
    tree *t4 = malloc(sizeof (tree));
    tree_init(t4);
    strcpy((char*)&t4->label, "child4");

    tree_push_child(t0, t1);
    tree_push_child(t0, t2);
    tree_push_child(t2, t3);
    tree_push_child(t2, t4);

    // p (*(tree **)darray_get(&t0->children, 0))->label

    // why is this not equiv to (tree*)?
    //tree *foo = *(tree**)darray_get(&t0->children, 0);
    puts("--------");
    tree_dfs(t0, print_tree);
    puts("--------");
    tree_bfs(t0, print_tree);


    darray_free(&lines, lines_destruct);
    return 0;
}
