/**
  ******************************************************************************
  * @file    memory_map.h
  * @brief   This file contains definitions for Flash/RAM memory sections
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 Sigma Embedded.
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef INC_MEMORY_MAP_H_
#define INC_MEMORY_MAP_H_


/* Error codes */
#define MEMORY_OK          ((uint32_t)0x00)
#define MEMORY_ERROR       ((uint32_t)0x01)


// Addresses of different sections in flash
#define FBL_START_ADDR        ((uint32_t)0x08020000)
#define APP_START_ADDR        ((uint32_t)0x080A0000)
#define CALIB_START_ADDR      ((uint32_t)0x080E0000)
#define FBL_TARGET_ADDR 	  ((uint32_t)0x00000000)
#define APP_TARGET_ADDR 	  APP_START_ADDR
#define FBL_LOAD_ADDR		  FBL_START_ADDR
#define APP_LOAD_ADDR		  APP_START_ADDR

// Size of different sections in memory
#define FBL_SECTION_SIZE      0x80000
#define APP_SECTION_SIZE      0x40000
#define CALIB_SECTION_SIZE    0x10000
#define BM_SECTION_SIZE	      0x20000

#endif /* INC_MEMORY_MAP_H_ */
