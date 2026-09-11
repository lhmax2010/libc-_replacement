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

#ifndef __TIZEN_MEDIA_PACKET_H__
#define __TIZEN_MEDIA_PACKET_H__

#include <tizen.h>
#include <stdint.h>

#include <media_format.h>

#include <tbm_type.h>
#include <tbm_surface.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file media_packet.h
 * @brief This file contains the capi media tool API.
 */

/**
 * @addtogroup CAPI_MEDIA_TOOL_MODULE
 * @{
 */

/**
 * @brief  The Media Packet handle.
 * @since_tizen 2.3
 */
typedef struct media_packet_s *media_packet_h;

/**
 * @brief Enumeration for media packet error.
 * @since_tizen 2.3
 */
typedef enum {
	MEDIA_PACKET_ERROR_NONE = TIZEN_ERROR_NONE,											/**< Successful */
	MEDIA_PACKET_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,						/**< Out of memory */
	MEDIA_PACKET_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,				/**< Invalid parameter */
	MEDIA_PACKET_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION,				/**< Invalid operation */
	MEDIA_PACKET_ERROR_FILE_NO_SPACE_ON_DEVICE = TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE,	/**< No space left on device */
	MEDIA_PACKET_ERROR_NO_AVAILABLE_PACKET = TIZEN_ERROR_MEDIA_TOOL | 0x01,				/**< No Available packet, (Since 3.0) */
} media_packet_error_e;

/**
 * @brief Enumeration for media buffer flag.
 * @since_tizen 2.3
 */
typedef enum {
	MEDIA_PACKET_CODEC_CONFIG = 0x1,   /**< The buffer marked as such contains codec initialization/codec specific data instead of media data */
	MEDIA_PACKET_END_OF_STREAM = 0x2,  /**< The end of stream */
	MEDIA_PACKET_SYNC_FRAME = 0x4,	   /**< The buffer marked as such contains the data for a sync frame */
} media_buffer_flags_e;

/**
 * @deprecated Deprecated since 6.5.
 * @brief Enumeration for the return values of media packet finalize call back functions.
 * @since_tizen 2.3
 * @see media_packet_finalize_cb()
 */
typedef enum _finalize_cb_ret {
	MEDIA_PACKET_REUSE = 0,	 /**< Packet handle is not destroyed */
	MEDIA_PACKET_FINALIZE,	 /**< Destroy packet handle */
} media_packet_finalize_cb_ret_t;

/**
 * @brief Enumeration for media packet rotation method.
 * @since_tizen 5.0
 */
typedef enum {
	MEDIA_PACKET_ROTATE_IDENTITY,         /**< None */
	MEDIA_PACKET_ROTATE_90,               /**< Rotate 90 degrees */
	MEDIA_PACKET_ROTATE_180,              /**< Rotate 180 degrees */
	MEDIA_PACKET_ROTATE_270,              /**< Rotate 270 degrees */
	MEDIA_PACKET_ROTATE_HORIZONTAL_FLIP,  /**< Flip horizontal */
	MEDIA_PACKET_ROTATE_VERTICAL_FLIP     /**< Flip vertical */
} media_packet_rotate_method_e;

/**
 * @deprecated Deprecated since 6.5.
 * @brief   Called when the media packet is destroyed.
 * @details It will be invoked when media_packet_destroy() is called.
 *
 * @since_tizen 2.3
 *
 * @param[in] packet        The media packet handle
 * @param[in] error_code    The error code of #media_packet_error_e
 * @param[in] user_data  The user data passed from the callback registration function
 * @retval #MEDIA_PACKET_REUSE     Packet handle is not destroyed, the handle will be reused.
 * @retval #MEDIA_PACKET_FINALIZE  Destroy media packet handle, the handle will not be reused.
 * @pre It will be invoked when media packet is destroyed.
 * @see media_packet_destroy()
 * @see media_packet_create_alloc()
 * @see media_packet_create()
 * @see media_packet_copy()
 * @see media_packet_create_from_tbm_surface()
 */
typedef int (*media_packet_finalize_cb) (media_packet_h packet, int error_code, void *user_data);

