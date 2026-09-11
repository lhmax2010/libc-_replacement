/**
 * Copyright (c) 2011-2018 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __VOICE_CONTROL_MANAGER_H__
#define __VOICE_CONTROL_MANAGER_H__

#include <voice_control_command.h>
#include <voice_control_common.h>

/**
 * @addtogroup CAPI_UIX_VOICE_CONTROL_MANAGER_MODULE
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Definition of Bluetooth audio-in type.
 * @since_tizen 5.0
 */
#define VC_AUDIO_TYPE_BLUETOOTH		"VC_AUDIO_ID_BLUETOOTH"		/**< Bluetooth audio type */

/**
 * @brief Definition of Wi-Fi audio-in type.
 * @since_tizen 5.0
 */
#define VC_AUDIO_TYPE_WIFI		"VC_AUDIO_ID_WIFI"		/**< Wi-Fi audio type */

/**
 * @brief Definition for foreground command type.
 * @since_tizen 5.0
 */
#define VC_COMMAND_TYPE_FOREGROUND	1

/**
 * @brief Definition for background command type.
 * @since_tizen 5.0
 */
#define VC_COMMAND_TYPE_BACKGROUND	2

/**
 * @brief Definition for widget command type.
 * @since_tizen 5.0
 */
#define VC_COMMAND_TYPE_WIDGET		3

/**
 * @brief Definition for system command type.
 * @since_tizen 5.0
 */
#define VC_COMMAND_TYPE_SYSTEM		4

/**
 * @brief Definition for system background command type.
 * @since_tizen 5.0
 */
#define VC_COMMAND_TYPE_SYSTEM_BACKGROUND	5

/**
 * @brief Definition for exclusive command type.
 * @since_tizen 5.0
 */
#define VC_COMMAND_TYPE_EXCLUSIVE	6

/**
 * @brief Definition for none message.
 * @since_tizen 5.0
 */
#define VC_RESULT_MESSAGE_NONE			"vc.result.message.none"

/**
 * @brief Definition for failed recognition because the speech is too loud to listen.
 * @since_tizen 5.0
 */
#define VC_RESULT_MESSAGE_ERROR_TOO_LOUD	"vc.result.message.error.too.loud"

/**
 * @brief Enumeration of recognition mode.
 * @since_tizen 5.0
 */
typedef enum {
	VC_RECOGNITION_MODE_STOP_BY_SILENCE,		/**< Default mode */
	VC_RECOGNITION_MODE_RESTART_AFTER_REJECT,	/**< Restart recognition after rejected result */
	VC_RECOGNITION_MODE_RESTART_CONTINUOUSLY,	/**< Continuously restart recognition - not support yet*/
	VC_RECOGNITION_MODE_MANUAL					/**< Start and stop manually without silence */
} vc_recognition_mode_e;

/**
 * @brief Enumeration of send event type.
 * @since_tizen 5.0
 */
typedef enum {
	VC_SEND_EVENT_TYPE_TEXT,			/**< Send text event to VC engine */
	VC_SEND_EVENT_TYPE_LIST_EVENT,		/**< Send list event to VC engine */
	VC_SEND_EVENT_TYPE_HAPTIC_EVENT		/**< Send haptic event to VC engine */
} vc_send_event_type_e;

/**
 * @brief Enumeration of pre result event.
 * @since_tizen 5.0
 */
typedef enum {
	VC_PRE_RESULT_EVENT_FINAL_RESULT = 0,	/**< Pre-result event for final result */
	VC_PRE_RESULT_EVENT_PARTIAL_RESULT,		/**< Pre-result event for partial result */
	VC_PRE_RESULT_EVENT_ERROR				/**< Pre-result event for error */
} vc_pre_result_event_e;

/**
 * @brief Enumeration of audio streaming event.
 * @since_tizen 6.0
 */
typedef enum {
	VC_AUDIO_STREAMING_EVENT_FAIL = -1,		/**< Failed */
	VC_AUDIO_STREAMING_EVENT_START = 1,		/**< Start event */
	VC_AUDIO_STREAMING_EVENT_CONTINUE = 2,	/**< Continue event */
	VC_AUDIO_STREAMING_EVENT_FINISH = 3		/**< Finish event */
} vc_audio_streaming_event_e;

/**
 * @brief Enumeration of audio streaming mode.
 * @since_tizen 6.0
 */
typedef enum {
	VC_AUDIO_STREAMING_MODE_VC_SERVICE = 0,			/**< Use audio recording from VC service */
	VC_AUDIO_STREAMING_MODE_MULTI_ASSISTANT = 1,	/**< Use audio streaming from multi-assistant */
	VC_AUDIO_STREAMING_MODE_OUTSIDE = 2,			/**< Use audio streaming from outside */
} vc_audio_streaming_mode_e;

/**
 * @brief Enumeration of background volume event.
 * @since_tizen 6.0
 */
typedef enum {
	VC_BACKGROUND_VOLUME_EVENT_CHANGE_FOR_NEARFIELD = 0,	/**< Background volume event for near-field */
	VC_BACKGROUND_VOLUME_EVENT_CHANGE_FOR_FARFIELD			/**< Background volume event for far-field */
} vc_background_volume_event_e;

