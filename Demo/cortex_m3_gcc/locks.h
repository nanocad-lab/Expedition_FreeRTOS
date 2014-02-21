#ifndef LOCKS_H_
#define LOCKS_H_

#include "FreeRTOS.h"
#include "../../Source/include/semphr.h"

extern xSemaphoreHandle xBinarySemaphore;
extern xSemaphoreHandle xMutex;

#endif
