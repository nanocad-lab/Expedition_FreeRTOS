#include "mmap.h"

// GPIO control registers
volatile unsigned long * const pulGPIO_DATA = \
        (volatile unsigned long *)0x44001000;

volatile unsigned long * const pulGPIO_DIRECTION = \
        (volatile unsigned long *)0x44001004;

volatile unsigned long * const pulGPIO_MASK = \
        (volatile unsigned long *)0x44001008;

// Memory range of ram buffer
volatile unsigned long * const pulRAMBUF_BEGIN = \
        (volatile unsigned long *)0x24000100;

volatile unsigned long * const pulRAMBUF_END = \
        (volatile unsigned long *)0x24000180;
