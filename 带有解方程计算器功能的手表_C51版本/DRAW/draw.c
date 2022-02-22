#include "draw.h"
#include "time.h"

u16 pointx_sec[200];
u16 pointy_sec[200];
u16 pointcolor_sec[200];
u16 pointx_min[200];
u16 pointy_min[200];
u16 pointcolor_min[200];
u16 pointx_hour[200];
u16 pointy_hour[200];
u16 pointcolor_hour[200];

//绘制表盘
//x,y		:圆心位置
//r			:半径
//d			:刻度长度及中心圆半径
void clock_dial(u16 x,u16 y,u16 r,u16 d)
{
	u16 px0,px1;
	u16 py0,py1; 
	u16 i; 
	LCD_Draw_Circle(x,y,r-1);		//画外圈
	for(i=0;i<60;i++)//画秒钟格
	{ 
		px0=x + (r-3)*sin((pi/30)*i); 
		py0=y - (r-3)*cos((pi/30)*i); 
		px1=x + r*sin((pi/30)*i); 
		py1=y - r*cos((pi/30)*i);  
		LCD_DrawLine(px0,py0,px1,py1);	
	}
	for(i=0;i<12;i++)//画小时格
	{ 
		px0=x+(r-6)*sin((pi/6)*i); 
		py0=y-(r-6)*cos((pi/6)*i); 
		px1=x+(r-3)*sin((pi/6)*i); 
		py1=y-(r-3)*cos((pi/6)*i);  
		LCD_DrawLine(px0,py0,px1,py1);		
	}
	for(i=0;i<4;i++)//画3小时格
	{ 
		px0=x+(r-8)*sin((pi/2)*i); 
		py0=y-(r-8)*cos((pi/2)*i); 
		px1=x+(r-6)*sin((pi/2)*i); 
		py1=y-(r-6)*cos((pi/2)*i);  
		LCD_DrawLine(px0,py0,px1,py1);		
	}
	LCD_Draw_Circle(x,y,d/2);		//画中心圈
}
u8 clockpoint_flag=0;
//绘制时针，秒针，分针
//x,y		:圆心坐标
void clock_Pointer(u16 x,u16 y,u16 r)
{
		static u16 px0,px1;
		static u16 py0,py1; 
	  static u16 last_second=0;
		static u16 current_second=0;
	
		static u16 px2,px3;
		static u16 py2,py3; 
		static u16 last_min=0;
		static u16 current_min=0;
	
		static u16 px4,px5;
		static u16 py4,py5; 
		static u16 last_hour=0;
		static u16 current_hour=0;
		u16 px10,py10;
		u16 px00,py00;
		//秒针
		current_second=timer[0];
		px0=x+sin((pi/30)*current_second);
		py0=y-cos((pi/30)*current_second);
		px1=x+3*(r-3)*sin((pi/30)*current_second); 
		py1=y-3*(r-3)*cos((pi/30)*current_second); 
		//分针
		current_min=timer[1];
		px2=x+sin((pi/30)*current_min+(pi/30)*current_second/60);
		py2=y-cos((pi/30)*current_min+(pi/30)*current_second/60);
		px3=x+2*(r-3)*sin((pi/30)*current_min+(pi/30)*current_second/60); 
		py3=y-2*(r-3)*cos((pi/30)*current_min+(pi/30)*current_second/60);
		//时针
		current_hour=timer[2];
		px4=x+sin((pi/6)*current_hour+(pi/6)*current_min/60);
		py4=y-cos((pi/6)*current_hour+(pi/6)*current_min/60);
		px5=x+(r-3)*sin((pi/6)*current_hour+(pi/6)*current_min/60); 
		py5=y-(r-3)*cos((pi/6)*current_hour+(pi/6)*current_min/60);
		
		if(clockpoint_flag)
		{
			
			line_info(px0,py0,px1,py1,pointx_sec,pointy_sec,pointcolor_sec,0);
			line_info(px2,py2,px3,py3,pointx_min,pointy_min,pointcolor_min,1);
			line_info(px4,py4,px5,py5,pointx_hour,pointy_hour,pointcolor_hour,1);
			clockpoint_flag=0;
		}
		
		if(last_second != current_second)
		{
			clearline(pointx_sec,pointy_sec,pointcolor_sec);
			clearline(pointx_min,pointy_min,pointcolor_min);
			clearline(pointx_hour,pointy_hour,pointcolor_hour);
			line_info(px0,py0,px1,py1,pointx_sec,pointy_sec,pointcolor_sec,0);
			line_info(px2,py2,px3,py3,pointx_min,pointy_min,pointcolor_min,1);
			line_info(px4,py4,px5,py5,pointx_hour,pointy_hour,pointcolor_hour,1);
			last_hour=current_hour;
			last_second =current_second;
			last_min = current_min;
		}
		
		drawline(px0,py0,px1,py1,0xAAAA,0);
		drawline(px2,py2,px3,py3,0xBBBB,1);
		drawline(px4,py4,px5,py5,WHITE,1);
}

