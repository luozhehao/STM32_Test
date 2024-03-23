/*
 * delay.h
 *
 *  Created on: 2024年3月12日
 *      Author: luozhehao
 */

#ifndef DELAY_DELAY_H_
#define DELAY_DELAY_H_


#include "sys.h"

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);


#endif /* DELAY_DELAY_H_ */
