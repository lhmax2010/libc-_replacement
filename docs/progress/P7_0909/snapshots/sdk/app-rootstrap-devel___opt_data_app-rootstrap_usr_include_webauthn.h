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
 * @file    webauthn.h
 * @version 1.0
 * @brief   Public APIs of WebAuthn module
*/
#ifndef __TIZEN_CORE_WEBAUTHN_H__
#define __TIZEN_CORE_WEBAUTHN_H__

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>

#include <webauthn-types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_WEBAUTHN_MODULE
 * @{
 */

/**
 * @brief Sets API version that the caller uses.
 *
 * @since_tizen 7.0
 *
 * @remarks This function is related with following feature:\n
 *          - %http://tizen.org/feature/security.webauthn\n
 *
 * @remarks This function must be called before other functions are called.
 *
 * @param[in] api_version_number API version number to set.
 *                Use #WAUTHN_API_VERSION_NUMBER as an input.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WAUTHN_ERROR_NONE Successful
 * @retval #WAUTHN_ERROR_NOT_SUPPORTED The specified API version or required feature is not
 *                                     supported
 */
int wauthn_set_api_version(int api_version_number);

/**
 * @brief Gets information on authenticator types that the client platform supports.
 *
 * @since_tizen 7.0
 *
 * @remarks This function is related with following feature:\n
 *          - %http://tizen.org/feature/security.webauthn\n
 *
 * @remarks This version of API only support hybrid roaming authenticator.
 *          So the bit for #WAUTHN_TRANSPORT_HYBRID is set 1 and
 *          bits for other authenticators set 0.
 *          Each authenticators can be checked using bitwise operation.
 *
 * @param[out] supported The pointer to an unsigned int for return supported authenticators
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WAUTHN_ERROR_NONE Successful
 * @retval #WAUTHN_ERROR_NOT_SUPPORTED The required feature is not supported
 * @retval #WAUTHN_ERROR_INVALID_PARAMETER The @a supported is NULL
 *
 * @code
 * unsigned int supported = 0;
 * auto ret = wauthn_supported_authenticators(&supported);
 * if (ret != WAUTHN_ERROR_NONE)
 *     return ret;
 *
 * if (supported & WAUTHN_TRANSPORT_HYBRID) {
 *     // hybrid transport is supported
 * } else {
 *     // hybrid transport is not supported
 * }
 * @endcode
 *
 * @see #wauthn_authenticator_transport_e
 */
int wauthn_supported_authenticators(unsigned int *supported);

/**
 * @brief Makes a new web authentication credential and store it to authenticator.
 *
 * @since_tizen 7.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth\n
 *            %http://tizen.org/privilege/internet
 *
 * @remarks This function is related with following features:\n
 *          - %http://tizen.org/feature/security.webauthn\n
 *          - %http://tizen.org/feature/network.bluetooth.le\n
 *          - and at least one of the below features:\n
 *            - %http://tizen.org/feature/network.wifi\n
 *            - %http://tizen.org/feature/network.ethernet\n
 *            - %http://tizen.org/feature/network.telephony\n
 *
 * @remarks Refer to the following W3C specification for more information.
 *           https://www.w3.org/TR/webauthn-3/#sctn-createCredential
 *
 * @param[in]  client_data UTF-8 encoded JSON serialization of the client data.
 * @param[in]  options This argument is a #wauthn_pubkey_cred_creation_options_s object specifying the
 *                     desired attributes of the to-be-created public key credential
 * @param[in]  callbacks The callback functions to be invoked
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WAUTHN_ERROR_NONE Successful
 * @retval #WAUTHN_ERROR_NOT_SUPPORTED The required feature is not supported
 * @retval #WAUTHN_ERROR_PERMISSION_DENIED Required privilege is missing
 * @retval #WAUTHN_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #WAUTHN_ERROR_NOT_ALLOWED Not allowed in the current context including busy state
 * @retval #WAUTHN_ERROR_INVALID_STATE Get in an invalid state
 * @retval #WAUTHN_ERROR_CANCELED Cancelled by cancel request
 * @retval #WAUTHN_ERROR_CONNECTION_REFUSED Authenticator is permanently uncontactable. Forget link info
 *
 * @pre API version must be set with wauthn_set_api_version() and required authenticator type must
 *      be supported (can be checked with wauthn_supported_authenticators()).
 *
 * @see wauthn_get_assertion()
 * @see #wauthn_pubkey_cred_creation_options_s
 * @see #wauthn_mc_callbacks_s
 */
int wauthn_make_credential(const wauthn_client_data_s *client_data,
                           const wauthn_pubkey_cred_creation_options_s *options,
                           wauthn_mc_callbacks_s *callbacks);

/**
 * @brief Gets assertion from authenticator.
 *
 * @since_tizen 7.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth\n
 *            %http://tizen.org/privilege/internet
 *
 * @remarks This function is related with following features:\n
 *          - %http://tizen.org/feature/security.webauthn\n
 *          - %http://tizen.org/feature/network.bluetooth.le\n
 *          - and at least one of the below features:\n
 *            - %http://tizen.org/feature/network.wifi\n
 *            - %http://tizen.org/feature/network.ethernet\n
 *            - %http://tizen.org/feature/network.telephony\n
 *
 * @remarks Refer to the following W3C specification for more information.
 *           https://www.w3.org/TR/webauthn-3/#sctn-getAssertion
 *
 * @param[in]  client_data UTF-8 encoded JSON serialization of the client data
 * @param[in]  options This argument is a #wauthn_pubkey_cred_request_options_s object specifying the
 *                     desired attributes of the public key credential to discover
 * @param[in]  callbacks The callback functions to be invoked
 *
 * @return @c 0 on success,
 * @retval #WAUTHN_ERROR_NONE Successful
 * @retval #WAUTHN_ERROR_NOT_SUPPORTED The required feature is not supported
 * @retval #WAUTHN_ERROR_PERMISSION_DENIED Required privilege is missing
 * @retval #WAUTHN_ERROR_INVALID_PARAMETER Input parameter is invalid
 * @retval #WAUTHN_ERROR_NOT_ALLOWED Not allowed in the current context including busy state
 * @retval #WAUTHN_ERROR_CANCELED Cancelled by cancel request
 * @retval #WAUTHN_ERROR_CONNECTION_REFUSED Authenticator is permanently uncontactable. Forget link info
 *
 * @pre API version must be set with wauthn_set_api_version() and required authenticator type must
 *      be supported (can be checked with wauthn_supported_authenticators()).
 *
 * @see wauthn_make_credential()
 * @see #wauthn_pubkey_cred_request_options_s
 * @see #wauthn_ga_callbacks_s
 */
int wauthn_get_assertion(const wauthn_client_data_s *client_data,
                         const wauthn_pubkey_cred_request_options_s *options,
                         wauthn_ga_callbacks_s *callbacks);

/**
 * @brief Stops operation of the previous wauthn_make_credential() or wauthn_get_assertion() call.
 *
 * @since_tizen 7.0
 *
 * @remarks This function is related with following feature:\n
 *          - %http://tizen.org/feature/security.webauthn\n
 *
 * @return @c 0 on success,
 * @retval #WAUTHN_ERROR_NONE Successful
 * @retval #WAUTHN_ERROR_NOT_SUPPORTED The required feature is not supported
 * @retval #WAUTHN_ERROR_NOT_ALLOWED Not allowed in the current context
 *
 * @see wauthn_make_credential()
 * @see wauthn_get_assertion()
 */
int wauthn_cancel(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_CORE_WEBAUTHN_H__ */