/**
 * @brief Called when client gets the all recognition results from voice control service.
 * @since_tizen 5.0
 *
 * @remarks The @a vc_cmd_list should not be released. It is managed by the framework and will be released when invoking this callback is finished. \n
 *      The @a result and @a msg are also managed by the framework and will be released when invoking this callback is finished.
 *
 * @param[in] event         The result event
 * @param[in] vc_cmd_list   Command list handle
 * @param[in] result        Command text
 * @param[in] msg           Engine message (e.g. #VC_RESULT_MESSAGE_NONE, #VC_RESULT_MESSAGE_ERROR_TOO_LOUD)
 * @param[in] user_data     The user data passed from the callback registration function
 *
 * @return @c true to release command to client, \n @c false to wait for selecting command.
 * @pre An application registers callback function using vc_mgr_set_all_result_cb().
 *
 * @see vc_mgr_set_all_result_cb()
 * @see vc_mgr_unset_all_result_cb()
 */
typedef bool (*vc_mgr_all_result_cb)(vc_result_event_e event, vc_cmd_list_h vc_cmd_list,
				const char *result, const char *msg, void *user_data);

/**
 * @brief Called when client gets the pre recognition results (partial ASR) from voice control service.
 * @since_tizen 5.0
 *
 * @remarks The @a result is managed by the framework and will be released when invoking this callback is finished.
 *
 * @param[in] event       The pre result event
 * @param[in] result      ASR text
 * @param[in] user_data   The user data passed from the callback registration function
 *
 * @pre An application registers callback function using vc_mgr_set_pre_result_cb().
 *
 * @see vc_mgr_set_pre_result_cb()
 * @see vc_mgr_unset_pre_result_cb()
 */
typedef void (*vc_mgr_pre_result_cb)(vc_pre_result_event_e event, const char *result, void *user_data);

/**
 * @brief Called when client gets the specific engine's result from vc-service.
 * @since_tizen 5.0
 *
 * @remarks The @a engine_app_id, @a event and @a result are managed by the framework and will be released when invoking this callback is finished.
 *
 * @param[in] engine_app_id The specific engine's app id
 * @param[in] event The specific engine event type
 * @param[in] result The specific engine result
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @pre An application registers callback function using vc_mgr_set_specific_engine_result_cb().
 *
 * @see vc_mgr_set_specific_engine_result_cb()
 * @see vc_mgr_unset_specific_engine_result_cb()
 */
typedef void (*vc_mgr_specific_engine_result_cb)(const char* engine_app_id, const char* event, const char* result, void *user_data);

/**
 * @brief Called when user speaking is detected.
 * @since_tizen 5.0
 *
 * @param[in] user_data   The user data passed from the callback registration function
 *
 * @pre An application registers callback function using vc_mgr_set_speech_detected_cb().
 *
 * @see vc_mgr_set_speech_detected_cb()
 * @see vc_mgr_unset_speech_detected_cb()
 */
typedef void (*vc_mgr_begin_speech_detected_cb)(void *user_data);


/**
 * @brief Called when an application (VC client) requests dialog.
 * @since_tizen 5.0
 *
 * @remarks The @a disp_text and @a utt_text are managed by the framework and will be released when invoking this callback is finished.
 *
 * @param[in] pid          Pid of VC client to request dialog
 * @param[in] disp_text    Text requested to be displayed
 * @param[in] utt_text     Text requested to be spoken
 * @param[in] continuous   Continue dialog session
 * @param[in] user_data    The user data passed from the callback registration function
 *
 * @pre An application registers callback function using vc_mgr_set_dialog_request_cb().
 *
 * @see vc_mgr_set_dialog_request_cb()
 * @see vc_mgr_unset_dialog_request_cb()
 */
typedef void (*vc_mgr_dialog_request_cb)(int pid, const char *disp_text, const char *utt_text, bool continuous, void *user_data);

/**
 * @brief Called when engine sets private data to manager client.
 * @since_tizen 5.0
 *
 * @remarks The @a key and @a data are managed by the framework and will be released when invoking this callback is finished.
 *
 * @param[in] key         Private key
 * @param[in] data        Private data
 * @param[in] user_data   The user data passed from the callback registration function
 *
 * @pre An application registers callback function using vc_mgr_set_private_data_set_cb().
 *
 * @see vc_mgr_set_private_data_set_cb()
 * @see vc_mgr_unset_private_data_set_cb()
 */
typedef int (*vc_mgr_private_data_set_cb)(const char *key, const char *data, void *user_data);

/**
 * @brief Called when engine requests private data from manager client.
 * @since_tizen 5.0
 *
 * @remarks The @a data should not be released. The @a data and @a key are managed by the framework and will be released when invoking this callback is finished.
 *
 * @param[in]  key         Private key
 * @param[out] data        Private data
 * @param[in]  user_data   The user data passed from the callback registration function
 *
 * @pre An application registers callback function using vc_mgr_set_private_data_requested_cb().
 *
 * @see vc_mgr_set_private_data_requested_cb()
 * @see vc_mgr_unset_private_data_requested_cb()
 */
typedef int (*vc_mgr_private_data_requested_cb)(const char *key, char **data, void *user_data);

