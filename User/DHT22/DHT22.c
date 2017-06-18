

#include  "DHT22.h"





void DHT22_init (u8 cmd)
	

{
	GPIO_InitTypeDef GPIO_SET;
	
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);          //用PA11

	GPIO_SET.GPIO_Mode=GPIO_Mode_Out_PP;
	
	GPIO_SET.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_SET.GPIO_Pin=DHT22_GPIO_pin;
	
	GPIO_Init(GPIOA,&GPIO_SET);

	GPIOA->BSRR=DHT22_GPIO_pin;     //先拉高等待命令

     printf("DHT22_init \n");
   

}


void Pin_IPU(void)    // 上拉输入 释放数据线
{
	 GPIO_InitTypeDef GPIO_SET;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);        
	
	GPIO_SET.GPIO_Mode=GPIO_Mode_IPU;
	
	GPIO_SET.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_SET.GPIO_Pin=DHT22_GPIO_pin;
	
	GPIO_Init(GPIOA,&GPIO_SET);

}



u16  read(void)
{

  
	u16 i, temp=0;
	
// DHT22_check();
	
	for(i=0;i<16;i++)    
	{	 	
			/*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/  
		//SDelay_us(3);
			while(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin)==0);

			SDelay_us(4); //延时40 us 这个延时需要大于数据0持续的时间即可	   	  

			if(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin))/* x us后仍为高电平表示数据“1” */
			{	
				/* 等待数据1的高电平结束 */
				temp|=(0x01<<(15-i));  //把第7-i位置1，MSB先行 
			}

			
			  while(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin));
	}


return (temp);


}



u32  read_check(void)             //读取验校位
{

	u32 i, temp=0;
	
	for(i=0;i<8;i++)    
	{	 

			/*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/  
		        
			while(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin)==0);

			SDelay_us(4); //延时40 us 这个延时需要大于数据0持续的时间即可	   	  

			if(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin))/* x us后仍为高电平表示数据“1” */
			{
				temp  |=   (0x01<<(7-i));  //把第7-i位置1，MSB先行 
			}
			/* 等待数据1的高电平结束 */
			while(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin));  
	}


return (temp);


}



void  DHT22_read(u8 cmd)
{
                   
		DHT22_Data_TypeDef   DHT_DATA;

		printf("read  start \n");


		GPIOA->BRR=DHT22_GPIO_pin;
		SDelay_us(98);  //拉低1ms  发送准备信号
		GPIOA->BSRR=DHT22_GPIO_pin;  //拉高30us等待DHT22反应
		SDelay_us(3); 
		Pin_IPU();          //释放总线 上拉输入

	if(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin)==0)
	{
	
	        while ( !( GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin))  ){};   //DHT22发送 80us低 
			SDelay_us(6);
		while(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin)){};    // 80us高

		DHT_DATA.humi=read();
		DHT_DATA.temp=read();
		DHT_DATA.check_sum=read_check();
	
		if( (DHT_DATA.humi)/10<101 &&(DHT_DATA.temp/10)<81)
		{
			printf("%d#humi=%d.%d\n" ,point_id,(DHT_DATA.humi)/10,(DHT_DATA.humi)%10);
			printf("%d#temp=%d.%d\n" ,point_id,(DHT_DATA.temp)/10,(DHT_DATA.temp)%10);
			printf("sum= %d \n" ,DHT_DATA.check_sum );	

		}
		else
		{
		printf("sensor data error \n");
		}
		
			
	}	
	else
      {
		printf("read   error!\n");
      }

}

