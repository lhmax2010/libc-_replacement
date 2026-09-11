/*
 *  Copyright (c) 2023 - 2024 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 *
 *
 * @file    webauthn-types.h
 * @version 1.0
 * @brief   WebAuthn module types
*/
#ifndef __TIZEN_CORE_WEBAUTHN_TYPES_H__
#define __TIZEN_CORE_WEBAUTHN_TYPES_H__

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include <tizen_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_WEBAUTHN_MODULE
 * @{
 */

/**
 * @brief Current API version.
 * @since_tizen 7.0
 */
#define WAUTHN_API_VERSION_NUMBER                                   0x00000001

/**
 * @brief WebAuthn Errors.
 * @since_tizen 7.0
 */
typedef enum {
    WAUTHN_ERROR_NONE                       = TIZEN_ERROR_NONE,  /**< Successful */
    WAUTHN_ERROR_UNKNOWN                    = TIZEN_ERROR_UNKNOWN, /**< Unknown error */
    WAUTHN_ERROR_INVALID_PARAMETER          = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid function parameter */
    WAUTHN_ERROR_PERMISSION_DENIED          = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
    WAUTHN_ERROR_NOT_SUPPORTED              = TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported operation */
    WAUTHN_ERROR_OUT_OF_MEMORY              = TIZEN_ERROR_OUT_OF_MEMORY, /**< Memory error */
    WAUTHN_ERROR_CANCELED                   = TIZEN_ERROR_CANCELED, /**< Canceled by cancel request */
    WAUTHN_ERROR_TIMED_OUT                  = TIZEN_ERROR_TIMED_OUT, /**< Timeout */
    WAUTHN_ERROR_CONNECTION_REFUSED         = TIZEN_ERROR_CONNECTION_REFUSED, /**< Authenticator is uncontactable */
    WAUTHN_ERROR_NONE_AND_WAIT              = TIZEN_ERROR_WAUTHN | 0x01, /**< Successful and needs to wait for other result*/
    WAUTHN_ERROR_NOT_ALLOWED                = TIZEN_ERROR_WAUTHN | 0x02, /**< Not allowed in the current context */
    WAUTHN_ERROR_INVALID_STATE              = TIZEN_ERROR_WAUTHN | 0x03, /**< Invalid State */
    WAUTHN_ERROR_ENCODING_FAILED            = TIZEN_ERROR_WAUTHN | 0x04, /**< Encoding operation failed */
    WAUTHN_ERROR_SOCKET                     = TIZEN_ERROR_WAUTHN | 0x05, /**< Socket error */
    WAUTHN_ERROR_NO_SUCH_SERVICE            = TIZEN_ERROR_WAUTHN | 0x06, /**< Socket operation on non-socket error */
    WAUTHN_ERROR_ACCESS_DENIED              = TIZEN_ERROR_WAUTHN | 0x07, /**< Socket access denied */
} wauthn_error_e;

/**
 * @brief WebAuthn authenticator transports.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#enum-transport
 *
 * @remarks Multiple transport values can be combined using bit-wise operation.
 */
typedef enum __wauthn_authenticator_transport {
    WAUTHN_TRANSPORT_NONE                   = 0x00000000, /**< No transport specified */
    WAUTHN_TRANSPORT_USB                    = 0x00000001, /**< USB */
    WAUTHN_TRANSPORT_NFC                    = 0x00000002, /**< NFC */
    WAUTHN_TRANSPORT_BLE                    = 0x00000004, /**< BLE */
    WAUTHN_TRANSPORT_SMARTCARD              = 0x00000008, /**< Smart-card */
    WAUTHN_TRANSPORT_HYBRID                 = 0x00000010, /**< Hybrid */
    WAUTHN_TRANSPORT_INTERNAL               = 0x00000020, /**< Internal */
} wauthn_authenticator_transport_e;

/**
 * @brief WebAuthn hash algorithms.
 *        Currently one hash algorithm is used, namely "SHA-256".
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#collectedclientdata-hash-of-the-serialized-client-data
 *
 */
typedef enum __wauthn_hash_algorithm {
    WAUTHN_HASH_ALGORITHM_SHA_256           = 1, /**< SHA-256 */
} wauthn_hash_algorithm_e;

/**
 * @brief WebAuthn COSE (CBOR Object Signing and Encryption) algorithms.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#sctn-alg-identifier
 *
 */
typedef enum __wauthn_cose_algorithm {
    WAUTHN_COSE_ALGORITHM_ECDSA_P256_WITH_SHA256        = -7,   /**< ES256 */
    WAUTHN_COSE_ALGORITHM_ECDSA_P384_WITH_SHA384        = -35,  /**< ES384 */
    WAUTHN_COSE_ALGORITHM_ECDSA_P521_WITH_SHA512        = -36,  /**< ES512 */
    WAUTHN_COSE_ALGORITHM_EDDSA                         = -8,   /**< EdDSA */
    WAUTHN_COSE_ALGORITHM_RSA_PSS_WITH_SHA256           = -37,  /**< PS256 */
    WAUTHN_COSE_ALGORITHM_RSA_PSS_WITH_SHA384           = -38,  /**< PS384 */
    WAUTHN_COSE_ALGORITHM_RSA_PSS_WITH_SHA512           = -39,  /**< PS512 */
    WAUTHN_COSE_ALGORITHM_RSASSA_PKCS1_V1_5_WITH_SHA256 = -257, /**< RS256 */
    WAUTHN_COSE_ALGORITHM_RSASSA_PKCS1_V1_5_WITH_SHA384 = -258, /**< RS384 */
    WAUTHN_COSE_ALGORITHM_RSASSA_PKCS1_V1_5_WITH_SHA512 = -259, /**< RS512 */
} wauthn_cose_algorithm_e;

/**
 * @brief WebAuthn attestation preference.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#enumdef-attestationconveyancepreference
 *
 */
typedef enum __wauthn_attestation_pref {
    WAUTHN_ATTESTATION_PREF_NONE                     = 0, /**< None */
    WAUTHN_ATTESTATION_PREF_INDIRECT                 = 1, /**< Indirect */
    WAUTHN_ATTESTATION_PREF_DIRECT                   = 2, /**< Direct */
    WAUTHN_ATTESTATION_PREF_ENTERPRISE               = 3, /**< Enterprise */
} wauthn_attestation_pref_e;

/**
 * @brief WebAuthn public key credential hint value.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#enumdef-publickeycredentialhints
 *
 */
typedef enum __wauthn_pubkey_cred_hint {
    WAUTHN_PUBKEY_CRED_HINT_NONE                    = 0, /**< None */
    WAUTHN_PUBKEY_CRED_HINT_SECURITY_KEY            = 1, /**< Security-key */
    WAUTHN_PUBKEY_CRED_HINT_CLIENT_DEVICE           = 2, /**< Client-device */
    WAUTHN_PUBKEY_CRED_HINT_HYBRID                  = 3, /**< Hybrid */
} wauthn_pubkey_cred_hint_e;

/**
 * @brief WebAuthn user verification requirement value.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#enumdef-userverificationrequirement
 *
 */
typedef enum __wauthn_user_verification_requirement {
    WAUTHN_USER_VERIFICATION_REQUIREMENT_NONE                    = 0, /**< None */
    WAUTHN_USER_VERIFICATION_REQUIREMENT_REQUIRED                = 1, /**< Required */
    WAUTHN_USER_VERIFICATION_REQUIREMENT_PREFERRED               = 2, /**< Preferred */
    WAUTHN_USER_VERIFICATION_REQUIREMENT_DISCOURAGED             = 3, /**< Discouraged */
} wauthn_user_verification_requirement_e;

/**
 * @brief WebAuthn resident key requirement value.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#enum-residentKeyRequirement
 *
 */
typedef enum __wauthn_resident_key_requirement {
    WAUTHN_RESIDENT_KEY_REQUIREMENT_NONE                    = 0, /**< None */
    WAUTHN_RESIDENT_KEY_REQUIREMENT_DISCOURAGED             = 1, /**< Discouraged */
    WAUTHN_RESIDENT_KEY_REQUIREMENT_PREFERRED               = 2, /**< Preferred */
    WAUTHN_RESIDENT_KEY_REQUIREMENT_REQUIRED                = 3, /**< Required */
} wauthn_resident_key_requirement_e;

/**
 * @brief WebAuthn authentication attachment value.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#enumdef-authenticatorattachment
 */
typedef enum __wauthn_authenticator_attachment {
    WAUTHN_AUTHENTICATOR_ATTACHMENT_NONE                     = 0, /**< None */
    WAUTHN_AUTHENTICATOR_ATTACHMENT_PLATFORM                 = 1, /**< Platform */
    WAUTHN_AUTHENTICATOR_ATTACHMENT_CROSS_PLATFORM           = 2, /**< Cross-platform */
} wauthn_authenticator_attachment_e;

/**
 * @brief WebAuthn credential type.
 *        Currently one credential type is defined, namely "public-key".
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#enumdef-publickeycredentialtype
 */
typedef enum __wauthn_pubkey_cred_type {
    WAUTHN_PUBKEY_CRED_TYPE_PUBLIC_KEY              = 1, /**< Public-key */
} wauthn_pubkey_cred_type_e;

/**
 * @brief The structure for const binary buffer used in this Native API.
 * @since_tizen 7.0
 */
typedef struct __wauthn_const_buffer {
    const unsigned char *data; /**<
        Byte array containing binary data */
    size_t size; /**<
        The size of the binary data */
} wauthn_const_buffer_s;

/**
 * @brief The structure for response of wauthn_make_credential().
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#authenticatorattestationresponse
 *
 * @see wauthn_make_credential()
 * @see #wauthn_pubkey_credential_attestation_s
 * @see #wauthn_authenticator_transport_e
 * @see #wauthn_cose_algorithm_e
 */
typedef struct __wauthn_authenticator_attestation_response {
    wauthn_const_buffer_s *client_data_json; /**<
        JSON-compatible serialization of client data*/
    wauthn_const_buffer_s *attestation_object; /**<
        The CBOR encoded Attestation Object to be returned to the RP */
    unsigned int transports; /**<
        To represent multiple transports, #wauthn_authenticator_transport_e
        can be ORed multiple times */
    wauthn_const_buffer_s *authenticator_data; /**<
        The authenticator data contained within attestation_object.
        For more information, refer to https://www.w3.org/TR/webauthn-3/#sctn-authenticator-data */
    wauthn_const_buffer_s *subject_pubkey_info; /**<
        DER SubjectPublicKeyInfo of the new credential, or null if this is not available.
        For more information, refer to https://datatracker.ietf.org/doc/html/rfc5280#section-4.1.2.7 */
    wauthn_cose_algorithm_e pubkey_alg; /**<
        The COSEAlgorithmIdentifier of the new credential.*/
} wauthn_authenticator_attestation_response_s;

/**
 * @brief The structure for response of wauthn_get_assertion().
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#authenticatorassertionresponse
 *
 * @see wauthn_get_assertion()
 * @see #wauthn_pubkey_credential_assertion_s
 */
typedef struct __wauthn_authenticator_assertion_response {
    wauthn_const_buffer_s *client_data_json; /**<
        JSON-compatible serialization of client data*/
    wauthn_const_buffer_s *authenticator_data; /**<
        The authenticator data contained within attestation_object.
        For more information, refer to https://www.w3.org/TR/webauthn-3/#sctn-authenticator-data .*/
    wauthn_const_buffer_s *signature; /**<
        The raw signature returned from the authenticator.
        For more information, refer to https://www.w3.org/TR/webauthn-3/#sctn-op-get-assertion .*/
    wauthn_const_buffer_s *user_handle; /**<
        The user handle returned from the authenticator,
        or null if the authenticator did not return a user handle*/
    wauthn_const_buffer_s *attestation_object; /**<
        This OPTIONAL attribute contains an attestation object,
        if the authenticator supports attestation in assertions. */
} wauthn_authenticator_assertion_response_s;

/**
 * @brief The structure for a RP entity.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#dictdef-publickeycredentialrpentity
 */
typedef struct __wauthn_rp_entity {
    const char *name; /**<
        The name of RP */
    const char *id; /**<
        The RPID */
} wauthn_rp_entity_s;

/**
 * @brief The structure for a user entity.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#dictdef-publickeycredentialuserentity
 */
typedef struct __wauthn_user_entity {
    const char *name; /**< A human-palatable name for the entity. */
    wauthn_const_buffer_s *id; /**<
        The ID of the user account. An ID is a byte sequence with a maximum size of 64 bytes,
        and is not meant to be displayed to the user */
    const char *display_name; /**<
        A human-palatable name for the user account, intended only for display */
} wauthn_user_entity_s;


/**
 * @brief The structure for a parameter for credential generation.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#dictdef-publickeycredentialparameters
 *
 * @see #wauthn_pubkey_cred_type_e
 * @see #wauthn_cose_algorithm_e
 */
typedef struct __wauthn_pubkey_cred_param {
    wauthn_pubkey_cred_type_e type; /**<
        Well-known credential type specifying a credential to create */
    wauthn_cose_algorithm_e alg; /**<
        Well-known COSE algorithm specifying the algorithm to use for the credential */
} wauthn_pubkey_cred_param_s;

/**
 * @brief The list structure for parameters for credential generation.
 * @since_tizen 7.0
 * @see #wauthn_pubkey_cred_param_s
 */
typedef struct __wauthn_pubkey_cred_params {
    size_t size;  /**<
        The number of params */
    wauthn_pubkey_cred_param_s *params; /**<
        The sequence of #wauthn_pubkey_cred_param_s */
} wauthn_pubkey_cred_params_s;

/**
 * @brief The structure for a public key credential descriptor.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#dictdef-publickeycredentialdescriptor
 *
 * @see #wauthn_pubkey_cred_type_e
 * @see #wauthn_authenticator_transport_e
 */
typedef struct __wauthn_pubkey_cred_descriptor {
    wauthn_pubkey_cred_type_e type; /**<
        The type of the public key credential */
    wauthn_const_buffer_s *id; /**<
        The credential ID of the public key credential */
    unsigned int transports; /**<
        To represent multiple transports, #wauthn_authenticator_transport_e can be ORed multiple times */
} wauthn_pubkey_cred_descriptor_s;

/**
 * @brief The list structure for public key credential descriptors.
 * @since_tizen 7.0
 * @see #wauthn_pubkey_cred_descriptor_s
 */
typedef struct __wauthn_pubkey_cred_descriptors {
    size_t size; /**<
        The number of descriptors */
    wauthn_pubkey_cred_descriptor_s *descriptors; /**<
        The sequence of #wauthn_pubkey_cred_descriptor_s */
} wauthn_pubkey_cred_descriptors_s;

/**
 * @brief The structure for an authentication extension.
 * @since_tizen 7.0
 * @see #wauthn_authentication_exts_s
 */
typedef struct __wauthn_authentication_ext {
    wauthn_const_buffer_s *extension_id;  /**<
        Extension Identifier defined in the following registry.
        https://www.iana.org/assignments/webauthn/webauthn.xhtml#webauthn-extension-ids. */
    wauthn_const_buffer_s *extension_value; /**<
        Extension value */
} wauthn_authentication_ext_s;

/**
 * @brief The list structure for authentication extensions.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#client-extension-input
 *
 * @see #wauthn_authentication_ext_s
 */
typedef struct __wauthn_authentication_exts {
    size_t size; /**<
        The number of extensions */
    wauthn_authentication_ext_s *extensions; /**<
        The sequence of #wauthn_authentication_ext_s */
} wauthn_authentication_exts_s;

/**
 * @brief The structure for an authenticator selection criteria.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#dictdef-authenticatorselectioncriteria
 *
 * @see #wauthn_authenticator_attachment_e
 * @see #wauthn_resident_key_requirement_e
 * @see #wauthn_user_verification_requirement_e
 */
typedef struct __wauthn_authenticator_sel_cri {
    wauthn_authenticator_attachment_e attachment; /**<
        Authenticator attachment modality*/
    wauthn_resident_key_requirement_e resident_key; /**<
        Specifies the extent to which the Relying Party desires to create a client-side discoverable credential*/
    bool require_resident_key; /**<
        Relying Parties SHOULD set it to true if, and only if, residentKey is set to required.*/
    wauthn_user_verification_requirement_e user_verification; /**<
        Specifies the Relying Party's requirements regarding user verification. */
} wauthn_authenticator_sel_cri_s;

/**
 * @brief The list structure for public key credential hints.
 * @since_tizen 7.0
 * @see #wauthn_pubkey_cred_hint_e
 */
typedef struct __wauthn_pubkey_cred_hints {
    size_t size; /**<
        The number of hints */
    wauthn_pubkey_cred_hint_e *hints; /**<
        The sequence of #wauthn_pubkey_cred_hint_e */
} wauthn_pubkey_cred_hints_s;

/**
 * @brief The structure for a linked device data.
 * @since_tizen 7.0
 *
 * @remarks The linked device data is used for state assisted transaction.
 *          From the successful QR initiated transaction, the linked device data
 *          might be returned from an authenticator to a webauthn client
 *          via #wauthn_pubkey_credential_attestation_s or #wauthn_pubkey_credential_assertion_s.
 *          Then the client can store the linked device data and use it in the next call
 *          for #wauthn_pubkey_cred_creation_options_s or #wauthn_pubkey_cred_request_options_s.
 *          Then the stated assisted transaction will start instead of QR initiated transaction.
 *
 * @remarks For more information, find a section with the keyword, "linking map",
 *          from the following specification.
 *          https://fidoalliance.org/specs/fido-v2.2-rd-20230321/fido-client-to-authenticator-protocol-v2.2-rd-20230321.html

 * @remarks For more information about state assisted transaction, refer to the following.
 *          https://fidoalliance.org/specs/fido-v2.2-rd-20230321/fido-client-to-authenticator-protocol-v2.2-rd-20230321.html#hybrid-state-assisted
 *
 */
typedef struct __wauthn_hybrid_linked_data {
    wauthn_const_buffer_s *contact_id;           /**< CBOR:"1" */
    wauthn_const_buffer_s *link_id;              /**< CBOR:"2" */
    wauthn_const_buffer_s *link_secret;          /**< CBOR:"3" */
    wauthn_const_buffer_s *authenticator_pubkey; /**< CBOR:"4" */
    wauthn_const_buffer_s *authenticator_name;   /**< CBOR:"5" */
    wauthn_const_buffer_s *signature;            /**< CBOR:"6" */
    wauthn_const_buffer_s *tunnel_server_domain; /**< Domain String of tunnel server.*/
    wauthn_const_buffer_s *identity_key;         /**< Identity Key created during QR initiated transaction.*/
} wauthn_hybrid_linked_data_s;

/**
 * @brief The list structure for attestation formats.
 * @since_tizen 7.0
 *
 * @remarks The attestation formats are a sequence of strings that expresses
 *          the Relying Party's preference for attestation statement formats,
 *          from most to least preferable.
 * @remarks Each #wauthn_const_buffer_s has a string of an attestation format
 *          such as packed, android-key, fido-u2f, apple, none, and so on.
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#attestation-statement-format-identifier
 *
 */
typedef struct __wauthn_attestation_formats {
    size_t size; /**<
        The number of attestation_formats */
    wauthn_const_buffer_s *attestation_formats; /**<
        The sequence of #wauthn_const_buffer_s */
} wauthn_attestation_formats_s;

/**
 * @brief The structure for a make credential option.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#dictdef-publickeycredentialcreationoptions
 *
 * @see wauthn_make_credential()
 * @see #wauthn_rp_entity_s
 * @see #wauthn_user_entity_s
 * @see #wauthn_pubkey_cred_params_s
 * @see #wauthn_pubkey_cred_descriptors_s
 * @see #wauthn_authenticator_sel_cri_s
 * @see #wauthn_pubkey_cred_hints_s
 * @see #wauthn_attestation_pref_e
 * @see #wauthn_attestation_formats_s
 * @see #wauthn_authentication_exts_s
 * @see #wauthn_hybrid_linked_data_s
 */
typedef struct __wauthn_pubkey_cred_creation_options {
    wauthn_rp_entity_s *rp; /**< This member contains a name and
        an identifier for the Relying Party responsible for the request */
    wauthn_user_entity_s *user; /**< This member contains names and
        an identifier for the user account performing the registration */
    wauthn_pubkey_cred_params_s *pubkey_cred_params; /**< This member lists the key types and
        signature algorithms the Relying Party supports, ordered from most preferred
        to least preferred */
    unsigned long timeout; /**< This member specifies a time, in milliseconds,
        that the Relying Party is willing to wait for the call to complete.
        This is treated as a hint, and MAY be overridden by the client.
        The value, '0', means no timeout is set. (optional) */
    wauthn_pubkey_cred_descriptors_s *exclude_credentials; /**< The Relying Party SHOULD use
        this member to list any existing credentials mapped to this user account
        (as identified by user.id). (optional) */
    wauthn_authenticator_sel_cri_s *authenticator_selection; /**< The Relying Party MAY use
        this member to specify capabilities and settings that the authenticator
        MUST or SHOULD satisfy to participate in this operation. (optional)*/
    wauthn_pubkey_cred_hints_s *hints; /**<
        This member contains zero or more elements from #wauthn_pubkey_cred_hint_e to
        guide the user agent in interacting with the user. (optional)*/
    wauthn_attestation_pref_e attestation; /**<
        The Relying Party MAY use this member to specify a preference regarding attestation
        conveyance. The default value is #WAUTHN_ATTESTATION_PREF_NONE (optional) */
    wauthn_attestation_formats_s *attestation_formats; /**<
        The Relying Party MAY use this member to specify a preference regarding the attestation
        statement format used by the authenticator. The default value is the empty list, which
        indicates no preference (optional) */
    wauthn_authentication_exts_s *extensions; /**< The Relying Party MAY use this member to provide
        client extension inputs requesting additional processing by the client and authenticator.(optional) */
    wauthn_hybrid_linked_data_s *linked_device; /**< Linked Device Connection Info. Optional.
        If not null, the state assisted transaction will start.*/
} wauthn_pubkey_cred_creation_options_s;

/**
 * @brief The structure for a get assertion option.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#dictionary-assertion-options
 *
 * @see wauthn_get_assertion()
 * @see #wauthn_pubkey_cred_descriptors_s
 * @see #wauthn_user_verification_requirement_e
 * @see #wauthn_pubkey_cred_hints_s
 * @see #wauthn_attestation_pref_e
 * @see #wauthn_attestation_formats_s
 * @see #wauthn_authentication_exts_s
 * @see #wauthn_hybrid_linked_data_s
 */
typedef struct __wauthn_pubkey_cred_request_options {
    unsigned long timeout; /**< This member specifies a time, in milliseconds,
        that the Relying Party is willing to wait for the call to complete.
        This is treated as a hint, and MAY be overridden by the client.
        The value, '0', means no timeout is set. (optional) */
    const char *rpId; /**< This member specifies the RP ID claimed by the Relying Party (optional)*/
    wauthn_pubkey_cred_descriptors_s *allow_credentials; /**< This member is used by the client
        to find authenticators eligible for this authentication ceremony. (optional)*/
    wauthn_user_verification_requirement_e user_verification; /**<
        This member specifies the Relying Party's requirements regarding
        user verification for the wauthn_get_assertion() operation.
        The default value is #WAUTHN_USER_VERIFICATION_REQUIREMENT_PREFERRED (optional) */
    wauthn_pubkey_cred_hints_s *hints; /**<
        This member contains zero or more elements from #wauthn_pubkey_cred_hint_e to
        guide the user agent in interacting with the user. (optional)*/
    wauthn_attestation_pref_e attestation; /**<
        The Relying Party MAY use this member to specify a preference regarding
        attestation conveyance. The default value is #WAUTHN_ATTESTATION_PREF_NONE (optional)*/
    wauthn_attestation_formats_s *attestation_formats; /**<
        The Relying Party MAY use this member to specify a preference regarding the attestation
        statement format used by the authenticator. The default value is the empty list, which
        indicates no preference (optional) */
    wauthn_authentication_exts_s *extensions; /**<
        The Relying Party MAY use this member to provide client extension inputs requesting
        additional processing by the client and authenticator.(optional) */
    wauthn_hybrid_linked_data_s *linked_device; /**< Linked Device Connection Info. Optional.
        If not null, the state assisted transaction will start.*/
} wauthn_pubkey_cred_request_options_s;

/**
 * @brief The structure for a publickey_credential response for wauthn_make_credential().
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#iface-pkcredential
 *          https://www.w3.org/TR/webauthn-3/#sctn-credentialcreationoptions-extension
 *
 * @see wauthn_make_credential()
 * @see #wauthn_pubkey_cred_type_e
 * @see #wauthn_authenticator_attestation_response_s
 * @see #wauthn_authenticator_attachment_e
 * @see #wauthn_authentication_exts_s
 * @see #wauthn_hybrid_linked_data_s
 */
typedef struct __wauthn_pubkey_credential_attestation {
    wauthn_const_buffer_s *id; /**<
        The based64url encoding of credential’s identifier.*/
    wauthn_pubkey_cred_type_e type; /**<
        The credential’s type.*/
    wauthn_const_buffer_s *rawId; /**<
        The raw value of credential’s identifier.*/
    wauthn_authenticator_attestation_response_s *response; /**<
        Authenticator's response. */
    wauthn_authenticator_attachment_e authenticator_attachment; /**<
        Authenticator attachment modality.*/
    wauthn_authentication_exts_s *extensions; /**<
        The results of processing client extensions requested by the Relying Party
        upon the Relying Party's invocation of wauthn_make_credential(). (optional)*/
    wauthn_hybrid_linked_data_s *linked_device; /**<
        Linked Device Connection Info (optional).
        If not null, the caller has to store this value and use this
        in the next transaction to invoke state assisted transaction. */
} wauthn_pubkey_credential_attestation_s;

/**
 * @brief The structure for a publickey_credential response for wauthn_get_assertion().
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification for more information.
 *          https://www.w3.org/TR/webauthn-3/#iface-pkcredential
 *          https://www.w3.org/TR/webauthn-3/#sctn-credentialrequestoptions-extension
 *
 * @see wauthn_get_assertion()
 * @see #wauthn_pubkey_cred_type_e
 * @see #wauthn_authenticator_assertion_response_s
 * @see #wauthn_authenticator_attachment_e
 * @see #wauthn_authentication_exts_s
 * @see #wauthn_hybrid_linked_data_s
 */
typedef struct __wauthn_pubkey_credential_assertion {
    wauthn_const_buffer_s *id; /**<
        The based64url encoding of credential’s identifier.*/
    wauthn_pubkey_cred_type_e type; /**<
        The credential’s type.*/
    wauthn_const_buffer_s *rawId; /**<
        The raw value of credential’s identifier.*/
    wauthn_authenticator_assertion_response_s *response;; /**<
        Authenticator's response. */
    wauthn_authenticator_attachment_e authenticator_attachment; /**<
        Authenticator attachment modality.*/
    wauthn_authentication_exts_s *extensions; /**<
        The results of processing client extensions requested by the Relying Party
        upon the Relying Party's invocation of wauthn_get_assertion(). (optional)*/
    wauthn_hybrid_linked_data_s *linked_device; /**<
        Linked Device Connection Info (optional).
        If not null, the caller has to store this value and use this
        in the next transaction to invoke state assisted transaction. */
} wauthn_pubkey_credential_assertion_s;

/**
 * @brief The structure for a client data JSON.
 * @since_tizen 7.0
 *
 * @remarks Refer to the following W3C specification about how to encode the client_data_json.
 *          https://www.w3.org/TR/webauthn-3/#collectedclientdata-json-compatible-serialization-of-client-data
 *
 * @see #wauthn_hash_algorithm_e
 */
typedef struct __wauthn_client_data {
    wauthn_const_buffer_s *client_data_json; /**<
        UTF-8 encoded JSON serialization of the client data */
    wauthn_hash_algorithm_e hash_alg; /**<
        Hash algorithm used to hash the client_data_json field */
} wauthn_client_data_s;

/**
 * @brief Invoked when a QR Code need to be displayed.
 *
 * @since_tizen 7.0
 *
 * @remarks The qr_contents are encoded as you can see in the encodeQRContents() function of the
 *          FIDO specification:
 *          https://fidoalliance.org/specs/fido-v2.2-rd-20230321/fido-client-to-authenticator-protocol-v2.2-rd-20230321.html#hybrid-qr-initiated.
 *          The qr_contents is encoded like "FIDO:/0254318383..........7406596245".
 * @remarks The image to be displayed shall be created from qr_contents
 *          with media vision API(mv_barcode_generate_image()).
 * @remarks If the request does not need to display a QR code
 *          then this callback function won't be invoked.
 * @remarks The @a qr_contents can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] qr_contents The contents of the QR code to be displayed.
 * @param[in] user_data The user data passed from the callback structure,
 *                      #wauthn_mc_callbacks_s or #wauthn_ga_callbacks_s.
 *
 * @code
 * #include <mv_barcode.h>
 *
 * int ret;
 * mv_engine_config_h engine_cfg = NULL;
 * mv_barcode_type_e type = MV_BARCODE_QR;
 * mv_barcode_qr_mode_e qr_enc_mode = MV_BARCODE_QR_MODE_UTF8;
 * mv_barcode_qr_ecc_e qr_ecc = MV_BARCODE_QR_ECC_LOW;
 * int qr_version = 5;
 * const char* image_path = TMP_QR_FILE_PRX;
 * mv_barcode_image_format_e image_format = MV_BARCODE_IMAGE_FORMAT_PNG;
 * size_t image_width = 200;
 * size_t image_height = 200;
 *
 * // the file, TMP_QR_FILE_PRX + ".png", will be created.
 * ret = mv_barcode_generate_image(engine_cfg, qr_contents, image_width, image_height, type,
 *                                  qr_enc_mode, qr_ecc, qr_version, image_path, image_format);
 * if (ret !=  MEDIA_VISION_ERROR_NONE) {
 *     std::cout << "mv_barcode_generate_image failed. ret=" << ret << std::endl;
 *     return -1;
 * }
 * @endcode
 *
 * @see #wauthn_mc_callbacks_s
 * @see #wauthn_ga_callbacks_s
 */
typedef void (*wauthn_display_qrcode_cb)(const char *qr_contents, void *user_data);

/**
 * @brief Invoked when the response for the make credential request need to be returned.
 * @since_tizen 7.0
 * @remarks The @a pubkey_cred can be used only in the callback. To use outside, make a copy.
 * @param[in] pubkey_cred The public key credential contains response data.
 * @param[in] result The result of the wauthn_make_credential() request.
 *                   #WAUTHN_ERROR_NONE if the request is completed well,
 *                   #WAUTHN_ERROR_CANCELED if the request is cancelled by wauthn_cancel() request.
 *                   #WAUTHN_ERROR_INVALID_STATE if the server entered invalid state. Known causes:
 *                   - proxy issues,
 *                   - reached the limit of credentials stored by the authenticator.
 *                   #WAUTHN_ERROR_TIMED_OUT if the request times out. Know causes:
 *                   - authenticator does not respond during state assisted transactions due to
 *                     lack of push notifications support (e.g. missing Google Account).
 * @param[in] user_data The user data passed from the callback structure, #wauthn_mc_callbacks_s.
 *
 * @see #wauthn_mc_callbacks_s
 * @see #wauthn_pubkey_credential_attestation_s
 */
typedef void (*wauthn_mc_on_response_cb)(const wauthn_pubkey_credential_attestation_s *pubkey_cred,
                                         wauthn_error_e result,
                                         void *user_data);

/**
 * @brief Invoked when the response for the get assertion request need to be returned.
 * @since_tizen 7.0
 * @remarks The @a pubkey_cred can be used only in the callback. To use outside, make a copy.
 * @param[in] pubkey_cred The public key credential contains response data.
 * @param[in] result The result of the request.
 *                   #WAUTHN_ERROR_NONE if the request is completed well,
 *                   #WAUTHN_ERROR_CANCELED if the request is cancelled by wauthn_cancel() request.
 *                   #WAUTHN_ERROR_INVALID_STATE if the server entered invalid state. Known causes:
 *                   - proxy issues,
 *                   - reached the limit of credentials stored by the authenticator.
 *                   #WAUTHN_ERROR_TIMED_OUT if the request times out. Know causes:
 *                   - authenticator does not respond during state assisted transactions due to
 *                     lack of push notifications support (e.g. missing Google Account).
 * @param[in] user_data The user data passed from the callback structure, #wauthn_ga_callbacks_s.
 *
 * @see #wauthn_ga_callbacks_s
 * @see #wauthn_pubkey_credential_assertion_s
 */
typedef void (*wauthn_ga_on_response_cb)(const wauthn_pubkey_credential_assertion_s *pubkey_cred,
                                         wauthn_error_e result,
                                         void *user_data);

/**
 * @brief Invoked when the authenticator updates its linked device data or
 *            the authenticator disconnects.
 * @since_tizen 7.0
 *
 * @remarks An authenticator might send its updated linked device data to a client within 2 minutes
 *          after sending a response to the client.
 * @remarks If an authenticator updates its linked device data,
 *          this callback is called with #WAUTHN_ERROR_NONE_AND_WAIT and not null linked_data.
 * @remarks If an authenticator disconnects or 2 minutes have passed after receiving
 *          #wauthn_mc_on_response_cb or #wauthn_ga_on_response_cb,
 *          this callback is called with #WAUTHN_ERROR_NONE and null linked_data.
 * @remarks The @a linked_data can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] linked_data The Linked Device Connection Info.
 *                        If this is not null, the client has to update the data for next use.
 * @param[in] result The result of the request.
 *                   #WAUTHN_ERROR_NONE_AND_WAIT if a update message has arrived and
 *                   the client needs to wait for another message to arrive.
 *                   #WAUTHN_ERROR_NONE if the client doesn't have to to wait for another message.
 *                   #WAUTHN_ERROR_INVALID_STATE if the server entered invalid state. Known causes:
 *                   - reached the limit of credentials stored by the authenticator.
 * @param[in] user_data The user data passed from the callback structure, #wauthn_ga_callbacks_s.
 *
 * @see #wauthn_ga_callbacks_s
 * @see #wauthn_hybrid_linked_data_s
 */
typedef void (*wauthn_update_linked_data_cb)(const wauthn_hybrid_linked_data_s *linked_data,
                                             wauthn_error_e result,
                                             void *user_data);

/**
 * @brief The structure for callback function list used to make credential.
 * @since_tizen 7.0
 * @see wauthn_make_credential()
 * @see wauthn_display_qrcode_cb()
 * @see wauthn_mc_on_response_cb()
 */
typedef struct __wauthn_mc_callbacks {
    wauthn_display_qrcode_cb qrcode_callback; /**<
        Callback function for displaying QR code*/
    wauthn_mc_on_response_cb response_callback; /**<
        Callback function for getting the final response*/
    wauthn_update_linked_data_cb linked_data_callback; /**<
        Callback function for getting the updated linked device data*/
    void *user_data; /**<
        User data to be passed to qrcode_callback, response_callback and linked_data_callback */
} wauthn_mc_callbacks_s;

/**
 * @brief The structure for callback function list used to get assertion.
 * @since_tizen 7.0
 * @see wauthn_get_assertion()
 * @see wauthn_display_qrcode_cb()
 * @see wauthn_ga_on_response_cb()
 */
typedef struct __wauthn_ga_callbacks {
    wauthn_display_qrcode_cb qrcode_callback; /**<
        Callback function for displaying QR code*/
    wauthn_ga_on_response_cb response_callback; /**<
        Callback function for getting the final response*/
    wauthn_update_linked_data_cb linked_data_callback; /**<
        Callback function for getting the updated linked device data*/
    void *user_data; /**<
        User data to be passed to qrcode_callback, response_callback and linked_data_callback */
} wauthn_ga_callbacks_s;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_CORE_WEBAUTHN_TYPES_H__ */
