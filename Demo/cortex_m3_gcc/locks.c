#include "basic_io.h"
#include "locks.h"

xSemaphoreHandle xPrintACK_BinarySemphr;
xSemaphoreHandle xScanACK_BinarySemphr;
xSemaphoreHandle xDDRO_BinarySemphr;

xSemaphoreHandle xPrint_Mutex;
xSemaphoreHandle xScan_Mutex;
xSemaphoreHandle xDDRO_Mutex;
xSemaphoreHandle xREQ_Mutex;

void init_locks(void) {
    // init binary semaphores
    vSemaphoreCreateBinary(xPrintACK_BinarySemphr);
    xSemaphoreTake(xPrintACK_BinarySemphr, 0);

    vSemaphoreCreateBinary(xScanACK_BinarySemphr);
    xSemaphoreTake(xScanACK_BinarySemphr, 0);
    
    vSemaphoreCreateBinary(xDDRO_BinarySemphr);
    xSemaphoreTake(xDDRO_BinarySemphr, 0);

    // init mutexes
    xPrint_Mutex = xSemaphoreCreateMutex();
    xScan_Mutex = xSemaphoreCreateMutex();
    xDDRO_Mutex = xSemaphoreCreateMutex();
    xREQ_Mutex = xSemaphoreCreateMutex();
}
