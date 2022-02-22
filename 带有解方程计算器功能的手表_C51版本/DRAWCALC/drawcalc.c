#include "drawcalc.h"
//char calc_buffer[200]="\0";
char calc_str[200]="\0";
char long_str[19]="\0";
u8 mode_flag=0;
u8 twinkle_cnt=0;
u8 shift_flag=0;
//计算器按钮初始化
u8 calc_init(_btn_obj * ptr)
{
	ptr->bcfucolor=BLACK;//松开时为黑色
	ptr->bcfdcolor=WHITE;//按下时为白色			
	ptr->bkctbl[0]=0X453A;//边框颜色
	ptr->bkctbl[1]=0X5DDC;//第一行的颜色				
	ptr->bkctbl[2]=0XAAAA;
	ptr->bkctbl[3]=ORANGE;//下半部分颜色
	return 0;
}

u8 calc_init_flag=0;

//计算器按钮绘制
u8 calc_draw_button(u16 x,u16 y,char *str)
{
	u8 selx=0;
	u8 i=0;
	static _btn_obj * calc_button[19];
	if(!calc_init_flag)
	{
		LCD_Clear(WHITE);
		picture_play("0:PICTURE/calc_bgd.bmp",0,0,240,240,1);
		LCD_Fill(76,15,164,31,BLUE);
		LCD_Fill(44,40,196,55,GREEN);
		LCD_Fill(44,56,196,72,GREEN);
		buff_show(calc_buffer);
		mode_show(mode_flag);
		LCDShowString(44,56,152,20,16,(u8*)"                   ");
		calc_button[0]=btn_creat(x+20,y+55,40,30,0,0x02);			//创建按钮
		calc_button[1]=btn_creat(x+20+40,y+55,40,30,0,0x02);			//创建按钮
		calc_button[2]=btn_creat(x+20+40*2,y+55,40,30,0,0x02);			//创建按钮
		calc_button[3]=btn_creat(x+20+40*3,y+55,40,30,0,0x02);			//创建按钮
		calc_button[4]=btn_creat(x+20+40*4,y+55,40,30,0,0x02);			//创建按钮
		calc_button[5]=btn_creat(x+20,y+55+30,40,30,0,0x02);			//创建按钮
		calc_button[6]=btn_creat(x+20+40,y+55+30,40,30,0,0x02);			//创建按钮
		calc_button[7]=btn_creat(x+20+40*2,y+55+30,40,30,0,0x02);			//创建按钮
		calc_button[8]=btn_creat(x+20+40*3,y+55+30,40,30,0,0x02);			//创建按钮
		calc_button[9]=btn_creat(x+20+40*4,y+55+30,40,30,0,0x02);			//创建按钮
		calc_button[10]=btn_creat(x+20,y+55+30*2,40,30,0,0x02);			//创建按钮
		calc_button[11]=btn_creat(x+20+40,y+55+30*2,40,30,0,0x02);			//创建按钮
		calc_button[12]=btn_creat(x+20+40*2,y+55+30*2,40,30,0,0x02);			//创建按钮
		calc_button[13]=btn_creat(x+20+40*3,y+55+30*2,40,30,0,0x02);			//创建按钮
		calc_button[14]=btn_creat(x+20+40*4,y+55+30*2,40,30,0,0x02);			//创建按钮
		calc_button[15]=btn_creat(x+20+40,y+55+30*3,40,30,0,0x02);			//创建按钮
		calc_button[16]=btn_creat(x+20+40*2,y+55+30*3,40,30,0,0x02);			//创建按钮
		calc_button[17]=btn_creat(x+20+40*3,y+55+30*3,40,30,0,0x02);			//创建按钮
		calc_button[18]=btn_creat(x+20+40*2-10,y+55+30*4,60,30,0,0x02);	//创建按钮
		for(i=0;i<19;i++)//按钮标题初始化
		{
			calc_init(calc_button[i]);
			if(i==0)calc_button[i]->caption = "1/M";
			if(i==1)calc_button[i]->caption = "2/l";
			if(i==2)calc_button[i]->caption = "3/^";
			if(i==3)calc_button[i]->caption = "+";
			if(i==4)calc_button[i]->caption = "D/C";
			if(i==5)calc_button[i]->caption = "4/s";
			if(i==6)calc_button[i]->caption = "5/c";
			if(i==7)calc_button[i]->caption = "6/t";
			if(i==8)calc_button[i]->caption = "-";
			if(i==9)calc_button[i]->caption = "x";
			if(i==10)calc_button[i]->caption = "7/(";
			if(i==11)calc_button[i]->caption = "8/)";
			if(i==12)calc_button[i]->caption = "9/%";
			if(i==13)calc_button[i]->caption = "*";
			if(i==14)calc_button[i]->caption = "=";
			if(i==15)calc_button[i]->caption = "0";
			if(i==16)calc_button[i]->caption = ".";
			if(i==17)calc_button[i]->caption = "/";
			if(i==18)calc_button[i]->caption = "shift";
		}
		tou_dev_clear();
		calc_init_flag=1;
	}
	touch_scan();
	delay_ms(50);
	twinkle();
	for(i=0;i<19;i++)
	{
		//通过按钮的区域判断是否点击该按钮
		selx=simple_btn_check(calc_button[i],tou_dev.touchflag);//确认按钮检测
		if(selx==0x20 && shift_flag==0) //点按状态下的输出
		{
			switch(i)
			{
				case 0://输出"1"
					//strcpy(calc_buffer,"aa");
					strcat(calc_buffer,"1");
					break;
				case 1://输出"2"
					strcat(calc_buffer,"2");
					break;
				case 2://输出"3"
					strcat(calc_buffer,"3");
					break;
				case 3://输出"+"
					strcat(calc_buffer,"+");
					break;
				case 4://
					//strcpy(calc_buffer,"");
					calc_delete(calc_buffer);
					break;
				case 5://输出"4"
					strcat(calc_buffer,"4");
					break;
				case 6://输出"5"
					strcat(calc_buffer,"5");
					break;
				case 7://输出"6"
					strcat(calc_buffer,"6");
					break;
				case 8://输出"-"
					strcat(calc_buffer,"-");
					break;
				case 9://输出"(x)"
					strcat(calc_buffer,"(x)");
					break;
				case 10://输出"7"
					strcat(calc_buffer,"7");
					break;
				case 11://输出"8"
					strcat(calc_buffer,"8");
					break;
				case 12://输出"9"
					strcat(calc_buffer,"9");
					break;
				case 13://输出"*"
					strcat(calc_buffer,"*");
					break;
				case 14://输出结果ANSWER

					calc_error_flag=0;
					if(mode_flag)
					{
						ANSWER=Newtown_5(str,ANSWER,ANSWER-1);
					}
					else
					{
						ANSWER=eval_safe(calc_buffer,ANSWER);
					}
					if(!calc_error_flag)
					{
						result_show(ANSWER);
					}
					else
					{
						LCDShowString(44,56,152,20,16,(u8*)"         Math Error");
					}
					break;
				case 15://输出"0"
					strcat(calc_buffer,"0");
					break;
				case 16://输出"."
					strcat(calc_buffer,".");
					break;
				case 17://输出"/"
					strcat(calc_buffer,"/");
					break;
				case 18:
					shift_flag=1;
					calc_button[18]->bkctbl[2]=ORANGE;
					break;
				default:
					break;
			}
			selx=0;
			buff_show(calc_buffer);
		}
		if(selx==0x90 || (shift_flag && selx==0x20)) //长按状态下的输出
		{ 
			tou_dev_clear();
			shift_flag=0;
			calc_button[18]->bkctbl[2]=0xAAAA;
			switch(i)
			{
				case 0://切换模式
					mode_flag++;
					mode_flag %= 2;
					mode_show(mode_flag);
					break;
				case 1://输出'l'
					strcat(calc_buffer,"l");
					break;
				case 2://输出"^"
					strcat(calc_buffer,"p");
					break;
				case 4://清除整个字符串
					strcpy(calc_buffer,"");
					LCDShowString(44,56,152,20,16,(u8*)"                   ");
					break;
				case 5://输出"s"
					strcat(calc_buffer,"s");
					break;
				case 6://输出"c"
					strcat(calc_buffer,"c");
					break;
				case 7://输出"t"
					strcat(calc_buffer,"t");
					break;
				case 10://输出"("
					strcat(calc_buffer,"(");
					break;
				case 11://输出")"
					strcat(calc_buffer,")");
					break;
				case 12://输出"%"
					strcat(calc_buffer,"%");
					break;
				default:
					break;
 			}
			buff_show(calc_buffer);
		}
	}
		//退出识别		
	if(tou_dev.touchflag == 0x82)
	{
		for(i=0;i<18;i++)btn_delete(calc_button[i]);//删除按钮
		calc_init_flag=0;
		sys_return();
	}
	return 0;
}

