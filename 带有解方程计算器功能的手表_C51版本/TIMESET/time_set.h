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
    //                 ʱ��������غ���
    //===================================================
    //����������� ��ť��ɫ��ʼ��
    u8 SetBtn_color_init(_btn_obj * ptr);
    //���������������ʱ���ֵ���Ϣ
    u8 SetTime(u16 x,u16 y,u16*timer);
    //���������������ʱ��
    void SetTime_HM(u16 x,u16 y);
    //====================================================
    //                 ������غ���
    //===================================================
    //�������������������
    void SetClock(u16 x,u16 y);
    u8 clock_check(void);
    void clock(void);
		//u16 timer[6];
    //�ú��������������趨ʱ�䵽��ʱ����1�����򷵻�0
    extern u16 time_clock[6];
#endif
