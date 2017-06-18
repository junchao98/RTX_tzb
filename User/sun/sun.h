

#ifndef  SUN

#define SUN
#define GY_30_ADDR  0x46

#include "stm32f10x.h"
#include <RTL.h> 
#include "myiic.h"
#include "time.h"

#include "set.h"

void sun_init(u8 cmd);


void  GY_30_read(u8 cmd);



#endif








