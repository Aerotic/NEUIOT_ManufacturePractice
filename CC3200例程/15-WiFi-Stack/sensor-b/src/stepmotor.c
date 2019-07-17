/*********************************************************************************************
* 文件：stepmotor.c
* 作者：zonesion 2016.4.13
* 说明：电机驱动程序
*       通过P0_0、P0_1端口控制电机的工作
* 修改：Chenkm 2017.01.10 修改代码格式，增加代码注释和文件说明
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_apps_rcm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stepmotor.h"
//#include "delay.h"

/*********************************************************************************************
* 宏定义
*********************************************************************************************/
 

/*********************************************************************************************
* 全局变量
*********************************************************************************************/
static unsigned int dir = 0;

void stepmotor_init(void)
{
  PRCMPeripheralClkEnable(PRCM_GPIOA1, PRCM_RUN_MODE_CLK);      //使能时钟
  PinTypeGPIO(PIN_04,PIN_MODE_0,0);                           //选择引脚为GPIO模式（gpio03）
  GPIODirModeSet(GPIOA1_BASE, G13_UCPINS, GPIO_DIR_MODE_OUT);   //设置GPIO14为输出模式
  
  
  PRCMPeripheralClkEnable(PRCM_GPIOA0, PRCM_RUN_MODE_CLK);      //使能时钟
  PinTypeGPIO(PIN_58,PIN_MODE_0,0);                           //选择引脚为GPIO模式（gpio16）
  GPIODirModeSet(GPIOA0_BASE, G03_UCPINS, GPIO_DIR_MODE_OUT);   //设置GPIO16为输出模式
  
  PRCMPeripheralClkEnable(PRCM_GPIOA1, PRCM_RUN_MODE_CLK);      //使能时钟
  PinTypeGPIO(PIN_02,PIN_MODE_0,0);                           //选择引脚为GPIO模式（gpio16）
  GPIODirModeSet(GPIOA1_BASE, G11_UCPINS, GPIO_DIR_MODE_OUT);   //设置GPIO16为输出模式
  
}

/*********************************************************************************************
* 名称：step(int dir,int steps)
* 功能：电机单步
* 参数：int dir,int steps
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void step(int dir,int steps)
{
  int i;
  if (dir) GPIOPinWrite(GPIOA0_BASE, pin_dir, 0xff);									//步进电机方向设置
  else GPIOPinWrite(GPIOA0_BASE, pin_dir, 0x00);
  SysCtlDelay(5*(g_ulClockRate/3000000)) ;		        //延时5us
  for (i=0; i<steps; i++){				        //步进电机旋转
   GPIOPinWrite(GPIOA1_BASE, pin_step, 0x00);
   SysCtlDelay(80*(g_ulClockRate/3000000)) ;
   GPIOPinWrite(GPIOA1_BASE, pin_step, 0xff);
   SysCtlDelay(80*(g_ulClockRate/3000000)) ;
  }
}

/*********************************************************************************************
* 名称：forward()
* 功能：电机正转
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void forward(int data)
{
  dir = 0;
  GPIOPinWrite(GPIOA1_BASE, pin_enable, 0x00);                  //步进电机方向设置
  step(dir, data);                              	        //启动步进电机
  GPIOPinWrite(GPIOA1_BASE, pin_enable, 0xff);

}

/*********************************************************************************************
* 名称：reversion()
* 功能：电机反转
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void reversion(int data)
{
  dir = 1;														//步进电机方向设置
  GPIOPinWrite(GPIOA1_BASE, pin_enable, 0x00);                  //步进电机方向设置
  step(dir, data);                              	        //启动步进电机
  GPIOPinWrite(GPIOA1_BASE, pin_enable, 0xff);
}