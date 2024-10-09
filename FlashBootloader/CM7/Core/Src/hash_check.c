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
#include "public_key.h"

int32_t SHA256_HASH_Init(void) {
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
		return HASH_ERR_BAD_OPERATION;
	}

	if (computed_size != CMOX_SHA256_SIZE) {
		return HASH_ERR_BAD_OPERATION;
	}
	*MessageDigestLength = computed_size;

	return HASH_SUCCESS;
}



int32_t FW_Signature_Verify(const uint8_t* Msg_Digest ,const uint8_t *signature, size_t signature_len) {

    // Context and key structures
    cmox_rsa_handle_t rsa_ctx;
    cmox_rsa_key_t rsa_key;


    // Buffer for the RSA context
    uint8_t rsa_buffer[BUFFER_SIZE];

    // Initialize the RSA context with appropriate math and modular exponentiation functions
    cmox_rsa_construct(&rsa_ctx, CMOX_MATH_FUNCS_FAST, CMOX_MODEXP_PUBLIC, rsa_buffer, BUFFER_SIZE);

    // Set the RSA key with the provided modulus and public exponent
    cmox_rsa_retval_t retval = cmox_rsa_setKey(&rsa_key, modulus, sizeof(modulus), exponent, sizeof(exponent));
    if (retval != CMOX_RSA_SUCCESS) {
        cmox_rsa_cleanup(&rsa_ctx);
        return retval;  // Handle error if key setting fails
    }


    // Perform signature verification using PKCS#1 v1.5
    uint32_t fault_check = 0;  // Optional fault check parameter
    retval = cmox_rsa_pkcs1v15_verify(
        &rsa_ctx,                   // RSA context
        &rsa_key,                   // Public key structure
        Msg_Digest,                 // Message digest to verify against
        CMOX_RSA_PKCS1V15_HASH_SHA256, 		// Hash ID (assuming SHA-256 was used)
        signature,                  // Signature to verify
        signature_len,              // Length of the signature
        &fault_check);              // Optional fault check (can be NULL if not used)

    // Clean up RSA context after the operation
    cmox_rsa_cleanup(&rsa_ctx);

    // Return the result of the verification
    return retval;
}

