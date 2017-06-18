


#ifndef  __DH22

#define  __DH22



#include "stm32f10x.h"
#include "time.h"
#include "sensor_data.h"
#include "set.h"

#define DHT22_GPIO_pin GPIO_Pin_12         //µ¥×ÜÏß



void DHT22_init (u8 cmd);
void  DHT22_read(u8 cmd);





#endif











