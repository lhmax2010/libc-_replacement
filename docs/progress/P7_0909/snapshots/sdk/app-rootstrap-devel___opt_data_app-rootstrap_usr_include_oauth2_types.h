/*
 * Copyright (c) 2014 - 2015 Samsung Electronics Co., Ltd. All rights reserved.
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
 * limitations under the License.
 *
 */

#ifndef OAUTH2_TYPES_H_
#define OAUTH2_TYPES_H_

#include <tizen_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file     oauth2_types.h
 * @brief    This file defines common types and enums of OAuth 2.0.
 */

/**
 * @addtogroup CAPI_OAUTH2_MODULE
 * @{
 */

#ifndef OAUTH2_API
#define OAUTH2_API __attribute__((__visibility__("default")))
#endif

/**
 * @brief Enumerations for Client authentication scheme, used to sign client id and client secret accordingly.
 * @since_tizen 2.4
 * @remarks Facebook and Google does not support HTTP Basic Authentication, instead they require client credentials to be sent via request body.
 * Default is #OAUTH2_CLIENT_AUTHENTICATION_TYPE_BASIC (<a href="http://tools.ietf.org/html/rfc2617#section-2">rfc2617#section-2</a>)
 *
 * @see oauth2_request_set_client_authentication_type()
 */
typedef enum {
	OAUTH2_CLIENT_AUTHENTICATION_TYPE_BASIC,/**< HTTP Basic Authentication for client authentication. */
	OAUTH2_CLIENT_AUTHENTICATION_TYPE_DIGEST,/**< HTTP Digest Authentication for client authentication. */
	OAUTH2_CLIENT_AUTHENTICATION_TYPE_REQUEST_BODY/**< Client credentials are sent via request body */
}oauth2_client_authentication_type_e;

/**
 * @brief Enumerations to set grant_type.
 * @since_tizen 2.4
 *
 * @see oauth2_request_set_grant_type()
 */
typedef enum {
	OAUTH2_GRANT_TYPE_AUTH_CODE, /**< Access Token Request for Authorization Code Grant type. */
	OAUTH2_GRANT_TYPE_PASSWORD, /**< Access Token Request for Resource Owner Password Credentials Grant type. */
	OAUTH2_GRANT_TYPE_CLIENT_CREDENTIALS, /**< Access Token Request for Client Credentials Grant type. */
	OAUTH2_GRANT_TYPE_REFRESH /**< Refresh Token Request. */
}oauth2_grant_type_e;

/**
 * @brief Enumerations to set response_type.
 * @since_tizen 2.4
 *
 * @see oauth2_request_set_response_type()
 */
typedef enum {
	OAUTH2_RESPONSE_TYPE_CODE,/**< Requesting an authorization code as response type. */
	OAUTH2_RESPONSE_TYPE_TOKEN/**< Requesting an access token(implicit grant) as response type. */
}oauth2_response_type_e;

/**
 *  @brief Enumerations of error codes for oauth2 APIs.
 *  @since_tizen 2.4
 */
typedef enum {
    OAUTH2_ERROR_NONE                   = TIZEN_ERROR_NONE,                   /**< Successful */
    OAUTH2_ERROR_OUT_OF_MEMORY             = TIZEN_ERROR_OUT_OF_MEMORY,              /**< Out of memory */
    OAUTH2_ERROR_INVALID_PARAMETER         = TIZEN_ERROR_INVALID_PARAMETER,        /**< Invalid parameter */
    OAUTH2_ERROR_ALREADY_IN_PROGRESS = TIZEN_ERROR_ALREADY_IN_PROGRESS, /**< Operation already in progress */
    OAUTH2_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Operation not supported */
    OAUTH2_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
    OAUTH2_ERROR_PARSE_FAILED = TIZEN_ERROR_ACCOUNT_OAUTH | 0x01, /**< Response parse failed */
    OAUTH2_ERROR_NETWORK_ERROR = TIZEN_ERROR_ACCOUNT_OAUTH | 0x02, /**< Network error */
    OAUTH2_ERROR_SERVER = TIZEN_ERROR_ACCOUNT_OAUTH | 0x03, /**< Server error */
    OAUTH2_ERROR_USER_CANCELLED = TIZEN_ERROR_ACCOUNT_OAUTH | 0x04, /**< User cancelled the operation  */
    OAUTH2_ERROR_VALUE_NOT_FOUND = TIZEN_ERROR_ACCOUNT_OAUTH | 0x05,	 /**< Value not found */
    OAUTH2_ERROR_UNKNOWN = TIZEN_ERROR_UNKNOWN /**< Unknown error */
}oauth2_error_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* OAUTH2_TYPES_H_ */
