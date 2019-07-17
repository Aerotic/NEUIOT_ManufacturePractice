/*********************************************************************************************
* 文件：sensor.h
* 作者：Xuzhy 2018.5.16
* 说明：sensor头文件
* 修改：
* 注释：
*********************************************************************************************/
#ifndef SENSOR_H
#define SENSOR_H
/*********************************************************************************************
* 宏定义
*********************************************************************************************/
#define t4ms            UTUtilsGetSysTime
#define t4exp(x)        ((((signed long)UTUtilsGetSysTime()-(signed long)(x))>=0)?1:0)
#define clock_time      UTUtilsGetSysTime

#define NODE_NAME "603"
#define NODE_TYPE 3
/*********************************************************************************************
* 函数原型
*********************************************************************************************/
extern void sensorInit(void);
extern void sensorLinkOn(void);
extern void sensorUpdate(void);
extern void sensorCheck(void);
extern void sensorControl(unsigned char cmd);
extern void sensorLoop(void);

#endif // SENSOR_H
