#include "filter.h"
#include "math.h"





u16 filter_super(u16 *num, u8 n)       
{
	u8 i, j,k;
	u16 sum= 0;	
	u16 temp = 0;
//	u8 temp_i=0;
//	u8 temp_j=0;

	for(i = 0; i < n; i++) 
	{
		for(j = 0; i + j < n - 1; j++)
		{
			if(num[j] > num[j + 1])
			{
				temp = num[j];
				num[j] = num[j + 1];
				num[j + 1] = temp;
			}            
		}
	}
	
	
	for(k=n-4;k<n-2;k++)  //????????  ??0123 45 67
	{
		sum += num[k];
	}
	
//	for(k=(n/2);k<(n/2+2);k++)  //????????  ??
//	{
//		sum += num[k];
//	}
	sum=sum / 2;
	return sum;
}


// 限幅消抖滤波法

u16 g_UT_3filter_val[17][20]; 
u8 g_UT_filter_i[17];
u16 g_UT_true_val[17];
u16 g_UT_flag_ErroVal[17];


u16 UT_Filter(u16 UT_NewVal,u8 ID) 
{
    u16 Value=0;

	Value = g_UT_true_val[ID];
	if(((UT_NewVal - Value) > FILTER_A) || ((Value - UT_NewVal) > FILTER_A))
	{
		g_UT_3filter_val[ID][g_UT_filter_i[ID]] = UT_NewVal;  //??? 
		g_UT_filter_i[ID]++;
		if(g_UT_filter_i[ID] >= FILTER_times)
		{
			g_UT_true_val[ID] = filter_super(g_UT_3filter_val[ID],FILTER_times);//8????56????,???
			g_UT_flag_ErroVal[ID]++;
			g_UT_filter_i[ID] = 0;
		}
	}
	else
	{
		g_UT_filter_i[ID] = 0;
		g_UT_true_val[ID] = UT_NewVal; 
	}
	return g_UT_true_val[ID]; 
}


void New_Filter(u16 *UT_RX_buff,u16 *ID_buff) 
{
    u16 Value=0;
	int temp_12=0;

	
	temp_12 = UT_RX_buff[0] - UT_RX_buff[1] ;

	
	if( abs(temp_12) < FILTER_A  )
	{
		 
		Value = (UT_RX_buff[0] + UT_RX_buff[1]) / 20;
		if(Value > 255)
		{
			Value = 255;
		}
		else if(Value < 35)
		{
			Value = 0;
		}

		
		*ID_buff = Value;
	}
	else
	{
		*ID_buff = *ID_buff;
	}
	
}

void zijian_Filter(u16 *UT_RX_buff,u16 *ID_buff) 
{
//    u16 Value=0;

//	if( ( (UT_RX_buff[0] - UT_RX_buff[1]) < FILTER_A ) || ( (UT_RX_buff[2] - UT_RX_buff[1]) < FILTER_A ) )
//	{	 
//		Value = (UT_RX_buff[1] + UT_RX_buff[2]) / 2;

//		*ID_buff = Value;
//	}
//	else
//	{
//		*ID_buff = Value;
//	}
	
	u16 Value=0;
	int temp_12=0;

	
	temp_12 = UT_RX_buff[0] - UT_RX_buff[1] ;

	
	if( abs(temp_12) < FILTER_A  )
	{
		 
		Value = (UT_RX_buff[0] + UT_RX_buff[1]) / 20;
		if(Value > 255)
		{
			Value = 255;
		}
		*ID_buff = Value;
	}
	else
	{
		*ID_buff = *ID_buff;
	}	
	
	
}

u8 exti_fillter(u16 *exti_timeBUFF)
{
	u8 temp_cha[5]={0};
	u8 temp_i=0;
	u8 temp_flag=0;
	
	do
	{
		temp_cha[temp_i] = exti_timeBUFF[temp_i+1] - exti_timeBUFF[temp_i];
		temp_i++;
	}while(temp_cha[temp_i-1]>g_UT_Min_huibo && temp_cha[temp_i-1]<g_UT_Max_huibo && temp_i<4);
	
	if(temp_i >= 4)
	{
		temp_flag = 0;	//回波正常
	}
	else
	{
		temp_flag = 1; //回波宽度不够100us,滤除
	}
	return temp_flag;
	
	
	
}



void my_Filter(u16 *UT_RX_buff,u16 *ID_buff) 
{
    u16 Value=0;
	int temp_12=0;

	
	temp_12 = UT_RX_buff[0] - UT_RX_buff[1] ;

	
	if( abs(temp_12) < FILTER_A  )
	{
		 
		Value = (UT_RX_buff[0] + UT_RX_buff[1]) / 20;
		if(Value > 255)
		{
			Value = 255;
		}
		else if(Value < 35)
		{
			Value = 0;
		}

		
		*ID_buff = Value;
	}
	else
	{
		*ID_buff = *ID_buff;
	}
	
}