/**
 * @brief   Called when the media packet is disposed.
 * @details It will be invoked when media_packet_unref() is called, \n
 *          and reference count is decreased to 0.
 * @since_tizen 6.5
 * @remarks The media packet handle should not be released in this callback. \n
 *          It will be released by the framework after return.
 * @param[in] packet    The media packet handle
 * @param[in] user_data The user data passed from the callback registration function
 * @pre It will be invoked when media packet is being destroyed.
 * @see media_packet_unref()
 * @see media_packet_new()
 * @see media_packet_new_alloc()
 * @see media_packet_new_from_tbm_surface()
 * @see media_packet_new_from_external_memory()
 */
typedef void (*media_packet_dispose_cb)(media_packet_h packet, void *user_data);

/**
 * @deprecated Deprecated since 6.5. Use media_packet_new_alloc() instead.
 * @brief    Creates a media packet handle and allocates buffer.
 * @details  The buffer will be allocated to heap or tbm_surface.
 * @since_tizen 2.3
 * @remarks The @a packet should be released using media_packet_destroy().
 * @param[in]  fmt       The allocated #media_format_h by caller
 * @param[in]  fcb       The media_packet_finalize_cb() to register
 * @param[in]  fcb_data  The user data to be passed to the media_packet_finalize_cb() function
 * @param[out] packet    A new handle for media packet
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Must have media_format_h instance by media_format_create()
 * @post Must do media_format_unref()
 *
 * @see media_packet_destroy()
 * @see media_packet_finalize_cb()
 * @see media_format_unref()
 * @par Example
   @code
   #include <media_packet.h>

   {
   media_format_h fmt;
   media_packet_h packet;

   media_format_create(&fmt);
   media_format_set_video_mime(fmt, MEDIA_FORMAT_H264_HP);
   media_format_set_video_width(fmt, 640);
   media_format_set_video_height(fmt, 480);
   media_format_set_video_avg_bps(fmt, 10000000);
   media_format_set_video_max_bps(fmt, 15000000);

   media_packet_create_alloc (fmt, _finalize_callback, fcb_data, &packet);
   media_format_unref(fmt);

   ...
   media_packet_destroy(packet);
   }

   int _finalize_callback(media_packet_h packet, int err, void* userdata)
   {
       ...
       return MEDIA_PACKET_FINALIZE;
   }

   @endcode
 */
int media_packet_create_alloc(media_format_h fmt, media_packet_finalize_cb fcb, void *fcb_data, media_packet_h * packet) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 6.5. Use media_packet_new() instead.
 * @brief    Creates a media packet handle.
 * @details  It creates only media packet handle without allocated buffer.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a packet must be released by using media_packet_destroy().
 * @param[in] fmt       The allocated #media_format_h by caller
 * @param[in] fcb       The media_packet_finalize_cb() to register
 * @param[in] fcb_data  The user data to be passed to the media_packet_finalize_cb() function
 * @param[out] packet   A new handle for media packet
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Must have media_format_h instance by media_format_create()
 * @post Must do media_format_unref()
 *
 * @see media_packet_destroy()
 * @see media_packet_finalize_cb()
 * @see media_format_unref()
 * @par Example
   @code
   #include <media_packet.h>

   {
   media_format_h fmt;
   media_packet_h packet;

   media_format_create(&fmt);
   media_format_set_video_mime(fmt, MEDIA_FORMAT_H264_HP);
   media_format_set_video_width(fmt, 640);
   media_format_set_video_height(fmt, 480);
   media_format_set_video_avg_bps(fmt, 10000000);
   media_format_set_video_max_bps(fmt, 15000000);

   media_packet_create (fmt, _finalize_callback, fcb_data, &packet);
   media_format_unref(fmt);

   ...
   media_packet_destroy(packet);
   }

   int _finalize_callback(media_packet_h packet, int err, void* userdata)
   {
       ...
       return MEDIA_PACKET_FINALIZE;
   }

   @endcode
 */
int media_packet_create(media_format_h fmt, media_packet_finalize_cb fcb, void *fcb_data, media_packet_h * packet) TIZEN_DEPRECATED_API;

