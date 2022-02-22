#include<colorset.h>
u16 color_set(u16 x,u16 y,u16 color){
	u8 selx=0;
	u8 i=0;
	u16 judge_flag=0;
	_btn_obj * tbtn[6];		//总共六个按钮:0,时钟加按钮;1,时钟减按钮;2,分钟加按钮;3,分钟减按钮;4,确认按钮;5,取消按钮.		  
 	tbtn[0]=btn_creat(x+20,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);			//创建按钮
	tbtn[1]=btn_creat(x+20,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[2]=btn_creat(x+20+60,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[3]=btn_creat(x+20+60,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[4]=btn_creat(x+20+60+60,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[5]=btn_creat(x+20+60+60,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	for(i=0;i<6;i++)//这一段是按钮初始化
	{
			//SetBtn_color_init(tbtn[i]);	
			if(i==0||i==2||i==4)tbtn[i]->caption = "+";
			if(i==1||i==3||i==5)tbtn[i]->caption = "-";			
	}
		for(i=0;i<6;i++)btn_draw(tbtn[i]);	//画按钮
		gui_fill_rectangle(x+20+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,RED);  		//显示颜色快
		gui_fill_rectangle(x+20+60+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,GREEN);  		//显示颜色快
		gui_fill_rectangle(x+20+120+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,BLUE);  		//显示颜色快
		while(1)
		{
			touch_scan();
			
			if(tou_dev.touchflag==0x82){sys_return();break; }      //左移
		//	app_show_nummid(80,100,200,SYSSET_EDIT_HEIGHT,tou_dev.stacnt,8,16,BLACK,SYSSET_EDIT_BACK_COLOR);
		//	app_show_nummid(60,42,200,SYSSET_EDIT_HEIGHT,tou_dev.touchflag,8,16,BLACK,SYSSET_EDIT_BACK_COLOR);
			delay_ms(50);		//延时一个时钟节拍
			for(i=0;i<6;i++)
			{
				gui_fill_rectangle(x+60+1,y+160,SYSSET_EDIT_WIDTH*2,SYSSET_EDIT_HEIGHT,color);  		//显示颜色快
				tbtn[i]->bkctbl[2]=color;
				//通过按钮的区域判断是否点击该按钮
				selx=simple_btn_check(tbtn[i],tou_dev.touchflag);//确认按钮检测	
				if(selx)
				{
						switch(i)
						{
							case 0://时钟增加按钮按下了
								if(color<0xF800) color+=0x0800;
								break;
							case 1://时钟减少按钮按下了	  
								if(color>0x0800)color-=0x0800;
								break;
							case 2://分钟增加按钮按下了
								judge_flag=color<<5;
								if(judge_flag<0xFC00) color+=0x0020;
								break;
							case 3://分钟减少按钮按下了	
								judge_flag=color<<5;
								if(judge_flag>0x0400) color-=0x0020;
								break;
							case 4://时钟增加按钮按下了
								judge_flag= color & 0x001F;
								if(judge_flag<0x0020) color+=0x0001;
								break;
							case 5://时钟减少按钮按下了	
								judge_flag= color & 0x001F;
								if(judge_flag)  color-=0x0001;
								break;
 						}
				}
			}
		//app_show_nummid(x+20,y+42+41,200,SYSSET_EDIT_HEIGHT,color,8,16,BLACK,SYSSET_EDIT_BACK_COLOR);
		//app_show_nummid(x+20+60,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,timer[1],2,16,BLACK,SYSSET_EDIT_BACK_COLOR);
		}
	for(i=0;i<6;i++)btn_delete(tbtn[i]);//删除按钮
		return color;
}
u16 cho_ply_color1=0X5DDC;
u16 cho_ply_color2=0X453A;
void cho_ply_colorset(){
	cho_ply_color1=color_set(100,200,cho_ply_color1);
	cho_ply_color2=color_set(100,200,cho_ply_color2);
}

