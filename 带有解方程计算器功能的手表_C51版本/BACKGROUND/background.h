#include "spblcd.h"  
#include "malloc.h"
#include "lcd.h"
#include "picturedraw.h"
#include "includes.h"
#include "gui.h" 
//#include "common.h"
#include "newtouch.h"
#include "simplesys.h"
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#define BGD_SIZE_ERR		0x28	//³ß´ç´íÎó
extern u8 background_draw_flag;
extern u8 bgd;
extern u8*const background_path[4];
extern u8*const background_icos_path[4];

void FLASH_SD_init(void);
u8 background_init(void);
void background_choose(void);
void circle_fill(u16 x0, u16 y0, u16 r, u16 color);
void circle_play(u16 x0, u16 y0, u16 r, u16 color);
u8 picture_play(const u8 *filename,u16 x,u16 y,u16 width,u16 height,u8 mode);
#endif
