

#include "yin.h"
#include "DHT22.h"

//#include "bsp_SysTick.h"
////      ����bus  ״̬λ
#define  DOG                                0    
#define  SUPER                           1
#define  POWER_M                     2
#define  GET_DATA                    3
#define  SELF_CHECK              4


#define get_id()   _id+=1

OS_SEM  sem_busy;   //����һ���ź���

extern __sensor_typ *sensor;


//struct  __time_con    my_time_con;     //ʱ��ṹ��

u16 *p_cache1=NULL;      //
u16 *p_cache2=NULL;      //
volatile u16 bus_busy=0;    ///��״̬ �Ĵ���  dog��0��  super��1��  power_m��2��  get_data��3��   self_check��4��    ��λ����λ  �����б�ʾλ�� 

volatile u16 DHT_sensor_stat=0;  //�¶�ʪ�ȴ�����״̬ �Ĵ��� 

volatile u16 OTHER_sensor_stat=0;  //��ų�DHT22 ��Ĵ������û�  �Ĵ��� 

store_data    sensor_able=0;      //������ʹ�ܼĴ���


u8  _id=0;

//  �Լ� ����  ���Ź�  ��Դ����  data      һ��5��ģ�� ��Ӧ5�������� 

/////////////////  �ṹ�嶨�� 
//struct   __time_con ST_time_con; 

/**************************************
//@ע��  ��ȡ����������
//@������ fun_get_data
//@����
***************************************/
void  fun_get_data(u8 id,u8 cmd)           //����
{
                u8 i=0;
               set_busy(&bus_busy,GET_DATA);
           if(PRINT_ALL)  printf(" sensor[id].sen_operations <> BUS=%d \n" ,bus_busy);
	

	for(i=0;i<sensor[id].count;i++)
	{
	
	}
	
          sensor[id].sen_operations->sensor_read(cmd);
	

           reset_busy(&bus_busy,GET_DATA);
	if(PRINT_ALL)   printf(" sensor[id].sen_operations <> BUS=%d \n" ,bus_busy);
} 
/**************************************
//@ע��  
//@������ 
//@����
***************************************/

void fun_power_m(void)
{
		 float  power_adc,power_per;
	
              set_busy(&bus_busy,POWER_M);
	
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		os_dly_wait(1);
		power_adc=(ADC_GetConversionValue(ADC1)*3.3/0xfff)*ADC_REPAIR;     //*3+ADC_REPAIR
		power_per=(1-((7.4-power_adc)/0.8))*100;
		
		if(PRINT_ALL) printf("\npower_adc=%.1f \n",power_adc);
               printf("the power=%.1f%%\n",power_per);
		
		if(power_per<5)
		{
			printf("pwder_low");
		}


 reset_busy(&bus_busy,POWER_M);
}

/**************************************
//@ע��  
//@������ 
//@����
***************************************/
void fun_super(void)
{
set_busy(&bus_busy,SUPER);





reset_busy(&bus_busy,SUPER);
}


/**************************************
//@ע��  
//@������ 
//@����
***************************************/

void fun_dog(void)
{
set_busy(&bus_busy,DOG);
	
	
reset_busy(&bus_busy,DOG);
}

/**************************************
//@ע��     Ӳ��״̬���
//@������ 
//@����
***************************************/
void fun_self_check(void)    //��鴫����Ӧ�� 
{
	u32 i=0;
    u32 ret =0;   // ret  ����check ����ֵ 0Ϊok   1��ʾ�������쳣 
set_busy(&bus_busy,SELF_CHECK);



//check_wireless();

 for(i=0;i< DHT_SENSOR_NUM ;i++ )      //�¶�ʪ�ȴ�����Ӧ���� 
 {
 // ������ check
 

 
  #ifdef  DBUG
   printf("sensor_stat = %d \n" ,DHT_sensor_stat);
  #endif
 }
 
 
 //check_gps();
 
 //check_b_press();

reset_busy(&bus_busy,SELF_CHECK);
}




