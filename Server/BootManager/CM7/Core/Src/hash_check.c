/**
  ******************************************************************************
  * @file    hash_check.c
  * @brief   source file of firmware hash check module.
  *          This file contains implementation for set of firmware functions to
  *          manage Common functionalities.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 Sigma Embedded.
  * All rights reserved.
  *
  ******************************************************************************
  */
#include "hash_check.h"


int32_t SHA256_HASH_Init() {
	  int32_t retval = HASH_SUCCESS;

	  if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS) {
		  retval = HASH_ERR_BAD_OPERATION;
	  }
	  return retval;
}
int32_t SHA256_HASH_DigestCompute(uint8_t* InputMessage, uint32_t InputMessageLength,
                                        uint8_t *MessageDigest, int32_t* MessageDigestLength) {
	int32_t retval;
	size_t computed_size;
	retval = cmox_hash_compute(CMOX_SHA256_ALGO, InputMessage, InputMessageLength, MessageDigest, CMOX_SHA256_SIZE, &computed_size);
	if (retval != CMOX_HASH_SUCCESS) {
		return RSA_ERR_BAD_OPERATION;
	}

	if (computed_size != CMOX_SHA256_SIZE) {
		return RSA_ERR_BAD_OPERATION;
	}
	*MessageDigestLength = computed_size;

	return HASH_SUCCESS;
}

