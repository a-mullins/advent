#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int
main(void)
{
    // To determine the number of bits needed to represent the largest
    // value in the input:
    //   bc -l <<< "l($(tr ",-" "[\n*]" <input | sort -h | tail -1))/l(2);"
    // Which is >33 in my input so a 64 bit integer is necessary. On
    //  my platform, that works out to a long.
    assert(sizeof (long) * CHAR_BIT > 34);
    typedef struct {
        long start;
        long end;
    } range;

    enum reader_state {
        READ_START,
        READ_END
    };
    enum reader_state state = READ_START;

    range r = {0, 0};
    int n = EOF;
    char s[64] = {0};
    while (EOF != (n = fgetc(stdin))) {
        char c = (char)n;
        if(state == READ_START && c == '-') {
            r.start = atoll(s);
            s[0] = '\0';
            state = READ_END;
        } else if(state == READ_END && c == ',') {
            r.end = atoll(s);
            s[0] = '\0';
            state = READ_START;
            printf("range: %ld-%ld\n", r.start, r.end);
        } else {
            strncat(s, &c, 1);
        }
    }

    return 0;
}
