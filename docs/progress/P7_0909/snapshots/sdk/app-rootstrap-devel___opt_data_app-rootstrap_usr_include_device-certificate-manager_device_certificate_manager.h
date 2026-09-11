/******************************************************************
 *
 * Copyright 2017 - 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef __TIZEN_SECURITY_DEVICE_CERTIFICATE_MANAGER_H__
#define __TIZEN_SECURITY_DEVICE_CERTIFICATE_MANAGER_H__

#include <stddef.h>
#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_DEVICE_CERTIFICATE_MANAGER_MODULE
 * @{
 */


/**
 * @brief Enumeration for DCM error values.
 * @since_tizen 5.0
 */
typedef enum {
    DCM_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
    DCM_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid function parameter */
    DCM_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
    DCM_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
    DCM_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Feature needed to run API is not supported */
    DCM_ERROR_NO_DATA = TIZEN_ERROR_NO_DATA, /**< No data available */
    DCM_ERROR_UNKNOWN = TIZEN_ERROR_UNKNOWN, /**< Unknown error */

    DCM_ERROR_SOCKET = TIZEN_ERROR_DEVICE_CERTIFICATE_MANAGER | 0x01, /**< Socket error between client and server */
} dcm_error_e;


/**
 * @brief Enumeration for DCM message digest algorithms.
 * @since_tizen 5.0
 */
typedef enum {
    DCM_DIGEST_NONE=0, /**< No message digest algorithm */
    DCM_DIGEST_MD2, /**< Message digest algorithm MD2 */
    DCM_DIGEST_MD4, /**< Message digest algorithm MD4 */
    DCM_DIGEST_MD5, /**< Message digest algorithm MD5 */
    DCM_DIGEST_SHA1, /**< Message digest algorithm SHA1 */
    DCM_DIGEST_SHA224, /**< Message digest algorithm SHA224 */
    DCM_DIGEST_SHA256, /**< Message digest algorithm SHA256 */
    DCM_DIGEST_SHA384, /**< Message digest algorithm SHA384 */
    DCM_DIGEST_SHA512, /**< Message digest algorithm SHA512 */
    DCM_DIGEST_RIPEMD160, /**< Message digest algorithm RIPEMD160 */
} dcm_digest_algorithm_e;


/**
 * @platform
 * @brief Creates a new key context based on specific name indication (service name, key usage, key type).
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/devicecertificate
 *
 * @remarks The @a key_ctx should be freed with dcm_free_key_context() after use.
 *
 * @param[in] service  Service name indicates first category name (if null, default value is used)
 * @param[in] usage  Usage name indicates sub-category name (if null, default value is used)
 * @param[in] key_type  Key type name indication (if null, default value is used)
 * @param[out] key_ctx  Newly created key context
 * @return #DCM_ERROR_NONE on success,
 *         otherwise a negative error value
 *
 * @retval #DCM_ERROR_NONE Successful
 * @retval #DCM_ERROR_NOT_SUPPORTED Feature needed to run API is not supported
 * @retval #DCM_ERROR_PERMISSION_DENIED Failed to access device certificate manager
 * @retval #DCM_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #DCM_ERROR_SOCKET Socket error between client and server
 * @retval #DCM_ERROR_OUT_OF_MEMORY Out of memory during processing
 * @retval #DCM_ERROR_UNKNOWN Unknown error
 *
 * @see dcm_free_key_context()
 */
int dcm_create_key_context(const char *service, const char *usage, const char *key_type, void **key_ctx);


/**
 * @platform
 * @brief Destroys the key context that was created by calling dcm_create_key_context().
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/devicecertificate
 *
 * @param[in] key_ctx  Key context object to be deallocated
 * @return #DCM_ERROR_NONE on success,
 *         otherwise a negative error value
 *
 * @retval #DCM_ERROR_NONE Successful
 * @retval #DCM_ERROR_NOT_SUPPORTED Feature needed to run API is not supported
 * @retval #DCM_ERROR_PERMISSION_DENIED Failed to access device certificate manager
 * @retval #DCM_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #DCM_ERROR_SOCKET Socket error between client and server
 * @retval #DCM_ERROR_OUT_OF_MEMORY Out of memory during processing
 * @retval #DCM_ERROR_NO_DATA No such key context object
 * @retval #DCM_ERROR_UNKNOWN Unknown error
 *
 * @see dcm_create_key_context()
 */
int dcm_free_key_context(void *key_ctx);


