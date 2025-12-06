#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// There are a few different approaches to today's puzzle that might work.
// 1. Bloom filter might be made to work?
// 2. Interval or segment tree is a perfect fit for this problem.
// 3. The naive approach of checking every id against every range.
//    This is, I think, O(m*n).
// I use the naive approach because it is quickest to implement and m and n
// are both reasonably small.


struct range {
    long start;
    long end;
};


bool is_fresh_id(struct range *rs, long id);


int
main(void)
{
    struct range fresh_id_ranges[256] = {0};

    char line[128];
    for (size_t i = 0; NULL != fgets(line, 128, stdin); i++) {
        if (!strcmp(line, "\n"))
            break;
        sscanf(line, "%ld-%ld",
               &fresh_id_ranges[i].start,
               &fresh_id_ranges[i].end);
    }

    // for (size_t i = 0; fresh_id_ranges[i].start != 0; i++) {
    //     printf("range %03ld: %ld-%ld\n", i,
    //             fresh_id_ranges[i].start,
    //             fresh_id_ranges[i].end);
    // }

    long num_fresh_ids = 0;
    while (NULL != fgets(line, 128, stdin)) {
        if (!strcmp(line, "\n"))
            continue;
        long id = atol(line);
        if (is_fresh_id(fresh_id_ranges, id)) {
            // printf("%ld is fresh.\n", id);
            num_fresh_ids++;
        }
        //  else {
        //     printf("%ld is spoiled.\n", id);            
        // }
    }

    printf("%ld\n", num_fresh_ids);
    return 0;
}


bool
is_fresh_id(struct range *rs, long id)
{
    for (size_t i = 0; rs[i].start != 0; i++) {
        if (id >= rs[i].start && id <= rs[i].end)
            return true;
    }

    return false;
}
