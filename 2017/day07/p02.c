// TODO cleanup pass
#include <limits.h>
#include <stdbool.h>
#include <signal.h> // raise
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/darray.h"
#include "../lib/stack.h"


#define DELIMS   " \t\r\n()->,"
#define LINE_CAP 128
#define NAME_CAP 16


typedef struct tree {
    char name[16];
    unsigned int node_weight;
    unsigned int tree_weight;
    darray children;
} tree;


typedef tree * treeptr;


STACK(treeptr)


int nameinline(const void *a, const void *b);
int linecmp(const void *a, const void *b);
int namecmp(const void *a, const void *b);
void find_root(darray *lines, char *root_name);
void darray_char_p_destr(void *elem);
void tree_init(tree *t, const char *name);
size_t tree_len_children(tree *parent);
void tree_add_child(tree *parent, tree *child);
tree *tree_get_child(tree *parent, size_t i);
bool tree_post_order(tree *t, bool (*visit)(tree *));
bool tree_print_node(tree *t);
bool set_tree_weight(tree *t);
bool set_print_node(tree *t);
bool tree_setweight_checkkids(tree *t);

int
main(void)
{
    darray lines = {0};
    darray_init(&lines, LINE_CAP * sizeof (char));

    char *line = NULL;
    char  buf[LINE_CAP] = {'\0'};
    while (fgets(buf, LINE_CAP, stdin) != NULL) {
        size_t len = strlen(buf);
        line = calloc(len+1, sizeof (char));
        strcpy(line, buf);
        darray_push(&lines, &line);
    }

    // So we can bsearch() later.
    darray_qsort(&lines, linecmp);

    // Find root.
    char root_name[NAME_CAP] = {0};
    find_root(&lines, root_name);
    printf("root: %s\n", root_name);

    // Build tree.
    tree root = {0};
    tree_init(&root, root_name);

    treeptr_stack stack = {0};
    treeptr_stack_init(&stack);
    treeptr_stack_push(&stack, &root);

    while (!treeptr_stack_empty(&stack)) {
        tree *cur = treeptr_stack_pop(&stack);
        char line[LINE_CAP];
        char *res = *(char**)darray_bsearch(&lines, cur->name, nameinline);
        if(res == NULL) {fprintf(stderr, "ERR");exit(1);}
        strncpy(line, res, LINE_CAP-1);

        // Update current node's weight.
        strtok(line, DELIMS);
        cur->node_weight = (unsigned int)atoi(strtok(NULL, DELIMS));

        // Update current node's children.
        // 1. Find child names in line.
        // 2. Create node for each child.
        // 3. Add child to cur's children.
        // 4. Push child to stack so that it is visited.
        char *child_name;
        while ((child_name = strtok(NULL, DELIMS)) != NULL) {
            tree *child = malloc(sizeof (tree));
            tree_init(child, child_name);
            tree_add_child(cur, child);
            treeptr_stack_push(&stack, child);
        }
    }

    // Traverse tree.
    // treeptr_stack_push(&stack, &root);
    // while (!treeptr_stack_empty(&stack)) {
    //     tree *cur = treeptr_stack_pop(&stack);
    //     printf("%s (%d)\n", cur->name, cur->node_weight);

    //     for (size_t i = cur->children.len; i != 0; /*nop*/) {
    //         treeptr_stack_push(&stack, tree_get_child(cur, --i));
    //     }
    // }

    //tree_print_node(&root);
    tree_post_order(&root, tree_setweight_checkkids);

    return 0;
}


int
nameinline(const void *a, const void *b)
{
    char line[LINE_CAP] = {0};
    strncpy(line, *(char**)b, LINE_CAP);
    line[strcspn(line, " ")] = '\0';
    return strcmp((const char *)a, line);
}


int
linecmp(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}


int
namecmp(const void *a, const void *b)
{
    // a, b are pointers to char.
    return strcmp(a, b);
}