/* for TTS feedback */
/**
 * @brief Called when engine sends audio formats necessary for playing TTS feedback.
 * @since_tizen 5.0
 *
 * @param[in] rate Audio sampling rate
 * @param[in] channel Audio channel (e.g. #VC_AUDIO_CHANNEL_MONO, #VC_AUDIO_CHANNEL_STEREO)
 * @param[in] audio_type Audio type (e.g. #VC_AUDIO_TYPE_PCM_S16_LE, #VC_AUDIO_TYPE_PCM_U8)
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @pre An application registers callback function using vc_mgr_set_feedback_audio_format_cb().
 *
 * @see vc_mgr_set_feedback_audio_format_cb()
 * @see vc_mgr_unset_feedback_audio_format_cb()
 */
typedef void (*vc_mgr_feedback_audio_format_cb)(int rate, vc_audio_channel_e channel, vc_audio_type_e audio_type, void *user_data);

/**
 * @brief Called when engine sends audio streaming for TTS feedback.
 * @since_tizen 5.0
 *
 * @remarks The @a buffer must be released with free() by you when you no longer need it.
 *
 * @param[in] event TTS feedback event (e.g. #VC_FEEDBACK_EVENT_START, #VC_FEEDBACK_EVENT_CONTINUE)
 * @param[in] buffer Audio streaming data
 * @param[in] len Length of the audio streaming data
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @pre An application registers callback function using vc_mgr_set_feedback_streaming_cb().
 *
 * @see vc_mgr_set_feedback_streaming_cb()
 * @see vc_mgr_unset_feedback_streaming_cb()
 */
typedef void (*vc_mgr_feedback_streaming_cb)(vc_feedback_event_e event, char* buffer, int len, void *user_data);

/**
 * @brief Called when the vc client sends audio streaming for TTS feedback.
 * @since_tizen 5.0
 *
 * @remarks The @a buffer must be released with free() by you when you no longer need it.
 *
 * @param[in] pid The process id of the vc client
 * @param[in] utt_id The utterance id
 * @param[in] event TTS feedback event (e.g. #VC_FEEDBACK_EVENT_START, #VC_FEEDBACK_EVENT_CONTINUE)
 * @param[in] buffer Audio streaming data
 * @param[in] len Length of the audio streaming data
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @pre An application registers callback function using vc_mgr_set_vc_tts_streaming_cb().
 *
 * @see vc_mgr_set_vc_tts_streaming_cb()
 * @see vc_mgr_unset_vc_tts_streaming_cb()
 */
typedef void (*vc_mgr_vc_tts_streaming_cb)(int pid, int utt_id, vc_feedback_event_e event, char* buffer, int len, void *user_data);

/**
 * @platform
 * @brief Initializes the voice control manager.
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks If the function succeeds, VC manager must be released with vc_mgr_deinitialize().
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_OUT_OF_MEMORY       Out of memory
 * @retval #VC_ERROR_OPERATION_FAILED    Operation fail
 *
 * @pre The state should be #VC_STATE_NONE.
 * @post If this function is called, the state will be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_deinitialize()
 */
int vc_mgr_initialize(void);

/**
 * @platform
 * @brief Deinitializes the voice control manager.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @post If this function is called, the state will be #VC_STATE_NONE.
 *
 * @see vc_mgr_initialize()
 */
int vc_mgr_deinitialize(void);

/**
 * @platform
 * @brief Connects the voice control service.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 * @post If this function is called, the state will be #VC_STATE_READY.
 *
 * @see vc_mgr_unprepare()
 */
int vc_mgr_prepare(void);

/**
 * @platform
 * @brief Disconnects the voice control service.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_READY.
 * @post If this function is called, the state will be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_prepare()
 */
int vc_mgr_unprepare(void);

/**
 * @platform
 * @brief Retrieves all supported languages using callback function.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] callback    Callback function to invoke
 * @param[in] user_data   The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_OPERATION_FAILED    Operation failure
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should NOT be #VC_SERVICE_STATE_NONE.
 * @post This function invokes vc_supported_language_cb() for each supported language.
 *
 * @see vc_supported_language_cb()
 * @see vc_mgr_get_current_language()
 */
int vc_mgr_foreach_supported_languages(vc_supported_language_cb callback, void *user_data);

/**
 * @platform
 * @brief Gets the current language set by user.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks If the function succeeds, @a language must be released with free() by you when you no longer need it.
 *
 * @param[out] language   A language is specified as an ISO 3166 alpha-2 two letter country-code \n
 *      followed by ISO 639-1 for the two-letter language code. \n
 *      For example, "ko_KR" for Korean, "en_US" for American English.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_OUT_OF_MEMORY       Out of memory
 * @retval #VC_ERROR_OPERATION_FAILED    Operation failure
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should NOT be #VC_SERVICE_STATE_NONE.
 *
 * @see vc_mgr_foreach_supported_languages()
 */
int vc_mgr_get_current_language(char **language);

/**
 * @platform
 * @brief Gets the current state of voice control manager.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[out] state   The current state
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should NOT be #VC_SERVICE_STATE_NONE.
 *
 * @see vc_state_changed_cb()
 * @see vc_set_state_changed_cb()
 */
int vc_mgr_get_state(vc_state_e *state);

