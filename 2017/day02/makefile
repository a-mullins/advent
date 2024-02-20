all: p01 p02

# Note that the source files use getline(), which is defined in
# stdio.h. However, getline() is not part of POSIX or ISO C and
# is therefore excluded when using -std=c17. We have to use
# -std=gnu17 instead.

opt = -std=gnu17 -Wall -Wextra -Werror

p01: p01.c
	gcc $(opt) -o p01 p01.c

p02: p02.c
	gcc $(opt) -o p02 p02.c
