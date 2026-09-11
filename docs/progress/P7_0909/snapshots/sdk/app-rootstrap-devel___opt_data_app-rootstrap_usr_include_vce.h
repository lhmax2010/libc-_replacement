/*
 * Copyright (c) 2011-2017 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __VCE_H__
#define __VCE_H__

#include <tizen.h>

/**
 * @addtogroup CAPI_UIX_VOICE_CONTROL_ENGINE_MODULE
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumerations of error codes.
 * @since_tizen 4.0
 */
typedef enum {
	VCE_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	VCE_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of Memory */
	VCE_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< I/O error */
	VCE_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	VCE_ERROR_OUT_OF_NETWORK = TIZEN_ERROR_NETWORK_DOWN, /**< Out of network */
	VCE_ERROR_RECORDER_BUSY = TIZEN_ERROR_RESOURCE_BUSY, /**< Busy resource */
	VCE_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< VC Engine NOT supported */
	VCE_ERROR_INVALID_STATE = TIZEN_ERROR_VOICE_CONTROL | 0x011, /**< Invalid state */
	VCE_ERROR_INVALID_LANGUAGE = TIZEN_ERROR_VOICE_CONTROL | 0x012, /**< Invalid language */
	VCE_ERROR_OPERATION_FAILED = TIZEN_ERROR_VOICE_CONTROL | 0x014, /**< Operation failed */
	VCE_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	VCE_ERROR_NOT_SUPPORTED_FEATURE = TIZEN_ERROR_VOICE_CONTROL | 0x022, /**< Not supported feature of current engine */
	VCE_ERROR_TTS_FAILED = TIZEN_ERROR_VOICE_CONTROL | 0x024 /**< TTS operation failed (Since 7.0) */
} vce_error_e;

/**
 * @brief Enumerations of audio type.
 * @since_tizen 4.0
 */
typedef enum {
	VCE_AUDIO_TYPE_PCM_S16_LE = 0, /**< Signed 16bit audio type, Little endian */
	VCE_AUDIO_TYPE_PCM_U8 /**< Unsigned 8bit audio type */
} vce_audio_type_e;

/**
 * @brief Enumerations of callback event.
 * @since_tizen 4.0
 */
typedef enum {
	VCE_RESULT_EVENT_SUCCESS = 0, /**< Event when the recognition full result is ready  */
	VCE_RESULT_EVENT_REJECTED, /**< Event when the recognition result is rejected */
	VCE_RESULT_EVENT_ERROR /**< Event when the recognition has failed */
} vce_result_event_e;

/**
 * @brief Enumerations of command type.
 * @since_tizen 4.0
 */
typedef enum {
	VCE_COMMAND_FORMAT_FIXED = 0, /**< Fixed command */
	VCE_COMMAND_FORMAT_FIXED_AND_VFIXED, /**< Fixed command + variable-fixed command */
	VCE_COMMAND_FORMAT_VFIXED_AND_FIXED, /**< variable-fixed command + Fixed command */
	VCE_COMMAND_FORMAT_FIXED_AND_NONFIXED, /**< Fixed command + Non-fixed command */
	VCE_COMMAND_FORMAT_NONFIXED_AND_FIXED, /**< Non-fixed command + Fixed command */
	VCE_COMMAND_FORMAT_ACTION, /**< Action command */
	VCE_COMMAND_FORMAT_PARTIAL /**< Partial matched command */
} vce_command_format_e;

/**
 * @brief Enumerations of speech detect.
 * @since_tizen 4.0
 */
typedef enum {
	VCE_SPEECH_DETECT_NONE = 0, /**< No event */
	VCE_SPEECH_DETECT_BEGIN, /**< Begin of speech detected */
	VCE_SPEECH_DETECT_END, /**< End of speech detected */
} vce_speech_detect_e;

/**
 * @brief Enumerations of ASR result events.
 * @since_tizen 4.0
 */
typedef enum {
	VCE_ASR_RESULT_EVENT_FINAL_RESULT = 0, /**< Event when the ASR result is last data or ASR result is only one result */
	VCE_ASR_RESULT_EVENT_PARTIAL_RESULT, /**< Event when the ASR result exist, not first and not last */
	VCE_ASR_RESULT_EVENT_ERROR /**< Event when the ASR result is failed */
} vce_asr_result_event_e;

/**
 * @brief Enumerations of audio channels.
 * @since_tizen 4.0
 */
typedef enum {
	VCE_AUDIO_CHANNEL_MONO = 0, /**< 1 channel, mono */
	VCE_AUDIO_CHANNEL_STEREO = 1 /**< 2 channels, stereo */
} vce_audio_channel_e;

/**
 * @brief Enumeration for TTS feedback events.
 * @since_tizen 4.0
 */
typedef enum {
	VCE_FEEDBACK_EVENT_FAIL = -1, /**< Failed */
	VCE_FEEDBACK_EVENT_START = 1, /**< Start event */
	VCE_FEEDBACK_EVENT_CONTINUE = 2, /**< Continue event */
	VCE_FEEDBACK_EVENT_FINISH = 3 /**< Finish event */
} vce_feedback_event_e;

/**
 * @brief A structure of handle for VC command.
 * @since_tizen 4.0
 */
typedef struct vce_cmd_s* vce_cmd_h;

/**
 * @brief Definition for foreground command type.
 * @since_tizen 4.0
 */
#define VCE_COMMAND_TYPE_FOREGROUND	1