/**
 * @platform
 * @brief Gets the current state of voice control service.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[out] state   The current state
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED    Operation failure
 *
 * @pre The state should be #VC_STATE_READY.
 *
 * @see vc_mgr_start()
 * @see vc_mgr_stop()
 * @see vc_mgr_cancel()
 * @see vc_set_service_state_changed_cb()
 * @see vc_unset_service_state_changed_cb()
 */
int vc_mgr_get_service_state(vc_service_state_e *state);

/**
 * @platform
 * @brief Checks whether the command format is supported.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks The command formats are defined in @ref CAPI_UIX_VOICE_CONTROL_COMMAND_MODULE. (e.g. #VC_COMMAND_FORMAT_FIXED, #VC_COMMAND_FORMAT_FIXED_AND_NONFIXED, and so on). Please refer to @ref CAPI_UIX_VOICE_CONTROL_COMMAND_MODULE.
 *
 * @param[in]  format    The command format
 * @param[out] support   The result status @c true = supported, @c false = not supported
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_READY.
 */
int vc_mgr_is_command_format_supported(int format, bool *support);

/**
 * @platform
 * @brief Sets all types of commands.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks VC manager client can register all types of commands.
 *      The commands should include type, command text, format.
 *
 * @param[in] vc_cmd_list   The command list handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_READY.
 *
 * @see vc_mgr_unset_command_list()
 */
int vc_mgr_set_command_list(vc_cmd_list_h vc_cmd_list);

/**
 * @platform
 * @brief Unsets all types of commands.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks All previously registered commands will be unset.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_READY.
 *
 * @see vc_mgr_set_command_list()
 */
int vc_mgr_unset_command_list(void);

/**
 * @platform
 * @brief Sets commands from file.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks The commands should include type, command text, format.
 *
 * @param[in] file_path   The directory of a file which has command list
 * @param[in] type          The command type (e.g. #VC_COMMAND_TYPE_FOREGROUND, #VC_COMMAND_TYPE_BACKGROUND, #VC_COMMAND_TYPE_WIDGET, #VC_COMMAND_TYPE_SYSTEM, #VC_COMMAND_TYPE_SYSTEM_BACKGROUND, #VC_COMMAND_TYPE_EXCLUSIVE)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED    Operation failure
 *
 * @pre The state should be #VC_STATE_READY.
 *
 * @see vc_mgr_unset_command_list()
 */
int vc_mgr_set_command_list_from_file(const char *file_path, int type);

/**
 * @platform
 * @brief Sets background commands of preloaded app from file.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks The command type is valid for #VC_COMMAND_TYPE_BACKGROUND.
 *      The commands should include type, command text, format.
 *
 * @param[in] file_path   The directory of a file which has command list
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED    Operation failure
 *
 * @pre The state should be #VC_STATE_READY.
 *
 * @see vc_mgr_unset_command_list()
 */
int vc_mgr_set_preloaded_commands_from_file(const char *file_path);

/**
 * @platform
 * @brief Retrieves all available commands.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks If the function succeeds, @a vc_cmd_list must be released with vc_cmd_list_destroy(vc_cmd_list, true).
 *
 * @param[out] vc_cmd_list   The command list
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED    Operation failure
 *
 * @pre The state should be #VC_STATE_READY and the service state should be #VC_SERVICE_STATE_READY.
 */
int vc_mgr_get_current_commands(vc_cmd_list_h *vc_cmd_list);

/**
 * @platform
 * @brief Sets a type of audio-in.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] audio_id   The audio type (e.g. #VC_AUDIO_TYPE_BLUETOOTH or USB device ID)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED    Operation failure
 *
 * @pre The state should be #VC_STATE_READY and the service state should be #VC_SERVICE_STATE_READY.
 *
 * @see vc_mgr_get_audio_type()
 */
int vc_mgr_set_audio_type(const char *audio_id);

/**
 * @platform
 * @brief Gets a type of audio-in.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks audio_id must be released using free() when it is no longer required.
 *
 * @param[out] audio_id   The audio id (e.g. #VC_AUDIO_TYPE_BLUETOOTH or USB device ID)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_READY and the service state should be #VC_SERVICE_STATE_READY.
 *
 * @see vc_mgr_set_audio_type()
 */
int vc_mgr_get_audio_type(char **audio_id);

/**
 * @platform
 * @brief Sets recognition mode.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] mode   Recognition mode (e.g. #VC_RECOGNITION_MODE_STOP_BY_SILENCE is default value)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_READY and the service state should be #VC_SERVICE_STATE_READY.
 *
 * @see vc_mgr_get_recognition_mode()
 */
int vc_mgr_set_recognition_mode(vc_recognition_mode_e mode);

/**
 * @platform
 * @brief Gets recognition mode.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[out] mode   Recognition mode
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_READY.
 *
 * @see vc_mgr_set_recognition_mode()
 */
int vc_mgr_get_recognition_mode(vc_recognition_mode_e *mode);

/**
 * @platform
 * @brief Sets private data to VC engine.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks VC manager client can set private data to VC engine using this function.
 *
 * @param[in] key    Private key
 * @param[in] data   Private data
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_OUT_OF_MEMORY       Not enough memory
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The service state should be #VC_SERVICE_STATE_READY.
 */
int vc_mgr_set_private_data(const char *key, const char *data);

