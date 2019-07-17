/*********************************************************************************************
* 文件：oled_iic.h
* 作者：zonesion
* 说明：oled_iic头文件
* 修改：
* 注释：
*********************************************************************************************/
#ifndef _IIC_H_
#define _IIC_H_

#include "hw_ints.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "prcm.h"
#include "pinmux.h"
#include "rom.h"
#include "rom_map.h"
#include "gpio.h"
#include "pin.h"
#include <math.h>
#include <stdio.h>
/*********************************************************************************************
* 外部原型函数
*********************************************************************************************/
void iic_delay_us(unsigned int i);
void delay_ms(unsigned int xms);
void iic_init(void);
void iic_start(void);
void iic_stop(void);
void iic_send_ack(int ack);
int iic_recv_ack(void);
unsigned char iic_write_byte(unsigned char data);
unsigned char iic_read_byte(unsigned char ack);
void delay(unsigned int t);

#endif 