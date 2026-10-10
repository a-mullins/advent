#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct machine {
	int target;
	int target_len; /* for printing purposes */
	int lights;
	int buttons[16];
	int buttons_len;
};


struct tree {
	struct machine state;
	struct machine children[16];
	int children_len;
};


void machinetos(struct machine *, char *);
int hamming_d(int, int);
void a_star(struct tree *start, int (*goalcmp)(struct tree *),
	    int (*h)(struct machine *));


int
main(void)
{
	struct machine machines[256] = { 0 };
	int machines_len = 0;

	enum PARSING { NOTHING, TARGET, BUTTON } parsing = NOTHING;
	char c;
	int target = 0;
	int target_shift = 0;
	while (1) {
		c = getchar();
		if (c == '\n' || c == EOF) {
			machines_len++;
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
			machines[machines_len].target = target;
			machines[machines_len].target_len = target_shift;
		}

		/*
		 * button digits are only 0-9 so it's not necessary to collect
		 * multiple characters.
		 */
		if (parsing == BUTTON && isdigit(c)) {
			struct machine *m = &machines[machines_len];
			int *button_p = &m->buttons[m->buttons_len];
			*button_p ^= 1 << atoi(&c);
		}
		/* if (parsing == BUTTON && c == ',') { */
		/* 	memset(button_str, '\0', sizeof (button_str)); */
		/* 	button_str_len = 0; */
		/* } */
		if (parsing == BUTTON && c == ')') {
			machines[machines_len].buttons_len++;
			parsing = NOTHING;
		}

	};

	char line[1024];
	for (int i = 0; i < machines_len; i++) {
		memset(line, (int)'\0', sizeof (line));
		machinetos(&machines[i], &line[0]);
		printf("%d %d %s\n",
		       hamming_d(0, machines[i].target),
		       machines[i].buttons_len,
		       line);
 		/* puts(line); */
	}

	return 0;
}


void
machinetos(struct machine *m, char *s)
{
	/* char *s_o = s; */
	*s++ = '[';
	for (int i = 0; i < m->target_len; i++)
	{
		*s++ = m->target & (1 << i) ? '#' : '.';
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
}


int
hamming_d(int m, int n)
{
	return __builtin_popcount(m^n);
}