/**
 * @platform
 * @brief Gets private data from VC engine.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks VC manager client can get private data from VC engine using this function.
 *      @a data must be released using free() when it is no longer required.
 *
 * @param[in]  key    Private key
 * @param[out] data   Private data
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_OUT_OF_MEMORY       Not enough memory
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The service state should be #VC_SERVICE_STATE_READY.
 */
int vc_mgr_get_private_data(const char *key, char **data);

/**
 * @platform
 * @brief Requests to do action as if utterance is spoken.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] type         Event type
 * @param[in] send_event   The string for send event
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_OUT_OF_MEMORY       Not enough memory
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The service state should be #VC_SERVICE_STATE_READY.
 */
int vc_mgr_do_action(vc_send_event_type_e type, char *send_event);

/**
 * @platform
 * @brief Sends the specific engine request to the vc-service.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] engine_app_id A specific engine's app id
 * @param[in] event A engine service user request event
 * @param[in] request A engine service user request text
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE               Successful
 * @retval #VC_ERROR_NOT_SUPPORTED      VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #VC_ERROR_OUT_OF_MEMORY      Not enough memory
 * @retval #VC_ERROR_INVALID_STATE      Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED   Operation failure
 */
int vc_mgr_send_specific_engine_request(const char* engine_app_id, const char* event, const char* request);

/**
 * @platform
 * @brief Starts recognition.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks The default recognition mode is #VC_RECOGNITION_MODE_STOP_BY_SILENCE. \n
 *      If you want to use other mode, you can set mode with vc_mgr_set_recognition_mode().
 *
 * @param[in] exclusive_command_option   Exclusive command option
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                       Successful
 * @retval #VC_ERROR_NOT_SUPPORTED              VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED          Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER          Invalid parameter.
 * @retval #VC_ERROR_INVALID_STATE              Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED           Operation failure
 * @retval #VC_ERROR_IN_PROGRESS_TO_RECORDING   In progress to recording
 *
 * @pre The state should be #VC_STATE_READY and the service state should be #VC_SERVICE_STATE_READY.
 * @post It will invoke vc_service_state_changed_cb(), if you register a callback with vc_service_state_changed_cb(). \n
 *      If this function succeeds, the service state will be #VC_SERVICE_STATE_RECORDING.
 *
 * @see vc_mgr_stop()
 * @see vc_mgr_cancel()
 * @see vc_service_state_changed_cb()
 * @see vc_mgr_set_recognition_mode()
 * @see vc_mgr_get_recognition_mode()
 */
int vc_mgr_start(bool exclusive_command_option);

/**
 * @platform
 * @brief Stops recognition.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                        Successful
 * @retval #VC_ERROR_NOT_SUPPORTED               VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED           Permission denied
 * @retval #VC_ERROR_INVALID_STATE               Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED            Operation failure
 * @retval #VC_ERROR_IN_PROGRESS_TO_READY        In progress to ready
 * @retval #VC_ERROR_IN_PROGRESS_TO_RECORDING    In progress to recording
 * @retval #VC_ERROR_IN_PROGRESS_TO_PROCESSING   In progress to processing
 *
 * @pre The service state should be #VC_SERVICE_STATE_RECORDING.
 * @post It will invoke vc_service_state_changed_cb(), if you register a callback with vc_service_state_changed_cb(). \n
 *      If this function succeeds, the service state will be #VC_SERVICE_STATE_PROCESSING.
 *
 * @see vc_mgr_start()
 * @see vc_mgr_cancel()
 * @see vc_service_state_changed_cb()
 * @see vc_result_cb()
 */
int vc_mgr_stop(void);

/**
 * @platform
 * @brief Cancels recognition.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                        Successful
 * @retval #VC_ERROR_NOT_SUPPORTED               VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED           Permission denied
 * @retval #VC_ERROR_OUT_OF_MEMORY               Not enough memory
 * @retval #VC_ERROR_INVALID_STATE               Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED            Operation failure
 * @retval #VC_ERROR_IN_PROGRESS_TO_READY        In progress to ready
 * @retval #VC_ERROR_IN_PROGRESS_TO_RECORDING    In progress to recording
 * @retval #VC_ERROR_IN_PROGRESS_TO_PROCESSING   In progress to processing
 *
 * @pre The service state should be #VC_SERVICE_STATE_RECORDING or #VC_SERVICE_STATE_PROCESSING.
 * @post It will invoke vc_service_state_changed_cb(), if you register a callback with vc_service_state_changed_cb(). \n
 *      If this function succeeds, the service state will be #VC_SERVICE_STATE_READY.
 *
 * @see vc_mgr_start()
 * @see vc_mgr_stop()
 * @see vc_service_state_changed_cb()
 */
int vc_mgr_cancel(void);

/**
 * @platform
 * @brief Gets the microphone volume during recording.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[out] volume   Recording volume
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The service state should be #VC_SERVICE_STATE_RECORDING.
 *
 * @see vc_mgr_start()
 */
int vc_mgr_get_recording_volume(float *volume);

/**
 * @platform
 * @brief Selects valid results from all results.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks This function should be called when vc_mgr_all_result_cb() returns @c false.
 *      The @a vc_cmd_list can be NULL, in that case the function does nothing.
 * @param[in] vc_cmd_list   The valid result list
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre vc_mgr_all_result_cb() should be called
 *
 * @see vc_mgr_all_result_cb()
 */
