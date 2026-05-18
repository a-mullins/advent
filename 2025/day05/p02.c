#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alist.h"


struct range {
    unsigned long start;
    unsigned long end;
};


int rangecmp(const void *a, const void *b);
struct alist *merge_overlaps(struct alist *ranges);


int
main(void)
{
    struct alist *fresh_id_ranges;
    fresh_id_ranges = alist_new(sizeof (struct range));

    char line[128];
    for (size_t i = 0; NULL != fgets(line, 128, stdin); i++) {
        if (!strcmp(line, "\n")) break;

        struct range r;
        sscanf(line, "%ld-%ld", &r.start, &r.end);
        alist_push(fresh_id_ranges, &r);
    }

    struct alist *merged = merge_overlaps(fresh_id_ranges);

    unsigned long total_ids = 0;
    for(size_t i = 0; i < alist_len(merged); i++) {
        struct range *r_p = alist_get(merged, i);
        printf("range %3d: %15ld-%-15ld %14ld ids\n",
               i, r_p->start, r_p->end,
               1 + (r_p->end - r_p->start));
        total_ids += 1 + (r_p->end - r_p->start);
    }
    printf("%ld\n", total_ids);
    alist_free(fresh_id_ranges, NULL);
    alist_free(merged, NULL);
    return 0;
}


int
rangecmp(const void *a, const void *b)
{
    struct range *q = (struct range *)a;
    struct range *r = (struct range *)b;

    if (q->start < r->start) return -1;
    if (q->start > r->start) return 1;
    return 0;
}


struct alist *
merge_overlaps(struct alist *ranges)
{
    alist_qsort(ranges, rangecmp);

    struct alist *merged;
    merged = alist_new(sizeof (struct range));
    alist_push(merged, alist_get(ranges, 0));

    for (size_t i = 1; i < alist_len(ranges); i++) {
        struct range *last_p = alist_last(merged);
        struct range *cur_p = alist_get(ranges, i);

        if (cur_p->start <= last_p->end)
            last_p->end = last_p->end >= cur_p->end ? last_p->end : cur_p->end;
        else
            alist_push(merged, cur_p);
    }
    return merged;
}
