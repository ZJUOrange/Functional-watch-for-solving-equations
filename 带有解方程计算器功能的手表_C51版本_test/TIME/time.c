#include "time.h" 	

u16 week;
u16 timer[6];
//自己的初始化函数
u8 timeinit(void)
{
	u8 temp=0;
	printf("OK\n"); 
		if(temp>=250)return 1;//初始化时钟失败,晶振有问题	   
		RCC->BDCR|=1<<8; //LSI作为RTC时钟 	    
		RCC->BDCR|=1<<15;//RTC时钟使能	  
	  	while(!(RTC->CRL&(1<<5)));//等待RTC寄存器操作完成	 
    	while(!(RTC->CRL&(1<<3)));//等待RTC寄存器同步  
    	RTC->CRH|=0X01;  		  //允许秒中断
    	RTC->CRH|=0X02;  		  //允许闹钟中断
    	while(!(RTC->CRL&(1<<5)));//等待RTC寄存器操作完成	 
		RTC->CRL|=1<<4;           //允许配置	 
		RTC->PRLH=0X0000;
		RTC->PRLL=32767;          //时钟周期设置(有待观察,看是否跑慢了?)理论值：32767	
		timeset(2021,12,28,10,36,25); //设置时间	  
		RTC->CRL&=~(1<<4);           //配置更新
		while(!(RTC->CRL&(1<<5)));   //等待RTC寄存器操作完成		 									    
	 	printf("FIRST TIME\n");				  
	MY_NVIC_Init(0,0,RTC_IRQn,2);//优先级设置    
	timeget();//更新时间 
	printf("Alarm Time1:%d-%d-%d %d:%d:%d\n",timer[5],timer[4],timer[3],timer[2],timer[1],timer[0]);//输出闹铃时间
	return 0; //ok
}

//RTC时钟中断 更改版
//每秒触发一次  	 
void RTC_IRQHandler(void)
{		 
	if(RTC->CRL&0x0001)			//秒钟中断
	{							
		timeget();
 	}
	if(RTC->CRL&0x0002)			//闹钟中断
	{
		RTC->CRL&=~(0x0002);	//清闹钟中断	  
		timeget();
		//printf("Alarm Time:%d-%d-%d %d:%d:%d\n",timer[5],timer[4],timer[3],timer[2],timer[1],timer[0]);//输出闹铃时间
		
  	} 				  								 
    RTC->CRL&=0X0FFA;         	//清除溢出，秒钟中断标志
	while(!(RTC->CRL&(1<<5)));	//等待RTC寄存器操作完成	  	    						 	   	 
}

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//year:年份
//返回值:该年份是不是闰年.1,是.0,不是
u8 leapyear(u16 year)
{
    int yearflag = 0;
    if (year % 4 == 0)
    {
        if (year % 100 != 0)
        {
            yearflag = 1;
        }
        else if (year % 400 == 0)
        {
            yearflag = 1;
        }
    }
    else
    {
        yearflag = 0;
    }
    return yearflag;
}

