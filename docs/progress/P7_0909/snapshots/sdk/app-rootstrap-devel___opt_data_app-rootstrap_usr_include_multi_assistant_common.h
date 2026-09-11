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


#ifndef __TIZEN_UIFW_MULTI_ASSISTANT_COMMON_H__
#define __TIZEN_UIFW_MULTI_ASSISTANT_COMMON_H__

#include <tizen.h>

/**
 * @addtogroup CAPI_UIX_MULTI_ASSISTANT_MODULE
 * @{
 */


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Enumerations for multi-assistant error codes.
 * @since_tizen 5.0
 */
typedef enum {
	MA_ERROR_NONE = TIZEN_ERROR_NONE,		/**< Successful */
	MA_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of Memory */
	MA_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< I/O error */
	MA_ERROR_INVALID_PARAMETER	= TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	MA_ERROR_TIMED_OUT = TIZEN_ERROR_TIMED_OUT, /**< No answer from service */
	MA_ERROR_RECORDER_BUSY = TIZEN_ERROR_RESOURCE_BUSY, /**< Busy recorder */
	MA_ERROR_PERMISSION_DENIED	= TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	MA_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Multi-assistant NOT supported */
	MA_ERROR_INVALID_STATE = TIZEN_ERROR_MULTI_ASSISTANT | 0x011, /**< Invalid state */
	MA_ERROR_INVALID_LANGUAGE = TIZEN_ERROR_MULTI_ASSISTANT | 0x012, /**< Invalid language */
	MA_ERROR_ENGINE_NOT_FOUND = TIZEN_ERROR_MULTI_ASSISTANT | 0x013, /**< No available engine */
	MA_ERROR_OPERATION_FAILED = TIZEN_ERROR_MULTI_ASSISTANT | 0x014, /**< Operation failed */
	MA_ERROR_SERVICE_RESET = TIZEN_ERROR_MULTI_ASSISTANT | 0x015, /**< Service daemon reset */
	MA_ERROR_NOT_SUPPORTED_FEATURE = TIZEN_ERROR_MULTI_ASSISTANT | 0x016 /**< Not supported feature of current engine */
} ma_error_e;

/**
 * @brief Enumerations for multi-assistant client state.
 * @since_tizen 5.0
 */
typedef enum {
	MA_STATE_NONE = 0,			/**< 'None' state */
	MA_STATE_INITIALIZED = 1,	/**< 'Initialized' state */
	MA_STATE_READY = 2,			/**< 'Ready' state */
} ma_state_e;

/**
 * @brief Enumerations for multi-assistant audio streaming events.
 * @since_tizen 5.0
 */
typedef enum {
	MA_AUDIO_STREAMING_EVENT_FAIL = -1,		/**< Failed */
	MA_AUDIO_STREAMING_EVENT_START = 1,		/**< Start event */
	MA_AUDIO_STREAMING_EVENT_CONTINUE = 2,	/**< Continue event */
	MA_AUDIO_STREAMING_EVENT_FINISH = 3		/**< Finish event */
} ma_audio_streaming_event_e;

/**
 * @brief Enumerations for audio types.
 * @since_tizen 5.0
 */
typedef enum {
	MA_AUDIO_TYPE_PCM_S16_LE = 0,	/**< Signed 16bit audio type, Little endian */
	MA_AUDIO_TYPE_PCM_U8			/**< Unsigned 8bit audio type */
} ma_audio_type_e;

/**
 * @brief Enumerations for audio channels.
 * @since_tizen 5.0
 */
typedef enum {
	MA_AUDIO_CHANNEL_MONO = 0,		/**< 1 channel, mono */
	MA_AUDIO_CHANNEL_STEREO = 1		/**< 2 channels, stereo */
} ma_audio_channel_e;

/**
 * @brief Enumerations for ASR result events.
 * @since_tizen 5.0
 */
typedef enum {
	MA_ASR_RESULT_EVENT_PARTIAL_RESULT = 0,		/**< ASR result event for partial result */
	MA_ASR_RESULT_EVENT_FINAL_RESULT,			/**< ASR result event for final result */
	MA_ASR_RESULT_EVENT_ERROR					/**< ASR result event for error */
} ma_asr_result_event_e;

/**
 * @brief Enumerations for recognition result events.
 * @since_tizen 5.5
 */
typedef enum {
	MA_RECOGNITION_RESULT_EVENT_SUCCESS = 0,		/**< Recognition succeeded */
	MA_RECOGNITION_RESULT_EVENT_EMPTY_TEXT,			/**< No text result recognized */
	MA_RECOGNITION_RESULT_EVENT_ERROR,				/**< Unknown error occurred */
	MA_RECOGNITION_RESULT_EVENT_FALSE_TRIGGER		/**< Turned out to be a false trigger */
} ma_recognition_result_event_e;

