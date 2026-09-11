/*
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


#ifndef __TTS_H__
#define __TTS_H__


#include <tizen.h>
#include <tizen_error.h>


/**
* @file tts.h
*/


/**
* @addtogroup CAPI_UIX_TTS_MODULE
* @{
*/


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Enumeration for error code.
 * @since_tizen 2.3
*/
typedef enum {
	TTS_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	TTS_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of Memory */
	TTS_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< I/O error */
	TTS_ERROR_INVALID_PARAMETER	= TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	TTS_ERROR_OUT_OF_NETWORK = TIZEN_ERROR_NETWORK_DOWN, /**< Network is down */
	TTS_ERROR_TIMED_OUT = TIZEN_ERROR_TIMED_OUT, /**< No answer from the daemon */
	TTS_ERROR_PERMISSION_DENIED	= TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	TTS_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< TTS NOT supported */
	TTS_ERROR_INVALID_STATE = TIZEN_ERROR_TTS | 0x01, /**< Invalid state */
	TTS_ERROR_INVALID_VOICE = TIZEN_ERROR_TTS | 0x02, /**< Invalid voice */
	TTS_ERROR_ENGINE_NOT_FOUND = TIZEN_ERROR_TTS | 0x03, /**< No available engine */
	TTS_ERROR_OPERATION_FAILED = TIZEN_ERROR_TTS | 0x04, /**< Operation failed */
	TTS_ERROR_AUDIO_POLICY_BLOCKED = TIZEN_ERROR_TTS | 0x05, /**< Audio policy blocked */
	TTS_ERROR_NOT_SUPPORTED_FEATURE	= TIZEN_ERROR_TTS | 0x06, /**< Not supported feature of current engine (Since 3.0) */
	TTS_ERROR_SERVICE_RESET = TIZEN_ERROR_TTS | 0x07, /**< Service reset (Since 3.0) */
	TTS_ERROR_SCREEN_READER_OFF = TIZEN_ERROR_TTS | 0x08 /**< Screen reader is off (Since 6.5) */
} tts_error_e;


/**
 * @brief Enumeration for TTS mode.
 * @since_tizen 2.3
*/
typedef enum {
	TTS_MODE_DEFAULT = 0, /**< Default mode for normal application */
	TTS_MODE_NOTIFICATION = 1, /**< Notification mode */
	TTS_MODE_SCREEN_READER = 2 /**< Accessibility mode */
} tts_mode_e;


/**
 * @brief Enumeration for state.
 * @since_tizen 2.3
*/
typedef enum {
	TTS_STATE_CREATED = 0, /**< 'CREATED' state */
	TTS_STATE_READY = 1, /**< 'READY' state */
	TTS_STATE_PLAYING = 2, /**< 'PLAYING' state */
	TTS_STATE_PAUSED = 3 /**< 'PAUSED' state*/
} tts_state_e;


/**
 * @brief Enumeration for service state.
 * @since_tizen 7.0
*/
typedef enum {
	TTS_SERVICE_STATE_NONE = -1, /**< 'None' state (Since 8.0) */
	TTS_SERVICE_STATE_READY = 0, /**< 'Ready' state */
	TTS_SERVICE_STATE_SYNTHESIZING, /**< 'Synthesizing' state */
	TTS_SERVICE_STATE_PLAYING, /**< 'Playing' state */
	TTS_SERVICE_STATE_BLOCKED /**< 'Blocked' state (Since 8.0) */
} tts_service_state_e;


/**
 * @brief Enumeration for playing mode of TTS.
 * @since_tizen 8.0
*/
typedef enum {
	TTS_PLAYING_MODE_BY_SERVICE = 0, /**< Mode for TTS playing on TTS service */
	TTS_PLAYING_MODE_BY_CLIENT = 1, /**< Mode for TTS playing on TTS client */
} tts_playing_mode_e;


/**
 * @brief Enumeration for audio type.
 * @since_tizen 8.0
 */
typedef enum {
	TTS_AUDIO_TYPE_RAW_S16 = 0, /**< Signed 16-bit audio type */
	TTS_AUDIO_TYPE_RAW_U8 /**< Unsigned 8-bit audio type */
} tts_audio_type_e;


/**
 * @brief Enumeration for synthesized pcm event.
 * @since_tizen 8.0
*/
typedef enum {
	TTS_SYNTHESIZED_PCM_EVENT_FAIL = -1, /**< Event when the synthesized PCM is failed */
	TTS_SYNTHESIZED_PCM_EVENT_START = 1, /**< Event when the synthesized PCM is received at first */
	TTS_SYNTHESIZED_PCM_EVENT_CONTINUE = 2, /**< Event when the synthesized PCM is received, not first and not last */
	TTS_SYNTHESIZED_PCM_EVENT_FINISH = 3 /**< Event when the synthesized PCM is received finally */
} tts_synthesized_pcm_event_e;


