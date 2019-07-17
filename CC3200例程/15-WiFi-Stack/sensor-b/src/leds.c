/*********************************************************************************************
* 文件：led.c
* 作者：Meixin 2017.09.15
* 说明：LED灯驱动代码     
* 修改：liutong 20171027 修改了LED控制引脚初始化函数名称、优化了注释、增加了LED打开和关闭函数
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_apps_rcm.h"
#include "leds.h"

/*********************************************************************************************
* 名称：led_init()
* 功能：LED控制引脚初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void led_init(void)
{
  PRCMPeripheralClkEnable(PRCM_GPIOA0, PRCM_RUN_MODE_CLK);      //使能时钟
  PinTypeGPIO(PIN_59,PIN_MODE_0,0);                           //选择引脚为GPIO模式（gpio17）
  GPIODirModeSet(GPIOA0_BASE, G04_UCPINS, GPIO_DIR_MODE_OUT);   //设置GPIO17为输出模式
  
  PRCMPeripheralClkEnable(PRCM_GPIOA0, PRCM_RUN_MODE_CLK);      //使能时钟
  PinTypeGPIO(PIN_60,PIN_MODE_0,0);                           //选择引脚为GPIO模式（gpio17）
  GPIODirModeSet(GPIOA0_BASE, G05_UCPINS, GPIO_DIR_MODE_OUT);   //设置GPIO17为输出模式
  
  GPIOPinWrite(GPIOA0_BASE, G04_UCPINS, 0xff);
  GPIOPinWrite(GPIOA0_BASE, G05_UCPINS, 0xff);
}
  
/*********************************************************************************************
* 名称：led_on()
* 功能：LED控制打开函数
* 参数：LED号，在led.h中宏定义为LEDS1，LEDS2
* 返回：0，打开LED成功，-1，参数错误
* 修改：
* 注释：参数只能填入LEDS1，LEDS2,否则会返回-1
*********************************************************************************************/
signed char led_on(unsigned char led)
{
  if(led & 0x01){                                                //如果要打开LED1
    GPIOPinWrite(GPIOA0_BASE, G04_UCPINS, 0x00);
    return 0;
  }
    
  if(led & 0x02){                                                //如果要打开LED2
    GPIOPinWrite(GPIOA0_BASE, G05_UCPINS, 0x00);
    return 0;
  }
  
  return -1;                                                    //参数错误，返回-1
}

/*********************************************************************************************
* 名称：led_off()
* 功能：LED控制关闭函数
* 参数：LED号，在led.h中宏定义为LEDS1，LEDS2
* 返回：0，关闭LED成功，-1，参数错误
* 修改：
* 注释：参数只能填入LEDS1，LEDS2,否则会返回-1
*********************************************************************************************/
signed char led_off(unsigned char led)
{
  if(led &0x01){                                                //如果要关闭LED1
    GPIOPinWrite(GPIOA0_BASE, G04_UCPINS, 0xff);
    return 0;
  }
    
  if(led &0x2){                                                //如果要关闭LED2
    GPIOPinWrite(GPIOA0_BASE, G05_UCPINS, 0xff);
    return 0;
  }
  
  return -1;                                                    //参数错误，返回-1
}