.PHONY: all
all: p01

p01.o: p01.c
p01: LDFLAGS = -L../lib/ -Wl,-rpath=../lib
p01: LDLIBS  = -ldarray
p01: p01.o

.PHONY: clean
clean:
	-rm -f p01.o
	-rm -f p01
