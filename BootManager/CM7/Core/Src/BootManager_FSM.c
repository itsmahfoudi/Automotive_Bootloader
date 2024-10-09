/**
  ******************************************************************************
  * @file           : BootManager_FSM.c
  * @brief          : Boot Manager FSM
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 Sigma Embedded.
  * All rights reserved.
  *
  ******************************************************************************/

#include "BootManager_FSM.h"
#include <string.h>
#include "main.h"
#include "cmac_check.h"


Boot_Manager_State  State = State_Init;
pFunction JumpToApplication;
uint32_t ProgrammingMode;
uint32_t JumpAddress;
uint32_t LoadAddress;
uint32_t TargetAddress;
uint32_t LoadSize;
uint16_t VirtAddVarTab[NB_OF_VAR]  = {0x1111, 0x2222};
uint16_t data[2] = {0x0};

// Private Function declaration
void Initialize(void);
void APP_Check_Fails(void);
void FBL_Check_Fails(void);
void CAL_Check_Fails(void);
HAL_StatusTypeDef calcalute_store_MAC(uint8_t* buffer, size_t size);

// BootManager_FSM implementation
void BootManager_FSM(void){

  switch(State) {

    	case State_Init :
    		Initialize();
    		State = State_FBLCheck;

    	case State_FBLCheck :
    		if(FBL_Check()){
    			State = State_ProgRequest_Check;
    		}else{
    			FBL_Check_Fails();
    		}
    		break;

    	case State_ProgRequest_Check :
    		if(ProgRequest_Check()){
    			// Call FBL To update the APP
    			JumpToFW();
    		}else{
    			State = State_CALIB_Check;
    		}
    		break;

    	case State_APP_Check :

    		if(APP_Check()){
    			Set_Target_FW(APP_START_ADDR, APP_START_ADDR, APP_START_ADDR);
    			JumpToFW();
    		}else{
    			//Indicate integrity check fails
    			APP_Check_Fails();
    			Set_Target_FW(FBL_START_ADDR, FBL_START_ADDR, FBL_START_ADDR);
    			JumpToFW();
    		}
    		break;


    	case State_CALIB_Check:
    		if (CAL_Check()) {
    			State = State_APP_Check;
    		} else {
    			//Indicate integrity check fails
    			CAL_Check_Fails();
    			Set_Target_FW(APP_START_ADDR, APP_START_ADDR, APP_START_ADDR);
    			JumpToFW();
    		}
    		break;

  }
}

void Initialize(void) {

	  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	  HAL_Init();

	  SystemClock_Config();
	  /* Initialize all configured peripherals */
	  MX_GPIO_Init();
	  MX_USART3_UART_Init();
	  MX_CRC_Init();

	  printmsg("The BootManager has started");

	  BSP_LED_Init(LED_GREEN);
	  BSP_LED_Init(LED_RED);

	  // Initialize eeprom_flash
	  if (EE_Init() != HAL_OK) {
		  printmsg("Unable to initialize eeprom-flash\n");
		  Error_Handler();
	  }

	  //Initialize cmac routine
	  AES_CMAC_Init();
}

void JumpToFW(void){

	/* Disable Systick interrupt */
	__disable_irq();
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;

	/* Initialize user application's Stack Pointer & Jump to user application */
	JumpToApplication = (pFunction) (*(__IO uint32_t*) (JumpAddress + 4));

	__set_MSP(*(__IO uint32_t*) JumpAddress);
	__DSB();
	__DMB();
	__ISB();
	//while(1);
	JumpToApplication();
}

