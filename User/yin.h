

#include "stm32f10x.h"
#include  <RTL.h> 
#include  "stdio.h"
 #include  "stdlib.h"
#include   "sun.h"
#include "BMP180.H"

/*@�����ǵ���ģʽ����@ */

#define DBUG          //�д˺궨���ʾ���ڵ���ģʽ 
//////////////////////////////////////////////////////////////////////////


/* @�����Ǵ��ڵ������� */
#define USART_Pin_TX  GPIO_Pin_9
#define USART_Pin_RX  GPIO_Pin_10
#define USARTx  USART1

//////////////////////////////////////////////////////////////////////////


//void SysTick_Init(void)��

/*@������ϵͳ �Զ���Ĵ���*/
#define  store_data         volatile u32
#define  ex_store_data  extern volatile u32

//////////////////////////////////////////////////////////////////////////

//��״̬ �Ĵ���  dog��0��  super��1��  power_m��2��  get_data��3��   self_check��4��

/*@�����Ǵ���������*/
#define  SENSOR_KIND  3
#define DHT_SENSOR_NUM  2   //DHT���������� 


#define SEC   1   
#define MIN   60
#define HOUR  3600
#define DAY   HOUR*12

#define BUS 0         //����switch  ���      

#define DHT 1           ///���¶�������set_busy  ��һ����������ʾ�����ĸ�����
#define B_PRESS   2
#define SUNSHINE  3
#define  GPS_D             4
#define other 9

#define ALL_SENSOR_NUM    (DHT_SENSOR_NUM)

//////////////////////////////////////////////////////////////////////////

#define  ADC_REPAIR   3.08221


/*@���������ݽṹ��*/

 typedef struct   {    // ����ʱ�����  ����ʱ��ɼ� 

 u32 unit;  //��λ 
u32  time;

}__time_con;


 struct  __sen_operations{

	void (*sensor_init)(u8 cmd);
	void (*sensor_read)(u8 cmd);
	void(*sensor_ioctr)(u8 cmd); 
};



typedef  struct {

	struct __sen_operations * sen_operations;
	         
	u8 name;         //������id��
	
	u8  count;         //�Ӵ�������     Ϊ1ʱ���Ӵ�����

}__sensor_typ;



//////////////////////////////////////////////////////////////////////////




// ����5����Ӧ5������   //  �Լ� ����  ���Ź�  ��Դ����  data  
//  ����ʵ�ֺ���
void fun_dog(void);
void fun_super(void);
void fun_power_m(void);
void fun_get_data(u8 id,u8 cmd); 
void fun_self_check(void);

//void get_data_con(void) ;


/*����Ϊ��������*/


u8 set_busy(u16 *data ,u8 bit);  //  ��ָ���ļĴ��� ָ��λ �� 1 
u8 reset_busy(u16 *data,u8 bit);  //  ��ָ���ļĴ��� ָ��λ �� 0
 void   sensor_register(__sensor_typ  *sensor_typ,__time_con * time_con) ;        //������ע�ᴫ����
 void hard_set(void);
//////////////////////////////////////////////////////////////////////////
void USART_init(void);

__sensor_typ * mem_init(void);



