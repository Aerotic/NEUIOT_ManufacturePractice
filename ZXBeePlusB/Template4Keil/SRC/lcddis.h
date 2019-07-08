#define ONE 0.0
#define TWO 1.0
#define THREE 2.0
#define ONE_HALF 1.5
#define TWO_HALF 2.5
#define NONE  3.0

#define _16 0
#define _24 1
#include "stm32f4xx.h"
#include "ili93xx.h"
extern int flag1,flag2;
void border();
void ShowLCD(char* SensorName,float Result,float level);
void Setlevel(float level);
void ShowResult_1(char* SensorName);
void ShowResult_2(char* SensorName);
void ShowResult_3(char* SensorName);
void ShowResult_0(char* SensorName);
void SetResult1();
void Draw_rect(int x0, int y0,int x1,int y1,int color);
void Draw_rectRGB(int x0, int y0,int x1,int y1,s32 color);
void LCDShowFnt(int x,int y,char* text,u32 fnt_color,s32 bg_color,int type);
void Draw_five(int x0, int y0,int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,int color);
void Draw_five_RGB(int x0, int y0,int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,s32 color);
//三角形
void Draw_three_RGB(int x0, int y0,int x1,int y1,int x2,int y2,s32 color);
//四边形（菱形，平行四边形）
void Draw_four_RGB(int x0, int y0,int x1,int y1,int x2,int y2,int x3,int y3,s32 color);
//颜色转换
u16 toRGBShow(s32 color);