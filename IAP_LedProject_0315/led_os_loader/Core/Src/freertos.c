/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "iap.h"
#include "delay.h"
#include "led.h"
#include "stmflash.h"

#include "usart.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

osThreadId uart1TaskHandle;

void Uart1RecvTask(void const * argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 4 */
__weak void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */

  osThreadDef(uart1Task, Uart1RecvTask, osPriorityHigh, 0, 1024);
  uart1TaskHandle = osThreadCreate(osThread(uart1Task), NULL);

  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
	  printf("StartDefaultTask\r\n");
	  osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void Uart1RecvTask(void const * argument)
{

	uint16_t t = 0;
	uint32_t oldcount=0;	//老的串口接收数据�????
	uint32_t applenth=0;	//接收到的app代码长度

	for(;;)
	{
		if(USART_RX_CNT)
		{
//			printf("USART_RX_CNT = %d, oldcount = %d\n\n", USART_RX_CNT, oldcount);
			if(oldcount==USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				applenth = USART_RX_CNT;
				oldcount = 0;
				USART_RX_CNT = 0;
//				printf("用户程序接收完成!\r\n");
//				printf("代码长度:%dBytes\r\n",applenth);
				printf("app receive finish!\r\n");
				printf("app length:%d Bytes\r\n",applenth);

//				printf("USART_RX_BUF:\r\n");
//				for(int i = 0; i < 8; i++)
//					printf("0x%x ", USART_REC_BUF[i]);
//				printf("\n\n");
//				printf("flash data before:\n");
//				PrintFlashTest(4,FLASH_APP1_ADDR);

				// recv finish, and write app data to flash
//				printf("�????始更新固�????...\r\n");
//				LCD_ShowString(30,210,200,16,16,"Copying APP2FLASH...");
				printf("begin update app\r\n");
//				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否�????0X08XXXXXX.
				printf("ram addr data:0x%x\r\n", ((*(vu32*)(0x68000000))) );

				for(int i= 0; i < 4; i++)
				{
					printf("i = 0x%x, ramdata =  0x%x\r\n", i, ((*(vu32*)(0x68000000+4*i))));
				}

				printf("iap_write_appbin begin ...\r\n");
//				if(((*(vu32*)(0x68000000+4))&0xFF000000)==0x08000000)//判断是否�????0X08XXXXXX.
				if(((*(vu32*)(0x10000000+4))&0xFF000000)==0x08000000)//判断是否�????0X08XXXXXX.
				{
					iap_write_appbin(FLASH_APP1_ADDR, USART_REC_BUF, applenth);//更新FLASH代码   applenth
//					uint32_t addr_offset = 0;
//					for(uint32_t i = 0; i< applenth/2048; i++)
//					{
//						addr_offset += i*2048;
//						iap_write_appbin(FLASH_APP1_ADDR+addr_offset, &USART_REC_BUF[addr_offset], 2048);//更新FLASH代码   applenth
//					}
//					addr_offset += 2048;
//					iap_write_appbin(FLASH_APP1_ADDR+addr_offset, &USART_REC_BUF[addr_offset], applenth%2048);//更新FLASH代码   applenth
//					LCD_ShowString(30,210,200,16,16,"Copy APP Successed!!");
//					printf("固件更新完成!\r\n");
					printf("iap_write_appbin finish\r\n");

//					for(int i =0 ;i <8; i++)
//						printf("0x%x ", USART_REC_BUF[i]);
//					printf("\n\n\n");
//					PrintFlashTest(100,FLASH_APP1_ADDR);
//					delay_ms(500);

					for(int i= 0; i < applenth/4; i++)
					{
						printf("ADDR = 0x%x, data =  0x%x\r\n", FLASH_APP1_ADDR+4*i, ((*(vu32*)(FLASH_APP1_ADDR+4*i))));
					}

					//start run app
//					printf("�????始执行FLASH用户代码!!\r\n");
					printf("iap_load_app begin \r\n");


					printf("FLASH_APP1_ADDR  : 0x%x\r\n", ((*(vu32*)(FLASH_APP1_ADDR))));
					printf("FLASH_APP1_ADDR+4: 0x%x\r\n", ((*(vu32*)(FLASH_APP1_ADDR+4))));

					if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否�????0X08XXXXXX.
					{
//						iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
						BSPJumpToReset(FLASH_APP1_ADDR);
					}else
					{
//						printf("非FLASH应用程序,无法执行!\r\n");
//						LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!");
						printf("--FLASH_APP1_ADDR  : 0x%x\r\n", ((*(vu32*)(FLASH_APP1_ADDR))));
						printf("--FLASH_APP1_ADDR+4: 0x%x\r\n", ((*(vu32*)(FLASH_APP1_ADDR+4))));
						printf("--can not execute app, it is not flash app\r\n");
					}

				}else
				{
//					LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!  ");
//					printf("非FLASH应用程序!\r\n");
					printf("can not update app, it is not flash app\r\n");
				}

			}else  oldcount = USART_RX_CNT;
		}

		t++;
//		delay_ms(20);
//		HAL_Delay(20);
		osDelay(20);
		if(t==30)
		{
//			printf("LED0 ... \n\n");
			LED0=!LED0;
			t=0;
		}
	}

}



/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
