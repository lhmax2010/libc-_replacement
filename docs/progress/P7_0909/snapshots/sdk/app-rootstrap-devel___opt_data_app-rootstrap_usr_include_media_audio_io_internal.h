/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MEDIA_AUDIO_IO_INTERNAL_H__
#define __TIZEN_MEDIA_AUDIO_IO_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file audio_io_internal.h
 * @brief This file contains audio-io internal APIs
 */

/**
 * @addtogroup CAPI_MEDIA_AUDIO_IN_INTERNAL_MODULE
 * @{
 */

/**
 * @internal
 * @brief Enumeration for audio effect
 * @since_tizen 9.0
 */
typedef enum {
    AUDIO_IO_EFFECT_METHOD_ACOUSTIC_ECHO_CANCELLATION = 0x1,
    AUDIO_IO_EFFECT_METHOD_NOISE_SUPPRESSION = 0x2,
    AUDIO_IO_EFFECT_METHOD_AUTO_GAIN_CONTROL = 0x4,
} audio_io_effect_method_e;

/**
 * @internal
 * @brief audio effect method handle;
 * @since_tizen 9.0
 */
typedef struct audio_io_effect_method_s *audio_io_effect_method_h;

/**
 * @internal
 * @brief audio effect method chain handle
 * @since_tizen 9.0
 */
typedef struct audio_io_effect_method_chain_s *audio_io_effect_method_chain_h;

/**
 * @internal
 * @brief audio effect method chain builder handle
 * @since_tizen 9.0
 */
typedef struct audio_io_effect_method_chain_builder_s *audio_io_effect_method_chain_builder_h;

/**
 * @internal
 * @brief Creates an acoustic echo canceller effect method.
 * @since_tizen 9.0
 *
 * @param[in] rate The sample rate for the audio processing
 * @param[in] channels The number of audio channels to use
 * @param[in] sample_type The audio sample type to be used
 * @param[in] framesize The size of the audio frames
 * @param[out] audio_io_effect The handle to the created effect method
 * @param[out] adjust_framesize The adjusted frame size for the effect
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_OPERATION Invalid operation
 */
int audio_io_create_acoustic_echo_canceller(int rate, audio_channel_e channels,
                                            audio_sample_type_e sample_type, size_t framesize,
                                            audio_io_effect_method_h *audio_io_effect, size_t *adjust_framesize);

/**
 * @internal
 * @brief Creates a noise suppressor effect method.
 * @since_tizen 9.0
 *
 * @param[in] rate The sample rate for the audio processing
 * @param[in] channels The number of audio channels to use
 * @param[in] sample_type The audio sample type to be used
 * @param[in] framesize The size of the audio frames
 * @param[out] audio_io_effect The handle to the created effect method
 * @param[out] adjust_framesize The adjusted frame size for the effect
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_OPERATION Invalid operation
 */
int audio_io_create_noise_suppressor(int rate, audio_channel_e channels,
                                            audio_sample_type_e sample_type, size_t framesize,
                                            audio_io_effect_method_h *audio_io_effect, size_t *adjust_framesize);

/**
 * @internal
 * @brief Creates an auto gain control effect method.
 * @since_tizen 9.0
 *
 * @param[in] rate The sample rate for the audio processing
 * @param[in] channels The number of audio channels to use
 * @param[in] sample_type The audio sample type to be used
 * @param[in] framesize The size of the audio frames
 * @param[out] audio_io_effect The handle to the created effect method * @param[out] adjust_framesize The adjusted frame size for the effect
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_OPERATION Invalid operation
 */
int audio_io_create_auto_gain_control(int rate, audio_channel_e channels,
                                            audio_sample_type_e sample_type, size_t framesize,
                                            audio_io_effect_method_h *audio_io_effect, size_t *adjust_framesize);

/**
 * @internal
 * @brief Destroys an effect method.
 * @since_tizen 9.0
 *
 * @param[in] audio_io_effect The handle to the effect method to be destroyed
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_OPERATION Invalid operation
 * @see audio_io_create_acoustic_echo_canceller()
 * @see audio_io_create_noise_suppressor()
 * @see audio_io_create_auto_gain_control()
 */
int audio_io_destroy_effect_method(audio_io_effect_method_h audio_io_effect);

