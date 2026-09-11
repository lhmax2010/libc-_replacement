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

#ifndef __TIZEN_UIX_AUTOFILL_SERVICE_H__
#define __TIZEN_UIX_AUTOFILL_SERVICE_H__

/**
 * @file autofill_service.h
 * @brief This file contains autofill service APIs and related enumeration.
 */

#include <autofill_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_UIX_AUTOFILL_SERVICE_MODULE
 * @{
 */

/**
 * @brief Called when receiving authentication information request.
 * @since_tizen 5.5
 * @remarks @a vi should not be freed and can be used only in the callback.
 * @param[in] context_id The autofill context identification value of an associated autofill client handle
 * @param[in] vi The autofill view info handle
 * @param[in] user_data The user data passed from the callback function
 * @see autofill_service_set_auth_info_requested_cb()
 */
typedef void (*autofill_service_auth_info_requested_cb)(int context_id, autofill_view_info_h vi, void *user_data);

/**
 * @brief Called when receiving fill request.
 * @since_tizen 5.5
 * @remarks @a vi should not be freed and can be used only in the callback.
 * @param[in] context_id The autofill context identification value of an associated autofill client handle
 * @param[in] vi The autofill view info handle
 * @param[in] user_data The user data passed from the callback function
 * @see autofill_service_set_fill_requested_cb()
 */
typedef void (*autofill_service_fill_requested_cb)(int context_id, autofill_view_info_h vi, void *user_data);

/**
 * @brief Called when receiving the cancellation of autofill request.
 * @since_tizen 5.5
 * @remarks @a vi should not be freed and can be used only in the callback.
 * @param[in] context_id The autofill context identification value of an associated autofill client handle
 * @param[in] vi The autofill view info handle
 * @param[in] user_data The user data passed from the callback function
 * @see autofill_service_set_cancel_fill_requested_cb()
 */
typedef void (*autofill_service_cancel_fill_requested_cb)(int context_id, autofill_view_info_h vi, void *user_data);

/**
 * @brief Called when receiving commit request.
 * @since_tizen 5.5
 * @remarks @a vi should not be freed and can be used only in the callback.
 * @param[in] context_id The autofill context identification value of an associated autofill client handle
 * @param[in] vi The autofill save view info handle
 * @param[in] user_data The user data passed from the callback function
 * @see autofill_service_set_committed_cb()
 */
typedef void (*autofill_service_committed_cb)(int context_id, autofill_save_view_info_h vi, void *user_data);

/**
 * @brief Called when receiving terminate request.
 * @since_tizen 5.5
 * @param[in] user_data The user data passed from the callback function
 * @see autofill_service_set_terminate_received_cb()
 */
typedef void (*autofill_service_terminate_received_cb)(void *user_data);

/**
 * @brief Initializes autofill service library.
 * @since_tizen 5.5
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 */
int autofill_service_initialize(void);

/**
 * @brief Deinitializes autofill service library.
 * @since_tizen 5.5
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 */
int autofill_service_deinitialize(void);

/**
 * @brief Sets the callback to receive the request of authentication information.
 * @since_tizen 5.5
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_service_set_auth_info_requested_cb(autofill_service_auth_info_requested_cb callback, void *user_data);

/**
 * @brief Unsets the callback to receive the request of authentication information.
 * @since_tizen 5.5
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 */
int autofill_service_unset_auth_info_requested_cb(void);

/**
 * @brief Sends the authentication information.
 * @since_tizen 5.5
 * @param[in] context_id The autofill context identification value of an associated autofill client handle
 * @param[in] h The autofill authentication info handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_service_send_auth_info(int context_id, autofill_auth_info_h h);

/**
 * @brief Sets the callback to receive the fill request.
 * @since_tizen 5.5
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_service_set_fill_requested_cb(autofill_service_fill_requested_cb callback, void *user_data);

/**
 * @brief Unsets the callback to receive the fill request.
 * @since_tizen 5.5
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 */
int autofill_service_unset_fill_requested_cb(void);

/**
 * @brief Sets the callback to receive the cancellation of autofill request.
 * @since_tizen 5.5
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_service_set_cancel_fill_requested_cb(autofill_service_cancel_fill_requested_cb callback, void *user_data);

/**
 * @brief Unsets the callback to receive the cancellation of autofill request.
 * @since_tizen 5.5
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 */
int autofill_service_unset_cancel_fill_requested_cb(void);

/**
 * @brief Sends the fill response.
 * @since_tizen 5.5
 * @param[in] context_id The autofill context identification value of an associated autofill client handle
 * @param[in] h The autofill fill response handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_service_send_fill_response(int context_id, autofill_fill_response_h h);

/**
 * @brief Sets the callback to receive the commit request.
 * @since_tizen 5.5
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_service_set_committed_cb(autofill_service_committed_cb callback, void *user_data);

/**
 * @brief Unsets the callback to receive the commit request.
 * @since_tizen 5.5
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 */
int autofill_service_unset_committed_cb(void);

/**
 * @brief Sets the callback to receive the terminate request.
 * @since_tizen 5.5
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_service_set_terminate_received_cb(autofill_service_terminate_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback to receive the terminate request.
 * @since_tizen 5.5
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 */
int autofill_service_unset_terminate_received_cb(void);

/**
 * @brief Sends error to the autofill client.
 * @since_tizen 5.5
 * @param[in] context_id The autofill context identification value of an associated autofill client handle
 * @param[in] h The autofill error info handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_service_send_error_info(int context_id, autofill_error_info_h h);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_UIX_AUTOFILL_SERVICE_H__ */
