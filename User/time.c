
#include "time.h"

   volatile u32  tim_num;
  

  
void my_time_init(u32  per)                 //ѡ��time4ͨ�ö�ʱ��
{
	

	
        TIM_TimeBaseInitTypeDef   TIME_SET;
	NVIC_InitTypeDef                   NVIC_SET;
	
		printf("time init \n");
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);   //����tim4ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	
	
        TIME_SET.TIM_ClockDivision=0x0 ;
	TIME_SET.TIM_CounterMode=TIM_CounterMode_Up  ;                            //���ϼ���
	TIME_SET.TIM_Period=per-1;                                                                        //1	��
	TIME_SET.TIM_Prescaler=71;                                                                   //72��Ƶ        T=1us
	TIM_InternalClockConfig(TIM4);
	TIM_TimeBaseInit(TIM4,&TIME_SET);

	//////////////////////////////1s 
	 TIME_SET.TIM_ClockDivision=0x0 ;
	TIME_SET.TIM_CounterMode=TIM_CounterMode_Up  ;                       //���ϼ���
	TIME_SET.TIM_Period=10000-1;                                                             //10000	��
	TIME_SET.TIM_Prescaler=7200-1;                                                          //7200��Ƶ        T=100us
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInit(TIM2,&TIME_SET);

	
	
	
	
	 TIM_ARRPreloadConfig(TIM4,ENABLE);                                             //ʹ���Զ���װ
	TIM_ARRPreloadConfig(TIM2,ENABLE); 
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);                                            //ʹ���ж�
         TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);   
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_SET.NVIC_IRQChannel=TIM4_IRQn;                                                 //�жϿ���
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










