#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include <stdio.h>
#include <string.h>

struct machine {
	char lights;
	/* buttons */
	/* joltage */
};


/* void strrev(const char *, char *); */


int
main(void)
{
	/* Set up a regex to details from an input line. */
	int re_errno;
	PCRE2_SIZE re_erroffset;
	char *re_str = "\\[([.#]+)\\] *"
		"((?:\\([0-9]+(?:,[0-9]+)*\\) *)+) *"
		"((?:\\{[0-9]+(?:,[0-9]+)*\\} *)+)";
	pcre2_code *re = pcre2_compile(
		(PCRE2_SPTR)re_str,
		PCRE2_ZERO_TERMINATED,
		0,
		&re_errno,
		&re_erroffset,
		NULL);
	if (re == NULL) {
		PCRE2_UCHAR buffer[256];
		pcre2_get_error_message(re_errno, buffer, sizeof(buffer));
		fprintf(stderr, "PCRE2 compilation failed at offset %d: %s\n",
			(int)re_erroffset,
			buffer);
		return 1;
	}
	
	char line[128];
	int line_len;
	pcre2_match_data *match_data;
	
	match_data = pcre2_match_data_create_from_pattern(re, NULL);
	
	while (NULL != fgets(line, sizeof (line), stdin)) {
		int rc = pcre2_match(re, line, strlen(line),
				     0, 0, match_data, NULL);
		
		if (rc > 0) {
			puts("Match!");
			PCRE2_UCHAR *substr_buf = NULL;
			PCRE2_SIZE substr_len = 0;
			for (int i = 0; i < rc; i++) {
				int status = pcre2_substring_get_bynumber(
					match_data,
					i+1,
					&substr_buf,
					&substr_len);

				if (status == 0) {
					printf("group %d: \"%s\" (length: %d)\n",
					       i+1, (char *)substr_buf, (int)substr_len);
					pcre2_substring_free(substr_buf);
					/* printf("match: %s\n", match_str_p); */
				}
			}
		} else {
			if (rc == PCRE2_ERROR_NOMATCH)
				puts("No match.");
			else
				printf("Error %d\n", rc);
		}

	}

	pcre2_match_data_free(match_data);
	pcre2_code_free(re);
	return 0;
}


/* void */
/* strrev(const char *s, char *r) */
/* { */
/*     if (strlen(s) < 2) */
/* 	return; */

/*     r = r + (strlen(s) - 1); */
    
/*     while (*s) */
/* 	*r-- = *s++; */

/*     return; */
/* } */