int vc_mgr_set_selected_results(vc_cmd_list_h vc_cmd_list);


/**
 * @platform
 * @brief Sets a callback function for getting recognition result.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks This callback must be set. Without this callback, the framework can not deliver the result of voice recognition to proper client.
 *
 * @param[in] callback    Callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_all_result_cb()
 * @see vc_mgr_unset_all_result_cb()
 */
int vc_mgr_set_all_result_cb(vc_mgr_all_result_cb callback, void *user_data);

/**
 * @platform
 * @brief Unsets the recognition result callback function.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_set_all_result_cb()
 */
int vc_mgr_unset_all_result_cb(void);

/**
 * @platform
 * @brief Sets a callback function for getting pre recognition result.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] callback    Callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_pre_result_cb()
 * @see vc_mgr_unset_pre_result_cb()
 */
int vc_mgr_set_pre_result_cb(vc_mgr_pre_result_cb callback, void *user_data);

/**
 * @platform
 * @brief Unsets the pre recognition result callback function.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_set_pre_result_cb()
 */
int vc_mgr_unset_pre_result_cb(void);

/**
 * @platform
 * @brief Sets a callback function for getting specific engine result.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] callback Callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE               Successful
 * @retval #VC_ERROR_NOT_SUPPORTED      Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE      Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_unset_specific_engine_result_cb()
 */
int vc_mgr_set_specific_engine_result_cb(vc_mgr_specific_engine_result_cb callback, void* user_data);

/**
 * @brief Unsets the specific engine result callback function.
 * @since_tizen 5.0
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE           Successful
 * @retval #VC_ERROR_NOT_SUPPORTED  Not supported
 * @retval #VC_ERROR_INVALID_STATE  Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_set_specific_engine_result_cb()
 */
int vc_mgr_unset_specific_engine_result_cb(void);

/**
 * @platform
 * @brief Sets a callback function for getting all types of recognition results.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] callback    Callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_result_cb()
 * @see vc_mgr_unset_result_cb()
 */
int vc_mgr_set_result_cb(vc_result_cb callback, void *user_data);

/**
 * @platform
 * @brief Unsets the callback function for getting all types of recognition results.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_set_result_cb()
 */
int vc_mgr_unset_result_cb(void);

/**
 * @platform
 * @brief Sets a callback function to be called when state is changed.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] callback    Callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_state_changed_cb()
 * @see vc_mgr_unset_state_changed_cb()
 */
int vc_mgr_set_state_changed_cb(vc_state_changed_cb callback, void *user_data);

/**
 * @platform
 * @brief Unsets the state changed callback function.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_set_state_changed_cb()
 */
int vc_mgr_unset_state_changed_cb(void);

/**
 * @platform
 * @brief Sets a callback function to be called when service state is changed.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] callback    Callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_service_state_changed_cb()
 * @see vc_mgr_unset_service_state_changed_cb()
 */
int vc_mgr_set_service_state_changed_cb(vc_service_state_changed_cb callback, void *user_data);

/**
 * @platform
 * @brief Unsets the service state changed callback function.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_set_service_state_changed_cb()
 */
int vc_mgr_unset_service_state_changed_cb(void);

/**
 * @platform
 * @brief Sets a callback function to be called when begin of speech is detected.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] callback    Callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_begin_speech_detected_cb()
 * @see vc_mgr_unset_speech_detected_cb()
 */
int vc_mgr_set_speech_detected_cb(vc_mgr_begin_speech_detected_cb callback, void *user_data);

/**
 * @platform
 * @brief Unsets the speech detected callback function.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_set_speech_detected_cb()
 */
int vc_mgr_unset_speech_detected_cb(void);

/**
 * @platform
 * @brief Sets a callback function to be called when current language is changed.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] callback    Callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_current_language_changed_cb()
 * @see vc_mgr_unset_current_language_changed_cb()
 */
int vc_mgr_set_current_language_changed_cb(vc_current_language_changed_cb callback, void *user_data);

/**
 * @platform
 * @brief Unsets the current language changed callback function.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_set_current_language_changed_cb()
 */
int vc_mgr_unset_current_language_changed_cb(void);

/**
 * @platform
 * @brief Gets the current error message.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks This function should be called during as vc error callback. If not, the error as operation failure will be returned. \n
 *      If the function succeeds, @a err_msg must be released using free() when it is no longer required.
 *
 * @param[out] err_msg   The current error message
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED    Operation failure
 * @retval #VC_ERROR_OUT_OF_MEMORY       Out of memory
 *
 * @see vc_error_cb()
 */
int vc_mgr_get_error_message(char **err_msg);

/**
 * @platform
 * @brief Sets a callback function to be called when an error occurred.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] callback    Callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_error_cb()
 * @see vc_mgr_unset_error_cb()
 */
int vc_mgr_set_error_cb(vc_error_cb callback, void *user_data);

/**
 * @platform
 * @brief Unsets the callback function to be called when an error occurred.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_set_error_cb()
 */
int vc_mgr_unset_error_cb(void);


/**
 * @platform
 * @brief Sets a callback function to be called when dialog requests.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] callback    Callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_unset_dialog_request_cb()
 */
