# include<newtouch.h>
//==========================================================================================================
//                                      这一部分是新的触屏扫描函数
//========================================================================================================
_scan_touch_  tou_dev={
	0,0,0,0,0,0,0,0,0,0
};
//该函数用于清零tou_dev内的寄存器
void tou_dev_clear_core(void)
{
	tou_dev.touchflag=0;
	tou_dev.xsta=0;
	tou_dev.ysta=0;
	tou_dev.stacnt=0;
}
//该函数与tou_dev_clear_core的区别在于该函数会开启禁止标记
u8 tou_dev_clear(void)
{
	tou_dev_clear_core();
	tou_dev.touchflag|=1<<6;
	return 0;
}

//这个函数用于扫描触屏操作，并修改寄存器tou_dev.flag
//[7]:第一次按下标志
//[5]短按标志
//[4]长按标志16
//[3]下滑标志8
//[2]上滑标志4
//[1]左滑标志2
//[0]右滑标志1
//当存在滑动标志的时候，i返回移动的位置 使用方式为 x+=i
//在tou_dev.flag&0x20!=0时说明是短按
/*使用案例
		touch_scan();
		switch(tou_dev.touchflag){
			case 0x81: Show_Str(30,10,200,16,"right",16,0);delay_ms(2000);break;
			case 0x82: Show_Str(30,10,200,16,"left ",16,0);delay_ms(2000);break;
			case 0x84: Show_Str(30,10,200,16,"down ",16,0);delay_ms(2000);break;
			case 0x88: Show_Str(30,10,200,16,"up   ",16,0);delay_ms(2000);break;
			case 0x90: Show_Str(30,10,200,16,"long ",16,0);delay_ms(2000);break;
			case 0x20: Show_Str(30,10,200,16,"short",16,0);delay_ms(2000);break;
			default:break;
		}
*/
u8 touch_scan(void)
{		
		u8 i=0;
		u16 movexcnt=0;	//得到x滑动点数
		u16 moveycnt=0;//得到y滑动点数
		tp_dev.scan(0);
	  tou_dev.touchflag&=0xDF;//短按标志位清零
		if(tp_dev.sta&TP_PRES_DOWN)
		{
			if(tou_dev.touchflag&0X80)//已经被标记了
			{
				
				movexcnt=tou_dev.xsta;
				moveycnt=tou_dev.ysta;
				if(gui_disabs(tou_dev.curxpos,tp_dev.x[0])>=SPB_MOVE_WIN)//移动大于等于SPB_MOVE_WIN个点
				{
					if(movexcnt<SPB_MOVE_MIN/SPB_MOVE_WIN) tou_dev.xsta++;//点数增加1
				}
				else{tou_dev.stacnt++;}
				if(gui_disabs(tou_dev.curypos,tp_dev.y[0])>=SPB_MOVE_WIN)//移动大于等于SPB_MOVE_WIN个点
				{
					if(moveycnt<SPB_MOVE_MIN/SPB_MOVE_WIN) tou_dev.ysta++;//点数增加1
				}
				else tou_dev.stacnt++;

			}			
		
			tou_dev.curxpos=tp_dev.x[0];				//记录当前坐标
			tou_dev.curypos=tp_dev.y[0];				//记录当前坐标
			//下面是对滑动，按下，等等操作的判断
			if((tou_dev.touchflag&0XC0)==0)			//按键第一次被按下
			{
				if(tou_dev.curxpos>4096||tou_dev.curypos>4096) return 0xFF;//非法的数据
				tou_dev_clear_core();				 	//状态清零
				tou_dev.touchflag|=1<<7;				//标记第一次按下	    
				tou_dev.oldxpos=tp_dev.x[0];			//记录按下时的x坐标 
				tou_dev.oldypos=tp_dev.y[0];			//记录按下时的x坐标 
			}else if(tou_dev.touchflag&0X5F)			//有滑动或者禁止标记
			{    
				tou_dev.oldxpos=tp_dev.x[0];
				tou_dev.oldypos=tp_dev.y[0];	
			}else if(((tou_dev.curxpos-tou_dev.oldxpos)>=TOUCH_MOVE_MIN) && (movexcnt>=SPB_MOVE_MIN/SPB_MOVE_WIN))//滑动距离超过SPB_MOVE_MIN,并且检测到的有效滑动数不少于SPB_MOVE_MIN/SPB_MOVE_WIN.
			{ 
				tou_dev.touchflag|=1;//标记滑动	,优先标记左移右移
			}
			else if(((tou_dev.oldxpos-tou_dev.curxpos)>=TOUCH_MOVE_MIN)&&(movexcnt>=SPB_MOVE_MIN/SPB_MOVE_WIN))//滑动距离超过SPB_MOVE_MIN,并且检测到的有效滑动数不少于SPB_MOVE_MIN/SPB_MOVE_WIN.
			{ 
				tou_dev.touchflag|=1<<1;//标记滑动	
			}else if(((tou_dev.curypos-tou_dev.oldypos)>=TOUCH_MOVE_MIN)&&(moveycnt>=SPB_MOVE_MIN/SPB_MOVE_WIN))//滑动距离超过SPB_MOVE_MIN,并且检测到的有效滑动数不少于SPB_MOVE_MIN/SPB_MOVE_WIN.
			{ 
				tou_dev.touchflag|=1<<2;//标记滑动	
			}else if(((tou_dev.oldypos-tou_dev.curypos)>=TOUCH_MOVE_MIN)&&(moveycnt>=SPB_MOVE_MIN/SPB_MOVE_WIN))//滑动距离超过SPB_MOVE_MIN,并且检测到的有效滑动数不少于SPB_MOVE_MIN/SPB_MOVE_WIN.
			{ 
				tou_dev.touchflag|=1<<3;//标记滑动	
			}else if(tou_dev.stacnt>TOUCH_LONG_MIN)//滑动距离超过SPB_MOVE_MIN,并且检测到的有效滑动数不少于SPB_MOVE_MIN/SPB_MOVE_WIN.
			{ 
				tou_dev.touchflag|=1<<4;//标记滑动	
			}
			switch(tou_dev.touchflag){
				case 0x81: i=tou_dev.curxpos-tou_dev.oldxpos;break;
				case 0x82: i=tou_dev.oldxpos-tou_dev.curxpos;break;
				case 0x84: i=tou_dev.curypos-tou_dev.oldypos;break;
				case 0x88: i=tou_dev.oldypos-tou_dev.curypos;break;
				default: i=0;
			}
		}
		else //按键松开了     // 判断点按
		{
			if(tou_dev.touchflag&0x80)//之前有按下
			{	  
				if(!(tou_dev.touchflag&0x5f)){//不存在禁止标志或其他标志则标记短按标志
					i=1;//i先充当一下暂时的标志物
				}
			}
			tou_dev_clear_core();//清空标志
			if(i){
				tou_dev.touchflag|=1<<5;
			}
		} 							
			
		return i;
}


