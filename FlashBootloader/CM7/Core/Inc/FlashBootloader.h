/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : FlashBootloader.h
  * @brief          : Header for BootManager.h file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 Sigma Embedded.
  * All rights reserved.
  *
  ******************************************************************************/

/* USER CODE END Header */

#ifndef INC_FLASHBOOTLOADER_H_
#define INC_FLASHBOOTLOADER_H_

#include "main.h"

#define USE_CAN_BUS		0
#define USE_UART		1

/*Functions declaration*/
void FlashBootloader_Main(void);
HAL_StatusTypeDef FBL_Periph_Init(void);
HAL_StatusTypeDef App_Sector_Erase(void);
HAL_StatusTypeDef Receive_FW(void);
HAL_StatusTypeDef Extract_Sig(void);
HAL_StatusTypeDef Copy_FW_To_Flash(void) __attribute__((section(".itcm_code")));
HAL_StatusTypeDef FW_Auth_Verify(void);
HAL_StatusTypeDef Calculate_And_Store_MAC(void);
HAL_StatusTypeDef Com_Transmit(uint8_t* tx_buffer, size_t tx_len);
HAL_StatusTypeDef Com_Receive(uint8_t* rx_buffer, size_t rx_len);

#endif /* INC_FLASHBOOTLOADER_H_ */
