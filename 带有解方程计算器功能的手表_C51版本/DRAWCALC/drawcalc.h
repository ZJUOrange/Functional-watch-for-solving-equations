#include "newtouch.h"
#include "time.h"
#include "simplesys.h"
#include "string.h"
#include "calc.h"
#include "picturedraw.h"

#ifndef _DARWCALC_H_
#define _DARWCALC_H_
//�洢�ַ���
extern char calc_buffer[100];
extern char calc_str[200];
extern char long_str[19];
extern u8 mode_flag;				//Ĭ��,0Ϊ���ּ���ģʽ;1Ϊţ�ٷ��ⷽ��
extern u8 twinkle_cnt;
extern u8 shift_flag;				//shift��

u8 calc_init(_btn_obj * ptr);			//��ť��ʼ��
u8 calc_draw_button(u16 x,u16 y,char *str);		//���ư�ť
u8 calc_delete(char *str);
void buff_show(char* str);
void result_show(double x);
void mode_show(u8 mode);
void twinkle(void);

#endif
