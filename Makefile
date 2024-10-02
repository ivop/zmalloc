all: test

test: test.c zmalloc.c
	$(CC) -g3 -o test test.c zmalloc.c -DZMALLOC_DEBUG

clean:
	rm -f *~ test
