.PHONY: all
all: p01 p02

p01.o: p01.c
p01: p01.o

p02.o: p02.c
p02: LDFLAGS = -L../lib/ -Wl,-rpath=../lib
p02: LDLIBS  = -ldarray
p02: p02.o

.PHONY: clean
clean:
	-rm -f p01.o
	-rm -f p01
	-rm -f p02.o
	-rm -f p02
