

#include "myiic.h"
#include "time.h"
     
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;      //SCL  SDA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11); 	//PB10,PB11 �����
}




//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	
	IIC_SDA_HIGH;
	
	Delay_us(1);	 
	
	IIC_SCL_HIGH;
	
	Delay_us(5);
 IIC_SDA_LOW;//START:when CLK is high,DATA change form high to low 
	Delay_us(5);
	IIC_SCL_LOW;//ǯסI2C���ߣ�׼�����ͻ�������� 
	Delay_us(2);
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	
  IIC_SCL_LOW;
	IIC_SDA_LOW;//STOP:when CLK is high DATA change form low to high
	
 	Delay_us(4);
  IIC_SCL_HIGH; 
	Delay_us(5);
	IIC_SDA_HIGH;//����I2C���߽����ź�
	Delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	
	
	IIC_SCL_LOW;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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

//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL_LOW;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA_HIGH;
		else
			IIC_SDA_LOW;
		txd<<=1; 	  
		Delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_HIGH;
		Delay_us(2); 
		IIC_SCL_LOW;	
		Delay_us(2);
		Delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
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


























