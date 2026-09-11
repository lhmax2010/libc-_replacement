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

#ifndef __TIZEN_MULTIMEDIA_IMAGE_UTIL_H__
#define __TIZEN_MULTIMEDIA_IMAGE_UTIL_H__

#include <image_util_type.h>
#include <image_util_decode.h>
#include <image_util_encode.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file image_util.h
 * @brief This file contains the image util API.
 */

/**
 * @addtogroup CAPI_MEDIA_IMAGE_UTIL_MODULE
 * @{
 */

/**
* @brief Extracts representative color from an image buffer.
* @since_tizen 3.0
* @remarks @a image_buffer should be RGB888 colorspace.
*
* @param[in] image_buffer The original image buffer
* @param[in] width The image width
* @param[in] height The image height
* @param[out] rgb_r The red color in RGB color space
* @param[out] rgb_g The green color in RGB color space
* @param[out] rgb_b The blue color in RGB color space
*
* @return 0 on success,
*           otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
*/
int image_util_extract_color_from_memory(const unsigned char *image_buffer, int width, int height, unsigned char *rgb_r, unsigned char *rgb_g, unsigned char *rgb_b);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_IMAGE_UTIL_TRANSFORM_MODULE
 * @{
 */

/**
* @brief Creates a handle of image util transform.
* @details This function creates a handle of image util transform.
* @since_tizen 2.3
*
* @remarks The @a handle should be released using image_util_transform_destroy().
*
* @param[out] handle The handle of image util transform
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
* @see image_util_transform_destroy()
*
*/
int image_util_transform_create(transformation_h *handle);

/**
* @brief Sets the information of the colorspace to transform.
* @since_tizen 2.3
*
* @param[in] handle The handle for image util transform
* @param[in] colorspace The colorspace to transform
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
* @pre image_util_transform_create()
*
* @see image_util_transform_get_colorspace()
*
*/
int image_util_transform_set_colorspace(transformation_h handle, image_util_colorspace_e colorspace);

/**
* @brief Sets the information of the resolution to transform.
* @since_tizen 2.3
*
* @remarks image_util_transform_set_resolution() and image_util_transform_set_crop_area() can't do that at the same time.
*
* @param[in] handle The handle for image util transform
* @param[in] width The width to transform
* @param[in] height The height to transform
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_transform_create()
*
* @see image_util_transform_get_resolution()
*
*/
int image_util_transform_set_resolution(transformation_h handle, unsigned int width, unsigned int height);

/**
* @brief Sets the information of the rotation to transform.
* @since_tizen 2.3
*
* @param[in] handle The handle of image util transform
* @param[in] rotation The rotation to transform
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_transform_create()
*
* @see image_util_transform_get_rotation()
*
*/
int image_util_transform_set_rotation(transformation_h handle, image_util_rotation_e rotation);

/**
* @brief Sets the information of the crop area to transform.
* @since_tizen 2.3
*
* @remarks image_util_transform_set_resolution() and image_util_transform_set_crop_area() can't do that at the same time.
*
* @param[in] handle The handle of image util transform
* @param[in] start_x The start x position to transform
* @param[in] start_y The start y position to transform
* @param[in] end_x The end x position to transform
* @param[in] end_y The end y position to transform
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_transform_create()
*
* @see image_util_transform_get_crop_area()
*
*/
int image_util_transform_set_crop_area(transformation_h handle, unsigned int start_x, unsigned int start_y, unsigned int end_x, unsigned int end_y);

/**
* @brief Gets the colorspace of the image buffer.
* @since_tizen 2.3
*
* @param[in] handle The handle of image util transform
* @param[in,out] colorspace The colorspace to transform
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
* @pre image_util_transform_create()
*
* @see image_util_transform_set_colorspace()
*
*/
int image_util_transform_get_colorspace(transformation_h handle, image_util_colorspace_e *colorspace);

/**
* @brief Gets the resolution of the image buffer.
* @since_tizen 2.3
*
* @param[in] handle The handle of image util transform
* @param[in,out] width The width to transform
* @param[in,out] height The height to transform
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_transform_create()
*
* @see image_util_transform_set_resolution()
*
*/
int image_util_transform_get_resolution(transformation_h handle, unsigned int *width, unsigned int *height);

/**
* @brief Gets the information of the rotating.
* @since_tizen 2.3
*
* @param[in] handle The handle of image util transform
* @param[in,out] rotation The rotation to transform
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_transform_create().\n
*            image_util_transform_set_rotation().
*
* @see image_util_transform_set_rotation()
*
*/
int image_util_transform_get_rotation(transformation_h handle, image_util_rotation_e *rotation);

/**
* @brief Gets the information of the cropping.
* @since_tizen 2.3
*
* @param[in] handle The handle of image util transform
* @param[in,out] start_x The start x position to transform
* @param[in,out] start_y The start y position to transform
* @param[in,out] end_x The end x position to transform
* @param[in,out] end_y The end y position to transform
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_transform_create().\n
*            image_util_transform_set_crop_area().
*
* @see image_util_transform_set_crop_area()
*
*/
int image_util_transform_get_crop_area(transformation_h handle, unsigned int *start_x, unsigned int *start_y, unsigned int *end_x, unsigned int *end_y);

/**
* @brief Transforms an image using given @a handle.
* @details This function transforms @a src media packet synchronously using the @a handle.
*          If you set more than two transforming, the order of running is crop or resolution, colorspace converting, and rotation.
* @since_tizen 2.3
*
* @param[in] handle The handle of image util transform
* @param[in] src The handle of a source media packet
* @param[in] callback The callback function to be invoked
* @param[in] user_data The user data to be passed to the callback function
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @see image_util_transform_completed_cb()
*
*/
int image_util_transform_run(transformation_h handle, media_packet_h src, image_util_transform_completed_cb callback, void *user_data);

/**
* @brief Synchronously transforms an image using given @a handle.
* @details This function transforms @a src image synchronously using the @a handle.\n
*            A transformed image is stored into the @a dst image handle.\n
* @since_tizen 5.5
*
* @remarks If transforming is failed, the @a dst will be null.
*               The @a dst should be released using image_util_destroy_image().
*
* @param[in] handle The handle of image util transform
* @param[in] src The handle of a source image
* @param[out] dst The transformed image
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
* @pre Create a transformation handle by calling image_util_transform_create().
* @pre Set the information to transform by calling image_util_transform_set_colorspace(), image_util_transform_set_resolution(), \n
*         image_util_transform_set_rotation(), image_util_transform_set_crop_area().
*
*/
int image_util_transform_run2(transformation_h handle, image_util_image_h src, image_util_image_h *dst);

/**
* @brief Asynchronously transforms an image with the given transformation handle.
* @details This function transforms @a src image asynchronously using the @a handle.
*            @a callback will be invoked after transforming is completed.
* @since_tizen 5.5
*
* @remarks If transforming is failed, the dst of callback parameter will be null.
*
* @param[in] handle The handle of image util transform
* @param[in] src The handle of a source image
* @param[in] callback The callback function to be invoked
* @param[in] user_data The user data to be passed to the callback function
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
* @pre Create a transformation handle by calling image_util_transform_create().
* @pre Set the transformation information by calling image_util_transform_set_colorspace(), image_util_transform_set_resolution(), \n
*         image_util_transform_set_rotation(), image_util_transform_set_crop_area().
*
* @see image_util_transform_completed2_cb()
*
*/
int image_util_transform_run2_async(transformation_h handle, image_util_image_h src, image_util_transform_completed2_cb callback, void *user_data);

/**
* @brief Destroys a handle to image util.
* @details The function frees all resources related to the image util handle.
*               The image util handle no longer can be used to perform any operations.
*               A new image util handle has to be created before the next usage.
* @since_tizen 2.3
*
* @param[in] handle The handle of image util transform
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
*
* @see image_util_transform_create()
*
*/
int image_util_transform_destroy(transformation_h handle);

/**
* @brief Creates an image handle.
* @since_tizen 5.5
*
* @remarks The @a image should be released using image_util_destroy_image().
*
* @param[in] width		The width of image
* @param[in] height		The height of image
* @param[in] colorspace	The colorspace of image
* @param[in] data		The data of image
* @param[in] data_size	The size of data
* @param[out] image		The handle of image
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
* @see image_util_destroy_image()
*
*/
int image_util_create_image(unsigned int width, unsigned int height, image_util_colorspace_e colorspace, const unsigned char *data, size_t data_size, image_util_image_h *image);

/**
* @brief Creates an image handle from media packet.
* @since_tizen 7.0
*
* @remarks The @a image should be released using image_util_destroy_image().
*
* @param[in] media_packet		The media_packet
* @param[out] image		A handle of image
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
* @see image_util_destroy_image()
*
*/
int image_util_create_image_from_media_packet(media_packet_h media_packet, image_util_image_h *image);

/**
* @brief Clones an image handle.
* @since_tizen 5.5
*
* @remarks The @a dst should be released using image_util_destroy_image().
*
* @param[in] src		The handle of the image to clone
* @param[out] dst		The handle of new image
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
* @see image_util_destroy_image()
*
*/
int image_util_clone_image(image_util_image_h src, image_util_image_h *dst);

/**
* @brief Gets the information from the image.
* @details The function get the information from the image.
*          If you don't want to get specific information, you can pass unwanted parameters to NULL.
* @since_tizen 5.5
*
* @remarks The @a data should be released using free() if that's not NULL.
*
* @param[in] image		The handle of the image to get the information
* @param[out] width		The width of the image
* @param[out] height		The height of the image
* @param[out] colorspace	The colorspace of the image
* @param[out] data		The data of image
* @param[out] data_size	The size of data
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
*/
int image_util_get_image(image_util_image_h image, unsigned int *width, unsigned int *height, image_util_colorspace_e *colorspace, unsigned char **data, size_t *data_size);

/**
* @brief Destroys an image handle.
* @details The function frees all resources related to the @a image. The image handle no longer can be used to perform any operations.
*               A new image handle has to be created before the next usage.
* @since_tizen 5.5
*
* @param[in] image The handle of the image to destroy
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
*
* @see image_util_create_image()
*
*/
int image_util_destroy_image(image_util_image_h image);

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_IMAGE_UTIL_ENCODE_DECODE_MODULE
* @{
*/

/**
* @brief Called once for each supported image encode/decode colorspace.
* @since_tizen 3.0
*
* @param[in] colorspace The colorspace
* @param[in] user_data The user data passed from the foreach function
*
* @return @c true to continue with the next iteration of the loop, \n
*         otherwise @c false to break out of the loop
*
* @see image_util_foreach_supported_colorspace()
*
*/
typedef bool(*image_util_supported_colorspace_cb)(image_util_colorspace_e colorspace, void *user_data);

/**
* @brief Retrieves all supported colorspace for image encoding/decoding.
* @details Retrieves all supported colorspace by invoking a callback function for each colorspace
* @since_tizen 3.0
*
* @param[in] image_type The supported colorspace of @a image_type for which to create encode/decode handle
* @param[in] callback The callback function to be invoked
* @param[in] user_data The user data to be passed to the callback function
* @return 0 on success,
*         otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
*
* @see image_util_supported_colorspace_cb()
*
*/
int image_util_foreach_supported_colorspace(image_util_type_e image_type, image_util_supported_colorspace_cb callback, void *user_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MULTIMEDIA_IMAGE_UTIL_H__ */
