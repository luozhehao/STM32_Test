/*
 * bsp.c
 *
 *  Created on: Mar 16, 2024
 *      Author: lozoho
 */

//#include "stm32f4xx_hal_cortex.h"
#include "main.h"

#include "bsp.h"

void BSPCpuReset(void)
{
//	__disable_irq(); // Disable Interrupts
    NVIC_SystemReset();
	//SCB->AIRCR = 0x5fa0004;
}
