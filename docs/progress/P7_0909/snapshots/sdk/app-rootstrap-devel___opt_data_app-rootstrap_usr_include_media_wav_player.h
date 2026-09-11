/*
* Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/




#ifndef __TIZEN_MEDIA_WAV_PLAYER_H__
#define __TIZEN_MEDIA_WAV_PLAYER_H__

#include <tizen.h>
#include <sound_manager.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define WAV_PLAYER_ERROR_CLASS          TIZEN_ERROR_MULTIMEDIA_CLASS | 0x50

/**
 * @file wav_player.h
 * @brief This file contains the WAV player API
 */

/**
 * @addtogroup CAPI_MEDIA_WAV_PLAYER_MODULE
 * @{
 */

/**
 * @brief Enumeration of error codes for WAV player.
 * @since_tizen 2.3
 */
typedef enum {
	WAV_PLAYER_ERROR_NONE                 = TIZEN_ERROR_NONE,                  /**< Successful */
	WAV_PLAYER_ERROR_INVALID_PARAMETER    = TIZEN_ERROR_INVALID_PARAMETER,     /**< Invalid parameter */
	WAV_PLAYER_ERROR_INVALID_OPERATION    = TIZEN_ERROR_INVALID_OPERATION,     /**< Invalid operation */
	WAV_PLAYER_ERROR_FORMAT_NOT_SUPPORTED = TIZEN_ERROR_WAV_PLAYER | 0x01,     /**< Format not supported */
	WAV_PLAYER_ERROR_NOT_SUPPORTED_TYPE   = TIZEN_ERROR_WAV_PLAYER | 0x02      /**< Not supported (Since 3.0) */
} wav_player_error_e;

/**
 * @}
 */


/**
 * @addtogroup CAPI_MEDIA_WAV_PLAYER_MODULE
 * @{
 */

/**
 * @brief Called when a WAV file has finished playing.
 * @details This callback is not invoked by calling wav_player_stop().
 * @since_tizen 2.3
 * @param[in] id	The completed wav player ID
 * @param[in] user_data	The user data passed from the callback registration function
 * @see wav_player_start_new()
 */
typedef void (*wav_player_playback_completed_cb)(int id, void *user_data);

/**
 * @brief Plays a WAV file with stream information of sound-manager.
 * @since_tizen 3.0
 *
 * @remarks Voice Recognition and VOIP stream types are not supported by this function.
 *
 * @param[in] path	The file path to play
 * @param[in] stream_info	The sound stream information handle
 * @param[in] callback	The callback function to be invoked when the WAV file is no longer being played
 * @param[in] user_data	The user data to be passed to the callback function
 * @param[out] id	The WAV player ID (can be set to @c NULL)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WAV_PLAYER_ERROR_NONE Successful
 * @retval #WAV_PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WAV_PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WAV_PLAYER_ERROR_FORMAT_NOT_SUPPORTED Not supported format
 * @retval #WAV_PLAYER_ERROR_NOT_SUPPORTED_TYPE Not supported stream type
 *
 * @post	It invokes wav_player_playback_completed_cb() when the WAV file is no longer being played.
 * @see wav_player_stop()
 * @see wav_player_playback_completed_cb()
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 */
int wav_player_start_new(const char *path, sound_stream_info_h stream_info, wav_player_playback_completed_cb callback, void *user_data, int *id);

/**
 * @brief Plays a WAV file multiple times.
 * @since_tizen 6.0
 *
 * @remarks Voice Recognition and VOIP stream types are not supported by this function.
 *
 * @param[in] path	The file path to play
 * @param[in] stream_info	The sound stream information handle
 * @param[in] loop_count	The number of times the file should be played (@c 0 indicates infinite loops)
 * @param[in] callback	The callback function to be invoked when the WAV file is no longer being played
 * @param[in] user_data	The user data to be passed to the callback function
 * @param[out] id	The WAV player ID (can be set to @c NULL)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WAV_PLAYER_ERROR_NONE Successful
 * @retval #WAV_PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WAV_PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WAV_PLAYER_ERROR_FORMAT_NOT_SUPPORTED Not supported format
 * @retval #WAV_PLAYER_ERROR_NOT_SUPPORTED_TYPE Not supported stream type
 *
 * @post	It invokes wav_player_playback_completed_cb() when the WAV file is no longer being played.
 * @see wav_player_stop()
 * @see wav_player_playback_completed_cb()
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 */
int wav_player_start_loop(const char *path, sound_stream_info_h stream_info, unsigned int loop_count, wav_player_playback_completed_cb callback, void *user_data, int *id);

/**
 * @brief Stops playing the WAV file.
 * @since_tizen 2.3
 *
 * @remarks If the playback of @a id has been already finished at the server side and\n
 *	wav_player_playback_completed_cb() is not invoked yet, #WAV_PLAYER_ERROR_INVALID_OPERATION will be returned.
 *
 * @param[in] id	The WAV player ID to stop
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #WAV_PLAYER_ERROR_NONE Successful
 * @retval #WAV_PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WAV_PLAYER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see	wav_player_start_new()
 */
int wav_player_stop(int id);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_WAV_PLAYER_H__ */
