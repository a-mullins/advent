#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int sum = 0;

    char delims[] = " \r\n\t";
    size_t len = 0;
    char *line = NULL;
    while(getline(&line, &len, stdin) > 0) {
        char *tok = NULL;
        int digits[32];
        int num_digits = 0;
        tok = strtok(line, delims);
        // for each token of line
        do {
            digits[num_digits] = atoi(tok);
            num_digits++;
            tok = strtok(NULL, delims);
        } while(tok != NULL && num_digits < 32);

        // note: we do not have to check for size in
        // sum += i / j because if i % j == 0, then i>=j.
        for(int i = 0; i<num_digits; i++) {
            for(int j = 0; j<num_digits; j++) {
                if(i == j) { continue; }
                if(digits[i] % digits[j] == 0) {
                    sum += digits[i] / digits[j];
                }
            }
        }
    }
    free(line);

    printf("%d\n", sum);
    return 0;
}
