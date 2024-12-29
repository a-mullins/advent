# -std=gnu11 instead of c11 because of readline()
CFLAGS = -std=gnu11 -Wall -Wextra -Werror
LIBS = -lm

default: p01 p02

p01: p01.c
	gcc $(CFLAGS) $(LIBS) -o p01 p01.c

p02: p02.c
	gcc $(CFLAGS) $(LIBS) -o p02 p02.c

clean:
	-rm -f p01
	-rm -f p02
