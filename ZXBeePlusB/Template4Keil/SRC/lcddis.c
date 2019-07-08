/***********************************************************
* 文件: lcdshow.c
* 描述:用于LCD上显示输出 
* 作者: liding   2015-9-30
***********************************************************/


#include "lcddis.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "stm32f4xx.h"
int flag1,flag2;
char result1[20],result2[20],result3[20];
/***********************************************************
* 名称: border
* 功能: 画边框
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void border(){
  int i=10,j=10;
  for(j=i;j<310;j++)     
      LCDDrawPixel(j, i, 0x0000);    //top1
  for(j=i;j<230;j++)
      LCDDrawPixel(i,j , 0x0000);   //left1
  for(j=i;j<230;j++)
      LCDDrawPixel(310,j , 0x0000); //right1
  for(j=i;j<310;j++)
      LCDDrawPixel(j,230 , 0x0000);//bottom1
  
  for(j=i+5;j<305;j++)
      LCDDrawPixel(j, i+5, 0x0000); //top2
  for(j=i+5;j<225;j++)
      LCDDrawPixel(i+5,j , 0x0000);//left2
  for(j=i+5;j<225;j++)
      LCDDrawPixel(305, j, 0x0000);//right2
  for(j=i+5;j<305;j++)
      LCDDrawPixel(j, 225, 0x0000);//bottm2
}
/***********************************************************
* 名称: Draw_rect
* 功能: 画矩形
* 参数: 左上角点，右下角点，线条颜色
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void Draw_rect(int x0, int y0,int x1,int y1,int color){
  int temp_x=x0,temp_y=y0;
  while(x0<x1){  
    LCDDrawPixel(x0,y0,color);
    x0++;
  }
  while(y0<y1){
    LCDDrawPixel(x0,y0,color);
    y0++;
  }
  while(x1>temp_x){
    LCDDrawPixel(x1,y1,color);
    x1--;
  }
  while(y1>=temp_y){
    LCDDrawPixel(x1,y1,color);
    y1--;
  }
}
/***********************************************************
* 名称: Draw_rect
* 功能: 画矩形
* 参数: 左上角点，右下角点，线条颜色
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void Draw_rectRGB(int x0, int y0,int x1,int y1,s32 color){
  int temp_x=x0,temp_y=y0;
  while(x0<x1){  
    LCDDrawPixel(x0,y0,toRGBShow(color));
    x0++;
  }
  while(y0<y1){
    LCDDrawPixel(x0,y0,toRGBShow(color));
    y0++;
  }
  while(x1>temp_x){
    LCDDrawPixel(x1,y1,toRGBShow(color));
    x1--;
  }
  while(y1>=temp_y){
    LCDDrawPixel(x1,y1,toRGBShow(color));
    y1--;
  }
}
/***********************************************************
* 名称: toRGBShow
* 功能: 将填写的RGB形式（24位）颜色（板子本身不支持）转化为板子支持的16位颜色
* 参数: 要转换的RGB颜色
* 返回: 转化完成的16位颜色值
* 修改:
* 注释: 
***********************************************************/
u16 toRGBShow(s32 color){
  u8 R_t,G_t,B_t,R,G,B;
  u16 RGB=0;
  
  B_t=color;
  G_t=color>>8;
  R_t=color>>16;
  
  B=(u8)(31/(255.0/B_t));
  G=(u8)(63/(255.0/G_t));
  R=(u8)(31/(255.0/R_t));
  
  RGB |=R;
  RGB =RGB<<6;
  RGB |=G;
  RGB =RGB<<5;
  RGB |=B;                                    //将输入的RGB颜色转化为屏幕支持的16位色
  
  return RGB;
}
/***********************************************************
* 名称: LCDShowFnt
* 功能: 显示文字（汉字英文）
* 参数: x,y:显示点的坐标，text:要显示的内容，fnt_color:字体颜色，bg_color:背景颜色，type：显示的方式（16x16/24x24）
* 返回: 无
* 修改:
* 注释: 本函数为输入文字和背景颜色值为常见的RGB模式，如果习惯于RGB模式
        输入的请用本函数。（eg. 红色 在其他软件中颜色常用 #FF0000      本函数中直接使用16进制数即    0xFF0000）
***********************************************************/
void LCDShowFnt(int x,int y,char* text,u32 fnt_color,s32 bg_color,int type){
 // u8 R_t,G_t,B_t,R,G,B;
  
  u16 RGB=0,BG_RGB=-1;
  
  RGB=toRGBShow(fnt_color);
  if(bg_color>=0){
    BG_RGB=toRGBShow(bg_color);
  }
  
//  if(!type)
//    LCDDrawFnt16(x,y,text,RGB,BG_RGB);              //16x16形式显示
//  else{
//    LCDDrawFnt24(x, y, text, RGB, BG_RGB);          //24x24形式显示
//  }
}
/***********************************************************
* 名称: Draw_itaLine
* 功能: 画斜线
* 参数: 斜线的起点和终点，线条颜色
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void Draw_itaLine(int x0,int y0,int x1,int y1,int color){
  float k=0,b=0;
  int y=0;
  k=(float)(y1-y0)/(x1-x0);
  b=y1-k*x1;
  if(x0<x1){
    for(int i=x0;i<x1;i++){
      y=(int)(k*i+b);
      LCDDrawPixel(i,y,color);   //描像素点
    }
  }else{
      for(int i=x1;i<x0;i++){
      y=(int)(k*i+b);
      LCDDrawPixel(i,y,color);  //描像素点
    }
  }
}
/***********************************************************
* 名称: Draw_five
* 功能: 画五角星或五边形
* 参数: 五个顶点，和线的颜色
* 返回: 无
* 修改:                                 (4)
* 注释: 五角星的画法使用的是             *
                                        * *
                               (1) * * *   * * * (2)
                                     *       *
                                      *     *
                                     *   *  *
                                (3) * *    * *(5)
参数传入的顺序为（1-->2-->3-->4-->5），而五边形只需改变参数的传入顺为（1-->4-->2-->5-->3）
***********************************************************/
void Draw_five(int x0, int y0,int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,int color){
  Draw_itaLine(x0,y0,x1,y1,color);
  Draw_itaLine(x1,y1,x2,y2,color);
  Draw_itaLine(x2,y2,x3,y3,color);
  Draw_itaLine(x3,y3,x4,y4,color);
  Draw_itaLine(x4,y4,x0,y0,color);  
}

