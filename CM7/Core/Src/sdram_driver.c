/**
 * @brief                                             MT48LC4M32B2
 * <br><I>                                   Copyright Serious Integrated, Inc.
 * </I>
 * @version                                       $$$SHIP_VERSION$$$
 *
 *//**
 * @copyright $$$SHIPLICENSE_WARE.c
 * <CENTER><STRONG>              Copyright (c) Serious Integrated, Inc. All rights reserved.
 * <br>                This file is the confidential and proprietary property of Serious Integrated, Inc.
 * <br>                                Possession or use of this file requires
 * <br>                             a written license from Serious Integrated, Inc.
 * </STRONG></CENTER>$$$
 *
 * @file
 */

#include "memory.h"

#if 1

static MT48LC4M32B2_Context_t  sdram_context   = {0};
static FMC_SDRAM_TimingTypeDef sdram_timing    = {0};
static SDRAM_HandleTypeDef     sdram_handle    = {0};

void MX_FMC_Init(void)
{

  /** Perform the SDRAM1 memory initialization sequence
  */
  sdram_handle.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  sdram_handle.Init.SDBank = FMC_SDRAM_BANK2;
  sdram_handle.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
  sdram_handle.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  sdram_handle.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  sdram_handle.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  sdram_handle.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_2;
  sdram_handle.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  sdram_handle.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_3;
  sdram_handle.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  sdram_handle.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;

  /* SdramTiming */
  sdram_timing.LoadToActiveDelay = 2;
  sdram_timing.ExitSelfRefreshDelay = 7;
  sdram_timing.SelfRefreshTime = 4;
  sdram_timing.RowCycleDelay = 6;
  sdram_timing.WriteRecoveryTime = 3;
  sdram_timing.RPDelay = 2;
  sdram_timing.RCDDelay = 2;

  if (HAL_SDRAM_Init(&sdram_handle, &sdram_timing) != HAL_OK)
  {
    Error_Handler( );
  }

}

/****************** MT48LC4M32B2 FONCIONS ******************/

/** @defgroup MT48LC4M32B2_Private_Variables MT48LC4M32B2 Private Variables
  * @{
  */
static FMC_SDRAM_CommandTypeDef Command;
/**
  * @}
  */ 

/** @defgroup MT48LC4M32B2_Function_Prototypes MT48LC4M32B2 Function Prototypes
  * @{
  */
static int32_t MT48LC4M32B2_Delay(uint32_t Delay);

/**
  * @}
  */ 

void sdram_driver_Init(void){

	// Init the Pins and Clock for FMC
	MX_FMC_Init();

	// Init the SDRAM Driver
	sdram_context.TargetBank                = FMC_SDRAM_CMD_TARGET_BANK2;
  sdram_context.RefreshMode               = SDRAM_AUTOREFRESH_MODE_CMD;
  sdram_context.RefreshRate               = 770;
  sdram_context.BurstLength               = SDRAM_BURST_LENGTH_1;
  sdram_context.BurstType                 = SDRAM_BURST_TYPE_SEQUENTIAL;
  sdram_context.CASLatency                = SDRAM_CAS_LATENCY_2;
  sdram_context.OperationMode             = SDRAM_OPERATING_MODE_STANDARD;
  sdram_context.WriteBurstMode            = SDRAM_WRITEBURST_MODE_SINGLE;

  MT48LC4M32B2_Init(&sdram_handle, &sdram_context);

}

/** @defgroup MT48LC4M32B2_Exported_Functions MT48LC4M32B2 Exported Functions
  * @{
  */
/**
  * @brief  Initializes the MT48LC4M32B2 SDRAM memory
  * @param  Ctx : Component object pointer
  * @param  pRegMode : Pointer to Register Mode stucture
  * @retval error status
  */
