#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int
solve_captcha(short *digits, size_t len)
{
    short cur = 0;
    short next = 0;
    int acc = 0;
    for (size_t i = 0; i < len; i++) {
        cur = digits[i];
        next = digits[(i+len/2) % len];
        if (cur == next) {
            acc += (int)cur;
        }
    }
    return acc;
}


int
main(void)
{
    size_t len = 0;
    char *line = NULL;
    getline(&line, &len, stdin); // implicit malloc()
    line[strcspn(line, "\r\n")] = '\0'; // trim

    size_t digits_size = strlen(line);
    short *digits = calloc(digits_size, sizeof (int));
    for (size_t i=0; i<digits_size; i++) {
        digits[i] = (int)line[i] - 0x30;
    }

    printf("%d\n", solve_captcha(digits, digits_size));

    free(digits);
    free(line);
    return 0;
}
