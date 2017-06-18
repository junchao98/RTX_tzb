


#ifndef SENSOR_DATA

#define SENSOR_DATA


typedef struct
{
	u32  humi;		//湿度的整数部分
	u32  temp;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
		                 
} DHT22_Data_TypeDef;











#endif