/////////////////////////////////////////////////////////
//                            下方为新的按钮
////////////////////////////////////////////////////////

//删除按钮
//btn_del:要删除的按键
/*void btn_delete(_btn_obj * btn_del)
{
	if(btn_del==NULL)return;//非法的地址,直接退出
	if((btn_del->type&0X0f)==BTN_TYPE_TEXTA)btn_recover_backcolor(btn_del);//恢复背景色
	gui_memin_free(btn_del->bkctbl);
	gui_memin_free(btn_del);
}
*/


//检查	
//btnx:按键指针
//in_key:输入按键指针
//返回值:
//0,没有一次有效的操作	
//1,有一次有效操作		 					   
u8 simple_btn_check(_btn_obj * btnx,u8 MODE)
{
	u8 flag;
	flag = In_btn_check(btnx);
	simple_btn_draw(btnx,flag);
	if(flag){
		switch(MODE){
			case 0x90: return 0x90;break;//点按
			case 0x20: return 0x20;break;//长按
			default: return 0;
		}
		
	}
	
	return 0;
}
u8 In_btn_check(_btn_obj* ptr)
{
	if(ptr->top<tp_dev.y[0]&&tp_dev.y[0]<(ptr->top+ptr->height)&&ptr->left<tp_dev.x[0]&&tp_dev.x[0]<(ptr->left+ptr->width)){
		return 1;
	}
	else return 0;
}
void simple_btn_draw(_btn_obj * btnx,u8 flag)
{
	if(flag){
		    gui_draw_rectangle(btnx->left,btnx->top,btnx->width,btnx->height,GUI_COLOR_BLACK);		//外框
		  	gui_draw_rectangle(btnx->left+1,btnx->top+1,btnx->width-2,btnx->height-2,STD_BTN_RBOC);	//内框
				gui_fill_rectangle(btnx->left+2,btnx->top+2,btnx->width-4,btnx->height-4,btnx->bkctbl[3]);//填充内部	
	}
	else{
		gui_fill_rectangle(btnx->left+2,btnx->top+2,btnx->width-4,btnx->height-4,btnx->bkctbl[2]);//弹起的颜色
		  gui_draw_vline(btnx->left,btnx->top,btnx->height,STD_BTN_LTOC);			//左外线
		 	gui_draw_hline(btnx->left,btnx->top,btnx->width,STD_BTN_LTOC);			//上外线
			gui_draw_vline(btnx->left+1,btnx->top+1,btnx->height-2,STD_BTN_LTIC);	//左内线
		 	gui_draw_hline(btnx->left+1,btnx->top+1,btnx->width-2,STD_BTN_LTIC);	//上内线
			gui_draw_vline(btnx->left+btnx->width-1,btnx->top,btnx->height,STD_BTN_RBOC);		//右外线
		 	gui_draw_hline(btnx->left,btnx->top+btnx->height-1,btnx->width,STD_BTN_RBOC);		//下外线
			gui_draw_vline(btnx->left+btnx->width-2,btnx->top+1,btnx->height-2,STD_BTN_RBIC);	//右内线
		 	gui_draw_hline(btnx->left+1,btnx->top+btnx->height-2,btnx->width-2,STD_BTN_RBIC);	//右内线
	}
	gui_show_strmid(btnx->left+1,btnx->top+1,btnx->width,btnx->height,btnx->bcfdcolor,btnx->font,btnx->caption);//显示caption
}
//=============================以下为园型按钮====================================

