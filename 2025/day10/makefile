# .PHONY: all
# all: p01 p02

# p01: LDLIBS = -g
# p02: LDLIBS = -lm -g -Wall

# .PHONY: clean
# clean:
# 	-rm -f p01 p02

CFLAGS = -g

.PHONY: all
all: p01 

p01:  CFLAGS += -I../include
p01: LDFLAGS += -L../lib '-Wl,-rpath,$$ORIGIN/../lib' -lalist -lheapq
p01:

alist:  CFLAGS += -I../include
alist: LDFLAGS += -L../lib '-Wl,-rpath,$$ORIGIN/../lib' -lalist
alist:

heapq:  CFLAGS += -I../include
heapq: LDFLAGS += -L../lib '-Wl,-rpath,$$ORIGIN/../lib' -lalist
heapq: ../lib/libalist.so ../lib/heapq.sos

../lib/libalist.so:
	$(MAKE) -C ../lib

../lib/libheapq.so:
	$(MAKE) -C ../lib




.PHONY: clean
clean:
	-rm -f p01.o p01 p02.o p02
