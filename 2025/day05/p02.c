#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/darray.h"


struct range {
    unsigned long start;
    unsigned long end;
};


int rangecmp(const void *a, const void *b);
struct darray *merge_overlaps(struct darray *ranges);


int
main(void)
{
    struct darray fresh_id_ranges;
    darray_init(&fresh_id_ranges, sizeof (struct range));

    char line[128];
    for (size_t i = 0; NULL != fgets(line, 128, stdin); i++) {
        if (!strcmp(line, "\n")) break;

        struct range r;
        sscanf(line, "%ld-%ld", &r.start, &r.end);
        darray_push(&fresh_id_ranges, &r);
    }

    struct darray *merged = merge_overlaps(&fresh_id_ranges);

    unsigned long total_ids = 0;
    for(size_t i = 0; i<merged->len; i++) {
        struct range *r_p = darray_get(merged, i);
        printf("range %3d: %15ld-%-15ld %14ld ids\n",
               i, r_p->start, r_p->end,
               1 + (r_p->end - r_p->start));
        total_ids += 1 + (r_p->end - r_p->start);
    }
    printf("%ld\n", total_ids);
    darray_free(&fresh_id_ranges, NULL);
    darray_free(merged, NULL);
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


struct darray *
merge_overlaps(struct darray *ranges)
{
    darray_qsort(ranges, rangecmp);

    struct darray *merged = malloc(sizeof (struct darray));
    darray_init(merged, sizeof(struct range));
    darray_push(merged, darray_get(ranges, 0));

    for (size_t i = 1; i < ranges->len; i++) {
        struct range *last_p = darray_last(merged);
        struct range *cur_p = darray_get(ranges, i);

        if (cur_p->start <= last_p->end)
            last_p->end = last_p->end >= cur_p->end ? last_p->end : cur_p->end;
        else
            darray_push(merged, cur_p);
    }
    return merged;
}
