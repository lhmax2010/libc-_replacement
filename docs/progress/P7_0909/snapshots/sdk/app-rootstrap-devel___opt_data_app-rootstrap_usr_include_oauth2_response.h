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

#ifndef OAUTH2_RESPONSE_H_
#define OAUTH2_RESPONSE_H_

#include <bundle.h>
#include <oauth2_types.h>
#include <oauth2_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file oauth2_response.h
 * @brief The OAuth 2.0 response APIs are used to get various parameters received from server.
 * Refer to service provider documentation about the allowed and additional fields.
 */

/**
 * @addtogroup CAPI_OAUTH2_MODULE
 * @{
 */

/**
 * @brief The structure type for OAuth 2.0 Response handle.
 * @since_tizen 2.4
 */
typedef struct oauth2_response_s *oauth2_response_h;

/**
 * @brief Destroys the received handle.
 * @since_tizen 2.4
 *
 * @param[in] handle The response handle
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_response_destroy(oauth2_response_h handle);

/**
 * @brief Gets the authorization code.
 * @since_tizen 2.4
 *
 * @remarks You must not release @a code using free().
 * @param[in] handle The response handle
 * @param[out] code The code.
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 *
 * @see oauth2_request_set_authorization_code()
 */
OAUTH2_API int oauth2_response_get_authorization_code(oauth2_response_h handle, char **code);

/**
 * @brief Gets state.
 * @since_tizen 2.4
 *
 * @remarks You must not release @a state using free().
 * @param[in] handle The response handle
 * @param[out] state The state.
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_response_get_state(oauth2_response_h handle, char **state);

/**
 * @brief Gets access token.
 * @since_tizen 2.4
 *
 * @remarks You must not release @a access_token using free().
 * @param[in] handle The response handle
 * @param[out] access_token The access token
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_response_get_access_token(oauth2_response_h handle, char **access_token);

/**
 * @brief Gets token type.
 * @since_tizen 2.4
 *
 * @remarks You must not release @a token_type using free().
 * @param[in] handle The response handle
 * @param[out] token_type The token type
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_response_get_token_type(oauth2_response_h handle, char **token_type);

/**
 * @brief Gets expiry time.
 * @since_tizen 2.4
 *
 * @remarks You must not release @a expires_in using free().
 * @param[in] handle The response handle
 * @param[out] expires_in Expiry value
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_response_get_expires_in(oauth2_response_h handle, long long *expires_in);

/**
 * @brief Gets refresh token.
 * @since_tizen 2.4
 *
 * @remarks You must not release @a refresh_token using free().
 * @param[in] handle The response handle
 * @param[out] refresh_token The refresh token
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_response_get_refresh_token(oauth2_response_h handle, char **refresh_token);

/**
 * @brief Gets scope.
 * @since_tizen 2.4
 *
 * @remarks You must not release @a scope using free().
 * @param[in] handle The response handle
 * @param[out] scope The scope
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_response_get_scope(oauth2_response_h handle, char **scope);

/**
 * @brief Gets error.
 * @since_tizen 2.4
 *
 * @remarks You must not release @a error using free().
 * @param[in] handle The response handle
 * @param[out] error The error structure
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 *
 * @see oauth2_error_h
 */
OAUTH2_API int oauth2_response_get_error(oauth2_response_h handle, oauth2_error_h *error);

/**
 * @brief Gets the custom data.
 * @details Some service providers send additional keys not specified in OAuth 2.0 RFC. To get those additional fields this API needs to be used.
 * @since_tizen 2.4
 *
 * @remarks You must not release @a custom_value using free().
 * @param[in] handle The response handle
 * @param[in] custom_key The custom key
 * @param[out] custom_value The custom value
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_response_get_custom_data(oauth2_response_h handle, const char *custom_key, char **custom_value);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* OAUTH2_RESPONSE_H_ */