/**
 * @brief Definition for automatic speaking speed.
 * @since_tizen 2.3
*/
#define TTS_SPEED_AUTO		0


/**
 * @brief Definition for automatic speaking pitch.
 * @since_tizen 9.0
*/
#define TTS_PITCH_AUTO		0


/**
 * @brief Definition for automatic speaking volume.
 * @since_tizen 9.0
*/
#define TTS_VOLUME_AUTO		0


/**
 * @brief Definition for automatic voice type.
 * @since_tizen 2.3
*/
#define TTS_VOICE_TYPE_AUTO	0


/**
 * @brief Definition for male voice type.
 * @since_tizen 2.3
*/
#define TTS_VOICE_TYPE_MALE	1


/**
 * @brief Definition for female voice type.
 * @since_tizen 2.3
*/
#define TTS_VOICE_TYPE_FEMALE	2


/**
 * @brief Definition for child voice type.
 * @since_tizen 2.3
*/
#define TTS_VOICE_TYPE_CHILD	3


/**
 * @brief Definition for personal voice type.
 * @since_tizen 9.0
*/
#define TTS_VOICE_TYPE_PERSONAL  4


/**
 * @brief The TTS handle.
 * @since_tizen 2.3
*/
typedef struct tts_s *tts_h;


/**
 * @brief The TTS synthesis parameter handle.
 * @since_tizen 9.0
*/
typedef struct tts_synthesis_parameter_s *tts_synthesis_parameter_h;


/**
 * @brief Called when the state of TTS is changed.
 * @details If the daemon must stop player because of changing engine and
 *          the daemon must pause player because of other requests, this callback function is called.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[in] previous The previous state
 * @param[in] current The current state
 * @param[in] user_data The user data passed from the callback registration function
 * @pre An application registers this callback using tts_set_state_changed_cb() to detect changing state.
 * @see tts_set_state_changed_cb()
 * @see tts_unset_state_changed_cb()
*/
typedef void (*tts_state_changed_cb)(tts_h tts, tts_state_e previous, tts_state_e current, void* user_data);


/**
 * @brief Called when utterance has started.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[in] utt_id The utterance ID passed from the add text function
 * @param[in] user_data The user data passed from the callback registration function
 * @pre An application registers this callback using tts_set_utterance_started_cb() to detect utterance started.
 * @see tts_add_text()
 * @see tts_set_utterance_started_cb()
 * @see tts_unset_utterance_started_cb()
*/
typedef void (*tts_utterance_started_cb)(tts_h tts, int utt_id, void* user_data);


/**
 * @brief Called when utterance is finished.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[in] utt_id The utterance ID passed from the add text function
 * @param[in] user_data The user data passed from the callback registration function
 * @pre An application registers this callback using tts_set_utterance_completed_cb() to detect utterance completed.
 * @see tts_add_text()
 * @see tts_set_utterance_completed_cb()
 * @see tts_unset_utterance_completed_cb()
*/
typedef void (*tts_utterance_completed_cb)(tts_h tts, int utt_id, void *user_data);


/**
 * @brief Called when an error occurs.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[in] utt_id The utterance ID passed from the add text function
 * @param[in] reason The error code
 * @param[in] user_data The user data passed from the callback registration function
 * @pre An application registers this callback using tts_set_error_cb() to detect error.
 * @see tts_play()
 * @see tts_pause()
 * @see tts_stop()
 * @see tts_set_error_cb()
 * @see tts_unset_error_cb()
*/
typedef void (*tts_error_cb)(tts_h tts, int utt_id, tts_error_e reason, void* user_data);


/**
 * @brief Called to retrieve the supported voice.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[in] language Language specified as an ISO 3166 alpha-2 two letter country-code followed by ISO 639-1 for the two-letter language code (for example, "ko_KR" for Korean, "en_US" for American English)
 * @param[in] voice_type A voice type (e.g. #TTS_VOICE_TYPE_MALE, #TTS_VOICE_TYPE_FEMALE)
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop,
 *         @c false to break out of the loop
 * @pre tts_foreach_supported_voices() will invoke this callback function.
 * @see tts_foreach_supported_voices()
*/
typedef bool (*tts_supported_voice_cb)(tts_h tts, const char* language, int voice_type, void* user_data);

