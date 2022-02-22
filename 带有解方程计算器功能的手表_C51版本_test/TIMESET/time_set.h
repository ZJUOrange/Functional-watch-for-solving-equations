//#include "includes.h" 
#include "newtouch.h"
#include "time.h"
#include "BEEP.h"
#include "simplesys.h"
#include "picturedraw.h"
//#include "common.h"
#ifndef _TIME_SET_H_
#define _TIME_SET_H_
    //====================================================
    //                 时间设置相关函数
    //===================================================
    //这个函数用于 按钮颜色初始化
    u8 SetBtn_color_init(_btn_obj * ptr);
    //这个函数用于设置时、分等信息
    u8 SetTime(u16 x,u16 y,u16*timer);
    //这个函数用来设置时钟
    void SetTime_HM(u16 x,u16 y);
    //====================================================
    //                 闹钟相关函数
    //===================================================
    //这个函数用于设置闹钟
    void SetClock(u16 x,u16 y);
    u8 clock_check(void);
    void clock(void);
		//u16 timer[6];
    //该函数将会在闹钟设定时间到达时返回1，否则返回0
    extern u16 time_clock[6];
#endif
