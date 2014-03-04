#ifndef BASIC_IO_H_
#define BASIC_IO_H_

#include <stddef.h>

extern int term_printf(const char *format, ...);
extern int term_gets(char *str, size_t size);
extern int inet_printf(const char *format, ...);
extern int inet_gets(char *str, size_t size);
extern void panic(const char *format, ...);
#endif
