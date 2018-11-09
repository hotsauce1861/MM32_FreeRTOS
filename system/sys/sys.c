#include "sys.h"

/********************************************************************************************************
**函数信息 ：System_Clock_Init(u8 PLL)                     
**功能描述 ：外部时钟倍频后作为系统时钟
**输入参数 ：PLL：倍频倍数
**输出参数 ：无
********************************************************************************************************/
void System_Clock_Init(u8 PLL)
{
    unsigned char temp=0;   
    RCC->CR|=RCC_CR_HSEON;  //外部高速时钟使能HSEON
    while(!(RCC->CR&RCC_CR_HSERDY));//等待外部时钟就绪
    RCC->CFGR=RCC_CFGR_PPRE1_2; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
    
    RCC->CFGR|=RCC_CFGR_PLLSRC;	  //PLLSRC ON 
    RCC->CR &=~(RCC_CR_PLLON);		//清PLL//	RCC->CR &=~(7<<20);		//清PLL
    
    RCC->CR &=~(0x1f<<26);	
    RCC->CR|=(PLL - 1) << 26;   //设置PLL值 2~16
    
    FLASH->ACR|=FLASH_ACR_LATENCY_1|FLASH_ACR_PRFTBE|FLASH_ACR_PRFTBS;	  //FLASH 2个延时周期
    
    RCC->CR|=RCC_CR_PLLON;  //PLLON
    while(!(RCC->CR&RCC_CR_PLLRDY));//等待PLL锁定
    RCC->CFGR|=RCC_CFGR_SW_PLL;//PLL作为系统时钟	 
    while(temp!=0x02)     //等待PLL作为系统时钟设置成功
    {    
        temp=RCC->CFGR>>2;
        temp&=0x03;
    }    
}	





void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
    u32 temp,temp1;	  
    temp1=(~NVIC_Group)&0x07;//取后三位
    temp1<<=8;
    temp=SCB->AIRCR;  //读取先前的设置
    temp&=0X0000F8FF; //清空先前分组
    temp|=0X05FA0000; //写入钥匙
    temp|=temp1;	   
    SCB->AIRCR=temp;  //设置分组	    	  				   
}
//设置NVIC 
//NVIC_PreemptionPriority:抢占优先级
//NVIC_SubPriority       :响应优先级
//NVIC_Channel           :中断编号
//NVIC_Group             :中断分组 0~4
//注意优先级不能超过设定的组的范围!否则会有意想不到的错误
//组划分:
//组0:0位抢占优先级,4位响应优先级
//组1:1位抢占优先级,3位响应优先级
//组2:2位抢占优先级,2位响应优先级
//组3:3位抢占优先级,1位响应优先级
//组4:4位抢占优先级,0位响应优先级
//NVIC_SubPriority和NVIC_PreemptionPriority的原则是,数值越小,越优先	   
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
    u32 temp;	
    MY_NVIC_PriorityGroupConfig(NVIC_Group);//设置分组
    temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
    temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
    temp&=0xf;								//取低四位  
    NVIC->ISER[NVIC_Channel/32]|=(1<<NVIC_Channel%32);//使能中断位(要清除的话,相反操作就OK) 
    NVIC->IP[NVIC_Channel]|=temp<<4;		//设置响应优先级和抢断优先级   	    	  				   
} 

