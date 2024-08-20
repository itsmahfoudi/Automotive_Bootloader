/**
  ******************************************************************************
  * @file           : Appinfo.h
  * @brief          : Header for Appinfo.
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

#ifndef __APPINFO_H
#define __APPINFO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

#define SIGNATURE(a,b,c)   ((c<<16)|(b<<8)|(a))
#define SIG_APP   SIGNATURE('A','P','P')   
#define SIG_PBL   SIGNATURE('F','B','L')   
#define SIG_BTM   SIGNATURE('B','T','M')
#define TYPE_BTM  0 
#define TYPE_APP  1  
#define TYPE_FBL  2 
#define ProgFlag  0xAA55AA55

typedef struct _ApplicationInfo{
    uint32_t      Signature;     
    uint8_t       type;          
    uint8_t       version;       
    char          name[32];      
    uint8_t       hwModel; 
    uint32_t      StartAdd;
    uint32_t      Size;      
}ApplicationInfo;



#ifdef __cplusplus
}
#endif

#endif /* __APPINFO_H */
