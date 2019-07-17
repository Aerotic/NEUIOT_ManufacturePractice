#ifndef __SYN6288_H__
#define __SYN6288_H__
/*********************************************************************************************
* Í·ÎÄ¼þ
*********************************************************************************************/
#define UART1_BAUD_RATE  38400//115200
#define SYSCLK1          80000000
#define CONSOLE1         UARTA1_BASE
#define CONSOLE_PERIPH1  PRCM_UARTA1
#define G03_UCPINS  ( 1 << (3%8))

#include "hw_types.h"
#include "hw_memmap.h"
#include "prcm.h"
#include "pin.h"
#include "uart.h"
#include "rom.h"
#include "rom_map.h"

#include "uart_if.h"

#include "gpio.h"
#include "hw_memmap.h"
#include "systick_if.h"
#include "prcm.h"
#include <stdio.h>
#include "rom_map.h"

#define IS_SPACE(x)       (x == 32 ? 1 : 0)

void syn6288_init();
int syn6288_busy(void);
void syn6288_play(char *s);
char *hex2unicode(char *str);
void syn6288_play_unicode(char *s,int len);

#endif
