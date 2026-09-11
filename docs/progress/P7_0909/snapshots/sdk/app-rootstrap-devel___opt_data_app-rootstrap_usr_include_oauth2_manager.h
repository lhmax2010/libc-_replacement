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

#ifndef OAUTH2_MANAGER_H_
#define OAUTH2_MANAGER_H_

#include <stdbool.h>

#include <oauth2_request.h>
#include <oauth2_response.h>
#include <oauth2_error.h>
#include <oauth2_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file oauth2_manager.h
 * @brief The main OAuth 2.0 handler APIs.
 * @details The Application must use either:
 * oauth2_manager_request_token
 * or
 * oauth2_manager_request_authorization_grant and then oauth2_manager_request_access_token.
 * One instance handles only one pending request at any given time. If requested again before completion it throws TIZEN_ERROR_ALREADY_IN_PROGRESS.
 * Application  must have internet in order to use these APIs.
 */

/**
 * @addtogroup CAPI_OAUTH2_MODULE
 * @{
 */

/**
 * @brief The structure type for OAuth 2.0 Manager handle.
 * @since_tizen 2.4
 */
typedef struct oauth2_manager_s *oauth2_manager_h;

/**
 * @brief Creates #oauth2_manager_h handle.
 * @remarks The @a handle must be released using oauth2_manager_destroy().
 * @since_tizen 2.4
 *
 * @param[out]  handle  The created handle
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 *
 * @see oauth2_manager_destroy()
 */
OAUTH2_API int oauth2_manager_create(oauth2_manager_h *handle);

/**
 * @brief Destroys #oauth2_manager_h handle.
 * @since_tizen 2.4
 *
 * @param[in] handle The oauth2 manager handle
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API int oauth2_manager_destroy(oauth2_manager_h handle);

/**
 * @brief Called when the oauth2_manager_request_token() response comes.
 * @since_tizen 2.4
 * @remarks The @a response must be released using oauth2_response_destroy().
 *
 * @param[in] response The response
 * @param[in] user_data The user data passed from the callback function
 *
 * @pre oauth2_manager_request_token() must be called to get this callback invoked.
 * @see oauth2_manager_request_token()
 */
typedef void (*oauth2_token_cb)(oauth2_response_h response, void *user_data);

/**
 * @brief Request OAuth 2.0 access token.
 * @details The response is delivered via oauth2_token_cb().
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 * @remarks The "internet" privilege is required to call this API. Note, only one pending request is allowed.
 *
 * @param[in] handle   The oauth2 manager handle.
 * @param[in] request  The request handle.
 * @param[in] callback The callback to receive response.
 * @param[in] user_data  The user data to be passed to the callback function.
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY Out of memory.
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER Invalid input parameter(s) passed.
 * @retval     #OAUTH2_ERROR_ALREADY_IN_PROGRESS The previous request is already in progress.
 * @retval     #OAUTH2_ERROR_PERMISSION_DENIED Permission denied.
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED Not supported.
 * @retval     #OAUTH2_ERROR_PARSE_FAILED Parsing failed.
 * @retval     #OAUTH2_ERROR_NETWORK_ERROR Network Error.
 * @retval     #OAUTH2_ERROR_UNKNOWN Unknown system error.
 *
 * @see oauth2_token_cb()
 * @see oauth2_manager_create()
 * @see oauth2_request_create()
 */
OAUTH2_API int oauth2_manager_request_token(oauth2_manager_h handle, oauth2_request_h request, oauth2_token_cb callback, void *user_data);

/**
 * @brief Called when the oauth2_manager_request_token_auth_url() response comes.
 * @since_tizen 9.0
 * @remarks The @a response must be released using oauth2_response_destroy().
 *
 * @param[in] response The response
 * @param[in] user_data The user data passed from the callback function
 *
 * @pre oauth2_manager_request_token_auth_url() must be called to get this callback invoked.
 * @see oauth2_manager_request_token_auth_url()
 */
typedef void (*oauth2_token_auth_url_cb)(oauth2_response_h response, void *user_data);

