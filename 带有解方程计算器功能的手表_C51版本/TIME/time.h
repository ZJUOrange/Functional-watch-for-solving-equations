#ifndef __TIMER_H
#define __TIMER_H	  
#include "sys.h"
#include "lcd.h"
#include "simplesys.h"
#include "ourtest.h"
#include "delay.h"
#include "usart.h"

extern u16 timer[6];
//timer[5] 年
//timer[4] 月
//timer[3] 日
//timer[2] 时
//timer[1] 分
//timer[0] 秒
extern u16 week;

void Disp_Time(u8 x,u8 y,u8 size);			//在制定位置开始显示时间
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);	//在指定位置显示星期

u8 timeinit(void);									//自己的初始化RTC，返回，0，失败；1
u8 leapyear(u16 year);							//平年,闰年判断
u8 timeget(void);										//获取时间
u8 weekget(u16 year, u16 month, u16 day); //获取星期，自己的函数
u8 timeset(u16 syear,u16 smon,u16 sday,u16 hour,u16 min,u16 sec);	//设置时间自己的函数
u8 timeAlarmset(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);		//设置时间自己的函数
u8 showweek(u16 x,u16 y,u16 week);							//显示当前日期
#endif
