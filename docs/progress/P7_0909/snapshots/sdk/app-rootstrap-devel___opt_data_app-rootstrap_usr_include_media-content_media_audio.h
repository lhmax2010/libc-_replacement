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


#ifndef __TIZEN_CONTENT_MEDIA_AUDIO_H__
#define __TIZEN_CONTENT_MEDIA_AUDIO_H__

#include <media_content_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_audio.h
 * @brief This file contains the audio metadata API and related functions to proceed with audio metadata. \n
 *        Description of the audio content involves: album, artist, album_artist, genre and description tags. \n
 *        Parameters of the recording are also supported such as format, bitrate, duration, size etc.
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_AUDIO_META_MODULE
 * @{
 */

/**
 * @brief Clones the audio metadata.
 * @details This function copies the audio metadata handle from a source to destination.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a dst should be released using audio_meta_destroy().
 *
 * @param[out] dst The destination handle to the audio metadata
 * @param[in] src The source handle to the audio metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see audio_meta_destroy()
 */
int audio_meta_clone(audio_meta_h *dst, audio_meta_h src);

/**
 * @brief Destroys the audio metadata.
 * @details This function frees all resources related to the audio metadata handle. This handle
 *          no longer can be used to perform any operations. A new handle has to
 *          be created before the next use.
 *
 * @since_tizen 2.3
 *
 * @param[in] audio The handle to the audio metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Get a copy of audio metadata handle handle by calling audio_meta_clone().
 *
 * @see audio_meta_clone()
 */
int audio_meta_destroy(audio_meta_h audio);

/**
 * @brief Gets the ID of the media of the given audio metadata.
 * @since_tizen 2.3
 *
 * @remarks The @a media_id should be released using free().
 *
 * @param[in] audio The handle to the audio metadata
 * @param[out] media_id The media ID
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int audio_meta_get_media_id(audio_meta_h audio, char **media_id);

/**
 * @brief Gets the album of the given audio metadata.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *          Since 3.0, if the media content has no album info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a album should be released using free().
 *
 * @param[in] audio The handle to the audio metadata
 * @param[out] album The album of the audio metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int audio_meta_get_album(audio_meta_h audio, char **album);

/**
 * @brief Gets the artist of the given audio metadata.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *          Since 3.0, if the media content has no artist info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a artist should be released using free().
 *
 * @param[in] audio The handle to the audio metadata
 * @param[out] artist The artist of the audio metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int audio_meta_get_artist(audio_meta_h audio, char **artist);

/**
 * @brief Gets the album artist of the given audio metadata.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *          Since 3.0, if the media content has no album artist info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a album_artist should be released using free().
 *
 * @param[in] audio The handle to the audio metadata
 * @param[out] album_artist The album artist of the audio metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int audio_meta_get_album_artist(audio_meta_h audio, char **album_artist);

/**
 * @brief Gets the genre of the given audio metadata.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *          Since 3.0, if the media content has no genre info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a genre should be released using free().
 *
 * @param[in] audio The handle to the audio metadata
 * @param[out] genre The genre of the audio metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int audio_meta_get_genre(audio_meta_h audio, char **genre);

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the composer of the given audio metadata.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *          Since 3.0, if the media content has no composer info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a composer should be released using free().
 *
 * @param[in] audio The handle to the audio metadata
 * @param[out] composer The composer of the audio metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int audio_meta_get_composer(audio_meta_h audio, char **composer) TIZEN_DEPRECATED_API;

/**
 * @brief Gets the year of the given audio metadata.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *          Since 3.0, if the media content has no year info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a year should be released using free().
 *
 * @param[in] audio The handle to the audio metadata
 * @param[out] year The year of the audio metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int audio_meta_get_year(audio_meta_h audio, char **year);

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the recorded date of the given audio metadata.
 * @details The format of the recorded date may vary depending on the file format. \n
 *          For more details on the recorded date format, refer to the file format specification.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a recorded_date should be released using free().
 *
 * @param[in] audio The handle to the audio metadata
 * @param[out] recorded_date The recorded date of the audio metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int audio_meta_get_recorded_date(audio_meta_h audio, char **recorded_date) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the copyright notice of the given audio metadata.
 * @details If the media content has no copyright info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a copyright should be released using free().
 *
 * @param[in] audio The handle to the audio metadata
 * @param[out] copyright The copyright of the audio metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int audio_meta_get_copyright(audio_meta_h audio, char **copyright) TIZEN_DEPRECATED_API;

/**
 * @brief Gets the track number of the given audio metadata.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *          Since 3.0, if the media content has no track info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a track_num should be released using free().
 *
 * @param[in] audio The handle to the audio metadata
 * @param[out] track_num The audio track number of the audio metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int audio_meta_get_track_num(audio_meta_h audio, char **track_num);

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the bitrate of the given audio metadata in bitrate per second.
 * @since_tizen 2.3
 *
 * @param[in] audio The handle to the audio metadata
 * @param[out] bit_rate The audio bitrate in bit per second [bps]
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_meta_get_bit_rate(audio_meta_h audio, int *bit_rate) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets bit per sample of the given audio metadata.
 * @since_tizen 2.3
 *
 * @param[in] audio The handle to the audio metadata
 * @param[out] bitpersample The audio bit per sample
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_meta_get_bitpersample(audio_meta_h audio, int *bitpersample) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the sample rate of the given audio metadata.
 * @since_tizen 2.3
 *
 * @param[in] audio The handle to the audio metadata
 * @param[out] sample_rate The audio sample rate[hz]
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_meta_get_sample_rate(audio_meta_h audio, int *sample_rate) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the channel of the given audio metadata.
 * @since_tizen 2.3
 *
 * @param[in] audio The handle to the audio metadata
 * @param[out] channel The channel of the audio
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_meta_get_channel(audio_meta_h audio, int *channel) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the track duration of the given audio metadata.
 * @since_tizen 2.3
 *
 * @param[in] audio The handle to the audio metadata
 * @param[out] duration The audio file duration
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_meta_get_duration(audio_meta_h audio, int *duration) TIZEN_DEPRECATED_API;

/**
 *@}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__TIZEN_CONTENT_MEDIA_AUDIO_H__*/