//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
//syear,smon,sday,hour,min,sec：年月日时分秒
//返回值：设置结果。0，成功；1，失败。
u8 timeset(u16 syear,u16 smon,u16 sday,u16 hour,u16 min,u16 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//把所有年份的秒钟相加
	{
		if(leapyear(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
		if(leapyear(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(u32)hour*3600;//小时秒钟数
    seccount+=(u32)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去
	#if TEST == 0												    
	//设置时钟
    RCC->APB1ENR|=1<<28;//使能电源时钟
    RCC->APB1ENR|=1<<27;//使能备份时钟
	PWR->CR|=1<<8;    //取消备份区写保护
	//上面三步是必须的!
	RTC->CRL|=1<<4;   //允许配置 
	RTC->CNTL=seccount&0xffff;
	RTC->CNTH=seccount>>16;
	RTC->CRL&=~(1<<4);//配置更新
	while(!(RTC->CRL&(1<<5)));//等待RTC寄存器操作完成 
	#endif
	timeget();//设置完之后更新一下数据 	
	return 0;	    
}

//得到当前的时间，结果保存在timer[6]里面
//返回值:0,成功;其他:错误代码.
u8 timeget(void)
{
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
	#if TEST == 0
 	timecount=RTC->CNTH;//得到计数器中的值(秒钟数)
	timecount<<=16;
	timecount+=RTC->CNTL;	
	#endif 
	#if TEST == 1
	timecount = 31536000*6+86400*80+20000;
	#endif
 	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp)//超过一天了
	{	  
		daycnt=temp;
		temp1=1970;	//从1970年开始
		while(temp>=365)
		{				 
			if(leapyear(temp1))//是闰年
			{
				if(temp>=366)temp-=366;//闰年的秒钟数
				else break;  
			}
			else temp-=365;	  //平年 
			temp1++;  
		}   
		timer[5]=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(leapyear(timer[5])&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;  
		}
		timer[4]=temp1+1;	//得到月份
		timer[3]=temp+1;  	//得到日期 
	}
	temp=timecount%86400;     		//得到秒钟数   	   
	timer[2]=temp/3600;     	//小时
	timer[1]=(temp%3600)/60; 	//分钟	
	timer[0]=(temp%3600)%60; 	//秒钟
	week=weekget(timer[5],timer[4],timer[3]);//获取星期   
	return 0;
}	

//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//year,month,day：公历年月日 
//返回值：星期号	
u8 weekget(u16 year,u16 month,u16 day)
{
	u16 c,y,m,d;
	int W;
	if(month == 1 || month == 2)
	{
		month = month + 12;
		year--;
	}
	c=year/100;
  y=year%100;
	m=month;
  d=day;
  W=c/4-2*c+y+y/4+26*(m+1)/10+d-1;
	week = W%7;
  if( W < 0)
	{
		week = (W+(-W/7+1)*7)%7;
	}
  return week;
}


//显示当前日期
u8 showweek(u16 x,u16 y,u16 week)
{
	u8 t;
	#if TEST==0
	LCDShowString(20+x,250-y,200,16,16,"    -  -     ");	   
	LCDShowString(20+x,286-y,200,16,16,"  :  :  ");	
	#endif
	if(t!=timer[0])
		{
			t=timer[0];
			#if TEST==0
			LCDShowNum(20+x,250-y,timer[5],4,16);				
			//LCDShowString(20+x,250,200,16,16,timer[5]);	
			LCDShowNum(60+x,250-y,timer[4],2,16);									  
			LCDShowNum(84+x,250-y,timer[3],2,16);	
			#endif
			switch(week)
			{
				case 0:
					#if TEST==0
					LCDShowString(20+x,268-y,200,16,16,"Sunday   ");
					#endif				
					break;
				case 1:
					#if TEST==0
					LCDShowString(20+x,268-y,200,16,16,"Monday   ");
				  #endif
					break;
				case 2:
					#if TEST==0
					LCDShowString(20+x,268-y,200,16,16,"Tuesday  ");
				  #endif
					break;
				case 3:
					#if TEST==0
					LCDShowString(20+x,268-y,200,16,16,"Wednesday");
				  #endif
					break;
				case 4:
					#if TEST==0
					LCDShowString(20+x,268-y,200,16,16,"Thursday ");
			  	#endif
					break;
				case 5:
					#if TEST==0
					LCDShowString(20+x,268-y,200,16,16,"Friday   ");
				  #endif
					break;
				case 6:
					#if TEST==0
					LCDShowString(20+x,148-y,200,16,16,"Saturday ");
				  #endif
					break;  
			}
			#if TEST==0
			LCDShowNum(20+x,286-y,timer[2],2,16);									  
			LCDShowNum(44+x,286-y,timer[1],2,16);									  
			LCDShowNum(68+x,286-y,timer[0],2,16);
			#endif
		}	
}
