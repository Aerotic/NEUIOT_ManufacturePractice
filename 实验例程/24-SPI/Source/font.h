#ifndef __FONT_H__
#define __FONT_H__

struct typFNT_GB24                // 汉字字模数据结构
{
       char* Index;              // 汉字内码索引
       char Msk[72];                      // 点阵码数据
};
struct typFNT_GB16                // 汉字字模数据结构
{
       char* Index;              // 汉字内码索引
       char Msk[32];                      // 点阵码数据
};
extern const unsigned char nAsciiDot8x16[];
extern const unsigned char nAsciiDot12x24[];
extern const struct typFNT_GB24 GB_24[];
extern const struct typFNT_GB16 GB_16[];
extern int GB_24_SIZE;
extern int GB_16_SIZE;
#endif