u8 calc_delete(char *str)
{
	u8 len;
	len=strlen(str);
	if(str[len-1]==')')
	{
		if(str[len-2]=='x')
		{
			str[len-3] = '\0';
		}
	}
	else
	{
		str[len-1] = '\0';
	}
	return 0;
}

void buff_show(char* str)
{
	u8 i;
	u8 len;	
	len=strlen(str);
	for(i=0;i<100;i++)calc_str[i]='\0';
	for(i=0;i<19;i++)long_str[i]='\0';
	for(i=0;i<len;i++)
	{
		switch(str[i])
		{
			case 's':strcat(calc_str,"sin");break;
			case 'c':strcat(calc_str,"cos");break;
			case 't':strcat(calc_str,"tan");break;
			case 'l':strcat(calc_str,"log");break;
			case 'p':strcat(calc_str,"^");break;
			case '(':strcat(calc_str,"(");break;
			case ')':strcat(calc_str,")");break;
			case 'x':calc_str[strlen(calc_str)-1]='\0';strcat(calc_str,"x");i++;break;
			case '0':strcat(calc_str,"0");break;
			case '1':strcat(calc_str,"1");break;
			case '2':strcat(calc_str,"2");break;
			case '3':strcat(calc_str,"3");break;
			case '4':strcat(calc_str,"4");break;
			case '5':strcat(calc_str,"5");break;
			case '6':strcat(calc_str,"6");break;
			case '7':strcat(calc_str,"7");break;
			case '8':strcat(calc_str,"8");break;
			case '9':strcat(calc_str,"9");break;
			case '+':strcat(calc_str,"+");break;
			case '-':strcat(calc_str,"-");break;
			case '*':strcat(calc_str,"*");break;
			case '/':strcat(calc_str,"/");break;
			case '.':strcat(calc_str,".");break;
			case '%':strcat(calc_str,"%");break;
			default:break;
		}
	}
	len=strlen(calc_str);
	LCD_Fill(44,40,196,56,GREEN);
	if(len>=19)
	{
		for(i=0;i<18;i++)
		{
			long_str[i]=calc_str[len-18+i];
			strcat(long_str," ");
			LCDShowString(44,40,152,20,16,(u8*)long_str);
		}
	}
	else 
	{
		for(i=0;i<19-len;i++)
		{
			strcat(calc_str," ");
		}
		LCDShowString(44,40,152,20,16,(u8*)calc_str);
	}
}

