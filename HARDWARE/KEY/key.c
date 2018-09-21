#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "stdio.h"
 
 
 //1:  K3 -- PC15
 //2:  K2 -- PC14
 //3:  K1 -- PC13
 //4:  K4 -- PA2
								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

}

u8 g_boma_Key_CANID;

void KEY_GET_CAN_ID(u8 *UT_CAN_ID)
{
	if(boma_KEY1)
	{
		g_boma_Key_CANID |= 1<<3;
	}
	else
	{
		g_boma_Key_CANID &= ~(1<<3);
	}
	
	if(boma_KEY2)
	{
		g_boma_Key_CANID |= 1<<2;
	}
	else
	{
		g_boma_Key_CANID &= ~(1<<2);
	}
	
	if(boma_KEY3)
	{
		g_boma_Key_CANID |= 1<<1;
	}
	else
	{
		g_boma_Key_CANID &= ~(1<<1);
	}
	
	if(boma_KEY4)
	{
		g_boma_Key_CANID |= 1<<0;
	}
	else
	{
		g_boma_Key_CANID &= ~(1<<0);
	}	
	
	if(g_boma_Key_CANID < 16)
	{
		*UT_CAN_ID = g_boma_Key_CANID;
	}
	
	
}



