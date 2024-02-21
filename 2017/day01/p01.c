#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int solve_captcha(int *digits, size_t len);

int main(void) {
    size_t len = 0;
    char *line = NULL;
    getline(&line, &len, stdin); // implicit malloc()
    line[strcspn(line, "\r\n")] = '\0'; // trim

    // int *digits could be declared on the stack with a VLA alloca()
    // but that will make for a large stack frame.
    size_t digits_size = strlen(line);
    int *digits = calloc(digits_size, sizeof (int));
    for(size_t i=0; i<digits_size; i++) {
        digits[i] = (int)line[i] - 0x30;
    }

    printf("%d\n", solve_captcha(digits, digits_size));

    free(line);
    free(digits);
    return 0;
}

int solve_captcha(int *digits, size_t len) {
    int cur = 0;
    int next = 0;
    int acc = 0;
    for(size_t i = 0; i<len; i++) {
        cur = digits[i];
        next = (i+1 == len ? digits[0] : digits[i+1]);
        if(cur == next) {
            acc += cur;
        }
    }
    return acc;
}
