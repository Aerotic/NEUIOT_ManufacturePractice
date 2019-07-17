/*********************************************************************************************
* 文件：syn6288.c
* 作者：zonesion
* 说明：syn6288驱动程序
* 修改：Chenkm 2017.01.04 增加了注释
* 注释：
*********************************************************************************************/
/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "syn6288.h"
#include "string.h"

#define UART_IF_BUFFER           64
/*********************************************************************************************
* 名称：uart0_init(unsigned char StopBits,unsigned char Parity)
* 功能：串口0初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void uart1_init()
{
  
  MAP_PRCMPeripheralClkEnable(PRCM_UARTA1,PRCM_RUN_MODE_CLK);
  MAP_PinTypeUART(PIN_01, PIN_MODE_7);
  MAP_PinTypeUART(PIN_02, PIN_MODE_7);
  
  MAP_UARTConfigSetExpClk(CONSOLE1,MAP_PRCMPeripheralClockGet(CONSOLE_PERIPH1), 
                          9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                           UART_CONFIG_PAR_NONE));
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
  MAP_UARTCharPut(CONSOLE1,ch);

}

/*********************************************************************************************
* 名称：syn6288_init()
* 功能：syn6288初始化
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void syn6288_init()
{
  uart1_init(); 
 PRCMPeripheralClkEnable(PRCM_GPIOA0, PRCM_RUN_MODE_CLK); //使能时钟
  PinTypeGPIO(PIN_58,PIN_MODE_0,false);                    //选择引脚为GPIO模式（gpio16）
  GPIODirModeSet(GPIOA0_BASE, G03_UCPINS, GPIO_DIR_MODE_IN);   //设置GPIO16为输入模式
  PinConfigSet(PIN_58,PIN_TYPE_STD_PU,PIN_MODE_0);         //上拉
}

int syn6288_busy(void)
{ 
  if((unsigned char)GPIOPinRead(GPIOA0_BASE,G03_UCPINS) > 0)                                                      //忙检测引脚
    return 0;                                                   //没有检测到信号返回 0
  else
    return 1;                                                   //检测到信号返回 1
}
/*********************************************************************************************
* 名称：syn6288_play()
* 功能：
* 参数：s -- 数组名
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void syn6288_play(char *s)
{
  int i;
  int len = strlen(s);
  unsigned char c = 0;  
  unsigned char head[] = {0xFD,0x00,0x00,0x01,0x00};            //数据包头  
  head[1] = (len+3)>>8;
  head[2] = (len+3)&0xff;
  for (i=0; i<5; i++){
    uart_send_char(head[i]);
    c ^= head[i];
  }
  for (i=0; i<len; i++){
    uart_send_char(s[i]);
    c ^= s[i];
  }
  uart_send_char(0xA5);
}

/*******************************************************************************
* 名称: hex2unicode()
* 功能: 将16进制字符串转换成bin格式
* 参数: 
* 返回: 
* 修改:
* 注释: 
*******************************************************************************/
char *hex2unicode(char *str)
{       
  static char uni[64];
  int n = strlen(str)/2;
  if (n > 64) n = 64;
  
  for (int i=0; i<n; i++) {
    unsigned int x = 0;
    for (int j=0; j<2; j++) {
      char c = str[2*i+j];
      char o;
      if (c>='0' && c<='9') o = c - '0';
      else if (c>='A' && c<='F') o = 10+(c-'A');
      else if (c>='a' && c<='f') o = 10+(c-'a');
      else o = 0;
      x = (x<<4) | (o&0x0f);
    }
    uni[i] = x;
  }
  uni[n] = 0;
  return uni;
}


/*******************************************************************************
* syn6288_play_unicode()
* 功能：
* 参数：s -- 数组名
* 返回：
* 修改：
* 注释：
*******************************************************************************/
void syn6288_play_unicode(char *s, int len)
{
  int i;
  char c = 0;  
  unsigned char head[] = {0xFD,0x00,0x00,0x01,0x03};            //数据包头  
  head[1] = (len+3)>>8;
  head[2] = (len+3)&0xff;
  for (i=0; i<5; i++){
    uart_send_char(head[i]);
    c ^= head[i];
  }
  for (i=0; i<len; i++){
    uart_send_char(s[i]);
    c ^= s[i];
  }
  uart_send_char(c);
}
