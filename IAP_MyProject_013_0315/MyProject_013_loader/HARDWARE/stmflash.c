/*
 * stmflash.c
 *
 *  Created on: Mar 11, 2024
 *      Author: lozoho
 */


#include "stmflash.h"

#include "sys.h"

//#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//STM32内部FLASH读写 驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

//读取指定地址的字(32位数据)
//faddr:读地址
//返回值:对应数据.
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr;
}

//获取某个地址所在的flash扇区
//addr:flash地址
//返回值:0~11,即addr所在的扇区
u8 STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_SECTOR_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_SECTOR_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_SECTOR_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_SECTOR_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_SECTOR_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_SECTOR_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_SECTOR_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_SECTOR_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_SECTOR_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_SECTOR_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_SECTOR_10;
	return FLASH_SECTOR_11;
}


void STMFLASH_Erase(u32 WriteAddr, u32 NumToWrite)
{
	FLASH_EraseInitTypeDef FlashEraseInit;
	HAL_StatusTypeDef FlashStatus=HAL_OK;
	u32 SectorError=0;
	u32 addrx=0;
	u32 endaddr=0;
	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址

	HAL_FLASH_Unlock();             //解锁
	addrx=WriteAddr;				//写入的起始地址
	endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址

	if(addrx<0X1FFF0000)
	{
		while(addrx<endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
		{
			 if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
			{
				FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除
				FlashEraseInit.Sector=STMFLASH_GetFlashSector(addrx);   //要擦除的扇区
				FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
				FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
				if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK)
				{
					break;//发生错误了
				}
				}else addrx+=4;
				FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成
		}
	}
	FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //等待上次操作完成

	HAL_FLASH_Lock();           //上锁
}


//从指定地址开始写入指定长度的数据
//特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
//         写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
//         写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
//         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写.
//该函数对OTP区域也有效!可以用来写OTP区!
//OTP区域地址范围:0X1FFF7800~0X1FFF7A0F(注意：最后16字节，用于OTP数据块锁定，别乱写！！)
//WriteAddr:起始地址(此地址必须为4的倍数!!)
//pBuffer:数据指针
//NumToWrite:字(32位)数(就是要写入的32位数据的个数.)

void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
{
	FLASH_EraseInitTypeDef FlashEraseInit;
	HAL_StatusTypeDef FlashStatus=HAL_OK;
	u32 SectorError=0;
	u32 addrx=0;
	u32 endaddr=0;
	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址

	HAL_FLASH_Unlock();             //解锁
	addrx=WriteAddr;				//写入的起始地址
	endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址

	FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //等待上次操作完成
	if(FlashStatus==HAL_OK)
	{
		 while(WriteAddr<endaddr)//写数据
		 {
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer)!=HAL_OK)//写入数据
			{
				break;	//写入异常
			}
			WriteAddr+=4;
			pBuffer++;
		}
	}
	HAL_FLASH_Lock();           //上锁
}


//void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
//{
//	FLASH_EraseInitTypeDef FlashEraseInit;
//	HAL_StatusTypeDef FlashStatus=HAL_OK;
//	u32 SectorError=0;
//	u32 addrx=0;
//	u32 endaddr=0;
//	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址
//
//	HAL_FLASH_Unlock();             //解锁
//	addrx=WriteAddr;				//写入的起始地址
//	endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址
//
//	if(addrx<0X1FFF0000)
//	{
//		while(addrx<endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
//		{
//			 if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
//			{
//				FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除
//				FlashEraseInit.Sector=STMFLASH_GetFlashSector(addrx);   //要擦除的扇区
//				FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
//				FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
//				if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK)
//				{
//					break;//发生错误了
//				}
//				}else addrx+=4;
//				FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成
//		}
//	}
//	FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //等待上次操作完成
//	if(FlashStatus==HAL_OK)
//	{
//		 while(WriteAddr<endaddr)//写数据
//		 {
//			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer)!=HAL_OK)//写入数据
//			{
//				break;	//写入异常
//			}
//			WriteAddr+=4;
//			pBuffer++;
//		}
//	}
//	HAL_FLASH_Lock();           //上锁
//}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(32位)数
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
		ReadAddr+=4;//偏移4个字节.
	}
}

//////////////////////////////////////////测试用///////////////////////////////////////////
//WriteAddr:起始地址
//WriteData:要写入的数据
void Test_Write(u32 WriteAddr,u32 WriteData)
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字
}




/* USER CODE BEGIN 4 */

/*FLASH写入程序*/
void WriteFlashTest(uint32_t L,uint32_t Data[],uint32_t addr)
{
	uint32_t i=0;
	/* 1/4解锁FLASH*/
	HAL_FLASH_Unlock();
	/* 2/4擦除FLASH*/
	/*初始化FLASH_EraseInitTypeDef*/
	/*擦除方式页擦除FLASH_TYPEERASE_PAGES，块擦除FLASH_TYPEERASE_MASSERASE*/
	/*擦除页数*/
	/*擦除地址*/
	FLASH_EraseInitTypeDef FlashSet;
//	FlashSet.TypeErase = FLASH_TYPEERASE_PAGES;
//	FlashSet.PageAddress = addr;
//	FlashSet.NbPages = 1;

	FlashSet.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除
	FlashSet.Sector=STMFLASH_GetFlashSector(addr);   //要擦除的扇区
	FlashSet.NbSectors=1;                             //一次只擦除一个扇区

	/*设置PageError，调用擦除函数*/
	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&FlashSet, &PageError);
	/* 3/4对FLASH烧写*/
	for(i=0;i<L;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr+4*i, Data[i]);
	}
	/* 4/4锁住FLASH*/
	HAL_FLASH_Lock();
}


/*FLASH读取打印程序*/
void PrintFlashTest(uint32_t L,uint32_t addr)
{
	uint32_t i=0;
	for(i=0;i<L;i++)
	{
		printf("\naddr is:0x%x, data is:0x%x", addr+i*4,  *(__IO uint32_t*)(addr+i*4));
	}
	printf("\n");

}
/* USER CODE END 4 */

/**
 * https://blog.csdn.net/qq_24312945/article/details/127504619
 *
 * **/









