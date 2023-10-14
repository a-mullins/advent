#include <string.h>  // strcmp(), strtok() 
#include <stddef.h>  // size_t, ssize_t
#include <stdio.h>   // fprintf(), printf(), getline(), etc
#include <stdlib.h>  // free(), exit(), realloc()

#define WORD_LIMIT 32

int main(void) {
    const static char *DELIMS = " \t\r\n";

    size_t line_buf_size = 0;
    char *line_buf = NULL;
    int valid_count = 0;
    while(getline(&line_buf, &line_buf_size, stdin) != -1){
        char *words[WORD_LIMIT] = { NULL };
        int i = 0;
        char *tok = strtok(line_buf, DELIMS);
        do {
            words[i++] = tok;
            tok = strtok(NULL, DELIMS);
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
