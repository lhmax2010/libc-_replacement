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

#ifndef __TIZEN_MULTIMEDIA_IMAGE_UTIL_TYPE_H__
#define __TIZEN_MULTIMEDIA_IMAGE_UTIL_TYPE_H__

#include <stdlib.h>
#include <tizen.h>
#include <media_packet.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
* @brief Definition for image util Error Class.
* @since_tizen 2.3
*
*/
#define IMAGE_UTIL_ERROR_CLASS		  TIZEN_ERROR_IMAGE_UTIL

/**
 * @file image_util_type.h
 * @brief This file contains the image util API
 */

/**
 * @addtogroup CAPI_MEDIA_IMAGE_UTIL_MODULE
 * @{
 */

/**
 * @brief Enumeration for error.
 * @since_tizen 2.3
 */
typedef enum {
	IMAGE_UTIL_ERROR_NONE =				TIZEN_ERROR_NONE,                /**< Successful */
	IMAGE_UTIL_ERROR_INVALID_PARAMETER =	TIZEN_ERROR_INVALID_PARAMETER,   /**< Invalid parameter */
	IMAGE_UTIL_ERROR_OUT_OF_MEMORY =	TIZEN_ERROR_OUT_OF_MEMORY,       /**< Out of memory */
	IMAGE_UTIL_ERROR_NO_SUCH_FILE  =		TIZEN_ERROR_NO_SUCH_FILE,            /**< No such file */
	IMAGE_UTIL_ERROR_INVALID_OPERATION =	TIZEN_ERROR_INVALID_OPERATION,   /**< Internal error */
	IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT = IMAGE_UTIL_ERROR_CLASS | 0x01,   /**< Not supported format */
	IMAGE_UTIL_ERROR_PERMISSION_DENIED =	TIZEN_ERROR_PERMISSION_DENIED,   /**< Permission denied  */
	IMAGE_UTIL_ERROR_NOT_SUPPORTED =		TIZEN_ERROR_NOT_SUPPORTED          /**< Not supported */
} image_util_error_e;

/**
 * @brief Enumeration for colorspace.
 * @since_tizen 2.3
 */
typedef enum {
	IMAGE_UTIL_COLORSPACE_YV12,	 /**< YV12 - YCrCb planar format */
	IMAGE_UTIL_COLORSPACE_YUV422,   /**< YUV422 - planar */
	IMAGE_UTIL_COLORSPACE_I420,	 /**< YUV420 - planar */
	IMAGE_UTIL_COLORSPACE_NV12,	 /**< NV12- planar */

	IMAGE_UTIL_COLORSPACE_UYVY,	 /**< UYVY - packed */
	IMAGE_UTIL_COLORSPACE_YUYV,	 /**< YUYV - packed */

	IMAGE_UTIL_COLORSPACE_RGB565,   /**< RGB565, high-byte is Blue */
	IMAGE_UTIL_COLORSPACE_RGB888,   /**< RGB888, high-byte is Blue */
	IMAGE_UTIL_COLORSPACE_ARGB8888, /**< ARGB8888, high-byte is Blue */

	IMAGE_UTIL_COLORSPACE_BGRA8888, /**< BGRA8888, high-byte is Alpha */
	IMAGE_UTIL_COLORSPACE_RGBA8888, /**< RGBA8888, high-byte is Alpha */
	IMAGE_UTIL_COLORSPACE_BGRX8888, /**< BGRX8888, high-byte is X */
	IMAGE_UTIL_COLORSPACE_NV21,	 /**< NV21- planar */
	IMAGE_UTIL_COLORSPACE_NV16,	 /**< NV16- planar */
	IMAGE_UTIL_COLORSPACE_NV61,	 /**< NV61- planar */
} image_util_colorspace_e;

