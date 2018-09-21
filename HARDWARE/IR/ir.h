#ifndef __IR_H
#define __IR_H	 
#include "include.h"

#define  R_DATA 	PAin(1)	 	//红外数据输入脚
#define  FLASH_SAVE_ADDR  0X08020000 	//预留128k用户空间   设置FLASH 保存地址(必须为偶数，且其值要大于【用户代码】所占用FLASH的大小+0X08000000) 
#define  COOL_FLASH_SAVE_ADDR  0X08030000//制冷  后64k的空间  192k开始
#define IR_TXBUff IR_RXBUff

//红外发送接收的持续时间,跟编解码有关
#define	H_IR    2500	//高电平持续时间
#define L_IR    500		//低电平持续时间
#define Half_IR ((H_IR+L_IR)/2)




#define KEY_A1 PBout(12)
#define KEY_B1 PBout(13)

#define KEY_A2 PBout(14)
#define KEY_B2 PBout(15)

#define g_flag_UT_255      2550   //超过2m





extern u8 g_Send_CAN_or_USART;	//串口发送
extern u8 g_can_rx_double;
extern u8 g_can_rx_len;
extern u16 canbuf_u16[8];

extern u16 g_voicedelay;
extern u16 g_TXvoicedelay;


extern u8  g_filter_len[9];
extern u16 g_finale_lenth[9];

extern u16 ii;



//extern u16 IR_TXBUff[500];

extern u8 key_sta; 

extern u16 LOW_value;
extern u16 HIGH_value;

extern u8 g_NoSensor;
extern u8 g_flag_NoSensor[17];


extern u8 g_Son_ID;
extern u8 g_flag_oled_usart;


extern u8 filter_times;
extern u8 g_flag_check_low;
extern u8 g_time_ut;
extern u16 g_yuzhen_30cm; 
extern u16 g_UT_lenth[9][3];

extern u8 g_checkbuff[9];
extern u8 g_checkbuff_len ;

extern u16 g_dayu255 ;
extern u8 g_xiaoyu30 ;
extern u8 g_UT_miss  ;



void ir_re_init(void);

void TIM3_2PWM_Init(u16 arr,u16 psc);  //PWM发射初始化  PA7--TIM3--CH2

u16 filter_super(u16 *num, u8 n);
u8 check_0(u16 *buff,u8 len);
void ultrasound_interrupt(u8 ID,u8 TIM,u8 Channel);
void ultrasound_TX(u8 id);
void ultrasound_RxHandle(u16 *finale_lenth,u8 ID);
void check_NoSensor(void);
void SendUT_ID_1(u8 id);
void SendUT_ID_2(u8 id);
u8 check_all_num(u16 num,u16 *buff,u8 len);
u16 get_average(u16 *buff,u8 buff_len);
void self_check(void);
void oled_dis(void);
void UTdata_TX(void);
u8 UT_CANsend(void);
u8 Can_Receive_Msg_u16(u16 *buf); //return 16 is ok
u8 Can_Receive_Msg(u8 *buf);
u8 UT_CANSend_u8(u16 *can_buff);

void UT_TX_RX(void);
void cd4052_control(void);
void Power_ON_delay(void);

#endif
