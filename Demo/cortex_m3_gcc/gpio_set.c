#include "FreeRTOS.h"
#include "gpio_set.h"
#include "mmap.h"

/* GPIO port initialization */
void init_gpio(void) {
    volatile unsigned long *mask = GPIO_MASK;
    *mask = ~0UL;
    volatile unsigned long *req_dir = GPIO_REQ_DIR;
    *req_dir = 1UL;
    volatile unsigned long *req_data = GPIO_REQ_DATA;
    *req_data = 0UL;
    // enable ack interrupt
    volatile unsigned long *ack_dir = GPIO_ACK_DIR;
    *ack_dir = 0UL;
    volatile unsigned long *intr_bits = GPIO_INT_BITS;
    *intr_bits = 1UL << ACK_PIN;
    volatile unsigned long *ack_enable = GPIO_ACK_ENA;
    *ack_enable = 1UL;
    // set ack interrupt priority
    volatile unsigned char *ack_pri = GPIO_ACK_PRI;
    *ack_pri = configMAX_SYSCALL_INTERRUPT_PRIORITY;
    // enable IRQ1
    volatile unsigned long *IRQ1_enable = IRQ_ENA;
    *IRQ1_enable = 1UL << 1;
}

void send_print_req(void) {
    volatile unsigned long *req = GPIO_REQ_DATA;
    // generate an interrupt signal
    *req = 1UL;
    *req = 0UL;
}

void wait_print_ack(void) {
}
