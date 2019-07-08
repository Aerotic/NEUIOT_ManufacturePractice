#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include <string.h>
#include "stm32f4xx.h"

	
#define USART_REC_MAX 			200  	                        //定义最大接收字节数 200
extern unsigned char Usart_len;                              //缓冲区当前数据长度
extern unsigned char  USART_RX_BUF[USART_REC_MAX];           //接收缓冲区 
extern void usart_init(unsigned int bound);
extern void clean_usart(void);
extern void usart_send(unsigned char *s,unsigned char len);
#endif



