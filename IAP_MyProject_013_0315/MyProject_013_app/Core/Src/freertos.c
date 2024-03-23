/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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

#include "usart.h"
#include <stdio.h>
#include <string.h>

#include "lwip.h"
#include "netif.h"
#include "lwipopts.h"
#include "lwip/api.h"

#include "FreeRTOSConfig.h"


#include "tcp_client.h"
#include "tcp_process.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

const unsigned char CCDQUERRY_TCP_cmd[] = "{\"body\": {\"netType\": \"TCP\"},\"cmd\": \"CCDBOOST_COMMAND_QUERRYCCDSERVICE\",\"sessionId\": \"Amx2Test_SessionId\",\"clientId\": \"Amx2Test_ClientId\",\"type\": \"BT_REQUEST\"}";


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
osThreadId myTask02Handle;
osThreadId myTask03Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

osThreadId uart1TaskHandle;
void StartUart1Task(void const * argument);
void StartUart1RecvTask(void const * argument);

osThreadId TCPClientTaskHandle;
void StartTCPClientTask(void const * argument);

osThreadId  tcp_clientHandle;
//void tcp_clientTask(void const * argument);

void print_Task(void *parg);


/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);

extern void MX_LWIP_Init(void);
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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 1024);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityIdle, 0, 1024);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  //osThreadDef(myTask03, StartTask03, osPriorityIdle, 0, 1024);
  //myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  osThreadDef(uart1Task, StartUart1RecvTask, osPriorityAboveNormal, 0, 1024);   //  StartUart1Task
  uart1TaskHandle = osThreadCreate(osThread(uart1Task), NULL);

  //StartUart1RecvTask

  //osThreadDef(tcp_client, tcp_clientTask, osPriorityAboveNormal, 0, 1024);
  //tcp_clientHandle = osThreadCreate(osThread(tcp_client), NULL);

  //xTaskCreate(tcp_client, "tcp_client", 512, NULL, 3, NULL); // 创建TCP客户端任�????

  printf("MX_FREERTOS_Init init ...003 ...\n\n");

  //xTaskCreate(tcp_client, "tcp_client", 4096, NULL, osPriorityNormal, NULL); // 创建TCP客户端任�????   //3

  printf("MX_FREERTOS_Init init ...004 ...\n\n");

  //xTaskCreate(print_Task, "print_Task", 512, NULL, 2, NULL); // 创建TCP客户端任�????

  printf("MX_FREERTOS_Init init ...009 ...\n\n");

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
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
	  printf("StartDefaultTask loop ...  this is app\n\n");
	  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);
	  osDelay(500);
	  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_SET);
      osDelay(500);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/


__lwip_dev lwipdev;						//lwip鎺у埗缁撴瀯锟�???????????

uint8_t tcp_flag = 0;

