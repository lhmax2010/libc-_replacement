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

#ifndef __TIZEN_MULTIMEDIA_THUMBNAIL_UTIL_H__
#define __TIZEN_MULTIMEDIA_THUMBNAIL_UTIL_H__

#include <thumbnail_util_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file thumbnail_util.h
 * @brief This file contains the thumbnail util API and functions related with handling thumbnail. \n
 *        Functions include operations to get the various size thumbnail from media file.
 */

/**
 * @addtogroup CAPI_MEDIA_THUMBNAIL_UTIL_MODULE
 * @{
 */

/**
 * @brief Extracts the thumbnail for the given media into a file, synchronously.
 * @details This function creates thumbnail from a given media file. \n
 *          The generated thumbnail is saved in the given thumbnail path. \n
 *          The format of the thumbnail file is determined by the extension of the thumbnail path
 *          (e.g. path is input.jpg and thumbnail_path is thumbnail.png then thumbnail.png is encoded in the PNG format). \n
 *          If the extension is missing or the format is not supported, the thumbnail is encoded in the same format as the given media
 *          (e.g. path is input.jpg and thumbnail_path is thumbnail.out then thumbnail.out is encoded in the JPEG format). \n
 *          Supported image formats are as follows: \n
 *           - GIF, JPEG, PNG, BMP, WBMP \n
 *          Video files only support JPEG format thumbnail.
 *
 * @since_tizen 5.0
 *
 * @remarks %http://tizen.org/privilege/mediastorage is needed if input or output path points to media storage. \n
 *          %http://tizen.org/privilege/externalstorage is needed if input or output path points to external storage. \n
 *          The width and height of the thumbnail to be generated cannot exceed 2000. \n
 *          The width and height of the thumbnail to be generated cannot exceed the original resolution. \n
 *          Since 5.5, if the width and height of the thumbnail to be generated exceeds the original resolution, the value changes to the original resolution. \n
 *          In order to maintain the ratio, the requested size and generated size may be different. (Modify based on short axis) \n
 *          If the parent directory of the thumbnail path to be generated does not exist, an error is returned.
 *
 * @param[in] path       The path of the original media file
 * @param[in] width      The width of the thumbnail
 * @param[in] height     The height of the thumbnail
 * @param[in] thumbnail_path   The path to save the generated thumbnail
 *
 * @return @c 0 on success, otherwise a negative error value
 *
 * @retval #THUMBNAIL_UTIL_ERROR_NONE              Successful
 * @retval #THUMBNAIL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THUMBNAIL_UTIL_ERROR_INVALID_OPERATION Invalid operation
 * @retval #THUMBNAIL_UTIL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #THUMBNAIL_UTIL_ERROR_PERMISSION_DENIED Permission denied
 *
 */
int thumbnail_util_extract_to_file(const char *path, unsigned int width, unsigned int height, const char *thumbnail_path);

/**
 * @brief Extracts the thumbnail for the given media into a buffer, synchronously.
 * @details This function creates thumbnails from a given media file. \n
 *          The generated thumbnail is returned as a raw data stream.
 *
 * @since_tizen 5.0
 *
 * @remarks %http://tizen.org/privilege/mediastorage is needed if input or output path points to media storage. \n
 *          %http://tizen.org/privilege/externalstorage is needed if input or output path points to external storage. \n
 *          The width and height of the thumbnail to be generated cannot exceed 2000. \n
 *          The width and height of the thumbnail to be generated cannot exceed the original resolution. \n
 *          In the case of video file, color space of the generated thumbnail is RGB. \n
 *          In the case of image file, color space of the generated thumbnail is BGRA. \n
 *          The @a thumb_buffer should be released using free().
 *          Since 5.5, if the width and height of the thumbnail to be generated exceeds the original resolution, the value changes to the original resolution. \n
 *          In order to maintain the ratio, the requested size and generated size may be different. (Modify based on short axis)
 *
 * @param[in] path       The path of the original media file
 * @param[in] width      The width of the thumbnail
 * @param[in] height     The height of the thumbnail
 * @param[out] thumb_buffer   The raw data of the thumbnail which is generated
 * @param[out] thumb_size     The size of the thumbnail which is generated
 * @param[out] thumb_width    The width of the thumbnail which is generated
 * @param[out] thumb_height   The height of the thumbnail which is generated
 *
 * @return @c 0 on success, otherwise a negative error value
 *
 * @retval #THUMBNAIL_UTIL_ERROR_NONE              Successful
 * @retval #THUMBNAIL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THUMBNAIL_UTIL_ERROR_INVALID_OPERATION Invalid operation
 * @retval #THUMBNAIL_UTIL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #THUMBNAIL_UTIL_ERROR_PERMISSION_DENIED Permission denied
 *
 */
int thumbnail_util_extract_to_buffer(const char *path, unsigned int width, unsigned int height, unsigned char **thumb_buffer, size_t *thumb_size, unsigned int *thumb_width, unsigned int *thumb_height);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MULTIMEDIA_THUMBNAIL_UTIL_H__ */
