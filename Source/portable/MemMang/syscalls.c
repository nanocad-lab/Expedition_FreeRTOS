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

// provide definition for _write(), _read(), _close(), _read() to make
// linker silent
// I don't think any of functions used in this Demo actually calls any
// functions above. But it looks like if their definitions are not provided,
// the program cannot link....

int _write(int file, char *ptr, int len) {
    return 0;
}

int _read(int file, char *ptr, int len) {
    return 0;
}

int _close(int file) {
    return 0;
}

int _lseek(int file, int ptr, int dir) {
    return 0;
}
