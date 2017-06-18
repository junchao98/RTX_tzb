

#include "yin.h"
#include "DHT22.h"

//#include "bsp_SysTick.h"
////      总线bus  状态位
#define  DOG                                0    
#define  SUPER                           1
#define  POWER_M                     2
#define  GET_DATA                    3
#define  SELF_CHECK              4


#define get_id()   _id+=1

OS_SEM  sem_busy;   //声明一个信号量

extern __sensor_typ *sensor;


//struct  __time_con    my_time_con;     //时间结构体

u16 *p_cache1=NULL;      //
u16 *p_cache2=NULL;      //
volatile u16 bus_busy=0;    ///总状态 寄存器  dog（0）  super（1）  power_m（2）  get_data（3）   self_check（4）    低位到高位  括号中表示位数 

volatile u16 DHT_sensor_stat=0;  //温度湿度传感器状态 寄存器 

volatile u16 OTHER_sensor_stat=0;  //存放除DHT22 外的传感器好坏  寄存器 

store_data    sensor_able=0;      //传感器使能寄存器


u8  _id=0;

//  自检 采样  看门狗  电源管理  data      一共5个模块 对应5个处理函数 

/////////////////  结构体定义 
//struct   __time_con ST_time_con; 

/**************************************
//@注释  获取传感器数据
//@函数名 fun_get_data
//@参数
***************************************/
void  fun_get_data(u8 id,u8 cmd)           //传入
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
//@注释  
//@函数名 
//@参数
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
//@注释  
//@函数名 
//@参数
***************************************/
void fun_super(void)
{
set_busy(&bus_busy,SUPER);





reset_busy(&bus_busy,SUPER);
}


/**************************************
//@注释  
//@函数名 
//@参数
***************************************/

void fun_dog(void)
{
set_busy(&bus_busy,DOG);
	
	
reset_busy(&bus_busy,DOG);
}

/**************************************
//@注释     硬件状态检测
//@函数名 
//@参数
***************************************/
void fun_self_check(void)    //检查传感器应答 
{
	u32 i=0;
    u32 ret =0;   // ret  接受check 返回值 0为ok   1表示传感器异常 
set_busy(&bus_busy,SELF_CHECK);



//check_wireless();

 for(i=0;i< DHT_SENSOR_NUM ;i++ )      //温度湿度传感器应答检测 
 {
 // 传感器 check
 

 
  #ifdef  DBUG
   printf("sensor_stat = %d \n" ,DHT_sensor_stat);
  #endif
 }
 
 
 //check_gps();
 
 //check_b_press();

reset_busy(&bus_busy,SELF_CHECK);
}




///////////////////////////// 以下为子函数/////////////////////////

 /**************************************
//@注释  把指定位  置1 
//@函数名 set_busy
//@参数  (1) 指示写哪个寄存器  (2) 把指定位  置1 
***************************************/

u8 set_busy(u16 *data ,u8 bit)     //指定位 置1 
{
	
		u32 add=0;
		bit -= 1;  //  第二位 置1 只要把 add左移1 

		if(bit<0) goto error2;
	
		add= 1 << bit;
            
	
	if(data==p_cache1)
	{
	os_sem_wait(sem_busy,0xF);   //信号量
	*data |= add;  
	os_sem_send(sem_busy);  //返还信号量
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
 
printf("set_busy (a,b)  参数a错误 ");
#endif
//添加错误处理代码
return 1;

error2:
#ifdef  DBUG 
printf("set_busy (a,b)  bit 错误  #2");
#endif

//添加错误处理代码 
return 2;
	

}


 /**************************************
//@注释  把指定位  置0
//@函数名 set_busy
//@参数  (1) 指示写哪个寄存器  (2) 把指定位  置0
***************************************/

u8 reset_busy(u16 *data ,u8 bit)   //指定位 置0 
{

	u32 add=0xffff;
	bit -= 1;
	if(bit<0) goto error2;

	add ^= 1 << bit;
	
	
	if(p_cache2==data)
	{
	os_sem_wait(sem_busy,0xF);   //信号量
	*data &=add;
	os_sem_send(sem_busy);  //返还信号量	
	
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
	printf("reset_busy (a,b)  参数a错误 ");
	#endif
	//添加错误处理代码
	return 1;

	error2:
	#ifdef  DBUG 
	printf("reset_busy (a,b)  bit错误  #2");
	#endif
	//添加错误处理代码 
	return 2;

}




void get_time_con(__sensor_typ  *  sensor  ,u8  id)  //分析time结构体，执行采集 
{
set_busy(BUS,3); 

 //分析 传感器状态   //对非损坏传感器传达命令    ST_time_con  是存放采集频率的结构体 





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
	ADC_SET.ADC_NbrOfChannel=1;                 //一个通道
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

		sensor =   malloc( sizeof( __sensor_typ) *( SENSOR_KIND+1));        //为每个传感器分配空间
		//memset(sensor,0, sizeof( __sensor_typ) * ALL_SENSOR_NUM);
	
	
	for (num=0;num<SENSOR_KIND+1;num++)          //为每个传感器的操作函数分配空间
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



 
 






















