#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

struct machine {
	int lights;
	int lights_len; /* for printing purposes */
	int buttons[16];
	int buttons_len;
};

void machinetos(struct machine *, char *);

int
main(void)
{
	struct machine machines[256] = { 0 };
	int machines_len = 0;
	
	enum PARSING { NOTHING, LIGHTS, BUTTON } parsing = NOTHING;
	char c;
	int lights = 0;
	int lights_shift = 0;
	do {
		c = getchar();
		if (parsing == NOTHING && c == '[')
			parsing = LIGHTS;
		if (parsing == NOTHING && c == '(') {
			parsing = BUTTON;
		}
		if (parsing == NOTHING && (c == '\n' || c == EOF)) {
			lights = 0;
			lights_shift = 0;
			machines_len++;
		}

		if (parsing == LIGHTS && c == '.')
			lights_shift++;
		if (parsing == LIGHTS && c == '#')
			lights = lights ^ (1 << lights_shift++);
		if (parsing == LIGHTS && c == ']') {
			parsing = NOTHING;
			machines[machines_len].lights = lights;
			machines[machines_len].lights_len = lights_shift;
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

	} while (c != EOF);

	char line[128];
	for (int i = 0; i < machines_len; i++) {
		machinetos(&machines[i], &line[0]);
		puts(line);
	}
	
	return 0;
}


void
machinetos(struct machine *m, char *s)
{
	/* char *s_o = s; */
	*s++ = '[';
	for (int i = 0; i < m->lights_len; i++)
	{
		*s++ = m->lights & (1 << i) ? '#' : '.';
	}
	*s++ = ']';
	*s++ = ' ';
	
	for (int button_i = 0; button_i < m->buttons_len; button_i++)
	{
		*s++ = '(';
		for (int j = 0; j < m->lights_len; j++)
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
