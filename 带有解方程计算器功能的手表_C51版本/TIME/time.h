#ifndef __TIMER_H
#define __TIMER_H	  
#include "sys.h"
#include "lcd.h"
#include "simplesys.h"
#include "ourtest.h"
#include "delay.h"
#include "usart.h"

extern u16 timer[6];
//timer[5] ��
//timer[4] ��
//timer[3] ��
//timer[2] ʱ
//timer[1] ��
//timer[0] ��
extern u16 week;

void Disp_Time(u8 x,u8 y,u8 size);			//���ƶ�λ�ÿ�ʼ��ʾʱ��
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);	//��ָ��λ����ʾ����

u8 timeinit(void);									//�Լ��ĳ�ʼ��RTC�����أ�0��ʧ�ܣ�1
u8 leapyear(u16 year);							//ƽ��,�����ж�
u8 timeget(void);										//��ȡʱ��
u8 weekget(u16 year, u16 month, u16 day); //��ȡ���ڣ��Լ��ĺ���
u8 timeset(u16 syear,u16 smon,u16 sday,u16 hour,u16 min,u16 sec);	//����ʱ���Լ��ĺ���
u8 timeAlarmset(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);		//����ʱ���Լ��ĺ���
u8 showweek(u16 x,u16 y,u16 week);							//��ʾ��ǰ����
#endif
