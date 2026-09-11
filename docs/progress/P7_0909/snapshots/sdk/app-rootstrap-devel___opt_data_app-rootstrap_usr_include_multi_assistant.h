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


#ifndef __TIZEN_UIFW_MULTI_ASSISTANT_H__
#define __TIZEN_UIFW_MULTI_ASSISTANT_H__

#include <multi_assistant_common.h>

/**
 * @defgroup CAPI_UIX_MULTI_ASSISTANT_CLIENT_MODULE Multi assistant client
 * @ingroup CAPI_UIX_MULTI_ASSISTANT_MODULE
 * @brief The @ref CAPI_UIX_MULTI_ASSISTANT_CLIENT_MODULE API provides functions for getting queries from multi assistant service and sending responses.
 * @{
 */


#ifdef __cplusplus
extern "C"
{
#endif



/**
 * @brief Initializes multi-assistant client.
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
 * @post If this function is called, the state will be #MA_STATE_INITIALIZED.
 * @see ma_deinitialize()
 */
int ma_initialize(void);

/**
 * @brief Deinitializes multi-assistant client.
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
 * @post If this function is called, the state will be #MA_STATE_NONE.
 * @see ma_initialize()
 */
int ma_deinitialize(void);

/**
 * @brief Prepares multi-assistant client.
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
 * @see ma_unprepare()
 */
int ma_prepare(void);

/**
 * @brief Unprepares multi-assistant client.
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
 * @see ma_prepare()
 */
int ma_unprepare(void);

/**
 * @brief Gets the current state of the multi-assistant client.
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
int ma_get_state(ma_state_e* state);

/**
 * @brief Gets the current language of multi-assistant client.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @remarks The @a language should be released using free().
 * @param[out] language The current language
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ma_get_current_language(char** language);

/**
 * @brief Gets the recording audio format.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @param[out] rate The audio sampling rate
 * @param[out] channel The audio channel
 * @param[out] audio_type The audio type
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre The state should be #MA_STATE_READY.
 */
int ma_get_recording_audio_format(int* rate, ma_audio_channel_e* channel, ma_audio_type_e* audio_type);

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
int ma_set_state_changed_cb(ma_state_changed_cb callback, void* user_data);

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
 * @see ma_set_state_changed_cb()
 */
int ma_unset_state_changed_cb(void);

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
 * @see ma_unset_error_cb()
 */
int ma_set_error_cb(ma_error_cb callback, void* user_data);

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
 * @see ma_set_error_cb()
 */
int ma_unset_error_cb(void);

/**
 * @brief Sets a language changed callback.
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
 * @see ma_language_changed_cb()
 * @see ma_unset_language_changed_cb()
 */
int ma_set_language_changed_cb(ma_language_changed_cb callback, void* user_data);

/**
 * @brief Unsets a language changed callback.
 * @since_tizen 5.0
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 * @see ma_language_changed_cb()
 * @see ma_set_language_changed_cb()
 */
int ma_unset_language_changed_cb(void);

/**
 * @brief Sets an audio streaming callback.
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
 * @see ma_audio_streaming_cb()
 * @see ma_unset_audio_streaming_cb()
 */
int ma_set_audio_streaming_cb(ma_audio_streaming_cb callback, void* user_data);

/**
 * @brief Unsets an audio streaming callback.
 * @since_tizen 5.0
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 * @see ma_audio_streaming_cb()
 * @see ma_set_audio_streaming_cb()
 */
int ma_unset_audio_streaming_cb(void);

/**
 * @brief Sends ASR(auto speech recognition) results to the multi-assistant service.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @param[in] event The ASR result event (e.g. #MA_ASR_RESULT_EVENT_FINAL_RESULT)
 * @param[in] asr_result The ASR result text
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ma_send_asr_result(ma_asr_result_event_e event, const char* asr_result);

/**
 * @brief Sends results to the multi-assistant service.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @param[in] display_text The text shown on the display
 * @param[in] utterance_text The utterance text
 * @param[in] result_json The result data (JSON format)
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ma_send_result(const char* display_text, const char* utterance_text, const char* result_json);

/**
 * @brief Sends recognition result to the multi-assistant service.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @param[in] result The recognition result
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ma_send_recognition_result(ma_recognition_result_event_e result);

/**
 * @brief Sets the active state changed callback.
 * @since_tizen 5.5
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
 * @see ma_active_state_changed_cb()
 * @see ma_unset_active_state_changed_cb()
 */
int ma_set_active_state_changed_cb(ma_active_state_changed_cb callback, void* user_data);

/**
 * @brief Unsets the active state changed callback.
 * @since_tizen 5.5
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 * @see ma_active_state_changed_cb()
 * @see ma_set_active_state_changed_cb()
 */
int ma_unset_active_state_changed_cb(void);

/**
 * @brief Starts receiving audio streaming data.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @param[in] type The type of audio data to start streaming
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ma_start_receiving_audio_streaming_data(ma_audio_streaming_data_type_e type);

/**
 * @brief Stops receiving audio streaming data.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @param[in] type The type of audio data to stop streaming
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ma_stop_receiving_audio_streaming_data(ma_audio_streaming_data_type_e type);

/**
 * @brief Updates the state of client's voice feedback to the server.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @param[in] state The current state of voice feedback
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ma_update_voice_feedback_state(ma_voice_feedback_state_e state);

/**
 * @brief Sends an assistant-specific command to the server.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @param[in] command The command to be sent to the server
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ma_send_assistant_specific_command(const char* command);

/**
 * @brief Sets the wakeup engine command callback.
 * @details The wakeup engine command callback will be called when the wakeup engine
 *          sends wakeup engine specific command to the voice assistant.
 *          The command MUST be defined by both wakeup engine and voice assistant.
 *
 * @since_tizen 5.5
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
 * @see ma_wakeup_engine_command_cb()
 * @see ma_unset_wakeup_engine_command_cb()
 */
int ma_set_wakeup_engine_command_cb(ma_wakeup_engine_command_cb callback, void* user_data);

/**
 * @brief Unsets the callback.
 * @since_tizen 5.5
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 * @see ma_wakeup_engine_command_cb()
 * @see ma_set_wakeup_engine_command_cb()
 */
int ma_unset_wakeup_engine_command_cb(void);

/**
 * @brief Retrieves the information about all installed voice assistant applications.
 * @since_tizen 5.5
 *
 * @param[in] callback The callback for getting the information of installed assistants.
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @see ma_assistant_info_list_cb()
 */
int ma_assistant_info_foreach_assistants(ma_assistant_info_list_cb callback, void* user_data);

/**
 * @brief Gets the app id of the specified handle.
 * @since_tizen 5.5
 *
 * @remarks You must not release @a app_id using free().
 * @param[in] handle The handle to the assistant's information
 * @param[out] app_id The application ID of the given assistant handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ma_assistant_info_get_app_id(ma_assistant_info_h handle, char** app_id);

/**
 * @brief Gets the enabled status of the specified handle.
 * @since_tizen 5.5
 *
 * @param[in] handle The handle to the assistant's information
 * @param[out] status The enable status of the given assistant handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ma_assistant_info_get_enabled_status(ma_assistant_info_h handle, bool* status);

/**
 * @brief Gets the recording audio source type.
 * @since_tizen 5.5
 *
 * @remarks You must release @a type using free().
 * @param[out] type The audio source type
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre The state should be #MA_STATE_READY.
 */
int ma_get_recording_audio_source_type(char** type);

/**
 * @brief Sets the background volume.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/volume.set
 *
 * @remarks The @a ratio must be a value between 0.0 and 1.0 (inclusive).
 * @param[in] ratio The volume ratio to be set
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre The state should be #MA_STATE_READY.
 */
int ma_set_background_volume(double ratio);

/**
 * @brief Sets the preprocessing allow mode.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @remarks If the @a app_id is NULL, allow mode will be applied regardless of app_id.
 * @param[in] mode The preprocessing allow mode to be set
 * @param[in] app_id The application ID of the assistant to perform preprocessing
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre The state should be #MA_STATE_READY.
 */
int ma_set_preprocessing_allow_mode(ma_preprocessing_allow_mode_e mode, const char* app_id);

/**
 * @brief Sets the preprocessing information changed callback.
 * @since_tizen 5.5
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
 * @see ma_preprocessing_information_changed_cb()
 * @see ma_unset_preprocessing_information_changed_cb()
 */
int ma_set_preprocessing_information_changed_cb(ma_preprocessing_information_changed_cb callback, void* user_data);

/**
 * @brief Unsets the preprocessing information changed callback.
 * @since_tizen 5.5
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 * @see ma_preprocessing_information_changed_cb()
 * @see ma_set_preprocessing_information_changed_cb()
 */
int ma_unset_preprocessing_information_changed_cb(void);

/**
 * @brief Sends the preprocessing result.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @param[in] is_success The result value to be sent, indicating whether the preprocessing succeeded or not
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre The state should be #MA_STATE_READY.
 */
int ma_send_preprocessing_result(bool is_success);

/**
 * @brief Sets whether multi-assistant service should send wake word audio data.
 * @details If set to true, wake word audio data will be included in audio streaming data.<br/>
 *          The wake word is the way you initiate a conversation with your vocal assistant.
 *          It is a predefined keyword automatically detected from continuously streaming audio.<br/>
 *          Use ma_set_audio_streaming_data_section_changed_cb() to be informed about
 *          switching between wake word and regular audio data.
 * @since_tizen 5.5
 *
 * @param[in] require The require value to be set
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre The state should be #MA_STATE_READY.
 */
int ma_set_wake_word_audio_require_flag(bool require);

/**
 * @brief Sets the section changed callback for audio streaming data.
 * @since_tizen 5.5
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
 * @see ma_audio_streaming_data_section_changed_cb()
 * @see ma_unset_audio_streaming_data_section_changed_cb()
 */
int ma_set_audio_streaming_data_section_changed_cb(ma_audio_streaming_data_section_changed_cb callback, void* user_data);

/**
 * @brief Unsets the section changed callback for audio streaming data.
 * @since_tizen 5.5
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 * @see ma_audio_streaming_data_section_changed_cb()
 * @see ma_set_audio_streaming_data_section_changed_cb()
 */
int ma_unset_audio_streaming_data_section_changed_cb(void);

/**
 * @brief Sets the language configuration for wake word detection.
 * @details The language configuration of the assistant that invoked
 *          this function will be changed and language configurations
 *          of other assistants will remain unchanged.
 * @since_tizen 6.0
 *
 * @param[in] language The language configuration that will be used for wake word detection.
 *                     It should be denoted by two-letter code defined by ISO 639-1,
 *                     optionally combined with two-letter code defined by ISO 3166.
 *                     For example, "ko_KR" for Korean, "en_US" for American English.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_READY.
 */
int ma_set_assistant_wakeup_language(const char* language);

/**
 * @brief Sets the service state changed callback.
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
 * @see ma_service_state_changed_cb()
 * @see ma_unset_service_state_changed_cb()
 */
int ma_set_service_state_changed_cb(ma_service_state_changed_cb callback, void* user_data);

/**
 * @brief Unsets the service state changed callback.
 * @since_tizen 6.0
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 * @see ma_service_state_changed_cb()
 * @see ma_set_service_state_changed_cb()
 */
int ma_unset_service_state_changed_cb(void);

/**
 * @brief Sets a voice key status changed callback.
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
 * @see ma_voice_key_status_changed_cb()
 * @see ma_unset_voice_key_status_changed_cb()
 */
int ma_set_voice_key_status_changed_cb(ma_voice_key_status_changed_cb callback, void* user_data);

/**
 * @brief Unsets a voice key status changed callback.
 * @since_tizen 6.0
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #MA_STATE_INITIALIZED.
 * @see ma_set_voice_key_status_changed_cb()
 * @see ma_voice_key_status_changed_cb()
 */
int ma_unset_voice_key_status_changed_cb(void);

/**
 * @brief Adds a group of wake words for activating assistant.
 * @details A <word, language> pair with the same word and the different language
 *          identifies a different operation.
 *          For example, <"Hi Tizen", "en_US"> and <"Hi Tizen", "en_GB">
 *          will operate differently. When the wake up language is set to "en_GB",
 *          although  <"Hi Tizen", "en_US"> is added, then
 *          "Hi Tizen" won't wake up the Tizen voice assistant.
 *          <"Hi Tizen", "en_GB"> have to be added for this use case.\n
 *          Adding the same word twice does not return any errors.
 * @since_tizen 6.0
 *
 * @param[in] wake_word The wake word to be added to the list of wake words
 * @param[in] language The language code for which the word will be added.
 *                     The language is identified by its code (e.g. "en_US").
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre The state should be #MA_STATE_READY.
 */
int ma_add_wake_word(const char* wake_word, const char *language);

/**
 * @brief Removes a group of wake words for activating assistant.
 * @details A <word, language> pair with the same word and the different language
 *          identifies a different operation.
 *          For example, <"Hi Tizen", "en_US"> and <"Hi Tizen", "en_GB">
 *          will operate differently.
 *          Removing an unregistered word doesn't return any errors.
 * @since_tizen 6.0
 *
 * @param[in] wake_word The wake word to be removed from the list of wake words
 * @param[in] language The language code for which the word will be removed.
 *                     The language is identified by its code (e.g. "en_US").
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre The state should be #MA_STATE_READY.
 */
int ma_remove_wake_word(const char* wake_word, const char *language);

/**
 * @brief Retrieves wake word information of the last wakeup event.
 * @since_tizen 7.0
 *
 * @remarks You must release @a wake_word using free().
 *          @a wake_word can be changed whenever ma_active_state_changed_cb() is called.
 * @param[out] wake_word The wake word information, if exists.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre The state should be #MA_STATE_READY.
 * @see ma_active_state_changed_cb()
 */
int ma_get_wakeup_info_wake_word(char** wake_word);

/**
 * @brief Retrieves extra data information of the last wakeup event.
 * @details When required, wakeup engine adds extra data information
 *          that can be processed by the voice assistant.
 *          The information MUST be defined by both wakeup engine and voice assistant.
 * @since_tizen 7.0
 *
 * @remarks You must release @a extra_data and @a extra_data_desc using free().
 *          @a extra_data can be changed whenever ma_active_state_changed_cb() is called.
 * @param[out] extra_data The extra data information, if exists.
 * @param[out] extra_data_length The length of extra data.
 * @param[out] extra_data_desc The string value describing the type of extra data.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MA_ERROR_NONE Successful
 * @retval #MA_ERROR_NOT_SUPPORTED Not supported
 * @retval #MA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MA_ERROR_INVALID_STATE Invalid state
 * @retval #MA_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre The state should be #MA_STATE_READY.
 * @see ma_active_state_changed_cb()
 */
int ma_get_wakeup_info_extra_data(unsigned char** extra_data, int* extra_data_length, char **extra_data_desc);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __TIZEN_UIFW_MULTI_ASSISTANT_H__ */