/**
 * @brief Creates a media packet handle.
 * @details It creates only media packet handle without allocated buffer.
 * @since_tizen 6.5
 * @remarks The @a packet must be released by using media_packet_unref().
 * @param[in] fmt      The #media_format_h allocated by the caller
 * @param[in] dcb      The media_packet_dispose_cb() to register. Use @c NULL to skip the callback.
 * @param[in] dcb_data The user data to be passed to the media_packet_dispose_cb() function
 * @param[out] packet  A new handle for media packet
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 * @pre Must have media_format_h instance by media_format_create()
 * @post Must call media_format_unref()
 * @see media_packet_ref()
 * @see media_packet_unref()
 * @see media_packet_dispose_cb()
 */
int media_packet_new(media_format_h fmt, media_packet_dispose_cb dcb, void *dcb_data, media_packet_h *packet);

/**
 * @brief Creates a media packet handle and allocates buffer.
 * @details The buffer will be allocated to heap or tbm_surface.
 * @since_tizen 6.5
 * @remarks The @a packet should be released using media_packet_unref().
 * @param[in]  fmt      The allocated #media_format_h by caller
 * @param[in]  dcb      The media_packet_dispose_cb() to register. Use @c NULL to skip the callback.
 * @param[in]  dcb_data The user data to be passed to the media_packet_dispose_cb() function
 * @param[out] packet   A new handle for media packet
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 * @pre Must have media_format_h instance by media_format_create()
 * @post Must do media_format_unref()
 * @see media_packet_ref()
 * @see media_packet_unref()
 * @see media_packet_dispose_cb()
 */
int media_packet_new_alloc(media_format_h fmt, media_packet_dispose_cb dcb, void *dcb_data, media_packet_h *packet);

/**
 * @brief Creates media packet handle and allocates buffer with #tbm_surface_h.
 * @since_tizen 6.5
 * @remarks The @a packet must be released by using media_packet_unref().
 * @param[in]  fmt      The #media_format_h allocated by caller
 * @param[in]  surface  The #tbm_surface_h by caller
 * @param[in]  dcb      The media_packet_dispose_cb() to register. Use @c NULL to skip the callback.
 * @param[in]  dcb_data The user data to be passed to the media_packet_dispose_cb() function
 * @param[out] packet   A new handle for media packet
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 * @pre Must have media_format_h instance by media_format_create()
 * @post Must do media_format_unref()
 * @see media_packet_ref()
 * @see media_packet_unref()
 * @see media_packet_dispose_cb()
 */
int media_packet_new_from_tbm_surface(media_format_h fmt, tbm_surface_h surface, media_packet_dispose_cb dcb, void *dcb_data, media_packet_h *packet);

/**
 * @brief Creates media packet handle with already allocated buffer.
 * @details It does not support video's #MEDIA_FORMAT_RAW type.
 * @since_tizen 6.5
 * @remarks The @a packet must be released by using media_packet_unref().
 * @param[in]  fmt      The allocated #media_format_h by caller
 * @param[in]  mem_ptr  The memory pointer which is created by external module
 * @param[in]  size     The buffer size value to set
 * @param[in]  dcb      The media_packet_dispose_cb() to register. Use @c NULL to skip the callback.
 * @param[in]  dcb_data The user data to be passed to the media_packet_dispose_cb() function
 * @param[out] packet   A new handle for media packet
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 * @pre Must have media_format_h instance by media_format_create()
 * @post Must do media_format_unref()
 * @see media_packet_ref()
 * @see media_packet_unref()
 * @see media_packet_dispose_cb()
 * @see media_format_unref()
 */
int media_packet_new_from_external_memory(media_format_h fmt, void *mem_ptr, uint64_t size, media_packet_dispose_cb dcb, void *dcb_data, media_packet_h *packet);

/**
 * @brief Increases reference count of a media packet handle.
 * @since_tizen 6.5
 * @param[in] packet The media packet handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 * @see media_packet_unref()
 */
int media_packet_ref(media_packet_h packet);

/**
 * @brief Decreases reference count of a media packet handle.
 * @since_tizen 6.5
 * @param[in] packet The media packet handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 * @see media_packet_ref()
 */
int media_packet_unref(media_packet_h packet);

