#include <stdio.h>
#include <string.h>


int
main(void)
{
    unsigned long total_joltage = 0;
    char line[128];
    while (NULL != fgets(line, 128, stdin)) {
        line[strcspn(line, "\r\n")] = '\0';
        size_t len = strlen(line);
        char c;
        int n;
        int tens = -1;
        int tens_pos = -1;
        int ones = -1;
        for(ssize_t i = len-2; i >= 0; i--) {
            strncpy(&c, line + i, 1);
            n = c - 0x30;
            if (n >= tens) {
                tens = n;
                tens_pos = i;
            }
        }
        for(ssize_t i = len-1; i > tens_pos; i--) {
            strncpy(&c, line + i, 1);
            n = c - 0x30;
            if (n >= ones) {
                ones = n;
            }
        }
        total_joltage += tens * 10 + ones;
    }

    printf("%d\n", total_joltage);
    return 0;
}
