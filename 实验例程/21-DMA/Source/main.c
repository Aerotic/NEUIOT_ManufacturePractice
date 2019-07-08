/*********************************************************************************************
* 文件：main.c
* 作者：chennian 2017.11.2
* 说明：DMA实验逻辑代码      
* 功能：通过DMA通道将数组A的内容复制给数组B
*       上位机会分别显示复制前和复制后数组A、数组B的内容
* 修改：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "delay.h"
#include "string.h"
#include "led.h"
#include "usart.h"
#include "DMA.h"
#include "key.h"

/*********************************************************************************************
* 名称:main()
* 功能:
* 参数:无
* 返回:无
* 修改:
* 注释:
*********************************************************************************************/
void main(void)
{
  char A[]={'h','e','l','l','o',',','w','o','r','d'};
  char B[10]={'H','E','L','L','O',',','W','O','R','D'};
  int error=0;                                                  //错误计数
  
  usart_init(115200);                                           //串口初始化
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置系统中断优先级分组
  key_init();                                                   //按键初始化
  
  printf("Hello IOT!\r\n\r\n");                                     //串口输出信息 
  usart_send("Usart is ready!\r\n",strlen("Usart is ready!\r\n"));  //串口打印出提示信息
  printf("\r\n");                                               //串口打印换行
  
  DMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)A,(u32)B,SEND_BUF_SIZE);//DMA2配置
  
  printf("DMA执行前:\r\n");                                      //串口打印 
  printf("数组A：%s\r\n",A);                                     //串口打印数组A
  printf("数组B：%s\r\n",B);                                     //串口打印数组B
  
  for(;;){
    if(get_key_status()==K3_PREESED){                           //是否有按键按下 
      delay_count(500);                                         //延时消抖                            
      if(get_key_status()==K3_PREESED){                         //是否有按键按下
        while(get_key_status()==K3_PREESED);                    //等待按键放开
        printf("\r\nDMA执行后:\r\n");                           //串口打印
        
        DMA_Enable(DMA2_Stream7,SEND_BUF_SIZE);                 //启动一次DMA传输
        
        while(1){                                               //等待DMA2_Stream7传输完成
          if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET){ 
            DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);         //清除DMA2_Stream7传输完成标志
            break; 
          }
        }
        printf("数组B：%s  \r\n",B);                               //串口打印
        
        for(int j=0;j<10;j++){                                  //校验复制结果
          if(A[j]!=B[j])
            error++;                                              
        }
        printf("error=%d\r\n\r\n",error);                       //串口打印错误率
      }
    }
    delay_count(10000);                                         //延时     
  }
}