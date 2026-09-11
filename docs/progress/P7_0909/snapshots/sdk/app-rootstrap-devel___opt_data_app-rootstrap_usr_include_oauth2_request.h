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

#ifndef OAUTH2_REQUEST_H_
#define OAUTH2_REQUEST_H_

#include <bundle.h>
#include <curl/curl.h>
#include <oauth2_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file oauth2_request.h
 * @brief The OAuth 2.0 request APIs are used to set various parameters to send for OAuth 2.0.
 * Refer to service provider documentation about the allowed and additional fields.
 */

/**
 * @addtogroup CAPI_OAUTH2_MODULE
 * @{
 */

/**
 * @brief The structure type for OAuth 2.0 Request handle.
 *@since_tizen 2.4
 */
typedef struct oauth2_request_s *oauth2_request_h;

/**
 * @brief Creates #oauth2_request_h handle.
 * @details The handle must be freed via oauth2_request_destroy().
 *@since_tizen 2.4
 *
 * @param[out] handle The created oauth2_request handle, else NULL for error cases
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_create(oauth2_request_h *handle);

/**
 * @brief Destroys #oauth2_request_h handle.
 *@since_tizen 2.4
 *
 * @param[in] handle The oauth2_request handle to destroy
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_destroy(oauth2_request_h handle);

/**
 * @brief Sets authorization end point URL.
 *@since_tizen 2.4
 *
 * @param[in] handle The request handle
 * @param[in] url The url
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_set_auth_end_point_url(oauth2_request_h handle, const char *url);

/**
 * @brief Sets access token end point URL.
 *@since_tizen 2.4
 *
 * @param[in] handle The request handle
 * @param[in] url The url
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_set_token_end_point_url(oauth2_request_h handle, const char *url);

/**
 * @brief Sets redirection URL.
 *@since_tizen 2.4
 *
 * @param[in] handle The request handle
 * @param[in] url The url
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_set_redirection_url(oauth2_request_h handle, const char *url);

/**
 * @brief Sets refresh token end point URL.
 *@since_tizen 2.4
 *
 * @param[in] handle The request handle
 * @param[in] url The url
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_set_refresh_token_url(oauth2_request_h handle, const char *url);

/**
 * @brief Sets refresh token. Used mostly while using oauth2_manager_refresh_access_token().
 *@since_tizen 2.4
 *
 * @param[in] handle The request handle
 * @param[in] refresh_token The refresh token string
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_set_refresh_token(oauth2_request_h handle, char *refresh_token);

/**
 * @brief Sets response type.
 *@since_tizen 2.4
 *
 * @param[in] handle The request handle
 * @param[in] response_type The response type
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_set_response_type(oauth2_request_h handle, oauth2_response_type_e response_type);

/**
 * @brief Sets client id.
 *@since_tizen 2.4
 *
 * @param[in] handle The request handle
 * @param[in] client_id The client id
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_set_client_id(oauth2_request_h handle, const char *client_id);

/**
 * @brief Sets client secret.
 *@since_tizen 2.4
 *
 * @param[in] handle The request handle
 * @param[in] client_secret The secret
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_set_client_secret(oauth2_request_h handle, const char *client_secret);

/**
 * @brief Sets client authentication type. Default is OAUTH2_CLIENT_AUTHENTICATION_TYPE_BASIC.
 *@since_tizen 2.4
 *
 * @remarks Facebook and Google does not support HTTP Basic Authentication, instead they require client credentials to be sent via request body.\n
 * So application must set #OAUTH2_CLIENT_AUTHENTICATION_TYPE_REQUEST_BODY for them.
 *
 * @param[in] handle The request handle
 * @param[in] client_auth_type The client authentication type
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_set_client_authentication_type(oauth2_request_h handle, oauth2_client_authentication_type_e client_auth_type);

/**
 * @brief Sets scope.
 *@since_tizen 2.4
 *
 * @param[in] handle The request handle
 * @param[in] scope The scope
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_set_scope(oauth2_request_h handle, const char *scope);

/**
 * @brief Sets state.
 *@since_tizen 2.4
 *
 * @param[in] handle The request handle
 * @param[in] state The state
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_set_state(oauth2_request_h handle, const char *state);

/**
 * @brief Sets grant type.
 *@since_tizen 2.4
 *
 * @param[in] handle The request handle
 * @param[in] grant_type The grant type
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_set_grant_type(oauth2_request_h handle, oauth2_grant_type_e grant_type);

/**
 * @brief Sets authorization code.
 *@since_tizen 2.4
 *
 * @param[in] handle The request handle
 * @param[in] code The authorization code
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 *
 * @see oauth2_response_get_authorization_code()
 */
OAUTH2_API int oauth2_request_set_authorization_code(oauth2_request_h handle, const char *code);

