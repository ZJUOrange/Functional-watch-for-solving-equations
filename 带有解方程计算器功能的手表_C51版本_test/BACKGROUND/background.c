#include "background.h"

u8 background_draw_flag;  //�ж��Ƿ���Ҫˢ��ͼƬ
u8 bgd;   //��¼ѡ��ı���ͼƬ

//����ͼƬ·�� 240*240
u8*const background_path[4]=             
{
	"0:PICTURE/1.1.jpg",
	"0:PICTURE/1.2.jpg",
	"0:PICTURE/1.3.jpg",
	"0:PICTURE/1.4.jpg",
};	

//����ͼƬ·�� 80*80
u8*const background_icos_path[4]=             
{
	"0:PICTURE/1.1_80.bmp",
	"0:PICTURE/1.2_80.bmp",
	"0:PICTURE/1.3_80.bmp",
	"0:PICTURE/1.4_80.bmp",
};	

//��������FLASH_SD_init
//���ܣ�����SD����FLASH����غ�����ʼ��
void FLASH_SD_init(void)
{
	W25QXX_Init();				//��ʼ��W25Q128
 	AT24CXX_Init();    			//EEPROM��ʼ��
	piclib_init();				//piclib��ʼ��	  
	exfuns_init();				//FATFS �����ڴ�
  f_mount(fs[0],"0:",1); 		//����SD��  
  f_mount(fs[1],"1:",1); 		//���ع���FLASH.  
}
//��������background_init
//���ܣ���ʼ������
u8 background_init(void)
{
	u8 res;
	if(background_draw_flag)return 0;
	LCD_Clear(WHITE);
//	circle_fill(120,120,120,0);
  res=picture_play(background_path[bgd],0,0,240,240,1);
	return res;
}

//��������background_choose
//���ܣ����뱳��ѡ�����
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
        btn[i*2+j]=pic_btn_creat(76+88*j,76+88*i,40,"1",background_icos_path[i*2+j]);			//������ť
			}
    }
		for(i=0;i<4;i++)
		{
			pic_btn_draw(btn[i],1);
		}
		background_choose_init_flag=1;
	}
  touch_scan();
	delay_ms(50);		//��ʱһ��ʱ�ӽ���
	for(i=0;i<4;i++)
	{
    res=pic_btn_check(btn[i],tou_dev.touchflag);//ȷ�ϰ�ť���
		if(res==0x20)    //����Ч����(������㰴)
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

//��������circle_fill
//���ܣ����һ��ʵ��Բ
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

//��������background_play
//���ܣ�����򵥴�ɫԲ��ɫ��������ָ��λ��
//x0,y0ΪԲ��
void circle_play(u16 x0, u16 y0, u16 r, u16 color)
{
	LCD_Clear(WHITE);
	circle_fill(x0,y0,r,color);
}


