/*********************************************************************************************
* 文件：grating.c
* 作者：Lixm 2017.10.17
* 说明：红外光栅驱动代码
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "hw_types.h"
#include "grating.h"

/*********************************************************************************************
* 名称：grating_init()
* 功能：红外光栅传感器初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void grating_init(void)
{
  PRCMPeripheralClkEnable(PRCM_GPIOA0, PRCM_RUN_MODE_CLK); //使能时钟
  PinTypeGPIO(PIN_59,PIN_MODE_0,false);                    //选择引脚为GPIO模式（gpio16）
  GPIODirModeSet(GPIOA0_BASE, G04_UCPINS, GPIO_DIR_MODE_OUT);   //不能直接设置为输入，对adc有影响？
  //PinConfigSet(PIN_59,PIN_TYPE_STD_PD,PIN_MODE_0);         //下拉
  GPIOPinWrite(GPIOA0_BASE,G04_UCPINS, 0xff);
}

/*********************************************************************************************
* 名称：unsigned char get_grating_status(void)
* 功能：获取红外光栅传感器状态
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
unsigned char get_grating_status(void)
{ 
  int r;
  GPIODirModeSet(GPIOA0_BASE, G04_UCPINS, GPIO_DIR_MODE_IN);
  r = GPIOPinRead(GPIOA0_BASE,G04_UCPINS);
  GPIODirModeSet(GPIOA0_BASE, G04_UCPINS, GPIO_DIR_MODE_OUT);

  if(r > 0)                                                      
    return 1;                                                   //检测到信号返回 1
  else
    return 0;                                                   //没有检测到信号返回 0
}