/*
 * Copyright (c) 2018-2019 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_UIFW_MULTI_ASSISTANT_UI_H__
#define __TIZEN_UIFW_MULTI_ASSISTANT_UI_H__

#include <multi_assistant_common.h>

/**
 * @defgroup CAPI_UIX_MULTI_ASSISTANT_UI_MODULE Multi assistant UI
 * @ingroup CAPI_UIX_MULTI_ASSISTANT_MODULE
 * @brief The @ref CAPI_UIX_MULTI_ASSISTANT_UI_MODULE API provides functions for showing queries and responses sent from multi assistant service.
 * @{
 */


#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Called when the information of the assistants is requested.
 * @since_tizen 5.0
 * @remarks The @a app_id can be used only in the callback. To use outside, make a copy.
 *          The @a name can be used only in the callback. To use outside, make a copy.
 *          The @a icon_path can be used only in the callback. To use outside, make a copy.
 *          The @a wakeup_list can be used only in the callback. To use outside, make a copy.
 *          The @a supported_lang can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] app_id The app ID of the assistant
 * @param[in] name The name of the assistant
 * @param[in] icon_path The directory of the icon corresponding to the assistant
 * @param[in] wakeup_list The list of wakeup words which invoke the assistant
 * @param[in] cnt_wakeup The number of wakeup words
 * @param[in] supported_lang The list of languages supported by the assistant
 * @param[in] cnt_lang The number of languages supported by the assistant
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see ma_ui_foreach_assistant_info()
 */
typedef int (*ma_ui_assistant_info_cb)(const char* app_id, const char* name, const char* icon_path, const char* wakeup_list[], int cnt_wakeup, const char* supported_lang[], int cnt_lang, void* user_data);

/**
 * @brief Called when the multi-assistant service sends the ASR results for showing at UI.
 * @since_tizen 5.0
 * @remarks The @a asr_result can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] event The ASR result event
 * @param[in] asr_result The ASR result text
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see ma_ui_set_asr_result_cb()
 * @see ma_ui_unset_asr_result_cb()
 */
typedef void (*ma_ui_asr_result_cb)(ma_asr_result_event_e event, const char* asr_result, void* user_data);

/**
 * @brief Called when the multi-assistant service sends the results for showing at UI.
 * @since_tizen 5.0
 * @remarks The @a display_text can be used only in the callback. To use outside, make a copy.
 *          The @a utterance_text can be used only in the callback. To use outside, make a copy.
 *          The @a result_json can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] display_text The text shown on the display
 * @param[in] utterance_text The utterance text
 * @param[in] result_json The result data (JSON format)
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see ma_ui_set_asr_result_cb()
 * @see ma_ui_unset_asr_result_cb()
 */
typedef void (*ma_ui_result_cb)(const char* display_text, const char* utterance_text, const char* result_json, void* user_data);

/**
 * @brief Called when the multi-assistant service sends the request for changing assistant.
 * @since_tizen 5.0
 * @remarks The @a app_id can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] app_id The text shown on the display
 * @param[in] user_data The user data passed from the callback registration function
 *
 */
typedef void (*ma_ui_change_assistant_cb)(const char* app_id, void* user_data);

/**
 * @brief Called when the multi-assistant service sends the recognition results.
 * @since_tizen 5.5
 *
 * @param[in] result The recognition result
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see ma_ui_set_recognition_result_cb()
 * @see ma_ui_unset_recognition_result_cb()
 */
typedef void (*ma_ui_recognition_result_cb)(ma_recognition_result_event_e result, void* user_data);

/**
 * @brief Called when the multi-assistant service enables or disables the common UI.
 * @since_tizen 5.5
 *
 * @param[in] enable The status whether the common UI is enabled
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see ma_ui_set_enable_common_ui_cb()
 */
typedef void (*ma_ui_enable_common_ui_cb)(bool enable, void* user_data);

/**
 * @brief Initializes multi-assistant UI.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 * @retval #MA_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see ma_ui_deinitialize()
 */
int ma_ui_initialize(void);

/**
 * @brief Deinitializes multi-assistant UI.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @see ma_ui_initialize()
 */
int ma_ui_deinitialze(void);

