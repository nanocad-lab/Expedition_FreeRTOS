#include "basic_io.h"
#include "locks.h"

xSemaphoreHandle xPrintACK_BinarySemphr;
xSemaphoreHandle xMutex;

void init_locks(void) {
    vSemaphoreCreateBinary(xPrintACK_BinarySemphr);
    xMutex = xSemaphoreCreateMutex();
    xSemaphoreTake(xPrintACK_BinarySemphr, 0);
}