/**
 * @deprecated Deprecated since 6.5.
 * @brief    Copies a media packet handle.
 * @details  It re-creates only media packet handle with exist media packet handle.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a new_packet must be released by using media_packet_destroy().
 * @remarks It will return #MEDIA_PACKET_ERROR_INVALID_OPERATION \n
 *          if the media packet handle is from media_packet_new(), \n
 *          media_packet_new_alloc(), media_packet_new_from_tbm_surface() \n
 *          and media_packet_new_from_external_memory(). (since 6.5)
 * @param[in]   org_packet   The existing media packet handle
 * @param[in]   fcb          The media_packet_finalize_cb() to register
 * @param[in]   fcb_data     The user data to be passed to the media_packet_finalize_cb() function
 * @param[out]  new_packet   A new handle for media packet
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_packet_destroy()
 * @see media_packet_finalize_cb()
 */
int media_packet_copy(media_packet_h org_packet, media_packet_finalize_cb fcb, void *fcb_data, media_packet_h * new_packet) TIZEN_DEPRECATED_API;

/**
 * @brief    Allocates buffer with media packet handle.
 * @details  Before using media_packet_alloc(), media packet handle must be exist.
 *
 * @since_tizen 2.3
 *
 * @param[in] packet   The existing media packet handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_packet_new()
 * @see media_packet_unref()
 */
int media_packet_alloc(media_packet_h packet);

/**
 * @deprecated Deprecated since 6.5. Use media_packet_new_from_tbm_surface() instead.
 * @brief    Creates media packet handle and allocates buffer with #tbm_surface_h.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a packet must be released by using media_packet_destroy().
 * @param[in]   fmt       The allocated #media_format_h by caller
 * @param[in]   surface   The #tbm_surface_h by caller
 * @param[in]   fcb       The media_packet_finalize_cb() to register
 * @param[in]   fcb_data  The user data to be passed to the media_packet_finalize_cb() function
 * @param[out]  packet  A new handle for media packet
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Must have media_format_h instance by media_format_create()
 * @post Must do media_format_unref()
 *
 * @see media_packet_destroy()
 * @see media_packet_finalize_cb()
 * @see media_format_unref()
 * @par Example
   @code
   #include <media_packet.h>

   {
   media_format_h fmt;
   media_packet_h packet;

   media_format_create(&fmt);
   media_format_set_video_mime(fmt, MEDIA_FORMAT_H264_HP);
   media_format_set_video_width(fmt, 640);
   media_format_set_video_height(fmt, 480);
   media_format_set_video_avg_bps(fmt, 10000000);
   media_format_set_video_max_bps(fmt, 15000000);

   media_packet_create_from_tbm_surface (fmt, surface, _finalize_callback, fcb_data, &packet);
   media_format_unref(fmt);

   ...
   media_packet_destroy(packet);
   }

   int _finalize_callback(media_packet_h packet, int err, void* userdata)
   {
       ...
       return MEDIA_PACKET_FINALIZE;
   }

   @endcode
 */
int media_packet_create_from_tbm_surface(media_format_h fmt, tbm_surface_h surface, media_packet_finalize_cb fcb, void *fcb_data, media_packet_h * packet) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 6.5. Use media_packet_new_from_external_memory() instead.
 * @brief    Creates media packet handle with already allocated external buffer.
 * @details It does not support video's #MEDIA_FORMAT_RAW type.
 *
 * @since_tizen 2.4
 *
 * @remarks The @a packet must be released by using media_packet_destroy().
 * @param[in]   fmt       The allocated #media_format_h by caller
 * @param[in]   mem_ptr   The memory pointer which is created by external module
 * @param[in]   size      The buffer size value to set
 * @param[in]   fcb       The media_packet_finalize_cb() to register
 * @param[in]   fcb_data  The user data to be passed to the media_packet_finalize_cb() function
 * @param[out]  packet  A new handle for media packet
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Must have media_format_h instance by media_format_create()
 * @post Must do media_format_unref()
 *
 * @see media_packet_destroy()
 * @see media_packet_finalize_cb()
 * @see media_format_unref()
 * @par Example
   @code
   #include <media_packet.h>

   {
   media_format_h fmt;
   media_packet_h packet;

   media_format_create(&fmt);
   media_format_set_video_mime(fmt, MEDIA_FORMAT_H264_HP);
   media_format_set_video_width(fmt, 640);
   media_format_set_video_height(fmt, 480);
   media_format_set_video_avg_bps(fmt, 10000000);
   media_format_set_video_max_bps(fmt, 15000000);

   media_packet_create_from_external_memory (fmt, mem_ptr, size,  _finalize_callback, fcb_data, &packet);
   media_format_unref(fmt);

   ...
   media_packet_destroy(packet);
   }

   int _finalize_callback(media_packet_h packet, int err, void* userdata)
   {
       ...
       return MEDIA_PACKET_FINALIZE;
   }

   @endcode
 */