/**
 * @brief Prepares multi-assistant UI.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 * @post If this function is called, the state will be #MA_STATE_READY.
 * @see ma_ui_unprepare()
 */
int ma_ui_prepare(void);

/**
 * @brief Unprepares multi-assistant UI.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_READY.
 * @post If this function is called, the state will be #MA_STATE_INITIALIZED.
 * @see ma_ui_prepare()
 */
int ma_ui_unprepare(void);

/**
 * @brief Gets the current state of the multi-assistant UI.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @param[out] state The current state
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 */
int ma_ui_get_state(ma_state_e* state);

/**
 * @brief Retrieves the information for assistants.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @param[in] callback A callback for getting the information of the assistants
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see ma_ui_assistant_info_cb()
 */
int ma_ui_foreach_assistant_info(ma_ui_assistant_info_cb callback, void* user_data);

/**
 * @brief Sets the default assistant.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @param[in] app_id The app ID of the assistant
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 */
int ma_ui_set_default_assistant(const char* app_id);

/**
 * @brief Changes the assistant.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @param[in] app_id The app ID for the new assistant
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 */
int ma_ui_change_assistant(const char* app_id);

/**
 * @brief Sets a state changed callback.
 * @since_tizen 5.0
 *
 * @param[in] callback The callback
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 * @see ma_state_changed_cb()
 * @see ma_unset_state_changed_cb()
 */
int ma_ui_set_state_changed_cb(ma_state_changed_cb callback, void* user_data);

/**
 * @brief Unsets a state changed callback.
 * @since_tizen 5.0
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 * @see ma_state_changed_cb()
 * @see ma_ui_set_state_changed_cb()
 */
int ma_ui_unset_state_changed_cb(void);

/**
 * @brief Sets an error callback.
 * @since_tizen 5.0
 *
 * @param[in] callback The callback
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 * @see ma_error_cb()
 * @see ma_ui_unset_error_cb()
 */
int ma_ui_set_error_cb(ma_error_cb callback, void* user_data);

/**
 * @brief Unsets an error callback.
 * @since_tizen 5.0
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 * @see ma_error_cb()
 * @see ma_ui_set_error_cb()
 */
int ma_ui_unset_error_cb(void);

/**
 * @brief Sets a callback for getting ASR result.
 * @since_tizen 5.0
 *
 * @param[in] callback The callback function
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 */
int ma_ui_set_asr_result_cb(ma_ui_asr_result_cb callback, void* user_data);

/**
 * @brief Unsets a callback for getting ASR result.
 * @since_tizen 5.0
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 */
int ma_ui_unset_asr_result_cb(void);

/**
 * @brief Sets a callback for getting results.
 * @since_tizen 5.0
 *
 * @param[in] callback The callback function
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 */
int ma_ui_set_result_cb(ma_ui_result_cb callback, void* user_data);

/**
 * @brief Unsets a callback for getting results.
 * @since_tizen 5.0
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 */
int ma_ui_unset_result_cb(void);

/**
 * @brief Sets a callback for getting change assistant info.
 * @since_tizen 5.0
 *
 * @param[in] callback The callback function
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 */
int ma_ui_set_change_assistant_cb(ma_ui_change_assistant_cb callback, void* user_data);

/**
 * @brief Unsets a callback for getting change assistant info.
 * @since_tizen 5.0
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 */
int ma_ui_unset_change_assistant_cb(void);

/**
 * @brief Sets a callback for getting recognition results.
 * @since_tizen 5.5
 *
 * @param[in] callback The callback function
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 */
int ma_ui_set_recognition_result_cb(ma_ui_recognition_result_cb callback, void* user_data);

/**
 * @brief Unsets a callback for getting recognition results.
 * @since_tizen 5.5
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 */
int ma_ui_unset_recognition_result_cb(void);

/**
 * @brief Sets a callback for getting common UI enabled status.
 * @since_tizen 5.5
 *
 * @param[in] callback The callback function
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 */
int ma_ui_set_enable_common_ui_cb(ma_ui_enable_common_ui_cb callback, void* user_data);

/**
 * @brief Unsets a callback for common UI enabled status.
 * @since_tizen 5.5
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 */
int ma_ui_unset_enable_common_ui_cb(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __TIZEN_UIFW_MULTI_ASSISTANT_UI_H__ */

