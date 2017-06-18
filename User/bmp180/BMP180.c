

#include "BMP180.h"
#include <math.h> 
#define BMP180_addr  0xee

short AC1;
short AC2;
short AC3;
unsigned short AC4;
unsigned short AC5;
unsigned short AC6;
short B1;
short B2;
short MB;
short MC;
short MD;
 
float True_Temp=0;       //ÎÂ¶È
float True_Press=0;      //Pa
float True_Altitude=0;   //m


u16   IIC_Read_Calibeation(u8 reg_addr)
{

        u16 temp = 0;
	u16 temp2=0;

     
	IIC_Start();     //IIC start
	
	IIC_Send_Byte(BMP180_addr);   //slave address+W:0
	IIC_Wait_Ack();
	
	IIC_Send_Byte(reg_addr);      
	IIC_Wait_Ack();	
	
	IIC_Start();
	
	IIC_Send_Byte(BMP180_addr+1);         //slave address+R:1
	IIC_Wait_Ack();
	
	temp = IIC_Read_Byte(1);                     
	IIC_Wait_Ack();
	

	temp2= IIC_Read_Byte(0);                     
	IIC_Wait_Ack();
	
	IIC_Stop();
	
	temp=temp<<8;
	//printf("BMP_Calibeation_DATA= %d \n",temp);
	return  temp+temp2;

}



 void  Read_CalibrationData(void)
 {
 
     AC1 = IIC_Read_Calibeation(0xaa);
    AC2 = IIC_Read_Calibeation(0xac);
    AC3 = IIC_Read_Calibeation(0xae);
    AC4 = IIC_Read_Calibeation(0xb0);
    AC5 = IIC_Read_Calibeation(0xb2);
    AC6 = IIC_Read_Calibeation(0xb4);
    B1 = IIC_Read_Calibeation(0xb6);
    B2 = IIC_Read_Calibeation(0xb8);
    MB = IIC_Read_Calibeation(0xba);
    MC = IIC_Read_Calibeation(0xbc);
    MD = IIC_Read_Calibeation(0xbe);
 
 }


 long  Read_Old_temp(void)
 {
	 long tem=0;
	IIC_Write_One_Byte(BMP180_addr,0xf4,0x2e);
	SDelay_us(30000);
	 
	
	tem= IIC_Read_Calibeation(0xf6);
	 
	 printf("Old_temp=%d \n",tem);
	
	
	 return tem;
 }

 
 long  Read_Old_press(void)
 {
	  long tem=0;
 	IIC_Write_One_Byte(BMP180_addr,0xf4,0x34);
	SDelay_us(30000);
	 
	tem= IIC_Read_Calibeation(0xf6);
	 
	 tem &=0x0000FFFF;
	 printf("Old_press=%d \n",tem);
return tem;
 }
 
 
 
 void BMP_Read(u8 cmd)
{
	long X1,X2,X3,B3,B5,B6,B7,T,P;
	unsigned long B4;
	long UT=0;
	long UP=0;
	
	
	UT=Read_Old_temp();
	UP=Read_Old_press();

	Read_CalibrationData();
	
    X1 = ((UT-AC6)*AC5)>>15;      
    X2 = ((long)MC<<11)/(X1+MD);  
    B5 = X1+X2;                     
    T = (B5+8)>>4;                    
    True_Temp = T/10.0;          
 
    B6 = B5-4000;                     
    X1 = (B2*B6*B6)>>23;           
    X2 = (AC2*B6)>>11;            
    X3 = X1+X2;                      
    B3 = (((long)AC1*4+X3)+2)>>2;   
    X1 = (AC3*B6)>>13;              
    X2 = (B1*(B6*B6>>12))>>16;     
    X3 = ((X1+X2)+2)>>2;              
    B4 = AC4*(unsigned long)(X3+32768)>>15;   
    B7 = ((unsigned long)UP-B3)*50000;        
    if (B7 < 0x80000000)
    {
        P = (B7*2)/B4;  
    }
    else P=(B7/B4)*2;  
    
    X1 = (P>>8)*(P>>8);       
    X1 = (X1*3038)>>16;             
    X2 = (-7357*P)>>16;               
 P = P+((X1+X2+3791)>>4);    

    True_Press = P/1000;                    //@@@@@
  True_Altitude = 44330*(1-pow((P/101325.0),(1.0/5.255)));      
    
    printf("press=%.1f Pa \n",True_Press);
   printf("temp=%.1f \n",True_Temp);     
    printf("high=%d m \n",True_Altitude);
    
	}
