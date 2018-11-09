//#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "HAL_gpio.h"

#define STACK_SIZE								128

#define LED_OFF()                 do {GPIO_SetBits(GPIOA,GPIO_Pin_8);}while(0)
#define LED_ON()                  do {GPIO_ResetBits(GPIOA,GPIO_Pin_8);}while(0)
#define LED_TOGGLE()  						do {(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8)) \
																	?(GPIO_ResetBits(GPIOA,GPIO_Pin_8)) \
																	:(GPIO_SetBits(GPIOA,GPIO_Pin_8));} while(0)

void prvSetupHardware(void);
																	
void vCreateFlashTasks(void);																
void vTaskCodeLEDON( void * pvParameters );																	
void vTaskCodeLEDOFF( void * pvParameters );
																	
int main( void )
{
   /* Setup the microcontroller hardware for the demo. */
   prvSetupHardware();

   /* Leave this function. */
   vCreateFlashTasks();

   /*Start the RTOS scheduler. */
   vTaskStartScheduler();

   /* Should never get here! */
}

void prvSetupHardware(void){
	

    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); 
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		LED_OFF();
		//System_Clock_Init(6);
		//delay_init();
		//LED_ON();
}
#if 1
void vCreateFlashTasks(void){
	
		BaseType_t xReturned;
		TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
                    vTaskCodeLEDON,       /* Function that implements the task. */
                    "LEDON",          /* Text name for the task. */
                    STACK_SIZE,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY + 1,/* Priority at which the task is created. */
                    &xHandle );      /* Used to pass out the created task's handle. */
										
										
    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
                    vTaskCodeLEDOFF,       /* Function that implements the task. */
                    "LEDOFF",          /* Text name for the task. */
                    STACK_SIZE,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY + 2,/* Priority at which the task is created. */
                    &xHandle );      /* Used to pass out the created task's handle. */			
										
    if( xReturned == pdPASS )
    {
        /* The task was created.  Use the task's handle to delete the task. */
        //vTaskDelete( xHandle );
    }
	
}
/* Task to be created. */
void vTaskCodeLEDOFF( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below. 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
		*/
		const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
    for( ;; )
    {			
			LED_ON();	
			vTaskDelay(xDelay);
    }
}

/* Task to be created. */
void vTaskCodeLEDON( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below. 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
		*/
		const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
		//LED_ON();
    for( ;; )
    {			
			/* Task code goes here. */
			LED_OFF();	
			vTaskDelay(xDelay);

    }
}
#endif