/**
 * @brief Sets user name.
 *@since_tizen 2.4
 *
 * @param[in] handle The request handle
 * @param[in] user_name The user name
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_set_user_name(oauth2_request_h handle, const char *user_name);

/**
 * @brief Sets password.
 *@since_tizen 2.4
 *
 * @param[in] handle The request handle
 * @param[in] password The password
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_set_password(oauth2_request_h handle, const char *password);

/**
 * @brief Adds custom key-value pair to the request.
 *@since_tizen 2.4
 *
 * @param[in] handle The request handle
 * @param[in] key The key
 * @param[in] value The value
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 *
 * @see oauth2_request_get_custom_data()
 */
OAUTH2_API int oauth2_request_add_custom_data(oauth2_request_h handle, const char *key, const char *value);

/**
 * @brief Gets authorization end point URL.
 *@since_tizen 2.4
 *
 * @remarks You must not release @a url using free().
 * @param[in] handle The request handle
 * @param[out] url The url
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_get_auth_end_point_url(oauth2_request_h handle, char **url);

/**
 * @brief Gets access token end point URL.
 *@since_tizen 2.4
 *
 * @remarks You must not release @a url using free().
 * @param[in] handle The request handle
 * @param[out] url The url
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_get_token_end_point_url(oauth2_request_h handle, char **url);

/**
 * @brief Gets redirection URL.
 *@since_tizen 2.4
 *
 * @remarks You must not release @a url using free().
 * @param[in] handle The request handle
 * @param[out] url The url
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_get_redirection_url(oauth2_request_h handle, char **url);

/**
 * @brief Gets refresh token end point URL.
 *@since_tizen 2.4
 *
 * @remarks You must not release @a url using free().
 * @param[in] handle The request handle
 * @param[out] url The url
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_get_refresh_token_url(oauth2_request_h handle, char **url);

/**
 * @brief Gets refresh token.
 *@since_tizen 2.4
 *
 * @remarks You must not release @a refresh_token using free().
 * @param[in] handle The request handle
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
OAUTH2_API int oauth2_request_get_refresh_token(oauth2_request_h handle, char **refresh_token);

/**
 * @brief Gets response type.
 *@since_tizen 2.4
 *
 * @remarks		You must not free response_type
 * @param[in] handle The request handle
 * @param[out] response_type The response type
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_get_response_type(oauth2_request_h handle, oauth2_response_type_e *response_type);

/**
 * @brief Gets client id.
 *@since_tizen 2.4
 *
 * @remarks You must not release @a client_id using free().
 * @param[in] handle The request handle
 * @param[out] client_id The client id
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_get_client_id(oauth2_request_h handle, char **client_id);

/**
 * @brief Gets client secret.
 *@since_tizen 2.4
 *
 * @remarks You must not release @a client_secret using free().
 * @param[in] handle The request handle
 * @param[out] client_secret The client_secret
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_get_client_secret(oauth2_request_h handle, char **client_secret);

/**
 * @brief Gets scope.
 *@since_tizen 2.4
 *
 * @remarks You must not release @a scope using free().
 * @param[in] handle The request handle
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
OAUTH2_API int oauth2_request_get_scope(oauth2_request_h handle, char **scope);

/**
 * @brief Gets state.
 *@since_tizen 2.4
 *
 * @remarks You must not release @a state using free().
 * @param[in] handle The request handle
 * @param[out] state The state
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_get_state(oauth2_request_h handle, char **state);

/**
 * @brief Gets grant type.
 *@since_tizen 2.4
 *
 * @remarks You must not release @a grant_type using free().
 * @param[in] handle The request handle
 * @param[out] grant_type The grant type
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_get_grant_type(oauth2_request_h handle, oauth2_grant_type_e *grant_type);

/**
 * @brief Gets authorization code.
 *@since_tizen 2.4
 *
 * @remarks You must not release @a code using free().
 * @param[in] handle The request handle.
 * @param[out] code The code.
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_get_authorization_code(oauth2_request_h handle, char **code);

/**
 * @brief Gets user name.
 *@since_tizen 2.4
 *
 * @remarks You must not release @a user_name using free().
 * @param[in] handle The request handle
 * @param[out] user_name The user name
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_get_user_name(oauth2_request_h handle, char **user_name);

/**
 * @brief Gets password.
 *@since_tizen 2.4
 *
 * @remarks You must not release @a password using free().
 * @param[in] handle The request handle
 * @param[out] password The password
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_VALUE_NOT_FOUND    Value not found
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_request_get_password(oauth2_request_h handle, char **password);

/**
 * @brief Gets the custom value.
 *@since_tizen 2.4
 *
 * @remarks You must not release @a custom_value using free().
 * @param[in] handle The request handle
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
OAUTH2_API int oauth2_request_get_custom_data(oauth2_request_h handle, const char *custom_key, char **custom_value);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* OAUTH2_REQUEST_H_ */
