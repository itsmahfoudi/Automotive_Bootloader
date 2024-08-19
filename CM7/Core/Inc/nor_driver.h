/**
  ******************************************************************************
  * @file    quadspi.h
  * @brief   This file contains all the function prototypes for
  *          the quadspi.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QUADSPI_H__
#define __QUADSPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* USER CODE BEGIN Private defines */

uint8_t nor_driver_Init(void);
uint8_t nor_driver_copy_FW(void);
uint8_t CSP_QSPI_EraseSector(uint32_t EraseStartAddress);
uint8_t CSP_QSPI_WriteMemory(uint8_t* buffer, uint32_t address, uint32_t buffer_size);
uint8_t CSP_QSPI_EnableMemoryMappedMode(void);
uint8_t CSP_QSPI_Erase_Chip (void);
uint8_t CSP_QSPI_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
uint8_t QSPI_Read_ID(void);
uint32_t QSPI_Copy(uint32_t WriteAddr, uint32_t ReadAddr, uint32_t Size);
uint32_t QSPI_Shutdown(void);

/* USER CODE END Private defines */

void MX_QUADSPI_Init(void);

/* USER CODE BEGIN Prototypes */

/*MT25QL512 memory parameters*/
#define MEMORY_FLASH_SIZE               0x4000000 /* 512 MBits*/
#define MEMORY_BLOCK_SIZE               0x10000  /* 64kBytes */
#define MEMORY_SECTOR_SIZE              0x1000   /* 4kBytes */
#define MEMORY_PAGE_SIZE                0x100   /* 256 bytes */


/*MT25QL512 commands */
#define WRITE_ENABLE_CMD 0x06
#define READ_STATUS_REG_CMD 0x05
#define ENTER_4_BYTE_ADD_CMD 0xB7
#define WRITE_VOL_CFG_REG_CMD 0x81
#define SECTOR_ERASE_CMD 0x20
#define CHIP_ERASE_CMD 0xC7
#define QUAD_IN_FAST_PROG_CMD 0x32
#define PAGE_PROG_CMD 0x02
#define READ_CONFIGURATION_REG_CMD 0x85
#define QUAD_OUT_FAST_READ_CMD 0x6B
#define DUMMY_CLOCK_CYCLES_READ_QUAD 10
#define RESET_ENABLE_CMD 0x66
#define RESET_EXECUTE_CMD 0x99
#define QUAD_IN_OUT_FAST_READ_CMD 0xEB
#define DUAL_IN_OUT_FAST_READ_CMD 0x3B
#define READ_ID_CMD 0x9F

/*MT25QL512 timeouts*/
#define QUADSPI_MAX_ERASE_TIMEOUT 460000 /* 460s max */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __QUADSPI_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
