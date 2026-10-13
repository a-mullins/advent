all: libalist.so libheapq.so heapq_test

libalist.so: CFLAGS += -I../include -fpic -shared -g
libalist.so: alist.c
	$(CC) $(CFLAGS) -o libalist.so alist.c

libheapq.so: CFLAGS += -I../include -fpic -shared -g
libheapq.so: heapq.c
	$(CC) $(CFLAGS) -o libheapq.so heapq.c

# heapq_test: CFLAGS += -I../include -g
# heapq_test: LDFLAGS += -L. '-Wl,-rpath,$$ORIGIN' -lheapq
# heapq_test: libheapq.so
heapq_test: CFLAGS += -I../include -g
heapq_test: heapq.c heapq_test.c
	$(CC) $(CFLAGS) -o heapq_test heapq_test.c


alist_test: CFLAGS += -I../include -g
alist_test: LDFLAGS += -L. '-Wl,-rpath,$$ORIGIN' -lalist
alist_test: libalist.so

.PHONY: clean
clean:
	-rm -f libalist.so libheapq.so heapq_test
