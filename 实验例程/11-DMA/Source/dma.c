/*********************************************************************************************
* 文件：dma.c
* 作者：Meixin 2017.09.26
* 说明：DMA传输驱动代码    
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "dma.h"

/*********************************************************************************************
* 名称：
* 功能：用于配置DMA的结构体
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
#pragma bitfields=reversed
typedef struct
{
  unsigned char SRCADDRH;                                       //源地址高8位
  unsigned char SRCADDRL;                                       //源地址低8位
  unsigned char DESTADDRH;                                      //目的地址高8位
  unsigned char DESTADDRL;                                      //目的地址低8位
  unsigned char VLEN        :3;                                 //长度域模式选择
  unsigned char LENH        :5;                                 //传输长度高字节
  unsigned char LENL        :8;                                 //传输长度低字节
  unsigned char WORDSIZE    :1;                                 //字节（byte）或字（word）传输
  unsigned char TMODE       :2;                                 //传输模式选择
  unsigned char TRIG        :5;                                 //触发事件选择
  unsigned char SRCINC      :2;                                 //源地址增量：-1/0/1/2
  unsigned char DESTINC     :2;                                 //目的地址增量：-1/0/1/2
  unsigned char IRQMASK     :1;                                 //中断屏蔽
  unsigned char M8          :1;                                 //7或8bit传输长度，仅在字节传输模式下适用
  unsigned char PRIORITY    :2;                                 //优先级
}DMA_CFG;
#pragma bitfields=default

/*********************************************************************************************
* 名称：dma_test()
* 功能：DMA传输函数
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void dma_test(void)
{
  DMA_CFG dmaConfig;                                            //定义配置结构体
  
  char sourceString[]="{data=I'm the sourceString!}\r\n";        //源字符串
  char destString[sizeof(sourceString)]="{data=I'm the destString!}\r\n";       //目的字符串
  
  char i;
  char error=0;
   
  uart1_send_string(sourceString);                               //传输前的原字符数组
  hal_wait(250);
  hal_wait(250);
  uart1_send_string(destString);                                 //传输前的目的字符数组
  
  //配置DMA结构体
  dmaConfig.SRCADDRH=(unsigned char)((unsigned int)&sourceString >> 8);         //源地址
  dmaConfig.SRCADDRL=(unsigned char)(((unsigned int)&sourceString)&0xFF);
    
  dmaConfig.DESTADDRH=(unsigned char)((unsigned int)&destString >> 8);          //目的地址
  dmaConfig.DESTADDRL=(unsigned char)(((unsigned int)&destString)&0xFF);
  
  dmaConfig.VLEN=0x00;                                          //选择LEN作为传送长度
  
  dmaConfig.LENH=(unsigned char)((unsigned int)sizeof(sourceString) >> 8);      //传输长度
  dmaConfig.LENL=(unsigned char)((unsigned int)sizeof(sourceString));
  
  dmaConfig.WORDSIZE=0x00;                                      //选择字节（byte）传送
  
  dmaConfig.TMODE=0x01;                                         //选择块传送（block）模式
  
  dmaConfig.TRIG=0;                                             //无触发(可以理解为手动触发）
  
  dmaConfig.SRCINC=0x01;                                        //源地址增量为1
  
  dmaConfig.DESTINC=0x01;                                       //目的地址增量为1
  
  dmaConfig.IRQMASK=0;                                          //DMA中断屏蔽
    
  dmaConfig.M8=0x00;                                            //选择8位长的字节来传送数据
  
  dmaConfig.PRIORITY=0x02;                                      //传输优先级为高
  

  DMA0CFGH=(unsigned char)((unsigned int)&dmaConfig >> 8);      //将配置结构体的首地址赋予相关SFR
  DMA0CFGL=(unsigned char)((unsigned int)(&dmaConfig)&0xFF);
  
  DMAARM=0x01;                                                  //启用配置
  
  DMAIRQ=0x00;                                                  //清中断标志
  DMAREQ=0x01;                                                  //启动DMA传输
  
  while(!(DMAIRQ&0x01));                                        //等待传输结束
  hal_wait(250);
  hal_wait(250);
  for(i=0;i<sizeof(sourceString);i++)                           //校验传输的正确性
  {
    if(sourceString[i]!=destString[i])
      error++;
  }
  
  if(error==0)                                                  //将结果通过串口传输到PC
  {
    uart1_send_string("{data=Correct!}\r\n");
    hal_wait(250);
    hal_wait(250);
    uart1_send_string(destString);                               //传输后的目的字符数组
  }
  else
    uart1_send_string("{data=Error!}\r\n");
  
  hal_wait(250);
  hal_wait(250);
}