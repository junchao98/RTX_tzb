

#include "stm32f10x.h"
#include <RTL.h> 
#include "yin.h"
#include  "time.h"
#include  "myiic.h"
#include "DHT22.h"
#include "DS3231.H"
#include "sensor_data.h"
#include "set.h"
#include "sun.h"
#define SEM_NUM  1


//#include "bsp_SysTick.h"

         u8  point_id=0;
         u8  sensor_id=0;         //ȫ�ֱ�������¼�Ѿ�ע��Ĵ�������Ŀ

	__sensor_typ *sensor=NULL;        //�����������ṹ��
          __time_con   * sensor_time_con ; //�������ɼ�f
	u8 id =4;                               //������ȡ�ĸ�����������
	

	extern OS_SEM sem_busy;
	ex_store_data   bus_busy;    ///��״̬ �Ĵ���  dog��0��  super��1��  power_m��2��  get_data��3��   self_check��4��    ��λ����λ  �����б�ʾλ�� 

	ex_store_data   DHT_sensor_stat;  //�¶�ʪ�ȴ�����״̬ �Ĵ��� 

	ex_store_data  OTHER_sensor_stat;  //��ų�DHT22 ��Ĵ������û�  �Ĵ��� 
	ex_store_data  sensor_able;    //������ʹ�ܼĴ���


	OS_TID task_super;           // T5      
	OS_TID task_power_m;   //T4
	OS_TID task_getdata;   //T2
	OS_TID task_dog;       //T3
	OS_TID task_selfcheck;  //T4
	OS_TID task_clock_m;

	OS_ID timer=NULL;
	

__task void clock_m(void)
{
	#ifdef DBUG
	printf("clock_m  thread  creat !  1\n");
	#endif
	

	while(1)
	{
         os_dly_wait(100);
	
	GPIOB->ODR^=GPIO_Pin_6;
		
		//DHT22_read();
  
	}
}

__task void super(void)
{
	#ifdef DBUG
	    printf("super  thread  creat !  2\n");
	#endif

	while(1)
	{

         if(PRINT_ALL)  printf("get_data  heart = %d \n",heart);
		os_dly_wait(100);
	}

}


__task void power_m(void)       //��Դ����
{
	

	
	
	#ifdef DBUG
	printf("power_m  thread  creat !  3\n");
	#endif
	
	
	while(1)
	{
         
            fun_power_m();
		
		
		os_dly_wait(100);
            }
		
}




__task void get_data(void)     //���������ݶ���    ����һ  or  ������
{

	u8 i=0,j=0;
	u32 cache = heart;
	DHT22_Data_TypeDef  data;
	u32 next_time[SENSOR_KIND+1]={0};
		
	#ifdef DBUG
		printf("get_data  thread  creat !  4\n");
		printf("get_data  cache = %d \n",cache);
	#endif
 
	 os_dly_wait(150);
	
	for(i=0;i<SENSOR_KIND+1;i++)
	{
	next_time[i]=0;
	if(sensor[i].count!=0)next_time[i]=9;
	
	}
	

 
	do
	{
		
			for(i=1;i<SENSOR_KIND+1;i++)
			{
				 cache=heart;             //���������뻺��
				if(cache >= next_time[i])
				{
						if(next_time[i]>U32_MAX || heart <cache)        //next_time�������
						{
							next_time[i]=0;
							cache=0;
						}
				
					
					fun_get_data(i,0);                                                           //���ö�Ӧ����
					next_time[i] += ((sensor_time_con[i].unit) * (sensor_time_con[i].time));           //������һ��time
					//if(PRINT_ALL) printf("find id= %d next time %d \n",i,next_time[i]);
				}
			
		      }
		  
			os_dly_wait(90);
              
			
		}while(1);


}


__task void dog(void)      //���Ź�����
{
	#ifdef DBUG
	printf("dog  thread  creat !  5\n");
	#endif
	while(1)
	{
	IWDG_ReloadCounter();
	os_dly_wait(450);	
		
		if(PRINT_ALL)  printf("full dog\n");
      

	}

}




__task void self_check(void)   //���Ҽ��ģ��         ����һ  or  ������
{
	#ifdef DBUG
		int test=0;
		printf("self_check  thread  creat !  6\n");
	#endif


	while(1)
	{

		//Delay_ms(1000);
		os_dly_wait(100);
		//GPIOB->ODR ^= GPIO_Pin_8;    


  #ifdef DBUG
         printf("This is thread test  %d \n", test);	
		 test++;
   #endif		
   	
  
	}

}



__task void init (void)     //�����߳�
{



	//	int id=0;
	//hardware_init()    //�����Ǵ�������Ӳ����ʼ��
	#ifdef DBUG
		printf("sys star! \n");
	#endif

	os_sem_init(sem_busy,SEM_NUM);   //��ʼ���ź���  ��֤set_busy ��Ψһ����


		task_dog = os_tsk_create(dog,0);           //���Ź�ι���߳�

		task_selfcheck  = os_tsk_create(self_check,0); //  �����Լ� �߳�6

		task_clock_m=os_tsk_create(clock_m,0);        //ʱ�ӹ���1

		task_power_m =os_tsk_create(power_m,0);  // ������Դ����ģ��3

		task_getdata = os_tsk_create(get_data,0);  //  ��ȡ����������4

		task_super=os_tsk_create(super,0);  //����  __task void LEDS(void)    2
 
       TIM_Cmd(TIM2,ENABLE);          //��������
	     
	  os_tsk_delete_self ();   //��������

}








int  main(void)
{	
	// ��������main����֮ǰ�������ļ���statup_stm32f10x_hd.s�Ѿ�����
	// SystemInit()������ϵͳʱ�ӳ�ʼ����72MHZ
	// SystemInit()��system_stm32f10x.c�ж���
	// ����û����޸�ϵͳʱ�ӣ������б�д�����޸�
_ds3231_time  ds3231_time;
//	_ds3231_time  ds3231_time={
//	.year=2017,
//	.moun=2,
//	.day=14,
//	.hour=3,
//	.min=55,
//	.sec=0,
//	};

        ds3231_time.year=2017;
	ds3231_time.moun=2;
	ds3231_time.day=14;
	ds3231_time.hour=3;
	ds3231_time.min=55;
	ds3231_time.sec=0;
	
	
	//os_dly_wait(100);

	USART_init();     //����GPIO	

	printf("sensor addr = %x   \n", sensor);
	
	sensor = mem_init();      //�����ڴ棬���ؽṹ����
      
	 sensor_time_con =   malloc( sizeof( __time_con) * SENSOR_KIND);        //Ϊÿ��������ʱ�����ռ�
	 
	printf("sensor addr = %x   \n", sensor);

	printf("start_init  \n");

	sensor_register(sensor,sensor_time_con);

	my_time_init(10);       //10us  һ����ʱ���ж�

	hard_set();
	

	DHT22_init(0);

	IIC_Init();         //ģ��IIC

         point_id=1;

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);   //ȡ�����Ź��Ĵ�������
	IWDG_SetPrescaler(IWDG_Prescaler_256);    //���÷�Ƶ��
	IWDG_SetReload(1500);       //6s �ĸ�λ
	IWDG_ReloadCounter();
	IWDG_Enable();

	printf("dog !  run  now  \n");

	os_sys_init(init);  //ϵͳ��ʼ��������Ϊ������

	////  os_sys_init()   Ŀǰ���ԣ��Դ�while��(1)

	printf("#######error#######\n");
	//STM32 �Ķ������Ź����ڲ�ר�ŵ� 40Khz ����ʱ������
	


	return 0;
	
}

		

