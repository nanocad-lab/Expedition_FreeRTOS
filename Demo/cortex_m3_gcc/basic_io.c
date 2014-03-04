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
static char in_line[BUF_SIZE];
// retargeted stdio functions
// This function is not thread-safe. It can only be called after acquiring
// a mutex lock.
static int impl_write(unsigned long req_type, const char *format, \
        va_list args) {
    volatile unsigned long *addr;
    char *str;
    int i;
    int len = 0;
    
    // acquire a mutex to protect out_line and WRITEBUF
if (req_type != PANIC_REQ) {
    xSemaphoreTake(xPrint_Mutex, portMAX_DELAY);
}
  //{
        len = vsnprintf(out_line, BUF_SIZE, format, args);

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
        // add trailing \0
        if (addr < WRITEBUF_END)
            *addr = 0x0;
        // since IO_TYPE is in critical section, we need a lock to protect it.
        xSemaphoreTake(xREQ_Mutex, portMAX_DELAY);
        {
            // send a request signal to mbed
            addr = IO_TYPE;
            *addr = req_type;
            // send a request signal to mbed
            send_req();
        }
        xSemaphoreGive(xREQ_Mutex);

        // wait for acknowledge signal from mbed
        xSemaphoreTake(xPrintACK_BinarySemphr, portMAX_DELAY);
  //}
if (req_type != PANIC_REQ) {
    // Release a mutex lock
    xSemaphoreGive(xPrint_Mutex);
}

    return len;
}

static int impl_read(unsigned long req_type, char *buffer, size_t size) {
    volatile unsigned long *addr;
    char *str;
    int len;
    
    // acquire a mutex to protect READBUF
    xSemaphoreTake(xScan_Mutex, portMAX_DELAY);
    {
        // IO_TYPE is in critical section, need mutex to protect it.
        xSemaphoreTake(xREQ_Mutex, portMAX_DELAY);
        // write to IO_TYPE
        addr = IO_TYPE;
        *addr = req_type;
        // send a request signal to mbed
        send_req();
        xSemaphoreGive(xREQ_Mutex);

        // wait for acknowledge signal from mbed
        xSemaphoreTake(xScanACK_BinarySemphr, portMAX_DELAY);
        
        // read from ram buffer
        volatile char *str = (char *)READBUF_BEGIN;
        int i = 0;
        while (*str && i < size - 1) {
            buffer[i++] = *str++;
        }
        buffer[i] = '\0';
        //len = vsscanf(in_line, format, args);
    }
    // Release a mutex lock
    xSemaphoreGive(xScan_Mutex);

    return len;
}

int term_printf(const char *format, ...) {
    int len;

    va_list args;
    va_start(args, format);

    len = impl_write(TERM_PRINT_REQ, format, args);

    va_end(args);
    return len;
}

/* I should have implemented term_scanf and inet_scanf just like scanf in 
 * libc, but my implementation needs vsscanf which is problematic in this demo.
 * Calling vsscanf would result in a hard fault. 
 *
 * So the user has to read in a string firstly and then parse the string
 * by themselves.
 */
int term_gets(char *str, size_t size) {
    int len;
    len = impl_read(TERM_SCAN_REQ, str, size);
    return len;
}

int inet_printf(const char *format, ...) {
    int len;
    va_list args;

    va_start(args, format);
    len = impl_write(INET_PRINT_REQ, format, args);
    va_end(args);

    return len;
}


int inet_gets(char *str, size_t size) {
    int len;
    len = impl_read(INET_SCAN_REQ, str, size);
    return len;
}

// This function is called in fault handler to print information to ram
// buffer to debug use.
void panic(const char *format, ...) {
    // suspend all tasks and print error message
    vTaskSuspendAll();
    va_list args;

    va_start(args, format);
    impl_write(PANIC_REQ, format, args);
    va_end(args);

    while (1)
        ;
}
