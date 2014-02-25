#ifndef MMAP_H_
#define MMAP_H_

// memory address range of ram buffer used for data communication between
// mbed and Cortex-M3
#define RAMBUF_BEGIN    ( (unsigned long *)0x24000100 )
#define RAMBUF_END      ( (unsigned long *)0x2400017C )
#define RAMBUF_SIZE     ( (unsigned long)(RAMBUF_END - RAMBUF_BEGIN)*4 )
// write to this address to indicate whether it is a print req or scan req
#define IO_TYPE         ( (unsigned long *)0x2400017C )
#define PRINT_REQ       ( 0UL )
#define SCAN_REQ        ( 1UL )

#endif
