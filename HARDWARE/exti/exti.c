#include "exti.h"

//PB6   TIM4--CH1
//PB7  关
//PB8  增
//PB9  减

u8 g_flag_40K;

u16 cnt_ccr1=0;
u8 cnt_time=0;

void extix_init(void)  //外部中断:PA9 PA10
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStrcture;
	NVIC_InitTypeDef NVIC_InitStrcture;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStructure);		
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	
	
	
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);//////PB6 

	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource9);//////PA9
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource9);//////PA10
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource10);//////PA10

	//EXTI_InitStrcture.EXTI_Line=EXTI_Line6;
	//EXTI_InitStrcture.EXTI_Mode=EXTI_Mode_Interrupt;
	//EXTI_InitStrcture.EXTI_Trigger=EXTI_Trigger_Rising; //上升沿触发
	//EXTI_InitStrcture.EXTI_LineCmd=ENABLE;
	//EXTI_Init(&EXTI_InitStrcture);	

	//EXTI_InitStrcture.EXTI_Line=EXTI_Line9;
	//EXTI_InitStrcture.EXTI_Mode=EXTI_Mode_Interrupt;
	//EXTI_InitStrcture.EXTI_Trigger=EXTI_Trigger_Falling; //下降沿触发
	//EXTI_InitStrcture.EXTI_LineCmd=ENABLE;
	//EXTI_Init(&EXTI_InitStrcture);		

	EXTI_InitStrcture.EXTI_Line=EXTI_Line9;
	EXTI_InitStrcture.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStrcture.EXTI_Trigger=EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStrcture.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStrcture);	
	
	EXTI_InitStrcture.EXTI_Line=EXTI_Line10;
	EXTI_InitStrcture.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStrcture.EXTI_Trigger=EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStrcture.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStrcture);			
	
	NVIC_InitStrcture.NVIC_IRQChannel=EXTI9_5_IRQn; //PA9
	NVIC_InitStrcture.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStrcture.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStrcture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStrcture);
	
	NVIC_InitStrcture.NVIC_IRQChannel=EXTI15_10_IRQn; //PA10
	NVIC_InitStrcture.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStrcture.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStrcture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStrcture);
	
//	NVIC_InitStrcture.NVIC_IRQChannel=EXTI15_10_IRQn;
//	NVIC_InitStrcture.NVIC_IRQChannelPreemptionPriority=2;
//	NVIC_InitStrcture.NVIC_IRQChannelSubPriority=2;
//	NVIC_InitStrcture.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStrcture);
	
	
	MY_EXTI_Line_Key(9,0);
	MY_EXTI_Line_Key(10,0);
}

u16 g_exti_overTIME[2];
u8 g_flag_exti_get[9];

u16 g_exti_testTIME[9][30];
u8 g_test[9];

u8 g_flag_extiGet;
void EXTI9_5_IRQHandler(void)    ////  上升沿+下降沿,上升沿大于1000us,距离过近
{
	if(EXTI_GetFlagStatus(EXTI_Line9)==SET)             //PA9 																									//PB9  减
	{		

			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)==0)
			{		


				g_exti_overTIME[0]  = TIM_GetCounter(TIM2) + g_yuzhen_30cm;
				if(g_test[g_Son_ID] < 30)
				{
					g_exti_testTIME[g_Son_ID][g_test[g_Son_ID]] = g_exti_overTIME[0];
					g_test[g_Son_ID]++;	
					
				}

			
				g_flag_exti_get[g_Son_ID] = 1; 
//				MY_EXTI_Line_Key(9,0);//屏蔽外部中断0 EXTI->IMR|=1<<11;
					
			}		
		

		EXTI_ClearFlag(EXTI_Line9);
	}	
}


void EXTI15_10_IRQHandler(void)    ////  上升沿+下降沿,上升沿大于1000us,距离过近
{
	
	if(EXTI_GetFlagStatus(EXTI_Line10)==SET)             //PA10 																									//PB9  减
	{		

			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)==0)
			{		


				g_exti_overTIME[1] = TIM_GetCounter(TIM2) + g_yuzhen_30cm;
				
				if(g_test[g_Son_ID+4] < 30)
				{
					g_exti_testTIME[g_Son_ID+4][g_test[g_Son_ID+4]] = g_exti_overTIME[1];
					g_test[g_Son_ID+4]++;					
				}
			
					g_flag_exti_get[g_Son_ID+4] = 1; 
//					MY_EXTI_Line_Key(10,0);//屏蔽外部中断0 EXTI->IMR|=1<<11;
				
				
					
			}			
		

		EXTI_ClearFlag(EXTI_Line10);
	}	
}


void MY_EXTI_Line_Key(u8 my_EXTI_Line,u8 state)
{
	if(my_EXTI_Line < 16)
	{
		if(state) 		//打开
		{
			EXTI->IMR |= 1<<my_EXTI_Line; //打开外部中断线
		}
		else if(!state)	//屏蔽
		{
			EXTI->IMR &= ~(1<<my_EXTI_Line);//屏蔽外部中断0 EXTI->IMR|=1<<11;
		}	
	}	
}









