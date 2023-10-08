#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int sum = 0;
    // for each line of file
    while(true) {
        size_t buf_size = 0;
        char *buf = NULL;
        if(getline(&buf, &buf_size, stdin) <= 0) { break; }

        char delims[] = " \r\n\t";
        char *tok = NULL; 
        int digits[512];
        int num_digits = 0;        
        tok = strtok(buf, delims);
        // for each token of line
        do { 
            digits[num_digits] = atoi(tok);
            num_digits++;
            tok = strtok(NULL, delims);
        } while(tok != NULL);

        for(int i = 0; i<num_digits; i++) {
            for(int j = 0; j<num_digits; j++) {
                if(i == j) { continue; }
                if(digits[i] % digits[j] == 0) {
                    sum += digits[i] / digits[j];
                }
            }
        }

        free(buf);
    }

    printf("%d\n", sum);
    return EXIT_SUCCESS;
}
