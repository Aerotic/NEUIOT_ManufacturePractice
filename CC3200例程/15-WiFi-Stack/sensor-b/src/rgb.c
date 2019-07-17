/*********************************************************************************************
* 文件：rgb.c
* 作者：Meixin 2017.09.15
* 说明：rgb灯驱动代码     
* 修改：liutong 20171027 修改了rgb控制引脚初始化函数名称、优化了注释、增加了rgb打开和关闭函数
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_apps_rcm.h"
#include "rgb.h"

/*********************************************************************************************
* 名称：rgb_init()
* 功能：rgb控制引脚初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void rgb_init(void)
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
  
  GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0xff);
  GPIOPinWrite(GPIOA0_BASE, G03_UCPINS, 0xff);                  //初始化断开继电器
  GPIOPinWrite(GPIOA1_BASE, G11_UCPINS, 0xff);                  //初始化断开继电器
}
  
/*********************************************************************************************
* 名称：rgb_on()
* 功能：rgb控制打开函数
* 参数：rgb号，在rgb.h中宏定义为RGB_R，RGB_G，RGB_B
* 返回：0，打开rgb成功，-1，参数错误
* 修改：
* 注释：参数只能填入RGB_R，RGB_G，RGB_B否则会返回-1
*********************************************************************************************/
signed char rgb_on(unsigned char rgb)
{
  if(rgb & 0x01){                                                //如果要打开RGB_R
    GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0x00);
    return 0;
  }
    
  if(rgb & 0x02){                                                //如果要打开RGB_G
    GPIOPinWrite(GPIOA0_BASE, G03_UCPINS, 0x00);
    return 0;
  }
  
  if(rgb & 0x04){                                                //如果要打开RGB_B
    GPIOPinWrite(GPIOA1_BASE, G11_UCPINS, 0x00);
    return 0;
  }
   
  return -1;                                                    //参数错误，返回-1
}

/*********************************************************************************************
* 名称：rgb_off()
* 功能：rgb控制关闭函数
* 参数：rgb号，在rgb.h中宏定义为RGB_R，RGB_G，RGB_B
* 返回：0，关闭rgb成功，-1，参数错误
* 修改：
* 注释：参数只能填入RGB_R，RGB_G，RGB_B否则会返回-1
*********************************************************************************************/
signed char rgb_off(unsigned char rgb)
{
  if(rgb & 0x01){                                                //如果要关闭RGB_R
    GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0xff);
    return 0;
  }
    
  if(rgb & 0x02){                                                //如果要关闭RGB_G
    GPIOPinWrite(GPIOA0_BASE, G03_UCPINS, 0xff);
    return 0;
  }

  if(rgb & 0x04){                                                //如果要关闭RGB_B
    GPIOPinWrite(GPIOA1_BASE, G11_UCPINS, 0xff);
    return 0;
  }
  
  return -1;                                                    //参数错误，返回-1
}