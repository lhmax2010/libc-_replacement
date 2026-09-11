/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_UIX_AUTOFILL_MANAGER_H__
#define __TIZEN_UIX_AUTOFILL_MANAGER_H__

/**
 * @file autofill_manager.h
 * @brief This file contains autofill manager APIs and related enumeration.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <tizen.h>
#include <autofill_error.h>

/**
 * @addtogroup CAPI_UIX_AUTOFILL_MANAGER_MODULE
 * @{
 */

/**
 * @brief Enumeration of connection status.
 * @since_tizen 5.5
 */
typedef enum {
    AUTOFILL_MANAGER_CONNECTION_STATUS_CONNECTED = 0, /**< Connected */
    AUTOFILL_MANAGER_CONNECTION_STATUS_DISCONNECTED, /**< Disconnected */
    AUTOFILL_MANAGER_CONNECTION_STATUS_REJECTED, /**< Rejected */
} autofill_manager_connection_status_e;

/**
 * @brief The autofill manager handle.
 * @since_tizen 5.5
 */
typedef struct autofill_manager_s *autofill_manager_h;

/**
 * @brief Called when the connection status is changed.
 * @since_tizen 5.5
 * @remarks @a amh should not be freed and can be used only in the callback.
 * @param[in] amh The autofill manager handle
 * @param[in] status The connection status
 * @param[in] user_data The user data passed from the callback function
 * @see autofill_manager_connect()
 */
typedef void (*autofill_manager_connection_status_changed_cb)(autofill_manager_h amh, autofill_manager_connection_status_e status, void* user_data);

/**
 * @brief Called for each autofill service information.
 * @since_tizen 5.5
 * @remarks @a app_id should not be freed and can be used only in the callback.
 * @param[in] app_id The autofill service app ID
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @pre autofill_manager_foreach_autofill_service() will invoke this callback.
 * @see autofill_manager_foreach_autofill_service()
 */
typedef bool (*autofill_manager_autofill_service_cb)(const char *app_id, void *user_data);

/**
 * @platform
 * @brief Creates a handle for autofill manager.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/autofillmanager
 * @remarks If the function succeeds, @a amh handle must be released with autofill_manager_destroy().
 * @param[out] amh The autofill manager handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OUT_OF_MEMORY Out of memory
 * @see autofill_manager_destroy()
 */
int autofill_manager_create(autofill_manager_h *amh);

/**
 * @platform
 * @brief Destroys autofill manager handle.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/autofillmanager
 * @param[in] amh The autofill manager handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see autofill_manager_create()
 */
int autofill_manager_destroy(autofill_manager_h amh);

/**
 * @platform
 * @brief Connects to autofill manager daemon.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/autofillmanager
 * @param[in] amh The autofill manager handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_manager_connect(autofill_manager_h amh, autofill_manager_connection_status_changed_cb callback, void *user_data);

/**
 * @platform
 * @brief Sets autofill service.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/autofillmanager
 * @param[in] amh The autofill manager handle
 * @param[in] app_id The autofill service app ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see autofill_manager_get_autofill_service()
 */
int autofill_manager_set_autofill_service(autofill_manager_h amh, const char *app_id);

/**
 * @platform
 * @brief Gets autofill service app ID.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/autofillmanager
 * @remarks @a service_app_id must be released using free().
 * @param[in] amh The autofill manager handle
 * @param[out] service_app_id The autofill service app ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see autofill_manager_set_autofill_service()
 */
int autofill_manager_get_autofill_service(autofill_manager_h amh, char **service_app_id);

/**
 * @platform
 * @brief Retrieves all autofill services.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/autofillmanager
 * @param[in] amh The autofill manager handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_manager_foreach_autofill_service(autofill_manager_h amh, autofill_manager_autofill_service_cb callback, void* user_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_UIX_AUTOFILL_MANAGER_H__ */
