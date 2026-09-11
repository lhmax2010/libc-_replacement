/**
 * Copyright (c) 2011-2016 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __VOICE_CONTROL_H__
#define __VOICE_CONTROL_H__


#include <voice_control_command.h>
#include <voice_control_common.h>


/**
 * @addtogroup CAPI_UIX_VOICE_CONTROL_MODULE
 * @{
 */


#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @file voice_control.h
 * @brief This file contains the voice control client API and related callback definitions and enums.
 */


/**
 * @file voice_control_command.h
 * @brief This file contains the command list and command API and related handle definitions and enums.
 */


/**
 * @file voice_control_common.h
 * @brief This file contains the callback function definitions and enums.
 */


/**
 * @brief Definition for foreground command type.
 * @since_tizen 2.4
 */
#define VC_COMMAND_TYPE_FOREGROUND	1


/**
 * @brief Definition for background command type.
 * @since_tizen 2.4
 */
#define VC_COMMAND_TYPE_BACKGROUND	2


/**
 * @brief Definition for ended dialog.
 * @since_tizen 3.0
 */
#define VC_DIALOG_END		0


/**
 * @brief Definition for continued dialog.
 * @since_tizen 3.0
 */
#define VC_DIALOG_CONTINUE	1


/**
 * @brief Initializes voice control.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @remarks If the function succeeds, vc must be released with vc_deinitialize().
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #VC_ERROR_OPERATION_FAILED Operation failure
 * @post If this function is called, the state will be #VC_STATE_INITIALIZED.
 * @see vc_deinitialize()
 */
int vc_initialize(void);


/**
 * @brief Deinitializes voice control.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED Operation failure
 * @see vc_initialize()
 */
int vc_deinitialize(void);


/**
 * @brief Connects the voice control service.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED Operation failure
 * @pre The state should be #VC_STATE_INITIALIZED.
 * @post If this function is called, the state will be #VC_STATE_READY.
 * @see vc_unprepare()
 */
int vc_prepare(void);


/**
 * @brief Disconnects the voice control service.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_READY.
 * @post If this function is called, the state will be #VC_STATE_INITIALIZED.
 * @see vc_prepare()
 */
int vc_unprepare(void);


/**
 * @brief Retrieves all supported languages using callback function.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @param[in] callback Callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_OPERATION_FAILED Operation failure
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_INITIALIZED or #VC_STATE_READY.
 * @post This function invokes vc_supported_language_cb() repeatedly for getting languages.
 * @see vc_supported_language_cb()
 * @see vc_get_current_language()
 */
int vc_foreach_supported_languages(vc_supported_language_cb callback, void* user_data);


/**
 * @brief Gets current language.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @remarks If the function succeeds, @a language must be released with free() by you when you no longer need it.
 * @param[out] language A language is specified as an ISO 3166 alpha-2 two letter country-code
 *                      followed by ISO 639-1 for the two-letter language code.
 *                      For example, "ko_KR" for Korean, "en_US" for American English
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #VC_ERROR_OPERATION_FAILED Operation failure
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_INITIALIZED or #VC_STATE_READY.
 * @see vc_foreach_supported_languages()
 */
int vc_get_current_language(char** language);


/**
 * @brief Gets current state of voice control client.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @param[out] state The current state
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see vc_state_changed_cb()
 * @see vc_set_state_changed_cb()
 */
int vc_get_state(vc_state_e* state);


/**
 * @brief Gets current state of voice control service.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @param[out] state The current state
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_READY.
 * @see vc_set_service_state_changed_cb()
 * @see vc_unset_service_state_changed_cb()
 */
int vc_get_service_state(vc_service_state_e* state);


/**
 * @brief Gets the system command list.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @remarks In the system command list, there are system commands predefined by product manufacturers. Those commands have the highest priority.
 *          Therefore, the user can not set any commands same with the system commands.
 *          The @a vc_sys_cmd_list must be released using vc_cmd_list_destroy() when it is no longer required.
 * @param[out] vc_sys_cmd_list System command list handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The service state should be #VC_SERVICE_STATE_READY.
 * @see vc_cmd_list_destroy()
 */
int vc_get_system_command_list(vc_cmd_list_h* vc_sys_cmd_list);