int32_t MT48LC4M32B2_Init(SDRAM_HandleTypeDef *Ctx, MT48LC4M32B2_Context_t *pRegMode) 
{
  int32_t ret = MT48LC4M32B2_ERROR;
  
  /* Step 1: Configure a clock configuration enable command */
  if(MT48LC4M32B2_ClockEnable(Ctx, pRegMode->TargetBank) == MT48LC4M32B2_OK)
  {
    /* Step 2: Insert 100 us minimum delay */ 
    /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
    (void)MT48LC4M32B2_Delay(1);
    
    /* Step 3: Configure a PALL (precharge all) command */ 
    if(MT48LC4M32B2_Precharge(Ctx, pRegMode->TargetBank) == MT48LC4M32B2_OK)
    {
      /* Step 4: Configure a Refresh command */ 
      if(MT48LC4M32B2_RefreshMode(Ctx, pRegMode->TargetBank, pRegMode->RefreshMode) == MT48LC4M32B2_OK)
      {
        /* Step 5: Program the external memory mode register */
        if(MT48LC4M32B2_ModeRegConfig(Ctx, pRegMode) == MT48LC4M32B2_OK)
        {
          /* Step 6: Set the refresh rate counter */
          if(MT48LC4M32B2_RefreshRate(Ctx, pRegMode->RefreshRate) == MT48LC4M32B2_OK)
          {
            ret = MT48LC4M32B2_OK;
          }
        }
      }
    }
  } 
  return ret;
}

/**
  * @brief  Enable SDRAM clock
  * @param  Ctx : Component object pointer
  * @param  Interface : Could be FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2
  * @retval error status
  */
int32_t MT48LC4M32B2_ClockEnable(SDRAM_HandleTypeDef *Ctx, uint32_t Interface) 
{
  Command.CommandMode            = SDRAM_CLK_ENABLE_CMD;
  Command.CommandTarget          = Interface;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  if(HAL_SDRAM_SendCommand(Ctx, &Command, MT48LC4M32B2_TIMEOUT) != HAL_OK)
  {
    return MT48LC4M32B2_ERROR;
  }
  else
  {
    return MT48LC4M32B2_OK;
  }
}

/**
  * @brief  Precharge all sdram banks
  * @param  Ctx : Component object pointer
  * @param  Interface : Could be FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2
  * @retval error status
  */
int32_t MT48LC4M32B2_Precharge(SDRAM_HandleTypeDef *Ctx, uint32_t Interface) 
{
  Command.CommandMode            = SDRAM_PALL_CMD;
  Command.CommandTarget          = Interface;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  if(HAL_SDRAM_SendCommand(Ctx, &Command, MT48LC4M32B2_TIMEOUT) != HAL_OK)
  {
    return MT48LC4M32B2_ERROR;
  }
  else
  {
    return MT48LC4M32B2_OK;
  }
}

/**
  * @brief  Program the external memory mode register
  * @param  Ctx : Component object pointer
  * @param  pRegMode : Pointer to Register Mode stucture
  * @retval error status
  */
int32_t MT48LC4M32B2_ModeRegConfig(SDRAM_HandleTypeDef *Ctx, MT48LC4M32B2_Context_t *pRegMode) 
{
  uint32_t tmpmrd;

  /* Program the external memory mode register */
  tmpmrd = (uint32_t)pRegMode->BurstLength   |\
                     pRegMode->BurstType     |\
                     pRegMode->CASLatency    |\
                     pRegMode->OperationMode |\
                     pRegMode->WriteBurstMode;
  
  Command.CommandMode            = SDRAM_LOAD_MODE_CMD;
  Command.CommandTarget          = pRegMode->TargetBank;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = tmpmrd;
  
  /* Send the command */
  if(HAL_SDRAM_SendCommand(Ctx, &Command, MT48LC4M32B2_TIMEOUT) != HAL_OK)
  {
    return MT48LC4M32B2_ERROR;
  }
  else
  {
    return MT48LC4M32B2_OK;
  }
}

/**
  * @brief  Program the SDRAM timing
  * @param  Ctx : Component object pointer
  * @param  pTiming : Pointer to SDRAM timing configuration stucture
  * @retval error status
  */
int32_t MT48LC4M32B2_TimingConfig(SDRAM_HandleTypeDef *Ctx, FMC_SDRAM_TimingTypeDef *pTiming) 
{
  /* Program the SDRAM timing */
  if(HAL_SDRAM_Init(Ctx, pTiming) != HAL_OK)
  {
    return MT48LC4M32B2_ERROR;
  }
  else
  {
    return MT48LC4M32B2_OK;
  }
}

