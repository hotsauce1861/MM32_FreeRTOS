#ifndef __SYS_H 			   

#include "HAL_device.h"
#include "stdio.h"

void SystemClk_HSEInit(void);
void System_Clock_Init(u8 PLL);

void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);
#endif





























