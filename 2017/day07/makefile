.PHONY: all
all: p01 p02

p01.o: p01.c
p01: LDFLAGS = -L../lib/ -Wl,-rpath=../lib
p01: LDLIBS  = -ldarray
p01: p01.o
	$(CC) $(LDFLAGS) $(LDLIBS) -o p01 p01.o


p02: p02.c

.PHONY: clean
clean:
	-rm -f p01
	-rm -f p02
