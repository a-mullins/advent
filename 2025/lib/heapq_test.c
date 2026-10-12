#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "heapq.h"


int
main(void)
{
    srand(time(NULL));
    struct heapq *hq;
    
    hq = heapq_new(sizeof(char));
    for (int i = 0; i < 16; i++) {
	char c = 'a' + (char)i;
	size_t r = (unsigned int)rand();
	/* size_t s = (unsigned char)rand(); */
	heapq_push(hq, &c, r);
    }
    for (int i = 0; i < heapq_len(hq); i++) {
	printf("i: %2d data: %c pri: %u\n",
	       i,
	       *(char *)heapq_get(hq, i).data,
	       heapq_get(hq, i).pri);
    }
    heapq_free(hq, NULL);

    putchar('\n');
    hq = heapq_new(sizeof(long long));
    for (long long i = 0; i < 16; i++) {
	size_t r = (unsigned int)rand();
	/* size_t s = (unsigned char)rand(); */
	heapq_push(hq, &i, r);
    }
    for (int i = 0; i < heapq_len(hq); i++) {
	printf("i: %2d data: %2lld pri %u\n",
	       i,
	       *(long long *)heapq_get(hq, i).data,
	       heapq_get(hq, i).pri);
    }
    heapq_free(hq, NULL);

    putchar('\n');
    hq = heapq_new(sizeof(char *));
    char *strs[] = {"foo", "bar", "baz", "spam", "ni", "sheffield blades", "shrubbery", "tiger"};
    char *s;
    for (int i = 0; i < sizeof (strs) / sizeof (strs[0]); i++) {
	s = malloc(strlen(strs[i]) + 1);
	strcpy(s, strs[i]);
	heapq_push(hq, &s, i);
    }
    for (int i = 0; i < heapq_len(hq); i++) {
	printf("i: %2d pri %2u data: %s\n",
	       i,
	       heapq_get(hq, i).pri,
	       *(char **)heapq_get(hq, i).data);
    }
    heapq_free(hq, NULL);
    
    return 0;
}
