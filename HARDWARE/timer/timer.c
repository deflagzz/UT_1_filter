#include "timer.h"
//#include "exti.h"

//u8 tt=10;
//u8 rad;
//u8 tx_hh=0;
//u8 tx_h=0;
//u8 tx_l=0;

void time2_init(u16 arr,u16 psc)//
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrct;
	NVIC_InitTypeDef NVIC_InitStrct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	
	TIM_TimeBaseInitStrct.TIM_Period=arr;
	TIM_TimeBaseInitStrct.TIM_Prescaler=psc;
	TIM_TimeBaseInitStrct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStrct.TIM_CounterMode=TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStrct);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM2,ENABLE);//Ê§ÄÜ
	
	NVIC_InitStrct.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStrct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStrct.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStrct.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStrct);
	
	
	
	
	
}

u16 g_sum_timer2;
//u8 g_flag_out;


void TIM2_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM2,TIM_IT_Update)==SET)//10us
	{		
		g_sum_timer2++;
		if(g_sum_timer2 > 6000)
		{
			g_sum_timer2 = 6000;

		}


		TIM_ClearFlag(TIM2,TIM_IT_Update);
	}
}






