// n-ary tree
#include "darray.h"


typedef struct tree {
    void *data;
    darray children;
} tree;


void
tree_init(tree *t)
{
    data = NULL;
    darray_init(t->children, sizeof (tree *));
}


void
tree_set(tree *t, void *data, size_t size)
{
    if (data != NULL) {
        free(data);
        data = NULL;
    }
    t->data = malloc(size);
    memcpy(t->data, data, size);
}


void
tree_free(tree *t)
{
    if (t->data != NULL) {
        free(t->data);
    }
    // TODO traverse tree from here down and free each child
    darray_free(t->children, NULL);
}


void
tree_add_child(tree *t, tree *child)
{
    darray_push(&t->children, &child);
}


void
tree_get_child(tree *t, size_t i)
{
    return *(tree **)darray_get(&t->children, i);
}
