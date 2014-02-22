#ifndef GPIO_SET_H_
#define GPIO_SET_H_

#define REQ_PIN  (0)
#define ACK_PIN   (1)

#define GPIO_MASK   ( (unsigned long *)0x44001008 )
#define GPIO_INT_BITS   ( (unsigned long *)0x4400100C )
#define GPIO_ACK_ENA    ( (unsigned long *)0x44001304 )
#define GPIO_REQ_DATA    ( (unsigned long *)0x44001100 )
#define GPIO_REQ_DIR    ( (unsigned long *)0x44001200 )
#define GPIO_ACK_DIR    ( (unsigned long *)0x44001204 )
#define GPIO_ACK_PRI    ( (unsigned char *)0xE000E401 )

#define IRQ_ENA         ( (unsigned long *)0xE000E100 )

extern void init_gpio(void);
extern void send_print_req(void);
extern void wait_print_ack(void);

#endif
