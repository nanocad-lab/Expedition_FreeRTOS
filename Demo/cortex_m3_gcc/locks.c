#include "basic_io.h"
#include "locks.h"

xSemaphoreHandle xPrintACK_BinarySemphr;
xSemaphoreHandle xDDRO_BinarySemphr;

xSemaphoreHandle xMutex;
xSemaphoreHandle xDDRO_Mutex;

void init_locks(void) {
    // init binary semaphores
    vSemaphoreCreateBinary(xPrintACK_BinarySemphr);
    xSemaphoreTake(xPrintACK_BinarySemphr, 0);
    
    vSemaphoreCreateBinary(xDDRO_BinarySemphr);
    xSemaphoreTake(xDDRO_BinarySemphr, 0);

    // init mutexes
    xMutex = xSemaphoreCreateMutex();
    xDDRO_Mutex = xSemaphoreCreateMutex();
}
