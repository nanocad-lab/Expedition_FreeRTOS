#ifndef MMAP_H_
#define MMAP_H_

// memory address range of ram buffer used for data communication between
// mbed and Cortex-M3
#define WRITEBUF_BEGIN    ( (unsigned long *)0x24000100 )
#define WRITEBUF_END      ( (unsigned long *)0x2400017C )
#define READBUF_BEGIN     ( (unsigned long *)0x24000180 )
#define READBUF_END       ( (unsigned long *)0x240001FC )

#define BUF_SIZE     ( (unsigned long)(WRITEBUF_END - WRITEBUF_BEGIN)*4 )
// write to this address to indicate whether it is a print req or scan req
#define IO_TYPE         ( (unsigned long *)0x2400017C )
#define PRINT_REQ       ( 0UL )
#define SCAN_REQ        ( 1UL )

#endif
