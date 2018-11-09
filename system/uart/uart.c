#include "uart.h"
#include "sys.h"
#ifdef __GNUC__

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

#endif 

#ifdef USE_IAR
PUTCHAR_PROTOTYPE
{
    while((UART1->CSR&((uint16_t)0x0001))==0);//循环发送,直到发送完毕   
    UART1->TDR = (ch & (uint16_t)0x00FF);      
    return ch;
}

#else
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
    int handle; 
    
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
    x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
    while((UART1->CSR&((uint16_t)0x0001))==0);//循环发送,直到发送完毕   
    UART1->TDR = (ch & (uint16_t)0x00FF);      
    return ch;
}

#endif 



void uart_initwBaudRate(u32 bound)
{  	 
    
    u32 tempBaud;	
    
    //-------------------------------------------------
    RCC->AHBENR|=1<<17;   //使能PORTA口时钟  
    RCC->APB2ENR|=1<<14;  //使能串口时钟 
    
    RCC->APB2RSTR|=1<<14;   //复位串口1
    RCC->APB2RSTR&=~(1<<14);//停止复位	 
    
    GPIOA->AFRH&=0x00f;
    GPIOA->AFRH|=0x110;
    
    //-------------------------------------------------	
    //波特率设置 
    /* Determine the uart_baud*/
    tempBaud = (SystemCoreClock / 16) /(bound);
    UART1->FRA=(SystemCoreClock / 16) %(bound);
    /* Write to UART BRR */
    UART1->BRR = tempBaud;
    UART1->CCR|=0X30;  //1位停止,无校验位.
    //-------------------------------------------------	
    //使能接收中断 
    UART1->GCR = 0X19;			//收发使能	UART1->CCR|=1<<5;    //接收缓冲区非空中断使能	
    UART1->IER = 0X2;			//接收中断使能
    /*
    23.5.5 UART 中断使能寄存器(UART_IER)
    偏移地址：0x10
    复位值：0x0000
    位1
    RXIEN:接收缓冲中断使能位
    1=中断使能
    0=中断禁止	
    */
    UART1->ICR = 0X2;			//清接收中断
    /*
    23.5.6 UART 中断清除寄存器(UART_ICR)
    偏移地址：0x14
    复位值：0x0000
    位1
    RXICLR: 接收中断清除位
    1=中断清除
    0=中断没有清除
    */
    /*
    位14
    UART1EN：UART1时钟使能
    由软件置’1’或清’0’
    0：UART1时钟关闭；
    1：UART1时钟开启。	
    */
    GPIOA->CRH&=0XFFFFF00F;//IO状态设置
    GPIOA->CRH|=0X000008B0;//IO状态设置
    
}

