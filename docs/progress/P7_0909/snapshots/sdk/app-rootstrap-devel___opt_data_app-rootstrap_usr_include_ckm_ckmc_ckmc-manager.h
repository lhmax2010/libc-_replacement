/*
 * Copyright (c) 2000 - 2024 Samsung Electronics Co., Ltd All Rights Reserved
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
 *
 * @file ckmc-manager.h
 * @version 1.0
 * @brief Provides management functions(storing, retrieving, and removing) for keys,
 *        certificates and data of a user and additional crypto functions.
 */


#ifndef __TIZEN_CORE_CKMC_MANAGER_H__
#define __TIZEN_CORE_CKMC_MANAGER_H__


#include <stddef.h>
#include <sys/types.h>
#include <tizen.h>
#include <ckmc/ckmc-type.h>
#include <ckmc/ckmc-error.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_KEY_MANAGER_CLIENT_MODULE
 * @{
 */


/**
 * @brief Stores a key inside key manager based on the provided policy.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks Currently API supports nine types of keys. These are: RSA public/private key,
 *          DSA public/private key, ECDSA public/private key, KEM public/private key and
 *          AES symmetric key.
 * @remarks key_type in key may be set to #CKMC_KEY_NONE as an input. key_type is determined inside
 *          key manager during storing.
 * @remarks Some private key files are protected by a password. If raw_key in key read from those
 *          encrypted files is encrypted with a password, the password should be provided in
 *          the #ckmc_key_s structure.
 * @remarks If password in policy is provided, the key is additionally encrypted with the password
 *          in the policy.
 *
 * @param[in] alias The name of a key to be stored
 * @param[in] key The key's binary value to be stored
 * @param[in] policy Key storing policy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ALIAS_EXISTS Alias already exists
 * @retval #CKMC_ERROR_INVALID_FORMAT The format of raw_key is not valid
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_remove_alias()
 * @see ckmc_get_key()
 * @see ckmc_get_key_alias_list()
 * @see #ckmc_key_s
 * @see #ckmc_policy_s
 */
int ckmc_save_key(const char *alias,
                  const ckmc_key_s key,
                  const ckmc_policy_s policy);


/**
 * @brief Updates the key inside the key manager based on the provided policy.
 *
 * @since_tizen 8.0
 *
 * @remarks Currently API supports nine types of keys. These are RSA public/private key,
 *          DSA public/private key, ECDSA public/private key, KEM public/private key and
 *          AES symmetric key.
 * @remarks key_type in the key manager may be set to #CKMC_KEY_NONE as an input. key_type is
 *          determined inside the key manager during updating key.
 * @remarks Some private key files are protected with the password. If raw_key in the key is
 *          encrypted with the password, the password should be provided in the #ckmc_key_s
 *          structure.
 * @remarks If password exists in the provided policy, the key is additionally encrypted with
 *          the password from the provided policy.
 * @remarks If password changes in the provided policy, the key is re-encrypted with the new
 *          password inside the key manager.
 * @remarks If nothing exists inside the key manager under the provided alias, the key is saved
 *          inside the key manager based on the provided policy.
 * @remarks If anything exists inside the key manager under the provided alias, the old data is
 *          replaced with the key inside the key manager based on the provided policy.
 * @remarks If extractable flag changes in the provided policy, the extractable flag is updated
 *          inside the key manager.
 * @remarks If backend changes, the key is removed from the old backend and saved in the new
 *          backend.
 * @remarks Existing permissions are reset inside the key manager.
 *
 * @param[in] alias The name of the key to be updated
 * @param[in] key The key's binary value to be updated
 * @param[in] policy The policy about how to update the key securely
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access the key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED The user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_INVALID_FORMAT The format of raw_key is not valid
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text
 *      form.
 *
 * @see ckmc_save_key()
 * @see ckmc_remove_alias()
 * @see ckmc_get_key()
 * @see ckmc_get_key_alias_list()
 * @see #ckmc_key_s
 * @see #ckmc_policy_s
 */
int ckmc_update_key(const char *alias, const ckmc_key_s key, const ckmc_policy_s policy);


/**
 * @deprecated Deprecated since 2.4
 *             [Use ckmc_remove_alias() instead]
 *
 * @brief Removes a key from key manager.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks To remove a key, client must have removal permission to the specified key.
 * @remarks The key owner can remove it by default.
 *
 * @param[in] alias The name of a key to be removed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_save_key()
 * @see ckmc_get_key()
 * @see ckmc_get_key_alias_list()
 */
int ckmc_remove_key(const char *alias)
TIZEN_DEPRECATED_API;


/**
 * @brief Gets a key from key manager.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks A client can only access data stored by the client.
 * @remarks If policy contains password in ckmc_save_key(), the same password should be provided.
 * @remarks The newly created @a ppkey must be destroyed by calling ckmc_key_free() if it is no longer needed.
 *
 * @param[in] alias The name of a key to retrieve
 * @param[in] password The password used in decrypting a key value
 * @param[out] ppkey The pointer to a newly created ckmc_key_s handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Decryption failed because password is incorrect
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_save_key()
 * @see ckmc_remove_alias()
 * @see ckmc_get_key_alias_list()
 */
int ckmc_get_key(const char *alias,
                 const char *password,
                 ckmc_key_s **ppkey);


/**
 * @brief Gets a list of all of the keys aliases that the client can access.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks A client can only access data stored by the client.
 * @remarks If there is no available key alias the @a ppalias_list will be NULL on return.
 * @remarks The newly created @a ppalias_list must be destroyed by calling ckmc_alias_list_all_free()
 *          if it is no longer needed.
 *
 * @param[out] ppalias_list The pointer to a newly created #ckmc_alias_list_s handle containing all
 *                          available aliases of keys
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_save_key()
 * @see ckmc_remove_alias()
 * @see ckmc_get_key()
 */
int ckmc_get_key_alias_list(ckmc_alias_list_s **ppalias_list);


/**
 * @brief Gets the information about all of the aliases of keys that the client can access.
 *
 * @since_tizen 5.5
 *
 * @remarks A client can only access data stored by the client and the entries from system database
 *          if it was explicitly permitted to.
 * @remarks If there is no available key alias the @a ppalias_list will be NULL on return.
 * @remarks The newly created @a ppalias_list must be destroyed by calling
 *          ckmc_alias_info_list_all_free() if it is no longer needed.
 *
 * @param[out] ppalias_list The pointer to a newly created ckmc_alias_info_list_s handle containing
 *                          information about all keys aliases
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Insufficient permissions to access key manager or to read
 *                                       the alias list
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_save_key()
 * @see ckmc_remove_alias()
 * @see ckmc_get_key()
 */
int ckmc_get_key_alias_info_list(ckmc_alias_info_list_s **ppalias_list);


/**
 * @brief Stores a certificate inside key manager based on the provided policy.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0
 * @remarks The certificate's binary value will be converted and saved as binary DER encoded
 *          certificates.
 *
 * @param[in] alias The name of a certificate to be stored
 * @param[in] cert The certificate's binary value to be stored
 * @param[in] policy Certificate storing policy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ALIAS_EXISTS Alias already exists
 * @retval #CKMC_ERROR_INVALID_FORMAT The format of raw_cert is not valid
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_remove_alias()
 * @see ckmc_get_cert()
 * @see ckmc_get_cert_alias_list()
 * @see #ckmc_cert_s
 * @see #ckmc_policy_s
 */
int ckmc_save_cert(const char *alias,
                   const ckmc_cert_s cert,
                   const ckmc_policy_s policy);


/**
 * @brief Updates the certificate inside the key manager based on the provided policy.
 *
 * @since_tizen 8.0
 *
 * @remarks The certificate's binary value will be converted and updated as binary DER encoded
 *          certificate.
 * @remarks If password exists in the provided policy, the certificate is additionally encrypted
 *          with the password from the provided policy.
 * @remarks If password changes in the provided policy, the certificate is re-encrypted with the
 *          new password inside the key manager.
 * @remarks If nothing exists inside the key manager under the provided alias, the certificate is
 *          saved inside the key manager based on the provided policy.
 * @remarks If anything exists inside the key manager under the provided alias, the old data is
 *          replaced with the certificate inside the key manager based on the provided policy.
 * @remarks If extractable flag changes in the provided policy, the extractable flag is updated
 *          inside the key manager.
 * @remarks If backend changes, the certificate is removed from the old backend and saved in the
 *          new backend.
 * @remarks Existing permissions are reset inside the key manager.
 *
 * @param[in] alias The name of the certificate to be updated
 * @param[in] cert The certificate's binary value to be stored
 * @param[in] policy The policy about how to update the certificate securely
 *
 * @return @c 0 on success, otherwise a negative error value
 *
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_INVALID_FORMAT The format of raw_cert is not valid
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text
 *      form.
 *
 * @see ckmc_save_cert()
 * @see ckmc_remove_alias()
 * @see ckmc_get_cert()
 * @see ckmc_get_cert_alias_list()
 * @see #ckmc_cert_s
 * @see #ckmc_policy_s
 */
int ckmc_update_cert(const char *alias, const ckmc_cert_s cert, const ckmc_policy_s policy);


/**
 * @deprecated Deprecated since 2.4
 *             [Use ckmc_remove_alias() instead]
 *
 * @brief Removes a certificate from key manager.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks To remove certificate, client must have removal permission to the specified certificate.
 * @remarks The certificate owner can remove it by default.
 *
 * @param[in] alias The name of a certificate to be removed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_save_cert()
 * @see ckmc_get_cert()
 * @see ckmc_get_cert_alias_list()
 */
int ckmc_remove_cert(const char *alias)
TIZEN_DEPRECATED_API;


/**
 * @brief Gets a certificate from key manager.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks A client can only access certificate stored by the client.
 * @remarks A DER encoded certificate will be returned as a return value.
 * @remarks If policy contains password in ckmc_save_cert(), the same password should be provided.
 * @remarks The newly created @a ppcert must be destroyed by calling ckmc_cert_free() if it is no
 *          longer needed.
 *
 * @param[in] alias The name of a certificate to retrieve
 * @param[in] password The password used in decrypting a certificate value
 * @param[out] ppcert The pointer to a newly created ckmc_cert_s handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exists
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Decryption failed because password is incorrect
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_save_cert()
 * @see ckmc_remove_alias()
 * @see ckmc_get_cert_alias_list()
 */
int ckmc_get_cert(const char *alias,
                  const char *password,
                  ckmc_cert_s **ppcert);


/**
 * @brief Gets all alias of certificates which the client can access.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks A client can only access data stored by the client.
 * @remarks If there is no available key alias the @a ppalias_list will be NULL on return.
 * @remarks The newly created @a ppalias_list must be destroyed by calling ckmc_alias_list_all_free()
 *          if it is no longer needed.
 *
 * @param[out] ppalias_list The pointer to a newly created ckmc_alias_list_s handle containing all
 *                          available alias of keys
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_save_cert()
 * @see ckmc_remove_alias()
 * @see ckmc_get_cert()
 */
int ckmc_get_cert_alias_list(ckmc_alias_list_s **ppalias_list);


/**
 * @brief Gets the information about all the aliases of certificates that the client can access.
 *
 * @since_tizen 5.5
 *
 * @remarks A client can only access data stored by the client and the entries from system database
 *          if it was explicitly permitted to.
 * @remarks The newly created @a ppalias_list must be destroyed by calling
 *          ckmc_alias_info_list_all_free() if it is no longer needed.
 * @remarks If there is no available certificate alias, @a ppalias_list will return NULL.
 *
 * @param[out] ppalias_list The pointer to a newly created ckmc_alias_info_list_s handle containing
 *                          information about all certificate aliases
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Insufficient permissions to access key manager or to read
 *                                       the alias list
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_save_cert()
 * @see ckmc_remove_alias()
 * @see ckmc_get_cert()
 */
int ckmc_get_cert_alias_info_list(ckmc_alias_info_list_s **ppalias_list);


/**
 * @brief Stores PKCS12's contents inside key manager based on the provided policies.
 *
 * @since_tizen 2.4
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks All items from the PKCS12 will use the same alias.
 *
 * @param[in] alias The name of data to be stored
 * @param[in] pkcs Pointer to the pkcs12 structure to be saved
 * @param[in] key_policy Pkcs's private key storing policy
 * @param[in] cert_policy Pkcs's certificate storing policy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ALIAS_EXISTS Alias already exists
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_remove_alias()
 * @see ckmc_get_pkcs12()
 * @see ckmc_get_data_alias_list()
 * @see ckmc_pkcs12_load()
 * @see #ckmc_pkcs12_s
 * @see #ckmc_policy_s
 */
int ckmc_save_pkcs12(const char *alias,
                     const ckmc_pkcs12_s *pkcs,
                     const ckmc_policy_s key_policy,
                     const ckmc_policy_s cert_policy);


/**
 * @brief Updates PKCS12's contents inside the key manager based on the provided policies. All
 *        items from the PKCS12 use the same alias.
 *
 * @since_tizen 8.0
 *
 * @remarks If password exists in the provided key/certificate policy, the key/certificate/chain
 *          is additionally encrypted with the password from the provided key/certificate policy.
 * @remarks If password changes in the provided key/certificate policy, the key/certificate/chain
 *          is re-encrypted with the new password inside the key manager.
 * @remarks If nothing exists inside the key manager under the provided alias, the pkcs12 is
 *          saved inside the key manager based on the provided policy.
 * @remarks If anything exists inside the key manager under the provided alias, the old data is
 *          replaced with the pkcs12 inside the key manager based on the provided policy.
 * @remarks If extractable flag changes in the provided policy, the extractable flag is updated
 *          inside the key manager.
 * @remarks If backend changes, the pkcs12 is removed from the old backend and saved in the new
 *          backend.
 *
 * @remarks Existing permissions are reset inside the key manager.
 *
 * @param[in] alias The name of the pkcs12 to be updated
 * @param[in] pkcs Pointer to the pkcs12 structure to be updated
 * @param[in] key_policy The policy about how to store pkcs's private key
 * @param[in] cert_policy The policy about how to store pkcs's certificate and certificate chain
 *
 * @return @c 0 on success, otherwise a negative error value
 *
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_INVALID_FORMAT The format of raw_key or raw_cert is not valid
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text
 *      form.
 *
 * @see ckmc_save_pkcs12()
 * @see ckmc_remove_alias()
 * @see ckmc_get_pkcs12()
 * @see ckmc_get_data_alias_list()
 * @see ckmc_pkcs12_load()
 * @see #ckmc_pkcs12_s
 * @see #ckmc_policy_s
 */
int ckmc_update_pkcs12(const char *alias,
                       const ckmc_pkcs12_s *pkcs,
                       const ckmc_policy_s key_policy,
                       const ckmc_policy_s cert_policy);


/**
 * @brief Gets a pkcs12 from key manager.
 *
 * @since_tizen 2.4
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks A client can only access data stored by the client.
 * @remarks The newly created @a pkcs12 must be destroyed by calling ckmc_pkcs12_free() if it is no
 *          longer needed.
 *
 * @param[in] alias The name of a data to retrieve
 * @param[in] key_password Password that was used to encrypt privateKey (may be NULL)
 * @param[in] cert_password Password used to encrypt certificates (may be NULL)
 * @param[out] pkcs12 The pointer to a newly created ckmc_pkcs12_s handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED key_password or cert_password does not match with
 *                                           password used to encrypt data
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_save_pkcs12()
 * @see ckmc_remove_alias()
 */
int ckmc_get_pkcs12(const char *alias,
                    const char *key_password,
                    const char *cert_password,
                    ckmc_pkcs12_s **pkcs12);


/**
 * @brief Stores a data inside key manager based on the provided policy.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 *
 * @remarks Although the Key Manager can store data securely on its repository, it is not designed
 *          to handle large amounts of data (more than 32 bytes) or frequently updated data.
 *          Therefore, we recommend creating an AES key for each client specifically
 *          to encrypt/decrypt larger or frequently updated data. Once you have generated the AES key,
 *          you can then encrypt your data using this key before storing it in a file system accessible
 *          by the client. This way, when the client requires access to the data,
 *          they can easily decrypt it using their respective AES key.
 *
 * @param[in] alias The name of a data to be stored
 * @param[in] data The binary value to be stored
 * @param[in] policy Data storing policy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ALIAS_EXISTS Alias already exists
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @par Example
 * @snippet data_store.cpp DATA_STORE example
 *
 * @see ckmc_remove_alias()
 * @see ckmc_get_data()
 * @see ckmc_get_data_alias_list()
 * @see #ckmc_raw_buffer_s
 * @see #ckmc_policy_s
 *
 */
int ckmc_save_data(const char *alias,
                   ckmc_raw_buffer_s data,
                   const ckmc_policy_s policy);


/**
 * @brief Updates the data inside the key manager based on the provided policy.
 *
 * @since_tizen 8.0
 *
 * @remarks If password exists in the provided policy, the data is additionally encrypted with
 *          the password from the provided policy.
 * @remarks If password changes in the provided policy, the data is re-encrypted with the new
 *          password inside the key manager.
 * @remarks If nothing exists inside the key manager under the provided alias, the data is
 *          saved inside the key manager based on the provided policy.
 * @remarks If anything exists inside the key manager under the provided alias, the old data is
 *          replaced with the new data inside the key manager based on the provided policy.
 * @remarks If extractable flag changes in the provided policy, the extractable flag is updated
 *          inside the key manager.
 * @remarks If backend changes, the data is removed from the old backend and saved in the new
 *          backend.
 * @remarks Existing permissions are reset inside the key manager.
 *
 * @param[in] alias The name of the data to be updated
 * @param[in] data The binary value to be stored
 * @param[in] policy The policy about how to store the data securely
 *
 * @return @c 0 on success, otherwise a negative error value
 *
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text
 *      form.
 *
 * @see ckmc_save_data()
 * @see ckmc_remove_alias()
 * @see ckmc_get_data()
 * @see ckmc_get_data_alias_list()
 * @see #ckmc_raw_buffer_s
 * @see #ckmc_policy_s
 */
int ckmc_update_data(const char *alias, ckmc_raw_buffer_s data, const ckmc_policy_s policy);


/**
 * @deprecated Deprecated since 2.4
 *             [Use ckmc_remove_alias() instead]
 *
 * @brief Removes a data from key manager.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks To remove data, client must have removal permission to the specified data object.
 * @remarks The data owner can remove it by default.
 *
 * @param[in] alias The name of data to be removed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to the error with unknown reason
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_save_data()
 * @see ckmc_get_data()
 * @see ckmc_get_data_alias_list()
 */
int ckmc_remove_data(const char *alias)
TIZEN_DEPRECATED_API;


/**
 * @brief Gets a data from key manager.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks A client can only access data stored by the client.
 * @remarks If policy contains password in ckmc_save_data(), the same password should be provided.
 * @remarks The newly created @a ppdata must be destroyed by calling ckmc_buffer_free() if it is no
 *          longer needed.
 *
 * @param[in] alias The name of data to retrieve
 * @param[in] password The password used in decrypting a data value
 * @param[out] ppdata The pointer to a newly created ckmc_raw_buffer_s handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to the error with unknown reason
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Decryption failed because password is incorrect
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 * @see ckmc_save_data()
 * @see ckmc_remove_alias()
 * @see ckmc_get_data_alias_list()
 */
int ckmc_get_data(const char *alias,
                  const char *password,
                  ckmc_raw_buffer_s **ppdata);


/**
 * @brief Gets all alias of data which the client can access.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks A client can only access data stored by the client.
 * @remarks If there is no available key alias, @a ppalias_list will be NULL on return.
 * @remarks The newly created @a ppalias_list must be destroyed by calling ckmc_alias_list_all_free()
 *          if it is no longer needed.
 *
 * @param[out] ppalias_list The pointer to a newly created ckmc_alias_list_s handle containing all
 *             available alias of keys
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to the error with unknown reason
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_save_data()
 * @see ckmc_remove_alias()
 * @see ckmc_get_data()
 */
int ckmc_get_data_alias_list(ckmc_alias_list_s **ppalias_list);


/**
 * @brief Gets the information about all the aliases of data that the client can access.
 *
 * @since_tizen 5.5
 *
 * @remarks A client can only access data stored by the client and the entries from system database
 *          if it was explicitly permitted to.
 * @remarks The newly created @a ppalias_list must be destroyed by calling
 *          ckmc_alias_info_list_all_free() if it is no longer needed.
 * @remarks If there is no available data alias the @a ppalias_list will be NULL on return.
 *
 * @param[out] ppalias_list The pointer to a newly created ckmc_alias_info_list_s handle containing
 *                          information about all data aliases
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Insufficient permissions to access key manager or to read
 *                                       the alias list
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to the error with unknown reason
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_save_data()
 * @see ckmc_remove_alias()
 * @see ckmc_get_data()
 */
int ckmc_get_data_alias_info_list(ckmc_alias_info_list_s **ppalias_list);


/**
 * @brief Creates RSA private/public key pair and stores them inside key manager based on each policy.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks The supported sizes are: @c 1024, @c 2048, @c 3072 and @c 4096 bits.
 * @remarks If password in the policy is provided, the key is additionally encrypted with the
 *          password in the policy.
 * @remarks The @a private_key_alias must be different than @a public_key_alias.
 *
 * @param[in] size The size of key strength to be created
 * @param[in] private_key_alias The name of private key to be stored
 * @param[in] public_key_alias The name of public key to be stored
 * @param[in] policy_private_key Private key storing policy
 * @param[in] policy_public_key Public key storing policy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ALIAS_EXISTS Alias already exists
 * @retval #CKMC_ERROR_DB_ERROR Failed due to other DB transaction unexpectedly
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_create_key_pair_dsa()
 * @see ckmc_create_key_pair_ecdsa()
 * @see ckmc_create_signature()
 * @see ckmc_verify_signature()
 */
int ckmc_create_key_pair_rsa(const size_t size,
                             const char *private_key_alias,
                             const char *public_key_alias,
                             const ckmc_policy_s policy_private_key,
                             const ckmc_policy_s policy_public_key);


/**
 * @brief Creates DSA private/public key pair and stores them inside key manager based on each policy.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks The supported sizes are: @c 1024, @c 2048, @c 3072 and (Since 7.0) @c 4096 bits.
 * @remarks If password in the policy is provided, the key is additionally encrypted with the
 *          password in the policy.
 * @remarks The @a private_key_alias must be different than @a public_key_alias.
 *
 * @param[in] size The size of key strength to be created
 * @param[in] private_key_alias The name of private key to be stored
 * @param[in] public_key_alias The name of public key to be stored
 * @param[in] policy_private_key Private key storing policy
 * @param[in] policy_public_key Public key storing policy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ALIAS_EXISTS Alias already exists
 * @retval #CKMC_ERROR_DB_ERROR Failed due to other DB transaction unexpectedly
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_create_key_pair_rsa()
 * @see ckmc_create_key_pair_ecdsa()
 * @see ckmc_create_signature()
 * @see ckmc_verify_signature()
 */
int ckmc_create_key_pair_dsa(const size_t size,
                             const char *private_key_alias,
                             const char *public_key_alias,
                             const ckmc_policy_s policy_private_key,
                             const ckmc_policy_s policy_public_key);


/**
 * @brief Creates ECDSA private/public key pair and stores them inside key manager based on each policy.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks If password in the policy is provided, the key is additionally encrypted with the
 *          password in the policy.
 * @remarks Currently supported elliptic curves of ECDSA are: #CKMC_EC_PRIME192V1, #CKMC_EC_PRIME256V1,
 *          #CKMC_EC_SECP384R1
 * @remarks The @a private_key_alias must be different than @a public_key_alias.
 *
 * @param[in] type The type of elliptic curve of ECDSA
 * @param[in] private_key_alias The name of private key to be stored
 * @param[in] public_key_alias The name of public key to be stored
 * @param[in] policy_private_key Private key storing policy
 * @param[in] policy_public_key Public key storing policy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ALIAS_EXISTS Alias already exists
 * @retval #CKMC_ERROR_DB_ERROR Failed due to other DB transaction unexpectedly
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_create_key_pair_rsa()
 * @see ckmc_create_key_pair_dsa()
 * @see ckmc_create_signature()
 * @see ckmc_verify_signature()
 * @see #ckmc_ec_type_e
 */
int ckmc_create_key_pair_ecdsa(const ckmc_ec_type_e type,
                               const char *private_key_alias,
                               const char *public_key_alias,
                               const ckmc_policy_s policy_private_key,
                               const ckmc_policy_s policy_public_key);


/**
 * @brief Creates AES key and stores it inside key manager based on the policy.
 *
 * @since_tizen 3.0
 *
 * @remarks The supported sizes are: @c 128, @c 192 and @c 256 bits.
 * @remarks If password in the policy is provided, the key is additionally encrypted with the
 *          password in the policy.
 *
 * @param[in] size The size of key strength to be created
 * @param[in] key_alias The name of key to be stored
 * @param[in] key_policy Key storing policy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Insufficient permissions to access key manager or to create the key
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ALIAS_EXISTS Alias already exists
 * @retval #CKMC_ERROR_DB_ERROR Failed due to other DB transaction unexpectedly
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_create_key_pair_rsa()
 * @see ckmc_create_key_pair_dsa()
 * @see ckmc_create_key_pair_ecdsa()
 * @see #ckmc_policy_s
 */
int ckmc_create_key_aes(size_t size,
                        const char *key_alias,
                        ckmc_policy_s key_policy);


/**
 * @brief Creates and returns a signature for a given message using a private key.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks If policy contains password when storing a key, the same password should be provided.
 * @remarks The newly created @a ppsignature must be destroyed by calling ckmc_buffer_free() if it is
 *          no longer needed.
 * @remarks CKMC_HASH_NONE is invalid for DSA, ECDSA and RSA with X9.31 padding.
 * @remarks If @a padding is CKMC_NONE_PADDING the user must use CKMC_HASH_NONE and the message must
 *          be equal to the key length.
 * @remarks The @a padding is used only when the signature algorithm is RSA.
 * @remarks If an error occurs the *@a ppsignature will be NULL on return.
 *
 * @param[in] private_key_alias The name of private key
 * @param[in] password The password used in decrypting a private key value
 * @param[in] message The message that is signed with a private key
 * @param[in] hash The hash algorithm used in signature creation
 * @param[in] padding The RSA padding algorithm used in signature creation
 * @param[out] ppsignature The pointer to a newly created signature
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to the error with unknown reason
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Decryption failed because password is incorrect
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_create_key_pair_rsa()
 * @see ckmc_create_key_pair_ecdsa()
 * @see ckmc_verify_signature()
 * @see ckmc_buffer_free()
 * @see #ckmc_hash_algo_e
 * @see #ckmc_rsa_padding_algo_e
 */
int ckmc_create_signature(const char *private_key_alias,
                          const char *password,
                          const ckmc_raw_buffer_s message,
                          const ckmc_hash_algo_e hash,
                          const ckmc_rsa_padding_algo_e padding,
                          ckmc_raw_buffer_s **ppsignature);


/**
 * @brief Verifies a given signature created for a given message using a public key and returns the
 *        signature's status.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks If policy contains password when storing a key, the same password should be provided.
 * @remarks CKMC_HASH_NONE is invalid for DSA, ECDSA and RSA with X9.31 padding.
 * @remarks If @a padding is CKMC_NONE_PADDING the user must use CKMC_HASH_NONE and the message must
 *          be equal to key length.
 * @remarks The @a padding is used only when the signature algorithm is RSA.
 *
 * @param[in] public_key_alias The name of public key
 * @param[in] password The password used in decrypting a public key value
 * @param[in] message The message for which the signature is created
 * @param[in] signature The signature to be verified with public key
 * @param[in] hash The hash algorithm used in signature verification
 * @param[in] padding The RSA padding algorithm used in signature verification
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_VERIFICATION_FAILED The signature is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to the error with unknown reason
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Decryption failed because password is incorrect
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_create_key_pair_rsa()
 * @see ckmc_create_key_pair_ecdsa()
 * @see ckmc_create_signature()
 * @see #ckmc_hash_algo_e
 * @see #ckmc_rsa_padding_algo_e
 */
int ckmc_verify_signature(const char *public_key_alias,
                          const char *password,
                          const ckmc_raw_buffer_s message,
                          const ckmc_raw_buffer_s signature,
                          const ckmc_hash_algo_e hash,
                          const ckmc_rsa_padding_algo_e padding);


/**
 * @brief Verifies a certificate chain and returns that chain.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks The trusted root certificate of the chain should exist in the system's certificate
 *          storage.
 * @remarks The newly created @a ppcert_chain_list must be destroyed by calling
 *          ckmc_cert_list_all_free() if it is no longer needed.
 * @remarks If an error occurs the @a ppcert_chain_list will be NULL on return.
 *
 * @param[in] cert The certificate to be verified
 * @param[in] untrustedcerts The untrusted CA certificates to be used in verifying a certificate
 *                           chain
 * @param[out] ppcert_chain_list The pointer to a newly created certificate chain's handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_VERIFICATION_FAILED The certificate chain is not valid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to the error with unknown reason
 * @retval #CKMC_ERROR_INVALID_FORMAT The format of certificate is not valid
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Decryption failed because password is incorrect
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_cert_list_all_free()
 */
int ckmc_get_cert_chain(const ckmc_cert_s *cert,
                        const ckmc_cert_list_s *untrustedcerts,
                        ckmc_cert_list_s **ppcert_chain_list);


/**
 * @deprecated Deprecated since 2.4
 *             [Use ckmc_get_cert_chain() instead]
 *
 * @brief Verifies a certificate chain using an alias list of untrusted certificates and returns that
 *        chain.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks The trusted root certificate of the chain should exist in the system's certificate
 *          storage.
 * @remarks The newly created @a ppcert_chain_list must be destroyed by calling
 *          ckmc_cert_list_all_free() if it is no longer needed.
 * @remarks @a untrustedcerts shouldn't be protected with optional password.
 * @remarks If an error occurs the @a ppcert_chain_list will be NULL on return.
 *
 * @param[in] cert The certificate to be verified
 * @param[in] untrustedcerts The alias list of untrusted CA certificates stored in key manager to be
 *                           used to verify a certificate chain
 * @param[out] ppcert_chain_list The pointer to a newly created certificate chain's handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_VERIFICATION_FAILED The certificate chain is not valid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to the error with unknown reason
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 * @retval #CKMC_ERROR_INVALID_FORMAT The format of certificate is not valid
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Some certificates were encrypted with password and
 *                                           could not be used
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_get_cert_chain()
 * @see ckmc_cert_list_all_free()
 */
int ckmc_get_cert_chain_with_alias(const ckmc_cert_s *cert,
                                   const ckmc_alias_list_s *untrustedcerts,
                                   ckmc_cert_list_s **ppcert_chain_list) TIZEN_DEPRECATED_API;


/**
 * @brief Verifies a certificate chain and returns that chain using user-entered, trusted, and
 *        untrusted CA certificates.
 *
 * @since_tizen 2.4
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks If the trusted root certificates are provided as a user input, these certificates do not
 *          need to exist in the system's certificate storage.
 * @remarks The newly created @a ppcert_chain_list must be destroyed by calling
 *          ckmc_cert_list_all_free() if it is no longer needed.
 * @remarks If an error occurs the @a ppcert_chain_list will be NULL on return.
 *
 * @param[in] cert The certificate to be verified
 * @param[in] untrustedcerts The untrusted CA certificates to be used in verifying a certificate chain
 * @param[in] trustedcerts The trusted CA certificates to be used in verifying a certificate chain
 * @param[in] use_trustedsystemcerts The flag indicating the use of the trusted root certificates in
 *                                   the system's certificate storage
 * @param[out] ppcert_chain_list The pointer to a newly created certificate chain's handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_VERIFICATION_FAILED The certificate chain is not valid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to the error with unknown reason
 * @retval #CKMC_ERROR_INVALID_FORMAT The format of certificate is not valid
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_cert_list_all_free()
 */
int ckmc_get_cert_chain_with_trustedcert(const ckmc_cert_s *cert,
                                         const ckmc_cert_list_s *untrustedcerts,
                                         const ckmc_cert_list_s *trustedcerts,
                                         const bool use_trustedsystemcerts,
                                         ckmc_cert_list_s **ppcert_chain_list);


/**
 * @deprecated Deprecated since 6.5. Use raw OpenSSL instead.
 *
 * @brief Performs OCSP that checks whether a certificate is revoked or not.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 *
 * @remarks %http://tizen.org/privilege/internet (public level privilege) is required to use this
 *          function instead of %http://tizen.org/privilege/keymanager (public level privilege)
 *          since 3.0.
 *
 * @param[in] pcert_chain_list Valid certificate chain to perform OCSP check
 * @param[out] ocsp_status The pointer to the status of the result of OCSP check
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_NOT_SUPPORTED Device needed to run API is not supported
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 * @pre @a pcert_chain_list is created with ckmc_get_cert_chain() or
 *      ckmc_get_cert_chain_with_alias().
 *
 * @see ckmc_get_cert_chain()
 * @see ckmc_cert_list_all_free()
 */
int ckmc_ocsp_check(const ckmc_cert_list_s *pcert_chain_list,
                    ckmc_ocsp_status_e *ocsp_status) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 2.4
 *             [Use ckmc_set_permission() instead]
 *
 * @brief Allows another application to access client's application data.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks Data identified by @a alias should exist.
 *
 * @param[in] alias Data alias for which access will be granted
 * @param[in] accessor Package id of the application that will gain access rights
 * @param[in] granted Rights granted for @a accessor application
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager or modify permissions
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to the error with unknown reason
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_deny_access()
 */
int ckmc_allow_access(const char *alias,
                      const char *accessor,
                      ckmc_access_right_e granted) TIZEN_DEPRECATED_API;


/**
 * @brief Allows another application to access client's application data.
 *
 * @since_tizen 2.4
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks Data identified by @a alias should exist.
 * @remarks Previous permission mask will be replaced with the new mask value passed by @a permissions.
 *
 * @param[in] alias Data alias for which access will be granted
 * @param[in] accessor Package id of the application that will gain access rights
 * @param[in] permissions Mask of permissions granted for @a accessor application (#ckmc_permission_e)
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager or modify permissions
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to the error with unknown reason
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 */
int ckmc_set_permission(const char *alias,
                        const char *accessor,
                        int permissions);


/**
 * @deprecated Deprecated since 2.4
 *             [Use ckmc_set_permission() instead]
 *
 * @brief Revokes another application's access to client's application data.
 *
 * @since_tizen 2.3
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks Data identified by @a alias should exist.
 * @remarks Only access previously granted with ckmc_allow_access() can be revoked.
 *
 * @param[in] alias Data alias for which access will be revoked
 * @param[in] accessor Package id of the application that will lose access rights
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager or modify permissions
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid or the @a accessor doesn't have
 *                                       access to @a alias
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to the error with unknown reason
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_allow_access()
 * @see ckmc_set_permission()
 */
int ckmc_deny_access(const char *alias,
                     const char *accessor) TIZEN_DEPRECATED_API;


/**
 * @brief Removes an entry (no matter of type) from the key manager.
 *
 * @since_tizen 2.4
 *
 * @remarks %http://tizen.org/privilege/keymanager (public level privilege) is no longer required to
 *          use this function since 3.0.
 * @remarks To remove item, client must have removal permission to the specified item.
 * @remarks The item owner can remove it by default.
 *
 * @param[in] alias Item alias to be removed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager or the item to remove
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Alias does not exist
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_save_key()
 * @see ckmc_save_cert()
 * @see ckmc_save_data()
 * @see ckmc_save_pkcs12()
 * @see ckmc_create_key_pair_rsa()
 * @see ckmc_create_key_pair_dsa()
 * @see ckmc_create_key_pair_ecdsa()
 * @see ckmc_create_key_pair_kem()
 */
int ckmc_remove_alias(const char *alias);


/**
 * @brief Encrypts data using selected key and algorithm.
 *
 * @since_tizen 3.0
 *
 * @remarks Key identified by @a key_alias should exist.
 * @remarks If password of the policy is provided in ckmc_save_key(), the same password should be provided.
 * @remarks In case of AES algorithm the backend may impose limit on the maximum size of processed data
 *          (ckmc_backend_get_max_chunk_size()).
 * @remarks For RSA the size must be smaller or equal to:
 *          key size in bytes - 2 * hash function output size in bytes - 2.
 *          Example: for 1024 RSA key and hash SHA1 the maximum data size is 1024/8 - 2*160/8 = 86.
 * @remarks The @a ppencrypted must be destroyed with ckmc_buffer_free().
 * @remarks In #CKMC_ALGO_AES_GCM mode the @a ppencrypted includes the GCM tag appended at the end.
 *
 * @param[in] params Algorithm parameter list handle. See #ckmc_param_list_h and #ckmc_algo_type_e
 *                   for details. Supported algorithms:
 *                   - #CKMC_ALGO_AES_CTR,
 *                   - #CKMC_ALGO_AES_CBC,
 *                   - #CKMC_ALGO_AES_GCM,
 *                   - #CKMC_ALGO_AES_CFB,
 *                   - #CKMC_ALGO_RSA_OAEP
 * @param[in] key_alias Alias of the key to be used for encryption
 * @param[in] password The password used in decrypting a key value
 * @param[in] decrypted Data to be encrypted
 * @param[out] ppencrypted Encrypted data
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager or the encrypting key
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (missing or invalid mandatory
 *                                       algorithm parameter or RSA data too long, decrypted = NULL,
 *                                       ppencrypted = NULL)
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to the error with unknown reason
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Key with given alias does not exist
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Key decryption failed because password is incorrect
 * @retval #CKMC_ERROR_SERVER_ERROR Too big data size or unsupported GCM mode (32 and 64 bit tag
 *                                  lengths not supported on TEE backend) or internal error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_buffer_free()
 * @see ckmc_param_list_new()
 * @see ckmc_param_list_free()
 * @see ckmc_param_list_set_integer()
 * @see ckmc_param_list_set_buffer()
 * @see ckmc_generate_new_params()
 * @see #ckmc_param_list_h
 * @see #ckmc_param_name_e
 * @see #ckmc_algo_type_e
 */
int ckmc_encrypt_data(ckmc_param_list_h params,
                      const char *key_alias,
                      const char *password,
                      const ckmc_raw_buffer_s decrypted,
                      ckmc_raw_buffer_s **ppencrypted);


/**
 * @brief Decrypts data using selected key and algorithm.
 *
 * @since_tizen 3.0
 *
 * @remarks Key identified by @a key_alias should exist.
 * @remarks The @a ppdecrypted must be destroyed with ckmc_buffer_free().
 *
 * @param[in] params Algorithm parameter list handle. User should use the same parameters that were
 *                   used for encryption. See #ckmc_param_list_h and #ckmc_algo_type_e for details.
 *                   Supported algorithms:
 *                   - #CKMC_ALGO_AES_CTR,
 *                   - #CKMC_ALGO_AES_CBC,
 *                   - #CKMC_ALGO_AES_GCM,
 *                   - #CKMC_ALGO_AES_CFB,
 *                   - #CKMC_ALGO_RSA_OAEP
 * @param[in] key_alias Alias of the key to be used for encryption
 * @param[in] password The password used in decrypting a key value. If password of the policy is
 *                     provided in ckmc_save_key(), the same password should be provided
 * @param[in] encrypted Data to be decrypted. #CKMC_ALGO_AES_GCM mode requires GCM tag to be
 *                      appended at the end. In case of AES algorithm the backend may impose limit
 *                      on the maximum size of processed data (ckmc_backend_get_max_chunk_size()).
 * @param[out] ppdecrypted Decrypted data
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Failed to access key manager or the decrypting key
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (missing or invalid mandatory
 *                                       algorithm parameter, GCM tag authentication failed, key or
 *                                       data is wrong, in case of RSA key is wrong or data too
 *                                       long, encrypted = NULL, ppdecrypted = NULL)
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ERROR Failed due to the error with unknown reason
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN Key with given alias does not exist
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Key decryption failed because password is incorrect
 * @retval #CKMC_ERROR_SERVER_ERROR Too big data size or unsupported GCM mode (32 and 64 bit tag
 *                                  lengths not supported on TEE backend) or internal error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see ckmc_buffer_free()
 * @see ckmc_param_list_new()
 * @see ckmc_param_list_free()
 * @see ckmc_param_list_set_integer()
 * @see ckmc_param_list_set_buffer()
 * @see ckmc_generate_new_params()
 * @see #ckmc_param_list_h
 * @see #ckmc_param_name_e
 * @see #ckmc_algo_type_e
 */
int ckmc_decrypt_data(ckmc_param_list_h params,
                      const char *key_alias,
                      const char *password,
                      const ckmc_raw_buffer_s encrypted,
                      ckmc_raw_buffer_s **ppdecrypted);


/**
 * @brief Unwraps one key with another and stores it inside key manager.
 *
 * @since_tizen 6.0
 *
 * @remarks The wrapping key must be either symmetric (#CKMC_KEY_AES) or private RSA
 *          (#CKMC_KEY_RSA_PRIVATE).
 * @remarks key_type in @a wrapped_key can only be #CKMC_KEY_AES.
 * @remarks password in @a wrapped_key must be set to NULL. There's no need to additionally encrypt
 *          a wrapped key.
 * @remarks If password in @a policy is provided, the stored key is additionally encrypted with it.
 * @remarks If extractable in @a policy is set to false, the stored key may still be exported in a
 *          wrapped form.
 * @remarks Note that the backend may impose limit on the maximum size of @a wrapped_key
 *          (ckmc_backend_get_max_chunk_size()).
 * @remarks #CKMC_ALGO_AES_GCM mode requires GCM tag to be appended at the end of the @a wrapped_key.
 *
 * @param[in] params Algorithm parameter list handle. See #ckmc_param_list_h and #ckmc_algo_type_e
 *                   for details. Supported algorithms:
 *                   - #CKMC_ALGO_AES_CTR,
 *                   - #CKMC_ALGO_AES_CBC,
 *                   - #CKMC_ALGO_AES_GCM,
 *                   - #CKMC_ALGO_AES_CFB,
 *                   - #CKMC_ALGO_RSA_OAEP
 * @param[in] wrapping_key_alias The name of the wrapping key.
 * @param[in] wrapping_key_password An optional password of the wrapping key
 * @param[in] alias The name of a key to be stored
 * @param[in] wrapped_key The wrapped key to be unwrapped and stored
 * @param[in] policy Key storing policy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Insufficient permissions to access key manager, the
 *                                       wrapping key or to create the unwrapped key
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (missing or invalid mandatory
 *                                       algorithm parameter, GCM tag authentication failed,
 *                                       @a wrapping_key_alias = NULL, @a alias = NULL,
 *                                       @a wrapped_key = NULL)
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
 * @see ckmc_export_wrapped_key()
 * @see #ckmc_key_s
 * @see #ckmc_param_list_h
 * @see #ckmc_policy_s
 */
int ckmc_import_wrapped_key(const ckmc_param_list_h params,
                            const char *wrapping_key_alias,
                            const char *wrapping_key_password,
                            const char *alias,
                            const ckmc_key_s *wrapped_key,
                            const ckmc_policy_s policy);


/**
 * @brief Wraps one key with another and returns it to the client.
 *
 * @since_tizen 6.0
 *
 * @remarks The wrapping key must be either symmetric (#CKMC_KEY_AES) or public RSA
 *          (#CKMC_KEY_RSA_PUBLIC).
 * @remarks The @a ppwrapped_key should be released using ckmc_key_free().
 * @remarks The key denoted by @a alias can only be #CKMC_KEY_AES.
 * @remarks If the wrapping key is public RSA, the key size denoted by @a alias must be smaller than:
 *          wrapping key size in bits - 2* hash function output size in bits - 16.
 *          Example: for 1024 RSA wrapping key and hash SHA384 the key size must be smaller than:
 *          1024 - 2*384 - 16 = 240 bits.
 * @remarks Considering the key size limit it's recommended to use RSA key longer than @c 1024 bits.
 * @remarks In #CKMC_ALGO_AES_GCM mode the @a ppwrapped_key includes the GCM tag appended at the end.
 *
 * @param[in] params Algorithm parameter list handle. See #ckmc_param_list_h and #ckmc_algo_type_e
 *                   for details. Supported algorithms:
 *                   - #CKMC_ALGO_AES_CTR,
 *                   - #CKMC_ALGO_AES_CBC,
 *                   - #CKMC_ALGO_AES_GCM,
 *                   - #CKMC_ALGO_AES_CFB,
 *                   - #CKMC_ALGO_RSA_OAEP
 * @param[in] wrapping_key_alias The name of the wrapping key
 * @param[in] wrapping_key_password An optional password of the wrapping key
 * @param[in] alias The name of the key to be wrapped and exported
 * @param[in] password An optional password used to decrypt the key pointed by @a alias
 * @param[out] ppwrapped_key The wrapped key
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Insufficient permissions to access key manager, the
 *                                       wrapping key or the key being wrapped
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (missing or invalid mandatory
 *                                       algorithm parameter, GCM tag authentication failed,
 *                                       @a wrapping_key_alias = NULL, @a alias = NULL,
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
 * @see ckmc_import_wrapped_key()
 * @see #ckmc_key_s
 * @see #ckmc_param_list_h
 */
int ckmc_export_wrapped_key(const ckmc_param_list_h params,
                            const char *wrapping_key_alias,
                            const char *wrapping_key_password,
                            const char *alias,
                            const char *password,
                            ckmc_key_s **ppwrapped_key);


/**
 * @brief Derives a secret or key from another key/secret and stores it inside key manager.
 *
 * @since_tizen 6.0
 *
 * @remarks In case of #CKMC_ALGO_KBKDF algorithm, the secret pointed to by @a secret_alias must be
 *          a binary data or a symmetric key (#CKMC_KEY_AES). The derived key pointed to by
 *          @a new_key_alias will be a symmetric one. It will be stored as a #CKMC_KEY_AES.
 * @remarks In case of #CKMC_ALGO_ECDH algorithm, the key pointed to by @a secret_alias must be a
 *          private EC key (#CKMC_KEY_ECDSA_PRIVATE). The derived secret pointed to by
 *          @a new_key_alias  will be in binary data form.
 *
 * @param[in] params Algorithm parameter list handle. See #ckmc_param_list_h and #ckmc_algo_type_e
 *                   for details. Supported algorithms:
 *                   - #CKMC_ALGO_KBKDF,
 *                   - #CKMC_ALGO_ECDH,
 * @param[in] secret_alias Alias of the secret/key to use as an input
 * @param[in] secret_password Optional password of the secret/key used as an input
 * @param[in] new_key_alias The name under which the derived key or secret will be stored
 * @param[in] new_key_policy Policy used to store the derived key or secret
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Insufficient permissions to access key manager, the secret
 *                                       or to create the new key/secret
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (missing or invalid mandatory
 *                                       algorithm parameter, @a secret_alias = NULL,
 *                                       @a new_key_alias = NULL)
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN @a secret_alias does not exist
 * @retval #CKMC_ERROR_DB_ALIAS_EXISTS @a new_key_alias already exists
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Secret decryption failed because @a secret_password is
 *                                           incorrect
 * @retval #CKMC_ERROR_SERVER_ERROR Unknown error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @par Example
 * @snippet key_derive.cpp KEY_DERIVE example
 *
 * @see #ckmc_param_list_h
 * @see #ckmc_policy_s
 */
int ckmc_key_derive(const ckmc_param_list_h params,
                    const char *secret_alias,
                    const char *secret_password,
                    const char *new_key_alias,
                    ckmc_policy_s new_key_policy);

/**
 * @brief Sets up a symmetric encryption or decryption context with given key and parameters.
 *
 * @since_tizen 6.0
 *
 * @remarks The newly created @a context must be destroyed using ckmc_cipher_free() when it's no
 *          longer needed.
 * @remarks The @a context must point to NULL if it's the first call. Otherwise, it must point to
 *          the previously returned context.
 * @remarks To perform the encryption/decryption, one or more calls to ckmc_cipher_update() must be
 *          followed by one call to ckmc_cipher_finalize().
 * @remarks To pass #CKMC_PARAM_ED_AAD in multiple chunks call the ckmc_cipher_initialize() multiple
 *          times with consecutive portions of the AAD in the @a params and the @a context returned
 *          from the first call. It must be done before the first call to ckmc_cipher_update().
 *
 * @param[in] params Algorithm parameter list handle. See #ckmc_param_list_h and #ckmc_algo_type_e
 *                   for details. Supported algorithms:
 *                   - #CKMC_ALGO_AES_GCM,
 * @param[in] key_alias Alias of the key to be used for encryption/decryption
 * @param[in] key_password Optional password of the key used for encryption/decryption
 * @param[in] encrypt Encryption/decryption switch (true=encryption, false=decryption)
 * @param[out] context Encryption/decryption context
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Insufficient permissions to access key manager or the key
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (missing or invalid mandatory
 *                                       algorithm parameter, @a key_alias = NULL,
 *                                       @a context = NULL)
 * @retval #CKMC_ERROR_DB_LOCKED A user key is not loaded in memory (a user is not logged in)
 * @retval #CKMC_ERROR_DB_ALIAS_UNKNOWN @a key_alias does not exist
 * @retval #CKMC_ERROR_DB_ERROR Failed due to a database error
 * @retval #CKMC_ERROR_AUTHENTICATION_FAILED Key decryption failed because @a key_password is
 *                                           incorrect
 * @retval #CKMC_ERROR_SERVER_ERROR Unknown error
 *
 * @pre User is already logged in and the user key is already loaded into memory in plain text form.
 *
 * @see #ckmc_cipher_ctx_h
 * @see ckmc_cipher_update()
 * @see ckmc_cipher_finalize()
 * @see ckmc_cipher_free()
 */
int ckmc_cipher_initialize(ckmc_param_list_h params,
                           const char *key_alias,
                           const char *key_password,
                           bool encrypt,
                           ckmc_cipher_ctx_h *context);

/**
 * @brief Performs symmetric encryption or decryption of the input and places the result in the
 *        output.
 *
 * @since_tizen 6.0
 *
 * @remarks The function may be called multiple times to encrypt successive blocks of data.
 * @remarks The newly created @a ppout must be destroyed using ckmc_buffer_free() when it's no
 *          longer needed.
 * @remarks The @a ppout will be set to NULL if the output is empty.
 * @remarks Note that the backend may impose limit on the maximum size of processed data
 *          (ckmc_backend_get_max_chunk_size()).
 *
 * @param[in] context Encryption/decryption context created with ckmc_cipher_initialize()
 * @param[in] in Encryption/decryption input
 * @param[out] ppout Encryption/decryption output
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (@a context = NULL,
 *                                       @a ppout = NULL)
 * @retval #CKMC_ERROR_SERVER_ERROR Unknown error
 *
 * @see #ckmc_cipher_ctx_h
 * @see ckmc_cipher_initialize()
 * @see ckmc_cipher_finalize()
 * @see ckmc_cipher_free()
 */
int ckmc_cipher_update(ckmc_cipher_ctx_h context,
                       const ckmc_raw_buffer_s in,
                       ckmc_raw_buffer_s **ppout);

/**
 * @brief Finalizes symmetric encryption or decryption and returns remaining output if any.
 *
 * @since_tizen 6.0
 *
 * @remarks After the call to this function the ckmc_cipher_update() can be called no more.
 * @remarks The newly created @a ppout must be destroyed using ckmc_buffer_free() when it's no
 *          longer needed.
 * @remarks When using #CKMC_ALGO_AES_GCM decryption the GCM tag must be passed as @a in. In other
 *          cases @a in should be set to NULL.
 * @remarks When using #CKMC_ALGO_AES_GCM encryption the GCM tag will be returned in @a ppout.
 * @remarks The @a ppout will be set to NULL if the output is empty.
 *
 * @param[in] context Encryption/decryption context created with ckmc_cipher_initialize()
 * @param[in] in Optional additional decryption input required by some of the modes
 * @param[out] ppout Encryption/decryption output
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (@a context = NULL,
 *                                       @a ppout = NULL)
 * @retval #CKMC_ERROR_SERVER_ERROR Unknown error
 *
 * @see #ckmc_cipher_ctx_h
 * @see ckmc_cipher_initialize()
 * @see ckmc_cipher_update()
 * @see ckmc_cipher_free()
 */
int ckmc_cipher_finalize(ckmc_cipher_ctx_h context,
                         const ckmc_raw_buffer_s *in,
                         ckmc_raw_buffer_s **ppout);

/**
 * @brief Destroys the encryption/decryption context and releases all its resources.
 *
 * @since_tizen 6.0
 *
 * @param[in] context Encryption/decryption context created with ckmc_cipher_initialize()
 *
 * @see #ckmc_cipher_ctx_h
 * @see ckmc_cipher_initialize()
 * @see ckmc_cipher_update()
 * @see ckmc_cipher_finalize()
 */
void ckmc_cipher_free(ckmc_cipher_ctx_h context);

/**
 * @brief Retrieves backend information.
 *
 * @since_tizen 6.0
 *
 * @remarks The newly created @a ppinfo must be destroyed using ckmc_backend_info_free() when it's
 *          no longer needed.
 *
 * @param[in] backend Backend identifier
 * @param[out] ppinfo Backend information handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_PERMISSION_DENIED Insufficient permissions to access key manager
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (@a backend is invalid,
 *                                       @a ppinfo = NULL)
 * @retval #CKMC_ERROR_SERVER_ERROR Unknown error
 *
 * @see #ckmc_backend_id_e
 * @see #ckmc_backend_info_h
 * @see ckmc_alias_info_get_backend()
 * @see ckmc_backend_get_max_chunk_size()
 * @see ckmc_backend_info_free()
 */
int ckmc_get_backend_info(ckmc_backend_id_e backend,
                          ckmc_backend_info_h* ppinfo);

#ifdef __cplusplus
}
#endif


/**
 * @}
 */


#endif /* __TIZEN_CORE_CKMC_MANAGER_H__ */