/***********************************************************
* 名称: Draw_itaLine_RGB
* 功能: 画斜线
* 参数: 斜线的起点和终点，线条颜色
* 返回: 无
* 修改:
* 注释: 本函数为输入线条颜色值为常见的RGB模式，如果习惯于RGB模式
        输入的请用本函数。（eg. 红色 在其他软件中颜色常用 #FF0000      本函数中直接使用16进制数即    0xFF0000）
***********************************************************/
void Draw_itaLine_RGB(int x0,int y0,int x1,int y1,s32 color){
  float k=0,b=0;
  int y=0;
  k=(float)(y1-y0)/(x1-x0);
  b=y1-k*x1;
  if(x0<x1){
    for(int i=x0;i<x1;i++){
      y=(int)(k*i+b);
      LCDDrawPixel(i,y,toRGBShow(color));   //描像素点
    }
  }else{
      for(int i=x1;i<x0;i++){
      y=(int)(k*i+b);
      LCDDrawPixel(i,y,toRGBShow(color));  //描像素点
    }
  }
}
/***********************************************************
* 名称: Draw_five_RGB
* 功能: 画五角星或五边形
* 参数: 五个顶点，和线的颜色
* 返回: 无
* 修改:                                 (4)
* 注释: 五角星的画法使用的是             *
                                        * *
                               (1) * * *   * * * (2)
                                     *       *
                                      *     *
                                     *   *  *
                                (3) * *    * *(5)
参数传入的顺序为（1-->2-->3-->4-->5），而五边形只需改变参数的传入顺为（1-->4-->2-->5-->3）

颜色值为常见的RGB模式，如果习惯于RGB模式输入的请用本函数。（eg. 红色 在其他软件中颜色常用 #FF0000      本函数中直接使用16进制数即    0xFF0000）
***********************************************************/
void Draw_five_RGB(int x0, int y0,int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,s32 color){
  Draw_itaLine_RGB(x0,y0,x1,y1,color);
  Draw_itaLine_RGB(x1,y1,x2,y2,color);
  Draw_itaLine_RGB(x2,y2,x3,y3,color);
  Draw_itaLine_RGB(x3,y3,x4,y4,color);
  Draw_itaLine_RGB(x4,y4,x0,y0,color);  
}
/***********************************************************
* 名称: Draw_four_RGB
* 功能: 画四边形
* 参数: 四个顶点，和线的颜色
* 返回: 无
* 修改:                                 
* 注释:      (1)*  *   *  *  *  *(2)
                *               *
                *               *
             (4)*  *   *  *  *  *(3)

参数传递按  
            →→→
           ↑   ↓
            ←←←
如图的形式传递得到的就是四边形不论是从（1）开始还是从（2）开始

颜色值为常见的RGB模式，如果习惯于RGB模式输入的请用本函数。（eg. 红色 在其他软件中颜色常用 #FF0000      本函数中直接使用16进制数即    0xFF0000）
***********************************************************/
void Draw_four_RGB(int x0, int y0,int x1,int y1,int x2,int y2,int x3,int y3,s32 color){
  Draw_five_RGB(x0,y0,x1,y1,x2,y2,x3,y3,x3,y3,color);
}
/***********************************************************
* 名称: Draw_three_RGB
* 功能: 画三角形
* 参数: 三个顶点，和线的颜色
* 返回: 无
* 修改:                              
* 注释:         

颜色值为常见的RGB模式，如果习惯于RGB模式输入的请用本函数。（eg. 红色 在其他软件中颜色常用 #FF0000      本函数中直接使用16进制数即    0xFF0000）
***********************************************************/
void Draw_three_RGB(int x0, int y0,int x1,int y1,int x2,int y2,s32 color){
  Draw_four_RGB(x0,y0,x1,y1,x2,y2,x2,y2,color);
}
/***********************************************************
* 名称: ShowLCD
* 功能: 显示实验信息
* 参数: SensorName：传感器名称  Result：实验结果输出的条数  level：实验难度等级
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void ShowLCD(char* SensorName,float Result,float level){
//  LCDDrawFnt16(30, 40, "Experiment:", 0x0000, -1);
//  LCDDrawFnt16(30, 70, "level:", 0x0000, -1);
//  LCDDrawFnt16(30, 100, "Key Point:", 0x0000, -1);
//  LCDDrawFnt16(30, 130, "Result:", 0x0000, -1);

  switch((int)Result){
    case 0: 
          Setlevel(level);
          ShowResult_1(SensorName);
      break;
    case 1: 
          Setlevel(level);
          ShowResult_2(SensorName);
      break;
      
    case 2:
          Setlevel(level);
          ShowResult_3(SensorName);
      break;
    case 3:
          Setlevel(level);
          //ShowResult_0(SensorName);
      break;
  }

}
/***********************************************************
* 名称: Setlevel
* 功能: 设置等级
* 参数:   level：实验难度等级
* 返回: 无
* 修改:
* 注释: 
***********************************************************/

