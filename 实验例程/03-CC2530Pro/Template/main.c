#include <ioCC2530.h>         //引入CC2530所对应的头文件（包含各SFR的定义）

#define D7    P1_0              //定义D7为P1_0口控制
#define D6    P1_1              //定义D6为P1_1口控制

void led_init(void);
void led_test(void);
void halWait(unsigned char wait);

/*主函数
-------------------------------------------------------*/
void main(void)
{
  led_init();
  
  while(1)
  {
    led_test();
  }
}

/*led初始化
-------------------------------------------------------*/
void led_init(void)
{
  P1SEL &= ~0x03;          //P1.0 P1.1为普通 I/O 口
  P1DIR |= 0x03;           //输出
  
  D7 = 1;                  //关LED
  D6 = 1;
}

/*led闪烁函数
-------------------------------------------------------*/
void led_test(void)
{
  D7 = 0;              //D7闪烁
  halWait(250);
  D7 = 1;
  halWait(250);
  
  D6 = 0;              //D6闪烁
  halWait(250);
  D6 = 1;
  halWait(250);
}

/*延时函数
-------------------------------------------------------*/
void halWait(unsigned char wait)
{
  unsigned long largeWait;

  if(wait == 0)
  {return;}
  largeWait = ((unsigned short) (wait << 7));
  largeWait += 114*wait;

  largeWait = (largeWait >> ( CLKCONCMD & 0x07 ));
  while(largeWait--);

  return;
}
