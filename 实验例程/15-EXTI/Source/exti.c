/*********************************************************************************************
* 文件：exti.c
* 说明：
* 功能：外部中断驱动代码
* 修改：增加消抖操作
        zhoucj   2017.12.18 修改注释
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "exti.h"
#include "key.h"
#include "delay.h"
extern char led_status;  
/*********************************************************************************************
* 名称：exti_init
* 功能：外部中断初始化
* 参数：无
* 返回：无
* 修改：无
*********************************************************************************************/
void exti_init(void)
{
  key_init();                                                   //按键引脚初始化
  NVIC_InitTypeDef   NVIC_InitStructure;
  EXTI_InitTypeDef   EXTI_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);        //使能SYSCFG时钟
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource14); //PB14 连接到中断线14
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15); //PB15 连接到中断线15
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line14 | EXTI_Line15;     //LINE14、LINE15
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //下降沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能LINE14、LINE15
  EXTI_Init(&EXTI_InitStructure);                               //按上述参数配置
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;          //外部中断15-10
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     //抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            //子优先级1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);                               //按上述配置初始化
  
}
/*********************************************************************************************
* 名称：EXTI15_10_IRQHandler
* 功能：外部中断15-10中断处理函数
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void EXTI15_10_IRQHandler(void)
{
  if(get_key_status() == K3_PREESED){                           //检测K3被按下
    delay_count(500);                                           //延时消抖
    if(get_key_status() == K3_PREESED){                         //确认K3被按下
      while(get_key_status() == K3_PREESED);                    //等待按键松开
      led_status = !led_status;                                 //翻转led状态标志
    }
  }
  if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
    EXTI_ClearITPendingBit(EXTI_Line14);                        //清除LINE14上的中断标志位
  if(EXTI_GetITStatus(EXTI_Line15)!=RESET)
    EXTI_ClearITPendingBit(EXTI_Line15);                        //清除LINE15上的中断标志位
}