int vc_mgr_set_dialog_request_cb(vc_mgr_dialog_request_cb callback, void *user_data);

/**
 * @platform
 * @brief Unsets the callback function to be called when dialog requests.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_set_dialog_request_cb()
 */
int vc_mgr_unset_dialog_request_cb(void);

/**
 * @platform
 * @brief Enables command type as candidate command.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] cmd_type   The command type (e.g. #VC_COMMAND_TYPE_FOREGROUND, #VC_COMMAND_TYPE_BACKGROUND, #VC_COMMAND_TYPE_WIDGET, #VC_COMMAND_TYPE_SYSTEM, #VC_COMMAND_TYPE_SYSTEM_BACKGROUND, #VC_COMMAND_TYPE_EXCLUSIVE)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_READY.
 *
 * @see vc_mgr_disable_command_type()
 */
int vc_mgr_enable_command_type(int cmd_type);

/**
 * @platform
 * @brief Disables command type as candidate command.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] cmd_type   The command type (e.g. #VC_COMMAND_TYPE_FOREGROUND, #VC_COMMAND_TYPE_BACKGROUND, #VC_COMMAND_TYPE_WIDGET, #VC_COMMAND_TYPE_SYSTEM, #VC_COMMAND_TYPE_SYSTEM_BACKGROUND, #VC_COMMAND_TYPE_EXCLUSIVE)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_READY.
 *
 * @see vc_mgr_enable_command_type()
 */
int vc_mgr_disable_command_type(int cmd_type);

/**
 * @platform
 * @brief Sets a callback function to be called when engine set private data.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] callback    Callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_private_data_set_cb()
 * @see vc_mgr_unset_private_data_set_cb()
 */
int vc_mgr_set_private_data_set_cb(vc_mgr_private_data_set_cb callback, void *user_data);

/**
 * @platform
 * @brief Unsets the callback function to be called when engine set private data.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_set_private_data_set_cb()
 */
int vc_mgr_unset_private_data_set_cb(void);

/**
 * @platform
 * @brief Sets a callback function to be called when engine request private data.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] callback    Callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_private_data_requested_cb()
 * @see vc_mgr_unset_private_data_requested_cb()
 */
int vc_mgr_set_private_data_requested_cb(vc_mgr_private_data_requested_cb callback, void *user_data);

/**
 * @platform
 * @brief Unsets the callback function to be called when engine request private data.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_set_private_data_requested_cb()
 */
int vc_mgr_unset_private_data_requested_cb(void);

/* for TTS feedback */
/**
 * @platform
 * @brief Sets a callback function to be called when engine sends audio formats necessary for playing TTS feedback.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] callback Callback function to set
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE               Successful
 * @retval #VC_ERROR_NOT_SUPPORTED      VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE      Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_feedback_audio_format_cb()
 * @see vc_mgr_unset_feedback_audio_format_cb()
 */
int vc_mgr_set_feedback_audio_format_cb(vc_mgr_feedback_audio_format_cb callback, void* user_data);

/**
 * @platform
 * @brief Unsets a callback function to be called when engine sends audio formats necessary for playing TTS feedback.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE               Successful
 * @retval #VC_ERROR_NOT_SUPPORTED      VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #VC_ERROR_INVALID_STATE      Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_feedback_audio_format_cb()
 * @see vc_mgr_set_feedback_audio_format_cb()
 */
int vc_mgr_unset_feedback_audio_format_cb(void);

/**
 * @platform
 * @brief Sets a callback function to be called when engine sends audio streaming for TTS feedback.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] callback Callback function to set
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE               Successful
 * @retval #VC_ERROR_NOT_SUPPORTED      VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE      Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_feedback_streaming_cb()
 * @see vc_mgr_unset_feedback_streaming_cb()
 */
int vc_mgr_set_feedback_streaming_cb(vc_mgr_feedback_streaming_cb callback, void* user_data);

/**
 * @platform
 * @brief Unsets a callback function to be called when engine sends audio streaming for TTS feedback.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE               Successful
 * @retval #VC_ERROR_NOT_SUPPORTED      VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #VC_ERROR_INVALID_STATE      Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_feedback_streaming_cb()
 * @see vc_mgr_set_feedback_streaming_cb()
 */
int vc_mgr_unset_feedback_streaming_cb(void);

/**
 * @platform
 * @brief Starts getting TTS feedback streaming data from the buffer.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks In order to get TTS feedback streaming data, the application should set 'vc_mgr_feedback_streaming_cb()' using vc_mgr_set_feedback_streaming_cb().
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE               Successful
 * @retval #VC_ERROR_NOT_SUPPORTED      VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #VC_ERROR_INVALID_STATE      Invalid state
 *
 * @pre The state should be #VC_STATE_READY. \n
 *      'vc_mgr_feedback_streaming_cb()' should be registered.
 *
 * @see vc_mgr_feedback_streaming_cb()
 * @see vc_mgr_set_feedback_streaming_cb()
 * @see vc_mgr_unset_feedback_streaming_cb()
 * @see vc_mgr_stop_feedback()
 */
int vc_mgr_start_feedback(void);

