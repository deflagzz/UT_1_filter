#include "ir.h"


u8 g_Send_CAN_or_USART = 0;	//0:CAN 1:串口发送

//15 -- 1900
//10 -- 1600
u16 g_voicedelay   = 25*20;	//us
u16 g_TXvoicedelay = 30;     //ms 3.4米
u16 g_yuzhen_30cm  = 1850;   //2058-35cm    <30异常的话修改这里


u16 g_dayu255 = 2550;
u8 g_xiaoyu30 = 0;
u8 g_UT_miss  = 11;

u8 filter_times = 1;  //滤波次数
//u8 average_times = 2;  //滤波次数
u8 g_time_ut = 1;

void cd4052_control(void)
{
	GPIO_InitTypeDef         GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外设时钟使能
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //关闭jtag
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; //PB0--TIM3--CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
}



//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_2PWM_Init(u16 arr,u16 psc)    //PWM发射初始化  PA6--TIM3--CH1 PA7--TIM3--CH2
{  
	GPIO_InitTypeDef         GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);// TIM3CH1 CH2部分重映射PB4 PB5
	        
   //设置该引脚为复用输出功能,输出的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //PB0--TIM3--CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse =0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High  ; //输出极性:TIM输出比较极性高
	
	//☆注意这块的通道数的选择☆ OC
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //☆注意这块的通道数的选择☆
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  
	//☆注意这块的通道数的选择☆

	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	
	cd4052_control();
}




u16 g_finale_lenth[9]; 
u8 g_Son_ID = 1;  //子ID: 1 2 3 4

u8 g_xiaoyu_30cm[9];


u16 g_UT_lenth[9][3];
u8  g_filter_len[9];

void ultrasound_TX(u8 id)  //id: 1 2 3 4
{
	SendUT_ID_1(id);
	SendUT_ID_2(id);
	
	TIM_SetCompare1(TIM3,((1800-1)/2));	
	TIM_SetCompare2(TIM3,((1800-1)/2));	
	//delay_us(50);
	delay_us(g_voicedelay);//25*20=500
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,0);

	delay_us(g_yuzhen_30cm - g_voicedelay);//1900-500=1400
	

	if( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9) == RESET) //PA9--TIM3CH1
	{
		g_xiaoyu_30cm[id] = 1;
	}
	else if( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9) == SET)
	{
		g_xiaoyu_30cm[id]   = 0;
		MY_EXTI_Line_Key(9,1); //open 1CH  //开1接收中断
	}	
	if( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10) == RESET)//PA10--TIM3CH2
	{
		g_xiaoyu_30cm[id+4] = 1;
	}
	else if( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10) == SET)
	{
		g_xiaoyu_30cm[id+4]   = 0;
		MY_EXTI_Line_Key(10,1);//open 1CH
	}	
	

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  //开时钟

	MY_EXTI_Line_Key(9,1); //open 1CH
	MY_EXTI_Line_Key(10,1);//open 1CH
	
	TIM_SetCounter(TIM2, 0);//清零计数器的值		
	delay_us(g_TXvoicedelay*1000 - g_yuzhen_30cm);  //等待20ms = 3.4m
	
				
	MY_EXTI_Line_Key(9,0);//ÆÁ±ÎÍâ²¿ÖÐ¶Ï0 EXTI->IMR|=1<<11;
	MY_EXTI_Line_Key(10,0);//ÆÁ±ÎÍâ²¿ÖÐ¶Ï0 EXTI->IMR|=1<<11;
	g_test[id] = 0;
	g_test[id+4] = 0;
	
	if(!g_flag_exti_get[id]) 	//1通道未捕获到回波
	{
		if(g_filter_len[id] < filter_times)
		{
			g_UT_lenth[id][g_filter_len[id]] = g_dayu255; //大于2.55米
			g_filter_len[id]++;		
			
			MY_EXTI_Line_Key(9,0);
			
		}
	}
	else if(g_flag_exti_get[id]) //1通道捕获到回波
	{
		if(g_xiaoyu_30cm[id])
		{				
			if(g_filter_len[id] < filter_times)
			{
				g_UT_lenth[id][g_filter_len[id]] = g_xiaoyu30; //小于30厘米
				g_filter_len[id]++;			
			}
		}
		else
		{
			if(g_filter_len[id] < filter_times)
			{
				if( !exti_fillter(g_exti_testTIME[id]) )
				{
					g_UT_lenth[id][g_filter_len[id]] = g_exti_testTIME[id][0] *17/100; //30--255
					g_filter_len[id]++;	
				}
			}
		}
	}
	
	if(!g_flag_exti_get[id+4])  	//2通道未捕获到回波
	{	
		if(g_filter_len[id+4] < filter_times)
		{
			g_UT_lenth[id+4][g_filter_len[id+4]] = g_dayu255;  //大于2.55米
			g_filter_len[id+4]++;	
			
			MY_EXTI_Line_Key(10,0);			
		}	
	}
	else if(g_flag_exti_get[id+4])	//2通道捕获到回波
	{
		if(g_xiaoyu_30cm[id+4])
		{
			if(g_filter_len[id+4] < filter_times)
			{
				g_UT_lenth[id+4][g_filter_len[id+4]] = g_xiaoyu30;  //小于30厘米
				g_filter_len[id+4]++;			
			}				
		}
		else
		{
			if(g_filter_len[id+4] < filter_times)
			{


				if( !exti_fillter(g_exti_testTIME[id+4]) )
				{
					g_UT_lenth[id+4][g_filter_len[id+4]] = g_exti_testTIME[id+4][0] *17/100;  //30--255
					g_filter_len[id+4]++;	
				}
			}			
		}		
	}

	g_flag_exti_get[id]   = 0;
	g_flag_exti_get[id+4] = 0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,DISABLE);  //关闭时钟
}

