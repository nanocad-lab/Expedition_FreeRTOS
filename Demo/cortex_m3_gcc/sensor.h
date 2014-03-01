#ifndef SENSOR_H_
#define SENSOR_H_

#define HCLK            ( (char)0x0 )
#define RING_OSC_HCLK   ( (char)0x1 )
#define PLL_REFCLK      ( (char)0x2 )
#define SYN_DDRO        ( (char)0x3 )
#define INV_DDRO        ( (char)0xC )
#define OXIDE_CLK       ( (char)0x16 )
#define PMOS_CLK        ( (char)0x17 )
#define NMOS_CLK        ( (char)0x18 )
#define TEMP_CLK        ( (char)0x19 )

// init_sensor: enable ddro_done interrupt
extern void init_sensor(void);
extern long sensor_read(char type, char id);

#endif
