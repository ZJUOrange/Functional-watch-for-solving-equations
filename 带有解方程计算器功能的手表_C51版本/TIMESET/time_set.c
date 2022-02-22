#include "time_set.h"
u16 mycolor=0X5DDC;
u8 clock_flag=1;
u8 setclock_flag=0;
u8 SetBtn_color_init(_btn_obj * ptr)
{
			ptr->bcfucolor=BLACK;//松开时为黑色
			ptr->bcfdcolor=WHITE;//按下时为白色			
			ptr->bkctbl[0]=0X453A;//边框颜色
			ptr->bkctbl[1]=0X5DDC;//第一行的颜色				
		//	ptr->bkctbl[2]=0X5DDC;//上半部分颜色
			ptr->bkctbl[2]=mycolor;
			ptr->bkctbl[3]=ORANGE;//下半部分颜色
	    return 0;
}

u16 time_clock[6]={0,0,0,0,0,0};
//从参考例程修改而来，xzc
u8 SetTime_init_flag=0;
u8 SetTime(u16 x,u16 y,u16*timer) 
{
	u8 selx=0;
	u8 i=0;
	static _btn_obj * tbtn[4];		//总共六个按钮:0,时钟加按钮;1,时钟减按钮;2,分钟加按钮;3,分钟减按钮;4,确认按钮;5,取消按钮.	
	if(!SetTime_init_flag){
		LCD_Clear(WHITE);  
		picture_play("0:PICTURE/clock_bgd.bmp",0,0,240,240,1);  //V2.5
		tbtn[0]=btn_creat(x,y,40,25,0,0x02);			//创建按钮V2.5
		tbtn[1]=btn_creat(x,y+55,40,25,0,0x02);		//创建按钮V2.5
		tbtn[2]=btn_creat(x+80,y,40,25,0,0x02);		//创建按钮V2.5
		tbtn[3]=btn_creat(x+80,y+55,40,25,0,0x02);		//创建按钮V2.5
		for(i=0;i<4;i++)//这一段是按钮初始化
		{
				SetBtn_color_init(tbtn[i]);
				if(i==0||i==2)tbtn[i]->caption = "+";
				if(i==1||i==3)tbtn[i]->caption = "-";			
		}
			tou_dev_clear();
		SetTime_init_flag=1;
		}
			touch_scan();
			delay_ms(50);		//延时一个时钟节拍
			for(i=0;i<4;i++)
			{
				//通过按钮的区域判断是否点击该按钮
				selx=simple_btn_check(tbtn[i],tou_dev.touchflag);//确认按钮检测
				if(selx)
				{
					
						switch(i)
						{
							case 0://时钟增加按钮按下了
								if(timer[2]==23) timer[2]=0;
								else timer[2]++;
								if(!setclock_flag) timeset(timer[5],timer[4],timer[3],timer[2],timer[1],timer[0]);
								break;
							case 1://时钟减少按钮按下了	  
								if(timer[2]) timer[2]--;
								else timer[2]=23;
							  if(!setclock_flag) timeset(timer[5],timer[4],timer[3],timer[2],timer[1],timer[0]);
								break;
							case 2://分钟增加按钮按下了
								(timer[1])++;
								if(timer[1]>59) timer[1]=0;
								if(!setclock_flag) timeset(timer[5],timer[4],timer[3],timer[2],timer[1],timer[0]);
								break;
							case 3://分钟减少按钮按下了	  
								if(timer[1])(timer[1])--;
								else timer[1]=59;
							  if(!setclock_flag) timeset(timer[5],timer[4],timer[3],timer[2],timer[1],timer[0]);
								break;
 						}
						//设置时钟  
				}
		}
			//退出识别
	if(tou_dev.touchflag == 0x82){
		for(i=0;i<4;i++)btn_delete(tbtn[i]);//删除按钮
		SetTime_init_flag=0;
		sys_return();
	}
	
		return 0;
}
void SetTime_HM(u16 x,u16 y)
{
	setclock_flag=0;
	SetTime(x,y,timer);
	LCD_ShowNum(72,112,timer[2],2,16);		//V2.5							  
	LCD_ShowNum(152,112,timer[1],2,16);		//V2.5
}

void SetClock(u16 x,u16 y) 
{
	setclock_flag=1;
	SetTime(x,y,time_clock);
	LCD_ShowNum(72,112,time_clock[2],2,16);		//V2.5							  
	LCD_ShowNum(152,112,time_clock[1],2,16);		//V2.5
}
/*if(clock_check) Buffer()*/
u8 clock_check(void)
{   u8 i=0;
    for (i=1;i<3;i++){
        if(timer[i]!=time_clock[i]){
            return 0;
        }
    }
    return timer[0]%2;
}
//BEEP_Init();
void clock(void)
{
  if(clock_flag &&clock_check()){
		BEEP=1;
  }
  else{
	  	BEEP=0;
  }
}

