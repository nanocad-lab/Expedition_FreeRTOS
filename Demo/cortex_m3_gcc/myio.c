#include <stdarg.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "mmap.h"
#include "myio.h"
#include "semphr.h"
#include "locks.h"
#include "gpio_set.h"

static union byte_chunk_union {
    unsigned long word;
    char bytes[4];
} byte_chunk;

static char out_line[RAMBUF_SIZE];
extern xSemaphoreHandle xMutex;
extern xSemaphoreHandle xBinarySemaphore;

// retargeted stdio functions
// This function is not thread-safe. It can only be called after acquiring
// a mutex lock.
static void impl_vprintf(const char *format, va_list args) {
    volatile unsigned long *addr;
    char *str;
    int i;
    unsigned len = 0;
    
    vsnprintf(out_line, RAMBUF_SIZE, format, args);

    addr = RAMBUF_BEGIN;
    str = out_line;
    do {
        for (i = 0; i < 4; ++i) {
            if (*str) {
                byte_chunk.bytes[i] = *str++;
            } else {
                byte_chunk.bytes[i] = '\0';
            }
        }
        ++len;
        *addr++ = byte_chunk.word;
    } while (*str);
    // write to IO_TYPE
    addr = IO_TYPE;
    *addr = PRINT_REQ;
    *(addr - 1) = len;
    // send a request signal to mbed
    send_req();
}

void myprintf(const char *format, ...) {
    // acquire a mutex
    xSemaphoreTake(xMutex, portMAX_DELAY);

    va_list args;

    va_start(args, format);

    impl_vprintf(format, args);
    // wait for acknowledge signal from mbed
    xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);

    va_end(args);
    // Release a mutex lock
    xSemaphoreGive(xMutex);
}

void myscanf(const char *format, ...) {
    // acquire a mutex
    xSemaphoreTake(xMutex, portMAX_DELAY);
   
    // Write IO_TYPE and send a scan request signal to mbed
    volatile unsigned long *addr = IO_TYPE;
    *addr = SCAN_REQ;
    send_req(); 
    // wait for acknowledge signal from mbed
    xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);

    // read from ram buffer
    va_list args;
    va_start(args, format);
    volatile char *str = (char *)RAMBUF_BEGIN;
    vsscanf(str, format, args);

    va_end(args);
    // Release mutex
    xSemaphoreGive(xMutex);
}

// This function is called in fault handler to print information to ram
// buffer to debug use.
void panic(const char *format, ...) {
    va_list args;

    va_start(args, format);
    impl_vprintf(format, args);
    va_end(args);
}
