/*********************************************************************************************
* 文件：main.c
* 作者：chennian 2017.11.20
* 说明：外部flash读写程序
* 修改：zhoucj   2017.12.19 添加按键写入数据功能，修复清除flash液晶屏乱码的BUG
* 注释：
*
*********************************************************************************************/
/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "delay.h"
#include "string.h"
#include "w25qxx.h"
#include "led.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
/*********************************************************************************************
* 全局变量
*********************************************************************************************/
unsigned int start_addr = 1;                                    //起始位置
unsigned char write[] = "Hello IOT!\r\n";                       //要写入的数据
unsigned char  read[10];                                        //读取数据缓冲区

/*********************************************************************************************
* 名称:main()
* 功能:主函数，flash读写
* 参数:无
* 返回:无
* 修改:
* 注释:
*********************************************************************************************/
void main(void) 
{
  led_init();                                                   //LED初始化
  key_init();                                                   //按键初始化
  delay_init(168);                                              //延时初始化
  usart_init(115200);                                           //初始化
  W25QXX_Init();                                                //外部FLASH初始化
  turn_off(D3);                                                 //关闭D3 
  turn_off(D4);                                                 //关闭D4 
  lcd_init(FLASH1);
  
  
  //将要写入的数据写入100地址，从扇区起始地址开始写入
  W25QXX_Write(write,start_addr,strlen((char const *)write));
  
  for(;;){  
    u16 flash_ID=0;                                             //存储flash ID
    char str[5]={0};                                             
    delay_ms(1000);                                             //延时
    
    if(get_key_status() == (K3_PREESED|K4_PREESED)){            //检测K3K4同时被按下
      delay_ms(100);                                            //延时消抖
      if(get_key_status()==(K3_PREESED|K4_PREESED)){            //如果K3K4同时按下
        LCDDrawAsciiDot8x16(86, 166,"flash erasing", 0x0000, 0xffff);
        
        W25QXX_Erase_Chip();                                    //擦除整个flash
        LCD_Clear(85,166,319,211,0xffff);                       //lcd清除
        LCDDrawAsciiDot8x16(86, 166,"flash erased", 0x0000, 0xffff);
        delay_ms(1000);
      }
    }
    if(get_key_status() == (K1_PREESED|K2_PREESED)){            //检测K1K2同时被按下
      delay_ms(100);                                            //延时消抖
      if(get_key_status()==(K1_PREESED|K2_PREESED)){            //如果K1K2同时按下
        LCDDrawAsciiDot8x16(86, 166,"flash writing", 0x0000, 0xffff);
        delay_ms(1000);
        W25QXX_Write(write,start_addr,strlen((char const *)write));//写入数据
        LCD_Clear(85,166,319,211,0xffff);                       //lcd清除
        LCDDrawAsciiDot8x16(86, 166,"flash wrote", 0x0000, 0xffff);
        delay_ms(1000);
      }
    }
    turn_on(D3);                                                //点亮D3灯
    turn_on(D4);                                                //点亮D4灯
    W25QXX_Read(read,start_addr,strlen((char const *)write));   //从第七扇区其实地址开始读取数据，读取长度为写入数据的长度
    printf((char*)read);                                        //打印flash数据
    flash_ID=W25QXX_ReadID();                                   //读取flash ID
    sprintf(str,"flash_ID:%d \r\n",flash_ID);                   //将flash_ID 转化为字符串存储在str数组中
    printf((char*)str);                                         //打印flash ID到PC
    
    //显示读到的数据
    LCD_Clear(85,132,319,211,0xffff);                           //屏幕清除
    LCDDrawAsciiDot8x16(86, 132,str, 0x0000, 0xffff);           //显示flash ID
    LCDDrawAsciiDot8x16(86, 149,read, 0x0000, 0xffff);          //显示flash数据
    
    memset((char*)read,0,sizeof(write));                        //清空读取缓冲区
    delay_ms(200);                                              //稍加延时，得到D3、D4闪烁效果
    turn_off(D3);                                               //熄灭D3灯
    turn_off(D4);                                               //熄灭D4灯
  }
}
