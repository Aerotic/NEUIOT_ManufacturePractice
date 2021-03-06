/*********************************************************************************************
* 文件：exti.c
* 作者：Meixin 2017.09.15
* 说明：外部中断驱动代码头文件  
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "extint.h"

/*********************************************************************************************
* 名称：xtal_init()
* 功能：CC2530时钟初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void xtal_init(void)
{
  SLEEPCMD &= ~0x04;                                            //都上电
  while(!(CLKCONSTA & 0x40));                                   //晶体振荡器开启且稳定
  CLKCONCMD &= ~0x47;                                           //选择32MHz晶体振荡器
  SLEEPCMD |= 0x04;
}

/*********************************************************************************************
* 名称：ext_init()
* 功能：外部中断初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void ext_init(void)
{
  IEN2 |= 0x10;                                                 //端口1中断使能
  P1IEN |= 0x04;                                                //开P1口中断
  PICTL |= 0x02;                                                //下降沿触发    
  EA = 1;                                                       //总中断使能    
}