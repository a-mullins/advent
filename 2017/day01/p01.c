#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int solve_captcha(short *digits, size_t len);

int main(void) {
    size_t buf_size = 0;
    char *buf = NULL;
    getline(&buf, &buf_size, stdin); // implicit malloc()
    buf[strcspn(buf, "\r\n")] = '\0'; // trim

    size_t digits_size = strlen(buf);
    short *digits = calloc(digits_size, sizeof (short));
    for(int i=0; i<digits_size; i++) { digits[i] = (short)buf[i] - 0x30; }

    printf("%d\n", solve_captcha(digits, digits_size));

    free(buf);
    free(digits);
    return EXIT_SUCCESS;
}

int solve_captcha(short *digits, size_t len) {
    short cur = 0;
    short next = 0;
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
