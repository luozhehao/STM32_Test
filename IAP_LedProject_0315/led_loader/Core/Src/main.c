/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "iap.h"
#include "delay.h"
#include "led.h"
#include "stmflash.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void Check_update()
{
	  uint32_t cnt = 0;
	  while(cnt<20)
	  {
		  cnt++;
		  HAL_Delay(500);
		  printf("HAL_Delay cnt  = %d\n\n", cnt);

		PrintFlashTest(1024,FLASH_APP1_ADDR);

		if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否�???0X08XXXXXX.
		{
			iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
		}else
		{
	//						printf("非FLASH应用程序,无法执行!\r\n");
	//						LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!");
			printf("--FLASH_APP1_ADDR  : 0x%x\r\n", ((*(vu32*)(FLASH_APP1_ADDR))));
			printf("--FLASH_APP1_ADDR+4: 0x%x\r\n", ((*(vu32*)(FLASH_APP1_ADDR+4))));
			printf("--can not execute app, it is not flash app\r\n");
		}

	  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  printf("MX_USART1_UART_Init finish!\n\n");
  printf("Check_update begin!\n\n");
  Check_update();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  while (1)
//  {
    /* USER CODE END WHILE */

	HAL_Delay(500);
	printf("printf HAL_Delay\n\n");

	printf("this is led loader ... \n\n");
    /* USER CODE BEGIN 3 */
	uint16_t t = 0;
	uint32_t oldcount=0;	//老的串口接收数据�???
	uint32_t applenth=0;	//接收到的app代码长度

	for(;;)
	{
		if(USART_RX_CNT)
		{
//			printf("USART_RX_CNT = %d, oldcount = %d\n\n", USART_RX_CNT, oldcount);
			if(oldcount==USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				applenth = USART_RX_CNT;
				oldcount = 0;
				USART_RX_CNT = 0;
//				printf("用户程序接收完成!\r\n");
//				printf("代码长度:%dBytes\r\n",applenth);
				printf("app receive finish!\r\n");
				printf("app length:%d Bytes\r\n",applenth);

//				printf("USART_RX_BUF:\r\n");
//				for(int i = 0; i < 8; i++)
//					printf("0x%x ", USART_REC_BUF[i]);
//				printf("\n\n");
//				printf("flash data before:\n");
//				PrintFlashTest(4,FLASH_APP1_ADDR);

				// recv finish, and write app data to flash
//				printf("�???始更新固�???...\r\n");
//				LCD_ShowString(30,210,200,16,16,"Copying APP2FLASH...");
				printf("begin update app\r\n");
//				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否�???0X08XXXXXX.
				printf("ram addr data:0x%x\r\n", ((*(vu32*)(0x68000000))) );

				for(int i= 0; i < 4; i++)
				{
					printf("i = 0x%x, ramdata =  0x%x\r\n", i, ((*(vu32*)(0x68000000+4*i))));
				}

				printf("iap_write_appbin begin ...\r\n");
//				if(((*(vu32*)(0x68000000+4))&0xFF000000)==0x08000000)//判断是否�???0X08XXXXXX.
				if(((*(vu32*)(0x10000000+4))&0xFF000000)==0x08000000)//判断是否�???0X08XXXXXX.
				{
					iap_write_appbin(FLASH_APP1_ADDR, USART_REC_BUF, applenth);//更新FLASH代码   applenth
//					uint32_t addr_offset = 0;
//					for(uint32_t i = 0; i< applenth/2048; i++)
//					{
//						addr_offset += i*2048;
//						iap_write_appbin(FLASH_APP1_ADDR+addr_offset, &USART_REC_BUF[addr_offset], 2048);//更新FLASH代码   applenth
//					}
//					addr_offset += 2048;
//					iap_write_appbin(FLASH_APP1_ADDR+addr_offset, &USART_REC_BUF[addr_offset], applenth%2048);//更新FLASH代码   applenth
//					LCD_ShowString(30,210,200,16,16,"Copy APP Successed!!");
//					printf("固件更新完成!\r\n");
					printf("iap_write_appbin finish\r\n");

//					for(int i =0 ;i <8; i++)
//						printf("0x%x ", USART_REC_BUF[i]);
//					printf("\n\n\n");
//					PrintFlashTest(100,FLASH_APP1_ADDR);
//					delay_ms(500);

					for(int i= 0; i < applenth/4; i++)
					{
						printf("ADDR = 0x%x, data =  0x%x\r\n", FLASH_APP1_ADDR+4*i, ((*(vu32*)(FLASH_APP1_ADDR+4*i))));
					}


					//start run app
//					printf("�???始执行FLASH用户代码!!\r\n");
					printf("iap_load_app begin \r\n");


					printf("FLASH_APP1_ADDR  : 0x%x\r\n", ((*(vu32*)(FLASH_APP1_ADDR))));
					printf("FLASH_APP1_ADDR+4: 0x%x\r\n", ((*(vu32*)(FLASH_APP1_ADDR+4))));

					if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否�???0X08XXXXXX.
					{
						iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
					}else
					{
//						printf("非FLASH应用程序,无法执行!\r\n");
//						LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!");
						printf("--FLASH_APP1_ADDR  : 0x%x\r\n", ((*(vu32*)(FLASH_APP1_ADDR))));
						printf("--FLASH_APP1_ADDR+4: 0x%x\r\n", ((*(vu32*)(FLASH_APP1_ADDR+4))));
						printf("--can not execute app, it is not flash app\r\n");
					}

				}else
				{
//					LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!  ");
//					printf("非FLASH应用程序!\r\n");
					printf("can not update app, it is not flash app\r\n");
				}

			}else  oldcount = USART_RX_CNT;
		}

		t++;
//			delay_ms(20);
		HAL_Delay(20);
		if(t==30)
		{
//			printf("LED0 ... \n\n");
			LED0=!LED0;
			t=0;
		}

	}

//  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