/**
 * @brief Definition for background command type.
 * @since_tizen 4.0
 */
#define VCE_COMMAND_TYPE_BACKGROUND	2

/**
 * @brief Definition for widget command type.
 * @since_tizen 4.0
 */
#define VCE_COMMAND_TYPE_WIDGET		3

/**
 * @brief Definition for system command type.
 * @since_tizen 4.0
 */
#define VCE_COMMAND_TYPE_SYSTEM		4

/**
 * @brief Definition for system background command type.
 * @since_tizen 4.0
 */
#define VCE_COMMAND_TYPE_SYSTEM_BACKGROUND	5

/**
 * @brief Definitions for exclusive command type.
 * @since_tizen 4.0
 */
#define VCE_COMMAND_TYPE_EXCLUSIVE	6

/**
 * @brief Definition of bluetooth audio id.
 * @since_tizen 4.0
 */
#define VCE_AUDIO_ID_BLUETOOTH		"VC_AUDIO_ID_BLUETOOTH"		/**< Bluetooth audio id */

/**
 * @brief Definition of Wi-Fi audio id.
 * @since_tizen 4.0
 */
#define VCE_AUDIO_ID_WIFI		"VC_AUDIO_ID_WIFI"		/**< Wi-Fi audio id */

/**
 * @brief Definition for none message.
 * @since_tizen 4.0
 */
#define VC_RESULT_MESSAGE_NONE			"vc.result.message.none"

/**
 * @brief Definition for failed recognition because the speech is too loud to listen.
 * @since_tizen 4.0
 */
#define VC_RESULT_MESSAGE_ERROR_TOO_LOUD	"vc.result.message.error.too.loud"

/**
 * @brief Called when VC engine informs the engine service user about whole supported languages.
 * @details This callback function is implemented by the engine service user. Therefore, the engine developer does NOT have to implement this callback function.
 * @since_tizen 4.0
 * @remarks This callback function is called by vce_foreach_supported_languages_cb() to retrieve the whole supported language list.
 *          The @a user_data must be transferred from vce_foreach_supported_languages_cb().
 *          The @a language can be used only in the callback. To use outside, make a copy.
 * @param[in] language A language is specified as an ISO 3166 alpha-2 two letter country-code followed by ISO 639-1 for the two-letter language code \n
 *            For example, "ko_KR" for Korean, "en_US" for American English.
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop \n @c false to break out of the loop
 * @pre vce_foreach_supported_languages_cb() will invoke this callback.
 * @see vce_foreach_supported_languages_cb()
 */
typedef bool (*vce_supported_language_cb)(const char* language, void* user_data);

/**
 * @brief Called when the engine service user initializes Voice Control (VC) engine.
 * @since_tizen 4.0
 * @remarks This callback function is mandatory and must be registered using vce_main().
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_INVALID_STATE Already initialized
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failed
 * @see vce_deinitialize_cb()
 */
typedef int (*vce_initialize_cb)(void);

/**
 * @brief Called when the engine service user deinitializes VC engine.
 * @since_tizen 4.0
 * @remarks This callback function is mandatory and must be registered using vce_main().
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_INVALID_STATE Not initialized
 * @see vce_initialize_cb()
 */
typedef int (*vce_deinitialize_cb)(void);

/**
 * @brief Called when the engine service user requests the recording format of VC engine.
 * @since_tizen 4.0
 * @remarks This callback function is mandatory and must be registered using vce_main().
 *          The @a audio_id can be used only in the callback. To use outside, make a copy.
 *          The @a types is managed by the platform and will be released when this callback function is completed.
 *          The @a rate is managed by the platform and will be released when this callback function is completed.
 *          The @a channels is managed by the platform and will be released when this callback function is completed.
 * @param[in] audio_id The audio device id. (e.g. #VCE_AUDIO_ID_BLUETOOTH or #VCE_AUDIO_ID_WIFI)
 * @param[out] types The format used by the recorder.
 * @param[out] rate The sample rate used by the recorder.
 * @param[out] channels The number of channels used by the recorder.
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_INVALID_PARAMETER Not initialized
 */
typedef int (*vce_get_recording_format_cb)(const char* audio_id, vce_audio_type_e* types, int* rate, int* channels);

/**
 * @brief Called when the engine service user retrieves all supported languages of VC engine.
 * @since_tizen 4.0
 * @remarks This callback function is mandatory and must be registered using vce_main().
 * @param[in] callback a callback function
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_INVALID_STATE Not initialized
 * @post This function invokes vce_supported_language_cb() repeatedly for getting supported languages.
 *
 * @see vce_supported_language_cb()
 */
typedef int (*vce_foreach_supported_languages_cb)(vce_supported_language_cb callback, void* user_data);

/**
 * @brief Called when the engine service user checks whether a language is supported or not.
 * @since_tizen 4.0
 * @remarks This callback function is mandatory and must be registered using vce_main().
 *          The @a language can be used only in the callback. To use outside, make a copy.
 * @param[in] language A language
 * @return @c true = supported, \n @c false = not supported.
 */
typedef bool (*vce_is_language_supported_cb)(const char* language);

/**
 * @brief Called when the engine service user sets language.
 * @since_tizen 4.0
 * @remarks The @a language can be used only in the callback. To use outside, make a copy.
 * @param[in] language A language.
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_INVALID_LANGUAGE Invalid language
 * @retval #VCE_ERROR_INVALID_STATE Not initialized
 * @retval #VCE_ERROR_NOT_SUPPORTED_FEATURE Not supported feature
 */
