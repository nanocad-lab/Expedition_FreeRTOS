#ifndef LOCKS_H_
#define LOCKS_H_

#include "FreeRTOS.h"
#include "../../Source/include/semphr.h"

extern xSemaphoreHandle xPrintACK_BinarySemphr;
extern xSemaphoreHandle xMutex;
#define xScanACK_BinarySemphr   ( xPrintACK_BinarySemphr )

extern void init_locks(void);

#endif
