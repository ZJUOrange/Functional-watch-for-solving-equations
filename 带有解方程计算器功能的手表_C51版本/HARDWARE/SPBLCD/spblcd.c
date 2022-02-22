#include "spblcd.h"
#include "dma.h"
#include "lcd.h"
#include "delay.h"
#include "malloc.h"
#include "usart.h"
#include "w25qxx.h"
#include "spi.h"
#include "ucos_ii.h" 
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

//从lcd拷贝到spi flash
//frame:帧序号,0或1,必须先写第0帧(以执行擦除操作),再更新第1帧
//返回值:0,成功;
//    其他,错误代码
u8 slcd_frame_lcd2spi(u8 frame)
{
	u8 temp;
	u16 *pbuf;  
	u8* p;
	u32 startx;		//起始地址  
	u16 i,j,k;
	u32 woffset=0;	//写地址偏移
	startx=SPILCD_BASE+1+frame*spbdev.spbwidth*spbdev.spbheight*2;	//得到起始地址(字节)
	pbuf=mymalloc(SRAMIN,4096);						//申请内存
	if(!pbuf)return 1;
	woffset=0; 
	k=0;
	for(i=0;i<spbdev.spbwidth;i++)
	{
		for(j=0;j<spbdev.spbheight;j++)
		{
			pbuf[k++]=LCD_ReadPoint(i,j);			//读取点 
			if(k==2048)
			{
				p=(u8*)pbuf;
				for(k=0;k<2048;k++)					//调转高低字节
				{
					temp=p[2*k];
					p[2*k]=p[2*k+1];
					p[2*k+1]=temp;
				}
				W25QXX_Write((u8*)pbuf,startx+woffset,4096);//写入一个扇区
				woffset+=4096;
				k=0;
			}
		}
	}
	if(k)//还有一点数据要写入
	{
 		p=(u8*)pbuf;
		for(i=0;i<k;i++)							//调转高低字节
		{
			temp=p[2*i];
			p[2*i]=p[2*i+1];
			p[2*i+1]=temp;
		}
		W25QXX_Write((u8*)pbuf,startx+woffset,k*2);	//写入最后一次数据
	}
	myfree(SRAMIN,pbuf);							//释放内存	
	return 0;
} 
//SPI2模式设置
//mode:0，8位普通模式；1,16位DMA模式  
void slcd_spi2_mode(u8 mode)
{
	u16 tempreg=0;
	if(mode==0)	//8位普通模式,重新配置SPI
	{
		RCC->APB1RSTR|=1<<14;	//复位SPI2
		RCC->APB1RSTR&=~(1<<14);//停止复位SPI2
		tempreg|=0<<10;			//全双工模式	
		tempreg|=1<<9;			//软件nss管理
		tempreg|=1<<8;			 
		tempreg|=1<<2;			//SPI主机  
		tempreg|=0<<11;			//8位数据格式	
		tempreg|=1<<1;			//空闲模式下SCK为1 CPOL=1 
		tempreg|=1<<0;			//数据采样从第2个时间边沿开始,CPHA=1   
		tempreg|=0<<3;			//2分频,设置为42Mhz频率
		tempreg|=0<<7;			//MSB First  
		tempreg|=1<<6;			//SPI启动 
		SPI2->CR1=tempreg; 		//设置CR1  
		SPI2->I2SCFGR&=~(1<<11);//选择SPI模式 
	}else						//16位DMA模式
	{
		SPI2->CR1|=1<<10; 		//仅接收模式   
		SPI2->CR1|=1<<11;		//16bit数据格式	
		SPI2->CR2|=1<<0;		//使用DMA接收 
	}   
} 
//SPI2 --> LCD_RAM dma配置
//16位,SPI2_RX传输到LCD_RAM.
void slcd_dma_init(void)
{  
	RCC->AHBENR|=1<<0;				//开启DMA1时钟
	delay_ms(5);					//等待DMA时钟稳定
	DMA1_Channel4->CPAR=(u32)&SPI2->DR;			//外设地址为:SPI1->DR
	DMA1_Channel4->CMAR=(u32)(u32)&LCD->LCD_RAM;//目标地址为LCD_RAM
 	DMA1_Channel4->CNDTR=0;			//DMA1,传输数据量,暂时设置为0
	DMA1_Channel4->CCR=0X00000000;	//复位
	DMA1_Channel4->CCR|=0<<4;		//从外设读
	DMA1_Channel4->CCR|=0<<5;		//普通模式
	DMA1_Channel4->CCR|=0<<6;		//外设地址非增量模式
	DMA1_Channel4->CCR|=0<<7;		//存储器非增量模式
	DMA1_Channel4->CCR|=1<<8;		//外设数据宽度为16位
	DMA1_Channel4->CCR|=1<<10;		//存储器数据宽度16位
	DMA1_Channel4->CCR|=1<<12;		//中等优先级
	DMA1_Channel4->CCR|=0<<14;		//非存储器到存储器模式
} 
//开启一次SPI到LCD的DMA的传输 
void slcd_dma_enable(void)
{	  
	u32 lcdsize=spbdev.spbwidth*spbdev.spbheight;
	u32 dmatransfered=0;
	while(lcdsize)
	{     	  
		DMA1_Channel4->CCR&=~(1<<0);			//关闭DMA传输 
		DMA1->IFCR|=1<<13;						//清除上次的传输完成标记 
		if(lcdsize>SLCD_DMA_MAX_TRANS)
		{
			lcdsize-=SLCD_DMA_MAX_TRANS;
			DMA1_Channel4->CNDTR=SLCD_DMA_MAX_TRANS;	//设置传输长度
		}else
		{
			DMA1_Channel4->CNDTR=lcdsize;				//设置传输长度
			lcdsize=0;
		}	
 		dmatransfered+=SLCD_DMA_MAX_TRANS;		
		DMA1_Channel4->CCR|=1<<0;				//开启DMA RX传输 		
		while((DMA1->ISR&(1<<13))==0);			//等待传输完成 
	} 
	DMA1_Channel4->CCR&=~(1<<0);				//关闭DMA传输 
}
//显示一帧,即启动一次spi到lcd的显示.
//x:坐标偏移量
void slcd_frame_show(u16 x)
{
    OS_CPU_SR cpu_sr=0;
	u32 addr=SPILCD_BASE+1+(x*spbdev.spbheight)*2; 
	LCD_Scan_Dir(U2D_L2R);		//设置扫描方向
	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510||lcddev.id==0X6804)
	{
		LCD_Set_Window(spbdev.stabarheight,0,spbdev.spbheight,spbdev.spbwidth);
		LCD_SetCursor(spbdev.stabarheight,0);//设置光标位置 
	}else
	{
		LCD_Set_Window(0,spbdev.stabarheight,spbdev.spbwidth,spbdev.spbheight);
		if(lcddev.id!=0X1963)LCD_SetCursor(0,spbdev.stabarheight);//设置光标位置 		
	}
	LCD_WriteRAM_Prepare();     //开始写入GRAM	 
	W25QXX_CS=0;                       		//使能器件   
    SPI2_ReadWriteByte(W25X_FastReadData);	//发送快速读取命令   
    SPI2_ReadWriteByte((u8)((addr)>>16));	//发送24bit地址    
    SPI2_ReadWriteByte((u8)((addr)>>8));   
    SPI2_ReadWriteByte((u8)addr);  
    SPI2_ReadWriteByte(0XFF);	//8 dummy clock  
	slcd_spi2_mode(1); 			//设置为16位宽,DMA接收
	OS_ENTER_CRITICAL();		//进入临界区(无法被中断打断)    	
	slcd_dma_enable();			//启动一次SPI到LCD的dma传输 
	OS_EXIT_CRITICAL();			//退出临界区(可以被中断打断)
	slcd_spi2_mode(0);			//恢复8位模式
	//加入读数据,判断是不是白屏.
	W25QXX_CS=1;  	 	 		//禁止W25QXX
	LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认方向	
	LCD_Set_Window(0,0,lcddev.width,lcddev.height);//恢复默认窗口大小	 
}








 








