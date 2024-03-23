/**
  ******************************************************************************
  * File Name          : LWIP.h
  * Description        : This file provides code for the configuration
  *                      of the LWIP.
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
  *************************************************************************

  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __mx_lwip_H
#define __mx_lwip_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "ethernetif.h"

/* Includes for RTOS ---------------------------------------------------------*/
#if WITH_RTOS
#include "lwip/tcpip.h"
#endif /* WITH_RTOS */

/* USER CODE BEGIN 0 */

 extern struct netif gnetif;
 extern ip4_addr_t ipaddr;
 extern ip4_addr_t netmask;
 extern ip4_addr_t gw;

 //lwip控制结构�????

 typedef struct
 {
	 uint8_t mac[6];      //MAC地址
	 uint8_t remoteip[4];	//远端主机IP地址
	 uint8_t ip[4];       //本机IP地址
	 uint8_t netmask[4]; 	//子网掩码
	 uint8_t gateway[4]; 	//默认网关的IP地址

	 uint8_t dhcpstatus;	//dhcp状�??
 					//0,未获取DHCP地址;
 					//1,进入DHCP获取状�??
 					//2,成功获取DHCP地址
 					//0XFF,获取失败.
 }__lwip_dev;
 extern __lwip_dev lwipdev;	//lwip控制结构�????

 extern osThreadId LinkThrHandle;
 extern osThreadId tcpip_threadHandle;
 extern osThreadId EthIfHandle;

/* USER CODE END 0 */

/* Global Variables ----------------------------------------------------------*/
extern ETH_HandleTypeDef heth;

/* LWIP init function */
void MX_LWIP_Init(void);

#if !WITH_RTOS
/* USER CODE BEGIN 1 */
/* Function defined in lwip.c to:
 *   - Read a received packet from the Ethernet buffers
 *   - Send it to the lwIP stack for handling
 *   - Handle timeouts if NO_SYS_NO_TIMERS not set
 */
void MX_LWIP_Process(void);

/* USER CODE END 1 */
#endif /* WITH_RTOS */

#ifdef __cplusplus
}
#endif
#endif /*__ mx_lwip_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
