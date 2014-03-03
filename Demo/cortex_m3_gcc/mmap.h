#ifndef MMAP_H_
#define MMAP_H_

// memory address range of ram buffer used for data communication between
// mbed and Cortex-M3
#define WRITEBUF_BEGIN    ( (volatile unsigned long *)0x24000100 )
#define WRITEBUF_END      ( (volatile unsigned long *)0x2400017C )
#define READBUF_BEGIN     ( (volatile unsigned long *)0x24000180 )
#define READBUF_END       ( (volatile unsigned long *)0x240001FC )

#define BUF_SIZE     ( (unsigned long)(WRITEBUF_END - WRITEBUF_BEGIN)*4 )
// write to this address to indicate whether it is a print req or scan req
#define IO_TYPE         ( (volatile unsigned long *)0x2400017C )
#define ACK_TYPE        ( (volatile unsigned long *)0x240001FC )
#define TERM_PRINT_REQ       ( 0UL )
#define TERM_SCAN_REQ        ( 1UL )
#define PRINT_ACK  ( 0UL )
#define SCAN_ACK   ( 1UL )

#endif