typedef int (*vce_set_language_cb)(const char* language);

/**
 * @brief Called when the engine service user sets command list before recognition.
 * @since_tizen 4.0
 * @remarks This function should set commands via vce_get_foreach_command().
 *          The @a vc_command should not be released.
 *          The @a vc_command can be used only in the callback. To use outside, make a copy.
 * @param[in] vc_command command handle. The @a vc_command can be used only in the callback. To use outside, make a copy.
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_INVALID_STATE Invalid state
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failed
 * @retval #VCE_ERROR_NOT_SUPPORTED_FEATURE Not supported command type
 * @post vce_start_cb() is called after this function is successful.
 * @see vce_start_cb()
 * @see vce_get_foreach_command()
 * @see vce_unset_commands_cb()
 * @see vce_get_command_count()
 */
typedef int (*vce_set_commands_cb)(vce_cmd_h vc_command);

/**
 * @brief Called when the engine service user unsets command list for reset.
 * @since_tizen 4.0
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_INVALID_STATE Invalid state
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failed
 * @retval #VCE_ERROR_NOT_SUPPORTED_FEATURE Not supported feature
 * @see vce_set_commands_cb()
 */
typedef int (*vce_unset_commands_cb)(void);

/**
 * @brief Called when the engine service user starts recognition.
 * @since_tizen 4.0
 * @remarks This callback function is mandatory and must be registered using vce_main().
 * @param[in] stop_by_silence Silence detection option.
 *                   @c true to detect the silence,
 *                   @c false not to detect the silence
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_INVALID_STATE Invalid state
 * @retval #VCE_ERROR_INVALID_LANGUAGE Invalid language
 * @retval #VCE_ERROR_OUT_OF_NETWORK Out of network
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failed
 * @pre vce_set_commands_cb() is successful.
 * @see vce_set_recording_data_cb()
 * @see vce_stop_cb()
 * @see vce_cancel_cb()
 */
typedef int (*vce_start_cb)(bool stop_by_silence);

/**
 * @brief Called when the engine service user sets recording data for speech recognition from recorder.
 * @since_tizen 4.0
 * @remarks This function should be returned immediately after recording data copy.
 *          The @a data can be used only in the callback. To use outside, make a copy.
 *          The @a speech_detected should not be released. This is managed by the platform.
 * @param[in] data A recording data
 * @param[in] length A length of recording data
 * @param[out] speech_detected The status of speech (e.g. #VCE_SPEECH_DETECT_BEGIN or #VCE_SPEECH_DETECT_END). The @a speech_detected can be used only in the callback. To use outside, make a copy.
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_INVALID_STATE Invalid state
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failed
 * @pre vce_start_cb() is successful.
 * @see vce_start_cb()
 * @see vce_cancel_cb()
 * @see vce_stop_cb()
 */
typedef int(*vce_set_recording_data_cb)(const void* data, unsigned int length, vce_speech_detect_e* speech_detected);

/**
 * @brief Called when the engine service user stops to get the result of recognition.
 * @since_tizen 4.0
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_INVALID_STATE Invalid state
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failed
 * @retval #VCE_ERROR_OUT_OF_NETWORK Out of network
 * @pre vce_set_recording_data_cb() is successful.
 * @see vce_start_cb()
 * @see vce_set_recording_data_cb()
 * @see vce_send_result()
 * @see vce_cancel_cb()
 */
typedef int (*vce_stop_cb)(void);

/**
 * @brief Called when the engine service user cancels the recognition process.
 * @since_tizen 4.0
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful.
 * @retval #VCE_ERROR_INVALID_STATE Invalid state.
 * @pre vce_start_cb() is successful.
 * @see vce_start_cb()
 * @see vce_stop_cb()
 */
typedef int (*vce_cancel_cb)(void);

/**
 * @brief Called when the engine service user sets audio recording type.
 * @since_tizen 4.0
 * @remarks The @a audio_type can be used only in the callback. To use outside, make a copy.
 * @param[in] audio_type Current audio type (e.g. #VCE_AUDIO_ID_BLUETOOTH or #VCE_AUDIO_ID_WIFI)
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful.
 * @retval #VCE_ERROR_NOT_SUPPORTED_FEATURE Not supported feature.
 */
typedef int (*vce_set_audio_type_cb)(const char* audio_type);

/**
 * @brief Called when the engine service user sets app id which is want to ask server dialog.
 * @since_tizen 4.0
 * @remarks The @a app_id and @a credential can be used only in the callback. To use outside, make a copy.
 * @param[in] app_id App id which is to want to ask server dialog.
 * @param[in] credential Credential key.
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful.
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied.
 * @retval #VCE_ERROR_NOT_SUPPORTED_FEATURE Not supported feature.
 */
typedef int (*vce_set_server_dialog_cb)(const char* app_id, const char* credential);

/**
 * @brief Called when the engine service user sets domain (agent or device type).
 * @since_tizen 4.0
 * @remarks The @a domain can be used only in the callback. To use outside, make a copy.
 * @param[in] domain Agent (e.g. "music", "news", etc) or device type (e.g. "tv", "mobile", etc) corresponding to the command
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful.
 * @retval #VCE_ERROR_NOT_SUPPORTED_FEATURE Not supported feature
 */
typedef int (*vce_set_domain_cb)(const char* domain);

