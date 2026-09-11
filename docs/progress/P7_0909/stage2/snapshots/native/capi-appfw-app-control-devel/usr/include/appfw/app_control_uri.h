/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_APPFW_APP_CONTROL_URI_INCLUDE_H__
#define __TIZEN_APPFW_APP_CONTROL_URI_INCLUDE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <tizen_error.h>

/**
 * @addtogroup CAPI_APP_CONTROL_URI_MODULE
 * @{
 */

/**
 * @brief Enumeration for error codes of the App Control URI submodule.
 * @since_tizen 5.5
 */
typedef enum {
	APP_CONTROL_URI_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	APP_CONTROL_URI_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< Internal I/O error */
	APP_CONTROL_URI_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	APP_CONTROL_URI_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
} app_control_uri_error_e;

/**
 * @brief The URI builder handle.
 * @since_tizen 5.5
 */
typedef void *app_control_uri_builder_h;

/**
 * @brief The URI handle.
 * @since_tizen 5.5
 */
typedef void *app_control_uri_h;

/**
 * @brief The query component handle of the URI.
 * @since_tizen 5.5
 */
typedef const void *app_control_uri_query_h;

/**
 * @brief Creates a URI builder handle.
 * @since_tizen 5.5
 *
 * @remarks The @a handle should be released using app_control_uri_builder_destroy().
 *
 * @param[out]  handle                  The URI builder handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successful
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_URI_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_uri_builder_destroy()
 */
int app_control_uri_builder_create(app_control_uri_builder_h *handle);

/**
 * @brief Sets the scheme component.
 * @since_tizen 5.5
 *
 * @param[in]   handle                  The URI builder handle
 * @param[in]   scheme                  The scheme component
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successful
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_builder_set_scheme(app_control_uri_builder_h handle,
		const char *scheme);

/**
 * @brief Sets the authority component.
 * @since_tizen 5.5
 *
 * @param[in]   handle                  The URI builder handle
 * @param[in]   auth                    The authority component
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successful
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_builder_set_authority(app_control_uri_builder_h handle,
		const char *auth);

/**
 * @brief Sets the path component.
 * @details If the path was already set, it will be replaced with @a path.
 * @since_tizen 5.5
 *
 * @param[in]   handle                  The URI builder handle
 * @param[in]   path                    The path component
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successful
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_builder_set_path(app_control_uri_builder_h handle,
		const char *path);

/**
 * @brief Adds the path component.
 * @details Builder appends @a path component to the path. If @a path doesn't
 *          start with a '/', builder will prepend the given path with a '/'.
 * @since_tizen 5.5
 *
 * @param[in]   handle                  The URI builder handle
 * @param[in]   path                    The path component
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successful
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_builder_add_path(app_control_uri_builder_h handle,
		const char *path);

/**
 * @brief Sets the fragment identifier component.
 * @since_tizen 5.5
 *
 * @param[in]   handle                  The URI builder handle
 * @param[in]   fragment                The fragment identifier component
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successful
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_builder_set_fragment(app_control_uri_builder_h handle,
		const char *fragment);

/**
 * @brief Adds the key-value pair attribute of the query component.
 * @since_tizen 5.5
 *
 * @param[in]   handle                  The URI builder handle
 * @param[in]   key                     The name of the query component key-value pairs
 * @param[in]   val                     The value associated with the given key
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successful
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_builder_add_query(app_control_uri_builder_h handle,
		const char *key, const char *val);

/**
 * @brief Creates a URI handle using the attributes of URI builder handle.
 * @since_tizen 5.5
 *
 * @remarks The @a app_control_uri should be released using app_control_uri_destroy().
 *
 * @param[in]   builder                 The URI builder handle
 * @param[out]  app_control_uri         The URI handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successful
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_URI_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_control_uri_builder_build(app_control_uri_builder_h builder,
		app_control_uri_h* app_control_uri);

/**
 * @brief Destroys the URI builder handle.
 * @since_tizen 5.5
 *
 * @param[in]   handle                  The URI builder handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successful
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_builder_destroy(app_control_uri_builder_h handle);

/**
 * @brief Creates a URI handle from the encoded URI string.
 * @since_tizen 5.5
 *
 * @remarks The @a handle should be released using app_control_uri_destroy().
 *
 * @param[in]   encoded_app_control_uri The encoded URI string
 * @param[out]  handle                  The URI handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successful
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_URI_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_uri_encode()
 */
int app_control_uri_create(const char *encoded_app_control_uri,
		app_control_uri_h *handle);

