#ifndef _CALC_H_
#define  _CALC_H_
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "malloc.h"
#include "ourtest.h"
#define CH2DEC(x) ((x) - '0')
#define ABS(x) (x>0) ? x:-x
extern char calc_buffer[100];
extern double ANSWER;
extern u8 Newtown_outflag;
extern u8 calc_error_flag;
//该函数用于寻找符号
int findop(const char *str);
//符号排序
extern int oprank(char ch);
//字符串计算函数
double eval(char *str,double x);
//eval函数会修改str的值，该函数对str值进行了保护
double eval_safe(char *str,double x);
//该函数用于将字符串转换成double型数值
double str2db(const char *str);
//计算函数，针对a op b形式小单元求解
double calc(double a, char op, double b,double x);
//采用数值计算的方式求表达式在x点处的导数
double df_eval(char *str,double x);
//牛顿求根函数
double Newtown(char *str,double x,double x_last);
//因为内存受限,因此采用了多次调用函数的方式
double Newtown_5(char *str,double x,double x_last);
u16 mystrlen(char *str);
u16 myprintf(int a);
#endif