/**
 * @brief Called when the engine service user requests essential value from NLU result.
 * @since_tizen 4.0
 * @remarks The @a key can be used only in the callback. To use outside, make a copy.
 *          The @a value is managed by the platform and will be released when this callback function is completed.
 * @param[in] key NLU base info key.
 * @param[out] value NLU base info value.
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful.
 * @retval #VCE_ERROR_NOT_SUPPORTED_FEATURE Not supported feature.
 */
typedef int (*vce_nlu_base_info_requested_cb)(const char* key, char** value);

/**
 * @brief Called when client gets the specific engine's request from the engine service user.
 * @since_tizen 4.0
 * @remarks The @a engine_app_id is managed by the platform and will be released when this callback function is completed.
 *          The @a event is managed by the platform and will be released when this callback function is completed.
 *          The @a request is managed by the platform and will be released when this callback function is completed.
 *
 * @param[in] engine_app_id The specific engine's app id
 * @param[in] event The specific engine event type
 * @param[in] request The specific engine request
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 *
 * @pre An application registers callback function using vce_set_specific_engine_request_cb().
 *
 * @see vce_set_specific_engine_request_cb()
 * @see vce_unset_specific_engine_request_cb()
 */
typedef int (*vce_specific_engine_request_cb)(const char* engine_app_id, const char* event, const char* request);

/**
 * @brief Called when the engine service user sets private data between app and engine.
 * @since_tizen 4.0
 * @remarks The @a key, @a data can be used only in the callback. To use outside, make a copy.
 * @param[in] key Private key.
 * @param[in] data Private data.
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful.
 * @retval #VCE_ERROR_NOT_SUPPORTED_FEATURE Not supported feature.
 */
typedef int (*vce_private_data_set_cb)(const char* key, const char* data);

/**
 * @brief Called when the engine service user requests private data between app and engine.
 * @since_tizen 4.0
 * @remarks The @a key can be used only in the callback. To use outside, make a copy.
 *          The @a data is managed by the platform and will be released when this callback function is completed.
 * @param[in] key Private key.
 * @param[out] data Private data.
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful.
 * @retval #VCE_ERROR_NOT_SUPPORTED_FEATURE Not supported feature.
 */
typedef int (*vce_private_data_requested_cb)(const char* key, char** data);

/**
 * @brief Called when the engine service user requests process text.
 * @since_tizen 4.0
 * @remarks The @a text can be used only in the callback. To use outside, make a copy.
 * @param[in] text Requested text
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful.
 * @retval #VCE_ERROR_NOT_SUPPORTED_FEATURE Not supported feature.
 */
typedef int (*vce_process_text_cb)(const char* text);

/**
 * @brief Called when the engine service user requests list event.
 * @since_tizen 4.0
 * @remarks The @a event can be used only in the callback. To use outside, make a copy.
 * @param[in] event Requested list event
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful.
 * @retval #VCE_ERROR_NOT_SUPPORTED_FEATURE Not supported feature.
 */
typedef int (*vce_process_list_event_cb)(const char* event);

/**
 * @brief Called when the engine service user requests haptic event.
 * @since_tizen 4.0
 * @remarks The @a event can be used only in the callback. To use outside, make a copy.
 * @param[in] event Requested haptic event
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful.
 * @retval #VCE_ERROR_NOT_SUPPORTED_FEATURE Not supported feature.
 */
typedef int (*vce_process_haptic_event_cb)(const char* event);

/**
 * @brief Called when the engine service user requests the base information of VC engine.
 * @since_tizen 4.0
 * @remarks This callback function is mandatory and must be registered using vce_main().
 *          The @a engine_uuid is managed by the platform and will be released when this callback function is completed.
 *          The @a engine_name is managed by the platform and will be released when this callback function is completed.
 *          The @a engine_settings_app_id is managed by the platform and will be released when this callback function is completed.
 *          The @a use_network is managed by the platform and will be released when this callback function is completed.
 *          In order to upload the engine to Tizen Appstore, both the service app and the UI app (engine settings) are necessary.
 *          Therefore, @a engine_settings_app_id should be set to the application ID of the UI application.
 *          If there is no UI application, then @a engine_settings_app_id should be set to NULL.
 * @param[out] engine_uuid The engine id
 * @param[out] engine_name The engine name
 * @param[out] engine_settings_app_id The ID of the engine settings application (the UI application)
 * @param[out] use_network @c true to need network @c false not to need network.
 * @return @c 0 on success, otherwise a negative error code on failure
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 */
typedef int (*vce_get_info_cb)(char** engine_uuid, char** engine_name, char** engine_settings_app_id, bool* use_network);

/**
 * @brief Called to retrieve the commands.
 * @since_tizen 4.0
 * @remarks The @a command, @a param can be used only in the callback. To use outside, make a copy.
 * @param[in] id command id
 * @param[in] type command type
 * @param[in] format command format
 * @param[in] command command text
 * @param[in] param parameter text
 * @param[in] domain command domain
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop, \n @c false to break out of the loop.
 * @pre vce_get_foreach_command() will invoke this callback.
 * @see vce_get_foreach_command()
 */
typedef bool (*vce_command_cb)(int id, int type, int format, const char* command, const char* param, int domain, void* user_data);

