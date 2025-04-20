/**
  ******************************************************************************
  * @file    cmac_check.h
  * @brief   Header file of firmware cmac check module.
  *          This file provides set of firmware functions to manage Common
  *          functionalities.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 Sigma Embedded.
  * All rights reserved.
  *
  ******************************************************************************/

#ifndef INC_CMAC_CHECK_H_
#define INC_CMAC_CHECK_H_
#include "main.h"
#include "cmox_crypto.h"
#include "cmox_mac.h"
#include "CRYPT_RET_VAL.h"


#define cmac_Check			  				0
#define MAC_SIZE              				16
#define CRL_AES128_KEY   	  				16 						  /*!< Number of bytes (uint8_t) necessary to store an AES key of 128 bits. */
#define CRL_AES_BLOCK         				16 						  /*!< Number of bytes (uint8_t) necessary to store an AES block. */


typedef uint32_t SKflags_et;    									  /*!< Type definitation for Symmetric Key Flags */

#define E_SK_DEFAULT 						(SKflags_et) (0x00000000) /*!< User Flag: No flag specified. This is the default value that should be set to this flag  */
#define E_SK_DONT_PERFORM_KEY_SCHEDULE 		(SKflags_et) (0x00000001) /*!< User Flag: Used to force the init to not reperform key schedule.\n
                                                                      	   The classic example is where the same key is used on a new message, in this case to redo key scheduling is
                                                                           a useless waste of computation, could be particularly useful on GCM, where key schedule is very complicated. */
#define E_SK_FINAL_APPEND 					(SKflags_et)(0x00000020)  /*!< User Flag: Must be set in CMAC mode before the final Append call occurs. */
#define E_SK_OPERATION_COMPLETED 			(SKflags_et)(0x00000002)  /*!< Internal Flag (not to be set/read by user): used to check that the Finish function has been already called */
#define E_SK_NO_MORE_APPEND_ALLOWED 		(SKflags_et)(0x00000004)  /*!< Internal Flag (not to be set/read by user): it is set when the last append has been called. Used where the append is called with an InputSize not
                                                                           multiple of the block size, which means that is the last input.*/
#define E_SK_NO_MORE_HEADER_APPEND_ALLOWED  (SKflags_et)(0x00000010)  /*!< Internal Flag (not to be set/read by user): only for authenticated encryption modes. \n
                                                                            It is set when the last header append has been called. Used where the header append is called with an InputSize not
                                                                            multiple of the block size, which means that is the last input.*/
#define E_SK_APPEND_DONE 					(SKflags_et)(0x00000040)  /*!< Internal Flag (not to be set/read by user): only for CMAC.It is set when the first append has been called */
#define E_SK_SET_COUNTER 					(SKflags_et)(0x00000080)  /*!< User Flag: With ChaCha20 this is used to indicate a value for the counter, used to process non contiguous blocks (i.e. jump ahead)*/


/**
  * @brief Initialization for AES-CMAC for Authentication TAG Generation */

cmox_cmac_retval_t AES_CMAC_Init(void);

/**
  * @brief AES-CMAC Authentication TAG generation
  * @param[in,out]    *P_pAESCMACctx  AES CMAC context
  * @param[in]    *P_pInputBuffer   Input buffer
  * @param[in]     P_inputSize      Size of input data in uint8_t (octets)
  * @param[out]   *P_pOutputBuffer  Output buffer
  * @param[out]   *P_pOutputSize     Size of written output data in uint8_t
  * @retval    AES_SUCCESS Operation Successful
  * @retval    AES_ERR_BAD_PARAMETER At least one of the parameters is a NULL pointer
  * @retval    AES_ERR_BAD_CONTEXT   Context was not initialized with valid values, see the note below.
 */
cmox_cmac_retval_t AES_CMAC_Encrypt(const uint8_t *P_pInputBuffer,  int32_t P_inputSize,
								uint8_t *P_pOutputBuffer, int32_t *P_pOutputSize);
#endif /* INC_CMAC_CHECK_H_ */
