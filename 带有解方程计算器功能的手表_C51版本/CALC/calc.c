#include <calc.h>
char calc_buffer[100];
u8 calc_error_flag=0;
double ANSWER=0;
double eval(char *str,double x)
{   
     
    double ret = 0;
		double left=0;
		double right=0;
    //  find the most right and lowest operator
    int loc = 0;
		loc =	findop(str);
    if(loc == -2){
    	int  strlong=0;
        strlong=strlen(str);
      
		str[strlong-1]=0;
		return eval(str+1,x);
	}
    else if (loc == -1)  //  no operator found
    { 
        return str2db(str);
    }
    else    //  split the str into two parts
    { 
        char op = str[loc];
        str[loc] = 0;
        left = eval(str,x);
        right = eval(str + loc + 1,x);
        return  calc(left, op, right,x);
    }
   
}
char calc_backup[100];
double eval_safe(char *str,double x){
	strcpy(calc_backup,str);
	x=eval(calc_backup,x);
	return x;
}
u16 myprintf(int a)
{
	printf("%d",a);
	return a;
}
u16 mystrlen(char *str)
{
	return strlen(str);
}

//  internal functions
static int findop(const char *str)
{
    int ret = -1;   //  the location
    int lowestoprank = -1;  //  lowest rank found
    int i;
	  int rank=-1;
		int is=0;//在括号内is>0，在括号外is=1
		int was=0;
    for (i = strlen(str) - 1; i >= 0; i--)
    {   
			if(str[i]==')'){
    	  is++;
    	  was=1;
											}
			if(str[i]=='('){
				is--;
			}
		if(!is){ 
		if(str[i]=='-'){
			if(str[i-1]!=')'&&str[i-1]<'0'||str[i-1]>'9'){
				continue;
			}
		}
		rank = oprank(str[i]);
        if (rank > lowestoprank)
        {
        	
            ret = i;
            lowestoprank = rank;
         }
		}
       
    }
    if(ret==-1&&was==1){
    	ret=-2;
	}
    return ret;
}

int oprank(char ch)
{
    int ret = -1;
    if (ch == '+' || ch == '-')
    {
        ret = 4;
    }
    else if (ch == '*' || ch == '/'||ch =='%')
    {
        ret = 3;
    }
    else if (ch == 'c' || ch == 's'||ch =='t'||ch=='p'||ch=='l')
    {
        ret = 2;
    }
    else if (ch == 'x' )
    {
        ret = 1;
    }
   return ret;
}

double str2db(const char *str)
{   
    int is=0;
    double ret_i= 0;
    double ret=0;
    int count=0;//用于计数
    int i;
    for (i = 0; i < strlen(str); i++)
    {   
		count++;
        if(str[i]=='-'){
        	is=1;
        	continue;
		}
		if(str[i]=='.'){
			break;
		}
        ret_i = ret_i * 10 + CH2DEC(str[i]);
        
    }
    for(i=strlen(str)-1;i>=count; i--){
    	ret=(ret+CH2DEC(str[i]))/10;
	}
	ret = ret +ret_i;
    if(is) ret=-ret;
    return ret;
}

double calc(double a, char op, double b,double x)
{   
    
    double ret = 0;
    switch (op)
    {
    case '+':
        ret = a + b;break;
    case '-':
        ret = a - b;break;
    case '*':
        ret = a * b;break;
    case '/':
				if(b==0)  calc_error_flag=1;
        ret = a / b;break;
    case '%':
			if(b==0)  calc_error_flag=1;
    	ret =(int) a %(int) b;break;
    case 'p':
        ret =pow(a,b);break;
    case 'l':
			  if(a <=0 || b <= 0) calc_error_flag=1;
        ret =log(b)/log(a);break;
    case 'c':
        ret =cos(b);break;
    case 's':
        ret =sin(b);break;
    case 't':
        ret =tan(b);break;
    case 'x':
        ret =x;break;
    }
    return ret;
}
//用数值的方式求微分
double df_eval(char *str,double x){
	double eval1;
	double eval2;
	eval1=eval_safe(str,x+1e-10);
	eval2=eval_safe(str,x);
	return (eval1-eval2)/(1e-10);
}
u8 Newtown_outflag=0;
double Newtown(char *str,double x,double x_last){
	double value;
  double df_value;
	value=eval_safe(str,x);
	if(x-x_last<1E-6f&&x_last-x<1E-6f&&value<1E-12&&-value<1E-12){
		Newtown_outflag=1;
		return x;
	}
		df_value=df_eval(str,x);
		x=x-value/df_value;
		return x;
}
double Newtown_5(char *str,double x,double x_last){
	u8 cnt=0;
	double temp;
	Newtown_outflag=0;
	calc_error_flag=0;
	temp=x;
	x=Newtown(str,x,x_last);
	x_last=temp;
	for(cnt=0;cnt<100;cnt++){
		if(!Newtown_outflag) {
		temp=x;
		x=Newtown(str,x,x_last);
		x_last=temp;
		}
	}
	if(!Newtown_outflag) calc_error_flag=1;
	return x;
}