/**
 * @brief Called when the engine service user (voice control client) requests to send TTS feedback.
 * @since_tizen 5.0
 * @remarks The @a text and @a language can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] pid The process id of the engine service user (voice control client)
 * @param[in] utt_id The utterance id
 * @param[in] text The text for TTS feedback
 * @param[in] language The language
 * @param[in] user_data The user data passed from the callback setter function
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 *
 * @pre An application registers callback function using vce_set_request_tts_cb().
 *
 * @see vce_set_request_tts_cb()
 * @see vce_unset_request_tts_cb()
 */
typedef int (*vce_request_tts_cb)(int pid, int utt_id, const char* text, const char* language, void* user_data);

/**
 * @brief Called when the engine service user cancels TTS feedback.
 * @since_tizen 5.0
 *
 * @param[in] pid The process id of the engine service user
 * @param[in] utt_id The utterance id corresponding to the text
 * @param[in] user_data The user data passed from the callback setter function
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 *
 * @pre An application registers callback function using vce_set_cancel_tts_cb().
 *
 * @see vce_set_cancel_tts_cb()
 * @see vce_unset_cancel_tts_cb()
*/
typedef int (*vce_cancel_tts_cb)(int pid, int utt_id, void* user_data);

/**
 * @brief Called when the engine service user requests TTS audio format.
 * @since_tizen 5.0
 * @remarks The @a rate, @a channel, and @a audio_type should not be released.
 *          The @a rate, @a channel, and @a audio_type are managed by the platform and will be released after the audio format is transferred to the VC client.
 *
 * @param[out] rate The audio sample rate
 * @param[out] channel The audio channel
 * @param[out] audio_type The audio type
 * @param[in] user_data The user data passed from the callback setter function
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 *
 * @pre An application registers callback function using vce_set_tts_audio_format_request_cb().
 *
 * @see vce_set_tts_audio_format_request_cb()
 * @see vce_unset_get_tts_audio_format_cb()
 */
typedef int (*vce_tts_audio_format_request_cb)(int* rate, int* channel, int* audio_type, void* user_data);


/**
 * @brief A structure for the VC engine functions.
 * @details This structure contains essential callback functions for operating VC engine.
 * @since_tizen 4.0
 * @remarks You must register all callbacks except optional callbacks for operating VC engine.\n
 *      The following callbacks are optional callbacks : \n
 *      - vce_private_data_set_cb() \n
 *      - vce_private_data_requested_cb() \n
 *      - vce_nlu_base_info_requested_cb() \n
 *      - vce_specific_engine_request_cb() \n
 *      If you want to use the optional callbacks, you can set them using vce_set_private_data_set_cb(), vce_set_private_data_requested_cb(), vce_set_nlu_base_info_requested_cb(), and vce_set_specific_engine_request_cb() functions.
 *
 * @see vce_main()
 * @see vce_set_private_data_set_cb()
 * @see vce_set_private_data_requested_cb()
 * @see vce_set_nlu_base_info_requested_cb()
 * @see vce_set_specific_engine_request_cb()
 */
typedef struct {
	int version;						/**< Version */

	/* Get engine information */
	vce_get_info_cb						get_info;				/**< Called when the engine service user requests the basic information of VC engine */
	vce_get_recording_format_cb			get_recording_format;	/**< Get recording format */
	vce_foreach_supported_languages_cb	foreach_langs;			/**< Foreach language list */
	vce_is_language_supported_cb		is_lang_supported;		/**< Check language */

	vce_initialize_cb					initialize;				/**< Initialize engine */
	vce_deinitialize_cb					deinitialize;			/**< Shutdown engine */

	/* Set info */
	vce_set_language_cb					set_language;			/**< Set language */
	vce_set_commands_cb					set_commands;			/**< Request to set current commands */
	vce_unset_commands_cb				unset_commands;			/**< Request to unset current commands */

	/* Control recognition */
	vce_start_cb						start;					/**< Start recognition */
	vce_set_recording_data_cb			set_recording;			/**< Set recording data */
	vce_stop_cb							stop;					/**< Stop recording for getting result */
	vce_cancel_cb						cancel;					/**< Cancel recording and processing */

	vce_set_audio_type_cb				set_audio_type;			/**< Set audio type */
	vce_set_server_dialog_cb			set_server_dialog;		/**< Set server dialog */

	vce_set_domain_cb					set_domain;				/**< Set domain */
	vce_process_text_cb					process_text;			/**< Request to process text */
	vce_process_list_event_cb			process_list_event;		/**< Request to process list event */
	vce_process_haptic_event_cb			process_haptic_event;	/**< Request to process haptic event */

	/* Optional callbacks */
	vce_private_data_set_cb				private_data_set;		/**< Set private data from the app */
	vce_private_data_requested_cb		private_data_request;	/**< Send private data to the app */
	vce_nlu_base_info_requested_cb		nlu_base_info_request;	/**< Send essential value from NLU result */
	vce_specific_engine_request_cb		specific_engine_request;	/**< Get specific engine's request from the engine service user */
} vce_request_callback_s;

