#include <stdarg.h>
#include <stdio.h>
#include "mmap.h"
#include "myio.h"

static union byte_chunk_union {
    unsigned long word;
    char bytes[4];
} byte_chunk;

static char out_line[RAMBUF_SIZE];

// retargeted stdio functions
// This function is not thread-safe. It can only be called after acquiring
// a mutex lock.
static void impl_vprintf(const char *format, va_list args) {
    volatile unsigned long *addr;
    char *str;
    int i;
    
    // Acquire a mutex lock
    vsnprintf(out_line, RAMBUF_SIZE, format, args);

    addr = pulRAMBUF_BEGIN;
    str = out_line;
    do {
        for (i = 0; i < 4; ++i) {
            if (*str) {
                byte_chunk.bytes[i] = *str++;
            } else {
                byte_chunk.bytes[i] = '\0';
            }
        }
        *addr++ = byte_chunk.word;
    } while (*str && addr < pulRAMBUF_END);
    // Release the lock
    //
}

void myprintf(const char *format, ...) {
    va_list args;

    va_start(args, format);
    // Acquire a mutex lock
    impl_vprintf(format, args);
    // Release a mutex lock
    va_end(args);
}
// This function is called in fault handler to print information to ram
// buffer to debug use.
void panic(const char *format, ...) {
    va_list args;

    va_start(args, format);
    impl_vprintf(format, args);
    va_end(args);
}
