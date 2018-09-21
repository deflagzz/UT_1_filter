#include "lock.h"




u32 g_CpuID[3];
u32 g_Lock_Code;


void GetLockCode(void)
{
	//??CPU??ID
	g_CpuID[0]=*(vu32*)(0x1ffff7e8);
	g_CpuID[1]=*(vu32*)(0x1ffff7ec);
	g_CpuID[2]=*(vu32*)(0x1ffff7f0);
	//????,????????
	g_Lock_Code=((g_CpuID[0]>>0)+(g_CpuID[1]>>1)+(g_CpuID[2]>>2)) / 2;
}