/**
 * @brief Starts the main function for Voice Control (VC) engine.
 * @details This function is the main function for operating VC engine.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder \n
 *            %http://tizen.org/privilege/appmanager.launch (Since 7.0)  \n
 *            %http://tizen.org/privilege/datasharing (Since 7.0)
 * @remarks To use this function, privileges of appmanager.launch and datasharing should be included since 7.0. \n
 *          The service_app_main() should be used for working the engine after this function.
 * 
 * @param[in] argc The argument count(original)
 * @param[in] argv The argument(original)
 * @param[in] callback The structure of engine request callback function
 * @return This function returns @c zero on success, or negative with error code on failure
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OUT_OF_MEMORY Out of Memory
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failed
 * @pre The vce_get_info_cb() should be successful.
 * @see vce_get_info_cb()
 * @see vce_request_callback_s
 * @code
#include <vce.h>

// Required callback functions - MUST BE IMPLEMENTED
static int vce_default_initialize();
static int vce_default_deinitialize(void);

static int vce_default_get_info(char** engine_uuid, char** engine_name, char** engine_setting, bool* use_network);
static int vce_default_get_recording_format(const char* audio_id, vce_audio_type_e* types, int* rate, int* channels);
static int vce_default_foreach_langs(vce_supported_language_cb callback, void* user_data);
static bool vce_default_is_lang_supported(const char* lang);

static int vce_default_set_language(const char* language);
static int vce_default_set_commands(vce_cmd_h vc_command);
static int vce_default_unset_commands();

static int vce_default_start(bool stop_by_silence);
static int vce_default_set_recording(const void* data, unsigned int length, vce_speech_detect_e* silence_detected);
static int vce_default_stop();
static int vce_default_cancel(void);

static int vce_default_set_audio_type(const char* audio_type);
static int vce_default_set_domain(const char* domain);
static int vce_default_process_text(const char* text);
static int vce_default_process_list_event(const char* event);
static int vce_default_process_haptic_event(const char* event);

// Optional callback function
static int vce_default_private_data_set_cb(const char* key, const char* data);

int main(int argc, char* argv[])
{
	// 1. Create a structure 'vce_request_callback_s'
	vce_request_callback_s callback = {0,};

	callback.version = 1;

	callback.initialize	= vce_default_initialize;
	callback.deinitialize	= vce_default_deinitialize;

	callback.get_info	= vce_default_get_info;
	callback.get_recording_format	= vce_default_get_recording_format;
	callback.foreach_langs	= vce_default_foreach_langs;
	callback.is_lang_supported	= vce_default_is_lang_supported;

	callback.set_language	= vce_default_set_language;
	callback.set_commands	= vce_default_set_commands;
	callback.unset_commands	= vce_default_unset_commands;

	callback.start		= vce_default_start;
	callback.set_recording	= vce_default_set_recording;
	callback.stop		= vce_default_stop;
	callback.cancel		= vce_default_cancel;

	callback.set_audio_type	= vce_default_set_audio_type;
	callback.set_domain	= vce_default_set_domain;
	callback.process_text	= vce_default_process_text;
	callback.process_list_event	= vce_default_process_list_event;
	callback.process_haptic_event	= vce_default_process_haptic_event;

    // 2. Run 'vce_main()'
	if (0 != vce_main(argc, argv, &callback)) {
		SLOG(LOG_ERROR, TAG_VCE, "[ERROR] Fail to vce main");
		return -1;
	}

	// Optional
	vce_set_private_data_set_cb(vce_default_private_data_set_cb);

    // 3. Set event callbacks for service app and Run 'service_app_main()'
	char ad[50] = {0,};
	service_app_lifecycle_callback_s event_callback;
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	service_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, service_app_low_battery, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, service_app_low_memory, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, service_app_lang_changed, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, service_app_region_changed, &ad);

	return service_app_main(argc, argv, &event_callback, ad);
}

 * @endcode
 */
int vce_main(int argc, char** argv, vce_request_callback_s* callback);

/**
 * @brief Sends the results to the engine service user.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch (Since 7.0) \n
 *            %http://tizen.org/privilege/datasharing (Since 7.0)
 * @remarks To use this function, privileges of appmanager.launch and datasharing should be included since 7.0.
 * 
 * @param[in] event A result event
 * @param[in] result_id Result ids
 * @param[in] count Result count
 * @param[in] all_result All result text
 * @param[in] non_fixed_result Non-fixed command result text
 * @param[in] nlu_result NLU result text
 * @param[in] msg Engine message (e.g. #VC_RESULT_MESSAGE_NONE, #VC_RESULT_MESSAGE_ERROR_TOO_LOUD)
 * @param[out] user_info A user info (e.g. If ASR result is consumed, the value is 0x01. If not, the value is 0x00.)
 * @param[in] user_data The user data passed from set callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OUT_OF_MEMORY Out of Memory
 * @retval #VCE_ERROR_INVALID_STATE Invalid state
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 * @pre The vce_main() function should be invoked before this function is called.
 *      vce_stop_cb() will invoke this callback.
 * @see vce_stop_cb()
 */
int vce_send_result(vce_result_event_e event, int* result_id, int count, const char* all_result, const char* non_fixed_result, const char* nlu_result, const char* msg, int* user_info, void* user_data);

/**
 * @brief Sends the ASR result to the engine service user.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch (Since 7.0) \n
 *            %http://tizen.org/privilege/datasharing (Since 7.0)
 * @remarks To use this function, privileges of appmanager.launch and datasharing should be included since 7.0.
 * 
 * @param[in] event A asr result event
 * @param[in] asr_result A asr result text
 * @param[in] user_data The user data passed from the start
 * @return @c 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OUT_OF_MEMORY Out of Memory
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 * @pre The vce_main() function should be invoked before this function is called.
 * @see vce_start_cb()
 */
int vce_send_asr_result(vce_asr_result_event_e event, const char* asr_result, void* user_data);