/**
 * @brief Called to retrieve the supported personal voice.
 * @since_tizen 9.0
 * @param[in] tts The TTS handle
 * @param[in] language Language specified as ISO 3166 alpha-2 two letter country-code followed by ISO 639-1 for the two-letter language code (for example, "ko_KR" for Korean, "en_US" for American English)
 * @param[in] unique_id A unique identifier string for personal voice resource
 * @param[in] display_name A display name that will be shown in UI for user selection
 * @param[in] device_name A device name used for identifying the source target
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop,
 *         @c false to break out of the loop
 * @pre tts_foreach_supported_personal_voices() will invoke this callback function.
 * @see tts_foreach_supported_personal_voices()
*/
typedef bool (*tts_supported_personal_voice_cb)(tts_h tts, const char* language, const char* unique_id, const char* display_name, const char* device_name, void* user_data);

/**
 * @brief Called when the default voice is changed.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[in] previous_language The previous language
 * @param[in] previous_voice_type The previous voice type
 * @param[in] current_language The current language
 * @param[in] current_voice_type The current voice type
 * @param[in] user_data The user data passed from the callback registration function
 * @see tts_set_default_voice_changed_cb()
*/
typedef void (*tts_default_voice_changed_cb)(tts_h tts, const char* previous_language, int previous_voice_type,
				const char* current_language, int current_voice_type, void* user_data);


/**
 * @brief Called when the engine is changed.
 * @since_tizen 3.0
 * @param[in] tts The TTS handle
 * @param[in] engine_id Engine ID
 * @param[in] language The default language specified as an ISO 3166 alpha-2 two letter country-code followed by ISO 639-1 for the two-letter language code (for example, "ko_KR" for Korean, "en_US" for American English)
 * @param[in] voice_type The default voice type
 * @param[in] need_credential The necessity of credential
 * @param[in] user_data The user data passed from the callback registration function
 * @see tts_set_engine_changed_cb()
*/
typedef void (*tts_engine_changed_cb)(tts_h tts, const char* engine_id, const char* language, int voice_type, bool need_credential, void* user_data);


/**
 * @brief Called when the option of screen reader is changed.
 * @since_tizen 6.5
 * @param[in] tts The TTS handle
 * @param[in] is_on The status of screen reader. If @a is_on is @c true, screen reader is turned on. If not, it is turned off.
 * @param[in] user_data The user data passed from the callback registration function
 * @see tts_set_screen_reader_changed_cb()
*/
typedef void (*tts_screen_reader_changed_cb)(tts_h tts, bool is_on, void* user_data);


/**
 * @brief Called when the state of TTS service is changed.
 * @since_tizen 7.0
 * @remarks The @a tts handle should not be destroyed in the callback.
 * @param[in] tts The TTS handle, the same handle for which the callback was set.
 * @param[in] previous The previous state of TTS service
 * @param[in] current The current state of TTS service
 * @param[in] user_data The user data passed from the callback registration function
 * @pre An application registers this callback using tts_set_service_state_changed_cb() to detect changing state of TTS service.
 * @see tts_set_service_state_changed_cb()
 * @see tts_unset_service_state_changed_cb()
*/
typedef void (*tts_service_state_changed_cb)(tts_h tts, tts_service_state_e previous, tts_service_state_e current, void* user_data);


/**
 * @brief Called when the synthesized pcm data is come from the engine.
 * @since_tizen 8.0
 * @remarks The @a tts handle should not be destroyed in the callback.
 * @param[in] tts The TTS handle, the same handle for which the callback was set.
 * @param[in] utt_id The utterance ID
 * @param[in] event The event type
 * @param[in] pcm_data The synthesized pcm data. The @a pcm_data can be used only in the callback. To use outside, make a copy.
 * @param[in] pcm_data_size The size of the pcm data
 * @param[in] audio_type The audio type of pcm data
 * @param[in] sample_rate The sampling rate of pcm data
 * @param[in] user_data The user data passed from the callback registration function
 * @pre An application registers this callback using tts_set_synthesized_pcm_cb() to get pcm data.
 * @see tts_set_synthesized_pcm_cb()
 * @see tts_unset_synthesized_pcm_cb()
 */
typedef void (*tts_synthesized_pcm_cb)(tts_h tts, int utt_id, tts_synthesized_pcm_event_e event, const char* pcm_data, int pcm_data_size, tts_audio_type_e audio_type, int sample_rate, void *user_data);


