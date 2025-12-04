#include <assert.h>
#include <limits.h>
#include <math.h>
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
    while (EOF != scanf("%ld-%ld,", &start, &end)) {
        for (unsigned long id = start; id <= end; id++) {
            int digits = (int)floorf(log10f((float)id)) + 1;
            if (digits % 2 == 1) {
                // Skip to the next possibly valid id.
                id = (unsigned long)(pow(10, digits));
                continue;
            }
            unsigned long top = id / (unsigned long)pow(10, (digits/2));
            unsigned long bottom = id % (unsigned long)pow(10, (digits/2));
            if (bottom == top) invalid_id_sum += id;
        }
    }

    printf("%ld\n", invalid_id_sum);
    return 0;
}
