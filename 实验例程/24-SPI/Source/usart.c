/*********************************************************************************************
* 文件：usart.c
* 作者：zonesion 2017.02.17
* 说明：串口驱动程序
* 修改：
* 注释：
*
*********************************************************************************************/
/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include <string.h>
#include "usart.h"

/*********************************************************************************************
* 全局变量
*********************************************************************************************/
unsigned char Usart_len=0;	               	                //接收缓冲区当前数据长度
unsigned char USART_RX_BUF[USART_REC_MAX];     	        //接收缓冲,最大USART_REC_LEN个字节.

/*********************************************************************************************
* 名称：fputc
* 功能：将usart1映射到printf函数
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
int fputc(int ch, FILE *f)
{ 	
  while((USART1->SR&0X40)==0);                                  //循环发送,直到发送完毕   
  USART1->DR = (unsigned char) ch;      
  return ch;
}

/*********************************************************************************************
* 名称：usart_init
* 功能：usart1初始化
* 参数：bound波特率
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void usart_init(unsigned int bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);     	//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);    	//使能USART1时钟 
  //串口1对应引脚复用映射
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);  	//GPIOA9复用为USART1
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 	//GPIOA10复用为USART1	
  //USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;  	//GPIOA9与GPIOA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;             	//复用功能
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//速度50MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           	//推挽复用输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             	//上拉
  GPIO_Init(GPIOA,&GPIO_InitStructure);                    	//初始化PA9，PA10
  //USART1 初始化设置
  USART_InitStructure.USART_BaudRate = bound;                   //波特率设置
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //字长为8位数据格式
  USART_InitStructure.USART_StopBits = USART_StopBits_1;        //一个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;           //无奇偶校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
  //收发模式
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);                     //根据上述配置初始化串口1	
  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;             //串口1中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;       //抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;	        //子优先级1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	        //IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);	                        //根据指定的参数初始化VIC寄存器、
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                //开启串口1接收中断
  USART_Cmd(USART1, ENABLE);                                    //使能串口1 	
}

/*********************************************************************************************
* 名称：USART1_IRQHandler
* 功能：串口中断处理函数
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void USART1_IRQHandler(void)                	           
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){        //如果收到数据(接收中断) 
    USART_ClearFlag(USART1, USART_IT_RXNE);                     //清除接收中断标志   
    if(Usart_len < USART_REC_MAX)                        
      USART_RX_BUF[Usart_len++] = USART_ReceiveData(USART1);    //将数据放入接收缓冲区
  } 
}

/*********************************************************************************************
* 名称：clean_usart
* 功能：清除串口缓冲区
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/ 
void clean_usart(void)
{
  memset(USART_RX_BUF,0,Usart_len);
  Usart_len = 0;
}

/*********************************************************************************************
* 名称：usart_send
* 功能：串口1发送数据
* 参数：s待发送的数据指针，len待发送的数据长度
* 返回：
* 修改：
* 注释：
*********************************************************************************************/ 
void usart_send(unsigned char *s,unsigned char len)
{
  for(unsigned char i = 0;i < len;i++){
    USART_SendData(USART1, *(s+i));
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE ) == RESET);
  }
}