/**
 * @brief Destroys the URI handle.
 * @since_tizen 5.5
 *
 * @param[in]   handle                  The URI handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successful
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_destroy(app_control_uri_h handle);

/**
 * @brief Encodes the URI handle to string. The string is RFC 3986-compliant.
 * @since_tizen 5.5
 *
 * @remarks The @a encoded_app_control_uri URI string should be released using free().
 *
 * @param[in]   handle                  The URI handle
 * @param[out]  encoded_app_control_uri The encoded URI string
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successfully
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_URI_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_control_uri_encode(app_control_uri_h handle,
		char **encoded_app_control_uri);

/**
 * @brief Gets the scheme component from a URI handle.
 * @since_tizen 5.5
 *
 * @remarks The @a scheme must not be released. The platform will release it
 *          when the App Control URI object containing it is released.
 *
 * @param[in]   handle                  The URI handle
 * @param[out]  scheme                  The scheme component
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successfully
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_get_scheme(app_control_uri_h handle, const char **scheme);

/**
 * @brief Gets the authority component from a URI handle.
 * @since_tizen 5.5
 *
 * @remarks The @a auth must not be released. The platform will release it
 *          when the App Control URI object containing it is released.
 *
 * @param[in]   handle                  The URI handle
 * @param[out]  auth                    The authority component
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successfully
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_get_authority(app_control_uri_h handle, const char **auth);

/**
 * @brief Gets the path component from a URI handle.
 * @since_tizen 5.5
 *
 * @remarks The @a path must not be released. The platform will release it
 *          when the App Control URI object containing it is released.
 *
 * @param[in]   handle                  The URI handle
 * @param[out]  path                    The path component
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successfully
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_get_path(app_control_uri_h handle, const char **path);

/**
 * @brief Gets the fragment identifier component from a URI handle.
 * @since_tizen 5.5
 *
 * @remarks The @a fragment must not be released. The platform will release it
 *          when the App Control URI object containing it is released.
 *
 * @param[in]   handle                  The URI handle
 * @param[out]  fragment                The fragment identifier component
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successfully
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_get_fragment(app_control_uri_h handle,
		const char **fragment);

/**
 * @brief Gets the handle of the query component from a URI handle.
 * @since_tizen 5.5
 *
 * @remarks The @a query_handle will be released by the platform when
 *          the App Control URI object which contains the query is released.
 *
 * @param[in]    handle                 The URI handle
 * @param[out]   query_handle           The query component handle of the URI
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successfully
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_get_query(app_control_uri_h handle,
		app_control_uri_query_h *query_handle);

/**
 * @brief Gets the host subcomponent from a URI handle.
 * @since_tizen 5.5
 *
 * @remarks The @a host must not be released. The platform will release it
 *          when the App Control URI object containing it is released.
 *
 * @param[in]    handle                  The URI handle
 * @param[out]   host                    The host subcomponent of authority
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successfully
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_get_host(app_control_uri_h handle, const char **host);

/**
 * @brief Gets the port subcomponent from a URI handle.
 * @since_tizen 5.5
 *
 * @remarks The @a port must not be released. The platform will release it
 *          when the App Control URI object containing it is released.
 *
 * @param[in]   handle                   The URI handle
 * @param[out]  port                     The port subcomponent of authority
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successfully
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_get_port(app_control_uri_h handle, const char **port);

/**
 * @brief Gets the user subcomponent from a URI handle.
 * @since_tizen 5.5
 *
 * @remarks The @a user must not be released. The platform will release it
 *          when the App Control URI object containing it is released.
 *
 * @param[in]   handle                  The URI handle
 * @param[out]  user                    The user subcomponent of authority
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successfully
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_get_user(app_control_uri_h handle, const char **user);

/**
 * @brief Called to retrieve key-value pair attributes contained in the query component.
 * @since_tizen 5.5
 *
 * @remarks @a key and @a val must not be released. The platform will release them when the query object is released (see app_control_uri_get_query())
 *
 * @param[in]   key                     The name of the query component key-value pairs
 * @param[in]   val                     The value associated with the given key
 * @param[in]   user_data               The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @see app_control_uri_query_foreach()
 */
typedef bool (*app_control_uri_query_foreach_cb)(const char *key,
		const char *val, void *user_data);

/**
 * @brief Retrieves key-value pair attributes in the query component.
 * @since_tizen 5.5
 *
 * @param[in]   handle                  The query component handle of the URI
 * @param[in]   callback                The iteration callback function
 * @param[in]   user_data               The user data to be passed the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_URI_ERROR_NONE Successful
 * @retval #APP_CONTROL_URI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_uri_query_foreach(app_control_uri_query_h handle,
		app_control_uri_query_foreach_cb callback, void *user_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_APP_CONTROL_URI_INCLUDE_H__ */
