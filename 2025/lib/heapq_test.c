#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "heapq.c"


int
main(void)
{
    srand(time(NULL));
    struct heapq *hq = heapq_new(sizeof (char));
    
    for (char c = 'a'; c <= 'z'; c++)
	heapq_push(hq, &c, random() % 512);

    printf("id lchild rchild data priority\n");
    for (int i = 0; i < heapq_len(hq); i++) {
	printf("%2d %6d %6d %4c %8u\n",
	       i,
	       lchild_i(i),
	       rchild_i(i),
	       *(char *)heapq_get(hq, i),
	       heapq_pri(hq, i));
    }
    
    printf("Is heap? %s\n", is_heap(hq) ? "Yes" : "No");
    puts("Press enter to pop...");
    getchar(); 
    puts("Pop...");
    for (int i = 0; i < 13; i++) {
	if (i == 0) {
	    printf("Popping %c", *(char *)heapq_peek(hq));
	} else if (i == 12) {
	    printf(", and %c", *(char *)heapq_peek(hq));
	} else {
	    printf(", %c", *(char *)heapq_peek(hq));
	}
	heapq_pop(hq);
    }
    printf("\n\nid lchild rchild data priority\n");
    for (int i = 0; i < heapq_len(hq); i++) {
	printf("%2d %6d %6d %4c %8u\n",
	       i,
	       lchild_i(i),
	       rchild_i(i),
	       *(char *)heapq_get(hq, i),
	       heapq_pri(hq, i));
    }
    printf("Is heap? %s\n", is_heap(hq) ? "Yes" : "No");
    
    return 0;
}
