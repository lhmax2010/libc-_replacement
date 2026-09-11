/*
 * Copyright (c) 2000 - 2021 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @file ckmc-type.h
 * @version 1.0
 * @brief Definitions of struct for the Key Manager's CAPI and their utility functions.
 */


#ifndef __TIZEN_CORE_CKMC_TYPE_H__
#define __TIZEN_CORE_CKMC_TYPE_H__


#include <stddef.h>
#include <stdint.h>
#include <ckmc/ckmc-error.h>


#define KEY_MANAGER_CAPI __attribute__((visibility("default")))


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_KEY_MANAGER_TYPES_MODULE
 * @{
 */


/*
 * Note: on tizen 3.0 owner id is equal to pkgId.
 *       Preinstalled system (uid < 5000) and user (uid >= 5000) applications
 *       do not have any pkgId. That's why ckm uses special "virtual"
 *       pkgid for them. The virtual strings are defined under:
 *                       ckmc_owner_id_system
 */


/**
 * @deprecated Deprecated since 3.0. [Use ckmc_owner_id_separator instead]
 *
 * @brief Separator between alias and label.
 *
 * @since_tizen 2.3
 *
 * @remarks Alias can be provided as an alias alone, or together with label - in this
 *          case, separator " " (space bar) is used to separate label and alias.
 *
 * @see #ckmc_owner_id_separator
 */
KEY_MANAGER_CAPI extern char const *const ckmc_label_name_separator;


/**
 * @brief Separator between alias and owner id.
 *
 * @since_tizen 3.0
 *
 * @remarks Alias can be provided as an alias alone, or together with owner id.
 *          In this case, separator " " (space bar) is used to separate id and alias.
 *
 * @see ckmc_alias_new()
 */
KEY_MANAGER_CAPI extern char const *const ckmc_owner_id_separator;


/**
 * @brief The owner of system database.
 *
 * @since_tizen 3.0
 *
 * @remarks #ckmc_owner_id_system contains id connected with all system applications that run with
 *          uid less than 5000. Client should use #ckmc_owner_id_system to access data owned by
 *          system application and stored in system database. Client must have permission to access
 *          proper row.
 *
 * @see ckmc_alias_new()
 */
KEY_MANAGER_CAPI extern char const *const ckmc_owner_id_system;


/**
 * @brief Enumeration for key types of key manager.
 *
 * @since_tizen 2.3
 */
typedef enum __ckmc_key_type {
    CKMC_KEY_NONE = 0, /**< Key type not specified */
    CKMC_KEY_RSA_PUBLIC, /**< RSA public key */
    CKMC_KEY_RSA_PRIVATE, /**< RSA private key */
    CKMC_KEY_ECDSA_PUBLIC, /**< ECDSA public key */
    CKMC_KEY_ECDSA_PRIVATE, /**< ECDSA private key */
    CKMC_KEY_DSA_PUBLIC, /**< DSA public key */
    CKMC_KEY_DSA_PRIVATE, /**< DSA private key */
    CKMC_KEY_AES, /**< AES key */
    CKMC_KEY_KEM_PUBLIC, /**< KEM public key (Since 7.0) */
    CKMC_KEY_KEM_PRIVATE /**< KEM private key (Since 7.0) */
} ckmc_key_type_e;


/**
 * @brief Enumeration for KEM types.
 *
 * @since_tizen 7.0
 */
typedef enum __ckmc_kem_type {
    CKMC_ML_KEM_768 = 0, /**< ML-KEM-768 algorithm type */
    CKMC_ML_KEM_1024 /**< ML-KEM-1024 algorithm type */
} ckmc_kem_type_e;


/**
 * @brief Enumeration for data format.
 *
 * @since_tizen 2.3
 *
 * @remarks PEM encoded data consists of the DER format base64 encoded with additional
 *          header and footer lines.
 */
typedef enum __ckmc_data_format {
    CKMC_FORM_DER_BASE64 = 0, /**< DER format base64 encoded data */
    CKMC_FORM_DER, /**< DER encoded data */
    CKMC_FORM_PEM /**< PEM encoded data */
} ckmc_data_format_e;


/**
 * @brief Enumeration for elliptic curve.
 *
 * @since_tizen 2.3
 */
typedef enum __ckmc_ec_type {
    CKMC_EC_PRIME192V1 = 0, /**< Elliptic curve domain "secp192r1" recommended and listed in "SEC 2" */
    CKMC_EC_PRIME256V1, /**< Elliptic curve domain "secp256r1" recommended and listed in "SEC 2" */
    CKMC_EC_SECP384R1 /**< Elliptic curve domain NIST curve P-384 (covers "secp384r1"),
                      recommended and listed in "SEC 2" */
} ckmc_ec_type_e;


/**
 * @brief Enumeration for hash algorithm.
 *
 * @since_tizen 2.3
 */
typedef enum __ckmc_hash_algo {
    CKMC_HASH_NONE = 0, /**< No Hash Algorithm */
    CKMC_HASH_SHA1, /**< SHA1 Hash Algorithm */
    CKMC_HASH_SHA256, /**< SHA256 Hash Algorithm */
    CKMC_HASH_SHA384, /**< SHA384 Hash Algorithm */
    CKMC_HASH_SHA512 /**< SHA512 Hash Algorithm */
} ckmc_hash_algo_e;


/**
 * @brief Enumeration for RSA padding algorithm.
 *
 * @since_tizen 2.3
 */
typedef enum __ckmc_rsa_padding_algo {
    CKMC_NONE_PADDING = 0, /**< No Padding */
    CKMC_PKCS1_PADDING, /**< PKCS#1 Padding */
    CKMC_X931_PADDING /**< X9.31 Padding */
} ckmc_rsa_padding_algo_e;


/**
 * @brief Enumeration for AES padding algorithm.
 *
 * @since_tizen 10.0
 */
typedef enum __ckmc_aes_padding_algo {
    CKMC_AES_PADDING_NONE = 0, /**< No Padding */
    CKMC_AES_PADDING_PKCS7, /**< PKCS#7 Padding (default for openssl) */
    CKMC_AES_PADDING_ISO9797_M2 /**< ISO9797 method 2 Padding */
} ckmc_aes_padding_algo_e;


/**
 * @deprecated Deprecated since 2.4
 *             [Use #ckmc_permission_e instead]
 *
 * @brief Enumeration for database access rights.
 *
 * @since_tizen 2.3
 */
typedef enum __ckmc_access_right {
    CKMC_AR_READ = 0, /**< Access right for read*/
    CKMC_AR_READ_REMOVE /**< Access right for read and remove*/
} ckmc_access_right_e;


/**
 * @brief Enumeration for permissions to access/modify alias.
 *
 * @since_tizen 2.4
 */
typedef enum __ckmc_permission {
    CKMC_PERMISSION_NONE = 0x00, /**< Clear permissions */
    CKMC_PERMISSION_READ = 0x01, /**< Read allowed */
    CKMC_PERMISSION_REMOVE = 0x02 /**< Remove allowed */
} ckmc_permission_e;


/**
 * @brief The structure for binary buffer used in key manager CAPI.
 *
 * @since_tizen 2.3
 */
typedef struct __ckmc_raw_buff {
    unsigned char *data; /**< Byte array containing binary data */
    size_t size; /**< The size of the binary data */
} ckmc_raw_buffer_s;


/**
 * @brief The structure for a policy for storing key/certificate/binary data.
 *
 * @since_tizen 2.3
 *
 * @remarks If @a password is not NULL, the data (or key, or certificate) is stored encrypted
 *          with this password inside key manager.
 * @remarks If @a extractable is equal to true the key may be extracted from storage.
 *
 */
typedef struct __ckmc_policy {
    char *password; /**< Byte array used to encrypt data inside CKM*/
    bool extractable; /**< Flag to decide if the key can be extracted from storage or not */
} ckmc_policy_s;


/**
 * @brief The structure for key used in key manager CAPI.
 *
 * @since_tizen 2.3
 *
 * @remarks @a raw_key may be encrypted with password.
 *
 */
typedef struct __ckmc_key {
    unsigned char
    *raw_key; /**< Byte array of key*/
    size_t key_size; /**< The byte size of raw_key */
    ckmc_key_type_e key_type; /**< The raw_key's type */
    char *password; /**< Byte array used to decrypt data in raw_key inside key manager. */
} ckmc_key_s;


/**
 * @brief The structure for certificate used in key manager CAPI.
 *
 * @since_tizen 2.3
 */
typedef struct __ckmc_cert {
    unsigned char *raw_cert; /**< Byte array of certificate */
    size_t cert_size; /**< Byte size of raw_cert */
    ckmc_data_format_e data_format; /**< Raw_cert's encoding format */
} ckmc_cert_s;


/**
 * @brief The structure for linked list of alias.
 *
 * @since_tizen 2.3
 */
typedef struct __ckmc_alias_list {
    char *alias; /**< The name of key, certificate or data stored in key manager */
    struct __ckmc_alias_list *next; /**< The pointer to the next item in alias list */
} ckmc_alias_list_s;


struct ckmc_alias_info_s;

/**
 * @brief The structure of alias and additional information about it.
 *
 * @since_tizen 5.5
 */
typedef struct ckmc_alias_info_s ckmc_alias_info_s;


/**
 * @brief The structure for linked list of alias with additional information.
 *
 * @since_tizen 5.5
 */
typedef struct __ckmc_alias_info_list_s {
    struct ckmc_alias_info_s* info; /**< The pointer to the alias structure with additional information */
    struct __ckmc_alias_info_list_s* next; /**< The pointer to the next item in alias info list */
} ckmc_alias_info_list_s;


/**
 * @brief The structure for linked list of certificate.
 *
 * @since_tizen 2.3
 */
typedef struct __ckmc_cert_list {
    ckmc_cert_s *cert; /**< The pointer to the certificate */
    struct __ckmc_cert_list *next; /**< The pointer to the next item on certificate list */
} ckmc_cert_list_s;


/**
 * @brief Enumeration for OCSP status.
 *
 * @since_tizen 2.4
 */
typedef enum __ckmc_ocsp_status {
    CKMC_OCSP_STATUS_GOOD = 0, /**< OCSP status is good */
    CKMC_OCSP_STATUS_REVOKED, /**< The certificate is revoked */
    CKMC_OCSP_STATUS_UNKNOWN, /**< Unknown error */
    CKMC_OCSP_ERROR_UNSUPPORTED, /**< The certificate does not provide OCSP extension */
    CKMC_OCSP_ERROR_INVALID_URL, /**< The invalid URL in certificate OCSP extension */
    CKMC_OCSP_ERROR_INVALID_RESPONSE, /**< The invalid response from OCSP server */
    CKMC_OCSP_ERROR_REMOTE, /**< OCSP remote server error */
    CKMC_OCSP_ERROR_NET, /**< Network connection error */
    CKMC_OCSP_ERROR_INTERNAL /**< OpenSSL API error */
} ckmc_ocsp_status_e;


/**
 * @brief The structure for PKCS12 used in key manager CAPI.
 *
 * @since_tizen 2.4
 *
 * @remarks The @a priv_key, @a cert and @a ca_chain may be NULL.
 */
typedef struct __ckmc_pkcs12 {
    ckmc_key_s *priv_key; /**< The private key structure */
    ckmc_cert_s *cert; /**< The certificate structure */
    ckmc_cert_list_s *ca_chain; /**< The chain certificate list structure */
} ckmc_pkcs12_s;


/**
 * @brief Enumeration for crypto algorithm parameters.
 *
 * @since_tizen 3.0
 *
 * @see #ckmc_algo_type_e
 * @see #ckmc_key_s
 * @see #ckmc_kdf_prf_e
 * @see #ckmc_kbkdf_mode_e
 * @see #ckmc_kbkdf_counter_location_e
 * @see #ckmc_kem_type_e
 * @see #__ckmc_hash_algo
 */
typedef enum __ckmc_param_name {
    CKMC_PARAM_ALGO_TYPE = 1, /**< Integer - type of algorithm */

    CKMC_PARAM_ED_IV = 101, /**< Buffer - initialization vector */
    CKMC_PARAM_ED_CTR_LEN, /**< Integer - ctr length in bits */
    CKMC_PARAM_ED_AAD, /**< Buffer - Additional Authentication Data for AES GCM */
    CKMC_PARAM_ED_TAG_LEN, /**< Integer - tag length in bits */
    CKMC_PARAM_ED_LABEL, /**< Buffer - RSA OAEP label (not supported at the moment) */
    CKMC_PARAM_ED_OAEP_HASH, /**< Integer - function to be used both as Label and MGF hash function
                             in OAEP padding. If not given, the default #CKMC_HASH_SHA1 is used (Since 6.0) */
    CKMC_PARAM_ED_PADDING, /**< Integer - type of padding algorithm (Since 10.0) */

    CKMC_PARAM_KDF_PRF = 401, /**< Integer - pseudo-random function number (Since 6.0) */
    CKMC_PARAM_KDF_LEN, /**< Integer - length of the derived key in bytes. The value must be one of
                        {16, 24, 32} (Since 6.0) */

    CKMC_PARAM_KBKDF_MODE, /**< Integer - KDF mode number (Since 6.0) */
    CKMC_PARAM_KBKDF_LABEL, /**< Buffer - the purpose for the derived key. Conflicts with
                            #CKMC_PARAM_KBKDF_FIXED_INPUT (Since 6.0) */
    CKMC_PARAM_KBKDF_CONTEXT, /**< Buffer - information related to the derived key. Conflicts with
                              #CKMC_PARAM_KBKDF_FIXED_INPUT (Since 6.0) */
    CKMC_PARAM_KBKDF_FIXED_INPUT, /**< Buffer - KBKDF fixed input replacing context and label
                                   (Since 6.0). Conflicts with:
                                  - #CKMC_PARAM_KBKDF_LABEL,
                                  - #CKMC_PARAM_KBKDF_CONTEXT,
                                  - #CKMC_PARAM_KBKDF_LLEN,
                                  - #CKMC_PARAM_KBKDF_NO_SEPARATOR */
    CKMC_PARAM_KBKDF_COUNTER_LOCATION, /**< Integer - specifies location of the counter in KBKDF (Since 6.0) */
    CKMC_PARAM_KBKDF_RLEN, /**< Integer - specifies the length of the counter representation in bits
                           in KBKDF. The value must be one of {8, 16, 24, 32}. If not set, the
                           default value = 32 will be used. (Since 6.0) */
    CKMC_PARAM_KBKDF_LLEN, /**< Integer - specifies the extent of the length suffix representation
                           in bits in KBKDF. The value must be one of {0, 8, 16, 24, 32}. If set to
                           0 the length suffix will be skipped. If not set, the default value = 32
                           will be used. The length suffix is skipped if
                           #CKMC_PARAM_KBKDF_FIXED_INPUT is passed and this parameter conflicts
                           with it. (Since 6.0) */
    CKMC_PARAM_KBKDF_NO_SEPARATOR, /**< Integer - presence of this parameter will skip the zero
                                   octet separator between label and context in KBKDF.
                                   All values are allowed. This parameter conflicts with
                                   #CKMC_PARAM_KBKDF_FIXED_INPUT. (Since 6.0) */

    CKMC_PARAM_ECDH_PUBKEY, /**< Buffer - EC public key in DER form (Since 6.0) */

    CKMC_PARAM_KEM_TYPE, /**< Integer - specifies the KEM type (Since 7.0) */
} ckmc_param_name_e;

/**
 * @brief Enumeration for key derivation function pseudo-random function parameter.
 *
 * @since_tizen 6.0
 *
 * @see ckmc_key_derive()
 * @see #ckmc_param_name_e
 */
typedef enum __ckmc_kdf_prf {
    CKMC_KDF_PRF_HMAC_SHA256 = 1, /**< HMAC SHA256 */
    CKMC_KDF_PRF_HMAC_SHA384, /**< HMAC SHA384 */
    CKMC_KDF_PRF_HMAC_SHA512, /**< HMAC SHA512 */
} ckmc_kdf_prf_e;

/**
 * @brief Enumeration for key based key derivation function mode.
 *
 * @since_tizen 6.0
 *
 * @see ckmc_key_derive()
 * @see #ckmc_param_name_e
 */
typedef enum __ckmc_kbkdf_mode {
	CKMC_KBKDF_MODE_COUNTER = 1, /**< KBKDF counter mode */
} ckmc_kbkdf_mode_e;

/**
 * @brief Enumeration for KBKDF counter location relative to fixed input.
 *
 * @since_tizen 6.0
 *
 * @see ckmc_key_derive()
 * @see #ckmc_param_name_e
 */
typedef enum __ckmc_kbkdf_counter_location {
    CKMC_KBKDF_COUNTER_BEFORE_FIXED = 1, /**< Counter is located before fixed input */
    CKMC_KBKDF_COUNTER_AFTER_FIXED, /**< Counter is located after fixed input */
    CKMC_KBKDF_COUNTER_MIDDLE_FIXED, /**< Counter is located in the middle of the fixed input
                                     (between context and label). This setting requires setting
                                     #CKMC_PARAM_KBKDF_LABEL and #CKMC_PARAM_KBKDF_CONTEXT and
                                     conflicts with #CKMC_PARAM_KBKDF_FIXED_INPUT. If this location
                                     is used the separator will be skipped regardless of the
                                     #CKMC_PARAM_KBKDF_NO_SEPARATOR parameter */
} ckmc_kbkdf_counter_location_e;

/**
 * @brief Algorithm parameter list handle.
 *
 * @since_tizen 3.0
 *
 * @remarks Each parameter list must have at least one #CKMC_PARAM_ALGO_TYPE parameter that
 *          identifies the algorithm. See #ckmc_algo_type_e for available algorithms and additional
 *          parameters they support.
 *
 * @see ckmc_generate_new_params()
 * @see ckmc_param_list_new()
 * @see ckmc_param_list_set_integer()
 * @see ckmc_param_list_set_buffer()
 * @see ckmc_param_list_get_integer()
 * @see ckmc_param_list_get_buffer()
 * @see ckmc_param_list_free()
 * @see #ckmc_algo_type_e
 * @see #ckmc_param_name_e
 */
typedef struct __ckmc_param_list *ckmc_param_list_h;


/**
 * @brief Enumeration for crypto algorithm types.
 *
 * @since_tizen 3.0
 *
 * @see #ckmc_param_name_e
 * @see #ckmc_kdf_prf_e
 * @see #ckmc_kbkdf_mode_e
 * @see #ckmc_kbkdf_counter_location_e
 * @see #ckmc_key_s
 * @see #ckmc_kem_type_e
 * @see #__ckmc_hash_algo
 */
typedef enum __ckmc_algo_type {
    CKMC_ALGO_AES_CTR = 1, /**< AES-CTR algorithm
                           Supported parameters:
                           - #CKMC_PARAM_ALGO_TYPE = #CKMC_ALGO_AES_CTR (mandatory),
                           - #CKMC_PARAM_ED_IV = 16-byte initialization vector (mandatory),
                           - #CKMC_PARAM_ED_CTR_LEN = length of counter block in bits
                           (optional, only 128b is supported at the moment) */

    CKMC_ALGO_AES_CBC, /**< AES-CBC algorithm
                       Supported parameters:
                       - #CKMC_PARAM_ALGO_TYPE = #CKMC_ALGO_AES_CBC (mandatory),
                       - #CKMC_PARAM_ED_IV = 16-byte initialization vector (mandatory),
                       - #CKMC_PARAM_ED_PADDING = type of padding algorithm to be used (optional,
                       if not given, the default #CKMC_AES_PADDING_PKCS7 is used) (Since 10.0) */

    CKMC_ALGO_AES_GCM, /**< AES-GCM algorithm
                       Supported parameters:
                       - #CKMC_PARAM_ALGO_TYPE = #CKMC_ALGO_AES_GCM (mandatory),
                       - #CKMC_PARAM_ED_IV = 1 to (2^64-1) bytes long initialization vector.
                       Note that the backend may impose additional limit on the maximum size
                       (ckmc_backend_get_max_chunk_size()). Recommended length is 12B (mandatory),
                       - #CKMC_PARAM_ED_TAG_LEN = GCM tag length in bits. One of
                       {32, 64, 96, 104, 112, 120, 128} (optional, if not present, the
                       length 128 is used; (Since 5.0) if TrustZone backend is used,
                       32 and 64 lengths are not supported),
                       - #CKMC_PARAM_ED_AAD = additional authentication data. Note that the backend
                       may impose limit on the maximum size (ckmc_backend_get_max_chunk_size())
                       (optional) */

    CKMC_ALGO_AES_CFB, /**< AES-CFB algorithm
                       Supported parameters:
                       - #CKMC_PARAM_ALGO_TYPE = #CKMC_ALGO_AES_CFB (mandatory),
                       - #CKMC_PARAM_ED_IV = 16-byte initialization vector (mandatory) */

    CKMC_ALGO_RSA_OAEP, /**< RSA-OAEP algorithm (EME-OAEP as defined in PKCS #1 with MGF1)
                        Supported parameters:
                        - #CKMC_PARAM_ALGO_TYPE = #CKMC_ALGO_RSA_OAEP (mandatory),
                        - #CKMC_PARAM_ED_LABEL = label (encoding parameter) to be associated with
                        the message (optional, not supported at the moment)
                        - #CKMC_PARAM_ED_OAEP_HASH = hash algorithm to be used in OAEP padding (optional) */

    CKMC_ALGO_KBKDF, /**< Key based key derivation algorithm
                     Supported parameters:
                     - #CKMC_PARAM_ALGO_TYPE = #CKMC_ALGO_KBKDF (mandatory),
                     - #CKMC_PARAM_KDF_PRF = pseudo-random function (mandatory),
                     - #CKMC_PARAM_KBKDF_MODE = KDF mode (mandatory),
                     - #CKMC_PARAM_KBKDF_LABEL = the purpose for the derived key (optional),
                     - #CKMC_PARAM_KBKDF_CONTEXT = information related to the derived key
                     (optional),
                     - #CKMC_PARAM_KDF_LEN = length of the derived key (mandatory)
                     - #CKMC_PARAM_KBKDF_FIXED_INPUT = replacement for context and label (optional),
                     - #CKMC_PARAM_KBKDF_COUNTER_LOCATION = counter location (mandatory),
                     - #CKMC_PARAM_KBKDF_RLEN = length of the counter representation (optional),
                     - #CKMC_PARAM_KBKDF_LLEN = length of the length suffix representation
                     (optional),
                     - #CKMC_PARAM_KBKDF_NO_SEPARATOR = existence of zero separator (optional) */

    CKMC_ALGO_ECDH, /**< ECDH shared secret key agreement protocol
                     Supported parameters (all are required):
                     - #CKMC_PARAM_ALGO_TYPE = #CKMC_ALGO_ECDH,
                     - #CKMC_PARAM_ECDH_PUBKEY = peer's public key */

    CKMC_ALGO_KEM, /**< KEM algorithm
                   Supported parameters:
                   - #CKMC_PARAM_ALGO_TYPE = #CKMC_ALGO_KEM (mandatory),
                   - #CKMC_PARAM_KEM_TYPE = the type of KEM (mandatory) (Since 7.0) */
} ckmc_algo_type_e;

/**
 * @brief Enumeration for backend identifiers.
 *
 * @since_tizen 6.0
 *
 * @see ckmc_get_backend_info()
 * @see ckmc_alias_info_get_backend()
 */
typedef enum __ckmc_backend_id {
    CKMC_BACKEND_SW = 0, /**< Software backend */
    CKMC_BACKEND_TZ /**< TrustZone backend */
} ckmc_backend_id_e;

/**
 * @brief Backend information handle.
 *
 * @since_tizen 6.0
 *
 * @see ckmc_alias_info_get_backend()
 * @see ckmc_get_backend_info()
 * @see ckmc_backend_get_max_chunk_size()
 * @see ckmc_backend_info_free()
 */
typedef struct __ckmc_backend_info_s *ckmc_backend_info_h;

/**
 * @brief Encryption/decryption context handle.
 *
 * @since_tizen 6.0
 *
 * @see ckmc_cipher_initialize()
 * @see ckmc_cipher_update()
 * @see ckmc_cipher_finalize()
 * @see ckmc_cipher_free()
 */
typedef struct __ckmc_cipher_ctx *ckmc_cipher_ctx_h;

/**
 * @brief Gets the alias from #ckmc_alias_info_s structure.
 *
 * @since_tizen 5.5
 *
 * @remarks The @a alias should not be released.
 *          The @a alias cannot be used until #ckmc_alias_info_s is released.
 *
 * @param[in] info The pointer to the #ckmc_alias_info_s structure
 * @param[out] alias The pointer to the alias
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 *
 * @see #ckmc_alias_info_s
 */
int ckmc_alias_info_get_alias(const ckmc_alias_info_s* info,
                              char** alias);

/**
 * @brief Gets the password protection status from #ckmc_alias_info_s structure.
 *
 * @since_tizen 5.5
 *
 * @param[in] info The pointer to the #ckmc_alias_info_s structure
 * @param[out] is_password_protected The pointer to the password protection flag
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 *
 * @see #ckmc_alias_info_s
 */
int ckmc_alias_info_is_password_protected(const ckmc_alias_info_s* info,
                                          bool* is_password_protected);

/**
 * @brief Gets the backend identifier from #ckmc_alias_info_s structure.
 *
 * @since_tizen 6.0
 *
 * @param[in] info The pointer to the #ckmc_alias_info_s structure
 * @param[out] backend The pointer to the backend identifier
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 *
 * @see #ckmc_backend_id_e
 * @see #ckmc_alias_info_s
 * @see ckmc_get_backend_info()
 * @see ckmc_backend_info_free()
 * @see ckmc_backend_get_max_chunk_size()
 */
int ckmc_alias_info_get_backend(const ckmc_alias_info_s* info,
                                ckmc_backend_id_e* backend);

/**
 * @brief Destroys the #ckmc_alias_info_list_s handle and releases resources of
 *                 #ckmc_alias_info_list_s from the provided first handle cascadingly.
 *
 * @since_tizen 5.5
 *
 * @param[in] first The first #ckmc_alias_info_list_s handle to destroy
 *
 * @see #ckmc_alias_info_list_s
 */
void ckmc_alias_info_list_all_free(ckmc_alias_info_list_s *first);

/**
 * @brief Creates a new full alias which is a concatenation of @a owner_id and @a alias.
 *
 * @since_tizen 3.0
 *
 * @remarks @a full_alias should be freed with free() after use.
 * @remarks @a owner_id should be package id if data owner is application. If you want to access
 *             data stored by system services, it should be #ckmc_owner_id_system.
 *
 * @param[in] owner_id Data owner's id
 * @param[in] alias Data alias
 * @param[out] full_alias The newly created alias which is a concatenation of
 *                        @a owner_id, #ckmc_owner_id_separator and @a alias
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid or NULL, or @a owner_id is empty.
 * @retval #CKMC_ERROR_OUT_OF_MEMORY Not enough memory
 *
 * @see #ckmc_owner_id_separator
 * @see #ckmc_owner_id_system
 */
int ckmc_alias_new(const char *owner_id,
                   const char *alias,
                   char **full_alias);


/**
 * @brief Creates a new #ckmc_key_s handle and returns it.
 *
 * @since_tizen 2.4
 *
 * @remarks The newly created key must be destroyed by calling ckmc_key_free() if it is no
 *          longer needed.
 * @remarks The @a raw_key may be encrypted with password.
 * @remarks If @a raw_key is not encrypted, @a password can be NULL.
 *
 * @param[in] raw_key The byte array of key
 * @param[in] key_size The byte size of @a raw_key
 * @param[in] key_type The type of @a raw_key
 * @param[in] password The byte array used to decrypt @a raw_key inside key manager
 * @param[out] ppkey The pointer to a newly created handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_OUT_OF_MEMORY Not enough memory
 *
 * @see ckmc_key_free()
 * @see #ckmc_key_s
 */
int ckmc_key_new(unsigned char *raw_key,
                 size_t key_size,
                 ckmc_key_type_e key_type,
                 char *password,
                 ckmc_key_s **ppkey);


/**
 * @brief Destroys the #ckmc_key_s handle and releases all its resources.
 *
 * @since_tizen 2.3
 *
 * @param[in] key The #ckmc_key_s handle to destroy
 */
void ckmc_key_free(ckmc_key_s *key);


/**
 * @brief Creates a new #ckmc_raw_buffer_s handle and returns it.
 *
 * @since_tizen 2.4
 *
 * @remarks The newly created #ckmc_raw_buffer_s must be destroyed by calling ckmc_buffer_free() if
 *          it is no longer needed.
 *
 * @param[in] data The byte array of buffer
 * @param[in] size The byte size of buffer
 * @param[out] ppbuffer The pointer to a newly created handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_OUT_OF_MEMORY Not enough memory
 *
 * @see ckmc_buffer_free()
 * @see #ckmc_raw_buffer_s
 */
int ckmc_buffer_new(unsigned char *data,
                    size_t size,
                    ckmc_raw_buffer_s **ppbuffer);


/**
 * @brief Destroys the #ckmc_raw_buffer_s handle and releases all its resources.
 *
 * @since_tizen 2.3
 *
 * @param[in] buffer The #ckmc_raw_buffer_s structure to destroy
 */
void ckmc_buffer_free(ckmc_raw_buffer_s *buffer);


/**
 * @brief Creates a new #ckmc_cert_s handle and returns it.
 *
 * @since_tizen 2.4
 *
 * @remarks The newly created #ckmc_cert_s must be destroyed by calling ckmc_cert_free() if it is no
 *          longer needed.
 *
 * @param[in] raw_cert The byte array of certificate
 * @param[in] cert_size The byte size of raw_cert
 * @param[in] data_format The encoding format of raw_cert
 * @param[out] ppcert The pointer to a newly created handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_OUT_OF_MEMORY Not enough memory
 *
 * @see ckmc_cert_free()
 * @see ckmc_load_cert_from_file()
 * @see #ckmc_cert_s
 */
int ckmc_cert_new(unsigned char *raw_cert,
                  size_t cert_size,
                  ckmc_data_format_e data_format,
                  ckmc_cert_s **ppcert);


/**
 * @brief Destroys the #ckmc_cert_s handle and releases all its resources.
 *
 * @since_tizen 2.3
 *
 * @param[in] cert The #ckmc_cert_s handle to destroy
 *
 * @see ckmc_load_cert_from_file()
 */
void ckmc_cert_free(ckmc_cert_s *cert);


/**
 * @brief Creates a new #ckmc_cert_s handle from a given file and returns it.
 *
 * @since_tizen 2.3
 *
 * @remarks The newly created #ckmc_cert_s must be destroyed by calling ckmc_cert_free() if it is no
 *          longer needed.
 * @remarks Only DER or PEM encoded certificate file is supported in @a file_path.
 *
 * @param[in] file_path The path of certificate file to be loaded
 * @param[out] cert The pointer to a newly created handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_OUT_OF_MEMORY Not enough memory space
 * @retval #CKMC_ERROR_INVALID_FORMAT Invalid certificate file format
 * @retval #CKMC_ERROR_FILE_ACCESS_DENIED Provided file does not exist or cannot be accessed
 *
 * @see ckmc_cert_free()
 * @see #ckmc_cert_s
 */
int ckmc_load_cert_from_file(const char *file_path,
                             ckmc_cert_s **cert);


/**
 * @brief Creates a new #ckmc_pkcs12_s handle and returns it.
 *
 * @since_tizen 2.4
 *
 * @remarks The newly created #ckmc_pkcs12_s must be destroyed by calling ckmc_pkcs12_free() if it is
 *          no longer needed.
 * @remarks On success, private_key, cert && ca_cert_list ownership is transferred into newly
 *          returned @a pkcs12_bundle.
 *
 * @param[in] private_key Optional handle to the private key
 * @param[in] cert Optional handle to the certificate
 * @param[in] ca_cert_list Optional list of chain certificate handles
 * @param[out] pkcs12_bundle The pointer to a newly created handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid or @a private_key, @a cert and
 *                                       @a ca_cert_list all are NULL
 * @retval #CKMC_ERROR_OUT_OF_MEMORY Not enough memory
 *
 * @see ckmc_pkcs12_free()
 * @see ckmc_pkcs12_load()
 * @see #ckmc_key_s
 * @see #ckmc_cert_s
 * @see #ckmc_cert_list_s
 * @see #ckmc_pkcs12_s
 */
int ckmc_pkcs12_new(ckmc_key_s *private_key,
                    ckmc_cert_s *cert,
                    ckmc_cert_list_s *ca_cert_list,
                    ckmc_pkcs12_s **pkcs12_bundle);


/**
 * @deprecated Deprecated since 2.4
 *             [Use ckmc_pkcs12_load() instead]
 *
 * @brief Creates a new #ckmc_key_s (@a private_key), #ckmc_cert_s (@a cert),
 *        and #ckmc_cert_list_s (@a ca_cert_list) handle from a given PKCS#12 file and returns them.
 *
 * @since_tizen 2.3
 *
 * @remarks The newly created @a private_key, @a cert and @a ca_cert_list must be destroyed
 *          by calling ckmc_key_free(), ckmc_cert_free(), and ckmc_cert_list_all_free() if they are
 *          no longer needed.
 * @remarks If PKCS12 file is not encrypted, @a passphrase can be NULL.
 * @remarks @a ca_cert_list will be NULL if the PKCS12 file does not contain a certificate.
 *
 * @param[in] file_path The path of PKCS12 file to be loaded
 * @param[in] passphrase The passphrase used to decrypt the PCKS12 file
 * @param[out] private_key The pointer to a newly created private key
 * @param[out] cert The pointer to a newly created handle certificate
 * @param[out] ca_cert_list The pointer to a newly created handle for CA certificate
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_OUT_OF_MEMORY Not enough memory space
 * @retval #CKMC_ERROR_INVALID_FORMAT Invalid PKCS12 file format
 * @retval #CKMC_ERROR_FILE_ACCESS_DENIED Provided file does not exist or cannot be accessed
 *
 * @see ckmc_pkcs12_new()
 * @see ckmc_pkcs12_load()
 * @see ckmc_key_free()
 * @see ckmc_cert_free()
 * @see ckmc_cert_list_all_free()
 * @see #ckmc_key_s
 * @see #ckmc_cert_s
 * @see #ckmc_cert_list_s
 */
int ckmc_load_from_pkcs12_file(const char *file_path,
                               const char *passphrase,
                               ckmc_key_s **private_key,
                               ckmc_cert_s **cert,
                               ckmc_cert_list_s **ca_cert_list) TIZEN_DEPRECATED_API;


/**
 * @brief Creates a new #ckmc_pkcs12_s handle from a given PKCS#12 file and returns it.
 *
 * @since_tizen 2.4
 *
 * @remarks The newly created #ckmc_pkcs12_s must be destroyed by calling ckmc_pkcs12_free() if it
 *          is no longer needed.
 * @remarks If PKCS12 file is not encrypted, @a passphrase can be NULL.
 * @remarks (*@a pkcs12_bundle)->ca_cert_list will be NULL if the PKCS12 file does not contain a certificate.
 *
 * @param[in] file_path The path of PKCS12 file to be loaded
 * @param[in] passphrase The passphrase used to decrypt the PCKS12 file
 * @param[out] pkcs12_bundle The pointer of newly created handle for CA certificate
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_OUT_OF_MEMORY Not enough memory space
 * @retval #CKMC_ERROR_INVALID_FORMAT Invalid PKCS12 file format
 * @retval #CKMC_ERROR_FILE_ACCESS_DENIED Provided file does not exist or cannot be accessed
 *
 * @see ckmc_pkcs12_free()
 * @see #ckmc_pkcs12_s
 */
int ckmc_pkcs12_load(const char *file_path,
                     const char *passphrase,
                     ckmc_pkcs12_s **pkcs12_bundle);


/**
 * @brief Destroys the #ckmc_pkcs12_s handle and releases all its resources.
 *
 * @since_tizen 2.4
 *
 * @param[in] pkcs12 The #ckmc_pkcs12_s handle to destroy
 *
 * @see ckmc_pkcs12_new()
 * @see ckmc_pkcs12_load()
 */
void ckmc_pkcs12_free(ckmc_pkcs12_s *pkcs12);


/**
 * @brief Creates a new alias list handle and returns it. The alias pointer in the returned
 *        alias list handle points to the provided characters and next is NULL.
 *
 * @since_tizen 2.4
 *
 * @remarks The newly created alias list must be destroyed by calling ckmc_alias_list_free()
 *          or ckmc_alias_list_all_free() if it is no longer needed.
 *
 * @param[in] alias The first item to be set in the newly created alias list
 * @param[out] ppalias_list The pointer to a newly created alias list handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_OUT_OF_MEMORY Not enough memory
 *
 * @see ckmc_alias_list_all_free()
 * @see #ckmc_alias_list_s
 */
int ckmc_alias_list_new(char *alias,
                        ckmc_alias_list_s **ppalias_list);


/**
 * @brief Creates a new alias list handle, adds it to a previous alias list and
 *        returns it. The alias pointer in the returned alias list handle points to the
 *        provided characters and next is NULL.
 *
 * @since_tizen 2.4
 *
 * @remarks The newly created @a pplast must be destroyed using ckmc_alias_list_free().
 *
 * @param[in] previous The last alias list handle to which a newly created alias list is added
 * @param[in] alias The item to be set in the newly created alias list
 * @param[out] pplast The pointer to a newly created and added alias list handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_OUT_OF_MEMORY Not enough memory
 *
 * @see ckmc_alias_list_all_free()
 * @see #ckmc_alias_list_s
 */
int ckmc_alias_list_add(ckmc_alias_list_s *previous,
                        char *alias,
                        ckmc_alias_list_s **pplast);


/**
 * @brief Destroys the alias list handle and releases resources of alias list from
 *        the provided first handle cascadingly.
 *
 * @since_tizen 2.4
 *
 * @remarks It does not destroy an alias itself in alias list.
 *
 * @param[in] first The first alias list handle to destroy
 *
 * @see ckmc_alias_list_all_free()
 * @see #ckmc_alias_list_s
 */
void ckmc_alias_list_free(ckmc_alias_list_s *first);


/**
 * @brief Destroys the alias list handle and releases all its resources from the provided
 *        first handle cascadingly.
 *
 * @since_tizen 2.4
 *
 * @remarks It also destroys the alias in alias list.
 *
 * @param[in] first The first alias list handle to destroy
 *
 * @see #ckmc_alias_list_s
 */
void ckmc_alias_list_all_free(ckmc_alias_list_s *first);


/**
 * @brief Creates a new certificate list handle and returns it. The cert pointer in the returned
 *        certificate list handle points to the provided certificate and next is NULL.
 *
 * @since_tizen 2.4
 *
 * @remarks The newly created certificate list must be destroyed by calling ckmc_cert_list_free() or
 *          ckmc_cert_list_all_free() if it is no longer needed.
 *
 * @param[in] cert The first item to be set in the newly created certificate list
 * @param[out] ppalias_list The pointer to a newly created alias list handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_OUT_OF_MEMORY Not enough memory
 *
 * @see ckmc_cert_list_all_free()
 * @see #ckmc_cert_list_s
 */
int ckmc_cert_list_new(ckmc_cert_s *cert,
                       ckmc_cert_list_s **ppalias_list);


/**
 * @brief Creates a new certificate list handle, adds it to a previous certificate list and
 *        returns it. The cert pointer in the returned alias list handle points to the
 *        provided certificate and next is NULL.
 *
 * @since_tizen 2.4
 *
 * @remarks The newly created @a pplast must be destroyed using ckmc_cert_list_free().
 *
 * @param[in] previous The last certificate list handle to which a newly created certificate list
 *                     is added
 * @param[in] cert The item to be set in the newly created certificate list
 * @param[out] pplast The pointer to a newly created and added alias list handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #CKMC_ERROR_OUT_OF_MEMORY Not enough memory
 *
 * @see ckmc_cert_list_all_free()
 * @see #ckmc_cert_list_s
 */
int ckmc_cert_list_add(ckmc_cert_list_s *previous,
                       ckmc_cert_s *cert,
                       ckmc_cert_list_s **pplast);


/**
 * @brief Destroys the certificate list handle and releases resources of certificate list from the
 *        provided first handle cascadingly.
 *
 * @since_tizen 2.4
 *
 * @remarks It does not destroy certificate itself in certificate list.
 *
 * @param[in] first The first certificate list handle to destroy
 *
 * @see ckmc_cert_list_all_free()
 * @see #ckmc_cert_list_s
 */
void ckmc_cert_list_free(ckmc_cert_list_s *first);


/**
 * @brief Destroys the certificate list handle and releases all its resources from the provided
 *        first handle cascadingly.
 *
 * @since_tizen 2.3
 *
 * @remarks It also destroys certificate in certificate list.
 *
 * @param[in] first The first certificate list handle to destroy
 *
 * @see #ckmc_cert_list_s
 */
void ckmc_cert_list_all_free(ckmc_cert_list_s *first);


/**
 * @brief Creates new param list.
 *
 * @since_tizen 3.0
 *
 * @remarks Caller is responsible for freeing it with ckmc_param_list_free().
 *
 * @param[in] pparams Double pointer to the handle of param list to which the newly created
 *                    algorithm param list will be assigned
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 *
 * @see ckmc_param_list_set_integer()
 * @see ckmc_param_list_set_buffer()
 * @see ckmc_param_list_free()
 * @see ckmc_generate_new_params()
 * @see #ckmc_param_list_h
 * @see #ckmc_param_name_e
 * @see #ckmc_algo_type_e
 */
int ckmc_param_list_new(ckmc_param_list_h *pparams);


/**
 * @brief Sets integer parameter to the list.
 *
 * @since_tizen 3.0
 *
 * @remarks Caller is responsible for param list creation.
 * @remarks New param with @a name and @a value will be set in @a params.
 * @remarks Existing parameter will be overwritten by value passed in @a name.
 *          Passing invalid parameter name will result in an error.
 *
 * @param[in] params Algorithm param list handle created with ckmc_param_list_new() or
 *                   ckmc_generate_new_params()
 * @param[in] name Name of parameter to set
 * @param[in] value Value of the parameter in form of a integer
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 *
 * @see ckmc_param_list_new()
 * @see ckmc_param_list_set_buffer()
 * @see ckmc_param_list_get_integer()
 * @see ckmc_param_list_get_buffer()
 * @see ckmc_param_list_free()
 * @see ckmc_generate_new_params()
 * @see #ckmc_param_list_h
 * @see #ckmc_param_name_e
 * @see #ckmc_algo_type_e
 */
int ckmc_param_list_set_integer(ckmc_param_list_h params,
                                ckmc_param_name_e name,
                                uint64_t value);


/**
 * @brief Sets buffer parameter to the list.
 *
 * @since_tizen 3.0
 *
 * @remarks Caller is responsible for param list creation.
 * @remarks New param with @a name and @a buffer will be set in @a params.
 * @remarks Existing parameter will be overwritten by value passed in @a name.
 *          Passing invalid parameter name will result in an error.
 * @remarks Caller is responsible for creating and freeing the @a buffer.
 *
 * @param[in] params Algorithm param list handle created with ckmc_param_list_new() or
 *                   ckmc_generate_new_params()
 * @param[in] name Name of parameter to set
 * @param[in] buffer Value of the parameter in form of a buffer
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 *
 * @warning When using CTR, GCM or CFB mode algorithms (CKMC_ALGO_AES_CTR, CKMC_ALGO_AES_GCM,
 *          CKMC_ALGO_AES_CFB), the IV (Initialization Vector, CKMC_PARAM_ED_IV) must not be reused
 *          with the same key. IV reuse in CTR, GCM or CFB mode can expose plaintext. Each
 *          encryption operation must use a random IV generated for that specific operation.
 *          Failing to follow this requirement can result in critical security vulnerabilities.
 *
 * @see ckmc_param_list_new()
 * @see ckmc_param_list_set_integer()
 * @see ckmc_param_list_get_integer()
 * @see ckmc_param_list_get_buffer()
 * @see ckmc_param_list_free()
 * @see ckmc_generate_new_params()
 * @see #ckmc_param_list_h
 * @see #ckmc_param_name_e
 * @see #ckmc_algo_type_e
 */
int ckmc_param_list_set_buffer(ckmc_param_list_h params,
                               ckmc_param_name_e name,
                               const ckmc_raw_buffer_s *buffer);


/**
 * @brief Gets integer parameter from the list.
 *
 * @since_tizen 3.0
 *
 * @remarks Caller is responsible for param list creation.
 * @remarks New param with @a name will be set in @a params.
 *
 * @param[in] params Algorithm param list handle created with ckmc_param_list_new() or
 *                   ckmc_generate_new_params()
 * @param[in] name Name of parameter to get
 * @param[out] pvalue Value of the parameter in form of a integer
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 *
 * @see ckmc_param_list_new()
 * @see ckmc_param_list_set_integer()
 * @see ckmc_param_list_set_buffer()
 * @see ckmc_param_list_get_buffer()
 * @see ckmc_param_list_free()
 * @see ckmc_generate_new_params()
 * @see #ckmc_param_list_h
 * @see #ckmc_param_name_e
 * @see #ckmc_algo_type_e
 */
int ckmc_param_list_get_integer(ckmc_param_list_h params,
                                ckmc_param_name_e name,
                                uint64_t *pvalue);


/**
 * @brief Gets buffer parameter from the list.
 *
 * @since_tizen 3.0
 *
 * @remarks Caller is responsible for param list creation.
 * @remarks New param with @a name will be set in @a params.
 * @remarks The @a ppbuffer must be destroyed using ckmc_buffer_free().
 *
 * @param[in] params Algorithm param list handle created with ckmc_param_list_new() or
 *                   ckmc_generate_new_params()
 * @param[in] name Name of parameter to get
 * @param[out] ppbuffer Value of the parameter in form of a buffer
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 *
 * @see ckmc_param_list_new()
 * @see ckmc_param_list_set_integer()
 * @see ckmc_param_list_set_buffer()
 * @see ckmc_param_list_get_integer()
 * @see ckmc_param_list_free()
 * @see ckmc_generate_new_params()
 * @see ckmc_buffer_free()
 * @see #ckmc_param_list_h
 * @see #ckmc_param_name_e
 * @see #ckmc_algo_type_e
 */
int ckmc_param_list_get_buffer(ckmc_param_list_h params,
                               ckmc_param_name_e name,
                               ckmc_raw_buffer_s **ppbuffer);


/**
 * @brief Frees previously allocated list of algorithm params.
 *
 * @since_tizen 3.0
 *
 * @param[in] params First element of the list to be freed
 *
 * @see ckmc_param_list_new()
 * @see ckmc_param_list_set_integer()
 * @see ckmc_param_list_set_buffer()
 * @see ckmc_param_list_get_integer()
 * @see ckmc_param_list_get_buffer()
 * @see ckmc_generate_new_params()
 * @see #ckmc_param_list_h
 * @see #ckmc_param_name_e
 * @see #ckmc_algo_type_e
 */
void ckmc_param_list_free(ckmc_param_list_h params);


/**
 * @brief Generates algorithm parameters for a given algorithm type and set them to the list.
 *
 * @since_tizen 3.0
 *
 * @remarks Caller is responsible for param list destruction.
 * @remarks Algorithm parameters are set to default values. Optional fields are left empty.
 *          Initialization vectors are left empty (they have to be set manually).
 *          Caller is responsible for freeing the list with ckmc_param_list_free().
 * @remarks If the function returns error, provided param list may contain some of default
 *          parameters.
 * @remarks @a pparams should be freed by caller after use
 *
 * @param[in] type Type of the algorithm
 * @param[out] pparams Newly generated handle of param list
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid
 *
 * @see ckmc_param_list_new()
 * @see ckmc_param_list_set_integer()
 * @see ckmc_param_list_set_buffer()
 * @see ckmc_param_list_get_integer()
 * @see ckmc_param_list_get_buffer()
 * @see ckmc_param_list_free()
 * @see #ckmc_param_list_h
 * @see #ckmc_param_name_e
 * @see #ckmc_algo_type_e
 */
int ckmc_generate_new_params(ckmc_algo_type_e type,
                             ckmc_param_list_h *pparams);

/**
 * @brief Retrieves maximum data chunk size in bytes that can be passed to given backend. This is
 *        the maximum size of data passed as encryption/decryption input, AAD or IV.
 *
 * @since_tizen 6.0
 *
 * @remarks Chunk size set in @a size is equal to 0 if there's no backend specific limitation beside
 *          available memory.
 *
 * @param[in] info Backend info handle
 * @param[out] size Maximum chunk size
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CKMC_ERROR_NONE Successful
 * @retval #CKMC_ERROR_INVALID_PARAMETER Input parameter is invalid (@a info is invalid, @a size = NULL)
 *
 * @see #ckmc_backend_info_h
 * @see ckmc_get_backend_info()
 */
int ckmc_backend_get_max_chunk_size(const ckmc_backend_info_h info,
                                    size_t* size);

/**
 * @brief Destroys the backend information handle and releases all its resources.
 *
 * @since_tizen 6.0
 *
 * @param[in] info Backend information handle created with ckmc_get_backend_info()
 *
 * @see #ckmc_backend_info_h
 * @see ckmc_get_backend_info()
 * @see ckmc_backend_get_max_chunk_size()
 */
void ckmc_backend_info_free(ckmc_backend_info_h info);


/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif /* __TIZEN_CORE_CKMC_TYPE_H__ */
