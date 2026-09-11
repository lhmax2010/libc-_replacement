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


#ifndef __TTS_INTERNAL_H__
#define __TTS_INTERNAL_H__

#include <tizen.h>
#include <tts.h>


#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Definition for TTS interrupt mode
 */
#define TTS_MODE_INTERRUPT		3


/**
 * @brief Sets server tts.
 * @details Using this API, the application can ask server tts with a credential.
 *  The credential is a key to verify the authorization about using the engine based on server, not embedded engine.
 *  If the application sets the credential, it will be able to use functions of the server engine entirely.
 * @since_tizen 3.0
 *
 * @remarks The necessity of the credential depends on the engine. In case of the engine which is basically embedded in Tizen, the credential is not necessary so far.
 *  However, if the user wants to apply the 3rd party's engine, the credential may be necessary. In that case, please follow the policy provided by the corresponding engine.
 *
 * @param[in] tts The TTS handle
 * @param[in] credential The credential
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #TTS_ERROR_NONE Success
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_OUT_OF_MEMORY TTS Out of memory
 *
 * @pre The state should be #TTS_STATE_CREATED or #TTS_STATE_READY.
 *
 * @see tts_play()
*/
int tts_set_server_tts(tts_h tts, const char* credential);

/**
 * @brief Adds a sound stream to the queue.
 * @since_tizen 4.0
 *
 * @param[in] tts The TTS handle
 * @param[in] event An event about pcm buffer (-1: error, 1: start, 2: continue, 3: finish)
 * @param[in] data The pcm data buffer
 * @param[in] data_size The data size of pcm data buffer
 * @param[in] audio_type The audio type of pcm (0: signed 16-bit, 1: unsigned 8-bit)
 * @param[in] rate The sampling rate of pcm
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_PERMISSION_DENIED Permission denied
 * @pre The state should be #TTS_STATE_READY, #TTS_STATE_PLAYING, or #TTS_STATE_PAUSED.
 * @see tts_play_pcm()
 * @see tts_stop_pcm()
*/
int tts_add_pcm(tts_h tts, int event, const void* data, unsigned int data_size, int audio_type, int rate);

/**
 * @brief Request to play pcm.
 * @since_tizen 4.0
 *
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_PERMISSION_DENIED Permission denied
 * @pre The state should be #TTS_STATE_READY or #TTS_STATE_PAUSED.
 * @see tts_add_pcm()
 * @see tts_stop_pcm()
*/
int tts_play_pcm(tts_h tts);

/**
 * @brief Request to stop pcm.
 * @since_tizen 4.0
 *
 * @param[in] tts The TTS handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_INVALID_STATE Invalid state
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_PERMISSION_DENIED Permission denied
 * @pre The state should be #TTS_STATE_READY, #TTS_STATE_PLAYING, or #TTS_STATE_PAUSED.
 * @see tts_play_pcm()
 * @see tts_add_pcm()
*/
int tts_stop_pcm(tts_h tts);


/**
 * @brief Sets the engine_type.
 * @since_tizen 9.0
 * @param[in] parameter The TTS synthesis parameter handle
 * @param[in] engine_type The TTS engine's type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TTS_ERROR_NONE Successful
 * @retval #TTS_ERROR_NOT_SUPPORTED TTS NOT supported
 * @retval #TTS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TTS_ERROR_OPERATION_FAILED Operation failure
 * @retval #TTS_ERROR_OUT_OF_MEMORY Out of memory
*/
int tts_synthesis_parameter_set_engine_type(tts_synthesis_parameter_h parameter, const char *engine_type);


#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __TTS_INTERNAL_H__ */