/**
 * @brief Creates a handle for TTS.
 * @since_tizen 2.3
 * @remarks If the function succeeds, @a tts handle must be released with tts_destroy().
 * @param[out] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #TTS_ERROR_ENGINE_NOT_FOUND Engine not found
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @post If this function is called, the TTS state will be #TTS_STATE_CREATED.
 * @see tts_destroy()
*/
int tts_create(tts_h* tts);


/**
 * @brief Destroys the handle and disconnects the daemon.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @see tts_create()
*/
int tts_destroy(tts_h tts);


/**
 * @brief Sets the TTS mode.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[in] mode The mode
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_get_mode()
*/
int tts_set_mode(tts_h tts, tts_mode_e mode);


/**
 * @brief Gets the TTS mode.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[out] mode The mode
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_set_mode()
*/
int tts_get_mode(tts_h tts, tts_mode_e* mode);


/**
 * @brief Sets the app credential.
 * @details Using this API, the application can set a credential.
 *          The credential is a key to verify the authorization about using the engine.
 *          If the application sets the credential, it will be able to use functions of the engine entirely.
 * @since_tizen 3.0
 * @remarks The necessity of the credential depends on the engine. In case of the engine which is basically embedded in Tizen, the credential is not necessary so far.
 *          However, if the user wants to apply the 3rd party's engine, the credential may be necessary. In that case, please follow the policy provided by the corresponding engine.
 * @param[in] tts The TTS handle
 * @param[in] credential The app credential
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Success
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED or #TTS_STATE_READY.
 * @see tts_play()
*/
int tts_set_credential(tts_h tts, const char* credential);


/**
 * @brief Connects the daemon asynchronously.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_SCREEN_READER_OFF Screen reader is turned off
 * @pre The state should be #TTS_STATE_CREATED.
 * @post If this function is successful, the TTS state will be #TTS_STATE_READY.
 *       If this function is failed, the error callback is called. (e.g. #TTS_ERROR_ENGINE_NOT_FOUND)
 * @see tts_unprepare()
*/
int tts_prepare(tts_h tts);


/**
 * @brief Disconnects the daemon.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_READY.
 * @post If this function is called, the TTS state will be #TTS_STATE_CREATED.
 * @see tts_prepare()
*/
int tts_unprepare(tts_h tts);


/**
 * @brief Connects the daemon synchronously.
 * @since_tizen 7.0
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @pre The state should be #TTS_STATE_CREATED.
 * @post If this function is successful, the TTS state will be #TTS_STATE_READY.
 * @see tts_unprepare()
*/
int tts_prepare_sync(tts_h tts);


/**
 * @brief Retrieves all supported voices of the current engine using callback function.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_ENGINE_NOT_FOUND Engine not found
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @post This function invokes tts_supported_voice_cb() repeatedly for getting voices.
 * @see tts_get_default_voice()
*/
int tts_foreach_supported_voices(tts_h tts, tts_supported_voice_cb callback, void* user_data);

/**
 * @brief Retrieves all supported personal voices of the current engine using callback function.
 * @since_tizen 9.0
 * @param[in] tts The TTS handle
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_ENGINE_NOT_FOUND Engine not found
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @post This function invokes tts_supported_personal_voice_cb() repeatedly for getting personal voices.
*/
int tts_foreach_supported_personal_voices(tts_h tts, tts_supported_personal_voice_cb callback, void* user_data);

/**
 * @brief Gets the default voice set by the user.
 * @since_tizen 2.3
 * @remarks If the function succeeds, @a language must be released with free().
 * @param[in] tts The TTS handle
 * @param[out] language Language specified as an ISO 3166 alpha-2 two letter country-code followed by ISO 639-1 for the two-letter language code (for example, "ko_KR" for Korean, "en_US" for American English)
 * @param[out] voice_type The voice type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_ENGINE_NOT_FOUND Engine not found
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @see tts_foreach_supported_voices()
*/
int tts_get_default_voice(tts_h tts, char** language, int* voice_type);


/**
 * @brief Sets the private data to tts engine.
 * @details The private data is the setting parameter for applying keys provided by the engine.
 *          Using this API, the application can set the private data and use the corresponding key of the engine.
 *          For example, if the engine provides 'girl's voice' as a voice type, the application can set the private data as the following.
 *          int ret = tts_set_private_data(#tts_h, "voice_type", "GIRL");
 * @since_tizen 3.0
 * @remarks If the engine is replaced with the other engine, the key may be ignored.
 * @param[in] tts The TTS handle
 * @param[in] key The field name of private data
 * @param[in] data The data for set
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_ENGINE_NOT_FOUND Engine not found
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @pre The state should be #TTS_STATE_READY.
 * @see tts_get_private_data()
*/
int tts_set_private_data(tts_h tts, const char* key, const char* data);


