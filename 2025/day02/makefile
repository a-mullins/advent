CFLAGS = -g

.PHONY: all
all: p01 p01_alt p02

p01_alt: LDFLAGS += -lm

p02: LDFLAGS = -lpcre2-8

.PHONY: clean
clean:
	-rm -f p01 p01_alt p02
