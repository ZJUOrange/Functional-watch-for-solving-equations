#include "background.h"

u8 background_draw_flag;  //判断是否需要刷新图片
u8 bgd;   //记录选择的背景图片

//背景图片路径 240*240
u8*const background_path[4]=             
{
	"0:PICTURE/1.1.jpg",
	"0:PICTURE/1.2.jpg",
	"0:PICTURE/1.3.jpg",
	"0:PICTURE/1.4.jpg",
};	

//背景图片路径 80*80
u8*const background_icos_path[4]=             
{
	"0:PICTURE/1.1_80.bmp",
	"0:PICTURE/1.2_80.bmp",
	"0:PICTURE/1.3_80.bmp",
	"0:PICTURE/1.4_80.bmp",
};	

//函数名：FLASH_SD_init
//功能：挂载SD卡和FLASH及相关函数初始化
void FLASH_SD_init(void)
{
	W25QXX_Init();				//初始化W25Q128
 	AT24CXX_Init();    			//EEPROM初始化
	piclib_init();				//piclib初始化	  
	exfuns_init();				//FATFS 申请内存
  f_mount(fs[0],"0:",1); 		//挂载SD卡  
  f_mount(fs[1],"1:",1); 		//挂载挂载FLASH.  
}
//函数名：background_init
//功能：初始化背景
u8 background_init(void)
{
	u8 res;
	if(background_draw_flag)return 0;
	LCD_Clear(WHITE);
//	circle_fill(120,120,120,0);
  res=picture_play(background_path[bgd],0,0,240,240,1);
	return res;
}

//函数名：background_choose
//功能：进入背景选择界面
void background_choose(void)
{
	pic_btn_obj * btn[4];
	u8 i;
	u8 j;
	u8 res;
	static u8 background_choose_init_flag=0;
	if(!background_choose_init_flag)
	{
		circle_fill(120,120,120,0);
    for(i=0;i<2;i++){
			for(j=0;j<2;j++)
			{
        btn[i*2+j]=pic_btn_creat(76+88*j,76+88*i,40,"1",background_icos_path[i*2+j]);			//创建按钮
			}
    }
		for(i=0;i<4;i++)
		{
			pic_btn_draw(btn[i],1);
		}
		background_choose_init_flag=1;
	}
  touch_scan();
	delay_ms(50);		//延时一个时钟节拍
	for(i=0;i<4;i++)
	{
    res=pic_btn_check(btn[i],tou_dev.touchflag);//确认按钮检测
		if(res==0x20)    //有有效操作(长按或点按)
		{  
			bgd=i;
			sys_return0();
			background_choose_init_flag=0;
			for(i=0;i<4;i++)pic_btn_delete(btn[i]);
 		}
	}
	if(tou_dev.touchflag==0x82)
	{
		sys_return();
		background_choose_init_flag=0;
		for(i=0;i<4;i++)pic_btn_delete(btn[i]);
	}
}

//函数名：circle_fill
//功能：填充一个实心圆
void circle_fill(u16 x0, u16 y0, u16 r, u16 color)
{
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	LCD_Fill(x0-r,y0,x0+r,y0,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax) 
		{
			if (x>imax) 
			{
				LCD_Fill(x0-i+1,y0+x,x0+i-1,y0+x,color);
				LCD_Fill(x0-i+1,y0-x,x0+i-1,y0-x,color);
			}
			x--;
		}
		LCD_Fill(x0-x,y0+i,x0+x,y0+i,color);
		LCD_Fill(x0-x,y0-i,x0+x,y0-i,color);
	}
}

//函数名：background_play
//功能：导入简单纯色圆形色块至表盘指定位置
//x0,y0为圆心
void circle_play(u16 x0, u16 y0, u16 r, u16 color)
{
	LCD_Clear(WHITE);
	circle_fill(x0,y0,r,color);
}


