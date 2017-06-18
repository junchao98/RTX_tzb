

#include "DS3231.h"

u8 test=0;



u8 decToBcd(u8 val) {
// Convert normal decimal numbers to binary coded decimal
	return ( (val/10*16) + (val%10) );
}

u8   bcdToDec(u8 val) {
// Convert binary coded decimal to normal decimal numbers
	return ( (val/16*10) + (val%16) );
}


DS3231_read(u8 addr)
{

	IIC_Start();
	IIC_Send_Byte(0xD0); //Ð´  Ð´µØÖ·
	IIC_Wait_Ack();
	SDelay_us(1);
	
	IIC_Send_Byte(addr);    //Ð´ Ð´Òª¶ÁµÄµØÖ·
	IIC_Wait_Ack();
	SDelay_us(1);


	IIC_Start();        //ÖØÆô£¡
	IIC_Send_Byte(0xD1); //Ð´ ¶Á µØÖ·
	IIC_Wait_Ack();
	SDelay_us(1);


	test=bcdToDec(IIC_Read_Byte(1));

	IIC_Stop();

return test;

}

DS3231_write(u8 addr , u8 data)
{

	IIC_Start();
	
	IIC_Send_Byte(0xD1); //Ð´  Ð´µØÖ·
	IIC_Wait_Ack();
	SDelay_us(1);
	
	IIC_Send_Byte(addr);    //Ð´ Ð´ÒªÐ´µÄµØÖ·
	IIC_Wait_Ack();
	SDelay_us(1);

	IIC_Send_Byte(data); //Ð´ Êý¾Ý
	IIC_Wait_Ack();
	SDelay_us(1);
	
	IIC_Stop();

return test;

}


void   DS3231_write_time( _ds3231_time  * ds3231_time)
{
          u8 tem=0;
	

       
	tem=decToBcd(ds3231_time->min);
        DS3231_write(0x01,tem);
	
	tem=decToBcd(ds3231_time->hour);
        DS3231_write(0x02,tem);
	
	tem=decToBcd(ds3231_time->day);
        DS3231_write(0x04,tem);
	
	tem=decToBcd(ds3231_time->moun);
        DS3231_write(0x05,tem);
	
	tem=decToBcd(ds3231_time->year);
        DS3231_write(0x06,tem);

	tem=decToBcd(ds3231_time->sec);
        DS3231_write(0x00,tem);

}

  void  DS3231_read_time( _ds3231_time  * ds3231_time)
{

         ds3231_time->sec= DS3231_read(0x00);

	 ds3231_time->min= DS3231_read(0x01);
	
	 ds3231_time->hour= DS3231_read(0x02);
	
	 ds3231_time->day= DS3231_read(0x04);
	
	 ds3231_time->moun= DS3231_read(0x05);
	
	 ds3231_time->year= DS3231_read(0x06);
	
}



void ds3231_hard(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

	  I2C_InitTypeDef  I2C_InitStructure; 

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
    
  /* PB6-I2C1_SCL?       PB7-I2C1_SDA*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // ????
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // ????
  GPIO_Init(GPIOB, &GPIO_InitStructure);	




  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;

  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 =0xaa; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

  I2C_InitStructure.I2C_ClockSpeed = 100000;

  I2C_Init(I2C1, &I2C_InitStructure);
  
  I2C_Cmd(I2C1, ENABLE); 

}