/**
 * @brief Requests OAuth 2.0 access token with authorization URL.
 * @details The response is delivered via oauth2_token_auth_url_cb().
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 * @remarks The "internet" privilege is required to call this API. Note, only one pending request is allowed.
 *
 * @param[in] handle   The oauth2 manager handle.
 * @param[in] request  The request handle.
 * @param[in] callback The callback to receive response.
 * @param[in] response_url  The response URL after successful Google Sign-in which contains the token.
 * @param[in] user_data  The user data to be passed to the callback function.
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED Not supported.
 * @retval     #OAUTH2_ERROR_PERMISSION_DENIED Permission denied.
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER Invalid input parameter(s) passed.
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY Out of memory.
 * @retval     #OAUTH2_ERROR_ALREADY_IN_PROGRESS The previous request is already in progress.
 * @retval     #OAUTH2_ERROR_PARSE_FAILED Parsing failed.
 * @retval     #OAUTH2_ERROR_NETWORK_ERROR Network Error.
 * @retval     #OAUTH2_ERROR_UNKNOWN Unknown system error.
 *
 * @see oauth2_token_auth_url_cb()
 * @see oauth2_manager_create()
 * @see oauth2_request_create()
 */
OAUTH2_API int oauth2_manager_request_token_auth_url(oauth2_manager_h handle, oauth2_request_h request, oauth2_token_auth_url_cb callback, char *response_url, void *user_data);

/**
 * @brief Called when oauth2_manager_request_authorization_grant() response comes.
 * @since_tizen 2.4
 *
 * @param[in] response The response. The @a response must be released using oauth2_response_destroy().
 * @param[in] user_data The user data passed from the callback function.
 *
 * @pre oauth2_manager_request_authorization_grant() must be called to get this callback invoked.
 * @see oauth2_manager_request_authorization_grant()
 */
typedef void (*oauth2_auth_grant_cb)(oauth2_response_h response, void *user_data);

/**
 * @brief Request authorization grant.
 * @details The response is delivered via oauth2_auth_grant_cb().
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 * @remarks The "internet" privilege is required to call this API. Note, only one pending request is allowed at a time.
 *
 * @param[in] handle   The oauth2_manager handle.
 * @param[in] request  The request handle.
 * @param[in] callback The application callback.
 * @param[in] user_data  The user data to be passed to the callback function.
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY Out of memory.
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER Invalid input parameter(s) passed.
 * @retval     #OAUTH2_ERROR_ALREADY_IN_PROGRESS The previous request is already in progress.
 * @retval     #OAUTH2_ERROR_PERMISSION_DENIED Permission denied.
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED Not supported.
 * @retval     #OAUTH2_ERROR_PARSE_FAILED Parsing failed.
 * @retval     #OAUTH2_ERROR_NETWORK_ERROR Network Error.
 * @retval     #OAUTH2_ERROR_UNKNOWN Unknown system error.
 *
 * @see oauth2_auth_grant_cb()
 * @see oauth2_manager_create()
 * @see oauth2_request_create()
 */
OAUTH2_API int oauth2_manager_request_authorization_grant(oauth2_manager_h handle, oauth2_request_h request, oauth2_auth_grant_cb callback, void *user_data);

/**
 * @brief Called when oauth2_manager_request_access_token() response comes.
 * @since_tizen 2.4
 *
 * @param[in] response The response. The response must be released using oauth2_response_destroy().
 * @param[in] user_data The user data passed from the callback function.
 *
 * @pre oauth2_manager_request_access_token() must be called to get this callback invoked.
 * @see oauth2_manager_request_access_token()
 */
typedef void (*oauth2_access_token_cb)(oauth2_response_h response, void *user_data);

