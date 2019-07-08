#ifndef _DELAY_H_
#define _DELAY_H_
#include "stm32f4xx.h"
//0,不支持contiki
//1,支持contiki
#define SYSTEM_SUPPORT_CONTIKI		0			//定义系统文件夹是否支持CONTIKI

#if SYSTEM_SUPPORT_CONTIKI                                      //如果支持CONTIKI
#include "contiki.h"
void clock_delay_ms(unsigned int ut);                           //函数声明
#define delay_ms(x) clock_delay_ms(x)
#else                                                           //不支持CONTIKI
#include "delay.h"
#endif 	 /*SYSTEM_SUPPORT_CONTIKI*/

void delay_init(unsigned char SYSCLK);                          
void delay_ms(unsigned short nms);
void delay_us(unsigned int nus);
void delay_xms(unsigned short nms);

#endif  /*__DELAY_H__*/





























