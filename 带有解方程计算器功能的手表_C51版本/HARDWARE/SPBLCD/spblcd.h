#ifndef __SPBLCD_H
#define	__SPBLCD_H	   
#include "sys.h"  
#include "lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//SPB效果实现 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/3/6
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//*******************************************************************************
//V1.1 20150322
//新增gui_draw_bline函数
//修改针对4.3/7寸屏的主图标显示方式,直接全部采用DMA,以提高速度.
//V1.2 20150325
//修改LCD显存的存储介质为SPI FLASH
////////////////////////////////////////////////////////////////////////////////// 	

#define SLCD_DMA_MAX_TRANS	60*1024		//DMA一次最多传输60K字节	

#define SPILCD_BASE 	1024*1024*15.1 	//SPB界面,从SPI FLASH的第15.1M字节开始存储,占用最大为768KB+1字节
										//SPILCD_BASE，存放液晶分辨率标志：
										//0，表示240*320的屏；1，表示320*480的屏；2，表示480*800的屏；其他值，非法
										//对于320*240的屏,支持双屏滑动,占用   320*240*2*2=300K字节
										//对于480*320的屏,支持双屏滑动,占用   480*320*2*2=600K字节
										//对于800*480的屏,只支持单屏操作,占用 800*480*2=768K字节 

extern u16 *sramlcdbuf;					//SRAMLCD缓存,先在SRAM 里面将图片解码,并加入图标以及文字等信息
										//然后再保存到SPI FLASH里面

 
 
u8 slcd_frame_lcd2spi(u8 frame); 
void slcd_spi2_mode(u8 mode);
void slcd_dma_init(void);
void slcd_dma_enable(void);  
void slcd_frame_show(u16 x);


void slcd2lcd(u16 x);


#endif

























