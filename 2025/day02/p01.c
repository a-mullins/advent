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


int
main(void)
{
    unsigned long invalid_id_sum = 0;
    unsigned long start;
    unsigned long end;
    char s[16];
    while (EOF != scanf("%ld-%ld,", &start, &end)) {
        for (unsigned long id = start; id <= end; id++) {
            sprintf(s, "%ld", id);
            size_t len = strlen(s);
            if (len % 2 == 1)
                continue;
            if (!strncmp(s, s + ((len+1)/2), (len+1)/2))
                invalid_id_sum += (unsigned long)id;
        }
    }

    printf("%ld\n", invalid_id_sum);
    return 0;
}