/**
 * @platform
 * @brief Returns a certificate chain which was pre-injected in device.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/devicecertificate
 *
 * @remarks The @a cert_chain should be freed using free().
 *
 * @param[in] key_ctx  Key context object that identifies proper certificate chain
 * @param[out] cert_chain  Certificate chain in binary, will be allocated by the library
 * @param[out] cert_chain_len  The total length of certificate chain
 * @return #DCM_ERROR_NONE on success,
 *         otherwise a negative error value
 *
 * @retval #DCM_ERROR_NONE Successful
 * @retval #DCM_ERROR_NOT_SUPPORTED Feature needed to run API is not supported
 * @retval #DCM_ERROR_PERMISSION_DENIED Failed to access device certificate manager
 * @retval #DCM_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #DCM_ERROR_SOCKET Socket error between client and server
 * @retval #DCM_ERROR_OUT_OF_MEMORY Out of memory during processing
 * @retval #DCM_ERROR_NO_DATA No certificate chain available
 * @retval #DCM_ERROR_UNKNOWN Unknown error
 */
int dcm_get_certificate_chain(const void *key_ctx, char **cert_chain, size_t *cert_chain_len);


/**
 * @platform
 * @brief Returns the key size in bits for a given key context.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/devicecertificate
 *
 * @param[in] key_ctx  Key context object that identifies proper certificate chain
 * @param[out] key_bit_len  Key length in bits
 * @return #DCM_ERROR_NONE on success,
 *         otherwise a negative error value
 *
 * @retval #DCM_ERROR_NONE Successful
 * @retval #DCM_ERROR_NOT_SUPPORTED Feature needed to run API is not supported
 * @retval #DCM_ERROR_PERMISSION_DENIED Failed to access device certificate manager
 * @retval #DCM_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #DCM_ERROR_SOCKET Socket error between client and server
 * @retval #DCM_ERROR_OUT_OF_MEMORY Out of memory during processing
 * @retval #DCM_ERROR_NO_DATA No certificate chain available
 * @retval #DCM_ERROR_UNKNOWN Unknown error
 */
int dcm_get_key_bit_length(const void *key_ctx, size_t *key_bit_len);


/**
 * @platform
 * @brief Returns the key type name for a given key context.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/devicecertificate
 *
 * @remarks The @a key_type should be freed using free().
 *
 * @param[in] key_ctx  Key context object that identifies proper certificate chain
 * @param[out] key_type  Key type name (UNKNOWN, RSA or ECDSA), will be allocated by the library
 * @return #DCM_ERROR_NONE on success,
 *         otherwise a negative error value
 *
 * @retval #DCM_ERROR_NONE Successful
 * @retval #DCM_ERROR_NOT_SUPPORTED Feature needed to run API is not supported
 * @retval #DCM_ERROR_PERMISSION_DENIED Failed to access device certificate manager
 * @retval #DCM_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #DCM_ERROR_SOCKET Socket error between client and server
 * @retval #DCM_ERROR_OUT_OF_MEMORY Out of memory during processing
 * @retval #DCM_ERROR_NO_DATA No certificate chain available
 * @retval #DCM_ERROR_UNKNOWN Unknown error
 */
int dcm_get_key_type(const void *key_ctx, char **key_type);

/**
 * @platform
 * @brief Creates a signature on a given data using a private key and returns the signature.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/devicecertificate
 *
 * @remarks The private key is identified by @a key_ctx.
 * @remarks The @a message can be NULL but then @a message_len must be 0.
 * @remarks The @a signature should be freed using free().
 *
 * @param[in] key_ctx  Key context object that identifies a proper private key for signing
 * @param[in] md  Message digest algorithm used in creating signature
 * @param[in] message  Message that is signed with a key
 * @param[in] message_len  Length of the message
 * @param[out] signature  Newly created signature, will be allocated by the library
 * @param[out] signature_len  Length of a newly created signature
 * @return #DCM_ERROR_NONE on success,
 *         otherwise a negative error value
 *
 * @retval #DCM_ERROR_NONE Successful
 * @retval #DCM_ERROR_NOT_SUPPORTED Feature needed to run API is not supported
 * @retval #DCM_ERROR_PERMISSION_DENIED Failed to access device certificate manager
 * @retval #DCM_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #DCM_ERROR_SOCKET Socket error between client and server
 * @retval #DCM_ERROR_OUT_OF_MEMORY Out of memory during processing
 * @retval #DCM_ERROR_UNKNOWN Unknown error
 */
int dcm_create_signature(const void *key_ctx, dcm_digest_algorithm_e md,
                         const char *message, size_t message_len,
                         char **signature, size_t *signature_len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SECURITY_DEVICE_CERTIFICATE_MANAGER_H__ */
