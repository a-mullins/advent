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
typedef struct {
    unsigned long start;
    unsigned long end;
} range;


int
rangecmp(const void *a, const void*b);


int
main(void)
{
    enum reader_state {
        READ_START,
        READ_END
    };
    enum reader_state state = READ_START;

    range r[64];
    size_t r_len = 0;
    int n = EOF;
    char s[64] = {0};
    while (EOF != (n = fgetc(stdin))) {
        char c = (char)n;
        if (state == READ_START && c == '-') {
            r[r_len].start = (unsigned long)atol(s);
            s[0] = '\0';
            state = READ_END;
        } else if (state == READ_END && c == ',') {
            r[r_len].end = (unsigned long)atol(s);
            s[0] = '\0';
            state = READ_START;

            assert(r[r_len].start <= r[r_len].end);
            r_len++;
        } else {
            strncat(s, &c, 1);
        }
    }

    qsort(r, r_len, sizeof (range), rangecmp);
    unsigned long sum_of_deltas = 0;
    for(size_t i = 0; i < r_len; i++) {
        unsigned long delta = r[i].end - r[i].start;
        printf("range:%11ld -> %-11lddelta: %ld\n",
               r[i].start, r[i].end,
               delta);
        sum_of_deltas += delta;
    }
    printf("%38s\n", ":");
    printf("%38s %ld\n", "total vals to check:", sum_of_deltas);

    return 0;
}


int
rangecmp(const void *a, const void*b)
{
    range *r0 = (range *)a;
    range *r1 = (range *)b;

    if (r0->start < r1->start)
        return -1;
    if (r0->start == r1->start)
        return 0;
    return 1;
}
