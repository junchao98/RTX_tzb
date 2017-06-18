
#include "time.h"

   volatile u32  tim_num;
  

  
void my_time_init(u32  per)                 //选用time4通用定时器
{
	

	
        TIM_TimeBaseInitTypeDef   TIME_SET;
	NVIC_InitTypeDef                   NVIC_SET;
	
		printf("time init \n");
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);   //开启tim4时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	
	
        TIME_SET.TIM_ClockDivision=0x0 ;
	TIME_SET.TIM_CounterMode=TIM_CounterMode_Up  ;                            //向上计数
	TIME_SET.TIM_Period=per-1;                                                                        //1	次
	TIME_SET.TIM_Prescaler=71;                                                                   //72分频        T=1us
	TIM_InternalClockConfig(TIM4);
	TIM_TimeBaseInit(TIM4,&TIME_SET);

	//////////////////////////////1s 
	 TIME_SET.TIM_ClockDivision=0x0 ;
	TIME_SET.TIM_CounterMode=TIM_CounterMode_Up  ;                       //向上计数
	TIME_SET.TIM_Period=10000-1;                                                             //10000	次
	TIME_SET.TIM_Prescaler=7200-1;                                                          //7200分频        T=100us
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInit(TIM2,&TIME_SET);

	
	
	
	
	 TIM_ARRPreloadConfig(TIM4,ENABLE);                                             //使能自动重装
	TIM_ARRPreloadConfig(TIM2,ENABLE); 
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);                                            //使能中断
         TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);   
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_SET.NVIC_IRQChannel=TIM4_IRQn;                                                 //中断控制
	NVIC_SET.NVIC_IRQChannelCmd=ENABLE;
	NVIC_SET.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_SET.NVIC_IRQChannelSubPriority=1;
	
	NVIC_Init(&NVIC_SET);
	
	
	NVIC_SET.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_SET.NVIC_IRQChannel=TIM2_IRQn; 
	NVIC_Init(&NVIC_SET);
	
	
	TIM_Cmd(TIM4,DISABLE);
	
	TIM_Cmd(TIM2,DISABLE);
		 TIM4->CNT=1;
	printf("Time init  time4_T=10us  time3_T=1s\n");
}


void  SDelay_us(int  num)
{
	tim_num=num;
	TIM4->CNT=0;
	TIM_Cmd(TIM4,ENABLE);
	
       while(tim_num !=0){};
	
	TIM_Cmd(TIM4,DISABLE);
}










