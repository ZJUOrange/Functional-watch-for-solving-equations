//#include "includes.h"
#include "touch.h"
#include "picturedraw.h"
#include "button.h"
//#include "spb.h"
# ifndef _NEW_TOUCH_H_
#define _NEW_TOUCH_H_
# define TOUCH_LONG_MIN 30
# define TOUCH_MOVE_MIN 10
#define  ORANGE 0XFD20//橘色
#define SPB_MOVE_WIN 		2 		//SPB滑动检测窗口值
#define SPB_MOVE_MIN		8		//SPB认为是滑动的最小值,超过会认为是滑动,但是不一定会翻页	   
#define SPB_MOVE_ACT 		50 		//SPB滑动的启动值,超过这个值将会进行翻页
#define SPB_ICOS_NUM 		11 		//SPB主界面图标个数(不含3个主图标)
////////////////////////////////////////////////////////////////////////////////////////////

#define SYSSET_BTN1_WIDTH			50			
#define SYSSET_BTN1_HEIGHT			40		
#define SYSSET_BTN2_WIDTH			60			
#define SYSSET_BTN2_HEIGHT			30		
#define SYSSET_BTN3_WIDTH			100			
#define SYSSET_EDIT_WIDTH			50			
#define SYSSET_EDIT_HEIGHT			25			
 
#define SYSSET_INWIN_FONT_COLOR		0X736C		
#define SYSSET_EDIT_BACK_COLOR		0XE79E		
 
/////////////////////////////////////////////////////////////////////////////////////////////
//================================================================
//						涓嬮潰瀹氫箟touch缁撴瀯浣撲互鍙婄浉鍏冲嚱鏁?
//================================================================
typedef struct 
{										    
	u16 oldxpos;     		
	u16 oldypos;     			
 	u16 curxpos;     			
	u16 curypos;     			
	u8 touchflag;         
	
	u16	xsta;			
	u16	ysta;				
	u32 stacnt;       
	
	u16 pos;             
	u8 frame;				 	
}_scan_touch_;
u8 touch_scan(void);
extern _scan_touch_  tou_dev;
u8 tou_dev_clear(void);

void simple_btn_draw(_btn_obj * btnx,u8 flag);
u8 In_btn_check(_btn_obj* ptr);
//长按返回0x90,短按返回0x20，其余情况目前都返回0
u8 simple_btn_check(_btn_obj * btnx,u8 MODE);
//===============================================================
//                      简单图形按钮
//===============================================================
typedef struct 
{
	u16 x; 				  		//中心点坐标
	u16 y;                      
	u16 r2; 				  		//图片半径的平方
	u16 r;
	u8 *caption;					//按钮名字
	u8 *picbtnpath;				//图片路径
	u8 Mode;              //按钮上一次状态
}pic_btn_obj;
pic_btn_obj * pic_btn_creat(u16 x,u16 y,u16 r,u8 *caption,u8 *picbtnpath);
u8 pic_btn_check(pic_btn_obj * btnx,u8 MODE);
u8 In_pic_btn_check(pic_btn_obj* ptr);
void pic_btn_draw(pic_btn_obj * btnx,u8 Mode);
void pic_btn_delete(pic_btn_obj * btn_del);
#endif
