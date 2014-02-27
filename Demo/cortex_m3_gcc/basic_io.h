#ifndef BASIC_IO_H_
#define BASIC_IO_H_

extern int term_printf(const char *format, ...);
extern int term_scanf(const char *format, ...);
extern void panic(const char *format, ...);
#endif