//画点函数
void drawpoint(u16 x,u16 y,u16 color)
{
		LCD_SetCursor(x,y);		//设置光标位置 
		LCD_WriteRAM_Prepare();	//开始写入GRAM
		LCD->LCD_RAM=color; 
}

void drawline(u16 x1,u16 y1,u16 x2,u16 y2,u16 color,u16 size)
{
		u16 t; 
		int xerr=0,yerr=0,delta_x,delta_y,distance; 
		int incx,incy,uRow,uCol; 
		delta_x=x2-x1; //计算坐标增量 
		delta_y=y2-y1; 
		uRow=x1; 
		uCol=y1; 
		if(delta_x>0)incx=1; //设置单步方向 
		else if(delta_x==0)incx=0;//垂直线 
		else {incx=-1;delta_x=-delta_x;} 
		if(delta_y>0)incy=1; 
		else if(delta_y==0)incy=0;//水平线 
		else{incy=-1;delta_y=-delta_y;} 
		if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
		else distance=delta_y; 
		for(t=0;t<=distance;t++ )//画线输出 
		{  
			if(size==0)drawpoint(uRow,uCol,color);//画点 
			if(size==1)
			{
				drawpoint(uRow,uCol,color);//画点 
				drawpoint(uRow+1,uCol,color);//画点 
				drawpoint(uRow,uCol+1,color);//画点 
				drawpoint(uRow+1,uCol+1,color);//画点 
			}
			xerr+=delta_x ; 
			yerr+=delta_y ; 
			if(xerr>distance) 
			{ 
				xerr-=distance; 
				uRow+=incx; 
			} 
			if(yerr>distance) 
			{ 
				yerr-=distance; 
				uCol+=incy; 
			} 
		}  
}

//清除函数
void clearline(u16 *pointx,u16 *pointy,u16 *pointcolor)
{
		u16 i;
		for(i=0;i<200;i++)
		{
			if((pointx[i]+pointy[i])>0)
			{
				drawpoint(pointx[i],pointy[i],pointcolor[i]);
			}
			else break;
		}
}

//输入起始点终点信息，读取直线信息（具体点位及颜色），存放于pointx[],pointy[],pointcolor[]中
void line_info(u16 x1,u16 y1,u16 x2,u16 y2,u16 *pointx,u16 *pointy,u16 *pointcolor,u16 size)
{
		u16 t; 
		int xerr=0,yerr=0,delta_x,delta_y,distance; 
		int incx,incy,uRow,uCol,i=0; 
		for(t=0;t<200;t++)
		{
			pointx[t]=0;
			pointy[t]=0;
			pointcolor[t]=0;
		}
		delta_x=x2-x1; //计算坐标增量 
		delta_y=y2-y1; 
		uRow=x1; 
		uCol=y1; 
		if(delta_x>0)incx=1; //设置单步方向 
		else if(delta_x==0)incx=0;//垂直线 
		else {incx=-1;delta_x=-delta_x;} 
		if(delta_y>0)incy=1; 
		else if(delta_y==0)incy=0;//水平线 
		else{incy=-1;delta_y=-delta_y;} 
		if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
		else distance=delta_y; 
		for(t=0;t<=distance;t++ )//画线输出 
		{  

			if(size==0)
			{
				pointx[i]=uRow;
				pointy[i]=uCol;
				pointcolor[i]=LCD_ReadPoint(pointx[i],pointy[i]);
				i++;
			}
			if(size==1)
			{
				pointx[i]=uRow;
				pointy[i]=uCol;
				pointcolor[i]=LCD_ReadPoint(pointx[i],pointy[i]);
				i++;
				pointx[i]=uRow;
				pointy[i]=uCol+1;
				pointcolor[i]=LCD_ReadPoint(pointx[i],pointy[i]);
				i++;
				pointx[i]=uRow+1;
				pointy[i]=uCol;
				pointcolor[i]=LCD_ReadPoint(pointx[i],pointy[i]);
				i++;
				pointx[i]=uRow+1;
				pointy[i]=uCol+1;
				pointcolor[i]=LCD_ReadPoint(pointx[i],pointy[i]);
				i++;
			}
			xerr+=delta_x ; 
			yerr+=delta_y ; 
			if(xerr>distance) 
			{ 
				xerr-=distance; 
				uRow+=incx; 
			} 
			if(yerr>distance) 
			{ 
				yerr-=distance; 
				uCol+=incy; 
			} 
		}  
}

void LCDShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode,u16 color)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,color);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
} 

void LCDShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	u16 color;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
				color=LCD_ReadPoint(x,y);
        LCDShowChar(x,y,*p,size,0,color);
        x+=size/2;
        p++;
    }  
}

void LCDShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u16 color;
	u8 enshow=0;		
	color=LCD_ReadPoint(x,y);
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCDShowChar(x+(size/2)*t,y,' ',size,0,color);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCDShowChar(x+(size/2)*t,y,temp+'0',size,0,color); 
	}
} 
