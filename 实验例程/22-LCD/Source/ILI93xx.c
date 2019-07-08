/*********************************************************************************************
* 文件: ili93xx.c
* 作者：zonesion 2016.12.22
* 说明：LCD显示相关函数  
* 修改：
* 注释：
*********************************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include "font.h"
#include "ili93xx.h"

static char CMD_WR_RAM = 0x22;
unsigned int LCD_ID = 0x9325;

/*********************************************************************************************
* 名称：LCD_WR_REG
* 功能：写寄存器函数
* 参数：regval:寄存器值
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void LCD_WR_REG(vu16 regval)
{   
  regval=regval;		                                //使用-O2优化的时候,必须插入的延时
  ILI93xx_REG=regval;                                           //写入要写的寄存器序号	 
}

/*********************************************************************************************
* 名称：LCD_WR_DATA
* 功能：写LCD数据
* 参数：data:要写入的值
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void LCD_WR_DATA(vu16 data)
{	  
  data=data;			                                //使用-O2优化的时候,必须插入的延时
  ILI93xx_DAT=data;		 
}

/*********************************************************************************************
* 名称：LCD_RD_DATA
* 功能：读LCD数据
* 参数：无
* 返回：读到的值
* 修改：
* 注释：
*********************************************************************************************/
u16 LCD_RD_DATA(void)
{
  vu16 ram;			                                //防止被优化
  ram=ILI93xx_DAT;	
  return ram;	 
}

/*********************************************************************************************
* 名称：ILI93xx_WriteReg
* 功能：向LCD指定寄存器写入数据
* 参数：r:寄存器地址; d:要写入的值
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void ILI93xx_WriteReg(uint16_t r, uint16_t d)
{
  ILI93xx_REG = r;
  ILI93xx_DAT = d;
}

/*********************************************************************************************
* 名称：ILI93xx_ReadReg
* 功能：读取寄存器的值
* 参数：r:寄存器地址
* 返回：读到的值
* 修改：
* 注释：
*********************************************************************************************/
uint16_t ILI93xx_ReadReg(uint16_t r)
{
  uint16_t v;
  ILI93xx_REG = r;
  v = ILI93xx_DAT;
  return v;
}