pic_btn_obj * pic_btn_creat(u16 x,u16 y,u16 r,u8 *caption,u8 *picbtnpath)
{
 	pic_btn_obj * btn_crt;		  
	btn_crt=(pic_btn_obj*)gui_memin_malloc(sizeof(pic_btn_obj));//分配内存
	if(btn_crt==NULL)return NULL;//内存分配不够.
	btn_crt->x=x;
	btn_crt->y=y;
	btn_crt->r=r;
	btn_crt->r2=r*r;
	btn_crt->caption=caption;
	btn_crt->picbtnpath=picbtnpath;  
  return btn_crt;
}

u8 pic_btn_check(pic_btn_obj * btnx,u8 MODE){
	u8 flag;
	flag = In_pic_btn_check(btnx);
	//绘图部分,减少刷新次数
	//按钮逻辑部分
	if(flag){
		btnx->Mode = MODE;
		switch(MODE){
			case 0x90: return 0x90;break;//点按
			case 0x20: return 0x20;break;//长按
			default: btnx->Mode = 0;return 0;
		}
		
	}
	return 0;
}
u8 In_pic_btn_check(pic_btn_obj* ptr)
{	
	u16 dx;
	u16 dy;
	u16 R2;
	dx=tp_dev.x[0]-ptr->x;
	dy=tp_dev.y[0]-ptr->y;
	R2=dx*dx+dy*dy;
	if(R2<ptr->r2) return 1;
	else return 0;
}
void pic_btn_delete(pic_btn_obj * btn_del)
{
	if(btn_del==NULL)return;//非法的地址,直接退出
	gui_memin_free(btn_del);
}
void pic_btn_draw(pic_btn_obj * btnx,u8 Mode){
	picture_play(btnx->picbtnpath,btnx->x-btnx->r,btnx->y-btnx->r,2*btnx->r,2*btnx->r,1);
}