int media_packet_create_from_external_memory(media_format_h fmt, void *mem_ptr, uint64_t size, media_packet_finalize_cb fcb, void *fcb_data, media_packet_h * packet) TIZEN_DEPRECATED_API;

/**
 * @brief Gets #media_format_h of media packet.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] fmt      The media format of media packet
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_format_unref()
 * @par Example
   @code
   #include <media_packet.h>

   {
   media_format_h* fmt;

   media_packet_get_format (pakcet, &fmt);
   ..

   media_format_unref(fmt);
   ...
   }

   @endcode
 */
int media_packet_get_format(media_packet_h packet, media_format_h * fmt);

/**
 * @brief Sets #media_format_h of media packet.
 * @since_tizen 2.3
 *
 * @param[in] packet   The media packet handle
 * @param[in] fmt      The #media_format_h to set
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_format_unref()
 * @par Example
   @code
   #include <media_packet.h>

   {
   media_format_h fmt;

   media_packet_set_format (pakcet, fmt);
   ..

   media_format_unref(fmt);
   ...
   }

   @endcode
 */
int media_packet_set_format(media_packet_h packet, media_format_h fmt);

/**
 * @brief Sets presentation timestamp(pts) of media packet.
 * @since_tizen 2.3
 *
 * @param[in] packet   The media packet handle
 * @param[in] pts      The pts in nanoseconds to set
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_set_pts(media_packet_h packet, uint64_t pts);

/**
 * @brief Sets decoding timestamp(dts) of media packet handle.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[in]  dts      The dts in nanoseconds to set
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_set_dts(media_packet_h packet, uint64_t dts);

/**
 * @brief Sets duration of media packet.
 * @since_tizen 2.3
 *
 * @param[in] packet    The media packet handle
 * @param[in] duration  The duration in nanoseconds to set
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_set_duration(media_packet_h packet, uint64_t duration);

/**
 * @brief Sets buffer size of media packet.
 * @since_tizen 2.3
 *
 * @param[in] packet   The media packet handle
 * @param[in] size     The buffer size value to set
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_set_buffer_size(media_packet_h packet, uint64_t size);

/**
 * @brief Gets presentation timestamp(pts) of media packet.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] pts      The pts in nanoseconds to get
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_packet_get_pts(media_packet_h packet, uint64_t * pts);

/**
 * @brief Gets decoding timestamp(dts) of media packet.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] dts      The dts in nanoseconds to get
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_dts(media_packet_h packet, uint64_t * dts);

/**
 * @brief Gets duration of media packet.
 * @since_tizen 2.3
 *
 * @param[in]  packet    The media packet handle
 * @param[out] duration  The duration in nanoseconds to get
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_duration(media_packet_h packet, uint64_t * duration);

/**
 * @brief Gets buffer size of media packet.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] size     The buffer size value to get
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_buffer_size(media_packet_h packet, uint64_t * size);

/**
 * @brief Gets buffer data pointer of media packet.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] data     The allocated buffer data pointer
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_buffer_data_ptr(media_packet_h packet, void **data);

/**
 * @brief Gets TBM surface data of media packet.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] surface  The tbm_surface data pointer
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_tbm_surface(media_packet_h packet, tbm_surface_h * surface);

/**
 * @brief Sets extra data of media packet.
 * @since_tizen 2.3
 *
 * @param[in] packet   The media packet handle
 * @param[in] extra    The extra data to set
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_set_extra(media_packet_h packet, void *extra);

/**
 * @brief Gets extra data of media packet.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] extra    The extra data to get
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_extra(media_packet_h packet, void **extra);

/**
 * @brief Checks whether the given media packet is for video.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] is_video @c true if the given media packet is for video,
 *                      otherwise @c false if the given media packet is not for video
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_format_mimetype_e
 * @see media_format_type_e
 */
int media_packet_is_video(media_packet_h packet, bool * is_video);

/**
 * @brief Checks whether the given media packet is for audio.
 * @since_tizen 2.3
 *
 * @param[in]  packet    The media packet handle
 * @param[out] is_audio  @c true if the given media packet is for audio,
 *                       otherwise @c false if the given media packet is not for audio
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_mimetype_e
 * @see media_format_type_e
 */
