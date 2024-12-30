# -std=gnu11 instead of c11 because of readline()
CFLAGS = -std=gnu11 -Wall -Wextra -Werror

default: p01

p01: p01.c darray.c
	gcc $(CFLAGS) -o p01 p01.c darray.c

clean:
	-rm -f p01
