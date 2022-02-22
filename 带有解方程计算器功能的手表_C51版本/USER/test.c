#include "sys.h"
#include "delay.h"  
#include "usart.h"     
#include "newtouch.h"
#include "time_set.h"
#include "colorset.h"
#include "led.h"
#include "background.h"
#include "time.h"
#include "menu.h"
#include "drawcalc.h"
#include "calc.h"
#include "ourtest.h"
vu8 system_task_return;

void system_init(void)
{
 	Stm32_Clock_Init(9);		//����ʱ��,72Mhz 
  uart_init(72,115200);
	delay_init(72);				//��ʱ��ʼ��  
 	LED_Init();					//��ʼ��LED 
 	LCD_Init();					//LCD��ʼ��    
	gui_init();	  				//gui��ʼ��
	tp_dev.init();       //��������ʼ��
	timeinit();
	FLASH_SD_init();
	BEEP_Init();
}

//main����	  					
int main(void)
{ 
	#if TEST ==1
	test_time();
	test_Newtown_5();
	#endif
	#if TEST == 0
	system_init();
  while(1) 
	{	
		clock();
		sys_f_slex1();
	}
	#endif
	
}





