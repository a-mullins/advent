#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../darray.h"


#define NAME_CAP     16


int linecmp(const void *a, const void *b);
int namecmp(const void *a, const void *b);
void find_root(darray *lines, char *root_name);
void darray_char_p_destr(void *elem);


int
main(void)
{
    darray lines;
    darray_init(&lines, sizeof (char *));

    char *line = NULL;
    char  buf[128] = {'\0'};
    while (fgets(buf, 80, stdin) != NULL) {
        size_t len = strlen(buf);
        line = calloc(len+1, sizeof (char));
        strcpy(line, buf);
        darray_push(&lines, &line);
    }

    //darray_qsort(&lines, linecmp);

    char root_name[16] = {0};
    find_root(&lines, root_name);

    // for(size_t i = 0; i<lines.len; i++) {
    //     printf("%s", *(char **)darray_get(&lines, i));
    // }

    darray_free(&lines, darray_char_p_destr);
    return 0;
}


int
linecmp(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}


int
namecmp(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char*)b);
}


void
darray_char_p_destr(void *elem)
{
    char *p = *(char **)elem;
    free(p);
    p = NULL;
}


void
find_root(darray *lines, char *root_name)
{
    static const char *DELIMS = " \t\r\n()->,";
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
