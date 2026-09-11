/*
* Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MEDIA_CODEC_INTERNAL_H__
#define __TIZEN_MEDIA_CODEC_INTERNAL_H__

#include <media_codec.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @file media_codec_internal.h
* @brief This file contains the internal capi media codec API.
*/

/**
* @addtogroup CAPI_MEDIA_CODEC_MODULE
* @{
*/

/**
 * @brief Retrieves all supported codecs by invoking callback function once for each supported codecs.
 * @since_tizen 3.0
 * @param[in] callback  The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIACODEC_ERROR_NONE Successful
 * @retval #MEDIACODEC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see mediacodec_foreach_supported_codec()
 */
int mediacodec_foreach_supported_codec_static(mediacodec_supported_codec_cb callback, void *user_data);

int mediacodec_enable_extra_video_converter(mediacodec_h mediacodec, gboolean enable, const char *converter_name, int crop_x, int crop_y, int crop_w, int crop_h);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif /*__TIZEN_MEDIA_CODEC_INTERNAL_H__*/
