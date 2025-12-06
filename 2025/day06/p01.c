#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int
main(void)
{
    char line[4096] = {'\0'};
    unsigned long ns[4][1024] = {0};
    unsigned long total = 0;
    for (size_t row = 0; NULL != fgets(line, 4096, stdin); row++) {
        // char word[16];
        // strncpy(word, line, 16);
        size_t col = 0;
        char *tok = strtok(line, " \n");
        for (/*nop*/; tok != NULL; tok = strtok(NULL, " \n"), col++) {
            unsigned long sub_total;
            if (strchr(tok, '+')) {
                sub_total = 0;
                for (size_t i = 0; i<row; i++)
                    sub_total += ns[i][col];
                total += sub_total;
            } else if  (strchr(tok, '*')) {
                sub_total = 1;
                for (size_t i = 0; i<row; i++)
                    sub_total *= ns[i][col];
                total += sub_total;
            } else {
                ns[row][col] = atol(tok);
            }
        }
    }
    printf("%ld\n", total);
    return 0;
}