/**
 * @brief Sends the NLG (Natural Language Generation) result to the engine service user.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch (Since 7.0) \n
 *            %http://tizen.org/privilege/datasharing (Since 7.0)
 * @remarks To use this function, privileges of appmanager.launch and datasharing should be included since 7.0.
 * 
 * @param[in] nlg_result A nlg result
 * @param[in] user_data The user data passed from the start
 * @return @c 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OUT_OF_MEMORY Out of Memory
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 * @pre The vce_main() function should be invoked before this function is called.
 * @see vce_start_cb()
 */
int vce_send_nlg_result(const char* nlg_result, void* user_data);

/**
 * @brief Sends the specific engine result to the engine service user.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch (Since 7.0) \n
 *            %http://tizen.org/privilege/datasharing (Since 7.0)
 * @remarks To use this function, privileges of appmanager.launch and datasharing should be included since 7.0.
 *
 * @param[in] engine_app_id A specific engine's app id
 * @param[in] event A specific engine result event
 * @param[in] result A specific engine result text
 * @param[in] user_info The user info passed from the start
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OUT_OF_MEMORY Out of Memory
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 * @pre The vce_main() function should be invoked before this function is called.
 */
int vce_send_specific_engine_result(const char* engine_app_id, const char* event, const char* result, void *user_info);

/**
 * @brief Sends the error to the engine service user.
 * @details The following error codes can be delivered. \n
 *          #VCE_ERROR_NONE, \n
 *          #VCE_ERROR_OUT_OF_MEMORY, \n
 *          #VCE_ERROR_IO_ERROR, \n
 *          #VCE_ERROR_INVALID_PARAMETER, \n
 *          #VCE_ERROR_OUT_OF_NETWORK, \n
 *          #VCE_ERROR_RECORDER_BUSY, \n
 *          #VCE_ERROR_NOT_SUPPORTED, \n
 *          #VCE_ERROR_INVALID_STATE, \n
 *          #VCE_ERROR_INVALID_LANGUAGE, \n
 *          #VCE_ERROR_OPERATION_FAILED, \n
 *          #VCE_ERROR_PERMISSION_DENIED, \n
 *          #VCE_ERROR_NOT_SUPPORTED_FEATURE \n
 *          #VCE_ERROR_TTS_FAILED.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch (Since 7.0) \n
 *            %http://tizen.org/privilege/datasharing (Since 7.0)
 * @remarks To use this function, privileges of appmanager.launch and datasharing should be included since 7.0.
 * 
 * @param[in] error Error type
 * @param[in] msg Error message
 * @param[in] user_data The user data passed from set callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OUT_OF_MEMORY Out of Memory
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 * @pre The vce_main() function should be invoked before this function is called.
 */
int vce_send_error(vce_error_e error, const char* msg, void* user_data);

/**
 * @brief Sets a callback function for setting the private data to the engine service.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @remarks The vce_private_data_set_cb() function is called when the engine service user sets the private data to the engine service.
 * 
 * @param[in] callback_func vce_private_data_set event callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 * @retval #VCE_ERROR_NOT_SUPPORTED_FEATURE Not supported feature
 * @pre The vce_main() function should be invoked before this function is called.
 * @see vce_private_data_set_cb()
 */
int vce_set_private_data_set_cb(vce_private_data_set_cb callback_func);

/**
 * @brief Sets a callback function for requesting the private data to the engine service.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @remarks The vce_private_data_requested_cb() function is called when the engine service user requests the private data to the engine service.
 * 
 * @param[in] callback_func vce_private_data_requested event callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 * @see vce_private_data_requested_cb()
 */
int vce_set_private_data_requested_cb(vce_private_data_requested_cb callback_func);

/**
 * @brief Sets a callback function for requesting the NLU base information to the engine service.
 * @since_tizen 4.0
 * @remarks The vce_nlu_base_info_requested_cb() function is called when the engine service user requests the NLU base information to the engine service.
 * @param[in] callback_func vce_nlu_base_info_requested event callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 * @retval #VCE_ERROR_NOT_SUPPORTED_FEATURE Not supported feature
 * @see vce_nlu_base_info_requested_cb()
 */
int vce_set_nlu_base_info_requested_cb(vce_nlu_base_info_requested_cb callback_func);

/**
 * @brief Sets a callback function for getting the engine service request.
 * @since_tizen 4.0
 *
 * @param[in] callback_func Callback function to register
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see vce_unset_specific_engine_request_cb()
 */
int vce_set_specific_engine_request_cb(vce_specific_engine_request_cb callback_func);

/**
 * @brief Unsets the engine service request callback function.
 * @since_tizen 4.0
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 *
 * @see vce_set_specific_engine_request_cb()
 */
int vce_unset_specific_engine_request_cb(void);

/**
 * @brief Retrieves all commands using callback function.
 * @since_tizen 4.0
 *
 * @param[in] vce_command The handle to be passed to the vce_set_commands_cb() function
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 * @retval #VCE_ERROR_INVALID_STATE Invalid state
 * @post This function invokes vce_command_cb() repeatedly for getting commands.
 * @see vce_command_cb()
 * @see vce_set_commands_cb()
 */
int vce_get_foreach_command(vce_cmd_h vce_command, vce_command_cb callback, void* user_data);

/**
 * @brief Gets command length.
 * @since_tizen 4.0
 *
 * @param[in] vce_command The handle to be passed to the vce_set_commands_cb() function
 * @param[out] count The command count value
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 * @see vce_set_commands_cb()
 */
int vce_get_command_count(vce_cmd_h vce_command, int* count);