int media_packet_is_audio(media_packet_h packet, bool * is_audio);

/**
 * @brief Checks whether the given media packet is for text.
 * @since_tizen 3.0
 *
 * @param[in]  packet    The media packet handle
 * @param[out] is_text  @c true if the given media packet is for text,
 *                       otherwise @c false if the given media packet is not for text
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_mimetype_e
 * @see media_format_type_e
 */
int media_packet_is_text(media_packet_h packet, bool * is_text);

/**
 * @brief Checks whether the given media packet is encoded type.
 * @since_tizen 2.3
 *
 * @param[in]  packet      The media packet handle
 * @param[out] is_encoded  @c true if the given media packet is encoded,
 *                         otherwise @c false if the given media packet is not encoded
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_format_mimetype_e
 * @see media_format_data_type_e
 */
int media_packet_is_encoded(media_packet_h packet, bool * is_encoded);

/**
 * @brief Checks whether the given media packet is raw type.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] is_raw   @c true if the given media packet is for raw video,
 *                      otherwise @c false if the given media packet is not for raw video
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_format_mimetype_e
 * @see media_format_data_type_e
 */
int media_packet_is_raw(media_packet_h packet, bool * is_raw);

/**
 * @brief Gets #media_buffer_flags_e of media packet.
 * @since_tizen 3.0
 *
 * @param[in] packet    The media packet handle
 * @param[out] flags    The #media_buffer_flags_e of media packet to get
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_buffer_flags_e
 * @see media_packet_unset_flags()
 */
int media_packet_get_flags(media_packet_h packet, media_buffer_flags_e * flags);

/**
 * @brief Sets #media_buffer_flags_e of media packet.
 * @since_tizen 2.3
 *
 * @param[in] packet    The media packet handle
 * @param[in] flags    The #media_buffer_flags_e of media packet to set
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_buffer_flags_e
 * @see media_packet_unset_flags()
 */
int media_packet_set_flags(media_packet_h packet, media_buffer_flags_e flags);

/**
 * @brief Unsets #media_buffer_flags_e of media packet.
 * @since_tizen 2.3
 *
 * @param[in] packet   The media packet handle
 * @param[in] flags    The #media_buffer_flags_e of media packet to unset
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_buffer_flags_e
 * @see media_packet_set_flags()
 * */
int media_packet_unset_flags(media_packet_h packet, media_buffer_flags_e flags);

/**
 * @brief Checks whether the given media packet is codec data.
 * @since_tizen 2.3
 *
 * @param[in] packet            The media packet handle
 * @param[out] is_codec_config  @c true if the given media packet is for codec data,
 *                              otherwise @c false if the given media packet is not for codec data
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_format_mimetype_e
 * @see media_format_data_type_e
 */
int media_packet_is_codec_config(media_packet_h packet, bool * is_codec_config);

/**
 * @brief Checks whether the given media packet is eos.
 * @since_tizen 2.3
 *
 * @param[in] packet   The media packet handle
 * @param[out] is_eos  @c true if the given media packet is for eos,
 *                     otherwise @c false if the given media packet is not for eos
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_format_mimetype_e
 * @see media_format_data_type_e
 */
int media_packet_is_end_of_stream(media_packet_h packet, bool * is_eos);

/**
 * @brief Checks whether the given media packet is sync frame.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] is_sync  @c true if the given media packet is for sync frame,
 *                      otherwise @c false if the given media packet is not for sync frame
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_format_mimetype_e
 * @see media_format_data_type_e
 */
int media_packet_is_sync_frame(media_packet_h packet, bool * is_sync);