uint8_t FBL_Check(void){

	uint8_t mac_result[MAC_SIZE*2] = {0x0};
	uint8_t expected_mac[MAC_SIZE] = {0x0};
	int32_t inputSize = 0x80000 - 0x00020;
	int32_t outputSize = 0;
	uint8_t *buffer = (uint8_t*)(FBL_START_ADDR);

	// Load expected MAC from the last 16 bytes of the FBL section
	memcpy(expected_mac, (uint8_t*)(buffer + inputSize), MAC_SIZE);

	// Calculate MAC over the FBL section
	AES_CMAC_Encrypt((uint8_t*)buffer, inputSize, mac_result, &outputSize);

	//memcpy(expected_mac, mac_result,MAC_SIZE);
	//memcpy(expected_mac, (uint8_t*)(buffer + inputSize), MAC_SIZE);

	// Compare the calculated MAC with the expected MAC
	if (memcmp(mac_result, expected_mac, MAC_SIZE) == 0) {
		return 1;
	}
	return 0;
}

uint8_t APP_Check(void){

	uint8_t mac_result[MAC_SIZE] = {0x0};
	uint8_t expected_mac[MAC_SIZE] = {0x0};
	int32_t inputSize = 0x40000 - 0x00010;
	int32_t outputSize = 0;
	uint8_t* buffer = (uint8_t*)(APP_START_ADDR);

	// Load expected MAC from the last 16 bytes of the APP section
	memcpy(expected_mac, (uint8_t*)(buffer + inputSize), MAC_SIZE);

	// Calculate MAC over the APP section
	AES_CMAC_Encrypt(buffer, inputSize, mac_result, &outputSize);

	// Compare the calculated MAC with the expected MAC
	if (memcmp(mac_result, expected_mac, MAC_SIZE) == 0) {
		return 1;
	}

	return 0;
}

uint8_t CAL_Check(void) {
	uint8_t mac_result[MAC_SIZE] = {0x0};
	uint8_t expected_mac[MAC_SIZE] = {0x0};
	int32_t inputSize = 0x10000 - 0x00020;
	int32_t outputSize = 0;
	uint8_t* buffer = (uint8_t*)(CALIB_START_ADDR);

	// Load expected MAC from the last 16 bytes of the CALIB section
	memcpy(expected_mac, (uint8_t*)(buffer + inputSize), MAC_SIZE);

	// Calculate MAC over the CALIB section
	AES_CMAC_Encrypt((uint8_t*)buffer, inputSize, mac_result, &outputSize);
  
	// Compare the calculated MAC with the expected MAC
	if (memcmp(mac_result, expected_mac, MAC_SIZE) == 0) {
		return 1;
	}
	return 0;
} 

/* For this moment this function is not needed */
void Set_Target_FW(uint32_t LoadAddr, uint32_t JumpAddr, uint32_t TargetAddr){

	LoadAddress   = LoadAddr;
	JumpAddress   = JumpAddr;
	TargetAddress = TargetAddr;
}


uint32_t ProgRequest_Check(void){

  /* Choose the Entry Point */
  uint8_t IsFlag_Set = FALSE;
  if (EE_ReadVariable(VirtAddVarTab[0], data) != EE_OK) {
	  printmsg("Unable to read variable from eeprom!!\n");
	  return IsFlag_Set;
  }
  ProgrammingMode = (data[1] << 16) | data[0];
  if (ProgrammingMode == ProgFlag){
    Set_Target_FW(FBL_START_ADDR, FBL_TARGET_ADDR, FBL_TARGET_ADDR);
    IsFlag_Set = TRUE;
    ProgrammingMode = 0;
  }

  return IsFlag_Set;
}


void APP_Check_Fails(void) {
	uint8_t counter = 0xF;
	while(counter-- >= 0) {
		printmsg("Application Integrity check has failed!!\n");
	}

}

void FBL_Check_Fails(void) {

	uint8_t counter = 0xF;
	while(counter-- >= 0) {
		printmsg("BootUpdater Integrity check has failed!!\n");
	}

}
void CAL_Check_Fails(void) {
	uint8_t counter = 0xF;
	while(counter-- >= 0) {
		printmsg("Calibration Integrity check has failed!!\n");
	}
}