/**
* @brief The handle of an image.
* @since_tizen 5.5
*/
typedef void *image_util_image_h;

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_IMAGE_UTIL_TRANSFORM_MODULE
 * @{
 */

/**
 * @brief Enumeration for rotation.
 * @since_tizen 2.3
 */
typedef enum {
	IMAGE_UTIL_ROTATION_NONE = 0,	/**< None */
	IMAGE_UTIL_ROTATION_90,			/**< Rotation 90 degree */
	IMAGE_UTIL_ROTATION_180,			/**< Rotation 180 degree */
	IMAGE_UTIL_ROTATION_270,			/**< Rotation 270 degree */
	IMAGE_UTIL_ROTATION_FLIP_HORZ,	/**< Flip horizontal */
	IMAGE_UTIL_ROTATION_FLIP_VERT,	/**< Flip vertical */
} image_util_rotation_e;

/**
* @ingroup CAPI_MEDIA_IMAGE_UTIL_TRANSFORM_MODULE
* @brief The handle of image util transform.
* @since_tizen 2.3
*/
typedef struct transformation_s *transformation_h;

/**
* @ingroup CAPI_MEDIA_IMAGE_UTIL_TRANSFORM_MODULE
* @brief Called when transforming an media packet is completed.
* @since_tizen 2.3
*
* @remarks The @a dst should be released using media_packet_destroy(). \n
*          The callback is called in a separate thread(not in the main loop).
*
* @param[in] dst The transformed media packet handle
* @param[in] error_code The error code of image util transform
* @param[in] user_data The user data passed from the callback registration function
*
* @pre image_util_transform_run()
*
*/
typedef void(*image_util_transform_completed_cb)(media_packet_h *dst, int error_code, void *user_data);

/**
* @ingroup CAPI_MEDIA_IMAGE_UTIL_TRANSFORM_MODULE
* @brief Called when transforming an image is completed.
*
* @since_tizen 5.5
*
* @remarks The @a dst can be used only in the callback. To use outside, make a copy using image_util_clone_image(). \n
*          The callback is called in a separate thread(not in the main loop).
*
* @param[in] dst The transformed image
* @param[in] error_code The error code of transformation
* @param[in] user_data The user data passed from the callback registration function
*
* @pre image_util_transform_run2_async()
*
* @see image_util_clone_image()
*/
typedef void(*image_util_transform_completed2_cb)(image_util_image_h dst, int error_code, void *user_data);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_IMAGE_UTIL_ENCODE_DECODE_MODULE
 * @{
 */

/**
 * @brief Enumeration for image types.
 * @since_tizen 3.0
 */
typedef enum {
	IMAGE_UTIL_JPEG,    /**< Image format JPEG */
	IMAGE_UTIL_PNG,     /**< Image format PNG */
	IMAGE_UTIL_GIF,     /**< Image format GIF */
	IMAGE_UTIL_BMP,     /**< Image format BMP */
	IMAGE_UTIL_WEBP,    /**< Image format WEBP (Since 6.0) */
	IMAGE_UTIL_HEIF,    /**< Image format HEIF (Since 6.5) */
	IMAGE_UTIL_JPEG_XL, /**< Image format JPEG-XL (Since 7.0) */
} image_util_type_e;

/**
 * @brief Enumeration for animated image type.
 * @since_tizen 6.0
 */
typedef enum {
	IMAGE_UTIL_ANIM_GIF,	 /**< Image format animated GIF */
	IMAGE_UTIL_ANIM_WEBP,	 /**< Image format animated WEBP */
} image_util_anim_type_e;

/**
 * @brief Enumeration for JPEG downscale decoding.
 * @since_tizen 2.4
 */
typedef enum {
	IMAGE_UTIL_DOWNSCALE_1_1, /**< 1/1 downscale */
	IMAGE_UTIL_DOWNSCALE_1_2,	/**< 1/2 downscale */
	IMAGE_UTIL_DOWNSCALE_1_4,	/**< 1/4 downscale */
	IMAGE_UTIL_DOWNSCALE_1_8,	/**< 1/8 downscale */
} image_util_scale_e;

/**
 * @brief Enumeration for PNG compression values.
 * @since_tizen 3.0
 */
typedef enum {
	IMAGE_UTIL_PNG_COMPRESSION_0 = 0, /**< No compression */
	IMAGE_UTIL_PNG_COMPRESSION_1 = 1, /**< Compression Level 1. Best speed */
	IMAGE_UTIL_PNG_COMPRESSION_2 = 2, /**< Compression Level 2 */
	IMAGE_UTIL_PNG_COMPRESSION_3 = 3, /**< Compression Level 3 */
	IMAGE_UTIL_PNG_COMPRESSION_4 = 4, /**< Compression Level 4 */
	IMAGE_UTIL_PNG_COMPRESSION_5 = 5, /**< Compression Level 5 */
	IMAGE_UTIL_PNG_COMPRESSION_6 = 6, /**< Compression Level 6 */
	IMAGE_UTIL_PNG_COMPRESSION_7 = 7, /**< Compression Level 7 */
	IMAGE_UTIL_PNG_COMPRESSION_8 = 8, /**< Compression Level 8 */
	IMAGE_UTIL_PNG_COMPRESSION_9 = 9  /**< Compression Level 9. Best compression */
} image_util_png_compression_e;

/**
* @ingroup CAPI_MEDIA_IMAGE_UTIL_ENCODE_DECODE_MODULE
* @brief Called when decoding an image is completed.
* @since_tizen 5.5
*
* @remarks The @a image should not be released by the application. \n
*            The @a image can be used only in the callback. To use outside, make a copy using image_util_clone_image(). \n
*            The callback is called in a separate thread(not in the main loop). \n
*
* @param[in] error_code The error code of image util
*                    #IMAGE_UTIL_ERROR_NONE Successful
*                    #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
*                    #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*                    #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*                    #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
*
* @param[in] image The decoded image (or @c NULL if decoding failed)
* @param[in] user_data The user data passed from the callback registration function
*
* @pre image_util_decode_run_async2()
*/
typedef void (*image_util_decode_completed2_cb) (int error_code, image_util_image_h image, void *user_data);

/**
* @ingroup CAPI_MEDIA_IMAGE_UTIL_ENCODE_DECODE_MODULE
* @brief Called when encoding an image is completed just after storing the output to the file.
* @since_tizen 5.5
*
* @remarks The output will be stored in the file set using image_util_encode_run_to_file(). \n
*          The callback is called in a separate thread (not in the main loop).
*
* @param[in] error_code The error code of image util \n
*                    #IMAGE_UTIL_ERROR_NONE Successful \n
*                    #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter \n
*                    #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation \n
*                    #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory \n
*                    #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
* @param[in] user_data The user data passed from the callback registration function
*
* @pre image_util_encode_run_to_file() will invoke this function.
*/
typedef void (*image_util_encode_to_file_completed_cb) (image_util_error_e error_code, void *user_data);

/**
* @ingroup CAPI_MEDIA_IMAGE_UTIL_ENCODE_DECODE_MODULE
* @brief Called when encoding an image is completed just after storing the output to the buffer.
* @since_tizen 5.5
*
* @remarks The output will be stored in the @a buffer. \n
*          The callback is called in a separate thread (not in the main loop).\n
*          The @a buffer should not be released by the application.\n
*          The @a buffer can be used only in the callback. To use outside, make a copy.
*
* @param[in] error_code The error code of image util \n
*                    #IMAGE_UTIL_ERROR_NONE Successful \n
*                    #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter \n
*                    #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation \n
*                    #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory \n
*                    #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
* @param[in] buffer The buffer for the encoded image
* @param[in] buffer_size The size of the buffer for the encoded image
* @param[in] user_data The user data passed from the callback registration function
*
* @pre image_util_encode_run_to_buffer() will invoke this function.
*/
typedef void (*image_util_encode_to_buffer_completed_cb) (image_util_error_e error_code, unsigned char *buffer, size_t buffer_size, void *user_data);

/**
* @ingroup CAPI_MEDIA_IMAGE_UTIL_ENCODE_DECODE_MODULE
* @brief The handle of image util decode.
* @since_tizen 3.0
*/
typedef void *image_util_decode_h;

/**
* @ingroup CAPI_MEDIA_IMAGE_UTIL_ENCODE_DECODE_MODULE
* @brief The handle of image util encode.
* @since_tizen 3.0
*/
typedef void *image_util_encode_h;

/**
* @ingroup CAPI_MEDIA_IMAGE_UTIL_ENCODE_DECODE_MODULE
* @brief The handle of image util AGIF encode.
* @since_tizen 5.5
*/
typedef void *image_util_agif_encode_h;

/**
* @ingroup CAPI_MEDIA_IMAGE_UTIL_ENCODE_DECODE_MODULE
* @brief The handle of image util animation encode.
* @since_tizen 6.0
*/
typedef void *image_util_anim_encode_h;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MULTIMEDIA_IMAGE_UTIL_TYPE_H__ */
