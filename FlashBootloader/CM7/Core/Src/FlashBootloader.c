/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : FlashBootloader.c
  * @brief          : Source for FlashBootloader file.
  *                   This file contains implementations of  declared functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 Sigma Embedded.
  * All rights reserved.
  *
  ******************************************************************************/

#include "FlashBootloader.h"
#include <string.h>
#include "main.h"
#include "cmac_check.h"
#include "hash_check.h"

#define FM_LEN	APP_SECTION_SIZE + SIGNATURE_LEN

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
FLASH_EraseInitTypeDef EraseInitStruct;
const size_t fw_len = APP_SECTION_SIZE + SIGNATURE_LEN;
uint8_t firmware_buffer[FM_LEN] __attribute__((section(".firmware_buffer"))) = {0x0};
uint8_t Signature[SIGNATURE_LEN] = {0x0};
size_t Signature_Len = SIGNATURE_LEN;
uint8_t Msg_Digest[HASH_SIZE] = {0x0};
int32_t Msg_Digest_Len = 0;

void FlashBootloader_Main(void) {
	// Initialize nedeed peripherals
	if (FBL_Periph_Init() != HAL_OK) {
		BSP_LED_Toggle(LED_RED);
		printmsg("Peripheral initialization has failed!!\n");
		Error_Handler();
	}
	BSP_LED_Toggle(LED_GREEN);

	// Erase the app section
	if (App_Sector_Erase() != HAL_OK) {
		BSP_LED_Toggle(LED_RED);
		printmsg("Application erase has failed!!\n");
		Error_Handler();
	}

	BSP_LED_Toggle(LED_GREEN);

	// Receive the new firmware application and store it a RAM buffer
	if (Receive_FW() != HAL_OK) {
		BSP_LED_Toggle(LED_RED);
		printmsg("Firmware reception has failed!!\n");
		Error_Handler();
	}

	BSP_LED_Toggle(LED_GREEN);

	// Extract The signature from the firmware buffer.
	if (Extract_Sig() != HAL_OK) {
		BSP_LED_Toggle(LED_RED);
		printmsg("Signature extraction has failed!!\n");
		Error_Handler();
	}

	BSP_LED_Toggle(LED_GREEN);

	// Copy the Firmware from the RAM buffer to the FLASH
	if (Copy_FW_To_Flash() != HAL_OK) {
		BSP_LED_Toggle(LED_RED);
		printmsg("Firmware copy to FLASH has failed!!\n");
		Error_Handler();
	}

	BSP_LED_Toggle(LED_GREEN);

	// Calculate and verify the authenticity for the received firnware
	if (FW_Auth_Verify() != HAL_OK) {
		BSP_LED_Toggle(LED_RED);
		printmsg("Firmware authentication has failed !!");
		Error_Handler();
	}

	BSP_LED_Toggle(LED_GREEN);

	// Calculate the MAC and store it in the last 32Bytes of the app
	if (Calculate_And_Store_MAC() != HAL_OK) {
		BSP_LED_Toggle(LED_RED);
		printmsg("Mac Storing has failed.");
		Error_Handler();
	}

	BSP_LED_Toggle(LED_GREEN);

	// Sw reset to enter BootManager
	__NVIC_SystemReset();

}
HAL_StatusTypeDef FBL_Periph_Init(void) {

	  /* MCU Configuration--------------------------------------------------------*/
	  HAL_StatusTypeDef retVal;
	  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	  if ( HAL_Init() != HAL_OK) {
		  retVal = HAL_ERROR;
		  goto error;
	  }

	  /* Configure the system clock */
	  SystemClock_Config();

	  /* USER CODE BEGIN SysInit */

	  /* USER CODE END SysInit */

	  /* Initialize all configured peripherals */
	  MX_GPIO_Init();
#ifdef USE_CAN_BUS
	  MX_FDCAN1_Init();
	  MX_FDCAN2_Init();
#endif
	  MX_CRC_Init();
#ifdef USE_UART
	  MX_USART3_UART_Init();
	  MX_USART2_UART_Init();
#endif
	  /* Initialize leds */
	  /* Initialize leds */
	  if (BSP_LED_Init(LED_GREEN) != HAL_OK) {
		  retVal = HAL_ERROR;
		  goto error;
	  }
	  if (BSP_LED_Init(LED_RED) != HAL_OK) {
		  retVal = HAL_ERROR;
		  goto error;
	  }

	  //Initilize the cmox library
	  if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS) {
		  retVal = HAL_ERROR;
		  goto error;
	  }

	  retVal = HAL_OK;