/**
 * @brief Checks whether the allocated buffer is tbm surface or not.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] has_tbm_surface  @c true if the given media packet's allocated buffer is tbm surface,
 *                      otherwise @c false if the given media packet's allocated buffer is not tbm surface
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_has_tbm_surface_buffer(media_packet_h packet, bool * has_tbm_surface);

/**
 * @brief Gets the number of planes from tbm surface in the given media packet.
 * @details Use only if the media_format_h is #MEDIA_FORMAT_RAW and #MEDIA_FORMAT_VIDEO.
 *          It means that media_packet_h's buffer is allocated on tbm_surface.
 *          If not sure of that, use media_packet_is_video() and media_packet_is_raw() or media_packet_has_tbm_surface_buffer().
 *
 * @since_tizen 2.4
 *
 * @param[in]  packet   The media packet handle
 * @param[out] num  The number of planes from tbm_surface
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_number_of_video_planes(media_packet_h packet, uint32_t * num);

/**
 * @brief Gets stride width from tbm surface in the given media packet.
 * @details Use only if the media_format_h is #MEDIA_FORMAT_RAW and #MEDIA_FORMAT_VIDEO.
 *          It means that media_packet_h's buffer is allocated on tbm_surface.
 *          If not sure of that, use media_packet_is_video() and media_packet_is_raw() or media_packet_has_tbm_surface_buffer().
 *
 * @since_tizen 2.4
 *
 * @param[in]  packet   The media packet handle
 * @param[in]  plane_idx   The plane index value
 * @param[out] stride_width  the stride value from tbm_surface
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_video_stride_width(media_packet_h packet, int plane_idx, int *stride_width);

/**
 * @brief Gets stride height from tbm surface in the given media packet.
 * @details Use only if the media_format_h is #MEDIA_FORMAT_RAW and #MEDIA_FORMAT_VIDEO.
 *          It means that media_packet_h's buffer is allocated on tbm_surface.
 *          If not sure of that, use media_packet_is_video() and media_packet_is_raw() or media_packet_has_tbm_surface_buffer().
 *
 * @since_tizen 2.4
 *
 * @param[in]  packet   The media packet handle
 * @param[in]  plane_idx   The plane index value
 * @param[out] stride_height  The stride height value from tbm_surface
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_video_stride_height(media_packet_h packet, int plane_idx, int *stride_height);

/**
 * @brief Gets plane data pointer from tbm surface in the given media packet.
 * @details Use only if the media_format_h is #MEDIA_FORMAT_RAW and #MEDIA_FORMAT_VIDEO.
 *          It means that media_packet_h's buffer is allocated on tbm_surface.
 *          If not sure of that, use media_packet_is_video() and media_packet_is_raw() or media_packet_has_tbm_surface_buffer().
 *
 * @since_tizen 2.4
 *
 * @remarks The @a plane_data_ptr must not be released by using free(). Note that It is released by media_packet_destroy() or tbm_surface_destroy().
 * @param[in]  packet   The media packet handle
 * @param[in]  plane_idx   The plane index value
 * @param[out] plane_data_ptr  The plane data pointer from tbm_surface
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_video_plane_data_ptr(media_packet_h packet, int plane_idx, void **plane_data_ptr);

/**
 * @brief Gets codec data and the codec data size of media packet.
 * @since_tizen 2.4
 * @remarks The @a codec_data should be released using free().
 * @param[in] packet   The media packet handle
 * @param[out] codec_data    The codec data to get
 * @param[out] codec_data_size    The codec data size to get
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_codec_data(media_packet_h packet, void **codec_data, unsigned int *codec_data_size);

/**
 * @deprecated Deprecated since 6.5. Use media_packet_unref() instead.
 * @brief Destroys the media packet handle.
 * @details The registered media_packet_finalize_cb() or media_packet_dispose_cb() \n
 *          will be invoked when @a packet is released. (since 6.5)
 * @since_tizen 2.3
 * @param[in] packet  The handle to media packet to be destroyed
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_packet_create_alloc()
 * @see media_packet_create()
 * @see media_packet_copy()
 * @see media_packet_create_from_tbm_surface()
 * @see media_packet_create_from_external_memory()
 */
int media_packet_destroy(media_packet_h packet) TIZEN_DEPRECATED_API;

/**
 * @brief Sets the rotation method.
 * @since_tizen 5.0
 *
 * @param[in] packet   The media packet handle
 * @param[in] method   The rotation method
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_set_rotate_method(media_packet_h packet, media_packet_rotate_method_e method);

/**
 * @brief Gets the rotation method.
 * @since_tizen 5.0
 *
 * @param[in] packet   The media packet handle
 * @param[out] method  The rotation method
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_rotate_method(media_packet_h packet, media_packet_rotate_method_e *method);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif							/* __TIZEN_MEDIA_PACKET_H__ */
