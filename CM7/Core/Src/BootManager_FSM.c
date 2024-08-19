/**
  ******************************************************************************
  * @file           : BootManager_FSM.c
  * @brief          : Boot Manager FSM
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

#include "BootManager_FSM.h"
//#include "hashcheck.h"

Boot_Manager_State  State = State_Init;
pFunction JumpToApplication;

void BootManager_FSM(void){

  switch(State) {
    case State_Init : 
      /* MPU Configuration--------------------------------------------------------*/
      MPU_Config();
      /* Enable the CPU Cache */
      /* Enable I-Cache---------------------------------------------------------*/
      SCB_EnableICache();
      /* Enable D-Cache---------------------------------------------------------*/
      SCB_EnableDCache();
      /* MCU Configuration--------------------------------------------------------*/
      /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
      HAL_Init();
      /* Configure the system clock */
      SystemClock_Config();
      /* Configure the peripherals common clocks */
      PeriphCommonClock_Config();
      /* Initialize all configured peripherals */
      MX_GPIO_Init();
      /*QOS parameter configuration for the AXI interface, make the LTDC high priority*/
      QOS_config();

      if(Memory_Startup()){
        State = State_Error;
      }else{
        State = State_FBLCheck;
      }
#if 0
  	__disable_irq();
  	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;;
    __DSB();
    __DMB();
    __ISB();
    while(1);
#endif
      break;
    case State_FBLCheck : 
      if(FBL_Check()){
        State = State_ProgRequest_Check;
      }else{
        State = State_Error;
      }
      break;
    case State_ProgRequest_Check :
      if(ProgRequest_Check()){
        State = State_copy_jump_FW; // Copy FBL and Jump to it
      }else{
        State = State_APP_Check;
      }
      break;
    case State_copy_jump_FW :
      if(nor_driver_copy_FW()){
        State = State_Error;
      }else{
        JumpToFW();
      }
      break;
    case State_APP_Check :
      State = State_copy_jump_FW; // Copy APP and Jump to it
      if(APP_Check()){
        Set_Target_FW(APP_LOAD_ADDRESS, APP_ENTRY_ADDRESS, APP_TARGET_ADDRESS);
      }else{
        Set_Target_FW(FBL_LOAD_ADDRESS, FBL_ENTRY_ADDRESS, FBL_TARGET_ADDRESS);
      }
      break;
    case State_Error :
      while(1);
      break;
  }
}

void JumpToFW(void){

	/* Disable Systick interrupt */
	__disable_irq();
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;;

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
  uint32_t   File_Sig;
  uint32_t   FBL_Size;

  ApplicationInfo * Appheader = (ApplicationInfo *) FBL_LOAD_ADDRESS;
  File_Sig = Appheader->Signature;

  /* 1 - Check Signature */
  if(File_Sig != SIG_PBL){
    return 0;
  }
  
  /* 2 - Check Hash */
#if Hash_Check  
  FBL_Size = Appheader->Size;

  FW_Hash_Verify(FBL_LOAD_ADDRESS, FBL_Size);  
#endif

  return 1;
}

uint8_t APP_Check(void){

  uint32_t File_Sig;
  uint32_t App_Size;

  ApplicationInfo * Appheader = (ApplicationInfo *) APP_LOAD_ADDRESS;
  File_Sig = Appheader->Signature;

  /* 1 - Check Signature */
  if(File_Sig != SIG_APP){
    return 0;
  }

  /* 2 - Check Hash */
#if Hash_Check   
  App_Size = Appheader->Size;

  FW_Hash_Verify(APP_LOAD_ADDRESS, App_Size);
#endif

  return 1;
}

void Set_Target_FW(uint32_t LoadAddr, uint32_t JumpAddr, uint32_t TargetAddr){

  LoadAddress   = LoadAddr;
  JumpAddress   = JumpAddr;
  TargetAddress = TargetAddr;
}

uint32_t ProgRequest_Check(void){

  /* Choose the Entry Point */
  uint8_t IsFlag_Set = 0;
  //ProgrammingMode = ProgFlag;
  if (ProgrammingMode == ProgFlag){
    Set_Target_FW(FBL_LOAD_ADDRESS, FBL_ENTRY_ADDRESS, FBL_TARGET_ADDRESS);
    IsFlag_Set = (ProgrammingMode == ProgFlag);
    ProgrammingMode = 0;
  }

  return IsFlag_Set;
}
