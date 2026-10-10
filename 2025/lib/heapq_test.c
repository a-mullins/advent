#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "heapq.c"


int
main(void)
{
    srand(time(NULL));
    
    struct heapq *hq = heapq_new(sizeof(size_t));
    size_t i = 0;
    for (; i < 16; i++) {
	size_t r = (unsigned char)rand();
	/* size_t s = (unsigned char)rand(); */
	heapq_push(hq, &i, r);
    }
    i = SIZE_MAX;
// h.push("foo", 5);
// h.push("bar", 3);
// h.push("baz", 9);
// h.push("spam", 8);
// h.push("ni", 0);
// h.push("sheffield", 12);
// h.push("shrubbery", 30);
// h.push("tiger", 15);
// let range = n => Array.from({length: n}, (_, i) => i);
// range(h.array.length).map(n=>h.heap_p(n));
// console.log(h.pop());
// console.log(h.pop());
    for (size_t i = 0; i < heapq_len(hq); i++)
	printf("i: %zu\tdata: %zu\tpriority: %zu\n",
	       i,
	       *(size_t *)heapq_get(hq, i),
	       heapq_pri(hq, i));

    heapq_free(hq, NULL);
    return 0;
}
