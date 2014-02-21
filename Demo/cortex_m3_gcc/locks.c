#include "locks.h"

xSemaphoreHandle xBinarySemaphore;
xSemaphoreHandle xMutex;

void vInterruptHandler(void) {
    portBASE_TYPE xHigherPriorityTasksWoken = pdFALSE;
    xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTasksWoken);
    portEND_SWITCHING_ISR(xHigherPriorityTasksWoken);
}
