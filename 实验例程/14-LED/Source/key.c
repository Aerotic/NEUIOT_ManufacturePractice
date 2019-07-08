/*********************************************************************************************
* 文件：key.c
* 作者：chennian 2017.11.02
* 说明：K1 对应引脚 PB12
*       K2 对应管脚 PB13
*       K3 对应引脚 PB14
*       K4 对应管脚 PB15
* 功能：按键驱动代码
* 修改：zhoucj   2017.12.18 增加注释
*********************************************************************************************/
/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "key.h"

/*********************************************************************************************
* 名称：key_init
* 功能：按键管脚初始化
* 参数：无
* 返回：无
* 修改：无
*********************************************************************************************/
void key_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;                          //定义一个GPIO_InitTypeDef类型的结构体
  RCC_AHB1PeriphClockCmd( K1_CLK | K2_CLK
                         |K3_CLK | K4_CLK, ENABLE);             //开启KEY相关的GPIO外设时钟
  
  GPIO_InitStructure.GPIO_Pin = K1_PIN | K2_PIN |
                                K3_PIN | K4_PIN;                //选择要控制的GPIO引脚
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //设置引脚的输出类型为推挽输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                  //设置引脚模式为输入模式
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                  //设置引脚为上拉模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;              //设置引脚速率为2MHz
  
  GPIO_Init(K1_PORT, &GPIO_InitStructure);                      //初始化GPIOB配置
  GPIO_Init(K2_PORT, &GPIO_InitStructure);                      //初始化GPIOB配置
  GPIO_Init(K3_PORT, &GPIO_InitStructure);                      //初始化GPIOB配置
  GPIO_Init(K4_PORT, &GPIO_InitStructure);                      //初始化GPIOB配置
}

/*********************************************************************************************
* 名称：get_key_status
* 功能：按键管脚初始化
* 参数：无
* 返回：key_status
* 修改：
*********************************************************************************************/
char get_key_status(void)
{
  char key_status = 0;
  if(GPIO_ReadInputDataBit(K1_PORT,K1_PIN) == 0)                //判断PB12引脚电平状态
    key_status |= K1_PREESED;                                   //低电平key_status bit0位置1
  if(GPIO_ReadInputDataBit(K2_PORT,K2_PIN) == 0)                //判断PB13引脚电平状态
    key_status |= K2_PREESED;                                   //低电平key_status bit1位置1
  if(GPIO_ReadInputDataBit(K3_PORT,K3_PIN) == 0)                //判断PB14引脚电平状态
    key_status |= K3_PREESED;                                   //低电平key_status bit2位置1
  if(GPIO_ReadInputDataBit(K4_PORT,K4_PIN) == 0)                //判断PB15引脚电平状态
    key_status |= K4_PREESED;                                   //低电平key_status bit3位置1
  return key_status;
}
