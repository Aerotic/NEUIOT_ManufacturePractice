/*********************************************************************************************
* 文件：ld3320.c
* 作者：zonesion
* 说明：ld3320驱动程序
* 修改：Chenkm 2017.01.04 增加了注释
* 注释：
*********************************************************************************************/
/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "ld3320.h"
#include "string.h"
#include "iic.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "prcm.h"
#include "pin.h"
#include "uart.h"
#include "rom.h"
#include "rom_map.h"

#include "uart_if.h"
#include "common.h"

void uart1_handle(void);

static unsigned char rx_buf = 0;
char* cmd[13] = {"yu liu zhi ling",                             //预留指令
                 "da kai tai deng",                             //打开台灯
                 "guan bi tai deng",                            //关闭台灯
                 "da kai chuang lian",                          //打开窗帘
                 "guan bi chuang lian",                         //关闭窗帘
                 "da kai feng shan",                            //打开风扇
                 "guan bi feng shan",                           //关闭风扇
                 "da kai kong tiao",                            //打开空调
                 "guan bi kong tiao",                           //关闭空调
                 "da kai jia shi qi",                           //打开加湿器
                 "guan bi jia shi qi",                          //关闭加湿器
                 "zi dong mo shi",                              //自动模式
                 "shou dong mo shi",                            //手动模式
                 //"que ren",                                     //确认指令
};
/*********************************************************************************************
* 名称：uart1_init(void)
* 功能：串口1初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void uart1_init(void)
{
  MAP_PRCMPeripheralClkEnable(PRCM_UARTA1, PRCM_RUN_MODE_CLK);
  
  MAP_PinTypeUART(PIN_01, PIN_MODE_7);
  MAP_PinTypeUART(PIN_02, PIN_MODE_7);
  MAP_UARTConfigSetExpClk(UARTA1_BASE,MAP_PRCMPeripheralClockGet(PRCM_UARTA1), 
                          9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                 UART_CONFIG_PAR_NONE));
  UARTIntRegister(UARTA1_BASE,uart1_handle);
  UARTIntEnable(UARTA1_BASE,UART_INT_RX);
  UARTFIFODisable(UARTA1_BASE);
}

/*********************************************************************************************
* 名称：uart_send_char()
* 功能：串口发送字节函数
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void uart_send_char(unsigned char ch)
{
  MAP_UARTCharPut(UARTA1_BASE,ch);
}

/*********************************************************************************************
* 名称：ld3320_init()
* 功能：ld3320初始化
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void ld3320_init()
{
  char i = 0;
  uart1_init(); 
  ld3320_clean();
  delay_ms(200);
  ld3320_reload();
  delay_ms(200);
  for (i=0; i<13; i++){
    ld3320_add(cmd[i]);
    delay_ms(150);
  }
  delay_ms(200);
  ld3320_reload();
}
/*********************************************************************************************
* 名称：ld3320_add()
* 功能：添加一条识别语句
* 参数：s -- 数组名
* 返回：
* 修改：
* 注释：两次调用需至少隔0.1S,且调用ld3320_reload函数后语句才会生效，数据异常时模块绿灯会闪烁
*********************************************************************************************/
void ld3320_add(char *s)
{
  int i;
  int len = strlen(s);
  uart_send_char('{');
  uart_send_char('a');
  uart_send_char('0');
  for (i=0; i<len; i++){
    uart_send_char(s[i]);
  }
  uart_send_char('}');
}

/*********************************************************************************************
* 名称：ld3320_addrs()
* 功能：添加一条识别语句并返回字符串
* 参数：s -- 添加语句 r -- 返回语句
* 返回：
* 修改：
* 注释：两次调用需至少隔0.1S,且调用ld3320_reload函数后语句才会生效，数据异常时模块绿灯会闪烁
*********************************************************************************************/
void ld3320_addrs(char *s,char *r)
{
  int i;
  int len = strlen(s);
  uart_send_char('{');
  uart_send_char('a');
  uart_send_char('0');
  for (i=0; i<len; i++){
    uart_send_char(s[i]);
  }
  uart_send_char('|');
  uart_send_char('s');
  uart_send_char('0');
  len = strlen(r);
  for (i=0; i<len; i++){
    uart_send_char(s[i]);
  }
  uart_send_char('}');
}

/*********************************************************************************************
* 名称：ld3320_addrx()
* 功能：添加一条识别语句并返回一字节16进制数
* 参数：s -- 数组名 x -- 返回数
* 返回：
* 修改：
* 注释：两次调用需至少隔0.1S,且调用ld3320_reload函数后语句才会生效，数据异常时模块绿灯会闪烁
*********************************************************************************************/
void ld3320_addrx(char *s,unsigned char x)
{
  int i;
  int len = strlen(s);
  uart_send_char('{');
  uart_send_char('a');
  uart_send_char('0');
  for (i=0; i<len; i++){
    uart_send_char(s[i]);
  }
  uart_send_char('|');
  uart_send_char('x');
  uart_send_char('0');
  uart_send_char(x);
  uart_send_char('}');
}

/*********************************************************************************************
* 名称：ld3320_clean()
* 功能：清除所有语句列表
* 参数：
* 返回：
* 修改：
* 注释：清除语句列表后模块绿灯会闪烁
*********************************************************************************************/
void ld3320_clean(void)
{
  uart_send_char('{');
  uart_send_char('c');
  uart_send_char('0');
  uart_send_char('}');
}

/*********************************************************************************************
* 名称：ld3320_reload()
* 功能：重新加载语句列表
* 参数：
* 返回：
* 修改：
* 注释：添加语句后需重新加载语句列表
*********************************************************************************************/
void ld3320_reload(void)
{
  uart_send_char('{');
  uart_send_char('l');
  uart_send_char('0');
  uart_send_char('}');
}

/*********************************************************************************************
* 名称：ld3320_debug()
* 功能：开启/关闭调试模式
* 参数：
* 返回：
* 修改：
* 注释：添加语句后需重新加载语句列表
*********************************************************************************************/
void ld3320_debug(unsigned char cmd)
{
  uart_send_char('{');
  uart_send_char('d');
  if(cmd == 1)
    uart_send_char('1');
  else
    uart_send_char('0');
  uart_send_char('}');
}
/*********************************************************************************************
* 名称：ld3320_read
* 功能：
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
char ld3320_read(void)
{
  char x = rx_buf;
  rx_buf = 0;
  return x;
}
void uart1_handle(void)
{
  if(UART_INT_RX == UARTIntStatus(UARTA1_BASE,true))
  { 
    rx_buf = UARTCharGet(UARTA1_BASE);
    UARTIntClear(UARTA1_BASE,UART_INT_RX);
  }
}