/**
 * @platform
 * @brief Stops getting and removes TTS feedback streaming data from the buffer.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE               Successful
 * @retval #VC_ERROR_NOT_SUPPORTED      VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #VC_ERROR_INVALID_STATE      Invalid state
 *
 * @pre The state should be #VC_STATE_READY.
 *
 * @see vc_mgr_feedback_streaming_cb()
 * @see vc_mgr_set_feedback_streaming_cb()
 * @see vc_mgr_unset_feedback_streaming_cb()
 * @see vc_mgr_start_feedback()
 */
int vc_mgr_stop_feedback(void);

/**
 * @platform
 * @brief Sets TTS streaming callback function.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] callback The callback function
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE               Successful
 * @retval #VC_ERROR_NOT_SUPPORTED      VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE      Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_vc_tts_streaming_cb()
 * @see vc_mgr_unset_vc_tts_streaming_cb()
 */
int vc_mgr_set_vc_tts_streaming_cb(vc_mgr_vc_tts_streaming_cb callback, void* user_data);

/**
 * @platform
 * @brief Unsets TTS streaming callback function.
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE               Successful
 * @retval #VC_ERROR_NOT_SUPPORTED      VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #VC_ERROR_INVALID_STATE      Invalid state
 *
 * @pre The state should be #VC_STATE_INITIALIZED.
 *
 * @see vc_mgr_vc_tts_streaming_cb()
 * @see vc_mgr_set_vc_tts_streaming_cb()
 */
int vc_mgr_unset_vc_tts_streaming_cb(void);

/**
 * @platform
 * @brief Sends the utterance status to the VC client.
 * @since_tizen 6.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] pid The process id of the VC client
 * @param[in] utt_id The utterance id
 * @param[in] utt_status The utterance status
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_OUT_OF_MEMORY       Not enough memory
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED    Operation fail
 *
 * @pre The service state should be #VC_SERVICE_STATE_READY.
 */
int vc_mgr_send_utterance_status(int pid, int utt_id, vc_tts_utterance_status_e utt_status);

/**
 * @platform
 * @brief Sends audio streaming to the engine service.
 * @details Using this function, the developer can send audio streaming after vc_mgr_start() function is called.
 * @since_tizen 6.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] event Audio streaming event (e.g. #VC_AUDIO_STREAMING_EVENT_START, #VC_AUDIO_STREAMING_EVENT_CONTINUE)
 * @param[in] buffer Audio streaming data
 * @param[in] len Length of the audio streaming data
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_OUT_OF_MEMORY       Not enough memory
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED    Operation fail
 *
 * @pre The state should be #VC_SERVICE_STATE_RECORDING.
 *
 * @see vc_mgr_set_audio_streaming_mode()
 */
int vc_mgr_send_audio_streaming(vc_audio_streaming_event_e event, unsigned char *buffer, unsigned int len);

/**
 * @platform
 * @brief Sets audio streaming mode.
 * @since_tizen 6.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks The default audio streaming mode is #VC_AUDIO_STREAMING_MODE_VC_SERVICE. \n
 *      If you want to use other mode, you can set mode with vc_mgr_set_audio_streaming_mode().
 *
 * @param[in] mode Audio streaming mode
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED    Operation fail
 *
 * @pre The state should be #VC_STATE_INITIALIZED or #VC_SERVICE_STATE_READY.
 *
 * @see vc_mgr_send_audio_streaming()
 */
int vc_mgr_set_audio_streaming_mode(vc_audio_streaming_mode_e mode);

/**
 * @platform
 * @brief Changes background volume.
 * @since_tizen 6.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/volume.set
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @remarks If @a event is #VC_BACKGROUND_VOLUME_EVENT_CHANGE_FOR_NEARFIELD, the background volume will be reduced by 70%.
 *          And, if @a event is #VC_BACKGROUND_VOLUME_EVENT_CHANGE_FOR_FARFIELD, the background volume will be muted.
 *
 * @param[in] event The background volume event
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED    Operation fail
 *
 * @pre The state should be #VC_STATE_INITIALIZED or #VC_STATE_READY.
 *
 * @see vc_mgr_reset_background_volume()
 * @see vc_mgr_change_background_volume_by_ratio()
 */
int vc_mgr_change_background_volume(vc_background_volume_event_e event);

/**
 * @platform
 * @brief Changes background volume ratio.
 * @details Calling this function sets the background volume to a given percentage of its current value.
 * @since_tizen 6.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/volume.set
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @param[in] ratio Current background volume ratio (Range 0.0 ~ 1.0)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED    Operation fail
 *
 * @pre The state should be #VC_STATE_INITIALIZED or #VC_STATE_READY.
 *
 * @see vc_mgr_change_background_volume()
 * @see vc_mgr_reset_background_volume()
 */
int vc_mgr_change_background_volume_by_ratio(double ratio);

/**
 * @platform
 * @brief Resets background volume.
 * @details This function reverts all changes made by vc_mgr_change_background_volume() and vc_mgr_change_background_volume_by_ratio().
 * @since_tizen 6.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/volume.set
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/voicecontrol.manager
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED    Operation fail
 *
 * @pre The state should be #VC_STATE_INITIALIZED or #VC_STATE_READY.
 *
 * @see vc_mgr_change_background_volume()
 * @see vc_mgr_change_background_volume_by_ratio()
 */
int vc_mgr_reset_background_volume(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __VOICE_CONTROL_MANAGER_H__ */

