/*********************************************************************************************
* 文件：timer.c
* 作者：chennian 2017.11.2
* 说明：
* 功能：定时器驱动代码
* 修改：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "timer.h"

extern char led_status;  
/*********************************************************************************************
* 名称：timer3_init
* 功能：初始化TIM3
* 参数：period：自动重装值。 prescaler：时钟预分频数
* 返回：无
* 修改：
* 注释：定时器溢出时间计算方法:Tout=((period+1)*(prescaler+1))/Ft us.
*       AHB Prescaler = 1;AHB的时钟HCLK=SYSCLK/1 = 168MHz;
*       TIM3挂载在APB1上， APB1 Prescaler = 4，APB1的时钟频率PCLK1 = HCLK/4 = 42;Ft=2*PCLK1= 84MHz
*       Ft=定时器工作频率,单位:Mhz，
*********************************************************************************************/
void timer3_init(unsigned int period, unsigned short prescaler)//TIM_Period为16位的数
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;         //定时器配置
  	NVIC_InitTypeDef  NVIC_InitStructure;                   //中断配置
	 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
  	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;         //TIM3中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级0
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //使能中断
        
  	NVIC_Init(&NVIC_InitStructure);			        //按照上述配置初始化中断
	
  	TIM_TimeBaseStructure.TIM_Period = period;              //计数器重装值
  	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;        //预分频值
  	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分割
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);         //按上述配置初始化TIM3
        
        TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                //允许定时器3更新中断
  	TIM_Cmd(TIM3, ENABLE);                                  //使能TIM3
}


/*********************************************************************************************
* 名称：TIM3_IRQHandler
* 功能：TIM3中断处理函数
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update ) != RESET) {         //如果中断标志被设置
    	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);             //清除中断标志
          led_status = ~led_status;                             //LED灯状态标志位翻转
  	}
  	
}