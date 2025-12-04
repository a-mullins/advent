#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>



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

bool is_invalid_id(long);


int
main(void)
{
    // TODO run matches against different ranges in parallel.
    int re_errno;
    PCRE2_SIZE re_erroffset;
    //PCRE2_SPTR pattern = 
    pcre2_code *re = pcre2_compile((PCRE2_SPTR)"^(\\d+?)\\1+$",
                                   PCRE2_ZERO_TERMINATED,
                                   0,
                                   &re_errno,
                                   &re_erroffset,
                                   NULL);
    if (re == NULL) {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(re_errno, buffer, sizeof(buffer));
        fprintf(stderr, "PCRE2 compilation failed at offset %d: %s\n",
               (int)re_erroffset,
               buffer);
        return 1;
    }
    
    unsigned long invalid_id_sum = 0;
    unsigned long start;
    unsigned long end;
    char s[16];
    while (EOF != scanf("%ld-%ld,", &start, &end)) {
        for (unsigned long id = start; id <= end; id++) {
            sprintf(s, "%ld", id);
                        
            pcre2_match_data *match_data;
            match_data = pcre2_match_data_create_from_pattern(re, NULL);
            PCRE2_SPTR subject = (PCRE2_SPTR)s;
            PCRE2_SIZE subject_len = (PCRE2_SIZE)strlen((char *)subject);
            
            int rc = pcre2_match(re, subject, subject_len,
                                 0, 0, match_data, NULL);
            if (rc > 0) {
                //printf("match: %ld\n", id);
                invalid_id_sum += id;
            }
        }
    }

    printf("%ld\n", invalid_id_sum);
    return 0;
}
