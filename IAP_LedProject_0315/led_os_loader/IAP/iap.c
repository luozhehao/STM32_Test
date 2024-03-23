/*
 * iap.c
 *
 *  Created on: Mar 11, 2024
 *      Author: lozoho
 */


#include "sys.h"
//#include "delay.h"
//#include "usart.h"
#include "stmflash.h"
#include "iap.h"
#include "bsp.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//IAP 代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/24
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

iapfun jump2app;
//u16 iapbuf[1024];
////appxaddr:应用程序的起始地址
////appbuf:应用程序CODE.
////appsize:应用程序大小(字节).
//void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
//{
//	u32 t;
//	u16 i=0;
//	u16 temp;
//	u32 fwaddr=appxaddr;//当前写入的地址
//	u8 *dfu=appbuf;
//	for(t=0;t<appsize;t+=2)
//	{
//		temp=(u16)dfu[1]<<8;
//		temp+=(u16)dfu[0];
//		dfu+=2;//偏移2个字节
//		iapbuf[i++]=temp;
//		if(i==1024)
//		{
//			i=0;
//			STMFLASH_Write(fwaddr,iapbuf,1024);
////			printf("fwaddr = 0x%x\r\n", (*(vu32*)(fwaddr)));
//			PrintFlashTest(1024,fwaddr);
//			fwaddr+=2048;//偏移2048  16=2*8.所以要乘以2.
//		}
////		printf(" t*2 = %d, fwaddr data = 0x%x\r\n", t*2, (*(vu32*)(fwaddr + t*2)));
////		printf("write t = %d\r\n", t);
//	}
//	printf("write i = %d, fwaddr = 0x%x\r\n", i, fwaddr);
//	if(i)
//	{
//		printf("i = %d, is true\r\n\r\n", i );
//		STMFLASH_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.
//	}
//}

u32 iapbuf[512]; 	//2K字节缓存
//appxaddr:应用程序的起始地址
//appbuf:应用程序CODE.
//appsize:应用程序大小(字节).
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
	u32 t;
	u16 i=0;
	u32 temp;
	u32 fwaddr=appxaddr;//当前写入的地址
	u8 *dfu=appbuf;

	STMFLASH_Erase(fwaddr, appsize);

	for(t=0;t<appsize;t+=4)
	{
		temp=(u32)dfu[3]<<24;
		temp|=(u32)dfu[2]<<16;
		temp|=(u32)dfu[1]<<8;
		temp|=(u32)dfu[0];
		dfu+=4;//偏移4个字节
		iapbuf[i++]=temp;
		if(i==512)
		{
			i=0;
			STMFLASH_Write(fwaddr,iapbuf,512);
			PrintFlashTest(512,fwaddr);
			fwaddr+=2048;//偏移2048  512*4=2048
		}
		printf("write t = %d\r\n", t);
	}
	printf("i = %d, fwaddr = 0x%x\r\n", i, fwaddr);
	if(i)
		STMFLASH_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.
}


//u32 iapbuf[1024];
//void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
//{
//	u32 t;
//	u32 i=0;
//	u32 temp;
//	u32 fwaddr=appxaddr;//当前写入的地址
//	u8 *dfu=appbuf;
//	for(t=0;t<appsize;t+=4)
//	{
//		temp=(u32)dfu[3]<<24;
//		temp=(u32)dfu[2]<<16;
//		temp=(u32)dfu[1]<<8;
//		temp+=(u32)dfu[0];
//		dfu+=4;//偏移2个字节
//		iapbuf[i++]=temp;
//		if(i==1024)
//		{
//			i=0;
//			STMFLASH_Write(fwaddr,iapbuf,1024);
//			fwaddr+=4096;//偏移2048  16=2*8.所以要乘以2.
//		}
//		printf("write t = %d\r\n", t);
//	}
//	if(i)STMFLASH_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.
//}

//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(u32 appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20020000)	//检查栈顶地址是否合法.    0x20000000
	{
		printf("appxaddr == 0x20020000, jump2app ...\r\n");
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)
		printf("MSR_MSP begin ... %s\r\n", jump2app);
		//MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		__set_MSP(*(__IO uint32_t*)appxaddr);
		printf("__set_MSP finish!\r\n");
		jump2app();									//跳转到APP.
		printf("jump2app finish!\r\n");
	}
	else
	{
		printf("appxaddr addr: 0x%8x\r\n", appxaddr );
		printf("appxaddr data: 0x%8x\r\n", ((*(vu32*)appxaddr)) );  // ((*(vu32*)appxaddr)&0x2FFE0000)
		printf("appxaddr &dat: 0x%8x\r\n", ((*(vu32*)appxaddr)&0x2FFE0000) );
	}
}


void BSPJumpToReset(u32 appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20020000)	//检查栈顶地址是否合法.    0x20000000
	{
//		printf("appxaddr == 0x20020000, jump2app ...\r\n");
//		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)
//		printf("MSR_MSP begin ... %s\r\n", jump2app);
//		//MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
//		__set_MSP(*(__IO uint32_t*)appxaddr);
//		printf("__set_MSP finish!\r\n");

		BSPCpuReset();
		//jump2app();									//跳转到APP.
		printf("jump2app finish!\r\n");
	}
	else
	{
		printf("appxaddr addr: 0x%8x\r\n", appxaddr );
		printf("appxaddr data: 0x%8x\r\n", ((*(vu32*)appxaddr)) );  // ((*(vu32*)appxaddr)&0x2FFE0000)
		printf("appxaddr &dat: 0x%8x\r\n", ((*(vu32*)appxaddr)&0x2FFE0000) );
	}
}













