

#include "stm32f10x.h"
#include  <RTL.h> 
#include  "stdio.h"
 #include  "stdlib.h"
#include   "sun.h"
#include "BMP180.H"

/*@以下是调试模式开关@ */

#define DBUG          //有此宏定义表示处于调试模式 
//////////////////////////////////////////////////////////////////////////


/* @以下是串口调试配置 */
#define USART_Pin_TX  GPIO_Pin_9
#define USART_Pin_RX  GPIO_Pin_10
#define USARTx  USART1

//////////////////////////////////////////////////////////////////////////


//void SysTick_Init(void)；

/*@以下是系统 自定义寄存器*/
#define  store_data         volatile u32
#define  ex_store_data  extern volatile u32

//////////////////////////////////////////////////////////////////////////

//总状态 寄存器  dog（0）  super（1）  power_m（2）  get_data（3）   self_check（4）

/*@以下是传感器配置*/
#define  SENSOR_KIND  3
#define DHT_SENSOR_NUM  2   //DHT传感器数量 


#define SEC   1   
#define MIN   60
#define HOUR  3600
#define DAY   HOUR*12

#define BUS 0         //用于switch  语句      

#define DHT 1           ///以下定义用于set_busy  第一个参数，表示操作哪个变量
#define B_PRESS   2
#define SUNSHINE  3
#define  GPS_D             4
#define other 9

#define ALL_SENSOR_NUM    (DHT_SENSOR_NUM)

//////////////////////////////////////////////////////////////////////////

#define  ADC_REPAIR   3.08221


/*@以下是数据结构体*/

 typedef struct   {    // 保存时间变量  多少时间采集 

 u32 unit;  //单位 
u32  time;

}__time_con;


 struct  __sen_operations{

	void (*sensor_init)(u8 cmd);
	void (*sensor_read)(u8 cmd);
	void(*sensor_ioctr)(u8 cmd); 
};



typedef  struct {

	struct __sen_operations * sen_operations;
	         
	u8 name;         //传感器id号
	
	u8  count;         //子传感器数     为1时无子传感器

}__sensor_typ;



//////////////////////////////////////////////////////////////////////////




// 这里5个对应5个处理   //  自检 采样  看门狗  电源管理  data  
//  核心实现函数
void fun_dog(void);
void fun_super(void);
void fun_power_m(void);
void fun_get_data(u8 id,u8 cmd); 
void fun_self_check(void);

//void get_data_con(void) ;


/*以下为函数调用*/


u8 set_busy(u16 *data ,u8 bit);  //  把指定的寄存器 指定位 置 1 
u8 reset_busy(u16 *data,u8 bit);  //  把指定的寄存器 指定位 置 0
 void   sensor_register(__sensor_typ  *sensor_typ,__time_con * time_con) ;        //这里来注册传感器
 void hard_set(void);
//////////////////////////////////////////////////////////////////////////
void USART_init(void);

__sensor_typ * mem_init(void);



