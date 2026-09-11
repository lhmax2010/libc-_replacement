/*
* Copyright (c) 2022 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MEDIA_FORMAT_INTERNAL_H__
#define __TIZEN_MEDIA_FORMAT_INTERNAL_H__

#include <media_format.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file media_format_internal.h
 * @brief This file contains the media format internal API.
 */

/**
 * @addtogroup CAPI_MEDIA_TOOL_MEDIA_FORMAT_INTERNAL_MODULE
 * @{
 */

/**
 * @internal
 * @brief Get reference count of #media_format_h object.
 * @since_tizen 7.0
 * @param[in] fmt The media format handle
 * @param[out] ref_count Reference count of #media_format_h
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see media_format_ref()
 * @see media_format_unref()
 */
int media_format_get_refcount(media_format_h fmt, int *ref_count);

/**
 * @internal
 * @brief Sets codec data and the codec data size.
 * @since_tizen 7.5
 * @param[in] fmt        The media format handle
 * @param[in] codec_data The codec data to set
 * @param[in] size       The size of codec data
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_FORMAT_ERROR_OUT_OF_MEMORY Out of memory
 */
int media_format_set_codec_data(media_format_h fmt, void *codec_data, unsigned int size);

/**
 * @internal
 * @brief Gets codec data and the size.
 * @since_tizen 7.5
 * @remarks The @a codec_data should be released using free().
 * @param[in]  fmt        The media format handle
 * @param[out] codec_data The codec data to get
 * @param[out] size       The size of codec data
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 */
int media_format_get_codec_data(media_format_h fmt, void **codec_data, unsigned int *size);

/**
 * @internal
 * @brief Internal static function to free a media format handle.
 * @since_tizen 10.0
 * @remarks This function is used by the media_format_autoptr macro to automatically \n
 *          decrement the reference count of a media format when the variable goes out of scope. \n
 *          It is automatically called by the cleanup attribute. \n
 *          This function should not be called directly; it is intended for use with the media_format_autoptr macro.
 * @param[in] p    A double pointer to the media format handle. \n
 *                 This pointer is dereferenced to media format handle before being passed to media_format_unref().
 */
static inline void __media_format_free(void *p)
{
	void **pp = (void **)p;

	if (!*pp)
		return;

	media_format_unref((media_format_h)*pp);
}

/**
 * @internal
 * @brief Macro to provide automatic cleanup for media format handle.
 * @since_tizen 10.0
 * @remarks This macro is used in variable declarations to automatically call media_format_unref() \n
 *          when the declared variable goes out of scope, safely decrementing the media format handle's reference count. \n
 *          This helps prevent resource leaks.
 */
#define media_format_autoptr __attribute__((cleanup(__media_format_free))) media_format_h

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_FORMAT_INTERNAL_H__ */