/**
 * @brief Gets the private data from tts engine.
 * @details The private data is the information provided by the engine.
 *          Using this API, the application can get the private data which corresponds to the key from the engine.
 * @since_tizen 3.0
 * @remarks The @a data must be released using free() when it is no longer required.
 *          If the engine is replaced with the other engine, the key may be ignored.
 * @param[in] tts The TTS handle
 * @param[in] key The field name of private data
 * @param[out] data The data field of private data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_ENGINE_NOT_FOUND Engine not found
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @pre The state should be #TTS_STATE_READY.
 * @see tts_set_private_data()
*/
int tts_get_private_data(tts_h tts, const char* key, char** data);


/**
 * @brief Gets the maximum byte size for text.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[out] size The maximum byte size for text
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @pre The state should be #TTS_STATE_READY.
 * @see tts_add_text()
*/
int tts_get_max_text_size(tts_h tts, unsigned int* size);


/**
 * @brief Gets the current state of TTS.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[out] state The current state of TTS
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @see tts_play()
 * @see tts_stop()
 * @see tts_pause()
*/
int tts_get_state(tts_h tts, tts_state_e* state);


/**
 * @brief Gets the speed range.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[out] min The minimum speed value
 * @param[out] normal The normal speed value
 * @param[out] max The maximum speed value
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_add_text()
*/
int tts_get_speed_range(tts_h tts, int* min, int* normal, int* max);


/**
 * @brief Gets the current error message.
 * @since_tizen 3.0
 * @remarks This function should be called from a tts error callback. Calling in any other context will result in an Operation failed error.
 *          A successful call will allocate @a err_msg, which must be released by calling free() when it is no longer required.
 * @param[in] tts The TTS handle
 * @param[out] err_msg The current error message
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @see tts_set_error_cb()
 * @see tts_unset_error_cb()
*/
int tts_get_error_message(tts_h tts, char** err_msg);


/**
 * @brief Gets the current state of TTS service.
 * @since_tizen 7.0
 * @param[in] tts The TTS handle
 * @param[out] service_state The current state of TTS service
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @pre The TTS state should be one of: #TTS_STATE_READY, #TTS_STATE_PLAYING, #TTS_STATE_PAUSED.
 * @see tts_play()
 * @see tts_stop()
 * @see tts_pause()
*/
int tts_get_service_state(tts_h tts, tts_service_state_e* service_state);


/**
 * @brief Checks whether screen reader is turned on or not.
 * @since_tizen 6.5
 * @remarks If TTS mode is #TTS_MODE_SCREEN_READER, you should call this function to check whether screen reader is turned on or not, before calling 'tts_prepare()'.
 *          If TTS mode is #TTS_MODE_SCREEN_READER and @a is_on is @c false, all other functions will return #TTS_ERROR_SCREEN_READER_OFF.
 *          The @a is_on must be released using free() when it is no longer required.
 * @param[in] tts The TTS handle
 * @param[out] is_on The current status of screen reader
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @see tts_set_screen_reader_changed_cb()
 * @see tts_unset_screen_reader_changed_cb()
*/
int tts_check_screen_reader_on(tts_h tts, bool* is_on);


/**
 * @brief Adds a text to the queue.
 * @since_tizen 2.3
 * @remarks Locale(e.g. setlocale()) MUST be set for utf8 text validation check.
 * @param[in] tts The TTS handle
 * @param[in] text An input text based utf8
 * @param[in] language The language selected from the tts_foreach_supported_voices() (e.g. 'NULL'(Automatic), 'en_US')
 * @param[in] voice_type The voice type selected from the tts_foreach_supported_voices() (e.g. #TTS_VOICE_TYPE_AUTO, #TTS_VOICE_TYPE_FEMALE)
 * @param[in] speed A speaking speed (e.g. #TTS_SPEED_AUTO or the value from tts_get_speed_range())
 * @param[out] utt_id The utterance ID passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_PERMISSION_DENIED Permission denied
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_INVALID_VOICE Invalid voice about language, voice type
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @retval #TTS_ERROR_SCREEN_READER_OFF Screen reader is turned off
 * @pre The state should be #TTS_STATE_READY, #TTS_STATE_PLAYING or #TTS_STATE_PAUSED.
 * @see tts_get_max_text_size()
 * @see tts_set_credential()
*/
int tts_add_text(tts_h tts, const char* text, const char* language, int voice_type, int speed, int* utt_id);


