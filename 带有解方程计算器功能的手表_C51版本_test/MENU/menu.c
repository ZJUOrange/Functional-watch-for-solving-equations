#include "menu.h"

//ѡ��ͼƬ·�� 80*80
u8*const choose_path[4]=             
{
	"0:PICTURE/alarm.bmp",
	"0:PICTURE/calc.bmp",
	"0:PICTURE/background.bmp",
	"0:PICTURE/clock.bmp",
};	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                               ѡ���߼�
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 sys_f_slex1(void){
    switch(sys_slex1){
        case 0:main_face();break;//��ʾ������
        case 1:choose_play();break;//��ʾѡ����棺
        case 2:sub_play();break;//�μ�������ʾ
			  case 3:number_face();break;
			default:break;
    }
    return 0;
}

u8 sub_play(void){
    switch(sys_slex2){
				case 0:SetClock(60,80);break;
        case 1:calc_draw_button(0,30,calc_buffer);break;
				case 2:background_choose();break;
				case 3:SetTime_HM(60,80);break;
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                               ������ϵ�к���
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������ʾ
u8 main_face(void){
    main_display();
    main_control();
    return 0;
}
u8 main_control(void){
    touch_scan();
        switch(tou_dev.touchflag){
			case 0x81: sys_next_state();break;//����
			case 0x82: sys_slex1=3;tou_dev_clear();background_draw_flag=0;break;       //����
			default:break;
		}  
        return 0;
}

u8 main_display(void){
		background_init();
		background_draw_flag=1;
		clock_dial(120,120,120,3);
		clock_Pointer(120,120,30);
    return 0;
}

u8 num_background_flag=0;

void number_face(){
	if(!num_background_flag){
		picture_play("0:PICTURE/num_bgd.bmp",0,0,240,240,1);
		num_background_flag=1;
	}
	showweek(60,125,week);
	touch_scan();
	delay_ms(50);		//��ʱһ��ʱ�ӽ���
   switch(tou_dev.touchflag){
			case 0x81: sys_slex1=0;tou_dev_clear();num_background_flag=0;break;//����
			case 0x82: sys_slex1=3;break;       //����
			default:break;
		}  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                               ѡ�����ϵ�к���
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 choose_play(void){
 	pic_btn_obj * btn[4];
	u8 i;
	u8 j;
	u8 res;
	static u8 choose_init_flag=0;
	background_draw_flag=0;
	clockpoint_flag=1;
	if(!choose_init_flag)
	{
		circle_fill(120,120,120,0);
    for(i=0;i<2;i++){
			for(j=0;j<2;j++)
			{
        btn[i*2+j]=pic_btn_creat(76+88*j,76+88*i,40,"1",choose_path[i*2+j]);			//������ť
			}
    }
		for(i=0;i<4;i++)
		{
			pic_btn_draw(btn[i],1);
		}
		choose_init_flag=1;
	}
  touch_scan();
	delay_ms(50);		//��ʱһ��ʱ�ӽ���
	for(i=0;i<4;i++)
	{
    res=pic_btn_check(btn[i],tou_dev.touchflag);//ȷ�ϰ�ť���
		if(res==0x20)    //����Ч����(������㰴)
		{  
			sys_next_state();
      sys_slex2=i;
			choose_init_flag=0;
			for(i=0;i<4;i++)pic_btn_delete(btn[i]);
 		}
	}
	if(tou_dev.touchflag==0x82)
	{
		sys_return();
		choose_init_flag=0;
		for(i=0;i<4;i++)pic_btn_delete(btn[i]);
	}
	return res;
}

