/**
  ******************************************************************************
  * @file    cmac_check.c
  * @brief   source file of firmware cmac check module.
  *          This file contains implementation for set of firmware functions to
  *          manage Common functionalities
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 Sigma Embedded.
  * All rights reserved.
  *
  ******************************************************************************/
#include "cmac_check.h"
uint8_t crypt_key[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

/* Initialization for AES-CMAC for Authentication TAG Generation*/
cmox_cmac_retval_t AES_CMAC_Init(void) {
	  int32_t retval;

	  if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS) {
		  retval = AES_ERR_BAD_CONTEXT;
	  }
	  return retval;
}


/* AES-CMAC Authentication TAG generation */
cmox_cmac_retval_t AES_CMAC_Encrypt(const uint8_t *P_pInputBuffer,  int32_t P_inputSize, uint8_t *P_pOutputBuffer, int32_t *P_pOutputSize) {

	  int32_t retval;
	  size_t computed_size;
	  retval = cmox_mac_compute(CMOX_CMAC_AES_ALGO, P_pInputBuffer, P_inputSize, crypt_key, CRL_AES128_KEY, NULL, 0, P_pOutputBuffer, MAC_SIZE, &computed_size);
	  /* Verify API returned value */
	  if (retval != CMOX_MAC_SUCCESS)
	  {
		  retval = AES_ERR_BAD_OPERATION;
	  }

	  /* Verify generated data size is the expected one */
	  if (computed_size != MAC_SIZE)
	  {
		  retval = AES_ERR_BAD_OPERATION;
	  }
	  *P_pOutputSize = computed_size;
	  return retval;
}