/*********************************************************************************************
* 名称：BLOnOff
* 功能：开启或关闭LCD背光
* 参数：st：1，开启背光；0，关闭背光
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void BLOnOff(int st)
{
#ifdef ZXBEE_PLUSE                                              //zxbeepluse
  if (st) {
    GPIO_SetBits(GPIOD, GPIO_Pin_2);                            //开启背光
  } else {
    GPIO_ResetBits(GPIOD, GPIO_Pin_2);
  }
#else
  if (st) {
    GPIO_SetBits(GPIOB, GPIO_Pin_15);
  } else {
    GPIO_ResetBits(GPIOB, GPIO_Pin_15);
  }
#endif
}

/*********************************************************************************************
* 名称：BLInit
* 功能：背光IO口初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void BLInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
#ifdef ZXBEE_PLUSE                                              //zxbeepluse
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
#else
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
}

/*********************************************************************************************
* 名称：REST_Init
* 功能：复位IO口初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void REST_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                             
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_SetBits(GPIOD, GPIO_Pin_13);
  delay_ms(1); 
  GPIO_ResetBits(GPIOD, GPIO_Pin_13);
  delay_ms(10); 
  GPIO_SetBits(GPIOD, GPIO_Pin_13);
  delay_ms(120);
}

/*********************************************************************************************
* 名称：ILI93xxInit
* 功能：LCD初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void ILI93xxInit(void)
{
  //REST_Init(); 
  BLOnOff(1);                                                   //9341要先开启背光才能读取寄存器值
  LCD_ID = ILI93xx_ReadReg(0);  
  if (LCD_ID == 0) {
    LCD_WR_REG(0Xd3);	
    int a=LCD_RD_DATA();	                                //dummy read 	
    int b=LCD_RD_DATA();	                                //读到0X00
    int c=LCD_RD_DATA();   	                                //读取93								  
    int d=LCD_RD_DATA();  	                                //读取41 	   			       
    LCD_ID = (c << 8) | d;
  } 
  
  if (LCD_ID == 0x72) {		
    CMD_WR_RAM = 0x2C;
    //****信利2.6寸IPS 240*400 BOE+HX8352-C**///
    //Power Voltage Setting
    ILI93xx_WriteReg(0x001A, 0x0002);                           //BT
    ILI93xx_WriteReg(0x001B, 0x0088);                           //VRH
    
    //****VCOM offset**///
    ILI93xx_WriteReg(0x0023, 0x0000);                           //SEL_VCM
    ILI93xx_WriteReg(0x0024, 0x005F);                           //VCM
    ILI93xx_WriteReg(0x0025, 0x0015);                           //VDV
    ILI93xx_WriteReg(0x002D, 0x0003);                           //NOW[2:0]=011
    
    //Power on Setting
    ILI93xx_WriteReg(0x0018, 0x0004);                           //Frame rate 72Hz
    ILI93xx_WriteReg(0x0019, 0x0001);                           //OSC_EN='1', start Osc
    ILI93xx_WriteReg(0x0001, 0x0000);                           //DP_STB='0', out deep sleep
    
    //262k/65k color selection
    ILI93xx_WriteReg(0x0017, 0x0005);                           //default 0x06 262k color // 0x05 65k color
    //SET PANEL
    ILI93xx_WriteReg(0x0036, 0x0013);                           //REV_P, SM_P, GS_P, BGR_P, SS_P
    
    //Gamma 2.2 Setting
    ILI93xx_WriteReg(0x0040,0x0000);
    ILI93xx_WriteReg(0x0041,0x0045);
    ILI93xx_WriteReg(0x0042,0x0045);
    ILI93xx_WriteReg(0x0043,0x0004);
    ILI93xx_WriteReg(0x0044,0x0000);
    ILI93xx_WriteReg(0x0045,0x0008);
    ILI93xx_WriteReg(0x0046,0x0023);
    ILI93xx_WriteReg(0x0047,0x0023);
    ILI93xx_WriteReg(0x0048,0x0077);
    ILI93xx_WriteReg(0x0049,0x0040);
    
    ILI93xx_WriteReg(0x004A,0x0004);
    ILI93xx_WriteReg(0x004B,0x0000);
    ILI93xx_WriteReg(0x004C,0x0088);
    ILI93xx_WriteReg(0x004D,0x0088);
    ILI93xx_WriteReg(0x004E,0x0088);
    
    ILI93xx_WriteReg(0x001F,0x00D0);                            //VCOMG=1
    
    //Display ON Setting
    ILI93xx_WriteReg(0x0028,0x003F);                            //GON=1, DTE=1, D=11
    //
    ILI93xx_WriteReg(0x016,0x00);  //
    //////////////////////////////////////////////////////
    //Set GRAM Area
    unsigned int xStar = 0, xEnd=240, yStar = 0, yEnd=400;
    ILI93xx_WriteReg(0x02, xStar>>8);
    ILI93xx_WriteReg(0x03, xStar);
    ILI93xx_WriteReg(0x04, xEnd>>8);
    ILI93xx_WriteReg(0x05, xEnd);
    
    ILI93xx_WriteReg(0x06, yStar>>8);
    ILI93xx_WriteReg(0x07, yStar);
    ILI93xx_WriteReg(0x08, yEnd>>8);
    ILI93xx_WriteReg(0x09, yEnd);
    
    ILI93xx_REG = 0x22;
    for (int i=0;i<240; i++) {
      for (int j=0; j<400; j++) {
        ILI93xx_DAT = 0xf800;
      }
    }
  } 
  else if ((LCD_ID == 0x9325) || (LCD_ID == 0x9328) || (LCD_ID == 0xD3D3))            //ILI9328   OK  
  {
    CMD_WR_RAM = 0x22;
    char sm = 0;
    char ss = 1;
    char gs = 1;
#if SCREEN_ORIENTATION_LANDSCAPE
    gs = 0;
#endif    
    ILI93xx_WriteReg(0x00EC,0x108F);                            // internal timeing      
    ILI93xx_WriteReg(0x00EF,0x1234);                            // ADD        
    ILI93xx_WriteReg(0x0001,0x0000|(ss<<8) | (sm<<10));     
    ILI93xx_WriteReg(0x0002,0x0700);                            //电源开启                    
    //DRIVE TABLE(寄存器 03H)
    //BIT3=AM BIT4:5=ID0:1
    //AM      ID0     ID1               FUNCATION
    // 0      0       0	                R->L    D->U
    // 1      0       0	                D->U	R->L
    // 0      1       0	                L->R    D->U
    // 1      1       0                 D->U	L->R
    // 0      0       1	                R->L    U->D
    // 1      0       1                 U->D	R->L
    // 0      1       1                 L->R    U->D 正常就用这个.
    // 1      1       1	                U->D	L->R
#if SCREEN_ORIENTATION_LANDSCAPE
    ILI93xx_WriteReg(0x0003,(1<<12)|(0<<5)|(1<<4)|(1<<3));      //65K  
#else
    ILI93xx_WriteReg(0x0003,(1<<12)|(1<<5)|(1<<4)|(0<<3));      //65K    
#endif
    
    ILI93xx_WriteReg(0x0004,0x0000);                                   
    ILI93xx_WriteReg(0x0008,0x0202);	           
    ILI93xx_WriteReg(0x0009,0x0000);         
    ILI93xx_WriteReg(0x000a,0x0000);                            //display setting         
    ILI93xx_WriteReg(0x000c,0x0001);                            //display setting          
    ILI93xx_WriteReg(0x000d,0x0000);                            //0f3c          
    ILI93xx_WriteReg(0x000f,0x0000);
    //电源配置
    ILI93xx_WriteReg(0x0010,0x0000);   
    ILI93xx_WriteReg(0x0011,0x0007);
    ILI93xx_WriteReg(0x0012,0x0000);                                                                 
    ILI93xx_WriteReg(0x0013,0x0000);                 
    ILI93xx_WriteReg(0x0007,0x0001);                 
    delay_ms(50); 
    ILI93xx_WriteReg(0x0010,0x1490);   
    ILI93xx_WriteReg(0x0011,0x0227);
    delay_ms(50); 
    ILI93xx_WriteReg(0x0012,0x008A);                  
    delay_ms(50); 
    ILI93xx_WriteReg(0x0013,0x1a00);   
    ILI93xx_WriteReg(0x0029,0x0006);
    ILI93xx_WriteReg(0x002b,0x000d);
    delay_ms(50); 
    ILI93xx_WriteReg(0x0020,0x0000);                                                            
    ILI93xx_WriteReg(0x0021,0x0000);           
    delay_ms(50); 
    //伽马校正
    ILI93xx_WriteReg(0x0030,0x0000); 
    ILI93xx_WriteReg(0x0031,0x0604);   
    ILI93xx_WriteReg(0x0032,0x0305);
    ILI93xx_WriteReg(0x0035,0x0000);
    ILI93xx_WriteReg(0x0036,0x0C09); 
    ILI93xx_WriteReg(0x0037,0x0204);
    ILI93xx_WriteReg(0x0038,0x0301);        
    ILI93xx_WriteReg(0x0039,0x0707);     
    ILI93xx_WriteReg(0x003c,0x0000);
    ILI93xx_WriteReg(0x003d,0x0a0a);
    delay_ms(50); 
    ILI93xx_WriteReg(0x0050,0x0000);                            //水平GRAM起始位置 
    ILI93xx_WriteReg(0x0051,0x00ef);                            //水平GRAM终止位置     
    
    ILI93xx_WriteReg(0x0052,0x0000);                            //垂直GRAM起始位置                    
    ILI93xx_WriteReg(0x0053,0x013f);                            //垂直GRAM终止位置  
    
    ILI93xx_WriteReg(0x0060,(gs<<15)|0x2700);                   //320 line   
    
    ILI93xx_WriteReg(0x0061,0x0001); 
    ILI93xx_WriteReg(0x006a,0x0000);
    ILI93xx_WriteReg(0x0080,0x0000);
    ILI93xx_WriteReg(0x0081,0x0000);
    ILI93xx_WriteReg(0x0082,0x0000);
    ILI93xx_WriteReg(0x0083,0x0000);
    ILI93xx_WriteReg(0x0084,0x0000);
    ILI93xx_WriteReg(0x0085,0x0000);
    
    ILI93xx_WriteReg(0x0090,0x0010);     
    ILI93xx_WriteReg(0x0092,0x0600);  
    //开启显示设置    
    ILI93xx_WriteReg(0x0007,0x0133); 
    
#if SCREEN_ORIENTATION_LANDSCAPE  
    LCDSetWindow(0, 320, 0, 240);                               //设置窗口为整个屏幕 
#else
    LCDSetWindow(0, 240, 0, 320);
#endif
  }
  else if (LCD_ID == 0x9341) {
    CMD_WR_RAM = 0x2C;
    LCD_WR_REG(0xCF);  
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0xC1); 
    LCD_WR_DATA(0X30); 
    LCD_WR_REG(0xED);  
    LCD_WR_DATA(0x64); 
    LCD_WR_DATA(0x03); 
    LCD_WR_DATA(0X12); 
    LCD_WR_DATA(0X81); 
    LCD_WR_REG(0xE8);  
    LCD_WR_DATA(0x85); 
    LCD_WR_DATA(0x10); 
    LCD_WR_DATA(0x7A); 
    LCD_WR_REG(0xCB);  
    LCD_WR_DATA(0x39); 
    LCD_WR_DATA(0x2C); 
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0x34); 
    LCD_WR_DATA(0x02); 
    LCD_WR_REG(0xF7);  
    LCD_WR_DATA(0x20); 
    LCD_WR_REG(0xEA);  
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0x00); 
    LCD_WR_REG(0xC0);                                           //Power control 
    LCD_WR_DATA(0x1B);                                          //VRH[5:0] 
    LCD_WR_REG(0xC1);                                           //Power control 
    LCD_WR_DATA(0x01);                                          //SAP[2:0];BT[3:0] 
    LCD_WR_REG(0xC5);                                           //VCM control 
    LCD_WR_DATA(0x30); 	                                        //3F
    LCD_WR_DATA(0x30); 	                                        //3C
    LCD_WR_REG(0xC7);                                           //VCM control2 
    LCD_WR_DATA(0XB7); 
    
    LCD_WR_REG(0x36);                                           // Memory Access Control 

