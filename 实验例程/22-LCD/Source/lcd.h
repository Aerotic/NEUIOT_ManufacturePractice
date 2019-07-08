#ifndef _LCD_H_
#define _LCD_H_
#include "ili93xx.h"
#include "fsmc.h"
#define LCD1                    1
#define LCD2                    2
#define FLASH1                  3
#define FLASH2                  4
#define IIC1                    5
#define IIC2                    6
#define LIGHTINTENSITY1         7
#define LIGHTINTENSITY2         8
#define AIRPRESSURE1            9
#define AIRPRESSURE2            10
#define INFRARED1               11
#define INFRARED2               12
#define COMBUSTIBLEGAS1         13
#define COMBUSTIBLEGAS2         14
#define VIBRATION1              15
#define VIBRATION2              16
#define HALL1                   17
#define HALL2                   18
#define GRATING1                19
#define GRATING2                20
#define RELAY1                  21
#define RELAY2                  22
#define FAN1                    23
#define FAN2                    24
#define STEPMOTOR1              25
#define STEPMOTOR2              26
#define ALARM1                  27
#define ALARM2                  28
#define AIRGAS1                 29
#define AIRGAS2                 30
#define ACCELERATI0N1           31
#define ACCELERATI0N2           32
#define STADIOMETRY1            33
#define STADIOMETRY2            34
#define FLAME1                  35
#define FLAME2                  36
#define TOUCH1                  37
#define TOUCH2                  38

void lcd_init(unsigned char name);

#endif