/**
 * @brief Enumerations for speech data types.
 * @since_tizen 5.5
 */
typedef enum {
	MA_AUDIO_STREAMING_DATA_TYPE_CURRENT_UTTERANCE = 0,		/**< Current utterance */
	MA_AUDIO_STREAMING_DATA_TYPE_PREVIOUS_UTTERANCE,			/**< Previous utterance */
	MA_AUDIO_STREAMING_DATA_TYPE_FOLLOW_UP_SPEECH				/**< Follow-up speech */
} ma_audio_streaming_data_type_e;

/**
 * @brief Enumerations for individual assistant's voice feedback state.
 * @since_tizen 5.5
 */
typedef enum {
	MA_VOICE_FEEDBACK_STATE_NONE = 0,	/**< No voice feedback being provided */
	MA_VOICE_FEEDBACK_STATE_STARTED,	/**< Voice feedback being provided */
} ma_voice_feedback_state_e;

/**
 * @brief Enumerations for multi-assistant active state.
 * @since_tizen 5.5
 */
typedef enum {
	MA_ACTIVE_STATE_INACTIVE = 0,	/**< 'Inactive' state */
	MA_ACTIVE_STATE_ACTIVE,	/**< 'Active' state */
	MA_ACTIVE_STATE_PREPROCESSING,	/**< 'Preprocessing' state */
} ma_active_state_e;

/**
 * @brief Enumerations for allowing preprocessing by other voice assistants.
 * @since_tizen 5.5
 */
typedef enum {
	MA_PREPROCESSING_ALLOW_NONE = 0,	/**< No preprocessing allowed */
	MA_PREPROCESSING_ALLOW_UTTERANCE,	/**< Preprocessing allowed for utterance audio only */
	MA_PREPROCESSING_ALLOW_FOLLOW_UP,	/**< Preprocessing allowed for follow-up audio only */
	MA_PREPROCESSING_ALLOW_ALL,			/**< Preprocessing allowed for all audio */
} ma_preprocessing_allow_mode_e;

/**
 * @brief Enumerations for section information of audio streaming data.
 * @since_tizen 5.5
 */
typedef enum {
	MA_AUDIO_STREAMING_DATA_SECTION_UTTERANCE = 0,	/**< Utterance section started */
	MA_AUDIO_STREAMING_DATA_SECTION_WAKE_WORD,		/**< Wake word section started */
} ma_audio_streaming_data_section_e;

/**
 * @brief Enumerations for multi-assistant service state.
 * @since_tizen 6.0
 */
typedef enum {
	MA_SERVICE_STATE_INACTIVE = 0,			/**< 'Inactive' state */
	MA_SERVICE_STATE_LISTENING = 1,			/**< 'Listening' state */
	MA_SERVICE_STATE_UTTERANCE = 2,			/**< 'Utterance' state */
	MA_SERVICE_STATE_PROCESSING = 3,		/**< 'Processing' state */
	MA_SERVICE_STATE_VOICE_FEEDBACK = 4,	/**< 'VoiceFeedback' state */
} ma_service_state_e;

/**
 * @brief Enumerations for voice key status change.
 * @details When the voice key is pressed, the "pressed" state is reported.
 *          If the voice key gets released within the timeout value for tap events,
 *          the voice key status value will be reported as "released after tap".
 *          Otherwise, "released after push" will be reported.
 * @since_tizen 6.0
  */
typedef enum {
	MA_VOICE_KEY_STATUS_PRESSED = 0,				/**< 'Pressed' state */
	MA_VOICE_KEY_STATUS_RELEASED_AFTER_PUSH = 1,	/**< 'Released' state after push */
	MA_VOICE_KEY_STATUS_RELEASED_AFTER_TAP = 2,		/**< 'Released' state after tap*/
} ma_voice_key_status_e;