#if SCREEN_ORIENTATION_LANDSCAPE  
    LCD_WR_DATA((1<<7)|(0<<6)|(1<<5)|SWAP_RGB);                 //从下到上,从左到右
#else
    LCD_WR_DATA((0<<7)|(0<<6)|(0<<5)|SWAP_RGB);                 //从左到右,从上到下
#endif        
    
    LCD_WR_REG(0x3A);   
    LCD_WR_DATA(0x55); 
    LCD_WR_REG(0xB1);   
    LCD_WR_DATA(0x00);   
    LCD_WR_DATA(0x1A); 
    LCD_WR_REG(0xB6);                                           // Display Function Control 
    LCD_WR_DATA(0x0A); 
    LCD_WR_DATA(0xA2); 
    LCD_WR_REG(0xF2);                                           // 3Gamma Function Disable 
    LCD_WR_DATA(0x00); 
    LCD_WR_REG(0x26);                                           //Gamma curve selected 
    LCD_WR_DATA(0x01); 
    LCD_WR_REG(0xE0);                                           //Set Gamma 
    LCD_WR_DATA(0x0F); 
    LCD_WR_DATA(0x2A); 
    LCD_WR_DATA(0x28); 
    LCD_WR_DATA(0x08); 
    LCD_WR_DATA(0x0E); 
    LCD_WR_DATA(0x08); 
    LCD_WR_DATA(0x54); 
    LCD_WR_DATA(0XA9); 
    LCD_WR_DATA(0x43); 
    LCD_WR_DATA(0x0A); 
    LCD_WR_DATA(0x0F); 
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0x00); 		 
    LCD_WR_REG(0XE1);                                           //Set Gamma 
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0x15); 
    LCD_WR_DATA(0x17); 
    LCD_WR_DATA(0x07); 
    LCD_WR_DATA(0x11); 
    LCD_WR_DATA(0x06); 
    LCD_WR_DATA(0x2B); 
    LCD_WR_DATA(0x56); 
    LCD_WR_DATA(0x3C); 
    LCD_WR_DATA(0x05); 
    LCD_WR_DATA(0x10); 
    LCD_WR_DATA(0x0F); 
    LCD_WR_DATA(0x3F); 
    LCD_WR_DATA(0x3F); 
    LCD_WR_DATA(0x0F);
    
    LCD_WR_REG(0x2B); 
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);                                          //LCD_WR_DATA(0x01);
    LCD_WR_DATA(0xef);                                          //LCD_WR_DATA(0x3f);
    LCD_WR_REG(0x2A); 
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x01);                                          //LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x3f);                                          //LCD_WR_DATA(0xef);	 
    
    LCD_WR_REG(0x11);                                           //Exit Sleep
    delay_ms(120);
    LCD_WR_REG(0x29);                                           //display on  
