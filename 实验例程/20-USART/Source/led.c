/*********************************************************************************************
* 文件: led.c
* 作者：liutong 2015.8.19
* 说明：LED驱动程序
*       D1->PE0;D2->PE1;D3->PE2;D4->PE3;R->PB0;G->PB1;B->PB2
* 修改：chennian 2017.11.02 修改初始化引脚
*       chennian 2017.11.22 增加注释
* 注释：
*********************************************************************************************/
#include "led.h"
/*********************************************************************************************
* 名称：led_init
* 功能：初始化LED对应的GPIO
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void led_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;                    
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | 
                         RCC_AHB1Periph_GPIOB , ENABLE);        //使能GPIO时钟
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |
                                GPIO_Pin_2 | GPIO_Pin_3;        //选中引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                 //输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;              //输出速度2MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;              //无上下拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);                        //根据上述参数配置GPIOE0、GPIOE1、GPIOE2、GPIOE3
  GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 |
                      GPIO_Pin_2 | GPIO_Pin_3);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | 
                                GPIO_Pin_1 |
                                GPIO_Pin_2 ;                    //选中0、1、2号引脚
  GPIO_Init(GPIOB, &GPIO_InitStructure);                        //根据上述参数配置GPIOB0、GPIOB1、GPIOB2  
  GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);    //GPIOB0、GPIOB1、GPIOB2引脚置为高电平
}

/*********************************************************************************************
* 名称：turn_off
* 功能：置引脚高电平，关闭LED
* 参数：led
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void turn_off(unsigned char led){
  if(led & D1)                                                  //判断LED选择
    GPIO_SetBits(GPIOE, GPIO_Pin_0);                            //PE0置引脚高电平，关闭D1
  if(led & D2)
    GPIO_SetBits(GPIOE, GPIO_Pin_1);                            //PE1置引脚高电平，关闭D2
  if(led & D3)
    GPIO_SetBits(GPIOE, GPIO_Pin_2);                            //PE2置引脚高电平，关闭D3
  if(led & D4)
    GPIO_SetBits(GPIOE, GPIO_Pin_3);                            //PE3置引脚高电平，关闭D4
  if(led & LEDR)
    GPIO_SetBits(GPIOB, GPIO_Pin_0);                            //PB0置引脚高电平，关闭RGB灯的红灯
  if(led & LEDG)
    GPIO_SetBits(GPIOB, GPIO_Pin_1);                            //PB1置引脚高电平，关闭RGB灯的绿灯
  if(led & LEDB)
    GPIO_SetBits(GPIOB, GPIO_Pin_2);                            //PB2置引脚高电平，关闭RGB灯的蓝灯
}

/*********************************************************************************************
* 名称：turn_on
* 功能：置引脚低电平，打开LED
* 参数：led
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void turn_on(unsigned char led){
  if(led & D1)                                                  //判断LED选择
    GPIO_ResetBits(GPIOE, GPIO_Pin_0);                          //PE0置引脚低电平，打开D1
  if(led & D2)
    GPIO_ResetBits(GPIOE, GPIO_Pin_1);                          //PE1置引脚低电平，打开D2
  if(led & D3)
    GPIO_ResetBits(GPIOE, GPIO_Pin_2);                          //PE2置引脚低电平，打开D3
  if(led & D4)
    GPIO_ResetBits(GPIOE, GPIO_Pin_3);                          //PE3置引脚低电平，打开D4
  if(led & LEDR)
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);                          //PB0置引脚低电平，打开RGB灯的红灯
  if(led & LEDG)
    GPIO_ResetBits(GPIOB, GPIO_Pin_1);                          //PB1置引脚低电平，打开RGB灯的绿灯
  if(led & LEDB)
    GPIO_ResetBits(GPIOB, GPIO_Pin_2);                          //PB2置引脚低电平，打开RGB灯的蓝灯
}
/*********************************************************************************************
* 名称：get_led_status
* 功能：获取LED状态
* 参数：
* 返回：led_status--bit0-bit3分别表示4路LED灯的状态，bit4-bit6分别表示RGB灯的状态
* 修改：
* 注释：
*********************************************************************************************/
unsigned char get_led_status(void){
  unsigned char led_status = 0;
  if(GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_0))                 //判断PE0引脚电平
    led_status |= D1;                                           //高电平将led_status bit0置1
  else
    led_status &= ~D1;                                          //低电平将led_status bit0置0
  
  if(GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_1))                 //判断PE1引脚电平
    led_status |= D2;                                           //高电平将led_status bit1置1
  else
    led_status &= ~D2;                                          //低电平将led_status bit1置0
  
  if(GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_2))                 //判断PE2引脚电平
    led_status |= D3;                                           //高电平将led_status bit2置1
  else
    led_status &= ~D3;                                          //低电平将led_status bit2置0
  
  if(GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_3))                 //判断PE3引脚电平
    led_status |= D4;                                           //高电平将led_status bit3置1
  else
    led_status &= ~D4;                                          //低电平将led_status bit3置0
  
  if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0))                 //判断PB0引脚电平
    led_status |= LEDR;                                         //高电平将led_status bit4置1
  else
    led_status &= ~LEDR;                                        //低电平将led_status bit4置0
  
  if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1))                 //判断PB1引脚电平
    led_status |= LEDG;                                         //高电平将led_status bit5置1
  else
    led_status &= ~LEDG;                                        //低电平将led_status bit5置0
  
  if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_2))                 //判断PB2引脚电平
    led_status |= LEDB;                                         //高电平将led_status bit6置1
  else
    led_status &= ~LEDB;                                        //低电平将led_status bit6置0
  return led_status;                                            //返回led_status
}
