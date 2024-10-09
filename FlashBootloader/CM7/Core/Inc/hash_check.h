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
#include "CRYPT_RET_VAL.h"

#define Hach_Check 		0
#define HASH_SIZE 		32
#define SIGNATURE_LEN	0x100
// Define the buffer size according to the requirements
#define BUFFER_SIZE 	1024


int32_t SHA256_HASH_Init(void);
int32_t SHA256_HASH_DigestCompute(uint8_t* InputMessage, uint32_t InputMessageLength,
                                        uint8_t *MessageDigest, int32_t* MessageDigestLength);
/*
* @param[in]   Msg_Digest	   pointer to the Hash Value
* @param[in]   P_pSignature    Signature
* @param[in]   P_SignatureLen  Signature Length (in Bytes)
* @retval      CMOX_RSA_AUTH_SUCCESS              Signature verified
* @retval      CMOX_RSA_AUTH_FAIL                 Signature NOT verified
* @retval      CMOX_RSA_ERR_MATH_ALGO_MISMATCH    Mathematical function set is incompatible with current functionality
* @retval      CMOX_RSA_ERR_MEXP_ALGO_MISMATCH    Modexp function set is not compatible with current functionality
* @retval      CMOX_RSA_ERR_BAD_PARAMETER         Some NULL/wrong/empty parameter
* @retval      CMOX_RSA_ERR_INVALID_SIGNATURE     Input signature corrupted or not in the expected format
* @retval      CMOX_RSA_ERR_MEMORY_FAIL           Not enough memory
* @retval      CMOX_RSA_ERR_MODULUS_TOO_SHORT     Modulus too short for the message to verify
* */
int32_t FW_Signature_Verify(const uint8_t* Msg_Digest, const uint8_t *signature, size_t signature_len);

void Fatal_Error_Handler(void);

#endif /* __HASHCHECK_H */
