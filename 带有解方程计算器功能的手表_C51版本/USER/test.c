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
 	Stm32_Clock_Init(9);		//设置时钟,72Mhz 
  uart_init(72,115200);
	delay_init(72);				//延时初始化  
 	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化    
	gui_init();	  				//gui初始化
	tp_dev.init();       //触摸屏初始化
	timeinit();
	FLASH_SD_init();
	BEEP_Init();
}

//main函数	  					
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





