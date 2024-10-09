/**
  ******************************************************************************
  * @file    hashcheck.h
  * @brief   Header file of firmware hash check module.
  *          This file provides set of firmware functions to manage Common
  *          functionalities.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 Sigma Embedded.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef __HASHCHECK_H
#define __HASHCHECK_H
#include "main.h"
#include "cmox_crypto.h"
#include "cmox_sha256.h"
#include "CRYPT_RET_VAL.h"

#define Hach_Check 0



#define HASH_SIZE (32)

int32_t SHA256_HASH_Init(void);
int32_t SHA256_HASH_DigestCompute(uint8_t* InputMessage, uint32_t InputMessageLength,
                                        uint8_t *MessageDigest, int32_t* MessageDigestLength);
void Fatal_Error_Handler(void);

#endif /* __HASHCHECK_H */