/**
 * @brief Requests an access token.
 * @details The response is delivered via oauth2_access_token_cb() callback.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 * @remarks The "internet" privilege is required to call this API. Note, only one pending request is allowed at a time.
 *
 * @param[in] handle   The oauth2_manager handle.
 * @param[in] request  The request handle.
 * @param[in] callback The application callback.
 * @param[in] user_data  The user data to be passed to the callback function.
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY Out of memory.
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER Invalid input parameter(s) passed.
 * @retval     #OAUTH2_ERROR_ALREADY_IN_PROGRESS The previous request is already in progress.
 * @retval     #OAUTH2_ERROR_PERMISSION_DENIED Permission denied.
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED Not supported.
 * @retval     #OAUTH2_ERROR_PARSE_FAILED Parsing failed.
 * @retval     #OAUTH2_ERROR_NETWORK_ERROR Network Error.
 * @retval     #OAUTH2_ERROR_UNKNOWN Unknown system error.
 *
 * @see oauth2_access_token_cb()
 * @see oauth2_manager_create()
 * @see oauth2_request_create()
 */
OAUTH2_API int oauth2_manager_request_access_token(oauth2_manager_h handle, oauth2_request_h request, oauth2_access_token_cb callback, void *user_data);

/**
 * @brief Called when oauth2_manager_refresh_access_token() response comes.
 * @since_tizen 2.4
 *
 * @param[in] response The response. The @a response must be released using oauth2_response_destroy().
 * @param[in] user_data The user data passed from the callback function.
 *
 * @pre oauth2_manager_refresh_access_token() must be called to get this callback invoked.
 * @see oauth2_manager_refresh_access_token().
 */
typedef void (*oauth2_refresh_token_cb)(oauth2_response_h response, void *user_data);

/**
 * @brief Requests a refresh token.
 * @details The response is delivered via oauth2_refresh_token_cb() callback.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 * @remarks The "internet" privilege is required to call this API. Note, only one pending request is allowed at a time.
 *
 * @param[in] handle   The oauth2 manager handle.
 * @param[in] request  The request handle.
 * @param[in] callback The application callback.
 * @param[in]  user_data  The user data to be passed to the callback function.
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY Out of memory.
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER Invalid input parameter(s) passed.
 * @retval     #OAUTH2_ERROR_ALREADY_IN_PROGRESS The previous request is already in progress.
 * @retval     #OAUTH2_ERROR_PERMISSION_DENIED Permission denied.
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED Not supported.
 * @retval     #OAUTH2_ERROR_PARSE_FAILED Parsing failed.
 * @retval     #OAUTH2_ERROR_NETWORK_ERROR Network Error.
 * @retval     #OAUTH2_ERROR_UNKNOWN Unknown system error.
 *
 * @see oauth2_refresh_token_cb()
 * @see oauth2_manager_create()
 * @see oauth2_request_create()
 */
OAUTH2_API int oauth2_manager_refresh_access_token(oauth2_manager_h handle, oauth2_request_h request, oauth2_refresh_token_cb callback, void *user_data);

/**
 * @brief Returns whether oauth2_manager related request is pending.
 * @since_tizen 2.4
 *
 * @param[in] handle The auth2 manager handle.
 * @retval     false No pending request
 * @retval     true There is pending request.
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED      Not supported
 */
OAUTH2_API bool oauth2_manager_is_request_in_progress(oauth2_manager_h handle);

/**
 * @brief Clears the cookies.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 *
 * @param[in] handle The oauth2 manager handle.
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_PERMISSION_DENIED Permission denied.
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED Not supported.
 */
OAUTH2_API int oauth2_manager_clear_cookies(oauth2_manager_h handle);

/**
 * @brief Clears the cache.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 *
 * @param[in] handle The oauth2 manager handle.
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval     #OAUTH2_ERROR_NONE               Successful
 * @retval     #OAUTH2_ERROR_OUT_OF_MEMORY      Out of Memory
 * @retval     #OAUTH2_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval     #OAUTH2_ERROR_PERMISSION_DENIED Permission denied.
 * @retval     #OAUTH2_ERROR_NOT_SUPPORTED Not supported.
 */
OAUTH2_API int oauth2_manager_clear_cache(oauth2_manager_h handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* OAUTH2_MANAGER_H_ */
