

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
         u8  sensor_id=0;         //全局变量，记录已经注册的传感器数目

	__sensor_typ *sensor=NULL;        //传感器描述结构体
          __time_con   * sensor_time_con ; //传感器采集f
	u8 id =4;                               //决定获取哪个传感器数据
	

	extern OS_SEM sem_busy;
	ex_store_data   bus_busy;    ///总状态 寄存器  dog（0）  super（1）  power_m（2）  get_data（3）   self_check（4）    低位到高位  括号中表示位数 

	ex_store_data   DHT_sensor_stat;  //温度湿度传感器状态 寄存器 

	ex_store_data  OTHER_sensor_stat;  //存放除DHT22 外的传感器好坏  寄存器 
	ex_store_data  sensor_able;    //传感器使能寄存器


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


__task void power_m(void)       //电源管理
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




__task void get_data(void)     //传感器数据读回    王天一  or  马万里
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
				 cache=heart;             //把心跳放入缓冲
				if(cache >= next_time[i])
				{
						if(next_time[i]>U32_MAX || heart <cache)        //next_time溢出处理
						{
							next_time[i]=0;
							cache=0;
						}
				
					
					fun_get_data(i,0);                                                           //调用对应函数
					next_time[i] += ((sensor_time_con[i].unit) * (sensor_time_con[i].time));           //计算下一次time
					//if(PRINT_ALL) printf("find id= %d next time %d \n",i,next_time[i]);
				}
			
		      }
		  
			os_dly_wait(90);
              
			
		}while(1);


}


__task void dog(void)      //看门狗进程
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




__task void self_check(void)   //自我检测模块         王天一  or  马万里
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



__task void init (void)     //创建线程
{



	//	int id=0;
	//hardware_init()    //这里是传感器等硬件初始化
	#ifdef DBUG
		printf("sys star! \n");
	#endif

	os_sem_init(sem_busy,SEM_NUM);   //初始化信号量  保证set_busy 的唯一访问


		task_dog = os_tsk_create(dog,0);           //看门狗喂狗线程

		task_selfcheck  = os_tsk_create(self_check,0); //  周期自检 线程6

		task_clock_m=os_tsk_create(clock_m,0);        //时钟管理1

		task_power_m =os_tsk_create(power_m,0);  // 创建电源管理模块3

		task_getdata = os_tsk_create(get_data,0);  //  获取传感器数据4

		task_super=os_tsk_create(super,0);  //创建  __task void LEDS(void)    2
 
       TIM_Cmd(TIM2,ENABLE);          //心跳启动
	     
	  os_tsk_delete_self ();   //销毁自身

}








int  main(void)
{	
	// 程序来到main函数之前，启动文件：statup_stm32f10x_hd.s已经调用
	// SystemInit()函数把系统时钟初始化成72MHZ
	// SystemInit()在system_stm32f10x.c中定义
	// 如果用户想修改系统时钟，可自行编写程序修改
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

	USART_init();     //配置GPIO	

	printf("sensor addr = %x   \n", sensor);
	
	sensor = mem_init();      //分配内存，返回结构体句柄
      
	 sensor_time_con =   malloc( sizeof( __time_con) * SENSOR_KIND);        //为每个传感器时间分配空间
	 
	printf("sensor addr = %x   \n", sensor);

	printf("start_init  \n");

	sensor_register(sensor,sensor_time_con);

	my_time_init(10);       //10us  一个定时器中断

	hard_set();
	

	DHT22_init(0);

	IIC_Init();         //模拟IIC

         point_id=1;

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);   //取消看门狗寄存器保护
	IWDG_SetPrescaler(IWDG_Prescaler_256);    //设置分频数
	IWDG_SetReload(1500);       //6s 的复位
	IWDG_ReloadCounter();
	IWDG_Enable();

	printf("dog !  run  now  \n");

	os_sys_init(init);  //系统初始化，参数为函数名

	////  os_sys_init()   目前测试，自带while（(1)

	printf("#######error#######\n");
	//STM32 的独立看门狗由内部专门的 40Khz 低速时钟驱动
	


	return 0;
	
}

		

