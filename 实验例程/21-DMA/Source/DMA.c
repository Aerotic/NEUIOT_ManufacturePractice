/*********************************************************************************************
* 文件：DMA.c
* 作者：chennian 2017.11.2
* 说明：DMA驱动程序    
* 功能：通过DMA通道将数组A的内容复制给数组B
*       上位机会分别显示复制前和复制后数组A、数组B的内容
* 修改：
*********************************************************************************************/
/*********************************************************************************************
* 头文件
*********************************************************************************************/
		 
#include "DMA.h"																	   	  
#include "delay.h"
/*********************************************************************************************
* 名称：MYDMA_Config
* 功能：DMAx各通道配置
* 参数：DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
*       chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
*       par:外设地址
*       mar:存储器地址
*       ndtr:数据传输量 
* 返回：无
* 修改：无
*********************************************************************************************/
void DMA_Config(DMA_Stream_TypeDef *DMA_Streamx,uint32_t chx,uint32_t par,uint32_t mar,uint16_t ndtr)
{ 

  DMA_InitTypeDef  DMA_InitStructure;
  
  if((uint32_t)DMA_Streamx>(uint32_t)DMA2)                      //得到当前stream是属于DMA2还是DMA1
  {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);         //DMA2时钟使能
    
  }else 
    {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);         //DMA1时钟使能
    }
    DMA_DeInit(DMA_Streamx);
  
  while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}            //等待DMA可配置
  
  /* 配置DMA Stream */
  DMA_InitStructure.DMA_Channel = chx;                          //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;               //DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = mar;                  //DMA存储器0地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;           //存储器到存储器模式
  DMA_InitStructure.DMA_BufferSize = ndtr;                      //数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;       //存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度8
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度8
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                 //使用普通模式  
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;         //中等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_INC8;     //存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_INC8;//外设单次传输
  DMA_Init(DMA_Streamx, &DMA_InitStructure);                    //初始化DMA Stream
  
  
} 
/*********************************************************************************************
* 名称：DMA_Enable
* 功能：开启一次DMA传输
* 参数：DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
*       ndtr:数据传输量 
* 返回：无
* 修改：无
*********************************************************************************************/

void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,uint16_t ndtr)
{
  
  DMA_Cmd(DMA_Streamx, DISABLE);                                //关闭DMA传输
  
  while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	        //确保DMA可以被设置 
  
  DMA_SetCurrDataCounter(DMA_Streamx,ndtr);                     //数据传输量
  
  DMA_Cmd(DMA_Streamx, ENABLE);                                 //开启DMA传输
}	  

