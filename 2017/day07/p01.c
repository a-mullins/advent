#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../darray.h"


int
namecmp(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char*)b);
}


int
main(void)
{
    static const char *DELIMS = " \t\r\n()->,";
    static const size_t NAME_CAP = 16;
    if(fseek(stdin, 0, SEEK_SET) != 0) {
        fprintf(stderr, "ERR\n"); exit(1);
    }

    darray children;
    darray_init(&children, NAME_CAP * sizeof (char));

    // Gather names of all programs which are children.
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, stdin) > 0) {
        char *tok;

        // First field is name, second is weight. Discard them.
        strtok(line, DELIMS);
        strtok(NULL, DELIMS);

        // Now collect names of children.
        // We don't need to worry about repeated strings because
        // a child can only have one parent;
        // ie, no child will show up in two different lines.
        while ((tok = strtok(NULL, DELIMS)) != NULL) {
            darray_push(&children, tok);
        }
    }
    free(line); line = NULL; len = 0;

    // Sort so we can use bsearch() later.
    darray_qsort(&children, namecmp);

    // For every program name, check if it is a child.
    rewind(stdin);
    while (getline(&line, &len, stdin) > 0) {
        char *name = strtok(line, DELIMS);
        char *result = (char *)darray_bsearch(&children, name, namecmp);
        // If the name is If it is not a child of any node, then it must be the root.
        if (result == NULL) {
            printf("%s\n", name);
        }

    }
    free(line); line = NULL; len = 0;

    darray_free(&children, NULL);
    return 0;
}
