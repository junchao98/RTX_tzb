
#include "sun.h"
u8 GY_30_buff[4]={0};



void  _GY_30_write(u8 data)
{
	
         IIC_Start();
	
	IIC_Send_Byte(0x46);     //µØÖ·
	IIC_Wait_Ack();
	
	IIC_Send_Byte(data);
	IIC_Wait_Ack();


IIC_Stop();

}


void sun_init(u8 cmd)
{

_GY_30_write(0x01);      //¿ª»ú
_GY_30_write(0x10);

SDelay_us(18000);
	
	
}



void  GY_30_read(u8 cmd)
{

	U16 DATA=0;
	sun_init(0);
	
	os_dly_wait(10);
	IIC_Start();

	IIC_Send_Byte(0x47);
	IIC_Wait_Ack();
	

	GY_30_buff[0]=IIC_Read_Byte(1);
	IIC_Wait_Ack();
	SDelay_us(1);
	
	GY_30_buff[1]=IIC_Read_Byte(0);
	IIC_Wait_Ack();
	SDelay_us(1);
	
	IIC_Stop();

	DATA=(GY_30_buff[0]<<8 )+(GY_30_buff[1]);
	
	printf("sun=%.2f lx\n",DATA/1.2);
	
}















