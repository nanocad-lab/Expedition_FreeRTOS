#include <stdarg.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "mmap.h"
#include "basic_io.h"
#include "semphr.h"
#include "locks.h"
#include "gpio.h"

static union byte_chunk_union {
    unsigned long word;
    char bytes[4];
} byte_chunk;

static char out_line[BUF_SIZE];
extern xSemaphoreHandle xMutex;
extern xSemaphoreHandle xBinarySemaphore;

// retargeted stdio functions
// This function is not thread-safe. It can only be called after acquiring
// a mutex lock.
static int impl_write(const char *format, va_list args) {
    volatile unsigned long *addr;
    char *str;
    int i;
    int len = 0;
    
    vsnprintf(out_line, BUF_SIZE, format, args);

    addr = WRITEBUF_BEGIN;
    str = out_line;
    do {
        for (i = 0; i < 4; ++i) {
            if (*str) {
                byte_chunk.bytes[i] = *str++;
                ++len;
            } else {
                byte_chunk.bytes[i] = '\0';
            }
        }
        *addr++ = byte_chunk.word;
    } while (*str && addr < WRITEBUF_END);
    // write to IO_TYPE
    addr = IO_TYPE;
    *addr = PRINT_REQ;
    // send a request signal to mbed
    send_req();
    return len;
}

int term_printf(const char *format, ...) {
    int len;
    // acquire a mutex
    xSemaphoreTake(xMutex, portMAX_DELAY);

    va_list args;
    va_start(args, format);

    len = impl_write(format, args);
    // wait for acknowledge signal from mbed
    xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);

    va_end(args);
    // Release a mutex lock
    xSemaphoreGive(xMutex);
    return len;
}

int term_scanf(const char *format, ...) {
    int len;
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
    volatile char *str = (char *)WRITEBUF_BEGIN;
    len = vsscanf(str, format, args);

    va_end(args);
    // Release mutex
    xSemaphoreGive(xMutex);
    
    return len;
}

// This function is called in fault handler to print information to ram
// buffer to debug use.
void panic(const char *format, ...) {
    // suspend all tasks and print error message
    vTaskSuspendAll();
    va_list args;

    va_start(args, format);
    impl_write(format, args);
    va_end(args);

    while (1)
        ;
}