#if SCREEN_ORIENTATION_LANDSCAPE  
    LCDSetWindow(0, 320, 0, 240);                               //设置窗口为整个屏幕 
#else
    LCDSetWindow(0, 240, 0, 320);
#endif
  }
}

/*********************************************************************************************
* 名称：LCDSetWindow
* 功能：设置窗口
* 参数：x:窗口起始横坐标，xe：窗口终点横坐标，y:窗口起始纵坐标，ye：窗口终点纵坐标
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void LCDSetWindow(int x, int xe, int y, int ye)
{
  if (LCD_ID == 0x9341) {
    LCD_WR_REG(0X2A); 
    LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF); 
    LCD_WR_DATA(xe>>8);LCD_WR_DATA(xe&0XFF);
    LCD_WR_REG(0X2B); 
    LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF);
    LCD_WR_DATA(ye>>8);LCD_WR_DATA(ye&0XFF);
  }
  else{
#if SCREEN_ORIENTATION_LANDSCAPE
    ILI93xx_WriteReg(0x52, x);	                                //xstart        
    ILI93xx_WriteReg(0x53, xe);                                 //xend
    
    ILI93xx_WriteReg(0x50, y);	                                //ystart
    ILI93xx_WriteReg(0x51, ye);	 
#else
    ILI93xx_WriteReg(0x52, y);	                                //xstart        
    ILI93xx_WriteReg(0x53, ye);                                 //xend
    
    ILI93xx_WriteReg(0x50, x);	                                //ystart
    ILI93xx_WriteReg(0x51, xe);     
#endif
  }
}

/*********************************************************************************************
* 名称：LCDSetCursor
* 功能：设置坐标
* 参数：x,y：坐标
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void LCDSetCursor(int x, int y)
{
  if (LCD_ID == 0x9341) {
    LCD_WR_REG(0X2A); 
    LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF); 			 
    LCD_WR_REG(0X2B); 
    LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 	
  } 
  if ((LCD_ID == 0x9325) || (LCD_ID == 0x9328)){
#if SCREEN_ORIENTATION_LANDSCAPE
    ILI93xx_WriteReg(0x21, x);
    ILI93xx_WriteReg(0x20, y);
#else
    ILI93xx_WriteReg(0x21, y);
    ILI93xx_WriteReg(0x20, x);   
#endif 
  }
}

/*********************************************************************************************
* 名称：LCDWriteData
* 功能：写固定长度数据
* 参数：dat:数据，len：数据长度
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void LCDWriteData(uint16_t *dat, int len)
{
  ILI93xx_REG = CMD_WR_RAM;
  for (int i=0; i<len; i++) {
    ILI93xx_DAT = dat[i];
  }
}

/*********************************************************************************************
* 名称：LCDClear
* 功能：LCD清屏
* 参数：color:清屏颜色
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void LCDClear(uint16_t color)
{
  LCDSetCursor(0,0);
  ILI93xx_REG = CMD_WR_RAM;
  for (int i=0; i<320*240; i++) {
    ILI93xx_DAT = color;
  }
}

/*********************************************************************************************
* 名称：LCDDrawPixel
* 功能：画点
* 参数：x,y：坐标，color：点的颜色
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void LCDDrawPixel(int x, int y, uint16_t color)
{
  LCDSetCursor(x, y);
  ILI93xx_REG = CMD_WR_RAM;
  ILI93xx_DAT = color;
}

/*********************************************************************************************
* 名称：LCDrawLineH
* 功能：画横线
* 参数：x0：直线起始横坐标，x1：直线终点横坐标，y0：直线纵坐标
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void LCDrawLineH(int x0, int x1, int y0, int color)
{
  LCDSetCursor(x0, y0);
  ILI93xx_REG = CMD_WR_RAM;
  for (int i=x0; i<x1; i++) {
    ILI93xx_DAT = color;
  }
}

/*********************************************************************************************
* 名称：LCDDrawAsciiDot12x24_1
* 功能：显示一个ASCII（12*24）
* 参数：x，y：显示坐标，ch：显示字符，color：字符颜色，bc：背景色
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void LCDDrawAsciiDot12x24_1(int x, int y, char ch, int color, int bc)
{
  char dot;
  if (ch<0x20 || ch > 0x7e) ch = 0x20;
  ch -= 0x20;
  for (int i=0; i<3; i++) {
    for (int j=0; j<12; j++) {
      dot = nAsciiDot12x24[ch*36+i*12+j];
      for (int k=0; k<8; k++) {
        if (dot&1)LCDDrawPixel(x+j, y+(i*8)+k, color);
        else if (bc > 0) LCDDrawPixel(x+j, y+(i*8)+k, bc&0xffff);
        dot >>= 1;
      }
    }
  }
}

/*********************************************************************************************
* 名称：LCDDrawAsciiDot12x24
* 功能：显示多个ASCII（12*24）
* 参数：x，y：显示坐标，str：显示字符串，color：字符颜色，bc：背景色
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void LCDDrawAsciiDot12x24(int x, int y, char *str, int color, int bc)
{
  
  unsigned char ch = *str;
  
  while (ch != 0) {
    LCDDrawAsciiDot12x24_1(x, y, ch, color, bc);
    x += 12;
    ch = *++str;
  }
}

/*********************************************************************************************
* 名称：LCDDrawAsciiDot8x16_1
* 功能：显示一个ASCII（8*16）
* 参数：x，y：显示坐标，ch：显示字符，color：字符颜色，bc：背景色
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void LCDDrawAsciiDot8x16_1(int x, int y, char ch, int color, int bc)
{
  int i, j;
  char dot;
  if (ch<0x20 || ch > 0x7e) {
    ch = 0x20;
  }
  ch -= 0x20;
  for (i=0; i<16; i++) {
    dot = nAsciiDot8x16[ch*16+i];
    for (j=0; j<8; j++) {
      if (dot&0x80)LCDDrawPixel(x+j, y+i, color);
      else if (bc > 0)LCDDrawPixel(x+j, y+i, bc&0xffff);; 
      dot <<= 1;
    }
  }
}

/*********************************************************************************************
* 名称：LCDDrawAsciiDot8x16
* 功能：显示多个ASCII（8*16）
* 参数：x，y：显示坐标，str：显示字符串，color：字符颜色，bc：背景色
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void LCDDrawAsciiDot8x16(int x, int y, char *str, int color, int bc)
{
  
  unsigned char ch = *str;
  
#define CWIDTH  8
  while (ch != 0) {
    LCDDrawAsciiDot8x16_1(x, y, ch, color, bc);
    x += CWIDTH;
    
    ch = *++str;	
  }
}

///*********************************************************************************************
//* 名称：LCDDrawGB_16_1
//* 功能：显示一个汉字（16*16）
//* 参数：x，y：显示坐标，gb2：汉字字符串，color：字符颜色，bc：背景色
//* 返回：无
//* 修改：
//* 注释：
//*********************************************************************************************/
//void LCDDrawGB_16_1(int x, int y, char *gb2, int color, int bc) 
//{
//  char dot;
//  
//  for (int i=0; i<GB_16_SIZE; i++) {
//    if (gb2[0] == GB_16[i].Index[0] && gb2[1] == GB_16[i].Index[1]) {
//      for (int j=0; j<16; j++) {
//        for (int k=0; k<2; k++) {
//          dot = GB_16[i].Msk[j*2+k];
//          for (int m=0; m<8; m++) {
//            if (dot & 1<<(7-m)) {
//              LCDDrawPixel(x+k*8+m, y+j, color);
//            } else  if (bc > 0) {
//              LCDDrawPixel(x+k*8+m, y+j, bc);
//            }
//          }
//        }
//      }
//      break;
//    }
//  }
//}