/**
 * @brief Gets current audio type.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @remarks The @a audio_type must be released using free() when it is no longer required.
 * 
 * @param[in] audio_type Current audio type (e.g. #VCE_AUDIO_ID_BLUETOOTH or #VCE_AUDIO_ID_WIFI)
 * @return the value greater than 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 */
int vce_get_audio_type(char** audio_type);

/**
 * @brief Sets private data to a voice manager client.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder \n
 *            %http://tizen.org/privilege/appmanager.launch (Since 7.0) \n
 *            %http://tizen.org/privilege/datasharing (Since 7.0)
 * @remarks To use this function, privileges of appmanager.launch and datasharing should be included since 7.0.
 * 
 * @param[in] key Private key
 * @param[in] data Private data
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_INVALID_STATE Invalid state
 * @retval #VCE_ERROR_OUT_OF_MEMORY Out of Memory
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failed
 */
int vce_set_private_data(const char* key, const char* data);

/**
 * @brief Gets private data from a voice manager client.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder \n
 *            %http://tizen.org/privilege/appmanager.launch (Since 7.0) \n
 *            %http://tizen.org/privilege/datasharing (Since 7.0)
 * @remarks To use this function, privileges of appmanager.launch and datasharing should be included since 7.0. \n
 *          The @a data must be released using free() when it is no longer required.
 * 
 * @param[in] key Private key
 * @param[out] data Private data
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_INVALID_STATE Invalid state
 * @retval #VCE_ERROR_OUT_OF_MEMORY Out of Memory
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failed
 */
int vce_get_private_data(const char* key, char** data);

/**
 * @brief Starts recording voice.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder \n
 *            %http://tizen.org/privilege/appmanager.launch (Since 7.0) \n
 *            %http://tizen.org/privilege/datasharing (Since 7.0)
 * @remarks To use this function, privileges of appmanager.launch and datasharing should be included since 7.0.
 * 
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 * @retval #VCE_ERROR_RECORDER_BUSY Busy recorder
 */
int vce_start_recording(void);

/**
 * @brief Stops recording voice.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * 
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 */
int vce_stop_recording(void);

/**
 * @brief Sends audio formats necessary for playing TTS feedback.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch (Since 7.0) \n
 *            %http://tizen.org/privilege/datasharing (Since 7.0)
 * @remarks To use this function, privileges of appmanager.launch and datasharing should be included since 7.0.
 *
 * @param[in] rate A sampling rate
 * @param[in] channel The audio channel
 * @param[in] audio_type The audio type
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 * @retval #VCE_ERROR_OUT_OF_MEMORY Out of Memory
 */
int vce_send_feedback_audio_format(int rate, vce_audio_channel_e channel, vce_audio_type_e audio_type);

/**
 * @brief Sends audio streaming necessary for playing TTS feedback.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch (Since 7.0) \n
 *            %http://tizen.org/privilege/datasharing (Since 7.0)
 * @remarks To use this function, privileges of appmanager.launch and datasharing should be included since 7.0.
 *
 * @param[in] event A feedback event
 * @param[in] buffer The feedback data
 * @param[in] len The length of the feedback data
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VCE_ERROR_OPERATION_FAILED Operation failure
 * @retval #VCE_ERROR_OUT_OF_MEMORY Out of Memory
 */
int vce_send_feedback_streaming(vce_feedback_event_e event, char* buffer, int len);

/**
 * @brief Sets a callback function for getting the request of sending TTS feedback from the engine service user.
 * @since_tizen 5.0
 *
 * @param[in] callback_func Callback function to be registered
 * @param[in] user_data The user data passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see vce_request_tts_cb()
 * @see vce_unset_request_tts_cb()
 */
int vce_set_request_tts_cb(vce_request_tts_cb callback_func, void* user_data);

/**
 * @brief Unsets the TTS feedback request callback function.
 * @since_tizen 5.0
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 *
 * @see vce_set_request_tts_cb()
 */
int vce_unset_request_tts_cb(void);

/**
 * @brief Sets a callback function for getting the request of canceling TTS feedback from the engine service user.
 * @since_tizen 5.0
 *
 * @param[in] callback_func Callback function to be registered
 * @param[in] user_data The user data passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see vce_cancel_tts_cb()
 * @see vce_unset_cancel_tts_cb()
 */
int vce_set_cancel_tts_cb(vce_cancel_tts_cb callback_func, void* user_data);

/**
 * @brief Unsets the TTS feedback cancellation callback function.
 * @since_tizen 5.0
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 *
 * @see vce_set_cancel_tts_cb()
 */
int vce_unset_cancel_tts_cb(void);

/**
 * @brief Sets a callback function for sending TTS audio format to the engine service user.
 * @since_tizen 5.0
 *
 * @param[in] callback_func Callback function to be registered
 * @param[in] user_data The user data passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 * @retval #VCE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see vce_tts_audio_format_request_cb()
 * @see vce_unset_get_tts_audio_format_cb()
 */
int vce_set_tts_audio_format_request_cb(vce_tts_audio_format_request_cb callback_func, void* user_data);

/**
 * @brief Unsets the TTS audio format request callback function.
 * @since_tizen 5.0
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VCE_ERROR_NONE Successful
 * @retval #VCE_ERROR_NOT_SUPPORTED Not supported
 *
 * @see vce_set_tts_audio_format_request_cb()
 */
int vce_unset_get_tts_audio_format_cb(void);


#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __VCE_H__ */


