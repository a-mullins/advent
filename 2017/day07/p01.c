#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DELIMS " \t\r\n()->,"
#define CHILDREN_MAX 2048
#define NAME_MAX 8


static char children[CHILDREN_MAX][NAME_MAX] = {0};


int
namecmp(const void *a, const void *b);


int
main(void)
{
    // Gather names of all programs which are children.
    size_t children_len = 0;
    char line[128];
    while (fgets(line, 128, stdin)) {
        char *tok;

        // First field is name, second is weight. Discard them.
        strtok(line, DELIMS);
        strtok(NULL, DELIMS);

        // Now collect names of children.
        // We don't need to worry about repeated strings because
        // a child can only have one parent;
        // ie, no child will show up in two different lines.
        while ((tok = strtok(NULL, DELIMS)) != NULL) {
            strcpy(&children[children_len++][0], tok);
        }
    }

    // Sort so we can use bsearch() later.
    qsort(children, children_len, NAME_MAX * sizeof (char), namecmp);

    // For every program name, check if it is a child.
    rewind(stdin);
    while (fgets(line, 128, stdin)) {
        line[strcspn(line, " ")] = '\0';
        char *result = (char *)bsearch(line,
                                       children,
                                       children_len,
                                       NAME_MAX * sizeof (char),
                                       namecmp);
        // If it is not a child of any node, then it must be the root.
        if (result == NULL) {
            puts(line);
        }
    }

    // darray_free(&children, NULL);
    return 0;
}


int
namecmp(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char*)b);
}
