#include "sys.h"
#include "newtouch.h"
#ifndef _SYSPLESYS_H_
#define _SYSPLESYS_H_
#define HIGH     50
#define WDITH    100
extern u8 sys_slex1;
extern u8 sys_slex2;


u8 sys_return(void);
u8 sys_next_state(void);
u8 sys_return0(void);

#endif
