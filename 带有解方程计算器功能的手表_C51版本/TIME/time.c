#include "time.h" 	

u16 week;
u16 timer[6];
//�Լ��ĳ�ʼ������
u8 timeinit(void)
{
	u8 temp=0;
	printf("OK\n"); 
		if(temp>=250)return 1;//��ʼ��ʱ��ʧ��,����������	   
		RCC->BDCR|=1<<8; //LSI��ΪRTCʱ�� 	    
		RCC->BDCR|=1<<15;//RTCʱ��ʹ��	  
	  	while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������	 
    	while(!(RTC->CRL&(1<<3)));//�ȴ�RTC�Ĵ���ͬ��  
    	RTC->CRH|=0X01;  		  //�������ж�
    	RTC->CRH|=0X02;  		  //���������ж�
    	while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������	 
		RTC->CRL|=1<<4;           //��������	 
		RTC->PRLH=0X0000;
		RTC->PRLL=32767;          //ʱ����������(�д��۲�,���Ƿ�������?)����ֵ��32767	
		timeset(2021,12,28,10,36,25); //����ʱ��	  
		RTC->CRL&=~(1<<4);           //���ø���
		while(!(RTC->CRL&(1<<5)));   //�ȴ�RTC�Ĵ����������		 									    
	 	printf("FIRST TIME\n");				  
	MY_NVIC_Init(0,0,RTC_IRQn,2);//���ȼ�����    
	timeget();//����ʱ�� 
	printf("Alarm Time1:%d-%d-%d %d:%d:%d\n",timer[5],timer[4],timer[3],timer[2],timer[1],timer[0]);//�������ʱ��
	return 0; //ok
}

//RTCʱ���ж� ���İ�
//ÿ�봥��һ��  	 
void RTC_IRQHandler(void)
{		 
	if(RTC->CRL&0x0001)			//�����ж�
	{							
		timeget();
 	}
	if(RTC->CRL&0x0002)			//�����ж�
	{
		RTC->CRL&=~(0x0002);	//�������ж�	  
		timeget();
		//printf("Alarm Time:%d-%d-%d %d:%d:%d\n",timer[5],timer[4],timer[3],timer[2],timer[1],timer[0]);//�������ʱ��
		
  	} 				  								 
    RTC->CRL&=0X0FFA;         	//�������������жϱ�־
	while(!(RTC->CRL&(1<<5)));	//�ȴ�RTC�Ĵ����������	  	    						 	   	 
}

//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//year:���
//����ֵ:������ǲ�������.1,��.0,����
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

//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
//syear,smon,sday,hour,min,sec��������ʱ����
//����ֵ�����ý����0���ɹ���1��ʧ�ܡ�
u8 timeset(u16 syear,u16 smon,u16 sday,u16 hour,u16 min,u16 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//��������ݵ��������
	{
		if(leapyear(t))seccount+=31622400;//�����������
		else seccount+=31536000;			  //ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;//�·����������
		if(leapyear(syear)&&t==1)seccount+=86400;//����2�·�����һ���������	   
	}
	seccount+=(u32)(sday-1)*86400;//��ǰ�����ڵ���������� 
	seccount+=(u32)hour*3600;//Сʱ������
    seccount+=(u32)min*60;	 //����������
	seccount+=sec;//�������Ӽ���ȥ
	#if TEST == 0												    
	//����ʱ��
    RCC->APB1ENR|=1<<28;//ʹ�ܵ�Դʱ��
    RCC->APB1ENR|=1<<27;//ʹ�ܱ���ʱ��
	PWR->CR|=1<<8;    //ȡ��������д����
	//���������Ǳ����!
	RTC->CRL|=1<<4;   //�������� 
	RTC->CNTL=seccount&0xffff;
	RTC->CNTH=seccount>>16;
	RTC->CRL&=~(1<<4);//���ø���
	while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ���������� 
	#endif
	timeget();//������֮�����һ������ 	
	return 0;	    
}

//�õ���ǰ��ʱ�䣬���������timer[6]����
//����ֵ:0,�ɹ�;����:�������.
u8 timeget(void)
{
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
	#if TEST == 0
 	timecount=RTC->CNTH;//�õ��������е�ֵ(������)
	timecount<<=16;
	timecount+=RTC->CNTL;	
	#endif 
	#if TEST == 1
	timecount = 31536000*6+86400*80+20000;
	#endif
 	temp=timecount/86400;   //�õ�����(��������Ӧ��)
	if(daycnt!=temp)//����һ����
	{	  
		daycnt=temp;
		temp1=1970;	//��1970�꿪ʼ
		while(temp>=365)
		{				 
			if(leapyear(temp1))//������
			{
				if(temp>=366)temp-=366;//�����������
				else break;  
			}
			else temp-=365;	  //ƽ�� 
			temp1++;  
		}   
		timer[5]=temp1;//�õ����
		temp1=0;
		while(temp>=28)//������һ����
		{
			if(leapyear(timer[5])&&temp1==1)//�����ǲ�������/2�·�
			{
				if(temp>=29)temp-=29;//�����������
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
				else break;
			}
			temp1++;  
		}
		timer[4]=temp1+1;	//�õ��·�
		timer[3]=temp+1;  	//�õ����� 
	}
	temp=timecount%86400;     		//�õ�������   	   
	timer[2]=temp/3600;     	//Сʱ
	timer[1]=(temp%3600)/60; 	//����	
	timer[0]=(temp%3600)%60; 	//����
	week=weekget(timer[5],timer[4],timer[3]);//��ȡ����   
	return 0;
}	

//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//year,month,day������������ 
//����ֵ�����ں�	
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


//��ʾ��ǰ����
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
