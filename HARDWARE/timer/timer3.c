#include "timer3.h"

void timer3_init(u16 ARR,u16 PSC)//
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStrcture;
	NVIC_InitTypeDef NVIC_InitStrcture;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitStrcture.TIM_Period=ARR;
	TIM_TimeBaseInitStrcture.TIM_Prescaler=PSC;//7200 72000000/7200=10k 0.1ms
	TIM_TimeBaseInitStrcture.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStrcture.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStrcture.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrcture);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	

	
	NVIC_InitStrcture.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStrcture.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStrcture.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStrcture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStrcture);
	
	TIM_Cmd(TIM3,ENABLE);
	
	
}

void TIM3_IRQHandler(void)
{
	
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)==SET)
	{
		TIM_ClearFlag(TIM3,TIM_FLAG_Update);
		
		if(GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_2)==1)
		{
//			LED1_on;


		}
		else if(GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_2)==0)
		{

//			LED1_off;

		}
	}
	

	
}





