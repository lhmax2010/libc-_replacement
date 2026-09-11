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

#ifndef __TIZEN_MEDIA_PACKET_INTERNAL_H__
#define __TIZEN_MEDIA_PACKET_INTERNAL_H__

#include <media_packet.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file media_packet_internal.h
 * @brief This file contains the capi media tool internal API.
 */

/**
 * @addtogroup CAPI_MEDIA_TOOL_INTERNAL_MODULE
 * @{
 */

/**
 * @internal
 * @brief Sets codec data and the codec data size of media packet.
 * @since_tizen 2.4
 *
 * @param[in] packet   The media packet handle
 * @param[in] codec_data    The codec data to set
 * @param[in] codec_data_size    The codec data size to set
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_set_codec_data(media_packet_h packet, void *codec_data, unsigned int codec_data_size);

/**
 * @internal
 * @brief Reset media_buffer_flags_e of media packet
 * @details	It will clear all flags of media packet
 * @since_tizen 3.0
 * @param[in] packet   The media packet handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_packet_reset_flags(media_packet_h packet);

/**
 * @internal
 * @brief Gets allocated buffer size of media packet.
 * @since_tizen 3.0
 * @param[in]  packet   The media packet handle
 * @param[out] size     The allocated buffer size value to get
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_packet_get_allocated_buffer_size(media_packet_h packet, int *size);

/**
 * @internal
 * @brief Adds a callback function to be invoked when the media packet is disposed.
 * @since_tizen 7.0
 * @param[in]  packet   The media packet handle
 * @param[in]  dcb      The media_packet_dispose_cb() to add
 * @param[in]  dcb_data The user data to be passed to the media_packet_dispose_cb() function
 * @param[out] id       The callback id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_add_dispose_cb(media_packet_h packet, media_packet_dispose_cb dcb, void *dcb_data, int *id);

/**
 * @internal
 * @brief Removes ths media packet dispose callback.
 * @since_tizen 7.0
 * @param[in] packet   The media packet handle
 * @param[in] id       The id of the callback to remove
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_packet_remove_dispose_cb(media_packet_h packet, int id);

/**
 * @internal
 * @brief Internal static function to free a media packet handle.
 * @since_tizen 10.0
 * @remarks This function is used by the media_packet_autoptr macro to automatically \n
 *          decrement the reference count of a media packet when the variable goes out of scope. \n
 *          It is automatically called by the cleanup attribute. \n
 *          This function should not be called directly; it is intended for use with the media_packet_autoptr macro.
 * @param[in] p    A double pointer to the media packet handle. \n
 *                 This pointer is dereferenced to media packet handle before being passed to media_packet_unref().
 */
static inline void __media_packet_free(void *p)
{
	void **pp = (void **)p;

	if (!*pp)
		return;

	media_packet_unref((media_packet_h)*pp);
}

/**
 * @internal
 * @brief Macro to provide automatic cleanup for media packet handle.
 * @since_tizen 10.0
 * @remarks This macro is used in variable declarations to automatically call media_packet_unref() \n
 *          when the declared variable goes out of scope, safely decrementing the media packet handle's reference count. \n
 *          This helps prevent resource leaks.
 */
#define media_packet_autoptr __attribute__((cleanup(__media_packet_free))) media_packet_h


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_PACKET_INTERNAL_H__ */