///////////////////////////// ����Ϊ�Ӻ���/////////////////////////

 /**************************************
//@ע��  ��ָ��λ  ��1 
//@������ set_busy
//@����  (1) ָʾд�ĸ��Ĵ���  (2) ��ָ��λ  ��1 
***************************************/

u8 set_busy(u16 *data ,u8 bit)     //ָ��λ ��1 
{
	
		u32 add=0;
		bit -= 1;  //  �ڶ�λ ��1 ֻҪ�� add����1 

		if(bit<0) goto error2;
	
		add= 1 << bit;
            
	
	if(data==p_cache1)
	{
	os_sem_wait(sem_busy,0xF);   //�ź���
	*data |= add;  
	os_sem_send(sem_busy);  //�����ź���
        }
	else{
	
	*data |= add;  
	
	}

	 p_cache1=data;
	
#ifdef  DBUG 


#endif



 return 0;

/******************************************/
error1:
#ifdef  DBUG 
 
printf("set_busy (a,b)  ����a���� ");
#endif
//��Ӵ��������
return 1;

error2:
#ifdef  DBUG 
printf("set_busy (a,b)  bit ����  #2");
#endif

//��Ӵ�������� 
return 2;
	

}


 /**************************************
//@ע��  ��ָ��λ  ��0
//@������ set_busy
//@����  (1) ָʾд�ĸ��Ĵ���  (2) ��ָ��λ  ��0
***************************************/

u8 reset_busy(u16 *data ,u8 bit)   //ָ��λ ��0 
{

	u32 add=0xffff;
	bit -= 1;
	if(bit<0) goto error2;

	add ^= 1 << bit;
	
	
	if(p_cache2==data)
	{
	os_sem_wait(sem_busy,0xF);   //�ź���
	*data &=add;
	os_sem_send(sem_busy);  //�����ź���	
	
	}
	else{
	
	*data &=add;
	
	}
       

       p_cache2=data;

	#ifdef  DBUG 
	#endif
	return 0;

	/******************************************/
	error1:
	#ifdef  DBUG 
	printf("reset_busy (a,b)  ����a���� ");
	#endif
	//��Ӵ��������
	return 1;

	error2:
	#ifdef  DBUG 
	printf("reset_busy (a,b)  bit����  #2");
	#endif
	//��Ӵ�������� 
	return 2;

}




void get_time_con(__sensor_typ  *  sensor  ,u8  id)  //����time�ṹ�壬ִ�вɼ� 
{
set_busy(BUS,3); 

 //���� ������״̬   //�Է��𻵴�������������    ST_time_con  �Ǵ�Ųɼ�Ƶ�ʵĽṹ�� 





reset_busy(BUS,3); 
}


void USART_init(void)
	
{


	 GPIO_InitTypeDef GPIO_SET;
	 USART_InitTypeDef USART_SET;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_SET.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_SET.GPIO_Pin=GPIO_Pin_6;
	GPIO_SET.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_SET);

	
	
	
	

		
	
	
	
	/* USART1 GPIO config */
	/* Set PA9 PA10 as UART1 */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_SET.GPIO_Pin = USART_Pin_TX;		
	GPIO_SET.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_SET.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_SET);
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_SET.GPIO_Pin =  USART_Pin_RX;
	GPIO_SET.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_SET);
	

	USART_SET.USART_BaudRate = 115200;
	USART_SET.USART_HardwareFlowControl=USART_HardwareFlowControl_None  ;
	USART_SET.USART_Mode =USART_Mode_Tx ;
	USART_SET.USART_Parity= USART_Parity_No ;
	USART_SET.USART_StopBits=USART_StopBits_1 ;
	USART_SET.USART_WordLength=USART_WordLength_8b  ;
	
	USART_Init(USARTx,&USART_SET);
	USART_Cmd(USARTx,ENABLE);
	
	
	
	printf(" Set PA9 PA10 as UART1  \n115200   USART_StopBits_1  USART_WordLength_8b \n ");
	
}



