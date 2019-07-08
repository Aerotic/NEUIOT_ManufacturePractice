#ifndef _ADC_H_
#define _ADC_H_	
#include "stm32f4xx.h"
void adc_init(void); 				                        //ADC通道初始化
u16 adc_get(u8 ch); 				                        //获得某个通道值 
#endif 















