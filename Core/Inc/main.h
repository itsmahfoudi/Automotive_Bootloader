/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_crc.h>
#include <stm32f4xx_hal_def.h>
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_uart.h>
#include <string.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define FLASH_SECTOR2_BASE_ADDRESS 0x08008000U
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
/*
 * Some Start and end addresses of different memories inside the
 * STM32F446xx
 */
#define SRAM1_SIZE			112*1024 //112 KB
#define SRAM1_END			(SRAM1_BASE + SRAM1_SIZE)
#define SRAM2_SIZE			16*1024 // 16KB
#define SRAM2_END			(SRAM2_BASE + SRAM2_SIZE)
#define FLASH_SIZE			512*1024 // 512KB
#define BACKUP_SRAM_BASE	BKPSRAM_BASE
#define BACKUP_SRAM_SIZE	4*1024
#define BACKUP_SRAM_END		(BACKUP_SRAM_BASE + BACKUP_SRAM_SIZE)


/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