extern unsigned char HZKBuf[282752];
/*********************************************************************************************
* 名称：LCDDrawGB_16_1
* 功能：显示一个汉字（16*16）
* 参数：x，y：显示坐标，gb2：汉字字符串，color：字符颜色，bc：背景色
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void LCDDrawGB_16_1(int x, int y, char *gb2, int color, int bc) 
{
  char dot;
  unsigned int index = 0;
  index=(94*(gb2[0] - 0xa1)+(gb2[1] - 0xa1));	    //计算区位	
      for (int j=0; j<16; j++) {
        for (int k=0; k<2; k++) {
          dot = HZKBuf[index*32+j*2+k];
          for (int m=0; m<8; m++) {
            if (dot & 1<<(7-m)) {
              LCDDrawPixel(x+k*8+m, y+j, color);
            } else  if (bc > 0) {
              LCDDrawPixel(x+k*8+m, y+j, bc);
            }
          }
        }
      }
}

/*********************************************************************************************
* 名称：LCDDrawGB_24_1
* 功能：显示一个汉字（24*24）
* 参数：x，y：显示坐标，gb2：汉字字符串，color：字符颜色，bc：背景色
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void LCDDrawGB_24_1(int x, int y, char *gb2, int color, int bc) 
{
  char dot;
  
  for (int i=0; i<GB_24_SIZE; i++) {
    if (gb2[0] == GB_24[i].Index[0] && gb2[1] == GB_24[i].Index[1]) {
      for (int j=0; j<24; j++) {
        for (int k=0; k<3; k++) {
          dot = GB_24[i].Msk[j*3+k];
          for (int m=0; m<8; m++) {
            if (dot & 1<<(7-m)) {
              LCDDrawPixel(x+k*8+m, y+j, color);
            } else  if (bc > 0){
              LCDDrawPixel(x+k*8+m, y+j, bc);
            }
          }
        }
      }
      break;
    }
  }
}

/*********************************************************************************************
* 名称：LCDDrawGB_16
* 功能：显示多个汉字（16*16）
* 参数：x，y：显示坐标，xs:换行起始横坐标，xe:换行终止横坐标，str：汉字字符串，color：字符颜色，bc：背景色
* 返回：无
* 修改： 
* 注释：
*********************************************************************************************/
void LCDDrawFnt16(int x, int y, int xs, int xe,char *str, int color, int bc)
{
  while (*str != '\0') {
    if (*str & 0x80) {
      if (str[1] != '\0') {
        LCDDrawGB_16_1(x, y, str, color, bc);
        str += 2;
        x+= 16;
        if(x > (xe-16)){
          x = xs;
          y = y + 20;
        }
      } else break;
    } else {
      LCDDrawAsciiDot8x16_1(x, y, *str, color, bc);
      str ++;
      x += 8;
      if(x > (xe-8)){
          x = xs;
          y = y + 20;
        }
    }
  }
}

