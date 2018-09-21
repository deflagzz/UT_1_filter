#ifndef _EXTI_H
#define _EXTI_H

#include "include.h"

#define g_len_voicebuff 5






extern u8 g_flag_exti_get[9];
extern u16 g_exti_overTIME[2];
extern u8 g_test[9];
extern u16 g_exti_testTIME[9][30];




void extix_init(void);
void MY_EXTI_Line_Key(u8 my_EXTI_Line,u8 state);



#endif
