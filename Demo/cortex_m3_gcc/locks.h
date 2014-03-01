#ifndef LOCKS_H_
#define LOCKS_H_

#include "FreeRTOS.h"
#include "../../Source/include/semphr.h"

// binary semaphores to synchronize tasks
extern xSemaphoreHandle xPrintACK_BinarySemphr;
extern xSemaphoreHandle xDDRO_BinarySemphr;

// mutexes to lock resource
extern xSemaphoreHandle xMutex;
extern xSemaphoreHandle xDDRO_Mutex;

#define xScanACK_BinarySemphr   ( xPrintACK_BinarySemphr )

extern void init_locks(void);

#endif
