/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License
 *
 */


#ifndef __TIZEN_CORE_CKMC_EXTENDED_H__
#define __TIZEN_CORE_CKMC_EXTENDED_H__


#include <stddef.h>
#include <stdint.h>
#include <ckmc/ckmc-type.h>
#include <ckmc/ckmc-error.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_KEY_MANAGER_CLIENT_EXTENDED_MODULE
 * @{
 */


/**
 * @platform
 * @brief Wraps concatenated key and data (key|data) with wrapping key and returns it to the client.
 *
 * @since_tizen 7.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/keymanager.extended
 *
 * @remarks The wrapping key must be public RSA (#CKMC_KEY_RSA_PUBLIC).
 * @remarks The key denoted by @a alias can only be #CKMC_KEY_AES.
 * @remarks The key and the wrapping key must be stored in the same backend.
 * @remarks The data size must be smaller or equal to:
 *          wrapping key size in bytes - key size in bytes -
 *          2 * hash function output size in bytes - 2.
 *          Example: for 3072 RSA wrapping key, 256 AES key and hash SHA384 the maximum
 *          data size is: 3072/8 - 256/8 - 2*384/8 - 2 = 254 bytes.
 * @remarks Considering the data size limit it's recommended to use RSA key longer than
 *          @c 1024 bits.
 * @remarks The @a ppwrapped_key should be released using ckmc_key_free().
 *
 * @param[in] params Algorithm parameter list handle. See #ckmc_param_list_h and #ckmc_algo_type_e
 *                   for details. Supported algorithms:
 *                   - #CKMC_ALGO_RSA_OAEP
 * @param[in] wrapping_key_alias The name of the wrapping key
 * @param[in] wrapping_key_password An optional password of the wrapping key
 * @param[in] alias The name of the key to be concatenated, wrapped and exported
 * @param[in] password An optional password used to decrypt the key pointed by @a alias
 * @param[in] data Data to be concatenated, wrapped and exported
 * @param[out] ppwrapped_key The wrapped key.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Insufficient permissions to access key manager, the
 *                                       wrapping key or the key being wrapped
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (missing or invalid mandatory
 *                                       algorithm parameter or data too long,
 *                                       @a wrapping_key_alias = NULL,
 *                                       @a alias = NULL, @a data = NULL,
 *                                       @a ppwrapped_key = NULL)
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN @a wrapping_key_alias or @a alias does not exist
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Wrapping key decryption failed because
 *                                           @a wrapping_key_password is incorrect
 * @retval #CKMC_ERROR_SERVER_ERROR Unknown error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @code
 * ckmc_param_list_h params;    // Initialized elsewhere
 * ckmc_raw_buffer_s *data;     // Initialized elsewhere
 * ckmc_key_s *ppwrapped_key;
 * int ret = ckmc_wrap_concatenated_data(params,
 *                                       "wrapping_key_alias",
 *                                       "wrapping_key_password",
 *                                       "alias",
 *                                       "password",
 *                                       data,
 *                                       &ppwrapped_key);
 *  ...
 * ckmc_key_free(ppwrapped_key);
 * @endcode
 *
 * @see ckmc_unwrap_concatenated_data()
 * @see #ckmc_param_list_h
 * @see #ckmc_raw_buffer_s
 * @see #ckmc_key_s
 */
int ckmc_wrap_concatenated_data(const ckmc_param_list_h params,
                                const char *wrapping_key_alias,
                                const char *wrapping_key_password,
                                const char *alias,
                                const char *password,
                                const ckmc_raw_buffer_s *data,
                                ckmc_key_s **ppwrapped_key);


/**
 * @platform
 * @brief Unwraps concatenated key and data (key|data) with wrapping key.
 *        Splits to key (stored inside key manager) and data (returned to the client).
 *
 * @since_tizen 7.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/keymanager.extended
 *
 * @remarks The wrapping key must be private RSA (#CKMC_KEY_RSA_PRIVATE).
 * @remarks key_type in @a wrapped_key can only be #CKMC_KEY_AES.
 * @remarks password in @a wrapped_key must be set to NULL. There's no need to additionally encrypt
 *          a wrapped key.
 * @remarks The key denoted by @a alias can only be #CKMC_KEY_AES.
 * @remarks If password in @a policy is provided, the stored key is additionally encrypted with it.
 * @remarks If extractable in @a policy is set to false, the stored key may still be exported in a
 *          wrapped form.
 * @remarks The supported @a size for the key to be stored is @c 128, @c 192 and @c 256 bits.
 * @remarks The @a ppdata should be released using ckmc_buffer_free().
 *
 * @param[in] params Algorithm parameter list handle. See #ckmc_param_list_h and #ckmc_algo_type_e
 *                   for details. Supported algorithms:
 *                   - #CKMC_ALGO_RSA_OAEP
 * @param[in] wrapping_key_alias The name of the wrapping key
 * @param[in] wrapping_key_password An optional password of the wrapping key
 * @param[in] wrapped_key The wrapped key to be unwrapped, split and stored
 * @param[in] alias The name of a key to be stored
 * @param[in] size The size in bits of the key to be stored
 * @param[in] policy The policy about how to store a key securely
 * @param[out] ppdata The unwrapped data.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Insufficient permissions to access key manager, the
 *                                       wrapping key or to create the unwrapped key
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (missing or invalid mandatory
 *                                       algorithm parameter or invalid key size,
 *                                       @a wrapping_key_alias = NULL, @a wrapped_key = NULL,
 *                                       @a alias = NULL, @a ppdata = NULL)
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN @a wrapping_key_alias does not exist
 * @retval #CKMC_ERROR_DB_ALIAS_EXISTS @a alias already exists
 * @retval #CKMC_ERROR_INVALID_FORMAT The format of @a wrapped_key is not valid
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Wrapping key decryption failed because
 *                                           @a wrapping_key_password is incorrect
 * @retval #CKMC_ERROR_SERVER_ERROR Unknown error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @code
 * ckmc_param_list_h params;    // Initialized elsewhere
 * ckmc_key_s *wrapped_key;     // Initialized elsewhere
 * ckmc_policy_s policy;        // Initialized elsewhere
 * ckmc_raw_buffer_s *ppdata;
 * int ret = ckmc_unwrap_concatenated_data(params,
 *                                         "wrapping_key_alias",
 *                                         "wrapping_key_password",
 *                                         wrapped_key,
 *                                         "alias",
 *                                         192,
 *                                         policy,
 *                                         &ppdata);
 *  ...
 * ckmc_buffer_free(ppdata);
 * @endcode
 *
 * @see ckmc_wrap_concatenated_data()
 * @see #ckmc_param_list_h
 * @see #ckmc_key_s
 * @see #ckmc_policy_s
 * @see #ckmc_raw_buffer_s
 */
int ckmc_unwrap_concatenated_data(const ckmc_param_list_h params,
                                  const char *wrapping_key_alias,
                                  const char *wrapping_key_password,
                                  const ckmc_key_s *wrapped_key,
                                  const char *alias,
                                  size_t size,
                                  const ckmc_policy_s policy,
                                  ckmc_raw_buffer_s **ppdata);


/**
 * @platform
 * @brief Creates private/public key pair based on Key-Encapsulation Mechanism (KEM) type and stores
 *        them inside key manager based on each policy.
 *
 * @since_tizen 7.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/keymanager.extended
 *
 * @remarks If password in @a policy_private_key or @a policy_public_key is provided, the stored key
 *          is additionally encrypted with it.
 * @remarks Currently supported KEM types are: #CKMC_ML_KEM_768, #CKMC_ML_KEM_1024.
 * @remarks The @a private_key_alias must be different than @a public_key_alias.
 *
 * @param[in] kem_type The type of KEM key to be created
 * @param[in] private_key_alias The name of private key to be stored
 * @param[in] public_key_alias The name of public key to be stored
 * @param[in] policy_private_key Private key storing policy
 * @param[in] policy_public_key Public key storing policy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (missing or invalid mandatory
 *                                       algorithm parameter, @a private_key_alias = NULL,
 *                                       @a public_key_alias = NULL)
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ALIAS_EXISTS Alias already exists
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_SERVER_ERROR Unknown error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_encapsulate_key()
 * @see ckmc_decapsulate_key()
 * @see #ckmc_kem_type_e
 * @see #ckmc_policy_s
 */
int ckmc_create_key_pair_kem(const ckmc_kem_type_e kem_type,
                             const char *private_key_alias,
                             const char *public_key_alias,
                             const ckmc_policy_s policy_private_key,
                             const ckmc_policy_s policy_public_key);


/**
 * @platform
 * @brief Generates a random shared secret, encapsulates it using a public KEM key and produces a
 *        ciphertext. The ciphertext is returned and the shared secret is stored inside key
 *        manager using the shared secret alias and the policy provided.
 *
 * @since_tizen 7.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/keymanager.extended
 *
 * @remarks The key used for encapsulation must be public KEM type key (#CKMC_KEY_KEM_PUBLIC).
 * @remarks The KEM type used in key pair creation and encapsulation/decapsulation must be the same.
 * @remarks The supported format of the shared secret is a 32-byte #CKMC_KEY_AES key.
 * @remarks If policy contains password when storing a public key, the same password should be
 *          provided.
 * @remarks If password in @a shared_secret_policy is provided, the stored key is additionally
 *          encrypted with it.
 * @remarks The @a ppciphertext should be released using ckmc_buffer_free().
 *
 * @param[in] params Algorithm parameter list handle. See #ckmc_param_list_h and #ckmc_algo_type_e
 *                   for details. Supported algorithms:
 *                   - #CKMC_ALGO_KEM
 * @param[in] public_key_alias Alias of the public KEM type key to be used for encapsulation
 * @param[in] public_key_password An optional password used in decrypting a key value
 * @param[in] shared_secret_alias Alias to store the shared secret
 * @param[in] shared_secret_policy Shared secret storing policy
 * @param[out] ppciphertext Ciphertext
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Insufficient permissions to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (missing or invalid mandatory
 *                                       algorithm parameter, @a public_key_alias = NULL,
 *                                       @a shared_secret_alias = NULL)
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN @a public_key_alias does not exist
 * @retval #CKMC_ERROR_DB_ALIAS_EXISTS @a shared_secret_alias already exist
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Public key decryption failed because
 *                                           @a public_key_password is incorrect
 * @retval #CKMC_ERROR_SERVER_ERROR Unknown error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @code
 * ckmc_param_list_h params;               // Initialized elsewhere
 * ckmc_policy_s shared_secret_policy;     // Initialized elsewhere
 * ckmc_raw_buffer_s *ppciphertext;
 * int ret = ckmc_encapsulate_key(params,
 *                                "public_key_alias",
 *                                "public_key_password",
 *                                "shared_secret_alias",
 *                                shared_secret_policy,
 *                                &ppciphertext);
 *  ...
 * ckmc_buffer_free(ppciphertext);
 * @endcode
 *
 * @see ckmc_create_key_pair_kem()
 * @see ckmc_decapsulate_key()
 * @see ckmc_key_derive_hybrid()
 * @see #ckmc_param_list_h
 * @see #ckmc_policy_s
 * @see #ckmc_raw_buffer_s
 */
int ckmc_encapsulate_key(const ckmc_param_list_h params,
                         const char *public_key_alias,
                         const char *public_key_password,
                         const char *shared_secret_alias,
                         const ckmc_policy_s shared_secret_policy,
                         ckmc_raw_buffer_s **ppciphertext);


/**
 * @platform
 * @brief Decapsulates the shared secret from the ciphertext and KEM type private key.
 *        The shared secret is stored inside key manager using the shared secret alias and
 *        the policy provided.
 *
 * @since_tizen 7.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/keymanager.extended
 *
 * @remarks The key used for decapsulation must be private KEM type key (#CKMC_KEY_KEM_PRIVATE).
 * @remarks The KEM type used in key pair creation and encapsulation/decapsulation must be the same.
 * @remarks The supported format of the shared secret is a 32-byte #CKMC_KEY_AES key.
 * @remarks If policy contains password when storing a private key, the same password should be
 *          provided.
 * @remarks If password in @a shared_secret_policy is provided, the stored key is additionally
 *          encrypted with it.
 *
 * @param[in] params Algorithm parameter list handle. See #ckmc_param_list_h and #ckmc_algo_type_e
 *                   for details. Supported algorithms:
 *                   - #CKMC_ALGO_KEM
 * @param[in] private_key_alias Alias of the private KEM type key to be used for decapsulation
 * @param[in] private_key_password An optional password used in decrypting a key value
 * @param[in] shared_secret_alias Alias to store the shared secret
 * @param[in] shared_secret_policy Shared secret storing policy
 * @param[in] ciphertext Ciphertext
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Insufficient permissions to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (missing or invalid mandatory
 *                                       algorithm parameter, @a private_key_alias = NULL,
 *                                       @a shared_secret_alias = NULL, @a ciphertext = NULL)
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN @a private_key_alias does not exist
 * @retval #CKMC_ERROR_DB_ALIAS_EXISTS @a shared_secret_alias already exist
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Private key decryption failed because
 *                                           @a private_key_password is incorrect
 * @retval #CKMC_ERROR_SERVER_ERROR Unknown error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_create_key_pair_kem()
 * @see ckmc_encapsulate_key()
 * @see ckmc_key_derive_hybrid()
 * @see #ckmc_param_list_h
 * @see #ckmc_policy_s
 * @see #ckmc_raw_buffer_s
 */
int ckmc_decapsulate_key(const ckmc_param_list_h params,
                         const char *private_key_alias,
                         const char *private_key_password,
                         const char *shared_secret_alias,
                         const ckmc_policy_s shared_secret_policy,
                         const ckmc_raw_buffer_s *ciphertext);


/**
 * @platform
 * @brief Derives a new key from another two concatenated keys/secrets (first|second) with a given
 *        algorithm and stores it inside key manager using a new key alias and policy.
 *
 * @since_tizen 7.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/keymanager.extended
 *
 * @remarks The key/secret, pointed to by @a first_secret_alias and @a second_secret_alias must be
 *          a binary data or a symmetric key (#CKMC_KEY_AES).
 * @remarks The derived key pointed to by @a new_key_alias will be a symmetric one. It will be
 *          stored as a #CKMC_KEY_AES.
 * @remarks In this method, AES-type keys can be hybridized with KEM-type secrets derived
 *          from encapsulation/decapsulation methods.
 * @remarks If policy contains password when storing a key/secret, the same password should be
 *          provided.
 * @remarks If password in @a new_key_policy is provided, the stored key is additionally
 *          encrypted with it.
 *
 * @param[in] params Algorithm parameter list handle. See #ckmc_param_list_h and #ckmc_algo_type_e
 *                   for details. Supported algorithms:
 *                   - #CKMC_ALGO_KBKDF
 * @param[in] first_secret_alias Alias of the first key/secret to use as an input
 * @param[in] first_secret_password Optional password of the first key/secret used as an input
 * @param[in] second_secret_alias Alias of the second key/secret to use as an input
 * @param[in] second_secret_password Optional password of the second key/secret used as an input
 * @param[in] new_key_alias Alias to store the new derived key/secret
 * @param[in] new_key_policy Policy used to store the new derived key/secret
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Insufficient permissions to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (missing or invalid mandatory
 *                                       algorithm parameter, @a first_secret_alias = NULL,
 *                                       @a second_secret_alias = NULL, @a new_key_alias = NULL)
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN @a first_secret_alias or @a second_secret_alias
 *                                      does not exist
 * @retval #CKMC_ERROR_DB_ALIAS_EXISTS @a new_key_alias already exist
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Key decryption failed because @a first_secret_password
 *                                           or @a second_secret_password is incorrect
 * @retval #CKMC_ERROR_SERVER_ERROR Unknown error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_create_key_pair_kem()
 * @see ckmc_encapsulate_key()
 * @see ckmc_decapsulate_key()
 * @see #ckmc_param_list_h
 * @see #ckmc_policy_s
 */
int ckmc_key_derive_hybrid(const ckmc_param_list_h params,
                           const char *first_secret_alias,
                           const char *first_secret_password,
                           const char *second_secret_alias,
                           const char *second_secret_password,
                           const char *new_key_alias,
                           const ckmc_policy_s new_key_policy);


#ifdef __cplusplus
}
#endif


/**
 * @}
 */


#endif /* __TIZEN_CORE_CKMC_EXTENDED_H__ */
