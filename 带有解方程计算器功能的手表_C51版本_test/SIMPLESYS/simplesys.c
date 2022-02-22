#include<simplesys.h>
u8 sys_slex1=0;
u8 sys_slex2=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                               基本操作逻辑函数区
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8 sys_return(void){
    if(sys_slex1)
		{	
			sys_slex1--;	
			LCD_Clear(WHITE);
		}
		tou_dev_clear();
    return 0;
}

u8 sys_next_state(void){
    if(sys_slex1<2) 
		{
			sys_slex1++;
			LCD_Clear(WHITE);
		}
		tou_dev_clear();
    return 0;
}

u8 sys_return0(void){
		sys_slex1=0;	
		LCD_Clear(WHITE);
		tou_dev_clear();
    return 0;
}
