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


#ifndef __TIZEN_CONTENT_MEDIA_VIDEO_H__
#define __TIZEN_CONTENT_MEDIA_VIDEO_H__

#include <media_content_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_video.h
 * @brief This file contains the video metadata API and related functions to proceed with video metadata. \n
 *        Functions include cloning and destroying video metadata, getting video metadata such as width, height, \n
 *        album, genre, etc.
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_VIDEO_META_MODULE
 * @{
 */

/**
 * @deprecated Deprecated since 9.0.
 * @brief Clones the video metadata.
 * @details This function copies the video metadata handle from a source to destination.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a dst should be released using video_meta_destroy().
 *
 * @param[out] dst The destination handle to the video metadata
 * @param[in] src The source handle to the video metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see video_meta_destroy()
 */
int video_meta_clone(video_meta_h *dst, video_meta_h src) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Destroys the video metadata.
 * @details This function frees all resources related to the video metadata handle. This handle
 *          no longer can be used to perform any operations. A new handle has to
 *          be created before the next use.
 *
 * @since_tizen 2.3
 *
 * @param[in] video The handle to the video metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Get copy of video metadata handle by calling video_meta_clone().
 *
 * @see video_meta_clone()
 */
int video_meta_destroy(video_meta_h video) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the ID of the media of the given video metadata.
 * @since_tizen 2.3
 *
 * @remarks The @a media_id should be released using free().
 *
 * @param[in] video The handle to the video metadata
 * @param[out] media_id The media ID
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int video_meta_get_media_id(video_meta_h video, char **media_id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the album of the given video metadata.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *          Since 3.0, if the media content has no album info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a album should be released using free().
 *
 * @param[in] video The handle to the video metadata
 * @param[out] album The album of the video metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int video_meta_get_album(video_meta_h video, char **album) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the artist of the given video metadata.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *          Since 3.0, if the media content has no artist info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a artist should be released using free().
 *
 * @param[in] video The handle to the video metadata
 * @param[out] artist The artist of the video metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int video_meta_get_artist(video_meta_h video, char **artist) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the video album artist.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *          Since 3.0, if the media content has no album artist info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a album_artist should be released using free().
 *
 * @param[in] video The handle to the video metadata
 * @param[out] album_artist The album artist of the video metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int video_meta_get_album_artist(video_meta_h video, char **album_artist) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the genre of the given video metadata.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *          Since 3.0, if the media content has no genre info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a genre should be released using free().
 *
 * @param[in] video The handle to the video metadata
 * @param[out] genre The genre of the video metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int video_meta_get_genre(video_meta_h video, char **genre) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the composer of the given video metadata.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *          Since 3.0, if the media content has no composer info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a composer should be released using free().
 *
 * @param[in] video The handle to the video metadata
 * @param[out] composer The composer of the video metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int video_meta_get_composer(video_meta_h video, char **composer) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the year of the given video metadata.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *          Since 3.0, if the media content has no year info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a year should be released using free().
 *
 * @param[in] video The handle to the video metadata
 * @param[out] year The year of the video metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int video_meta_get_year(video_meta_h video, char **year) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the recorded date of the given video metadata.
 * @details The format of the recorded date may vary depending on the file format. \n
 *          For more details on the recorded date format, refer to the file format specification.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a recorded_date should be released using free().
 *
 * @param[in] video The handle to the video metadata
 * @param[out] recorded_date The recorded date of the video metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int video_meta_get_recorded_date(video_meta_h video, char **recorded_date) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the copyright notice of the given video metadata.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *          Since 3.0, if the media content has no copyright info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a copyright should be released using free().
 *
 * @param[in] video The handle to the video metadata
 * @param[out] copyright The copyright of the video metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int video_meta_get_copyright(video_meta_h video, char **copyright) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the track number of the given video metadata.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *          Since 3.0, if the media content has no track info, the method returns empty string.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a track_num should be released using free().
 *
 * @param[in] video The handle to the video metadata
 * @param[out] track_num The track number of the video metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int video_meta_get_track_num(video_meta_h video, char **track_num) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the bitrate of the given video metadata in bitrate per second.
 * @since_tizen 2.3
 *
 * @param[in] video The handle to the video metadata
 * @param[out] bit_rate The video bit rate in bit per second [bps]
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int video_meta_get_bit_rate(video_meta_h video, int *bit_rate) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the track duration of the given video metadata.
 * @since_tizen 2.3
 *
 * @param[in] video The handle to the video metadata
 * @param[out] duration The video duration in milliseconds
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int video_meta_get_duration(video_meta_h video, int *duration) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the width of the given video metadata.
 * @since_tizen 2.3
 *
 * @param[in] video The handle to the video metadata
 * @param[out] width The video width in pixels
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int video_meta_get_width(video_meta_h video, int *width) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the height of the given video metadata.
 * @since_tizen 2.3
 *
 * @param[in] video The handle to the video metadata
 * @param[out] height The video height in pixels
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int video_meta_get_height(video_meta_h video, int *height) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the rotation of the given video metadata.
 * @since_tizen 4.0
 *
 * @param[in] video The handle to the video metadata
 * @param[out] rotation The clockwise rotation angle of the video in degrees (can be returned from 0 to less than 360)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int video_meta_get_rotation(video_meta_h video, int *rotation) TIZEN_DEPRECATED_API;

/**
 *@}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__TIZEN_CONTENT_MEDIA_VIDEO_H__*/
