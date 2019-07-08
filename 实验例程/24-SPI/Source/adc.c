/*********************************************************************************************
* 文件: adc.c
* 作者：zonesion 2016.12.22
* 说明：ADC相关函数
* 修改：
* 注释：
*********************************************************************************************/
/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "adc.h"

/*********************************************************************************************
* 名称:adc_init()
* 功能:acd初始化
* 参数:无
* 返回:无
* 修改:
* 注释:
*********************************************************************************************/
void adc_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_InitTypeDef       ADC_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);         //使能GPIOC时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);          //使能ADC1时钟

  //先初始化ADC1通道0 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |
                                GPIO_Pin_1 |
                                GPIO_Pin_4 |
                                GPIO_Pin_5;                     //PA0 通道0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;                  //模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;             //不带上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);                        //初始化

  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	        //ADC1复位
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	        //复位结束

  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;      //独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;   //预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
  ADC_CommonInit(&ADC_CommonInitStructure);                     //初始化

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;        //12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;                 //非扫描模式
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;           //关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;        //右对齐
  ADC_InitStructure.ADC_NbrOfConversion = 1;                    //1个转换在规则序列中 也就是只转换规则序列1
  ADC_Init(ADC1, &ADC_InitStructure);                           //ADC初始化

  ADC_Cmd(ADC1, ENABLE);                                        //开启AD转换器
}
/*********************************************************************************************
* 名称:u16 adc_get(u8 ch)
* 功能:测量ADC
* 参数:u8 ch -- 选择通道
* 返回:无
* 修改:
* 注释:
*********************************************************************************************/
u16 adc_get(u8 ch)
{
  if (ch == 1) ch = ADC_Channel_10;
  else if (ch == 2) ch = ADC_Channel_11;
  else if (ch == 3) ch = ADC_Channel_14;
  else if (ch == 4) ch = ADC_Channel_15;
  else return 0;
  //设置指定ADC的规则组通道，一个序列，采样时间
  ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度

  ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能

  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

  return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}
