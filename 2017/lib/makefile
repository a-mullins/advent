.PHONY: all
all: libdarray.so

darray.o: CFLAGS += -fpic
darray.o: darray.c

libdarray.so: darray.o
	$(CC) -shared -o libdarray.so darray.o

.PHONY: clean
clean:
	-rm -f darray.o
	-rm -f libdarray.so