/**
 * @brief Sets the invocation name.
 * @details Invocation name is used to activate background commands. The invocation name can be the same as the application name or any other phrase.
 *          For example, an application "Tizen Sample" has a background command, "Play music", and the invocation name of the application is set to "Tizen Sample".
 *          In order to activate the background command, users can say "Tizen Sample, Play music".
 *          The invocation name is dependent on the current language. For example, if the current language is "en_US"(English), the invocation name is also "en_US".
 *          If the current language is "ja_JP"(Japanese) and the invocation name is "en_US", the invocation name will not be recognized.
 *          This function should be called before vc_set_command_list().
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @remarks If @a name is @c NULL, the invocation name will be unset.
 * @param[in] name Invocation name that an application wants to be invoked by
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_READY.
 * @see vc_set_command_list()
 */
int vc_set_invocation_name(const char* name);


/**
 * @brief Requests to start the dialogue.
 * @details Using this function, the developer can request starting the dialogue to the framework.
 *          When the developer requests the dialogue, two types of texts, @a disp_text and @a utt_text, can be sent by this function.
 *          @a disp_text is a text for displaying, and @a utt_text is that for uttering.
 *          For example, if @a disp_text is "October 10th" and @a utt_text is "Today is October 10th.", "October 10th" will be displayed on the screen and "Today is October 10th." will be spoken.
 *          Also, the developer can set whether the dialogue starts automatically or not, using @a auto_start.
 *          If the developer sets @a auto_start as @c true, the framework will start to record next speech and continue the dialogue.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @remarks If @a auto_start is @c true, the recognition will start again. In this case, it can be restarted up to 4 times.
 *          @a disp_text and @a utt_text allow @c NULL. However, it is not allowed to set both @a disp_text and @a utt_text as @c NULL.
 * @param[in] disp_text Text to be displayed on the screen
 * @param[in] utt_text Text to be spoken
 * @param[in] auto_start A variable for setting whether the dialog session will be restarted automatically or not
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The service state should be #VC_SERVICE_STATE_READY.
 */
int vc_request_dialog(const char* disp_text, const char* utt_text, bool auto_start);


/**
 * @brief Sets command list.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @remarks The command type is valid for #VC_COMMAND_TYPE_FOREGROUND or #VC_COMMAND_TYPE_BACKGROUND.
 *          The matched commands of command list should be set and they should include type and command text at least.
 * @param[in] vc_cmd_list Command list handle
 * @param[in] type Command type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_READY.
 * @see vc_unset_command_list()
 */
int vc_set_command_list(vc_cmd_list_h vc_cmd_list, int type);


/**
 * @brief Unsets command list.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @param[in] type Command type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_READY.
 * @see vc_set_command_list()
 */
int vc_unset_command_list(int type);


/**
 * @brief Gets the recognition result.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @param[in] callback Callback function to get recognition result
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_READY.
 * @see vc_result_cb()
 */
int vc_get_result(vc_result_cb callback, void* user_data);


/**
 * @brief Sets a callback function for getting recognition result.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @param[in] callback Callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_INITIALIZED.
 * @see vc_result_cb()
 * @see vc_unset_result_cb()
 */
int vc_set_result_cb(vc_result_cb callback, void* user_data);


/**
 * @brief Unsets the callback function.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_INITIALIZED.
 * @see vc_set_result_cb()
 */
int vc_unset_result_cb(void);


/**
 * @brief Sets a callback function to be called when service state is changed.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @param[in] callback Callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_INITIALIZED.
 * @see vc_service_state_changed_cb()
 * @see vc_unset_service_state_changed_cb()
 */
int vc_set_service_state_changed_cb(vc_service_state_changed_cb callback, void* user_data);


/**
 * @brief Unsets the callback function.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_INITIALIZED.
 * @see vc_set_service_state_changed_cb()
 */
int vc_unset_service_state_changed_cb(void);


/**
 * @brief Sets a callback function to be called when state is changed.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @param[in] callback Callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_INITIALIZED.
 * @see vc_state_changed_cb()
 * @see vc_unset_state_changed_cb()
 */
int vc_set_state_changed_cb(vc_state_changed_cb callback, void* user_data);


/**
 * @brief Unsets the callback function.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_INITIALIZED.
 * @see vc_set_state_changed_cb()
 */
int vc_unset_state_changed_cb(void);


/**
 * @brief Sets a callback function to be called when current language is changed.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @param[in] callback Callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_INITIALIZED.
 * @see vc_current_language_changed_cb()
 * @see vc_unset_current_language_changed_cb()
 */
int vc_set_current_language_changed_cb(vc_current_language_changed_cb callback, void* user_data);


