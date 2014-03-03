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
    return len;
}

int term_printf(const char *format, ...) {
    volatile unsigned long *addr;
    int len;
    // acquire a mutex
    xSemaphoreTake(xPrint_Mutex, portMAX_DELAY);

    va_list args;
    va_start(args, format);

    len = impl_write(format, args);

    // IO_TYPE is in critical section, need mutex to protect it.
    //xSemaphoreTake(xIOTYPE_Mutex, portMAX_DELAY);
    // write to IO_TYPE
    addr = IO_TYPE;
    *addr = TERM_PRINT_REQ;
    // send a request signal to mbed
    send_req();

    // use a spin lock to make sure mbed read addr properly
    /*
    while (*addr != IO_TYPE_ACK)
        ;
    xSemaphoreGive(xIOTYPE_Mutex);
    */

    // wait for acknowledge signal from mbed
    xSemaphoreTake(xPrintACK_BinarySemphr, portMAX_DELAY);

    va_end(args);
    // Release a mutex lock
    xSemaphoreGive(xPrint_Mutex);
    return len;
}

int term_scanf(const char *format, ...) {
    volatile unsigned long *addr;
    int len;
    // acquire a mutex
    xSemaphoreTake(xScan_Mutex, portMAX_DELAY);

    // IO_TYPE is in critical section, need mutex to protect it.
    xSemaphoreTake(xIOTYPE_Mutex, portMAX_DELAY);
    // write to IO_TYPE
    addr = IO_TYPE;
    *addr = TERM_SCAN_REQ;
    // send a request signal to mbed
    send_req();
    // use a spin lock to make sure mbed read addr properly
    while (*addr != IO_TYPE_ACK)
        ;
    xSemaphoreGive(xIOTYPE_Mutex);
   
    // wait for acknowledge signal from mbed
    xSemaphoreTake(xScanACK_BinarySemphr, portMAX_DELAY);

    // read from ram buffer
    va_list args;
    va_start(args, format);
    volatile char *str = (char *)READBUF_BEGIN;
    len = vsscanf(str, format, args);

    va_end(args);
    // Release mutex
    xSemaphoreGive(xScan_Mutex);
    
    return len;
}

int inet_printf(const char *format, ...) {
    volatile unsigned long *addr;
    int len;
    // acquire a mutex
    xSemaphoreTake(xPrint_Mutex, portMAX_DELAY);

    va_list args;
    va_start(args, format);

    len = impl_write(format, args);

    // IO_TYPE is in critical section, need mutex to protect it.
    xSemaphoreTake(xIOTYPE_Mutex, portMAX_DELAY);
    // write to IO_TYPE
    addr = IO_TYPE;
    *addr = INET_PRINT_REQ;
    // send a request signal to mbed
    send_req();
    // use a spin lock to make sure mbed read addr properly
    while (*addr != IO_TYPE_ACK)
        ;
    xSemaphoreGive(xIOTYPE_Mutex);

    // wait for acknowledge signal from mbed
    xSemaphoreTake(xPrintACK_BinarySemphr, portMAX_DELAY);

    va_end(args);
    // Release a mutex lock
    xSemaphoreGive(xPrint_Mutex);
    return len;
}

int inet_scanf(const char *format, ...) {
    volatile unsigned long *addr;
    int len;
    // acquire a mutex
    xSemaphoreTake(xScan_Mutex, portMAX_DELAY);

    // IO_TYPE is in critical section, need mutex to protect it.
    xSemaphoreTake(xIOTYPE_Mutex, portMAX_DELAY);
    // write to IO_TYPE
    addr = IO_TYPE;
    *addr = INET_SCAN_REQ;
    // send a request signal to mbed
    send_req();
    // use a spin lock to make sure mbed read addr properly
    while (*addr != IO_TYPE_ACK)
        ;
    xSemaphoreGive(xIOTYPE_Mutex);
   
    // wait for acknowledge signal from mbed
    xSemaphoreTake(xScanACK_BinarySemphr, portMAX_DELAY);

    // read from ram buffer
    va_list args;
    va_start(args, format);
    volatile char *str = (char *)READBUF_BEGIN;
    len = vsscanf(str, format, args);

    va_end(args);
    // Release mutex
    xSemaphoreGive(xScan_Mutex);
    
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
