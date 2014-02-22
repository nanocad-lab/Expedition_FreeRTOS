#include "gpio_set.h"
#include "mmap.h"

/* GPIO port initialization */
void init_gpio(void) {
    *pulGPIO_MASK = ~0;
    *pulGPIO_DIRECTION = ~0;
    *pulGPIO_DATA = 0;
}

void send_print_req(void) {
    // set direction as write
    *pulGPIO_DIRECTION |= (1U << PRINT_PORT);
    // generate an interrupt signal
    *pulGPIO_DATA |= (1U << PRINT_PORT);
    *pulGPIO_DATA &= (~(1U << PRINT_PORT));

}

void wait_print_ack(void) {
}