/**
 * @brief Starts synthesizing voice from the text and plays the synthesized audio data.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_PERMISSION_DENIED Permission denied
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_OUT_OF_NETWORK Out of network
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @retval #TTS_ERROR_SCREEN_READER_OFF Screen reader is turned off
 * @pre The current state should be #TTS_STATE_READY or #TTS_STATE_PAUSED.
 * @post If this function succeeds, the TTS state will be #TTS_STATE_PLAYING.
 * @see tts_add_text()
 * @see tts_pause()
 * @see tts_stop()
 * @see tts_utterance_started_cb()
 * @see tts_utterance_completed_cb()
 * @see tts_error_cb()
 * @see tts_set_credential()
*/
int tts_play(tts_h tts);


/**
 * @brief Stops playing the utterance and clears the queue.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @retval #TTS_ERROR_SCREEN_READER_OFF Screen reader is turned off
 * @pre The TTS state should be #TTS_STATE_READY or #TTS_STATE_PLAYING or #TTS_STATE_PAUSED.
 * @post If this function succeeds, the TTS state will be #TTS_STATE_READY.
 *       This function will remove all text via tts_add_text() and synthesized sound data.
 * @see tts_play()
 * @see tts_pause()
*/
int tts_stop(tts_h tts);


/**
 * @brief Pauses the currently playing utterance.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @retval #TTS_ERROR_SCREEN_READER_OFF Screen reader is turned off
 * @pre The TTS state should be #TTS_STATE_PLAYING.
 * @post If this function succeeds, the TTS state will be #TTS_STATE_PAUSED.
 * @see tts_play()
 * @see tts_stop()
 * @see tts_error_cb()
*/
int tts_pause(tts_h tts);

/**
 * @brief Repeats the last added text.
 * @since_tizen 5.0
 * @remarks This function repeats the last added text once again.
 *          If there is no previously added text, this function will not work.
 *          If the language is changed, the last added text is removed from the service.
 *          Before calling this function, please call 'tts_stop()' in order to stop playing the previous one.
 *          If this function succeeds, @a text_repeat must be released with free().
 * @param[in] tts The TTS handle
 * @param[out] text_repeat Texts to be played repeatedly
 * @param[out] utt_id The utterance ID passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @retval #TTS_ERROR_SCREEN_READER_OFF Screen reader is turned off
 * @pre The state should be #TTS_STATE_READY.
 * @post If this function succeeds, the TTS state will be #TTS_STATE_PLAYING.
 * @see tts_add_text()
 * @see tts_stop()
 */
int tts_repeat(tts_h tts, char** text_repeat, int* utt_id);

/**
 * @brief Sets a callback function to be called when the TTS state changes.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_state_changed_cb()
 * @see tts_unset_state_changed_cb()
*/
int tts_set_state_changed_cb(tts_h tts, tts_state_changed_cb callback, void* user_data);


/**
 * @brief Unsets the callback function to be called when the TTS state changes.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_set_state_changed_cb()
*/
int tts_unset_state_changed_cb(tts_h tts);


/**
 * @brief Sets a callback function to detect utterance start.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_utterance_started_cb()
 * @see tts_unset_utterance_started_cb()
*/
int tts_set_utterance_started_cb(tts_h tts, tts_utterance_started_cb callback, void* user_data);


/**
 * @brief Unsets the callback function to detect utterance start.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_set_utterance_started_cb()
*/
int tts_unset_utterance_started_cb(tts_h tts);


/**
 * @brief Sets a callback function to detect utterance completion.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_utterance_completed_cb()
 * @see tts_unset_utterance_completed_cb()
*/
int tts_set_utterance_completed_cb(tts_h tts, tts_utterance_completed_cb callback, void* user_data);


/**
 * @brief Unsets the callback function to detect utterance completion.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_set_utterance_completed_cb()
*/
int tts_unset_utterance_completed_cb(tts_h tts);


/**
 * @brief Sets a callback function to detect errors.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_error_cb()
 * @see tts_unset_error_cb()
*/
int tts_set_error_cb(tts_h tts, tts_error_cb callback, void* user_data);


/**
 * @brief Unsets the callback function to detect errors.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_set_error_cb()
*/
int tts_unset_error_cb(tts_h tts);


/**
 * @brief Sets a callback function to detect default voice change.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_default_voice_changed_cb()
 * @see tts_unset_default_voice_changed_cb()
*/
int tts_set_default_voice_changed_cb(tts_h tts, tts_default_voice_changed_cb callback, void* user_data);


