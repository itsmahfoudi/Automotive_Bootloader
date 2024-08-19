/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : BootManager_FSM.h
  * @brief          : Header for BootManager_FSM.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#ifndef __BOOTMANAGER_FSM_H
#define __BOOTMANAGER_FSM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Appinfo.h"
#include "memory.h"
#include "main.h"
#include "stm32h7xx_hal.h"

typedef enum _Boot_Manager_State{
  State_Init = 0,
  State_FBLCheck,
  State_ProgRequest_Check,
  State_copy_jump_FW,
  State_APP_Check,
  State_Error
}Boot_Manager_State;

typedef  void (*pFunction)(void);

void BootManager_FSM(void);
uint8_t FBL_Check(void);
uint8_t APP_Check(void);
uint32_t ProgRequest_Check(void);
void Set_Target_FW(uint32_t LoadAddr, uint32_t JumpAddr, uint32_t TargetAddr);
void JumpToFW(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOOTMANAGER_FSM_H */