void Setlevel(float level){
//  if(level==ONE)
//     LCDDrawFnt16(130, 70, "★", 0x0000, -1);
//  else if(level==ONE_HALF)
//     LCDDrawFnt16(130, 70, "★☆", 0x0000, -1);
//  else if(level==TWO)
//     LCDDrawFnt16(130, 70, "★★", 0x0000, -1);
//  else if(level==TWO_HALF)
//     LCDDrawFnt16(130, 70, "★★☆", 0x0000, -1);
//  else if(level==THREE)
//     LCDDrawFnt16(130, 70, "★★★", 0x0000, -1);
//  else if(level==NONE)
//     LCDDrawFnt16(130, 70, "☆", 0x0000, -1);
}
/***********************************************************
* 名称: ShowResult_2
* 功能: 显示实验结果
* 参数: SensorName：传感器名称  
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void ShowResult_2(char* SensorName){
  
  if(!strcmp(SensorName,"RFID"))
  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "SPI Used", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//    LCDDrawFnt16(130, 160, result2, 0x0000, -1);
  }
  if(!strcmp(SensorName,"HumiTemp"))
  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "DH11 module", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//    LCDDrawFnt16(130, 160, result2, 0x0000, -1);
  }
}

/***********************************************************
* 名称: ShowResult_3
* 功能: 显示实验结果
* 参数: SensorName：传感器名称  
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void ShowResult_3(char* SensorName){
  
  if(!strcmp(SensorName,"Acceleration"))
  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "IIC Used", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//    LCDDrawFnt16(130, 160, result2, 0x0000, -1);
//    LCDDrawFnt16(130, 190, result3, 0x0000, -1);
  }
}
/***********************************************************
* 名称: ShowResult_1
* 功能: 显示实验结果
* 参数: SensorName：传感器名称  
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void ShowResult_1(char* SensorName){
//  if(!strcmp(SensorName,"Photoresistance"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "ADC Transformation", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }
//  else if(!strcmp(SensorName,"Rain"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "ADC Transformation", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }
//  else  if(!strcmp(SensorName,"Flame"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "GPIO Check", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }
//  else  if(!strcmp(SensorName,"Relay"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "GPIO Contrl", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }
//  else  if(!strcmp(SensorName,"Hall"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "GPIO Check", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }
//  else  if(!strcmp(SensorName,"Ultrasonic"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "GPIO Contrl", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }
//  else  if(!strcmp(SensorName,"Infrared"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "GPIO Check", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }
//  else  if(!strcmp(SensorName,"CombustibleGas"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "ADC Transformation", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }
//  else  if(!strcmp(SensorName,"AlcoholGas"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "ADC Transformation", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }
//  else  if(!strcmp(SensorName,"AirGas"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "ADC Transformation", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }
//  else  if(!strcmp(SensorName,"Acceleration"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "IIC", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }
}
///***********************************************************
//* 名称: ShowResult_0
//* 功能: 显示实验结果
//* 参数: SensorName：实验名称  
//* 返回: 无
//* 修改:
//* 注释: 
//***********************************************************/
void ShowResult_0(char* SensorName){}
// void ShowResult_0(char* SensorName)
// {
//  if(!strcmp(SensorName,"KEY")){
//    //LCDDrawFnt16(30, 40, "Experiment:", 0x0000, -1);
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "KEY Used", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }else  if(!strcmp(SensorName,"Timer"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "Timer", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }else  if(!strcmp(SensorName,"Exti"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "Exti", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//    LCDDrawFnt16(130, 160, result2, 0x0000, -1);
//  }else  if(!strcmp(SensorName,"Iwdg"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "Iwdg", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }else  if(!strcmp(SensorName,"Usart"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "Usart", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }else  if(!strcmp(SensorName,"ADC"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "ADC Transformation", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }else  if(!strcmp(SensorName,"Flash"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "Flash", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }else  if(!strcmp(SensorName,"LCD"))
//  {
//    LCDDrawFnt16(130, 40, SensorName, 0x0000, -1);
//    LCDDrawFnt16(130, 100, "LCD", 0x0000, -1);
//    LCDDrawFnt16(130, 130, result1, 0x0000, -1);
//  }
//}
/***********************************************************
* 名称: SetRusult1
* 功能: 设置显示结果
* 参数: 无  
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void SetResult1(){

  if(flag2){
   sprintf(result1,"k4 press");
    //printf(result1);
   flag2=0;
  }
  else if(flag1){
    sprintf(result1,"K3 press");
    flag1=0;
    //printf(result1);
  }else{
    sprintf(result1,"No key press");
 }
 ShowResult_0("Exti");
}