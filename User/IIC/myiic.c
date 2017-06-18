

#include "myiic.h"
#include "time.h"
     
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//IIC驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;      //SCL  SDA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11); 	//PB10,PB11 输出高
}




//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	
	IIC_SDA_HIGH;
	
	Delay_us(1);	 
	
	IIC_SCL_HIGH;
	
	Delay_us(5);
 IIC_SDA_LOW;//START:when CLK is high,DATA change form high to low 
	Delay_us(5);
	IIC_SCL_LOW;//钳住I2C总线，准备发送或接收数据 
	Delay_us(2);
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	
  IIC_SCL_LOW;
	IIC_SDA_LOW;//STOP:when CLK is high DATA change form low to high
	
 	Delay_us(4);
  IIC_SCL_HIGH; 
	Delay_us(5);
	IIC_SDA_HIGH;//发送I2C总线结束信号
	Delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SCL_HIGH;Delay_us(1);	   
	IIC_SDA_HIGH;Delay_us(1);	 
	
	while(READ_SDA)
	{
	
	 
	
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	
	
	IIC_SCL_LOW;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL_LOW;
	SDA_OUT();
	IIC_SDA_LOW;
	Delay_us(2);
	IIC_SCL_HIGH;
	Delay_us(2);
	IIC_SCL_LOW;
}

//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL_LOW;
	
	SDA_OUT();
	IIC_SDA_HIGH;
	Delay_us(2);
	IIC_SCL_HIGH;
	Delay_us(2);
	IIC_SCL_LOW;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL_LOW;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA_HIGH;
		else
			IIC_SDA_LOW;
		txd<<=1; 	  
		Delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL_HIGH;
		Delay_us(2); 
		IIC_SCL_LOW;	
		Delay_us(2);
		Delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_LOW; 
        Delay_us(2);
		IIC_SCL_HIGH;
        receive<<=1;
        if(READ_SDA)receive++;   
		Delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}


u8 IIC_Read_One_Byte(u8 daddr,u8 reg_addr)
{
    u8 temp = 0;

     
	IIC_Start();     //IIC start
	
	IIC_Send_Byte(daddr);   //slave address+W:0
	IIC_Wait_Ack();
	
	IIC_Send_Byte(reg_addr);      
	IIC_Wait_Ack();	
	
	IIC_Start();
	
	IIC_Send_Byte(daddr|0x01);         //slave address+R:1
	IIC_Wait_Ack();
	
	temp = IIC_Read_Byte(1);                     
	IIC_Wait_Ack();
	
	IIC_Stop();

    
    return (temp);  

}



void IIC_Write_One_Byte(u8 daddr,u8 reg_addr,u8 data)
{
    IIC_Start();                       //IIC start
    IIC_Send_Byte(daddr);   //slave address+W:0
	IIC_Wait_Ack();
	
    IIC_Send_Byte(reg_addr);
	IIC_Wait_Ack();
	
    IIC_Send_Byte(data);
	IIC_Wait_Ack();
	
    IIC_Stop(); 



}


























