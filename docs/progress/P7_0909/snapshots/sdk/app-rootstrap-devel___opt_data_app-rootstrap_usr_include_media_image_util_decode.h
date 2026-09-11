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

#ifndef __TIZEN_MULTIMEDIA_IMAGE_UTIL_DECODE_H__
#define __TIZEN_MULTIMEDIA_IMAGE_UTIL_DECODE_H__

#include <image_util_type.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
* @addtogroup CAPI_MEDIA_IMAGE_UTIL_ENCODE_DECODE_MODULE
* @{
*/

/**
* @brief Creates a handle of image util decoding.
* @details This function creates a handle of image util decoding.
* @since_tizen 3.0
*
* @remarks The @a handle should be released using image_util_decode_destroy().
*
* @param[out] handle The handle of image util decode
*
* @return @c 0 on success,
*             otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
* @see image_util_decode_destroy()
*
*/
int image_util_decode_create(image_util_decode_h *handle);

/**
* @brief Sets the input file path from which to decode.
* @since_tizen 3.0
*
* @remarks One of image_util_decode_set_input_path() or image_util_decode_set_input_buffer() should be set.\n
*             If both are set then the latest input set, is considered.\n
*             %http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.\n
*             %http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage.\n
*             Finds out image type by reading the header of the image provided in input path.\n
*             Since 6.0, this module supports WEBP image format.\n
*             Since 6.5, this module supports HEIF image format.\n
*             Since 7.0, this module supports JPEG-XL image format.
*
* @param[in] handle The handle of image util decode
* @param[in] path The path to input image
*
* @return @c 0 on success,
*            otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
* @retval #IMAGE_UTIL_ERROR_NO_SUCH_FILE No such file
*
*/
int image_util_decode_set_input_path(image_util_decode_h handle, const char *path);

/**
* @brief Sets the input buffer from which to decode.
* @since_tizen 3.0
*
* @remarks One of image_util_decode_set_input_path() or image_util_decode_set_input_buffer() should be set.\n
*              If both are set then the latest input set, is considered.\n
*              Finds out image type by reading the header of the image provided in input buffer.\n
*              Since 6.0, this module supports WEBP image format.\n
*              Since 6.5, this module supports HEIF image format.\n
*              Since 7.0, this module supports JPEG-XL image format.
*
* @param[in] handle The handle of image util decode
* @param[in] src_buffer The input image buffer
* @param[in] src_size The input image buffer size
*
* @return @c 0 on success,
*             otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
*/
int image_util_decode_set_input_buffer(image_util_decode_h handle, const unsigned char *src_buffer, unsigned long long src_size);

/**
* @brief Sets the decoded image colorspace format.
* @since_tizen 3.0
*
* @remarks The default colorspace is #IMAGE_UTIL_COLORSPACE_RGBA8888.\n
*             Use image_util_foreach_supported_colorspace to get supported colorspaces for each image format.\n
*             Errors would be returned if not supported.
*
* @param[in] handle The handle of image util decode
* @param[in] colorspace The decoded image colorspace
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
*/
int image_util_decode_set_colorspace(image_util_decode_h handle, image_util_colorspace_e colorspace);

/**
* @brief Sets the downscale value at which JPEG image should be decoded.
* @since_tizen 3.0
*
* @remarks This is API is supported only for JPEG decoding.
*
* @param[in] handle The handle of image util decode
* @param[in] down_scale The downscale at which image is to be decoded
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
*/
int image_util_decode_set_jpeg_downscale(image_util_decode_h handle, image_util_scale_e down_scale);

/**
* @brief Decodes the image with the given decode handle.
* @details This function decodes the image synchronously.
* @since_tizen 5.5
*
* @remarks If the decoding fails, the @a image will be @c NULL.\n
*          The @a image should be released using image_util_destroy_image().\n
*          If any of the required functions listed in the preconditions section has not been called, #IMAGE_UTIL_ERROR_INVALID_PARAMETER is returned.
*
* @param[in] handle The handle of image util decode
* @param[out] image The decoded image
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
*/
int image_util_decode_run2(image_util_decode_h handle, image_util_image_h *image);

/**
* @brief Starts decoding of the image with the given decode handle.
* @details This function decodes the image asynchronously.
* @since_tizen 5.5
*
* @remarks If the decoding fails, the image_util_decode_completed2_cb() will be called with @c NULL image and a non-zero error_code.\n
*          If any of the required functions listed in the preconditions section has not been called, #IMAGE_UTIL_ERROR_INVALID_PARAMETER is returned.
*
* @param[in] handle The handle of image util decode
* @param[in] callback The callback function to be invoked
* @param[in] user_data The user data to be passed to the callback function
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
* @see image_util_decode_completed2_cb()
*/
int image_util_decode_run_async2(image_util_decode_h handle, image_util_decode_completed2_cb callback, void *user_data);

/**
* @brief Destroys the image decoding handle.
* @since_tizen 3.0
*
* @remarks Any image handle created should be destroyed.
*
* @param[in] handle The handle of image util decode
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_decode_create()
*
* @see image_util_decode_create()
*
*/
int image_util_decode_destroy(image_util_decode_h handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MULTIMEDIA_IMAGE_UTIL_DECODE_H__ */
