#include <stdio.h>
#include <string.h>


#define LEN 3072
static char line[LEN] = {0};
static short digits[LEN] = {0};


int
solve_captcha(short *digits, size_t len);


int
main(void)
{
    fgets(line, LEN, stdin);
    line[strcspn(line, "\r\n")] = '\0'; // trim

    size_t digits_size = strlen(line);
    for (size_t i=0; i<digits_size; i++) {
        digits[i] = (short)line[i] - 0x30;
    }

    printf("%d\n", solve_captcha(digits, digits_size));

    return 0;
}


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
