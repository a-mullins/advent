#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alist.h"
#include "heapq.h"


struct machine {
	int target;
	int target_len; /* for printing purposes */
	int lights;
	int buttons[16];
	int buttons_len;
	int presses;
};


void parseinput(struct machine *, int *);
void machinetos(struct machine *, char *);
int hamming_d(int, int);
int a_star(struct machine *start, int (*goalpred)(struct machine *),
	    int (*h)(struct machine *));
int goal_pred(struct machine *);
int h(struct machine *);
int intcmp(const void *, const void *);


int
main(void)
{
	struct machine machines[256] = { 0 };
	int machines_len = 0;
	long long sum = 0;

	parseinput(machines, &machines_len);

	/* char line[1024]; */
	/* for (int i = 0; i < machines_len; i++) { */
	/* 	memset(line, (int)'\0', sizeof (line)); */
	/* 	machinetos(&machines[i], &line[0]); */
	/* 	printf("%d %d %s\n", */
	/* 	       h(&machines[i]), */
	/* 	       machines[i].buttons_len, */
	/* 	       line); */
 	/* 	/\* puts(line); *\/ */
	/* } */

	/* puts(""); */
	printf("Checking %d machines.\n", machines_len);
	for (int i = 0; i < machines_len; i++)
		sum += a_star(&machines[i], goal_pred, h);
	/* a_star(&machines[0], goal_pred, h); */

	printf("%lld\n", sum);
	return 0;
}


void
parseinput(struct machine *machines, int *machines_len)
{
	/* Could use pcre here... */
	enum PARSING { NOTHING, TARGET, BUTTON } parsing = NOTHING;
	char c;
	int target = 0;
	int target_shift = 0;
	while (1) {
		c = getchar();
		if (c == '\n' || c == EOF) {
			(*machines_len)++;
			parsing = NOTHING;
			target = 0;
			target_shift = 0;
		}
		if (c == EOF)
			break;
		if (c == '\n') {
			c = getchar();
			if (c == EOF)
				break;
			else
				ungetc(c, stdin);
		}

		if (parsing == NOTHING && c == '[')
			parsing = TARGET;
		if (parsing == NOTHING && c == '(')
			parsing = BUTTON;

		if (parsing == TARGET && c == '.')
			target_shift++;
		if (parsing == TARGET && c == '#')
			target = target ^ (1 << target_shift++);
		if (parsing == TARGET && c == ']') {
			parsing = NOTHING;
			machines[*machines_len].target = target;
			machines[*machines_len].target_len = target_shift;
		}

		/*
		 * button digits are only 0-9 so it's not necessary to collect
		 * multiple characters.
		 */
		if (parsing == BUTTON && isdigit(c)) {
			struct machine *m = &machines[*machines_len];
			int *button_p = &m->buttons[m->buttons_len];
			/* *button_p ^= 1 << atoi(&c); */
			*button_p ^= 1 << (c - '0');
		}
		if (parsing == BUTTON && c == ')') {
			machines[*machines_len].buttons_len++;
			parsing = NOTHING;
		}

	};
}


void
machinetos(struct machine *m, char *s)
{
	/* char *s_o = s; */
	*s++ = '[';
	for (int i = 0; i < m->target_len; i++)
	{
		*s++ = m->lights & (1 << i) ? '#' : '.';
	}
	*s++ = ']';
	*s++ = ' ';

	for (int button_i = 0; button_i < m->buttons_len; button_i++)
	{
		*s++ = '(';
		for (int j = 0; j < m->target_len; j++)
		{
			if (m->buttons[button_i] & (1 << j)) {
				*s++ = j + '0';
				*s++ = ',';
			}
		}
		s--;
		*s++ = ')';
		*s++ = ' ';
	}
        sprintf(s, "button-presses %d", m->presses);
}


int
hamming_d(int m, int n)
{
	return __builtin_popcount(m^n);
}


int
a_star(struct machine *start, int (*pred)(struct machine *),
	    int (*h)(struct machine *))
{
	struct heapq *hq = heapq_new(sizeof (struct machine));
	struct alist *explored = alist_new(sizeof (int));
	int err;
	char buf[256];
	int presses = 0;

	/* machinetos(start, buf); */
	/* printf("Starting state\n%s\n", buf); */

	heapq_push(hq, start, 0);
	
	/* machinetos((struct machine *)heapq_peek(hq), buf); */
	/* printf("After push state\n%s\n", buf); */


	while (!heapq_empty(hq)) {
		struct machine cur = *(struct machine *)heapq_peek(hq);
		heapq_pop(hq);
		alist_push(explored, &cur.lights);

		/* printf("\tChecking machine\n\t%s\n", buf); */
		/* machinetos(&cur, buf); */

		if (pred(&cur)) {
			machinetos(&cur, buf);
			printf("\tSolution found:\n\t%s\n", buf);
			presses = cur.presses;
			break;
		}

		/*
		 * Generate children.
		 * For each child, filter out states already seen,
		 * then push to the queue with priority equal to hamming
		 * distance from the goal
		 */
		for (int i = 0; i < cur.buttons_len; i++) {
			struct machine child = cur;
			child.presses++;
			child.lights ^= cur.buttons[i];

			/* machinetos(&child, buf); */
			/* printf("\tspawned child machine \n\t%s\n", buf); */

			alist_qsort(explored, intcmp);
			ssize_t res = alist_bsearch(explored, &child.lights, intcmp);
			if (res < 0) {
				/* puts("\t\tNot seen this light pattern yet."); */
				/* We haven't explored this state yet,
				   but it might already be in the queue. */
				res = heapq_in(hq, &child);
				if (res == 0) {
					/* puts("\t\tThis machine is not in the queue."); */
					/* printf("\t\tAdding with priority %d\n", h(&child)); */
					heapq_push(hq, &child, child.presses);
				} else {
					/* puts("\t\tThis light pattern is already in the queue; discarding."); */
				}
			} else {
				/* puts("\t\tAlready seen this light pattern; discarding."); */
			}
			/* puts(""); */
		}
	}

	/* alist_free(explored, NULL); */
	heapq_free(hq, NULL);
	return presses;
}



int
goal_pred(struct machine *m)
{
	return m->target == m->lights;
}


int
h(struct machine *m)
{
	return hamming_d(m->target, m->lights);
}


int
intcmp(const void *p, const void *q)
{
	int n = *(int *)p;
	int m = *(int *)q;
	if (n < m) return -1;
	if (n > m) return  1;
	else       return  0;
}
