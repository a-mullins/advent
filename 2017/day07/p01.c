#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIMS " \t\r\n()->,"
#define FIELD_LEN 8
#define MAX_LINES 1606

bool includes(char list[][FIELD_LEN], char *s);

int main(void) {
    char labels[MAX_LINES][FIELD_LEN]; //= {'\0'};
    char children[MAX_LINES][FIELD_LEN]; //= {'\0'};
    int label_i = 0;
    int child_i = 0;
    size_t line_cap = 80;
    char *line = malloc(sizeof(char) * line_cap);
    while(getline(&line, &line_cap, stdin) != -1) {
        char *tok = strtok(line, DELIMS);
        strncpy(labels[label_i], tok, FIELD_LEN);
        // skip weight
        strtok(NULL, DELIMS);
        while((tok = strtok(NULL, DELIMS)) != NULL) {
            strncpy(children[child_i++], tok, FIELD_LEN);
        }
        label_i++;
    }
    free(line);

    for(int i=0; i<MAX_LINES && labels[i] != NULL; i++) {
        if(!includes(children, labels[i])) {
            printf("%s\n", labels[i]);
            break;
        }
    }

    return 0;
}

bool includes(char list[][FIELD_LEN], char *s) {
    if(s == NULL) { return true; }
    for(int i=0; i < MAX_LINES && list[i] != NULL; i++) {
        if(strcmp(list[i], s) == 0) {
            return true;
        }
    }
    return false;
}
