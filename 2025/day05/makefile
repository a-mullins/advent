CFLAGS = -g

.PHONY: all
all: p01 p02

p01:

p02:  CFLAGS += -I../include
p02: LDFLAGS += -L../lib '-Wl,-rpath,$$ORIGIN/../lib' -lalist
p02: ../lib/libalist.so

../lib/libalist.so:
	$(MAKE) -C ../lib

.PHONY: clean
clean:
	-rm -f p01.o p01 p02.o p02