void result_show(double x)
{
	u8 len,i;
	u32 y;
	char result[30]="\0";
	char result_str[19]="\0";
	LCD_Fill(44,56,196,72,GREEN);
	y=(int)x;
	if(x-(double)y<1e-6&&x-(double)y>-1e-6)
	{
		sprintf(result,"%d",y);
	}
	else
	{
		sprintf(result,"%lf",x);
	}
	len=strlen(result);
	for(i=0;i<19-len;i++)result_str[i]=' ';
	strcat(result_str,result);
	LCDShowString(44,56,152,20,16,(u8*)result_str);
}

void mode_show(u8 mode)
{
	LCD_Fill(76,15,164,31,GREEN);
	switch(mode)
	{
		case 0:LCDShowString(76,15,88,20,16," calculate ");break;
		case 1:LCDShowString(76,15,88,20,16,"solve for x");break;
		default:break;
	}
}

void twinkle(void)
{
	u8 len,i;
	if(twinkle_cnt>8)
	{
		twinkle_cnt=twinkle_cnt%8;
		len=strlen(long_str);
		if(len==19)
		{
			if(long_str[18]==' ')long_str[18]='_';
			else long_str[18]=' ';
			LCDShowString(44,40,152,20,16,(u8*)long_str);
		}
		else 
		{
			for(i=0;i<19;i++)
			{
				if(calc_str[i]==' ')
				{
					calc_str[i]='_';
					break;
				}
				else if(calc_str[i]=='_')
				{
					calc_str[i]=' ';
					break;
				}
			}
			LCDShowString(44,40,152,20,16,(u8*)calc_str);
		}
	}
	twinkle_cnt++;
}
