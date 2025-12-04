#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// To determine the number of bits needed to represent the largest
// value in the input:
// bc -l <<< "l($(tr ",-" "[\n*]" <input | sort -n | tail -1))/l(2);"
// Which is >33 in my input so a 64 bit integer is necessary. On my platform,
// that works out to a long. We could test against the exact value of the
// largest input of course, but that is not as general and more importantly,
// it leaks information about our input, which Watsl has asked participants to
// avoid.
static_assert(ULONG_MAX > ((unsigned long)1 << 33),
              "ERR: Unsigned long is less than 33 bits on this platform.");


bool is_valid_id(unsigned long);


int
main(void)
{
    enum reader_state { READ_START, READ_END } state = READ_START;

    unsigned long start;
    unsigned long end;
    unsigned long invalid_id_sum = 0;
    char s[16] = {'\0'};
    int n;
    while (EOF != (n = fgetc(stdin))) {
        char c = (char)n;
        if (state == READ_START && c == '-') {
            start = (unsigned long)atol(s);
            s[0] = '\0';
            state = READ_END;
        } else if (state == READ_END && c == ',') {
            end = (unsigned long)atol(s);

            for (unsigned long id = start; id <= end; id++) {
                if (!is_valid_id(id)) {
                    invalid_id_sum += (unsigned long)id;
                    printf("invalid id %ld\n", id);
                }
            }

            start = 0;
            end = 0;
            s[0] = '\0';
            state = READ_START;
        } else {
            strncat(s, &c, 1);
        }
    }

    printf("sum %ld\n", invalid_id_sum);
    return 0;
}


bool
is_valid_id(unsigned long id)
{
    char s[16] = {'\0'};
    //char *s_p = &s[0];
    sprintf(s, "%ld", id);
    size_t len = strlen(s);
    // single numbers are always valid, odd lengths can't have sequences
    // repeated exactly twice.
    if (len % 2 == 1) return true;

    if (!strncmp(s, s + ((len+1)/2), (len+1)/2))
        return false;
    
    return true;
}
