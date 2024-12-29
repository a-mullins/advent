// TODO cleanup pass
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int
namecmp(const void *a, const void *b)
{
    // `a` and `b` will be a pointer-to-pointer-to-char, aka
    // a pointer to a string.
    const char *p = *(const char **)a;
    const char *q = *(const char **)b;
    return strcmp(p, q);
}


int
main(void)
{
    const char *DELIMS = " \t\r\n()->,";
    const size_t MAX_LINES = 2048;

    char **names = calloc(MAX_LINES, sizeof(char *));
    memset(names, 0, MAX_LINES * sizeof(char *));
    int names_len = 0;

    char **children = calloc(MAX_LINES, sizeof(char *));
    memset(children, 0, MAX_LINES * sizeof(char *));
    int children_len = 0;

    size_t len = 80;
    char *line = calloc(len, sizeof(char));
    while (getline(&line, &len, stdin) > 0) {
        char *tok = strtok(line, DELIMS);
        // program name
        names[names_len] = calloc(strlen(tok) + 1, sizeof(char));
        strcpy(names[names_len], tok);
        // discard next field, which is weight
        strtok(NULL, DELIMS);

        // collect names of children.
        // We don't need to worry about repeated strings, because
        // a child can only have one parent.
        // ie, no child will show up in two different lines.
        while ((tok = strtok(NULL, DELIMS)) != NULL) {
            children[children_len] = calloc(strlen(tok) + 1, sizeof(char));
            strcpy(children[children_len], tok);
            children_len++;
        }
        names_len++;
    }
    free(line); line = NULL; len = 0;

    // For every program, check if it is a child.
    // If it is not a child of any node, then it must be the root.
    qsort(children, children_len, sizeof(*children), namecmp);
    for (size_t i = 0; i<MAX_LINES && names[i] != NULL; i++) {
        char **result = bsearch(
            &names[i], children, children_len,
            sizeof(*children), namecmp);
        if (result == NULL) {
            printf("%s\n", names[i]);
        }
    }

    for (int i = 0; children[i] != NULL; i++) {free(children[i]);}
    free(children);
    for (int i = 0; names[i] != NULL; i++) {free(names[i]);}
    free(names);
    return 0;
}