/**
  * @brief  Configure Refresh mode
  * @param  Ctx : Component object pointer
  * @param  Interface : Could be FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2 
  * @param  RefreshMode : Could be MT48LC4M32B2_CMD_AUTOREFRESH_MODE or
  *                      MT48LC4M32B2_CMD_SELFREFRESH_MODE
  * @retval error status
  */
int32_t MT48LC4M32B2_RefreshMode(SDRAM_HandleTypeDef *Ctx, uint32_t Interface, uint32_t RefreshMode) 
{
  Command.CommandMode            = RefreshMode;
  Command.CommandTarget          = Interface;
  Command.AutoRefreshNumber      = 8;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  if(HAL_SDRAM_SendCommand(Ctx, &Command, MT48LC4M32B2_TIMEOUT) != HAL_OK)
  {
    return MT48LC4M32B2_ERROR;
  }
  else
  {
    return MT48LC4M32B2_OK;
  }
}

/**
  * @brief  Set the device refresh rate
  * @param  Ctx : Component object pointer
  * @param  RefreshCount : The refresh rate to be programmed
  * @retval error status
  */
int32_t MT48LC4M32B2_RefreshRate(SDRAM_HandleTypeDef *Ctx, uint32_t RefreshCount) 
{
  /* Set the device refresh rate */
  if(HAL_SDRAM_ProgramRefreshRate(Ctx, RefreshCount) != HAL_OK)
  {
    return MT48LC4M32B2_ERROR;
  }
  else
  {
    return MT48LC4M32B2_OK;
  }
}

/**
  * @brief  Enter Power mode
  * @param  Ctx : Component object pointer
  * @param  Interface : Could be FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2
  * @retval error status
  */
int32_t MT48LC4M32B2_EnterPowerMode(SDRAM_HandleTypeDef *Ctx, uint32_t Interface) 
{
  Command.CommandMode            = SDRAM_POWERDOWN_MODE_CMD;
  Command.CommandTarget          = Interface;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  if(HAL_SDRAM_SendCommand(Ctx, &Command, MT48LC4M32B2_TIMEOUT) != HAL_OK)
  {
    return MT48LC4M32B2_ERROR;
  }
  else
  {
    return MT48LC4M32B2_OK;
  }
}

/**
  * @brief  Exit Power mode
  * @param  Ctx : Component object pointer
  * @param  Interface : Could be FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2
  * @retval error status
  */
int32_t MT48LC4M32B2_ExitPowerMode(SDRAM_HandleTypeDef *Ctx, uint32_t Interface) 
{
  Command.CommandMode            = SDRAM_NORMAL_MODE_CMD;
  Command.CommandTarget          = Interface;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  if(HAL_SDRAM_SendCommand(Ctx, &Command, MT48LC4M32B2_TIMEOUT) != HAL_OK)
  {
    return MT48LC4M32B2_ERROR;
  }
  else
  {
    return MT48LC4M32B2_OK;
  }
}

/**
  * @brief  Sends command to the SDRAM bank.
  * @param  Ctx : Component object pointer
  * @param  SdramCmd : Pointer to SDRAM command structure 
  * @retval SDRAM status
  */  
int32_t MT48LC4M32B2_Sendcmd(SDRAM_HandleTypeDef *Ctx, FMC_SDRAM_CommandTypeDef *SdramCmd)
{
  if(HAL_SDRAM_SendCommand(Ctx, SdramCmd, MT48LC4M32B2_TIMEOUT) != HAL_OK)
  {
    return MT48LC4M32B2_ERROR;
  }
  else
  {
    return MT48LC4M32B2_OK;
  }
}

/**
  * @}
  */ 

/** @defgroup MT48LC4M32B2_Private_Functions MT48LC4M32B2 Private Functions
  * @{
  */ 

/**
  * @brief This function provides accurate delay (in milliseconds)
  * @param Delay: specifies the delay time length, in milliseconds
  * @retval MT48LC4M32B2_OK
  */
static int32_t MT48LC4M32B2_Delay(uint32_t Delay)
{  
  uint32_t tickstart;
  tickstart = HAL_GetTick();
  while((HAL_GetTick() - tickstart) < Delay)
  {
  }
  return MT48LC4M32B2_OK;
}


#endif // SHIP_DRAM_MT48LC4M32B2_ENABLE

