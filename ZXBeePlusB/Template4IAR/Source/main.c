#include "stm32f4xx.h"

void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOE时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //选中 2、3 号引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //输出速度 2MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //无上下拉
  GPIO_Init(GPIOE, &GPIO_InitStructure); //根据上述参数配置GPIOE2、GPIOE3
}

void main(void)
{
  LED_Init(); //初始化LED
  GPIO_ResetBits(GPIOE, GPIO_Pin_2) ;//配置GPIOE2为低电平
  GPIO_ResetBits(GPIOE, GPIO_Pin_3) ;//配置GPIOE3为低电平
  while(1);//主循环
}
