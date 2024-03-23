/*
 * rf433.h
 *
 *  Created on: Nov 30, 2022
 *      Author: luozhehao
 */

#include "main.h"

#ifndef INC_RF433_H_
#define INC_RF433_H_


//#define RF_Dat            ((GPIOB->IDR & GPIO_IDR_8)>>8)


//#define RF_Dat    HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_14)
#define RF_Dat    HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)

#define _start_us_min    180//同步信号的低电平持续时间，最低值9ms
#define _start_us_max    280//同步信号的低电平持续时间，最高值14ms
#define _num0_us_min    15//数据0，低电平信号持续时间，最低值750us
#define _num0_us_max    30//数据0，低电平信号持续时间，最高值1500us
#define _num1_us_min    0//数据1，低电平信号持续时间，最低值0us
#define _num1_us_max    10//数据1，低电平信号持续时间，最高值500us



void soft_count();   //接受码计数函数
void soft_decode();  //接受码处理函数

#endif /* INC_RF433_H_ */
