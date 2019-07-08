/*********************************************************************************************
* 文件：key.c
* 作者：Liyw 2017.5.30
* 说明：K1 对应引脚 PB12
*       K2 对应管脚 PB13
*       K3 对应引脚 PB14
*       K4 对应管脚 PB15
* 功能：按键驱动代码
* 修改：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "key.h"

/*********************************************************************************************
* 名称：key_init
* 功能：按键初始化
* 参数：无
* 返回：无
* 修改：无
*********************************************************************************************/
void key_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;                          //定义一个GPIO_InitTypeKef类型的结构体
  RCC_AHB1PeriphClockCmd(KEY_CLK, ENABLE);                      //开启KEY相关的GPIO外设时钟
  
  GPIO_InitStructure.GPIO_Pin   = K1_PIN | K2_PIN | 
                                  K3_PIN | K4_PIN;              //选择要控制的GPIO引脚
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //设置引脚的输出类型为推挽
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;                 //设置引脚模式为输入模式
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;                 //设置引脚为上拉模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;              //设置引脚速率为2MHz
  
  GPIO_Init(KEY_PORT, &GPIO_InitStructure);                     //初始化GPIO配置
}

/*********************************************************************************************
* 名称：key_gpio_init
* 功能：按键管脚初始化
* 参数：无
* 返回：无
* 修改：无
*********************************************************************************************/
signed char key_status(char num)
{
  if(num == K1)
    return GPIO_ReadInputDataBit(KEY_PORT,K1_PIN);              //返回K1的状态

  if(num == K2)
    return GPIO_ReadInputDataBit(KEY_PORT,K2_PIN);              //返回K2的状态
  
  if(num == K3)
    return GPIO_ReadInputDataBit(KEY_PORT,K3_PIN);              //返回K3的状态
  
  if(num == K4)
    return GPIO_ReadInputDataBit(KEY_PORT,K4_PIN);              //返回K4的状态
  
  return -1;
}
