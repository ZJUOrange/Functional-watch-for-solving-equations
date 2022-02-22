#include "spblcd.h"  
#include "sys.h"
#include "touch.h"	 
#include "gui.h" 
#ifndef _PICTUREDRAW_H_
#define _PICTUREDRAW_H_

#define PIC_SIZE_ERR		0x28	//³ß´ç´íÎó

u8 picture_play(const u8 *filename,u16 x,u16 y,u16 width,u16 height,u8 mode);
#endif
