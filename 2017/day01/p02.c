#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int solve_captcha(int *digits, size_t len);

int main(void) {
    size_t buf_size = 0;
    char *buf = NULL;
    getline(&buf, &buf_size, stdin); // implicit malloc()
    buf[strcspn(buf, "\r\n")] = '\0'; // trim

    size_t digits_size = strlen(buf);
    int *digits = calloc(digits_size, sizeof (int));
    for(size_t i=0; i<digits_size; i++) {
        digits[i] = (int)buf[i] - 0x30;
    }

    printf("%d\n", solve_captcha(digits, digits_size));

    free(buf);
    free(digits);
    return 0;
}

int solve_captcha(int *digits, size_t len) {
    int cur = 0;
    int next = 0;
    int acc = 0;
    for(size_t i = 0; i<len; i++) {
        cur = digits[i];
        next = digits[(i+len/2) % len];
        if(cur == next) {
            acc += cur;
        }
    }
    return acc;
}