/**
 * @brief Unsets the callback function to detect default voice change.
 * @since_tizen 2.3
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_set_default_voice_changed_cb()
*/
int tts_unset_default_voice_changed_cb(tts_h tts);


 /**
 * @brief Sets a callback function to detect the engine change.
 * @since_tizen 3.0
 * @param[in] tts The TTS handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_engine_changed_cb()
 * @see tts_unset_engine_changed_cb()
*/
int tts_set_engine_changed_cb(tts_h tts, tts_engine_changed_cb callback, void* user_data);


/**
 * @brief Unsets the callback function to detect the engine change.
 * @since_tizen 3.0
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_set_engine_changed_cb()
*/
int tts_unset_engine_changed_cb(tts_h tts);


/**
 * @brief Sets a callback function to detect the option of screen reader is changed or not.
 * @since_tizen 6.5
 * @remarks If TTS mode is #TTS_MODE_SCREEN_READER, you should set the callback to check the option of screen reader is changed or not.
 * @param[in] tts The TTS handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_screen_reader_changed_cb()
 * @see tts_unset_screen_reader_changed_cb()
*/
int tts_set_screen_reader_changed_cb(tts_h tts, tts_screen_reader_changed_cb callback, void* user_data);


/**
 * @brief Unsets the callback function to detect the option of screen reader is changed or not.
 * @since_tizen 6.5
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_set_screen_reader_changed_cb()
*/
int tts_unset_screen_reader_changed_cb(tts_h tts);


/**
 * @brief Sets a callback function to be called when the TTS service state changes.
 * @since_tizen 7.0
 * @param[in] tts The TTS handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The TTS state should be #TTS_STATE_CREATED.
 * @see tts_service_state_changed_cb()
 * @see tts_unset_service_state_changed_cb()
*/
int tts_set_service_state_changed_cb(tts_h tts, tts_service_state_changed_cb callback, void* user_data);


/**
 * @brief Unsets the callback function to be called when the TTS service state changes.
 * @since_tizen 7.0
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The TTS state should be #TTS_STATE_CREATED.
 * @see tts_set_service_state_changed_cb()
*/
int tts_unset_service_state_changed_cb(tts_h tts);


/**
 * @brief Sets the TTS playing mode.
 * @since_tizen 8.0
 * @param[in] tts The TTS handle
 * @param[in] mode The mode
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The state should be #TTS_STATE_CREATED.
 * @see tts_play()
 */
int tts_set_playing_mode(tts_h tts, tts_playing_mode_e mode);


/**
 * @brief Sets the callback function to receive the synthesized pcm data from the tts service.
 * @since_tizen 8.0
 * @param[in] tts The TTS handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_PERMISSION_DENIED Permission denied
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The TTS state should be #TTS_STATE_CREATED.
 * @see tts_synthesized_pcm_cb()
 * @see tts_unset_synthesized_pcm_cb()
 */
int tts_set_synthesized_pcm_cb(tts_h tts, tts_synthesized_pcm_cb callback, void* user_data);


/**
 * @brief Unsets the callback function to receive synthesized PCM data from the tts service.
 * @since_tizen 8.0
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @pre The TTS state should be #TTS_STATE_CREATED.
 * @see tts_set_synthesized_pcm_cb()
  */
int tts_unset_synthesized_pcm_cb(tts_h tts);


/**
 * @brief Adds silence for the specified amount of time.
 * @since_tizen 9.0
 * @remarks The maximum silent time is 5000 msec. If you want to perform a silent time of more than 5000 msec, call this function several times.
 * @param[in] tts The TTS handle
 * @param[in] duration_in_msec The duration of the silence
 * @param[out] utt_id The utterance ID passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @retval #TTS_ERROR_SCREEN_READER_OFF Screen reader is turned off
 * @pre The state should be #TTS_STATE_READY, #TTS_STATE_PLAYING, or #TTS_STATE_PAUSED.
*/
int tts_add_silent_utterance(tts_h tts, unsigned int duration_in_msec, int* utt_id);


/**
 * @brief Creates a handle for TTS synthesis parameters.
 * @since_tizen 9.0
 * @remarks If the function succeeds, @a parameter handle must be released with tts_synthesis_parameter_destroy().
 * @param[out] parameter The TTS synthesis parameter handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_OUT_OF_MEMORY Out of memory
 * @see tts_synthesis_parameter_destroy()
*/
int tts_synthesis_parameter_create(tts_synthesis_parameter_h *parameter);

/**
 * @brief Destroys the TTS synthesis parameter handle.
 * @since_tizen 9.0
 * @param[in] parameter The TTS synthesis parameter handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @see tts_synthesis_parameter_create()
*/
int tts_synthesis_parameter_destroy(tts_synthesis_parameter_h parameter);

