#include "ourtest.h"
void test_time()
{
	//weekget 
	weekget(2021,11,22);
	weekget(2021,11,23);
	weekget(2021,11,24);
	weekget(2021,11,25);
	weekget(2021,11,26);
	weekget(2021,11,27);
	weekget(2021,11,28);
	weekget(2021,1,20);
	weekget(2021,2,20);
	weekget(2000,3,1);
	
	//leapyear
	leapyear(2000);
	leapyear(2004);
	leapyear(1900);
	leapyear(1901);
	
	//timeinit,timeget
	timeset(3000,1,1,1,1,1);
	timeset(2020,11,28,12,3,4);
	timeset(2021,11,28,0,0,0);
	timeset(2020,2,18,1,1,1);
	
	//showweek
	timer[5]=2020;
	timer[4]=11;
	timer[3]=3;
	timer[2]=1;
	timer[1]=1;
	timer[0]=1;
	showweek(0,0,0);
	showweek(0,0,1);
	showweek(0,0,2);
	showweek(0,0,3);
	showweek(0,0,4);
	showweek(0,0,5);
	showweek(0,0,6);
}


u8 test_Newtown_5_flag=0;
u8 answer_cmp(double ans,double anstrue,u8 testID){
	double diff;
	diff=ans-anstrue;
	if(diff<1E-6&&-diff<1E-6) {
		#if TEST ==0
		printf("\r\n testID=%d,OK",(int)testID);
		#endif 
		return 0;
	}
	else{ 
		#if TEST ==0
		printf("\r\n testID=%d,error",(int)testID);
		#endif 
		return testID;
	}
}

void test_Newtown_5(void)
{
	u8 testID=1;
	//1
	strcpy(calc_buffer,"(x)p2-1");
	ANSWER=Newtown_5(calc_buffer,2,0);
	test_Newtown_5_flag=answer_cmp(ANSWER,1,testID);
	testID++;
	//2
	strcpy(calc_buffer,"(x)/3-1");
	ANSWER=Newtown_5(calc_buffer,3,0);
	test_Newtown_5_flag=answer_cmp(ANSWER,3,testID);
	testID++;
	
	strcpy(calc_buffer,"2.718p(x)-1");
	ANSWER=Newtown_5(calc_buffer,1,0);
	test_Newtown_5_flag=answer_cmp(ANSWER,0,testID);
	testID++;
	//4
	strcpy(calc_buffer,"c(x)+1");
	ANSWER=Newtown_5(calc_buffer,1,0);
	test_Newtown_5_flag=answer_cmp(ANSWER,3.1415,testID);
	testID++;
//	printf("ANSWER=%lf",ANSWER);
	
	strcpy(calc_buffer,"2*(x)+1");
	ANSWER=Newtown_5(calc_buffer,1,0);
	test_Newtown_5_flag=answer_cmp(ANSWER,-0.5,testID);
	testID++;
	//6
	strcpy(calc_buffer,"s(x)+1");
	ANSWER=Newtown_5(calc_buffer,1,0);
	test_Newtown_5_flag=answer_cmp(ANSWER,-1.570840,testID);
	testID++;
	//7
	strcpy(calc_buffer,"-1+t(x)");
	ANSWER=Newtown_5(calc_buffer,1,0);
	test_Newtown_5_flag=answer_cmp(ANSWER,0.7853981634,testID);
	testID++;
	//8
	strcpy(calc_buffer,"2.718l(x)-1");
	ANSWER=Newtown_5(calc_buffer,3,0);
	test_Newtown_5_flag=answer_cmp(ANSWER,0,testID);
	testID++;
	
	strcpy(calc_buffer,"3%(x)-1");
	ANSWER=Newtown_5(calc_buffer,1,0);
	//test_Newtown_5_flag=answer_cmp(ANSWER,2,testID);
}