void hard_set(void)
{

	GPIO_InitTypeDef GPIO_SET;
	ADC_InitTypeDef  ADC_SET;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	
	GPIO_SET.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_SET.GPIO_Pin=GPIO_Pin_1;
	GPIO_SET.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_SET);

	ADC_SET.ADC_ContinuousConvMode=ENABLE;
	ADC_SET.ADC_DataAlign=ADC_DataAlign_Right ;
	ADC_SET.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_SET.ADC_Mode=ADC_Mode_Independent;
	ADC_SET.ADC_NbrOfChannel=1;                 //һ��ͨ��
	ADC_SET.ADC_ScanConvMode=DISABLE;
	
	ADC_Init(ADC1,&ADC_SET);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_239Cycles5);
        
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

         ADC_ResetCalibration(ADC1);
	 while(ADC_GetResetCalibrationStatus(ADC1));
	 
}

__sensor_typ * mem_init(void)
{
		int num=0;
	
		static  __sensor_typ  *  sensor;

		sensor =   malloc( sizeof( __sensor_typ) *( SENSOR_KIND+1));        //Ϊÿ������������ռ�
		//memset(sensor,0, sizeof( __sensor_typ) * ALL_SENSOR_NUM);
	
	
	for (num=0;num<SENSOR_KIND+1;num++)          //Ϊÿ���������Ĳ�����������ռ�
	{
			sensor[num].sen_operations=malloc( sizeof(struct  __sen_operations)   );
			//memset(sensor[num].sen_operations,0, sizeof( __sensor_typ) * ALL_SENSOR_NUM);
			sensor[num].count=0;
	}

	
     #ifdef DBUG
			printf("sen_op size = %d  \n" ,  sizeof( struct  __sen_operations));
			printf("sensor malloc  sizeof  =  %d \n",sizeof( __sensor_typ) * ALL_SENSOR_NUM);
	#endif
  
return   sensor;

}



     
     
    void  DHT22_register(__sensor_typ  *sensor_typ,__time_con * time_con)
    {		
	        u8 id = get_id();
	    
	        printf("DHT22_register \n");
		sensor_typ[id].name=DHT;
		sensor_typ[id].sen_operations->sensor_init=DHT22_init;
		sensor_typ[id].sen_operations->sensor_read=DHT22_read;
		sensor_typ[id].count=DHT_SENSOR_NUM;
		sensor_typ[id].sen_operations->sensor_ioctr=NULL;
	
	    time_con[id].unit=SEC;
	    time_con[id].time=2;
	    printf("DHT22_ID=%d \n", id);

	    
    }
    
     
        void  GY_30_register(__sensor_typ  *sensor_typ,__time_con * time_con)
    {		
	        u8 id = get_id();
	    
	        printf("GY_30_register \n");
		sensor_typ[id].name=SUNSHINE;
		sensor_typ[id].sen_operations->sensor_init=sun_init;
		sensor_typ[id].sen_operations->sensor_read=GY_30_read;
		sensor_typ[id].count=1;
		sensor_typ[id].sen_operations->sensor_ioctr=NULL;
	
	    time_con[id].unit=SEC;
	    time_con[id].time=4;
	    printf("GY_30_ID=%d \n", id);

	    
    }
     
            void  BMP180_register(__sensor_typ  *sensor_typ,__time_con * time_con)
    {		
	        u8 id = get_id();
	    
	        printf("BMP180_register \n");
		sensor_typ[id].name=B_PRESS;
	   
		sensor_typ[id].sen_operations->sensor_init=NULL;
		sensor_typ[id].sen_operations->sensor_read=BMP_Read;
		sensor_typ[id].count=1;
		sensor_typ[id].sen_operations->sensor_ioctr=NULL;
	
	    time_con[id].unit=SEC;
	    time_con[id].time=3;
	    printf("BMP180_register=%d \n", id);

	    
    }
     
 void   sensor_register(__sensor_typ  *sensor_typ,__time_con * time_con)
 {
 
	 DHT22_register(sensor_typ,time_con);
          GY_30_register(sensor_typ,time_con);
          BMP180_register(sensor_typ,time_con);
 }



 
 






















