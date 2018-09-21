#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
#include "can.h"


	//#define KEY0 PEin(4)   	//PE4
	//#define KEY1 PEin(3)	//PE3 
	//#define KEY2 PEin(2)	//PE2
	//#define WK_UP PAin(0)	//PA0  WK_UP

	//#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0
	//#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键1
	//#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取按键2 
	//#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键3(WK_UP) 

//1:  K3 -- PC15
//2:  K2 -- PC14
//3:  K1 -- PC13
//4:  K4 -- PA2 

#define boma_KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)
#define boma_KEY2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)
#define boma_KEY3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
#define boma_KEY4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)


void KEY_Init(void);//IO初始化
void KEY_GET_CAN_ID(u8 *UT_CAN_ID);		    
#endif
