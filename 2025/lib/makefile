libalist.so: CFLAGS += -I../include -fpic -shared -g
libalist.so: alist.c
	$(CC) $(CFLAGS) -o libalist.so alist.c

.PHONY: clean
clean:
	-rm -f libalist.so
