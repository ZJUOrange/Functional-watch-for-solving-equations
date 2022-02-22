#ifndef __DRAW_H
#define __DRAW_H	 
#include "lcd.h"
#include "stdlib.h"
#include "usart.h"	 
#include "delay.h"	
#include "math.h"
//#include "FONT.H"

#define pi 3.1415926
//清除指针的位置
extern u16 pointx_sec[200];
extern u16 pointy_sec[200];
extern u16 pointcolor_sec[200];
extern u16 pointx_min[200];
extern u16 pointy_min[200];
extern u16 pointcolor_min[200];
extern u16 pointx_hour[200];
extern u16 pointy_hour[200];
extern u16 pointcolor_hour[200];
extern u8 clockpoint_flag;
extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];
extern const unsigned char asc2_2412[95][36];
void clock_dial(u16 x,u16 y,u16 r,u16 d);
void clock_Pointer(u16 x,u16 y,u16 r);
void drawline(u16 x1,u16 y1,u16 x2,u16 y2,u16 color,u16 size);
void drawpoint(u16 x,u16 y,u16 color);
void clearline(u16 *pointx,u16 *pointy,u16 *pointcolor);
void line_info(u16 x1,u16 y1,u16 x2,u16 y2,u16 *pointx,u16 *pointy,u16 *pointcolor,u16 size);
void linesize(u16 size);
void LCDShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode,u16 color);
void LCDShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);
void LCDShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);

#endif