u8 g_flag_check_low;
u8 g_checkbuff[9];
u8 g_checkbuff_len = 1;


void ultrasound_RxHandle(u16 *finale_lenth,u8 ID)  //接收处理  //各个通道4次滤波完再切换 
{
	if(g_checkbuff_len < 9)
	{
		ultrasound_TX(ID);
		g_checkbuff[g_checkbuff_len] = ID;
		g_checkbuff_len++;		
		
		finale_lenth[ID] = g_UT_lenth[ID][0]/10;	
		finale_lenth[ID+4] = g_UT_lenth[ID+4][0]/10;
	
		if(finale_lenth[ID] > 255)
		{
			finale_lenth[ID] = 255;
		}	
		if(finale_lenth[ID+4] > 255)
		{
			finale_lenth[ID+4] = 255;
		}
	}
	
}

u8 g_UT_Send_shunxu[4] = {2,4,1,3}; //1324
void UT_TX_RX(void)
{
	u8 tempi = 0;
		
		for(g_time_ut=0;g_time_ut<4;g_time_ut++)
		{
			g_Son_ID = g_UT_Send_shunxu[g_time_ut];	//1-4	
															
			ultrasound_RxHandle(g_finale_lenth,g_Son_ID);	//采集满4次滤波处理	

			UT_CANSend_u8(g_finale_lenth);	//CAN发送			
		}
		
	
	for(tempi=1;tempi<9;tempi++)
	{
//		New_Filter(g_UT_lenth[ tempi ],g_finale_lenth + tempi );//滤波处理
		g_filter_len[tempi] = 0;
		
	}
	g_checkbuff_len = 1;
}

u8 g_flag_oled_usart = 0; //0:oled 1:usart
u16 g_flag_UT_usart;

void check_NoSensor(void)
{
	u8 i=0;
	u8 temp_y=0;

	for(i=1;i<5;i++)
	{

		switch(i)
		{							//其实是
			case 1: 				//2
				temp_y = 0;
				break;
			case 2: 				//3
				temp_y = 2;
				break;		
			case 3: 				//1
				temp_y = 4;
				break;		
			case 4: 				//4
				temp_y = 6;		
				break;
			default:
				break;
		}	
		OLED_ShowNum(81,temp_y,g_finale_lenth[i+4],5,16);
		OLED_ShowNum(17,temp_y,g_finale_lenth[i],5,16);	
		
	}
	
   	

}


void SendUT_ID_1(u8 id)
{
	switch(id)
	{							//其实是
		case 1: 				//2
			KEY_A1 = 0;
			KEY_B1 = 1;

			break;
		case 2: 				//3
			KEY_A1 = 1;
			KEY_B1 = 0;	
		
			break;		
		case 3: 				//1
			KEY_A1 = 0;
			KEY_B1 = 0;	
		
			break;		
		case 4: 				//4
			KEY_A1 = 1;
			KEY_B1 = 1;	
		
			break;		
		default:
			break;
	}
	delay_us(10);
}

void SendUT_ID_2(u8 id)
{
	switch(id)
	{							//其实是
		case 1: 				//2

			KEY_A2 = 0;
			KEY_B2 = 1;		
			break;
		case 2: 				//3

			KEY_A2 = 1;
			KEY_B2 = 0;			
			break;		
		case 3: 				//1

			KEY_A2 = 0;
			KEY_B2 = 0;			
			break;		
		case 4: 				//4
			KEY_A2 = 1;
			KEY_B2 = 1;			
			break;		
		default:
			break;
	}
	delay_us(10);
}


