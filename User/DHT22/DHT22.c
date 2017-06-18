

#include  "DHT22.h"





void DHT22_init (u8 cmd)
	

{
	GPIO_InitTypeDef GPIO_SET;
	
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);          //��PA11

	GPIO_SET.GPIO_Mode=GPIO_Mode_Out_PP;
	
	GPIO_SET.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_SET.GPIO_Pin=DHT22_GPIO_pin;
	
	GPIO_Init(GPIOA,&GPIO_SET);

	GPIOA->BSRR=DHT22_GPIO_pin;     //�����ߵȴ�����

     printf("DHT22_init \n");
   

}


void Pin_IPU(void)    // �������� �ͷ�������
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
			/*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����*/  
		//SDelay_us(3);
			while(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin)==0);

			SDelay_us(4); //��ʱ40 us �����ʱ��Ҫ��������0������ʱ�伴��	   	  

			if(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin))/* x us����Ϊ�ߵ�ƽ��ʾ���ݡ�1�� */
			{	
				/* �ȴ�����1�ĸߵ�ƽ���� */
				temp|=(0x01<<(15-i));  //�ѵ�7-iλ��1��MSB���� 
			}

			
			  while(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin));
	}


return (temp);


}



u32  read_check(void)             //��ȡ��Уλ
{

	u32 i, temp=0;
	
	for(i=0;i<8;i++)    
	{	 

			/*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����*/  
		        
			while(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin)==0);

			SDelay_us(4); //��ʱ40 us �����ʱ��Ҫ��������0������ʱ�伴��	   	  

			if(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin))/* x us����Ϊ�ߵ�ƽ��ʾ���ݡ�1�� */
			{
				temp  |=   (0x01<<(7-i));  //�ѵ�7-iλ��1��MSB���� 
			}
			/* �ȴ�����1�ĸߵ�ƽ���� */
			while(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin));  
	}


return (temp);


}



void  DHT22_read(u8 cmd)
{
                   
		DHT22_Data_TypeDef   DHT_DATA;

		printf("read  start \n");


		GPIOA->BRR=DHT22_GPIO_pin;
		SDelay_us(98);  //����1ms  ����׼���ź�
		GPIOA->BSRR=DHT22_GPIO_pin;  //����30us�ȴ�DHT22��Ӧ
		SDelay_us(3); 
		Pin_IPU();          //�ͷ����� ��������

	if(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin)==0)
	{
	
	        while ( !( GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin))  ){};   //DHT22���� 80us�� 
			SDelay_us(6);
		while(GPIO_ReadInputDataBit(GPIOA,DHT22_GPIO_pin)){};    // 80us��

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

