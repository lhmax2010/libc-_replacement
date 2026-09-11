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


#ifndef __TIZEN_UIFW_MULTI_ASSISTANT_INTERNAL_H__
#define __TIZEN_UIFW_MULTI_ASSISTANT_INTERNAL_H__

#include <multi_assistant_common.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief An alias for 'ma_set_assistant_wakeup_language' function.
 * @details Since the function name has been change from 'ma_set_assistant_language'
 *          to 'ma_set_assistant_wakeup_language' during the API review phase,
 *          this function should be provided for supporting backward compatibility.
 *          It will be removed all references are replaced with the newly added API.
 * @since_tizen 5.5
 *
 * @param[in] language The language configuration that will be used for wake word detection.
 *                     It should be denoted by two-letter code defined by IS 639-1,
 *                     optionally combined with two-letter code defined by ISO 3166.
 *                     For example, "ko_KR" for Korean, "en_US" for American English.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_READY.
 */
int ma_set_assistant_language(const char* language);

/**
 * @brief Initializes multi-assistant AP.
 * @since_tizen 6.0
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
 * @see ma_ap_deinitialize()
 */
int ma_ap_initialize(void);

/**
 * @brief Deinitializes multi-assistant AP.
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @see ma_ap_initialize()
 */
int ma_ap_deinitialize(void);

/**
 * @brief Sets an error callback.
 * @since_tizen 6.0
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
 * @see ma_ap_unset_error_cb()
 */
int ma_ap_set_error_cb(ma_error_cb callback, void* user_data);

/**
 * @brief Unsets an error callback.
 * @since_tizen 6.0
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 * @see ma_error_cb()
 * @see ma_ap_set_error_cb()
 */
int ma_ap_unset_error_cb(void);

/**
 * @brief Sets an audio streaming callback.
 * @since_tizen 6.0
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
 * @see ma_audio_streaming_cb()
 * @see ma_ap_unset_audio_streaming_cb()
 */
int ma_ap_set_audio_streaming_cb(ma_audio_streaming_cb callback, void* user_data);

/**
 * @brief Unsets an audio streaming callback.
 * @since_tizen 6.0
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 * @see ma_audio_streaming_cb()
 * @see ma_ap_set_audio_streaming_cb()
 */
int ma_ap_unset_audio_streaming_cb(void);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_UIFW_MULTI_ASSISTANT_INTERNAL_H__ */