/**
 * @brief Unsets the callback function.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_INITIALIZED.
 * @see vc_set_current_language_changed_cb()
 */
int vc_unset_current_language_changed_cb(void);


/**
 * @brief Sets a callback function to be called when an error occurred.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @param[in] callback Callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_INITIALIZED.
 * @see vc_error_cb()
 * @see vc_unset_error_cb()
 */
int vc_set_error_cb(vc_error_cb callback, void* user_data);


/**
 * @brief Unsets the callback function.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_INITIALIZED.
 * @see vc_set_error_cb()
 */
int vc_unset_error_cb(void);


/**
 * @partner
 * @brief Requests to send TTS streaming data, asynchronously.
 * @details Using this function, the developer can request text to speech to the framework.
 *          When the developer requests the TTS with @a language, VC engine will send PCM data which is synthesized using VC engine's own persona.
 *          If @a to_vc_manager is true, the synthesized PCM data will be delivered to the VC manager, otherwise it will be delivered to the VC client
 *          For example, if @a text is "Alarm is set as 7 PM" and @a to_vc_manager is true, the PCM data corresponding "Alarm is set as 7 PM" will be delivered to VC manager client,
 *          and then it will be spoken in VC manager. If @a to_vc_manager is false, you will receive PCM data through the vc_tts_streaming_cb() callback function if it was set using vc_tts_set_streaming_cb().
 *          This function is executed asynchronously, so if there is an error while synthesizing, vc_error_cb() will be called.
 * @since_tizen 5.5
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/voicecontrol.tts
 * @param[in] text The text to be requested for TTS
 * @param[in] language The language is specified as an ISO 3166 alpha-2 two-letter country code
 *                     followed by ISO 639-1 for the two-letter language code.
 *                     For example, "ko_KR" for Korean, "en_US" for American English
 * @param[in] to_vc_manager The value for selection between VC client and VC manager\n
 *                          If @c true, the synthesized PCM data will be delivered to the VC manager, otherwise it will be delivered to the VC client
 * @param[out] utt_id The utterance id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED Operation failure
 * @pre The state should be #VC_STATE_READY.
 * @see vc_tts_cancel()
 */
int vc_tts_request(const char* text, const char* language, bool to_vc_manager, int* utt_id);


/**
 * @partner
 * @brief Requests to cancel TTS streaming data.
 * @since_tizen 5.5
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/voicecontrol.tts
 * @param[in] utt_id The utterance id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_READY.
 * @see vc_tts_request()
 */
int vc_tts_cancel(int utt_id);


/**
 * @partner
 * @brief Gets the TTS audio details.
 * @details Using this function, the developer can get details of synthesized audio data which is requested by vc_tts_request() function.
 * @since_tizen 5.5
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/voicecontrol.tts
 * @param[out] rate The audio sampling rate
 * @param[out] channel The audio channel
 * @param[out] audio_type The audio type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_READY.
 */
int vc_tts_get_synthesized_audio_details(int* rate, vc_audio_channel_e* channel, vc_audio_type_e* audio_type);


/**
 * @partner
 * @brief Sets the TTS streaming callback function.
 * @since_tizen 5.5
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/voicecontrol.tts
 * @param[in] callback The callback function
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_INITIALIZED.
 */
int vc_tts_set_streaming_cb(vc_tts_streaming_cb callback, void* user_data);


/**
 * @partner
 * @brief Unsets the TTS streaming callback function.
 * @since_tizen 5.5
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/voicecontrol.tts
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_INITIALIZED.
 */
int vc_tts_unset_streaming_cb(void);


/**
 * @partner
 * @brief Sets the TTS utterance status callback function.
 * @details Using this function, the developer can set the utterance status callback to be called
 *          when the VC manager client starts or stops playing TTS PCM data which was requested to be synthesized with the vc_tts_request() function.
 *          This function is called when to_vc_manager in the vc_tts_request() function call is @c true.
 * @since_tizen 5.5
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/voicecontrol.tts
 * @param[in] callback The callback function
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED Operation failure
 * @pre The state should be #VC_STATE_INITIALIZED.
 */
int vc_tts_set_utterance_status_cb(vc_tts_utterance_status_cb callback, void* user_data);


/**
 * @partner
 * @brief Unsets the TTS utterance status callback function.
 * @since_tizen 5.5
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/voicecontrol.tts
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #VC_STATE_INITIALIZED.
 */
int vc_tts_unset_utterance_status_cb(void);


#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __VOICE_CONTROL_H__ */
