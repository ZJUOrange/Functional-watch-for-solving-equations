#include "picturedraw.h"

//函数名picture_play
//功能：导入背景至表盘指定位置
//x,y为图片左上角坐标，mode为显示模式（1为快，0为慢）
//返回尺寸错误：0x28 非图片格式：0x08
u8 picture_play(const u8 *filename,u16 x,u16 y,u16 width,u16 height,u8 mode)
{
	 
	u8 res;
	u8 temp;
	if((x+width)>lcddev.width)return PIC_SIZE_ERR;		//x坐标超范围
	if((y+height)>lcddev.height)return PIC_SIZE_ERR;		//y坐标超范围
	if(width==0||height==0)return PIC_SIZE_ERR;	//窗口设定错误
	if(pic_phy.fillcolor==NULL)
	{
		mode=0;       //颜色填充函数未实现,不能快速显示
	}
	//设定开始显示的坐标点与长宽
	picinfo.S_YOFF=y;
	picinfo.S_XOFF=x;
	picinfo.S_Height=height;
	picinfo.S_Width=width;
	//文件读取与解码		 
	temp=f_typetell((u8*)filename);	//得到文件的类型
	switch(temp)
	{											  
		case T_BMP:
			res=stdbmp_decode(filename); 				//解码bmp	  	  
			break;
		case T_JPG:
		case T_JPEG:
			res=jpg_decode(filename,mode);				//解码JPG/JPEG	  	  
			break;
		case T_GIF:
			res=gif_decode(filename,x,y,width,height);	//解码gif  	  
			break;
		default:
	 		res=0x08;  						//非图片格式
			break;
	}  											   
	return res;
}