/**
 * @brief Called when the client state is changed.
 * @since_tizen 5.0
 *
 * @param[in] previous The previous state
 * @param[in] current The current state
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef void (*ma_state_changed_cb)(ma_state_e previous, ma_state_e current, void* user_data);

/**
 * @brief Called when the error is occurred.
 * @details The following error codes can be received: \n
 *          #MA_ERROR_NONE:                         Success \n
 *          #MA_ERROR_OUT_OF_MEMORY:                Out of Memory \n
 *          #MA_ERROR_IO_ERROR:                     I/O error \n
 *          #MA_ERROR_INVALID_PARAMETER:            Invalid parameter \n
 *          #MA_ERROR_TIMED_OUT:                    No answer from service \n
 *          #MA_ERROR_RECORDER_BUSY:                Busy recorder \n
 *          #MA_ERROR_PERMISSION_DENIED:            Permission denied \n
 *          #MA_ERROR_NOT_SUPPORTED:                Multi-assistant NOT supported \n
 *          #MA_ERROR_INVALID_STATE:                Invalid state \n
 *          #MA_ERROR_INVALID_LANGUAGE:             Invalid language \n
 *          #MA_ERROR_ENGINE_NOT_FOUND:             No available engine \n
 *          #MA_ERROR_OPERATION_FAILED:             Operation failed \n
 *          #MA_ERROR_SERVICE_RESET:                Service daemon reset \n
 *          #MA_ERROR_NOT_SUPPORTED_FEATURE:        Not supported feature of current engine \n
 * @since_tizen 5.0
 *
 * @param[in] reason The error reason
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef void (*ma_error_cb)(ma_error_e reason, void* user_data);

/**
 * @brief Called when the default language is changed.
 * @since_tizen 5.0
 * @remarks The @a previous can be used only in the callback. To use outside, make a copy.
 *          The @a current can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] previous The previous language
 * @param[in] current The current language
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see ma_set_language_changed_cb()
 * @see ma_unset_language_changed_cb()
 */
typedef void (*ma_language_changed_cb)(const char* previous, const char* current, void* user_data);

/**
 * @brief Called when the multi-assistant service sends audio streaming.
 * @since_tizen 5.0
 * @remarks The @a buffer should not be released and can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] event The audio streaming event
 * @param[in] buffer The audio streaming data
 * @param[in] len The length of the audio streaming data
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see ma_set_audio_streaming_cb()
 * @see ma_unset_audio_streaming_cb()
 */
typedef void (*ma_audio_streaming_cb)(ma_audio_streaming_event_e event, char* buffer, int len, void* user_data);

/**
 * @brief Called when the active state is changed.
 * @since_tizen 5.5
 *
 * @param[in] previous The previous active state
 * @param[in] current The current active state
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef void (*ma_active_state_changed_cb)(ma_active_state_e previous, ma_active_state_e current, void* user_data);

/**
 * @brief Called when a wakeup engine specific command is retrieved.
 * @since_tizen 5.5
 * @remarks The @a command should not be released and can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] command The wakeup engine specific command
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef void (*ma_wakeup_engine_command_cb)(const char *command, void* user_data);

/**
 * @brief A handle to get assistant information.
 * @since_tizen 5.5
 */

typedef void *ma_assistant_info_h;

/**
 * @brief Called whenever to get the assistant information of each installed assistant.
 * @since_tizen 5.5
 *
 * @remarks The @a handle should not be released.
 * @remarks The @a handle is managed by the platform and will be released when application exits.
 * @param[in] handle The handle of the assistant
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef int (*ma_assistant_info_list_cb)(ma_assistant_info_h handle, void* user_data);

/**
 * @brief Called when the preprocessing information is changed.
 * @since_tizen 5.5
 *
 * @remarks The @a app_id should not be released and can be used only in the callback. To use outside, make a copy.
 * @remarks If the @a app_id is NULL, it means there is no preprocessing voice assistant available.
 * @param[in] app_id The application id of current preprocessing voice assistant
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef void (*ma_preprocessing_information_changed_cb)(const char* app_id, void* user_data);

/**
 * @brief Called when the section of audio streaming data is changed.
 * @since_tizen 5.5
 *
 * @param[in] section The current section information of audio streaming data
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef void (*ma_audio_streaming_data_section_changed_cb)(ma_audio_streaming_data_section_e section, void* user_data);

/**
 * @brief Called when the service state is changed.
 * @since_tizen 6.0
 *
 * @param[in] previous The previous service state
 * @param[in] current The new service state
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef void (*ma_service_state_changed_cb)(ma_service_state_e previous, ma_service_state_e current, void* user_data);

/**
 * @brief Called when the multi-assistant service sends voice key status change event.
 * @since_tizen 6.0
 *
 * @param[in] status The change of voice key status
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see ma_set_voice_key_status_changed_cb()
 * @see ma_unset_voice_key_status_changed_cb()
 */
typedef void (*ma_voice_key_status_changed_cb)(ma_voice_key_status_e status, void* user_data);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __TIZEN_UIFW_MULTI_ASSISTANT_COMMON_H__ */

