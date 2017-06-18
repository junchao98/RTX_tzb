

#ifndef  DS3231
#define DS3231

#include "stm32f10x.h"
#include <RTL.h> 



u16 B_BCD(u8 val);


u8 BCD2HEX(u8 val);


DS3231_read(u8 addr);


typedef  struct  {

	u8  year;
	u8  moun;
	u8  day;
	u8  hour;
	u8  min;
	u8  sec;
}_ds3231_time;


void   DS3231_write_time( _ds3231_time  * ds3231_time);
  void  DS3231_read_time( _ds3231_time  * ds3231_time);
void ds3231_hard(void);
#endif