void
find_root(darray *lines, char *root_name)
{
    darray children;
    darray_init(&children, NAME_CAP * sizeof (char));

    // Gather names of all programs which are children.
    for (size_t i = 0; i<lines->len; i++) {
        char line[128] = {0};
        strncpy(line, *(char**)darray_get(lines, i), 128);

        char *tok;

        // First field is name, second is weight. Discard them.
        strtok(line, DELIMS);
        strtok(NULL, DELIMS);

        // Collect names of nodes that are children.
        // We don't need to worry about repeated strings because
        // a child can only have one parent;
        // ie, no child will show up in two different lines.
        while ((tok = strtok(NULL, DELIMS)) != NULL) {
            darray_push(&children, tok);
        }
    }

    // Sort so we can use bsearch().
    darray_qsort(&children, namecmp);

    // For every program name, check if it is a child.
    for (size_t i = 0; i<lines->len; i++) {
        char name[NAME_CAP] = {0};
        strncpy(name, *(char**)darray_get(lines, i), 16);
        strtok(name, DELIMS);

        char *result = (char *)darray_bsearch(&children, name, namecmp);
        // If the name doesn't appear, then it's not the child of any
        // node. So it must be the root.
        if (result == NULL) {
            snprintf(root_name, NAME_CAP, "%s", name);
        }
    }

    darray_free(&children, NULL);
    return;
}


// -=[ TREE STUFF ]=-----------------------------------------------------------
void
tree_init(tree *t, const char *name)
{
    strncpy(t->name, name, NAME_CAP);
    t->node_weight = 0;
    t->tree_weight = 0;
    darray_init(&t->children, sizeof (tree *));
}


size_t
tree_len_children(tree *parent)
{
    return parent->children.len;
}


void
tree_add_child(tree *parent, tree *child)
{
    darray_push(&parent->children, &child);
}


tree *
tree_get_child(tree *parent, size_t i)
{
    return *(tree**)darray_get(&parent->children, i);
}

bool
tree_post_order(tree *t, bool (*visit)(tree *))
{
    for (size_t i = 0; i < t->children.len; i++) {
        bool cont = tree_post_order(tree_get_child(t, i), visit);
        if(!cont) { return false; }
    }

    return (*visit)(t);
}

bool
tree_print_node(tree *t)
{
    printf(
        "node \"%s\" (node: %d, tree: %d)",
        t->name,
        t->node_weight,
        t->tree_weight);
    if(t->children.len > 0) {
        puts(":");
        for (size_t i = 0; i < t->children.len; i++) {
            tree *c = tree_get_child(t, i);
            printf("\t%s (node: %d, tree: %d)\n",
                   c->name,
                   c->node_weight,
                   c->tree_weight);
        }
    } else {
        puts("");
    }
    fflush(stdin);
    return true;
}

bool
tree_set_weight(tree *t)
{
    // if(!strcmp(t->name, "hblcbb")) {
    //     raise(SIGTRAP);
    // }
    t->tree_weight = t->node_weight;
    for (size_t i = 0; i < t->children.len; i++)
        t->tree_weight += tree_get_child(t, i)->tree_weight;

    return true;
}

// void tree_set_print(tree *t)
// {
//     tree_set_weight(t);
//     tree_print_node(t);
// }

bool
tree_setweight_checkkids(tree *t)
{
    tree_set_weight(t);
    //tree_print_node(t);
    if(t->children.len > 0) {
        int max = INT_MIN;
        int min = INT_MAX;
        for (size_t i = 0; i < t->children.len; i++) {
            int cur = tree_get_child(t, i)->tree_weight;
            max = cur>max ? cur : max;
            min = cur<min ? cur : min;
        }
        if (max != min) {
            puts(  "!! inbalance in branches detected!");
            size_t max_i = 0, min_i = 0, inbal_i = 0;
            int max_c = 0, min_c = 0;
            for (size_t i = 0; i < t->children.len; i++) {
                if (tree_get_child(t, i)->tree_weight == min) {
                    min_i = i;
                    min_c++;
                }
                if (tree_get_child(t, i)->tree_weight == max) {
                    max_i = i;
                    max_c++;
                }
            }
            if (max_c > min_c) {
                // one branch weighs less than the others
                tree *unbal = tree_get_child(t, min_i);
                printf("%s should weigh %d",
                       unbal->name,
                       unbal->node_weight + (max - min));
            } else {
                // one branch weighs more than the others
                tree *unbal = tree_get_child(t, max_i);
                printf("%s should weigh %d\n",
                       unbal->name,
                       unbal->node_weight - (max - min));
            }
            // printf("!! %s should weigh %d",
            //        tree_get_child(t, i)
            //        tree_get_child(t, i)->node_weight + (max - min)
            //     );
            // printf("!! %d needed to correct\n", max - min);
            return false;
        }
    }
    return true;
}
// -=[ END TREE STUFF ]=-------------------------------------------------------
