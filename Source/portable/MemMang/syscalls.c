/*
 * Provide the implementation for function _sbrk used in libc.a
 */
#include <sys/types.h>

extern int __HEAP_START;
extern int __HEAP_END;

// typedef char * caddr_t
caddr_t _sbrk(int incr) {
    static unsigned char *heap = NULL;
    unsigned char *prev_heap;

    if (heap == NULL) {
        heap = (unsigned char *)&__HEAP_START;
    }
    prev_heap = heap;
    if (heap + incr > __HEAP_END) {
        /* heap reaches end */
        return (caddr_t)0;
    }

    heap += incr;
    return (caddr_t)prev_heap;
}