/**
 * @internal
 * @brief Applies the effect method to the input audio data.
 * @since_tizen 9.0
 *
 * @param[in] audio_io_effect The handle to the effect method to apply
 * @param[in] in Input audio data buffer
 * @param[out] out Output audio data buffer (processed)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_OPERATION Invalid operation
 * @see audio_io_create_acoustic_echo_canceller()
 * @see audio_io_create_noise_suppressor()
 * @see audio_io_create_auto_gain_control()
 */
int audio_io_apply_effect_method(audio_io_effect_method_h audio_io_effect, const char *in, char *out);

/**
 * @internal
 * @brief Applies the effect method to the input audio data with an additional reference.
 * @since_tizen 9.0
 *
 * @param[in] audio_io_effect The handle to the effect method to apply
 * @param[in] in Input audio data buffer
 * @param[in] ref Reference audio data buffer
 * @param[out] out Output audio data buffer (processed)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_OPERATION Invalid operation
 * @see audio_io_create_acoustic_echo_canceller()
 * @see audio_io_create_noise_suppressor()
 * @see audio_io_create_auto_gain_control()
 */
int audio_io_apply_effect_method_with_reference(audio_io_effect_method_h audio_io_effect, const char *in, const char *ref, char *out);

/**
 * @internal
 * @brief Creates a builder for an effect method chain.
 * @since_tizen 9.0
 *
 * @param[in] rate The sample rate for the audio processing
 * @param[in] channels The number of audio channels to use
 * @param[in] sample_type The audio sample type to be used
 * @param[in] request_framesize The requested size of frames
 * @param[out] builder The handle to the created effect method chain builder
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_OUT_OF_MEMORY Out of memory
 */
int audio_io_create_effect_method_chain_builder(int rate, audio_channel_e channels,
                                            audio_sample_type_e sample_type, size_t request_framesize,
                                            audio_io_effect_method_chain_builder_h *builder);

/**
 * @internal
 * @brief Appends an effect method to the chain builder.
 * @since_tizen 9.0
 *
 * @param[in] builder The chain builder to which the effect method will be added
 * @param[in] aio_method The effect method to append
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_OPERATION Invalid operation
 * @see audio_io_create_effect_method_chain_builder()
 */
int audio_io_append_effect_method_chain_builder(audio_io_effect_method_chain_builder_h builder, audio_io_effect_method_e aio_method);

/**
 * @internal
 * @brief Builds the effect method chain from the builder.
 * @since_tizen 9.0
 *
 * @param[in] builder The chain builder to be used
 * @param[out] chain The handle to the created effect method chain
 * @param[out] adjust_framesize The adjusted frame size for the effect chain
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_OUT_OF_MEMORY Out of memory
 * @see audio_io_create_effect_method_chain_builder()
 * @see audio_io_append_effect_method_chain_builder()
 */
int audio_io_build_effect_method_chain(audio_io_effect_method_chain_builder_h builder, audio_io_effect_method_chain_h *chain, size_t *adjust_framesize);

/**
 * @internal
 * @brief Applies an effect method chain to the input audio data.
 * @since_tizen 9.0
 *
 * @param[in] chain The effect method chain to apply
 * @param[in] in Input audio data buffer
 * @param[in] ref Reference audio data buffer (optional)
 * @param[out] out Output audio data buffer (processed)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUDIO_IO_ERROR_INVALID_OPERATION Invalid operation
 * @see audio_io_create_effect_method_chain_builder()
 * @see audio_io_append_effect_method_chain_builder()
 * @see audio_io_build_effect_method_chain()
 */
int audio_io_apply_effect_method_chain(audio_io_effect_method_chain_h chain, const char *in, const char *ref, char *out);

/**
 * @internal
 * @brief Destroys the effect method chain builder.
 * @since_tizen 9.0
 *
 * @param[in] builder The handle to the effect method chain builder to be destroyed
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @see audio_io_create_effect_method_chain_builder()
 */
int audio_io_destroy_effect_method_chain_builder(audio_io_effect_method_chain_builder_h builder);

/**
 * @internal
 * @brief Destroys the effect method chain.
 * @since_tizen 9.0
 *
 * @param[in] chain The handle to the effect method chain to be destroyed
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #AUDIO_IO_ERROR_NONE Successful
 * @retval #AUDIO_IO_ERROR_INVALID_PARAMETER Invalid parameter
 * @see audio_io_create_effect_method_chain_builder()
 * @see audio_io_build_effect_method_chain()
 */
int audio_io_destroy_effect_method_chain(audio_io_effect_method_chain_h chain);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // __TIZEN_MEDIA_AUDIO_IO_INTERNAL_H__

