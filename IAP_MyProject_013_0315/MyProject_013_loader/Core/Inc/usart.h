/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */


#define RXBUFFERSIZE 1
uint8_t aUart1RxBuffer[RXBUFFERSIZE];

#define RXBUFMAX  16
uint8_t Uart1RxBuf[RXBUFMAX];

extern uint16_t uart1_it;

#define USART_RX_LEN  			128     //定义�??大接收字节数 120K
#define USART_REC_LEN  			128*1024 //定义�??大接收字节数 120K
extern uint8_t  USART_RX_BUF[USART_RX_LEN]; //接收缓冲,�??大USART_REC_LEN个字�??.末字节为换行�??
extern uint8_t  USART_REC_BUF[USART_REC_LEN];
extern uint16_t USART_RX_STA;         		//接收状�?�标�??
extern uint32_t USART_RX_CNT;				//接收的字节数

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
