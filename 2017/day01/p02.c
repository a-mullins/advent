#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int solve_captcha(const int digits[], int n);
void trim(char *s);

int main(void) {
    size_t buf_size = 0;
    char *buf = NULL;
    getline(&buf, &buf_size, stdin); // implicit malloc()
    trim(buf);

    const size_t digits_size = strlen(buf);
    int digits[digits_size];
    for(int i=0; i<digits_size; i++) {
        digits[i] = (int)buf[i] - 0x30;
    }

    printf("%d\n", solve_captcha(digits, digits_size));
    free(buf);
    return EXIT_SUCCESS;
}

int solve_captcha(const int digits[], int n) {
    int cur = 0;
    int next = 0;
    int acc = 0;
    for(int i = 0; i<n; i++) {
        cur = digits[i];
        next = digits[(i + n/2) % n];
        if(cur == next) {
            acc += cur;
        }
    }
    return acc;
}

void trim(char *s) {
    for(int i = strlen(s) - 1; i>=0; i--) {
        if(s[i] == '\n' || s[i] == '\r') {
            s[i] = '\0';
        }
    }
}
