/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

#include <stdio.h>

#define RXBUFFERSIZE 1
uint8_t aUart1RxBuffer[RXBUFFERSIZE];


//RAM_D2 uint8_t USART_RX_BUF[USART_RX_LEN];
CCMRAM uint8_t USART_REC_BUF[USART_REC_LEN];

uint32_t  USART_RX_CNT=0;			//接收的字节数


/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  HAL_UART_Receive_IT(&huart1, (unsigned char *)aUart1RxBuffer, RXBUFFERSIZE);
  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


/*****************  发送字符串 **********************/
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do
  {
      HAL_UART_Transmit(&huart1,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');

}

/***********/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)  //中断执行任务函数  �???????????  中断服务函数调用
{
//	char rec_data;
	uint8_t rec_data;
	//uart_it++;
	//portBASE_TYPE higher_priority_task_woken = 0;
	if(huart->Instance==USART1)//如果是串�???????????1
	{

		rec_data=aUart1RxBuffer[0];
#if 1
		/* 把接收到的数据保存到环形buffer里面*/
		/***
		*(g_stDebugUartRevBuf.pbyBuf + g_stDebugUartRevBuf.wWritePos++) = rec_data;
		g_stDebugUartRevBuf.wBufSize++;

		if(g_stDebugUartRevBuf.wWritePos >= g_stDebugUartRevBuf.wBufMax)
		{
			g_stDebugUartRevBuf.wWritePos = 0;
		}
		***/

		//Uart1RxBuf[uart1_it++] = rec_data;

		if(USART_RX_CNT<USART_REC_LEN)
		{
			//USART_RX_BUF[USART_RX_CNT] = rec_data;
			USART_REC_BUF[USART_RX_CNT] = rec_data;
			USART_RX_CNT++;
		}

#else
		HAL_UART_Transmit(&huart1, (uint8_t *)&rec_data, 1, 0xFFFF);
#endif
		HAL_UART_Receive_IT(huart, (unsigned char *)aUart1RxBuffer, RXBUFFERSIZE);
	}

}



/***********/

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(FILE *f)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
//此处用的是USART1
//你在使用时仅仅需要把’USART1‘改成你的串口就可以�??
    //while ((USART1->SR & 0X40) == 0);
    //USART1->DR = (uint8_t) ch;
    //return ch;
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);	//发�?�一个字�?
    return ch;
}
GETCHAR_PROTOTYPE
{
	uint8_t ch;
	/*if (rx_event_semaphore != NULL)
	{
		ch = 'p';
		HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
	    xSemaphoreTake(rx_event_semaphore, portMAX_DELAY);
	}*/
//	int i=0;
//	while(SYS_ERROR == RingBufGetVal(&g_stDebugUartRevBuf, &ch))
//	{
//		osDelay(10);
//		if((i++) >1000)
//		{
//			USART1_BUF_Init();
//			break;
//		}
//	}

	return  (int)ch;
}


/* USER CODE END 1 */

//#ifdef __GNUC__
////  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf *****/
//// /*****    set to 'Yes') calls __io_putchar() */
//
//#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */
//
///**
//  * @brief  Retargets the C library printf function to the USART.
//  * @param  None
//  * @retval None
//  */
//
//PUTCHAR_PROTOTYPE
//{
//  HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xFFFF);
//  return ch;
//}
//
////作�?�：天上的星�?????????????260
////链接：https://www.jianshu.com/p/b102eccbf65e
////来源：简�?????????????
////著作权归作�?�所有�?�商业转载请联系作�?�获得授权，非商业转载请注明出处�?????????????



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