/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */

	uint32_t ip=0, netmask=0, gw=0 ;
	unsigned int remain_size = 0;

	int printfstate_on =1;

  /* Infinite loop */
  for(;;)
  {
	  /**
	   *
	   * https://blog.csdn.net/weixin_42328389/article/details/121153037
	   *
	   * ***/
	    if(printfstate_on){
	      //printfstate_on =0;
	      uint8_t mytaskstatebuffer[500];
	      printf("==================================\n\n");
	      //printf("任务�???????          任务状�??    优先�???????     剩余�???????   任务序号\r\n");  osPriorityAboveNormal
	      printf("TaskName    TaskState  Priority  RemainStask  TaskNumber\n\n");
	      osThreadList((uint8_t *)&mytaskstatebuffer);
	      printf("%s\n\n",mytaskstatebuffer);
	    }

	    if(printfstate_on)   //LWIP_DHCP
	     {
			  //printf("ipaddr.addr: %ld\n",ipaddr.addr);   //ipaddr.addr = 0;
			  //printf("netmask.addr: %ld\n",netmask.addr);   //netmask.addr = 0;
			  //printf("gw.addr: %ld\n\n\n",gw.addr);   //gw.addr = 0;

			  ip = gnetif.ip_addr.addr;

			  if(tcp_flag == 0 && ip != 0)
			  {
				  tcp_flag=1;
				  //xTaskCreate(tcp_client, "tcp_client", 4096, NULL, osPriorityNormal, NULL); // 创建TCP客户端任�????   //3

				  tcp_client_init();

				  printf("tcp_client create success ...\n\n");
			  }

			  printf("gnetif.ip_addr.addr: %ld\n",gnetif.ip_addr.addr);
			  lwipdev.ip[3]=(uint8_t)(ip>>24);
			  lwipdev.ip[2]=(uint8_t)(ip>>16);
			  lwipdev.ip[1]=(uint8_t)(ip>>8);
			  lwipdev.ip[0]=(uint8_t)(ip);
			  printf("DHCP:IP.......%d.%d.%d.%d\r\n",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);

			  netmask = gnetif.netmask.addr;
			  lwipdev.netmask[3]=(uint8_t)(netmask>>24);
			  lwipdev.netmask[2]=(uint8_t)(netmask>>16);
			  lwipdev.netmask[1]=(uint8_t)(netmask>>8);
			  lwipdev.netmask[0]=(uint8_t)(netmask);
			  printf("DHCP:Netmask......%d.%d.%d.%d\r\n",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);

			  gw = gnetif.gw.addr;
			  lwipdev.gateway[3]=(uint8_t)(gw>>24);
			  lwipdev.gateway[2]=(uint8_t)(gw>>16);
			  lwipdev.gateway[1]=(uint8_t)(gw>>8);
			  lwipdev.gateway[0]=(uint8_t)(gw);
			  printf("DHCP:gateway.....%d.%d.%d.%d\r\n",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);
		  }

		  remain_size = xPortGetFreeHeapSize();
		  printf("remain heap size:%d\n\n", remain_size);

    osDelay(5000);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void const * argument)
{
  /* USER CODE BEGIN StartTask03 */
  /* Infinite loop */
	uint8_t tcpState = 0;

	/***
	 * 	STA_NET_BEGIN = 0,
	STA_NET_NO_CONNECT = 1,//网络未连�????
	STA_NET_CONNECT = 2,//网络连接，PORT CCD服务未连�????
	STA_CCDPORT = 3,//CCD服务连接
	STA_CCDPORT_CONNECT = 4,//CCD服务连接
	STA_LOGINPORT = 5,//CCD服务连接
	STA_LOGINPORT_CONNECT = 6,//CCD服务连接
	STA_LOGIN_IN = 7,//已登�????
	STA_LOGIN_OUT = 8//已登�????
	 ****/

	tcpState = STA_TCP_NO_CONNECT;

    for(;;)
    {
		switch(tcpState)
		{
			case STA_TCP_NO_CONNECT:
				//CCDQUERRY_TCP_cmd
				//tcp_client_write(CCDQUERRY_TCP_cmd, sizeof(CCDQUERRY_TCP_cmd));
				printf("---STA_NET_NO_CONNECT...\n\n");
				//if(connect_flag == pdTRUE)
				{
					//packVwasCmd(CCDQUERRY_TCP_cmd);
					tcpState = STA_TCP_CONNECT;
				}
				break;

			case STA_TCP_CONNECT:
				printf("---tcpState = %d, STA_TCP_CONNECT...\n\n", tcpState);

				break;
			case STA_CCDPORT:
				printf("---STA_NET_CONNECT...\n\n");
				break;

			case STA_CCDPORT_CONNECT:
			//case MODESCREEN:
				STA_LOGINPORT;
				break;
			default:
				//refreshSTATUSMONITORING_2();
				break;
		}

	  osDelay(3000);
  }
  /* USER CODE END StartTask03 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

int DHCP_flag = 1;


void StartUart1RecvTask(void const * argument)
{

	printf("-----StartUart1Task--- 001\n");

	for(;;)
	{
	  if(uart1_it > 0)
	  {
		  printf("-----uart1 receive %d data:\n", uart1_it);
		  for(int i = 0; i < uart1_it; i++ )
		  printf("%c", Uart1RxBuf[i]);
		  uart1_it=0;
	  }

	  osDelay(20);
	}
}


void StartUart1Task(void const * argument)
{

	printf("-----StartUart1Task--- 001\n");

	for(;;)
	{
	  if(uart1_it > 0)
	  {
		  printf("-----uart1 receive %d data:\n", uart1_it);
		  for(int i = 0; i < uart1_it; i++ )
		  printf("%c", Uart1RxBuf[i]);

		  if(Uart1RxBuf[0] == 's')
		  {
			  // vTaskDelete(LED1Task_Handler);
			  //vTaskSuspend(myTask02Handle);
			  if(DHCP_flag == 1)
			  {
				  vTaskDelete(LinkThrHandle);
				  vTaskDelete(tcpip_threadHandle);
				  vTaskDelete(EthIfHandle);

				  dhcp_release_and_stop(&gnetif);

				  printf("--------------vTaskDelete finish ...\n\n");

				  osDelay(5000);

				  printf("--------------netif_remove start ...\n\n");
				  netif_remove(&gnetif);

				  printf("--------------MX_LWIP_Init_static start ...\n\n");
				  MX_LWIP_Init_static();
				  DHCP_flag = 0;
			  }
			  printf("--------------switch static ip finish ...\n\n");
		  }

		  if(Uart1RxBuf[0] == 't')
		  {
			  printf("--------------netif_remove start ...\n\n");
			  netif_remove(&gnetif);
			  printf("--------------MX_LWIP_Init_static start ...\n\n");
			  MX_LWIP_Init_static();
			  printf("--------------vTaskSuspend finish ...\n\n");
		  }

		  if(Uart1RxBuf[0] == 'r')
		  {
			  //vTaskResume(myTask02Handle);
			  vTaskResume(LinkThrHandle);
			  vTaskResume(tcpip_threadHandle);
			  vTaskResume(EthIfHandle);
			  printf("--------------vTaskResume finish ...\n\n");
		  }
		  //MX_LWIP_Init_DHCP
		  if(Uart1RxBuf[0] == 'd')
		  {
			  if(DHCP_flag == 0)
			  {
				  vTaskDelete(LinkThrHandle);
				  vTaskDelete(tcpip_threadHandle);
				  vTaskDelete(EthIfHandle);
				  dhcp_release_and_stop(&gnetif);
				  printf("--------------vTaskDelete finish ...\n\n");
				  osDelay(5000);
				  printf("--------------DHCP begin ...\n\n");
				  netif_remove(&gnetif);
				  printf("--------------netif_remove finish ...\n\n");
				  MX_LWIP_Init();
				  printf("--------------MX_LWIP_Init_DHCP finish ...\n\n");
				  DHCP_flag = 1;
			  }

			  printf("--------------switch DHCP finish ...\n\n");
		  }

		  if(Uart1RxBuf[0] == '0')
		  {
			  printf("----------tcp_client_write begin ...\n\n");
			  //tcp_client_write((const)Uart1RxBuf, uart1_it);

			  tcp_client_send_data(Uart1RxBuf, uart1_it);
		  }

		  printf("StartUart1Task--------finish------\n\n");
		  memset((char*)Uart1RxBuf, 0, RXBUFMAX);
		  uart1_it = 0;
	  }
    osDelay(20);
  }
}


void print_Task(void *parg)
{

	while(1)
	{
		printf("print_Task-----\n\n");

		osDelay(2000);
	}

}


/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
