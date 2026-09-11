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


#ifndef __TIZEN_UIFW_MULTI_ASSISTANT_SETTINGS_H__
#define __TIZEN_UIFW_MULTI_ASSISTANT_SETTINGS_H__

/**
 * @addtogroup CAPI_UIX_MULTI_ASSISTANT_MODULE
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Checks multi-assistant's multiple mode.
 * @since_tizen 5.5
 * @param[out] multiple The current multiple mode of multi-assistant
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 */
int ma_settings_is_multiple_mode(bool *multiple);

/**
 * @brief Sets multi-assistant's multiple mode.
 * @since_tizen 5.5
 * @param[in] multiple The multiple mode of multi-assistant need to be changed into
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 */
int ma_settings_set_multiple_mode(bool multiple);

/**
 * @brief Gets system's current voice assistant.
 * @since_tizen 5.5
 * @remarks Valid only in single voice assistant mode.
 * @remarks The @a app_id should be released using free().
 * @param[out] app_id The app id of the currently activated voice assistant
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 */
int ma_settings_get_current_voice_assistant(char** app_id);

/**
 * @brief Changes system's voice assistant.
 * @since_tizen 5.5
 * @remarks Valid only in single voice assistant mode.
 * @param[in] app_id The app id of the voice assistant that need to be activated
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 */
int ma_settings_change_voice_assistant(const char* app_id);

/**
 * @brief Gets whether the specific voice assistant should be enabled or not.
 * @since_tizen 7.0
 * @param[in] app_id The app id of the voice assistant
 * @param[out] enabled The enabled state of the voice assistant
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 */
int ma_settings_get_voice_assistant_enabled(const char* app_id, bool *enabled);

/**
 * @brief En/Disables a specific voice assistant.
 * @since_tizen 5.5
 * @remarks Valid only in multiple voice assistant mode.
 * @param[in] app_id The app id of the voice assistant that need to be changed
 * @param[in] enabled The voice assistant need to be enabled or not
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ma_settings_set_voice_assistant_enabled(const char* app_id, bool enabled);

/**
 * @brief Retrieves system's default voice assistant.
 * @since_tizen 5.5
 * @remarks Valid only in multiple voice assistant mode.
 * @remarks The @a app_id should be released using free().
 * @param[out] app_id The app id of the voice assistant currently set as default one
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 */
int ma_settings_get_default_voice_assistant(char** app_id);

/**
 * @brief Changes system's default voice assistant.
 * @since_tizen 5.5
 * @remarks Valid only in multiple voice assistant mode.
 * @param[in] app_id The app id of the voice assistant that need to set as default one
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 */
int ma_settings_set_default_voice_assistant(const char* app_id);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __TIZEN_UIFW_MULTI_ASSISTANT_SETTINGS_H__ */