error :
	 return retVal;
}

HAL_StatusTypeDef App_Sector_Erase(void) {
	uint32_t Erase_Result;
	if (HAL_FLASH_Unlock() != HAL_OK) {
		printmsg("Unable to unlock the flash.!!\n");
		return HAL_ERROR;
	}

	// Initialize the EraseInitTypeDef Structure
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = FLASH_SECTOR_5;
	EraseInitStruct.NbSectors = 2;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &Erase_Result) != HAL_OK) {
		printmsg("Unable to erase the app sectors!!!\n");
		return HAL_ERROR;
	}

	return HAL_FLASH_Lock();
}

HAL_StatusTypeDef Extract_Sig(void) {
	if (memcpy((void*)Signature, (void*)&firmware_buffer[APP_SECTION_SIZE],SIGNATURE_LEN) != 0) {
		return HAL_ERROR;
	}

	return HAL_OK;
}

HAL_StatusTypeDef Receive_FW(void) {
	uint32_t counter = fw_len;
	while (counter > 0) {
		if (HAL_UART_Receive(&huart3, &firmware_buffer[fw_len - counter], 1, HAL_MAX_DELAY) != HAL_OK) {
			return HAL_ERROR;
		}
		counter--;
	}
	return HAL_OK;
}

HAL_StatusTypeDef Copy_FW_To_Flash(void) {
	uint32_t counter = 0;
	uint32_t max_value = APP_SECTION_SIZE - 1;
	if (HAL_FLASH_Unlock() != HAL_OK) {
		printmsg("Unable to unlock the flash.!!\n");
		return HAL_ERROR;
	}

	while (counter < max_value) {
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, (uint32_t)(APP_START_ADDR + counter), (uint32_t)&firmware_buffer[counter]) != HAL_OK) {
			printmsg("write to flash has failed!!\n");
			return HAL_ERROR;
		}
		counter += 32;
	}
	return HAL_FLASH_Lock();;
}

HAL_StatusTypeDef FW_Auth_Verify(void) {
    // Calculate the digest:
    int32_t retVal = SHA256_HASH_DigestCompute((uint8_t*)APP_START_ADDR, APP_SECTION_SIZE, Msg_Digest, &Msg_Digest_Len);
    if (retVal != HASH_SUCCESS) {
    	return HAL_ERROR;
    }

    int32_t RetVal = FW_Signature_Verify(Msg_Digest, Signature, Signature_Len);
    if (RetVal != CMOX_RSA_AUTH_SUCCESS) {
    	return HAL_ERROR;
    }
    return HAL_OK;
}


HAL_StatusTypeDef Calculate_And_Store_MAC(void) {
	uint8_t mac_result[MAC_SIZE*2] = {0x0};
	int32_t inputSize = 0x40000 - 0x20;
	int32_t outputSize = 0;
	uint8_t *buffer = (uint8_t*)(APP_START_ADDR);
	uint32_t address = APP_START_ADDR + inputSize;

	// Calculate the mac
	AES_CMAC_Encrypt(buffer, inputSize, mac_result, &outputSize);
	if (HAL_FLASH_Unlock() != HAL_OK) {
		return HAL_ERROR;
	}

	// write the MAC to the FLASH
	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, address, (uint32_t)&mac_result) != HAL_OK ) {
		return HAL_ERROR;
	}

	return HAL_FLASH_Lock();
}
