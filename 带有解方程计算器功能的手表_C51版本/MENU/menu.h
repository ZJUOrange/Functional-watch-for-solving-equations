//#include "calendar.h"
#include "newtouch.h"
#include "sys.h"
#include "simplesys.h"
#include "draw.h"
#include "time.h"
#include "background.h"
#include "drawcalc.h"
#include "time.h"
#include "time_set.h"
#include "colorset.h"
#ifndef _MENU_H_
#define _MENU_H_

extern u8*const choose_path[4];

u8 sys_f_slex1(void);
u8 sub_play(void);
u8 main_face(void);
u8 main_control(void);
u8 main_display(void);
u8 choose_play(void);
u8 choose_play_short(_btn_obj * tbtn);
void number_face(void);

#endif
