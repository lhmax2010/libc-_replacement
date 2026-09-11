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

#ifndef __TIZEN_UIX_AUTOFILL_H__
#define __TIZEN_UIX_AUTOFILL_H__

/**
 * @file autofill.h
 * @brief This file contains autofill APIs and related enumeration.
 */

#include <autofill_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_UIX_AUTOFILL_CLIENT_MODULE
 * @{
 */

/**
 * @brief Enumeration of connection status.
 * @since_tizen 5.5
 */
typedef enum {
    AUTOFILL_CONNECTION_STATUS_CONNECTED = 0, /**< Connected */
    AUTOFILL_CONNECTION_STATUS_DISCONNECTED, /**< Disconnected */
    AUTOFILL_CONNECTION_STATUS_REJECTED, /**< Rejected */
} autofill_connection_status_e;

/**
 * @brief The autofill handle.
 * @since_tizen 5.5
 */
typedef struct autofill_s *autofill_h;

/**
 * @brief Called when the connection status is changed.
 * @since_tizen 5.5
 * @remarks @a ah should not be freed and can be used only in the callback.
 * @param[in] ah The autofill handle
 * @param[in] status The connection status
 * @param[in] user_data The user data passed from the callback function
 * @see autofill_connect()
 */
typedef void (*autofill_connection_status_changed_cb)(autofill_h ah, autofill_connection_status_e status, void* user_data);

/**
 * @brief Called when receiving autofill fill response data.
 * @since_tizen 5.5
 * @remarks @a ah should not be freed and can be used only in the callback.
 * @remarks @a fill_response_h should not be freed and can be used only in the callback.
 * @param[in] ah The autofill handle
 * @param[in] fill_response_h The autofill fill response handle
 * @param[in] user_data The user data to be passed to the callback function
 * @see autofill_fill_response_set_received_cb()
 */
typedef void (*autofill_fill_response_received_cb)(autofill_h ah, autofill_fill_response_h fill_response_h, void *user_data);

/**
 * @brief Called when receiving the authentication information.
 * @since_tizen 5.5
 * @remarks @a ah should not be freed and can be used only in the callback.
 * @remarks @a auth_info should not be freed and can be used only in the callback.
 * @param[in] ah The autofill handle
 * @param[in] auth_info The autofill authentication information handle
 * @param[in] user_data The user data passed from the callback function
 * @see autofill_auth_info_set_received_cb()
 */
typedef void (*autofill_auth_info_received_cb)(autofill_h ah, autofill_auth_info_h auth_info, void *user_data);

/**
 * @brief Called when receiving the error information.
 * @since_tizen 5.5
 * @remarks @a ah should not be freed and can be used only in the callback.
 * @remarks @a error_info should not be freed and can be used only in the callback.
 * @param[in] ah The autofill handle
 * @param[in] error_info The autofill error information handle
 * @param[in] user_data The user data passed from the callback function
 * @see autofill_error_info_set_received_cb()
 */
typedef void (*autofill_error_info_received_cb)(autofill_h ah, autofill_error_info_h error_info, void *user_data);

/**
 * @brief Creates a handle for autofill.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a ah handle must be released with autofill_destroy().
 * @param[out] ah The autofill handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OUT_OF_MEMORY Out of memory
 * @see autofill_destroy()
 */
int autofill_create(autofill_h *ah);

/**
 * @brief Destroys an autofill.
 * @since_tizen 5.5
 * @param[in] ah The autofill handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see autofill_create()
 */
int autofill_destroy(autofill_h ah);

/**
 * @brief Connects to autofill daemon.
 * @since_tizen 5.5
 * @param[in] ah The autofill handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_connect(autofill_h ah, autofill_connection_status_changed_cb callback, void *user_data);

/**
 * @brief Requests of authentication information.
 * @since_tizen 5.5
 * @param[in] ah The autofill handle
 * @param[in] vi The autofill view info handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_auth_info_request(autofill_h ah, autofill_view_info_h vi);

/**
 * @brief Sets the callback to receive the authentication information.
 * @since_tizen 5.5
 * @param[in] ah The autofill handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see autofill_auth_info_unset_received_cb()
 */
int autofill_auth_info_set_received_cb(autofill_h ah, autofill_auth_info_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback to receive the authentication information.
 * @since_tizen 5.5
 * @param[in] ah The autofill handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see autofill_auth_info_set_received_cb()
 */
int autofill_auth_info_unset_received_cb(autofill_h ah);

/**
 * @brief Sends fill request to fill out each input form.
 * @since_tizen 5.5
 * @param[in] ah The autofill handle
 * @param[in] vi The autofill view info handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 * @see autofill_cancel_fill_request()
 */
int autofill_fill_request(autofill_h ah, autofill_view_info_h vi);

/**
 * @brief Cancels autofill request to fill out each input form.
 * @since_tizen 5.5
 * @param[in] ah The autofill handle
 * @param[in] vi The autofill view info handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 * @see autofill_fill_request()
 */
int autofill_cancel_fill_request(autofill_h ah, autofill_view_info_h vi);

/**
 * @brief Sets the callback to receive autofill fill response.
 * @since_tizen 5.5
 * @param[in] ah The autofill handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see autofill_fill_response_unset_received_cb()
 */
int autofill_fill_response_set_received_cb(autofill_h ah, autofill_fill_response_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback to receive autofill fill response.
 * @since_tizen 5.5
 * @param[in] ah The autofill handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_fill_response_unset_received_cb(autofill_h ah);

/**
 * @brief Sends the autofill save view info.
 * @since_tizen 5.5
 * @param[in] ah The autofill handle
 * @param[in] vi The autofill save view info handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_commit(autofill_h ah, autofill_save_view_info_h vi);

/**
 * @brief Sets the callback to receive the error information.
 * @since_tizen 5.5
 * @param[in] ah The autofill handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see autofill_error_info_unset_received_cb()
 */
int autofill_error_info_set_received_cb(autofill_h ah, autofill_error_info_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback to receive the error information.
 * @since_tizen 5.5
 * @param[in] ah The autofill handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see autofill_error_info_set_received_cb()
 */
int autofill_error_info_unset_received_cb(autofill_h ah);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_UIX_AUTOFILL_H__ */
