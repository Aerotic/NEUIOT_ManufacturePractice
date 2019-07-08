/*********************************************************************************************
* 文件：led.c
* 作者：Liyw 2017.5.30
* 说明：D1 对应引脚 PE0
*       D2 对应管脚 PE1
*       D3 对应引脚 PE2
*       D4 对应管脚 PE3
* 功能：led灯驱动代码
* 修改：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "led.h"

/*********************************************************************************************
* 名称：led_init
* 功能：led灯初始化
* 参数：无
* 返回：无
* 修改：无
*********************************************************************************************/
void led_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;                          //定义一个GPIO_InitTypeDef类型的结构体
  RCC_AHB1PeriphClockCmd(LED_CLK, ENABLE);                      //开启LED相关的GPIO外设时钟
  
  GPIO_InitStructure.GPIO_Pin = D1_PIN | D2_PIN | 
                                D3_PIN | D4_PIN;                //选择要控制的GPIO引脚
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //设置引脚的输出类型为推挽输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                 //设置引脚模式为输出模式
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                  //设置引脚为上拉模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;              //设置引脚速率为2MHz
  
  GPIO_Init(LED_PORT, &GPIO_InitStructure);                     //初始化GPIO配置
  GPIO_SetBits(LED_PORT,D1_PIN | D2_PIN | 
                        D3_PIN | D4_PIN);                       //配置LED_D4初始状态为关闭
}

/*********************************************************************************************
* 名称：led_control
* 功能：led灯控制
* 参数：num------led灯编号,支持多位同时控制
* 返回：无
* 修改：无
*********************************************************************************************/
void led_control(char num)
{
  if(num & D1)
    GPIO_ResetBits(LED_PORT,D1_PIN);                            //配置D1为开启
  else
    GPIO_SetBits(LED_PORT,D1_PIN);                              //配置D1为关闭
  
  if(num & D2)
    GPIO_ResetBits(LED_PORT,D2_PIN);                            //配置D2为开启
  else
    GPIO_SetBits(LED_PORT,D2_PIN);                              //配置D2为关闭
  
  if(num & D3)
    GPIO_ResetBits(LED_PORT,D3_PIN);                            //配置D3为开启
  else
    GPIO_SetBits(LED_PORT,D3_PIN);                              //配置D3为关闭
  
  if(num & D4)
    GPIO_ResetBits(LED_PORT,D4_PIN);                            //配置D4为开启
  else
    GPIO_SetBits(LED_PORT,D4_PIN);                              //配置D4为关闭
}

/*********************************************************************************************
* 名称：led_status
* 功能：led灯状态查询
* 参数：num------led灯编号
* 返回：对应LED状态，ON为开启，OFF为关闭，-1比表示参数异常
* 修改：无
*********************************************************************************************/
signed char led_status(char num)
{
  if(num == D1)
    return GPIO_ReadOutputDataBit(LED_PORT,D1_PIN);             //返回D1的状态

  if(num == D2)
    return GPIO_ReadOutputDataBit(LED_PORT,D2_PIN);             //返回D2的状态
  
  if(num == D3)
    return GPIO_ReadOutputDataBit(LED_PORT,D3_PIN);             //返回D3的状态
  
  if(num == D4)
    return GPIO_ReadOutputDataBit(LED_PORT,D4_PIN);             //返回D4的状态
  
  return -1;
}

