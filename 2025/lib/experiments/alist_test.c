#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>
#include "alist.h"


int longcmp(const void *, const void *);
void freestr(void *);


int
main(void)
{
    /*
     * Set a limit on how much memory can be used by process so
     * malloc() (and alist) will error if it it's leaking.
     */
    rlim_t lim = 256 * 1<<10;
    struct rlimit r = {lim, lim};
    setrlimit(RLIMIT_DATA, &r);
    
    int alist_err;
    ALIST_DECL(, float) v;
    ALIST_INIT(&v, alist_err);    
    if (alist_err) {
	fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__);
	return 1;
    }

    float xs[] = {0.0, 1.0, 1.0, 2.0, 3.0, 5.0, 8.0, 13.0, 21.0, 34.0, 55.0, 89.0};
    for (size_t i = 0; i < sizeof (xs) / sizeof (xs[0]); i++) {
	ALIST_PUSH(&v, xs[i], alist_err);
	if (alist_err) {
	    fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__);
	    return 1;
	}	
    }

    float *fp;
    ALIST_FOREACH(&v, fp) {
	short n = (short)(*fp / 10.0f);
	// printf("%2.0f %2hd\n", *fp, n);
    }

    /* getchar() */

    ALIST_DECL(, long) l;
    ALIST_INIT(&l, alist_err);
    if (alist_err) {
	fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__);
	return 1;
    }
    srand(time(NULL));
    for (int i = 0; i < 24; i++) {
	ALIST_PUSH(&l, random(), alist_err);
	if (alist_err) {
	    fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__);
	    return 1;
	}
    }

    ALIST_QSORT(&l, longcmp);
    long *lp;
    /* ALIST_FOREACH(&l, lp) */
	// printf("%ld\n", *lp);

    /* getchar() */

    long key = 1649760492;
    /* if (ALIST_BSEARCH(&l, &key, longcmp)) */
	// printf("Found: %ld\n", key);
    
    key = 42;
    /* if (ALIST_BSEARCH(&l, &key, longcmp)) */
	// printf("Found: %ld\n", key);
    /* else */
	// printf("Not found: %ld.\n", key);

    // printf("l.cap %3lld, l.len %3lld\n", l.cap, ALIST_LEN(&l));
    ALIST_CLEAR(&l, NULL);
    // printf("l.cap %3lld, l.len %3lld\n", l.cap, ALIST_LEN(&l));
    ALIST_FREE(&l, NULL);
    // printf("l.cap %3lld, l.len %3lld\n", l.cap, ALIST_LEN(&l));

    ALIST_DECL(, char *) strs;
    ALIST_INIT(&strs, alist_err);
    if (alist_err) {
	fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__);
	return 1;
    }
    for (;;) {
	/* Generate random strings */
	for (int i = 0; i < 1024; i++) {
	    char buf[80];
	    char *s_p;
	    for (int j = 0; j < 72; j++) {
		buf[j] = (char)(97 + (random() % 25));	    

	    }
	    s_p = calloc(80, sizeof (char));
	    if (s_p == NULL) {
		fprintf(stderr, "Out-of-memory at %s:%d\n", __FILE__, __LINE__);
		return 1;
	    }
	    strncpy(s_p, buf, 72);
	    ALIST_PUSH(&strs, s_p, alist_err);
	    if (alist_err) {
		fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__);
		return 1;
	    }
	}

	/* Print them. */
	/* getchar() */
	/* printf("strs.cap %3lld, strs.len %3lld\n", strs.cap, ALIST_LEN(&strs)); */
	/* char **s_p; */
	/* ALIST_FOREACH(&strs, s_p) */
	/*     puts(*s_p); */
	puts(ALIST_AT(&strs, 2));

	/* getchar() */
	/* ALIST_CLEAR(&strs, free); */
	ALIST_CLEAR(&strs, free);
    }
    
    return 0;
}


int
longcmp(const void *p, const void *q)
{
    long n = *(long *)p;
    long m = *(long *)q;
    if (n < m) return -1;
    if (n > m) return  1;
    else       return  0;
}
