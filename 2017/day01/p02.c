#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int solve_captcha(int *digits, int len);

int main(void) {
    size_t len = 0;
    char *line = NULL;
    getline(&line, &len, stdin); // implicit malloc()
    line[strcspn(line, "\r\n")] = '\0'; // trim

    size_t digits_size = strlen(line);
    int *digits = calloc(digits_size, sizeof (int));
    for(size_t i=0; i<digits_size; i++) {
        digits[i] = (int)line[i] - 0x30;
    }

    printf("%d\n", solve_captcha(digits, (int)digits_size));

    free(digits);
    free(line);
    return 0;
}

int solve_captcha(int *digits, int len) {
    int cur = 0;
    int next = 0;
    int acc = 0;
    for(int i = 0; i<len; i++) {
        cur = digits[i];
        next = digits[(i+len/2) % len];
        if(cur == next) {
            acc += cur;
        }
    }
    return acc;
}
