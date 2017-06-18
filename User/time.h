
#ifndef TIME
#define TIME

#include "stm32f10x.h"
#include <RTL.h> 

  #define Delay_us(x) { unsigned int _dcnt; \
      _dcnt=(x*16); \
      while(_dcnt-- > 0) \
      { continue; }\
      }

      
 #define U32_MAX  900000000
      
void   my_time_init(u32  per);

void  SDelay_us(int  num);
extern  volatile  u32 heart;
      
#endif




