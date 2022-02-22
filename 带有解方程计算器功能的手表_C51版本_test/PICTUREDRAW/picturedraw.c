#include "picturedraw.h"

//������picture_play
//���ܣ����뱳��������ָ��λ��
//x,yΪͼƬ���Ͻ����꣬modeΪ��ʾģʽ��1Ϊ�죬0Ϊ����
//���سߴ����0x28 ��ͼƬ��ʽ��0x08
u8 picture_play(const u8 *filename,u16 x,u16 y,u16 width,u16 height,u8 mode)
{
	 
	u8 res;
	u8 temp;
	if((x+width)>lcddev.width)return PIC_SIZE_ERR;		//x���곬��Χ
	if((y+height)>lcddev.height)return PIC_SIZE_ERR;		//y���곬��Χ
	if(width==0||height==0)return PIC_SIZE_ERR;	//�����趨����
	if(pic_phy.fillcolor==NULL)
	{
		mode=0;       //��ɫ��亯��δʵ��,���ܿ�����ʾ
	}
	//�趨��ʼ��ʾ��������볤��
	picinfo.S_YOFF=y;
	picinfo.S_XOFF=x;
	picinfo.S_Height=height;
	picinfo.S_Width=width;
	//�ļ���ȡ�����		 
	temp=f_typetell((u8*)filename);	//�õ��ļ�������
	switch(temp)
	{											  
		case T_BMP:
			res=stdbmp_decode(filename); 				//����bmp	  	  
			break;
		case T_JPG:
		case T_JPEG:
			res=jpg_decode(filename,mode);				//����JPG/JPEG	  	  
			break;
		case T_GIF:
			res=gif_decode(filename,x,y,width,height);	//����gif  	  
			break;
		default:
	 		res=0x08;  						//��ͼƬ��ʽ
			break;
	}  											   
	return res;
}