u16 get_average(u16 *buff,u8 buff_len)
{
	u8 i,j;
	u8 temp_i=0;
	u32 temp_val=0;
	u16 temp = 0;
	
	for(i = 0; i < buff_len; i++) 
	{
		for(j = 0; i + j < buff_len - 1; j++)
		{
			if(buff[j] > buff[j + 1])
			{
				temp = buff[j];
				buff[j] = buff[j + 1];
				buff[j + 1] = temp;
			}            
		}
	}
	
	for(temp_i=buff_len-7;temp_i<buff_len-3;temp_i++)
	{
		temp_val += buff[temp_i];
	
	}
	temp_val = temp_val / 4;
	return temp_val;
	
}


void oled_dis(void)
{
	OLED_ShowString(16,2,(u8 *)"Check Sensor");
	LED3 = !LED3;
	delay_ms(50);
	OLED_Clear();
	LED3 = !LED3;
	delay_ms(200);
	OLED_ShowString(16,2,(u8 *)"Check Sensor");
	LED3 = !LED3;
	delay_ms(50);
	OLED_Clear();
	LED3 = !LED3;
	delay_ms(200);
	OLED_ShowString(16,2,(u8 *)"Check Sensor");
	LED3 = !LED3;
	delay_ms(50);
	OLED_Clear();
	LED3 = !LED3;
	delay_ms(200);
	OLED_ShowString(16,2,(u8 *)"Check Sensor");
	LED3 = !LED3;
	delay_ms(50);
	OLED_Clear();
	LED3 = !LED3;
	delay_ms(200);
}

void Power_ON_delay(void)
{
	LED3 = !LED3;
	delay_ms(50);
	LED3 = !LED3;
	delay_ms(450);
	LED3 = !LED3;
	delay_ms(50);
	LED3 = !LED3;
	delay_ms(450);



}

void UTdata_TX(void)
{
	u8 temp_i=0;
	for(temp_i=1;temp_i<9;temp_i++)
	{
		printf("%d#%d$ ", temp_i ,g_finale_lenth[temp_i]); 
	}
	printf("\r\n"); 
	g_flag_UT_usart = 0; //标记哪路探头丢失

}


//u8 UT_CANsend(void)
//{
//	u8 temp_i=0;
//	u8 flag_CAN_erro=0; //ok
//	
////	for(temp_i=1;temp_i<9;temp_i++)
////	{
////		if(g_flag_UT_usart & 1<<temp_i)
////		{
////			g_finale_lenth[temp_i] = 555; //探头丢失
////		}	
////	}
//	for(temp_i=0;temp_i<2;temp_i++)
//	{
//		flag_CAN_erro = Can_Send_Msg((u8 *)(g_finale_lenth+1+4*temp_i),8);
//	}
//	

//	
////	g_flag_UT_usart = 0; //标记哪路探头丢失 0:清除丢失状态 

//}

u8 g_temp_canbuff[8];
u8 UT_CANSend_u8(u16 *can_buff)
{
	u8 tem_i=0;
	u8 flag_CAN_erro=0; //ok
	
	
	
	for(tem_i=0;tem_i<17;tem_i+=2)
	{
		g_temp_canbuff[tem_i/2] = *((u8 *)can_buff+tem_i+2);
	}
	
	flag_CAN_erro = Can_Send_Msg(g_temp_canbuff,8);
	LED3 = !LED3;
	return flag_CAN_erro;
}


u8 g_can_rx_double;
u8 g_can_rx_len;
u16 canbuf_u16[8];



//u8 Can_Receive_Msg(u8 *buf)
//{		   		   
// 	u32 i;
//	CanRxMsg RxMessage;
//    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//Ã»ÓÐ½ÓÊÕµ½Êý¾Ý,Ö±½ÓÍË³ö 
//    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//¶ÁÈ¡Êý¾Ý	
//    for(i=0;i<8;i++)
//    buf[i]=RxMessage.Data[i];  
//	return RxMessage.DLC;	
//}


u8 Can_Receive_Msg_u16(u16 *buf) //return 16 is ok
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//no data,exit
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//read data	

	if(g_can_rx_double == 1)
	{
		for(i=8;i<16;i++)
		{
			*((u8 *)buf+i)=RxMessage.Data[i-8]; 
		}
		g_can_rx_double = 0;
		g_can_rx_len += RxMessage.DLC;
	}
	else if(g_can_rx_double == 0)
	{
		for(i=0;i<8;i++)
		{
			*((u8 *)buf+i)=RxMessage.Data[i]; 	
		}
		g_can_rx_double++;	
		g_can_rx_len = RxMessage.DLC;
	}
	return g_can_rx_len;
}



