#ifndef _FILTER_H
#define _FILTER_H
#include "include.h"
										 //5mm/ms
#define FILTER_A       400    //ÏÞ·ùÖµ   //   38CM/S=0.38MM/MS--7.6
#define FILTER_times   8
#define g_UT_check_huibo 25
#define g_UT_Max_huibo 30
#define g_UT_Min_huibo 20


u16 filter_super(u16 *num, u8 n);
u16 UT_Filter(u16 UT_NewVal,u8 ID); 
void New_Filter(u16 *UT_RX_buff,u16 *ID_buff) ;
void zijian_Filter(u16 *UT_RX_buff,u16 *ID_buff);

u8 exti_fillter(u16 *exti_timeBUFF);

#endif



