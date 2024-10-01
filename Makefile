all: test

test: test.c
	$(CC) -g3 -o test test.c zmalloc.c -DZMALLOC_DEBUG
