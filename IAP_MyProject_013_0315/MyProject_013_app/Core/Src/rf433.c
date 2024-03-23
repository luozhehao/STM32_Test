/*
 * rf433.c
 *
 *  Created on: Nov 30, 2022
 *      Author: luozhehao
 */

#include "rf433.h"


uint32_t Jump_flag, start_flag, cntint, Low = 0;

uint32_t RF_data, dataJ;

void soft_count()//接受码计数函数
{
    if(RF_Dat==0)      //低电平
    {
        Low++;//电平持续时间累积。
        if(Jump_flag==1)

        {Jump_flag = 0;}//防误触发，需确认为下降沿
    }
    else if(RF_Dat==1)      //高电平
    {
        if(Jump_flag == 0)//防误触发，确认为上升沿，则低电平累积时间结束。
        {
            Jump_flag=1;            //上升沿
            soft_decode();//对Low的计数长度进行处理。
            Low=0;
        }
    }
}

uint32_t press_cnt=0;

void soft_decode()//接受码处理函数
{
    if(start_flag==0)//处于同步状态中
    {
        if(( Low > _start_us_min ) && ( Low < _start_us_max ))            //同步码的低电平时间长度
        {
            start_flag=1;//同步结束，开始接收数据
            cntint=0;//数据长度
            RF_data=0;//数据临时存放地址
        }
    }
    else if((start_flag==1)&&(cntint<24))//接收数据中
    {
        if(( Low > _num0_us_min ) && ( Low < _num0_us_max ))                //数据 0 750~1500us
        {
            RF_data=RF_data<<1;
            cntint++;
        }
        else if(( Low > _num1_us_min ) && ( Low < _num1_us_max ))            //数据 1  0-500us
        {
            RF_data=RF_data<<1;
            RF_data|=1;
            cntint++;
        }
        else
        {
            start_flag=0;//数据错误，清零重新等待同步信号
            cntint=0;
        }
    }

    if(cntint==24)//长度抵达24bit，开始处理
    {
        cntint=0;
        start_flag=0;
        dataJ = RF_data;//记下当前数据
        RF_data = 0;

        printf("soft_decode, dataJ=%x\n\n", dataJ);


  }
}

