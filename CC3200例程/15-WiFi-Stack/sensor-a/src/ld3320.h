#ifndef __LD3320_H__
#define __LD3320_H__
/*********************************************************************************************
* Í·ÎÄ¼þ
*********************************************************************************************/
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

extern unsigned char rx_buf;
void ld3320_init();
void ld3320_add(char *s);
void ld3320_addrs(char *s,char *r);
void ld3320_addrx(char *s,unsigned char x);
void ld3320_clean(void);
void ld3320_reload(void);
char ld3320_read(void);
void ld3320_debug(unsigned char cmd);

#endif
