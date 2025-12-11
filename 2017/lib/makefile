.PHONY: all
all: libdarray.so

libdarray.so: CFLAGS += -g -fpic
libdarray.so: darray.c
	$(CC) $(CFLAGS) -shared -o libdarray.so darray.c

.PHONY: clean
clean:
	-rm -f libdarray.so
