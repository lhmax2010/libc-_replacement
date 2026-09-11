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

#ifndef __STT_INTERNAL_H__
#define __STT_INTERNAL_H__

#include <tizen.h>
#include <stt.h>

/**
 * @file stt_internal.h
 */

#ifdef __cplusplus
extern "C"
{
#endif


/**
* @brief Enumerations of system volume event.
*/
typedef enum {
	STT_SYSTEM_VOLUME_EVENT_CHANGE_FOR_NEARFIELD,	/**< Change system volume event for nearfield */
	STT_SYSTEM_VOLUME_EVENT_CHANGE_FOR_FARFIELD,	/**< Change system volume event for farfield */
	STT_SYSTEM_VOLUME_EVENT_RECOVER					/**< Recover system volume event */
} stt_system_volume_event_e;

/**
 * @brief Sets server STT.
 * @details Using this API, the application can set server STT with a @a key as a @a user_data
 *	The key is a private data to set STT server.
 *	There are 3 types of keys; "server", "rampcode" and "epd".
 *		"server": STT server address
 *		"rampcode": ASR ramp code
 *		"epd": A threshold for end-point detection
 *
 *	The application can input the @a user_data corresponding to the @a key.
 *		"server": "qa", "sbx"
 *		"rampcode": "dash_dict", "dash_da"
 *		"epd": "100", "750", etc
 *
 *  If the application sets those keys, it will be able to use corresponding STT engines and options.
 *
 * @since_tizen 3.0
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @param[in] stt The STT handle
 * @param[in] key The key
 * @param[in] user_data The user data corresponding to the key
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STT_ERROR_NONE Successful
 * @retval #STT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STT_ERROR_INVALID_STATE Invalid state
 * @retval #STT_ERROR_NOT_SUPPORTED STT NOT supported
 * @retval #STT_ERROR_OUT_OF_MEMORY STT Out of memory
 * @retval #STT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre The state should be #STT_STATE_READY.
 */
int stt_set_server_stt(stt_h stt, const char* key, char* user_data);

/**
 * @brief Starts file recognition asynchronously.
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @remarks This function starts sending recorded data from file to engine.
 * 		The pcm file should be in raw format as following
 * 			- 16000 for sample rate
 * 			- 1 channel for mono
 * 			- Signed 16 bits per sample
 * @param[in] stt The STT handle
 * @param[in] language The language selected from stt_foreach_supported_languages()
 * @param[in] type The type for recognition (e.g. #STT_RECOGNITION_TYPE_FREE, #STT_RECOGNITION_TYPE_FREE_PARTIAL)
 * @param[in] filepath PCM filepath for recognition
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #STT_ERROR_NONE Successful
 * @retval #STT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STT_ERROR_INVALID_STATE Invalid state
 * @retval #STT_ERROR_OPERATION_FAILED Operation failure
 * @retval #STT_ERROR_NOT_SUPPORTED STT NOT supported
 * @retval #STT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #STT_ERROR_IN_PROGRESS_TO_RECORDING Progress to recording is not finished
 * @pre The state should be #STT_STATE_READY.
 * @post It will invoke stt_state_changed_cb(), if you register a callback with stt_state_changed_cb().
 *       If this function succeeds, the STT state will be #STT_STATE_RECORDING.
 *       If you call this function again before state changes, you will receive STT_ERROR_IN_PROGRESS_TO_RECORDING.
 * @see stt_cancel_file()
 * @see stt_state_changed_cb()
*/
int stt_start_file(stt_h stt, const char* language, const char* type, const char* filepath);

/**
 * @brief Cancels processing file recognition asynchronously.
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @remarks This function cancels recording and engine cancels recognition processing.
 *	    After successful cancel, stt_state_changed_cb() is called otherwise if error is occurred, stt_error_cb() is called.
 * @param[in] stt The STT handle
 * @return @c 0 on success,
 *	   otherwise a negative error value
 * @retval #STT_ERROR_NONE Successful
 * @retval #STT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STT_ERROR_INVALID_STATE Invalid state
 * @retval #STT_ERROR_OPERATION_FAILED Operation failure
 * @retval #STT_ERROR_NOT_SUPPORTED STT NOT supported
 * @retval #STT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #STT_ERROR_IN_PROGRESS_TO_READY Progress to ready is not finished
 * @retval #STT_ERROR_IN_PROGRESS_TO_RECORDING Progress to recording is not finished
 * @retval #STT_ERROR_IN_PROGRESS_TO_PROCESSING Progress to processing is not finished
 * @pre The state should be #STT_STATE_RECORDING or #STT_STATE_PROCESSING.
 * @post It will invoke stt_state_changed_cb(), if you register a callback with stt_state_changed_cb().
 *	 If this function succeeds, the STT state will be #STT_STATE_READY.
 *	 If you call this function again before state changes, you will receive STT_ERROR_IN_PROGRESS_TO_READY.
 * @see stt_start_file()
 * @see stt_state_changed_cb()
*/
int stt_cancel_file(stt_h stt);

/**
 * @brief Changes system volume with event.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @privilege %http://tizen.org/privilege/volume.set
 * @remarks If @a volume_event is STT_SYSTEM_VOLUME_EVENT_CHANGE_FOR_NEARFIELD, the background volume will be reduced by 70 percentage.
 *          And, if @a volume_event is STT_SYSTEM_VOLUME_EVENT_CHANGE_FOR_FARFIELD, the background volume will be mute.
 * @param[in] stt The STT handle
 * @param[in] volume_event System volume event
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #STT_ERROR_NONE Successful
 * @retval #STT_ERROR_NOT_SUPPORTED STT NOT supported
 * @retval #STT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #STT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STT_ERROR_INVALID_STATE Invalid state
 * @retval #STT_ERROR_OPERATION_FAILED STT Operation failure
 * @pre The state should be #STT_STATE_CREATED or #STT_STATE_READY.
 * @see stt_recover_system_volume()
*/
int stt_change_system_volume(stt_h stt, stt_system_volume_event_e volume_event);

/**
 * @brief Recovers system volume.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @privilege %http://tizen.org/privilege/volume.set
 * @param[in] stt The STT handle
 * @param[in] volume_event System volume event
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #STT_ERROR_NONE Successful
 * @retval #STT_ERROR_NOT_SUPPORTED STT NOT supported
 * @retval #STT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #STT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STT_ERROR_INVALID_STATE Invalid state
 * @retval #STT_ERROR_OPERATION_FAILED STT Operation failure
 * @pre The state should be #STT_STATE_CREATED or #STT_STATE_READY.
 * @see stt_recover_system_volume()
*/
int stt_recover_system_volume(stt_h stt);


/* FIXME: This is temporal restoration. It's gonna be removed after build issue resovled. */
int stt_set_audio_type(stt_h stt, const char *audio_id);
int stt_get_audio_type(stt_h stt, char **audio_id);


#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __STT_INTERNAL_H__ */

