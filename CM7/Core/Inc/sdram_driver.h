/**
 * @brief                                           MT48LC4M32B2
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

#ifndef _sdram_driver_H
#define _sdram_driver_H

#include "main.h"

#ifdef __cplusplus
    extern "C" {
#endif

/****************** MT48LC4M32B2 Dependencies ******************/


#define MT48LC4M32B2_TIMEOUT             ((uint32_t)0xFFFF)

/** @defgroup MT48LC4M32B2_Exported_Types MT48LC4M32B2 Exported Types
  * @{
  */
typedef struct 
{
  uint32_t TargetBank;           /*!< Target Bank                             */
  uint32_t RefreshMode;          /*!< Refresh Mode                            */
  uint32_t RefreshRate;          /*!< Refresh Rate                            */
  uint32_t BurstLength;          /*!< Burst Length                            */
  uint32_t BurstType;            /*!< Burst Type                              */
  uint32_t CASLatency;           /*!< CAS Latency                             */
  uint32_t OperationMode;        /*!< Operation Mode                          */
  uint32_t WriteBurstMode;       /*!< Write Burst Mode                        */
} MT48LC4M32B2_Context_t;

/**
  * @}
  */

/** @defgroup MT48LC4M32B2_Exported_Constants MT48LC4M32B2 Exported Constants
  * @{
  */
#define MT48LC4M32B2_OK                (0)
#define MT48LC4M32B2_ERROR             (-1)

/* Register Mode */
#define SDRAM_BURST_LENGTH_1              0x00000000U
#define SDRAM_BURST_LENGTH_2              0x00000001U
#define SDRAM_BURST_LENGTH_4              0x00000002U
#define SDRAM_BURST_LENGTH_8              0x00000004U
#define SDRAM_BURST_TYPE_SEQUENTIAL       0x00000000U
#define SDRAM_BURST_TYPE_INTERLEAVED      0x00000008U
#define SDRAM_CAS_LATENCY_2               0x00000020U
#define SDRAM_CAS_LATENCY_3               0x00000030U
#define SDRAM_OPERATING_MODE_STANDARD     0x00000000U
#define SDRAM_WRITEBURST_MODE_PROGRAMMED  0x00000000U 
#define SDRAM_WRITEBURST_MODE_SINGLE      0x00000200U 

/* Command Mode */
#define SDRAM_NORMAL_MODE_CMD             0x00000000U
#define SDRAM_CLK_ENABLE_CMD              0x00000001U
#define SDRAM_PALL_CMD                    0x00000002U
#define SDRAM_AUTOREFRESH_MODE_CMD        0x00000003U
#define SDRAM_LOAD_MODE_CMD               0x00000004U
#define SDRAM_SELFREFRESH_MODE_CMD        0x00000005U
#define SDRAM_POWERDOWN_MODE_CMD          0x00000006U

/**
  * @}
  */ 

/** @addtogroup MT48LC4M32B2_Exported_Functions
  * @{
  */
int32_t MT48LC4M32B2_Init(SDRAM_HandleTypeDef *Ctx, MT48LC4M32B2_Context_t *pRegMode);
int32_t MT48LC4M32B2_ClockEnable(SDRAM_HandleTypeDef *Ctx, uint32_t Interface);
int32_t MT48LC4M32B2_Precharge(SDRAM_HandleTypeDef *Ctx, uint32_t Interface);
int32_t MT48LC4M32B2_ModeRegConfig(SDRAM_HandleTypeDef *Ctx, MT48LC4M32B2_Context_t *pRegMode);
int32_t MT48LC4M32B2_TimingConfig(SDRAM_HandleTypeDef *Ctx, FMC_SDRAM_TimingTypeDef *pTiming);
int32_t MT48LC4M32B2_RefreshMode(SDRAM_HandleTypeDef *Ctx, uint32_t Interface, uint32_t RefreshMode);
int32_t MT48LC4M32B2_RefreshRate(SDRAM_HandleTypeDef *Ctx, uint32_t RefreshCount);
int32_t MT48LC4M32B2_EnterPowerMode(SDRAM_HandleTypeDef *Ctx, uint32_t Interface);
int32_t MT48LC4M32B2_ExitPowerMode(SDRAM_HandleTypeDef *Ctx, uint32_t Interface);
int32_t MT48LC4M32B2_Sendcmd(SDRAM_HandleTypeDef *Ctx, FMC_SDRAM_CommandTypeDef *SdramCmd);
void sdram_driver_Init(void);
#ifdef __cplusplus
}
#endif

#endif // re-include protect