/*********************************************************************************************
* 名称：LCDDrawFnt24
* 功能：显示多个汉字（24*24）
* 参数：x，y：显示坐标，str：汉字字符串，color：字符颜色，bc：背景色
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void LCDDrawFnt24(int x, int y, char *str, int color, int bc)
{
  while (*str != '\0') {
    if (*str & 0x80) {
      if (str[1] != '\0') {
        LCDDrawGB_24_1(x, y, str, color, bc);
        str += 2;
        x+= 24;
      } else break;
    } else {
      LCDDrawAsciiDot12x24_1(x, y, *str, color, bc);
      str ++;
      x += 12;
    }
  }
}

/*********************************************************************************************
* 名称：LCD_Clear
* 功能：清指定大小的屏幕
* 参数：x1,y1:起始坐标值，x2,y2:终点坐标值， color：屏幕颜色
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void LCD_Clear(int x1,int y1,int x2,int y2,uint16_t color)
{
  LCDSetWindow(x1,x2,y1,y2);
  LCDSetCursor(x1, y1);
  ILI93xx_REG = CMD_WR_RAM;                                       //显示命令
  for(int i=x1;i<=x2;i++)
    for(int j=y1;j<y2;j++){
      ILI93xx_DAT = color;
    }
#if SCREEN_ORIENTATION_LANDSCAPE  
    LCDSetWindow(0, 320, 0, 240);                               //设置窗口为整个屏幕 
#else
    LCDSetWindow(0, 240, 0, 320);
#endif
}