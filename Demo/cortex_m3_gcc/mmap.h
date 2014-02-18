#ifndef MMAP_H_
#define MMAP_H_

// GPIO control registers
extern volatile unsigned long * const pulGPIO_DATA;
extern volatile unsigned long * const pulGPIO_DIRECTION;
extern volatile unsigned long * const pulGPIO_MASK;

// memory address range of ram buffer used for data communication between
// mbed and Cortex-M3
extern volatile unsigned long * const pulRAMBUF_BEGIN;
extern volatile unsigned long * const pulRAMBUF_END;

#define RAMBUF_SIZE (128)

#define SEND_PRINT_REQ ( *pulGPIO_DATA = 0x1 )

#endif