/**
 * @brief Sets the language.
 * @since_tizen 9.0
 * @param[in] parameter The TTS synthesis parameter handle
 * @param[in] language The language selected from the tts_foreach_supported_voices() (e.g. 'NULL'(Automatic), 'en_US')
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @retval #TTS_ERROR_OUT_OF_MEMORY Out of memory
*/
int tts_synthesis_parameter_set_language(tts_synthesis_parameter_h parameter, const char *language);

/**
 * @brief Sets the voice type.
 * @since_tizen 9.0
 * @param[in] parameter The TTS synthesis parameter handle
 * @param[in] voice_type The voice type selected from the tts_foreach_supported_voices() (e.g. #TTS_VOICE_TYPE_AUTO, #TTS_VOICE_TYPE_FEMALE)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
*/
int tts_synthesis_parameter_set_voice_type(tts_synthesis_parameter_h parameter, int voice_type);

/**
 * @brief Sets the ID for personal voice data.
 * @since_tizen 9.0
 * @param[in] parameter The TTS synthesis parameter handle
 * @param[in] ptts_id The id for personal voice data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_OUT_OF_MEMORY Out of memory
*/
int tts_synthesis_parameter_set_personal_voice(tts_synthesis_parameter_h parameter, const char *ptts_id);

/**
 * @brief Sets the speed.
 * @since_tizen 9.0
 * @param[in] parameter The TTS synthesis parameter handle
 * @param[in] speed A speaking speed (e.g. #TTS_SPEED_AUTO or the value from tts_get_speed_range())
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @see tts_get_speed_range()
*/
int tts_synthesis_parameter_set_speed(tts_synthesis_parameter_h parameter, int speed);

/**
 * @brief Sets the pitch.
 * @since_tizen 9.0
 * @param[in] parameter The TTS synthesis parameter handle
 * @param[in] pitch A speaking pitch (e.g. #TTS_PITCH_AUTO or the value from tts_get_pitch_range())
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @see tts_get_pitch_range()
*/
int tts_synthesis_parameter_set_pitch(tts_synthesis_parameter_h parameter, int pitch);

/**
 * @brief Sets the volume.
 * @since_tizen 9.0
 * @param[in] parameter The TTS synthesis parameter handle
 * @param[in] volume A speaking volume ratio (e.g. #TTS_VOLUME_AUTO or the value from tts_get_volume_range())
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @see tts_get_volume_range()
*/
int tts_synthesis_parameter_set_volume(tts_synthesis_parameter_h parameter, double volume);

/**
 * @brief Sets the background volume ratio.
 * @since_tizen 9.0
 * @param[in] parameter The TTS synthesis parameter handle
 * @param[in] background_volume_ratio A background volume ratio ratio (Range 0.0 ~ 1.0)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
*/
int tts_synthesis_parameter_set_background_volume_ratio(tts_synthesis_parameter_h parameter, double background_volume_ratio);

/**
 * @brief Gets the pitch range.
 * @since_tizen 9.0
 * @param[in] tts The TTS handle
 * @param[out] min The minimum pitch value
 * @param[out] normal The normal pitch value
 * @param[out] max The maximum pitch value
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @see tts_synthesis_parameter_set_pitch()
*/
int tts_get_pitch_range(tts_h tts, int* min, int* normal, int* max);

/**
 * @brief Gets the volume range.
 * @since_tizen 9.0
 * @param[in] tts The TTS handle
 * @param[out] min The minimum volume value
 * @param[out] max The maximum volume value
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @see tts_synthesis_parameter_set_volume()
*/
int tts_get_volume_range(tts_h tts, int* min, int* max);

/**
 * @brief Adds a text to the queue with synthesis parameter.
 * @since_tizen 9.0
 * @remarks Locale(e.g. setlocale()) MUST be set for utf8 text validation check.
 * @param[in] tts The TTS handle
 * @param[in] text An input text based utf8
 * @param[in] parameter The TTS synthesis parameter handle
 * @param[out] utt_id The utterance ID passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_INVALID_VOICE Invalid voice about language, voice type
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @retval #TTS_ERROR_SCREEN_READER_OFF Screen reader is turned off
 * @pre The state should be #TTS_STATE_READY, #TTS_STATE_PLAYING or #TTS_STATE_PAUSED.
 * @see tts_get_max_text_size()
 * @see tts_set_credential()
*/
int tts_add_text_with_synthesis_parameter(tts_h tts, const char* text, tts_synthesis_parameter_h parameter, int* utt_id);

#ifdef __cplusplus
}
#endif


/**
 * @}
 */


#endif	/* __TTS_H__ */
