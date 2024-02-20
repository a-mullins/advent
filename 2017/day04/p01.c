#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define WORD_LIMIT 32

int main(void) {
    const char *delims = " \t\r\n";

    size_t line_buf_size = 0;
    char *line_buf = NULL;
    int valid_count = 0;
    while(getline(&line_buf, &line_buf_size, stdin) != -1){
        char *words[WORD_LIMIT] = { NULL };
        int i = 0;
        char *tok = strtok(line_buf, delims);
        do {
            words[i++] = tok;
            tok = strtok(NULL, delims);
        } while(tok !=NULL && i<WORD_LIMIT);

        for(int i = 0; i<WORD_LIMIT; i++) {
            if(words[i] == NULL) { break; }
            for(int j = 0; j < i; j++) {
                if(!strcmp(words[i], words[j])) {
                    goto done;
                }
            }
        }
        valid_count++;

    done:
        free(line_buf);
        line_buf = NULL;
    }

    printf("%d\n", valid_count);
    return 0;
}
