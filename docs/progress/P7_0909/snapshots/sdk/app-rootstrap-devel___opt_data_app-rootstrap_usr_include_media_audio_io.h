/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MEDIA_AUDIO_IO_H__
#define __TIZEN_MEDIA_AUDIO_IO_H__

#include <sys/types.h>
#include <tizen.h>
#include <sound_manager.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file audio_io.h
 * @brief This file contains the Audio Input and Audio Output API.
 */

/**
 * @addtogroup CAPI_MEDIA_AUDIO_IN_MODULE
 * @{
 */

/**
 * @brief The audio input handle.
 * @since_tizen 2.3
 */
typedef struct audio_io_s *audio_in_h;

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_AUDIO_OUT_MODULE
 * @{
 */

/**
 * @brief The audio output handle.
 * @since_tizen 2.3
 */
typedef struct audio_io_s *audio_out_h;

 /**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_AUDIO_IO_MODULE
 * @{
 */

/**
 * @brief Enumeration for audio sample type with bit depth.
 * @since_tizen 2.3
 */
typedef enum {
	AUDIO_SAMPLE_TYPE_U8 = 0x70,    /**< Unsigned 8-bit audio samples */
	AUDIO_SAMPLE_TYPE_S16_LE,       /**< Signed 16-bit audio samples */
	AUDIO_SAMPLE_TYPE_S24_LE,       /**< Signed 24-bit audio samples (Since 5.0) */
	AUDIO_SAMPLE_TYPE_S24_32_LE,    /**< Signed 24-bit (packed in 32-bit) audio samples (Since 5.0) */
	AUDIO_SAMPLE_TYPE_S32_LE,       /**< Signed 32-bit audio samples (Since 5.5) */
	AUDIO_SAMPLE_TYPE_FLOAT32_LE,	/**< Float 32-bit audio samples (Since 9.0) */
} audio_sample_type_e;

/**
 * @brief Enumeration for audio channel.
 * @since_tizen 2.3
 */
typedef enum {
	AUDIO_CHANNEL_MONO = 0x80,         /**< 1 channel, mono */
	AUDIO_CHANNEL_STEREO,              /**< 2 channels, stereo */
	AUDIO_CHANNEL_MULTI_3,             /**< 3 channels (Since 5.5) */
	AUDIO_CHANNEL_MULTI_4,             /**< 4 channels (Since 5.5) */
	AUDIO_CHANNEL_MULTI_5,             /**< 5 channels (Since 5.5) */
	AUDIO_CHANNEL_MULTI_6,             /**< 6 channels (Since 5.5) */
	AUDIO_CHANNEL_MULTI_7,             /**< 7 channels (Since 5.5) */
	AUDIO_CHANNEL_MULTI_8,             /**< 8 channels (Since 5.5) */
	AUDIO_CHANNEL_MULTI_9,             /**< 9 channels (Since 6.5) */
	AUDIO_CHANNEL_MULTI_10,            /**< 10 channels (Since 6.5) */
	AUDIO_CHANNEL_MULTI_11,            /**< 11 channels (Since 6.5) */
	AUDIO_CHANNEL_MULTI_12,            /**< 12 channels (Since 6.5) */
	AUDIO_CHANNEL_MULTI_13,            /**< 13 channels (Since 6.5) */
	AUDIO_CHANNEL_MULTI_14,            /**< 14 channels (Since 6.5) */
	AUDIO_CHANNEL_MULTI_15,            /**< 15 channels (Since 6.5) */
	AUDIO_CHANNEL_MULTI_16             /**< 16 channels (Since 6.5) */
} audio_channel_e;

/**
 * @brief Enumeration for audio input and output state.
 * @since_tizen 3.0
 */
typedef enum {
	AUDIO_IO_STATE_IDLE,      /**< Audio-io handle is created, but not prepared */
	AUDIO_IO_STATE_RUNNING,   /**< Audio-io handle is ready and the stream is running */
	AUDIO_IO_STATE_PAUSED,    /**< Audio-io handle is ready and the stream is paused */
} audio_io_state_e;

/**
 * @brief Enumeration for audio input and output error.
 * @since_tizen 2.3
 */
typedef enum {
	AUDIO_IO_ERROR_NONE                = TIZEN_ERROR_NONE,              /**< Successful */
	AUDIO_IO_ERROR_OUT_OF_MEMORY       = TIZEN_ERROR_OUT_OF_MEMORY,     /**< Out of memory */
	AUDIO_IO_ERROR_INVALID_PARAMETER   = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	AUDIO_IO_ERROR_INVALID_OPERATION   = TIZEN_ERROR_INVALID_OPERATION, /**< Invalid operation */
	AUDIO_IO_ERROR_PERMISSION_DENIED   = TIZEN_ERROR_PERMISSION_DENIED, /**< Device open error by security */
	AUDIO_IO_ERROR_NOT_SUPPORTED       = TIZEN_ERROR_NOT_SUPPORTED,     /**< Not supported */
	AUDIO_IO_ERROR_DEVICE_POLICY_RESTRICTION = TIZEN_ERROR_DEVICE_POLICY_RESTRICTION, /**< Device policy restriction (Since 3.0) */
	AUDIO_IO_ERROR_DEVICE_NOT_OPENED   = TIZEN_ERROR_AUDIO_IO | 0x01,   /**< Device open error */
	AUDIO_IO_ERROR_DEVICE_NOT_CLOSED   = TIZEN_ERROR_AUDIO_IO | 0x02,   /**< Device close error */
	AUDIO_IO_ERROR_INVALID_BUFFER      = TIZEN_ERROR_AUDIO_IO | 0x03,   /**< Invalid buffer pointer */
	AUDIO_IO_ERROR_SOUND_POLICY        = TIZEN_ERROR_AUDIO_IO | 0x04,   /**< Sound policy error */
	AUDIO_IO_ERROR_INVALID_STATE       = TIZEN_ERROR_AUDIO_IO | 0x05,   /**< Invalid state (Since 3.0) */
	AUDIO_IO_ERROR_NOT_SUPPORTED_TYPE  = TIZEN_ERROR_AUDIO_IO | 0x06,   /**< Not supported stream type (Since 3.0) */
} audio_io_error_e;

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_AUDIO_IN_MODULE
 * @{
 */

/**
 * @brief Called when audio input data is available in asynchronous (event) mode.
 *
 * @since_tizen 2.3
 *
 * @remarks Use audio_in_peek() to get 'audio in' data inside callback, use audio_in_drop() after use of peeked data.
 *
 * @param[in] handle The handle to the audio input
 * @param[in] nbytes The amount of available 'audio in' data which can be peeked.
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see audio_in_set_stream_cb()
 */
typedef void (*audio_in_stream_cb)(audio_in_h handle, size_t nbytes, void *user_data);

/**
 * @brief Called when the state of audio input is changed.
 *
 * @since_tizen 3.0
 *
 * @param[in] handle The handle of the audio input
 * @param[in] previous The previous state of the audio input
 * @param[in] current The current state of the audio input
 * @param[in] by_policy @c true if the state is changed by policy, otherwise @c false if the state is not changed by policy
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see audio_in_set_state_changed_cb()
 * @see audio_in_unset_state_changed_cb()
 */
typedef void (*audio_in_state_changed_cb)(audio_in_h handle, audio_io_state_e previous, audio_io_state_e current, bool by_policy, void *user_data);

/**
 * @brief Creates an audio device instance and returns an input handle to record PCM (pulse-code modulation) data.
 *
 * @details This function is used for audio input initialization.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @remarks @a input must be released using audio_in_destroy().
 *          If the channel count of the requested @a channel is different from the system's supported channel count, then channel remapping will be processed internally.
 *
 * @param[in] sample_rate The audio sample rate \n
 *                        Before 5.0: 8000[Hz] ~ 48000[Hz] \n
 *                         Since 5.0: 8000[Hz] ~ 192000[Hz]
 * @param[in] channel The audio channel type \n
 *                    Before 5.5: Mono or stereo \n
 *                     Since 5.5: Mono, stereo or multi-channels
 * @param[in] type The type of audio sample \n
 *                 Before 5.0: 8 or 16-bit \n
 *                  Since 5.0: 8, 16 or 24-bit \n
 *                  Since 5.5: 8, 16, 24 or 32-bit
 * @param[out] input An audio input handle is created on success
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_PERMISSION_DENIED Permission denied
 * @retval #AUDIO_IO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #AUDIO_IO_ERROR_DEVICE_NOT_OPENED Device not opened
 * @retval #AUDIO_IO_ERROR_SOUND_POLICY Sound policy error
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 *
 * @post The state will be #AUDIO_IO_STATE_IDLE.\n
 *       audio_in_set_sound_stream_info() is recommended to be called after this API.
 * @see audio_in_destroy()
 */
int audio_in_create(int sample_rate, audio_channel_e channel, audio_sample_type_e type, audio_in_h *input);

/**
 * @brief Releases the audio input handle and all its resources associated with an audio stream.
 *
 * @since_tizen 2.3
 *
 * @param[in] input The handle to the audio input to destroy
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_DEVICE_NOT_CLOSED Device not closed
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 *
 * @see audio_in_create()
 */
int audio_in_destroy(audio_in_h input);

/**
 * @brief Sets the sound stream information to the audio input.
 *
 * @since_tizen 3.0
 *
 * @remarks The sound stream information includes audio routing and volume type.
 *          For more details, you can refer to @ref CAPI_MEDIA_SOUND_MANAGER_MODULE
 *          System, Alarm, Notification, Emergency, Voice Information, Ringtone VOIP and Ringtone Call stream types are not supported in this API.
 *
 * @param[in] input The handle to the audio input
 * @param[in] stream_info The handle of stream information
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED_TYPE Not supported stream type
 *
 * @pre The state should be #AUDIO_IO_STATE_IDLE.\n
 *      Call audio_in_create() before calling this function.
 * @post Call audio_in_prepare() after calling this function.
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 */
int audio_in_set_sound_stream_info(audio_in_h input, sound_stream_info_h stream_info);

/**
 * @brief Prepares the audio input for reading audio data by starting buffering of audio data from the device.
 *
 * @since_tizen 2.3
 *
 * @param[in] input The handle to the audio input
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 * @retval #AUDIO_IO_ERROR_DEVICE_POLICY_RESTRICTION Device policy restriction
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 *
 * @post The state will be #AUDIO_IO_STATE_RUNNING.
 * @see audio_in_unprepare()
 */
int audio_in_prepare(audio_in_h input);

/**
 * @brief Unprepares the audio input.
 *
 * @since_tizen 2.3
 *
 * @param[in] input The handle to the audio input
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 *
 * @post The state will be #AUDIO_IO_STATE_IDLE.
 * @see audio_in_prepare()
 */
int audio_in_unprepare(audio_in_h input);

/**
 * @brief Pauses buffering of audio data from the device.
 *
 * @since_tizen 3.0
 *
 * @param[in] input The handle to the audio input
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #AUDIO_IO_STATE_RUNNING.
 * @post The state will be #AUDIO_IO_STATE_PAUSED.
 * @see audio_in_resume()
 */
int audio_in_pause(audio_in_h input);

/**
 * @brief Resumes buffering audio data from the device.
 *
 * @since_tizen 3.0
 *
 * @param[in] input The handle to the audio input
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #AUDIO_IO_STATE_PAUSED.
 * @post The state will be #AUDIO_IO_STATE_RUNNING.
 * @see audio_in_pause()
 */
int audio_in_resume(audio_in_h input);

/**
 * @brief Flushes and discards buffered audio data from the input stream.
 *
 * @since_tizen 2.4
 *
 * @param[in] input The handle to the audio input
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #AUDIO_IO_STATE_RUNNING or #AUDIO_IO_STATE_PAUSED.
 */
int audio_in_flush(audio_in_h input);

/**
 * @brief Reads audio data from the audio input buffer.
 *
 * @since_tizen 2.3
 *
 * @param[in] input The handle to the audio input
 * @param[out] buffer The PCM buffer address
 * @param[in] length The length of the PCM data buffer (in bytes)
 * @return The number of read bytes on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_BUFFER Invalid buffer pointer
 * @retval #AUDIO_IO_ERROR_SOUND_POLICY Sound policy error
 * @retval #AUDIO_IO_ERROR_INVALID_OPERATION Invalid operation
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre The state should be #AUDIO_IO_STATE_RUNNING.
 */
int audio_in_read(audio_in_h input, void *buffer, unsigned int length);

/**
 * @brief Gets the size to be allocated for the audio input buffer.
 *
 * @since_tizen 2.3
 *
 * @param[in] input The handle to the audio input
 * @param[out] size The buffer size (in bytes, the maximum size is 1 MB)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 * @see audio_in_read()
 */
int audio_in_get_buffer_size(audio_in_h input, int *size);

/**
 * @brief Gets the sample rate of the audio input data stream.
 *
 * @since_tizen 2.3
 *
 * @param[in] input The handle to the audio input
 * @param[out] sample_rate The audio sample rate \n
 *                         Before 5.0: 8000[Hz] ~ 48000[Hz] \n
 *                          Since 5.0: 8000[Hz] ~ 192000[Hz]
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 */
int audio_in_get_sample_rate(audio_in_h input, int *sample_rate);

/**
 * @brief Gets the channel type of the audio input data stream.
 *
 * @since_tizen 2.3
 *
 * @param[in] input The handle to the audio input
 * @param[out] channel The audio channel type \n
 *                     Before 5.5: Mono or stereo \n
 *                      Since 5.5: Mono, stereo or multi-channels
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 */
int audio_in_get_channel(audio_in_h input, audio_channel_e *channel);

/**
 * @brief Gets the sample audio format of the audio input data stream.
 *
 * @since_tizen 2.3
 *
 * @param[in] input The handle to the audio input
 * @param[out] type The type of audio sample \n
 *                  Before 5.0: 8 or 16-bit \n
 *                   Since 5.0: 8, 16 or 24-bit \n
 *                   Since 5.5: 8, 16, 24 or 32-bit
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 */
int audio_in_get_sample_type(audio_in_h input, audio_sample_type_e *type);

/**
 * @brief Sets an asynchronous (event) callback function to handle recording PCM (pulse-code modulation) data.
 *
 * @details @a callback will be called when you can read a PCM data.
 * It might cause dead lock if change the state of audio handle in callback.
 * (ex: audio_in_destroy(), audio_in_prepare(), audio_in_unprepare())
 * Recommend to use as a VOIP only.
 * Recommend not to hold callback too long.(it affects latency)
 *
 * @since_tizen 2.3
 *
 * @remarks @a input must be created using audio_in_create().
 *
 * @param[in] input    An audio input handle
 * @param[in] callback notify stream callback when user can read data (#audio_in_stream_cb)
 * @param[in] user_data user data to be retrieved when callback is called
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #AUDIO_IO_ERROR_DEVICE_NOT_OPENED Device not opened
 * @retval #AUDIO_IO_ERROR_SOUND_POLICY Sound policy error
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 *
 * @see audio_in_unset_stream_cb()
 */
int audio_in_set_stream_cb(audio_in_h input, audio_in_stream_cb callback, void* user_data);

/**
 * @brief Unregisters the callback function.
 *
 * @since_tizen 2.3
 *
 * @param[in] input The handle to the audio input
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_OPERATION Invalid operation
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 *
 * @see audio_in_set_stream_cb()
 */
int audio_in_unset_stream_cb(audio_in_h input);

/**
 * @brief Peeks into the 'audio in' buffer.
 *
 * @details This function works correctly only with read callback. Otherwise it won't operate as intended.
 *
 * @since_tizen 2.3
 *
 * @remarks Works only in asynchronous (event) mode.
 *          This function provides the pointer to the 'audio in' buffer. The pointed memory is owned by the platform, therefore the @a buffer should not be released by the application.
 *          When the data in the @a buffer is not needed anymore, use audio_in_drop() with the @a input for which audio_in_peek() was called.
 *
 * @param[in] input The handle to the audio input
 * @param[out] buffer start buffer pointer of peeked 'audio in' data
 * @param[out] length amount of 'audio in' data to be peeked
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_OPERATION Invalid operation
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #AUDIO_IO_STATE_RUNNING.
 * @see audio_in_drop()
 */
int audio_in_peek(audio_in_h input, const void **buffer, unsigned int *length);

/**
 * @brief Drops the 'audio in' buffer that was peeked into.
 *
 * @details This function works correctly only with read callback. Otherwise it won't operate as intended.
 *
 * @since_tizen 2.3
 *
 * @remarks Works only in asynchronous (event) mode. This will remove 'audio in' data from the actual stream buffer. Use this if peeked data is not needed anymore.
 *
 * @param[in] input The handle to the audio input
 * @return 0 on success, otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_OPERATION Invalid operation
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #AUDIO_IO_STATE_RUNNING.
 * @see audio_in_peek()
 */
int audio_in_drop(audio_in_h input);

/**
 * @brief Sets the state changed callback function to the audio input handle.
 *
 * @since_tizen 3.0
 *
 * @remarks @a input must be created using audio_in_create().
 *
 * @param[in] input    The audio input handle
 * @param[in] callback the state changed callback called when the state of the handle is changed (#audio_in_state_changed_cb)
 * @param[in] user_data user data to be retrieved when callback is called
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 *
 * @see audio_in_unset_state_changed_cb()
 */
int audio_in_set_state_changed_cb(audio_in_h input, audio_in_state_changed_cb callback, void* user_data);

/**
 * @brief Unregisters the state changed callback function of the audio input handle.
 *
 * @since_tizen 3.0
 *
 * @param[in] input The handle to the audio input
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 *
 * @see audio_in_set_state_changed_cb()
 */
int audio_in_unset_state_changed_cb(audio_in_h input);

/**
 * @brief Gets the volume of the audio input data stream.
 *
 * @since_tizen 6.0
 *
 * @remarks The default @a volume of the audio input stream is 1.0.
 *
 * @param[in] input The handle to the audio input
 * @param[out] volume The current volume value of the audio input stream
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 *
 * @see audio_in_set_volume()
 */
int audio_in_get_volume(audio_in_h input, double *volume);

/**
 * @brief Sets the volume of the audio input data stream.
 *
 * @since_tizen 6.0
 *
 * @remarks The default @a volume of the audio input stream is 1.0.
 *          If the @a volume is less than 1.0, the loudness of recorded data will be decreased.
 *          If the @a volume is greater than 1.0, the loudness of recorded data will be increased,
 *          which can be useful when the loudness of original recorded data is too low in certain environments.
 *          Note that the volume can be clipped if the @a volume is greater than 1.0 and the loudness of original recorded data is high enough.
 *
 * @param[in] input The handle to the audio input
 * @param[in] volume The volume value to be set (0.0 <= volume <= 2.0)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 *
 * @see audio_in_get_volume()
 */
int audio_in_set_volume(audio_in_h input, double volume);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_AUDIO_OUT_MODULE
 * @{
 */

/**
 * @brief Called when 'audio out' data can be written in asynchronous (event) mode.
 *
 * @since_tizen 2.3
 *
 * @remarks Use audio_out_write() to write pcm data inside this callback.
 * @param[in] handle The handle to the audio output
 * @param[in] nbytes The amount of 'audio in' data which can be written.
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see audio_out_set_stream_cb()
 */
typedef void (*audio_out_stream_cb)(audio_out_h handle, size_t nbytes, void *user_data);

/**
 * @brief Called when the state of audio output is changed.
 *
 * @since_tizen 3.0
 *
 * @param[in] handle The handle of the audio output
 * @param[in] previous The previous state of the audio output
 * @param[in] current The current state of the audio output
 * @param[in] by_policy @c true if the state is changed by policy, otherwise @c false if the state is not changed by policy
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see audio_out_set_state_changed_cb()
 * @see audio_out_unset_state_changed_cb()
 */
typedef void (*audio_out_state_changed_cb)(audio_out_h handle, audio_io_state_e previous, audio_io_state_e current, bool by_policy, void *user_data);

/**
 * @brief Creates an audio device instance and returns an output handle to play PCM (pulse-code modulation) data.
 *
 * @details This function is used for audio output initialization.
 *
 * @since_tizen 3.0
 *
 * @remarks @a output must be released by audio_out_destroy().
 *          It is recommended to call audio_out_set_sound_stream_info() after this API.
 *          Multi-channel playback is not supported.
 *
 * @param[in] sample_rate The audio sample rate \n
 *                        Before 5.0: 8000[Hz] ~ 48000[Hz] \n
 *                         Since 5.0: 8000[Hz] ~ 192000[Hz]
 * @param[in] channel The audio channel type (mono or stereo)
 * @param[in] type The type of audio sample \n
 *                 Before 5.0: 8 or 16-bit \n
 *                  Since 5.0: 8, 16 or 24-bit \n
 *                  Since 5.5: 8, 16, 24 or 32-bit
 * @param[out] output An audio output handle is created on success
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #AUDIO_IO_ERROR_DEVICE_NOT_OPENED Device not opened
 * @retval #AUDIO_IO_ERROR_SOUND_POLICY Sound policy error
 *
 * @post The state will be #AUDIO_IO_STATE_IDLE.\n
 *       audio_out_set_sound_stream_info() is recommended to be called after this API.
 * @see audio_out_destroy()
 */
int audio_out_create_new(int sample_rate, audio_channel_e channel, audio_sample_type_e type, audio_out_h *output);

/**
 * @brief Releases the audio output handle, along with all its resources.
 *
 * @since_tizen 2.3
 *
 * @param[in] output The handle to the audio output to destroy
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #AUDIO_IO_ERROR_DEVICE_NOT_CLOSED Device not closed
 *
 * @see audio_out_create_new()
 */
int audio_out_destroy(audio_out_h output);

/**
 * @brief Sets the sound stream information to the audio output.
 *
 * @since_tizen 3.0
 *
 * @remarks The sound stream information includes audio routing and volume type.
 *          For more details, you can refer to @ref CAPI_MEDIA_SOUND_MANAGER_MODULE
 *          Voice Recognition and Loopback stream types are not supported in this API.
 *
 * @param[in] output The handle to the audio output
 * @param[in] stream_info The handle of stream information
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED Not supported
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 * @retval #AUDIO_IO_ERROR_NOT_SUPPORTED_TYPE Not supported stream type
 *
 * @pre The state should be #AUDIO_IO_STATE_IDLE.\n
 *      Call audio_out_create_new() before calling this function.
 * @post Call audio_out_prepare() after calling this function.
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 */
int audio_out_set_sound_stream_info(audio_out_h output, sound_stream_info_h stream_info);

/**
 * @brief Prepares the audio output for playback, this must be called before audio_out_write().
 *
 * @since_tizen 2.3
 *
 * @param[in] output The handle to the audio output
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 *
 * @post The state will be #AUDIO_IO_STATE_RUNNING.
 * @see audio_out_unprepare()
 */
int audio_out_prepare(audio_out_h output);

/**
 * @brief Unprepares the audio output.
 *
 * @since_tizen 2.3
 *
 * @param[in] output The handle to the audio output
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 *
 * @post The state will be #AUDIO_IO_STATE_IDLE.
 * @see audio_out_prepare()
 */
int audio_out_unprepare(audio_out_h output);

/**
 * @brief Pauses feeding of audio data to the device.
 *
 * @since_tizen 3.0
 *
 * @param[in] output The handle to the audio output
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #AUDIO_IO_STATE_RUNNING.
 * @post The state will be #AUDIO_IO_STATE_PAUSED.
 * @see audio_out_resume()
 */
int audio_out_pause(audio_out_h output);

/**
 * @brief Resumes feeding of audio data to the device.
 *
 * @since_tizen 3.0
 *
 * @param[in] output The handle to the audio output
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #AUDIO_IO_STATE_PAUSED.
 * @post The state will be #AUDIO_IO_STATE_RUNNING.
 * @see audio_out_pause()
 */
int audio_out_resume(audio_out_h output);

/**
 * @brief Drains buffered audio data from the output stream.
 *
 * @details This function waits until drains stream buffer completely. (e.g end of playback)
 *
 * @since_tizen 2.4
 *
 * @param[in] output The handle to the audio output
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #AUDIO_IO_STATE_RUNNING or #AUDIO_IO_STATE_PAUSED.
 * @see audio_out_flush()
 */
int audio_out_drain(audio_out_h output);

/**
 * @brief Flushes and discards buffered audio data from the output stream.
 *
 * @since_tizen 2.4
 *
 * @param[in] output The handle to the audio output
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #AUDIO_IO_STATE_RUNNING or #AUDIO_IO_STATE_PAUSED.
 * @see audio_out_drain()
 */
int audio_out_flush(audio_out_h output);

/**
 * @brief Starts writing the audio data to the device.
 *
 * @since_tizen 2.3
 *
 * @param[in] output The handle to the audio output
 * @param[in,out] buffer The PCM buffer address
 * @param[in] length The length of the PCM buffer (in bytes)
 * @return The written data size on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_BUFFER Invalid buffer pointer
 * @retval #AUDIO_IO_ERROR_SOUND_POLICY Sound policy error
 * @retval #AUDIO_IO_ERROR_INVALID_STATE Invalid state
 *
 * @pre The state should be #AUDIO_IO_STATE_RUNNING.
 */
int audio_out_write(audio_out_h output, void *buffer, unsigned int length);

/**
 * @brief Gets the size to be allocated for the audio output buffer.
 *
 * @since_tizen 2.3
 *
 * @param[in] output The handle to the audio output
 * @param[out] size The suggested buffer size (in bytes, the maximum size is 1 MB)
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval  #AUDIO_IO_ERROR_NONE Successful
 * @retval  #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see audio_out_write()
 */
int audio_out_get_buffer_size(audio_out_h output, int *size);

/**
 * @brief Gets the sample rate of the audio output data stream.
 *
 * @since_tizen 2.3
 *
 * @param[in] output The handle to the audio output
 * @param[out] sample_rate The audio sample rate \n
 *                         Before 5.0: 8000[Hz] ~ 48000[Hz] \n
 *                          Since 5.0: 8000[Hz] ~ 192000[Hz]
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval  #AUDIO_IO_ERROR_NONE Successful
 * @retval  #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_out_get_sample_rate(audio_out_h output, int *sample_rate);

/**
 * @brief Gets the channel type of the audio output data stream.
 *
 * @since_tizen 2.3
 *
 * @param[in] output The handle to the audio output
 * @param[out] channel The audio channel type (mono or stereo)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_out_get_channel(audio_out_h output, audio_channel_e *channel);

/**
 * @brief Gets the sample audio format of the audio output data stream.
 *
 * @since_tizen 2.3
 *
 * @param[in] output The handle to the audio output
 * @param[out] type The type of audio sample \n
 *                  Before 5.0: 8 or 16-bit \n
 *                   Since 5.0: 8, 16 or 24-bit \n
 *                   Since 5.5: 8, 16, 24 or 32-bit
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_out_get_sample_type(audio_out_h output, audio_sample_type_e *type);

/**
 * @brief Gets the sound type supported by the audio output device.
 *
 * @since_tizen 2.3
 *
 * @param[in] output The handle to the audio output
 * @param[out] type The sound type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_out_get_sound_type(audio_out_h output, sound_type_e *type);

/**
 * @brief Sets an asynchronous (event) callback function to handle playing PCM (pulse-code modulation) data.
 *
 * @details @a callback will be called when you can write a PCM data.
 * It might cause dead lock if change the state of audio handle in callback.
 * (ex: audio_out_destroy(), audio_out_prepare(), audio_out_unprepare())
 * Recommend to use as a VOIP only.
 * Recommend not to hold callback too long.(it affects latency)
 *
 * @since_tizen 2.3
 *
 * @remarks @a output must be created using audio_out_create_new().
 *
 * @param[in] output   An audio output handle
 * @param[in] callback notify stream callback when user can write data (#audio_out_stream_cb)
 * @param[in] user_data user data to be retrieved when callback is called
 * @return 0 on success, otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #AUDIO_IO_ERROR_DEVICE_NOT_OPENED Device not opened
 * @retval #AUDIO_IO_ERROR_SOUND_POLICY Sound policy error
 *
 * @see audio_out_unset_stream_cb()
 */
int audio_out_set_stream_cb(audio_out_h output, audio_out_stream_cb callback, void* user_data);

/**
 * @brief Unregisters the callback function.
 *
 * @since_tizen 2.3
 *
 * @param[in] output The handle to the audio output
 * @return 0 on success, otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see audio_out_set_stream_cb()
 */
int audio_out_unset_stream_cb(audio_out_h output);

/**
 * @brief Sets the state changed callback function to the audio output handle.
 *
 * @since_tizen 3.0
 *
 * @remarks @a input must be created using audio_out_create_new().
 *
 * @param[in] output    The audio output handle
 * @param[in] callback the state changed callback called when the state of the handle is changed (#audio_out_state_changed_cb)
 * @param[in] user_data user data to be retrieved when callback is called
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see audio_out_unset_state_changed_cb()
 */
int audio_out_set_state_changed_cb(audio_out_h output, audio_out_state_changed_cb callback, void* user_data);

/**
 * @brief Unregisters the state changed callback function of the audio output handle.
 *
 * @since_tizen 3.0
 *
 * @param[in] output The handle to the audio output
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see audio_out_set_state_changed_cb()
 */
int audio_out_unset_state_changed_cb(audio_out_h output);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_AUDIO_IO_H__ */
