/*
 * MAC_HASH_RET_VAL.h
 *
 *  Created on: Aug 27, 2024
 *      Author: ayoub
 */

#ifndef INC_CRYPT_RET_VAL_H_
#define INC_CRYPT_RET_VAL_H_

/**
  * @brief MAC module return value type
  */
typedef uint32_t cmox_mac_retval_t;

/**
  * @brief CMAC module return value type
  */
typedef int32_t cmox_cmac_retval_t;



#define AES_SUCCESS                        (int32_t) (0)             /*!< AES Success */
#define AES_ERR_BAD_INPUT_SIZE             (int32_t) (3101)          /*!< AES Invalid input size */
#define AES_ERR_BAD_OPERATION              (int32_t) (3102)          /*!< AES Invalid operation */
#define AES_ERR_BAD_CONTEXT                (int32_t) (3103)          /*!< AES context has invalid/uninitialized vals */
#define AES_ERR_BAD_PARAMETER              (int32_t) (3104)          /*!< AES function called with invalid params */

#define AUTHENTICATION_SUCCESSFUL          (int32_t) (1003)          /*!< Authentication successful */
#define AUTHENTICATION_FAILED              (int32_t) (1004)          /*!< Authentication failed */
#define SIGNATURE_VALID                     AUTHENTICATION_SUCCESSFUL /*!< Signature is valid */
#define SIGNATURE_INVALID                   AUTHENTICATION_FAILED     /*!< Signature is NOT valid */
#define ERR_MEMORY_FAIL                    (int32_t) (1005)          /*!< There's no more available memory */

#define HASH_SUCCESS                       (int32_t) (0)             /*!< Hash Success */
#define HASH_ERR_BAD_OPERATION             (int32_t) (4001)          /*!< Hash Invalid operation */
#define HASH_ERR_BAD_CONTEXT               (int32_t) (4002)          /*!< Hash ctx has invalid/uninitialized vals */
#define HASH_ERR_BAD_PARAMETER             (int32_t) (4003)          /*!< Hash function called with invalid params */
#define HASH_ERR_UNSUPPORTED               (int32_t) (4004)          /*!< Hash algorithm is not supported */
#define HASH_ERR_INTERNAL                  (int32_t) (4011)          /*!< Hash Generic internal error */

#define RSA_SUCCESS                        (int32_t) (0)             /*!< RSA Success */
#define RSA_ERR_BAD_OPERATION              (int32_t) (5102)          /*!< RSA Invalid operation */
#define RSA_ERR_BAD_KEY                    (int32_t) (5103)          /*!< RSA Invalid Key */
#define RSA_ERR_BAD_PARAMETER              (int32_t) (5104)          /*!< RSA function called with invalid params */
#define RSA_ERR_UNSUPPORTED_HASH           (int32_t) (5105)          /*!< RSA hash function is not supported */
#define RSA_ERR_MESSAGE_TOO_LONG           (int32_t) (5106)          /*!< RSA Message too long */
#define RSA_ERR_MODULUS_TOO_SHORT          (int32_t) (5107)          /*!< RSA modulus too short */
#define RSA_ERR_GENERIC                    (int32_t) (5108)          /*!< RSA Generic Error */

#define CMOX_MAC_SUCCESS                   ((cmox_mac_retval_t)0x00030000U)
#define CMOX_MAC_ERR_INTERNAL              ((cmox_mac_retval_t)0x00030001U)
#define CMOX_MAC_ERR_BAD_PARAMETER         ((cmox_mac_retval_t)0x00030002U)
#define CMOX_MAC_ERR_BAD_OPERATION         ((cmox_mac_retval_t)0x00030003U)
#define CMOX_MAC_AUTH_SUCCESS              ((cmox_mac_retval_t)0x0003C726U)
#define CMOX_MAC_AUTH_FAIL                 ((cmox_mac_retval_t)0x00036E93U)


/* Public types --------------------------------------------------------------*/



#endif /* INC_CRYPT_RET_VAL_H_ */
