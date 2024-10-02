#include "zmalloc.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void __zfree_null(void) {
    fprintf(stderr, "zfree: nullptr\n");
    abort();
}

uint8_t pool[4096];

int main(int argc, char **argv) {
    if (!zmalloc_init(pool, 4096)) {
        fprintf(stderr, "zmalloc_init() failed\n");
        return 1;
    }

    print_memory();

    char *a = zrealloc(NULL, 2);
    print_memory();

    char *b = zrealloc(NULL, 25);
    print_memory();

    char *c[10];
    for (int i=0; i<10; i++)
        c[i] = zrealloc(NULL, 5*i+1);

    print_memory();

    puts("================================");

    zfree(c[2]);
    print_memory();

    zfree(c[0]);
    zfree(c[1]);
    print_memory();

    zfree(a);
    print_memory();
    zfree(b);
    print_memory();

    zfree(c[4]);
    zfree(c[6]);
    zfree(c[8]);
    print_memory();

    zfree(c[9]);
    print_memory();
    zfree(c[5]);
    print_memory();
    zfree(c[7]);
    print_memory();
    zfree(c[3]);
    print_memory();

    puts("================================");

    char *p = zrealloc(NULL, 64);
    print_memory();
    char *q = zrealloc(NULL, 128);
    p = zrealloc(p, 32);
    print_memory();
    p = zrealloc(p, 0);
    print_memory();
    q = zrealloc(q, 0);
    print_memory();

    void *foo = zrealloc(NULL, 17);
    print_memory();

    void *bar = zrealloc(NULL, 129);
    print_memory();

    void *qwe = zrealloc(NULL, 127);
    print_memory();

    zfree(qwe);
    print_memory();

    void *asd = zaligned_alloc(32, 40);
    print_memory();
    if ((uintptr_t) asd & 0x1f) puts("asd not aligned to 32(!)");

    zfree(foo);
    zfree(bar);
    zfree(asd);

    print_memory();

    int ret = zposix_memalign(&foo, 128, 256);
    if (ret != 0) puts("posix_memlign failed");

    print_memory();
    if ((uintptr_t) foo & 0x7f) puts("foo not aligned to 128(!)");

    zfree(foo);
    print_memory();

    foo = zmalloc(64);
    print_memory();
    foo = zrealloc(foo, 31);
    print_memory();
    foo = zrealloc(foo,23);
    print_memory();
    zfree(foo);

    return 0;
}
