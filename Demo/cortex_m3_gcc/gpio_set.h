#ifndef GPIO_SET_H_
#define GPIO_SET_H_

#define PRINT_PORT  (0)
#define SCAN_PORT   (1)

extern void init_gpio(void);
extern void send_print_req(void);
extern void wait_print_ack(void);

#endif
