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

#ifndef __TIZEN_MULTIMEDIA_IMAGE_UTIL_ENCODE_H__
#define __TIZEN_MULTIMEDIA_IMAGE_UTIL_ENCODE_H__

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
* @brief Creates a handle of image util encode.
* @details This function creates a handle of image util encode.
* @since_tizen 3.0
*
* @remarks The @a handle should be released using image_util_encode_destroy().
*
* @param[in] image_type The type of output image for which to create encode handle.
* @param[out] handle The handle of image util encode
*
* @return @c 0 on success,
*            otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
* @see image_util_encode_destroy()
*
*/
int image_util_encode_create(image_util_type_e image_type, image_util_encode_h *handle);

/**
* @brief Sets the quality for encoding image.
* @since_tizen 3.0
*
* @remarks If application does not set this, then by default quality of 75 is set.\n
*              Quality is supported for JPEG format. #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT will be returned for other formats.
*
* @param[in] handle The handle of image util encode
* @param[in] quality Encoding quality from 1~100
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
*/
int image_util_encode_set_quality(image_util_encode_h handle, int quality);

/**
* @brief Sets the compression value of PNG image encoding(0~9).
* @since_tizen 3.0
*
* @remarks If application does not set this, then the default value is #IMAGE_UTIL_PNG_COMPRESSION_6.
*
* @param[in] handle The handle of image util encode
* @param[in] compression The compression value valid from 0~9
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
*/
int image_util_encode_set_png_compression(image_util_encode_h handle, image_util_png_compression_e compression);

/**
* @brief Sets lossless for #IMAGE_UTIL_WEBP and #IMAGE_UTIL_JPEG_XL.
* @details This function sets lossless compression of WEBP and JPEG-XL image format.
* @since_tizen 7.0
*
* @remarks If application does not set this, then the default value is @c false.
*
* @param[in] handle The handle of image util encode
* @param[in] lossless The flag determining whether the compression is lossless or lossy: @c true for lossless, @c false for lossy
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
*/
int image_util_encode_set_lossless(image_util_encode_h handle, bool lossless);

/**
* @brief Encodes an @a image using given @a handle synchronously.
* @details This function encodes an @a image and stores it in @a file_path.
* @since_tizen 5.5
*
* @remarks If any functions at the pre-condition are not called first, #IMAGE_UTIL_ERROR_INVALID_PARAMETER is returned.\n
*          The only supported colorspace for BMP and GIF is #IMAGE_UTIL_COLORSPACE_RGBA8888.\n
*          To get supported colorspaces for JPEG format, you can use image_util_foreach_supported_colorspace().\n
*          %http://tizen.org/privilege/mediastorage is required if @a file_path value is media storage.\n
*          %http://tizen.org/privilege/externalstorage is required if @a file_path value is external storage.\n
*          Before 6.0, #IMAGE_UTIL_COLORSPACE_RGBA8888 was the only supported colorspace for PNG. But since 6.0, this module supports more colorspaces for PNG.
*          To get supported colorspaces for PNG, you can use image_util_foreach_supported_colorspace().\n
*          Since 6.0, this module supports WEBP, you can use image_util_foreach_supported_colorspace() to get supported colorspaces for WEBP.\n
*          Since 7.0, this module supports JPEG-XL, you can use image_util_foreach_supported_colorspace() to get supported colorspaces for JPEG-XL.
*
* @param[in] handle The handle of image util encode
* @param[in] image The image handle for encoding
* @param[in] file_path The file path for encoding image
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED Permission denied
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
*/
int image_util_encode_run_to_file(image_util_encode_h handle, image_util_image_h image, const char *file_path);

/**
* @brief Encodes an @a image using given @a handle synchronously.
* @details This function encodes an @a image and stores it in @a buffer.
* @since_tizen 5.5
*
* @remarks If any functions at the pre-condition are not called first, #IMAGE_UTIL_ERROR_INVALID_PARAMETER is returned.\n
*          The @a buffer should be released using free().\n
*          The only supported colorspace for BMP and GIF is #IMAGE_UTIL_COLORSPACE_RGBA8888.\n
*          To get supported colorspaces for JPEG format, you can use image_util_foreach_supported_colorspace().\n
*          Before 6.0, #IMAGE_UTIL_COLORSPACE_RGBA8888 was the only supported colorspace for PNG. But since 6.0, this module supports more colorspaces for PNG.
*          To get supported colorspaces for PNG, you can use image_util_foreach_supported_colorspace().\n
*          Since 6.0, this module supports WEBP, you can use image_util_foreach_supported_colorspace() to get supported colorspaces for WEBP.\n
*          Since 7.0, this module supports JPEG-XL, you can use image_util_foreach_supported_colorspace() to get supported colorspaces for JPEG-XL.
*
* @param[in] handle The handle of image util encode
* @param[in] image The image handle for encoding
* @param[out] buffer The buffer that encoded image is stored
* @param[out] buffer_size The size of the buffer
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
int image_util_encode_run_to_buffer(image_util_encode_h handle, image_util_image_h image, unsigned char **buffer, size_t *buffer_size);

/**
* @brief Encodes an @a image using given @a handle asynchronously.
* @details The output will be stored in @a file_path. And the function executes asynchronously.
* @since_tizen 5.5
*
* @remarks If any functions at the pre-condition are not called first, #IMAGE_UTIL_ERROR_INVALID_PARAMETER is returned.\n
*          The only supported colorspace for BMP and GIF is #IMAGE_UTIL_COLORSPACE_RGBA8888.\n
*          To get supported colorspaces for JPEG format, you can use image_util_foreach_supported_colorspace().\n
*          %http://tizen.org/privilege/mediastorage is required if @a file_path value is media storage.\n
*          %http://tizen.org/privilege/externalstorage is required if @a file_path value is external storage.\n
*          Before 6.0, #IMAGE_UTIL_COLORSPACE_RGBA8888 was the only supported colorspace for PNG. But since 6.0, this module supports more colorspaces for PNG.
*          To get supported colorspaces for PNG, you can use image_util_foreach_supported_colorspace().\n
*          Since 6.0, this module supports WEBP, you can use image_util_foreach_supported_colorspace() to get supported colorspaces for WEBP.\n
*          Since 7.0, this module supports JPEG-XL, you can use image_util_foreach_supported_colorspace() to get supported colorspaces for JPEG-XL.
*
* @param[in] handle The handle of image util encode
* @param[in] image The image handle for encoding
* @param[in] file_path The file path for encoding image
* @param[in] completed_cb The callback function to be invoked
* @param[in] user_data The user data to be passed to the callback function
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED Permission denied
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
* @see image_util_encode_to_file_completed_cb()
*
*/
int image_util_encode_run_async_to_file(image_util_encode_h handle, image_util_image_h image, const char *file_path, image_util_encode_to_file_completed_cb completed_cb, void *user_data);

/**
* @brief Encodes an @a image using given @a handle asynchronously.
* @details The output will be stored in a buffer provided by the @a completed_cb callback.
* @since_tizen 5.5
*
* @remarks If any functions at the pre-condition are not called first, #IMAGE_UTIL_ERROR_INVALID_PARAMETER is returned.\n
*          The only supported colorspace for BMP and GIF is #IMAGE_UTIL_COLORSPACE_RGBA8888.\n
*          To get supported colorspaces for JPEG format, you can use image_util_foreach_supported_colorspace().\n
*          Before 6.0, #IMAGE_UTIL_COLORSPACE_RGBA8888 was the only supported colorspace for PNG. But since 6.0, this module supports more colorspaces for PNG.
*          To get supported colorspaces for PNG, you can use image_util_foreach_supported_colorspace().\n
*          Since 6.0, this module supports WEBP, you can use image_util_foreach_supported_colorspace() to get supported colorspaces for WEBP.\n
*          Since 7.0, this module supports JPEG-XL, you can use image_util_foreach_supported_colorspace() to get supported colorspaces for JPEG-XL.
*
* @param[in] handle The handle of image util encode
* @param[in] image The image handle for encoding
* @param[in] completed_cb The callback function to be invoked
* @param[in] user_data The user data to be passed to the callback function
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
* @see image_util_encode_to_buffer_completed_cb()
*
*/
int image_util_encode_run_async_to_buffer(image_util_encode_h handle, image_util_image_h image, image_util_encode_to_buffer_completed_cb completed_cb, void *user_data);

/**
* @brief Destroys the image encoding handle.
* @since_tizen 3.0
*
* @remarks Any image handle created should be destroyed.
*
* @param[in] handle The handle of image util encode
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_encode_create()
*
* @see image_util_encode_create()
*
*/
int image_util_encode_destroy(image_util_encode_h handle);

/**
* @brief Creates a handle for encoding an animated GIF.
* @since_tizen 5.5
*
* @remarks The @a handle should be released using image_util_agif_encode_destroy().
*
* @param[out] handle The handle of encoding an animated GIF
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
* @see image_util_agif_encode_destroy()
*
*/
int image_util_agif_encode_create(image_util_agif_encode_h *handle);

/**
* @brief Encodes an image and adds the encoded image to the frames of the animated GIF.
* @since_tizen 5.5
*
* @remarks This function should be called for each @a image which you want to add to the animated GIF. Each @a image should be the same size.\n
*          The supported colorspace is #IMAGE_UTIL_COLORSPACE_RGBA8888.\n
*          You should call image_util_agif_encode_save_to_file() or image_util_agif_encode_save_to_buffer() to save the animated GIF.\n
*          If you call this function after image_util_agif_encode_save_to_file() or image_util_agif_encode_save_to_buffer() function is called,
*          this function will encode a new animated GIF.
*
* @param[in] handle The handle of encoding an animated GIF
* @param[in] image The handle of the image for each frame
* @param[in] time_delay The time delay between @a image and the next image (in 0.01sec increments)
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
* @see image_util_agif_encode_save_to_file()
* @see image_util_agif_encode_save_to_buffer()
*
*/
int image_util_agif_encode_add_frame(image_util_agif_encode_h handle, image_util_image_h image, unsigned int time_delay);

/**
* @brief Saves the animated GIF image to the file.
* @details After the data has been written to a file,
*          the file cannot be modified. In other words,\n
*          it is not possible to add frames to the file.\n
*          Saving animation flushes the frames that has been added to the encoder.\n
*          However, the encoder can be still used after the data is written.\n
*          For example, although you write the data to one file(B0),\n
*          you can do the following operations:\n
*
*          1. Add frames 1-10 to the encoder.\n
*          2. Save the data in the encoder to another file(B1). 10 frames will be saved.\n
*          3. Add more frames (11-20) to the same encoder.\n
*          4. Save the data in the encoder to the other file (B2)\n
*             It is not possible to save the changed data to B1 file used in step 4.
* @since_tizen 5.5
*
* @remarks %http://tizen.org/privilege/mediastorage is required if @a file_path value is media storage.\n
*          %http://tizen.org/privilege/externalstorage is required if @a file_path value is external storage.
*
* @param[in] handle The handle of encoding an animated GIF
* @param[in] file_path The file path for saving the animated GIF
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED Permission denied
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
* @retval #IMAGE_UTIL_ERROR_NO_SUCH_FILE No such file
*
* @see image_util_agif_encode_add_frame()
*
*/
int image_util_agif_encode_save_to_file(image_util_agif_encode_h handle, const char *file_path);

/**
* @brief Saves the animated GIF image to the buffer.
* @details After the data has been written to a buffer,
*          the file cannot be modified. In other words,\n
*          it is not possible to add frames to the buffer.\n
*          Saving animation flushes the frames that has been added to the encoder.\n
*          However, the encoder can be still used after the data is written.\n
*          For example, although you write the data to one buffer(B0),\n
*          you can do the following operations:\n
*
*          1. Add frames 1-10 to the encoder.\n
*          2. Save the data in the encoder to another buffer(B1). 10 frames will be saved.\n
*          3. Add more frames (11-20) to the same encoder.\n
*          4. Save the data in the encoder to the other buffer (B2)\n
*             It is not possible to save the changed data to B1 file used in step 4.
* @since_tizen 5.5
*
* @remarks The @a buffer should be released using free() after using it.
*
* @param[in] handle The handle of encoding an animated GIF
* @param[out] buffer The buffer in which the animated GIF is saved
* @param[out] buffer_size The size of the buffer
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
* @see image_util_agif_encode_add_frame()
*
*/
int image_util_agif_encode_save_to_buffer(image_util_agif_encode_h handle, unsigned char **buffer, size_t *buffer_size);

/**
* @brief Destroys the handle for encoding an animated GIF.
* @since_tizen 5.5
*
* @remarks Any created animated GIF encoding handle created should be destroyed.
*
* @param[in] handle The handle of encoding an animated GIF
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
*
* @pre image_util_agif_encode_create()
*
* @see image_util_agif_encode_create()
*
*/
int image_util_agif_encode_destroy(image_util_agif_encode_h handle);

/**
* @brief Creates a handle for encoding animation.
* @since_tizen 6.0
*
* @remarks The @a handle should be released using image_util_anim_encode_destroy().
*
* @param[in] image_type The type of output image to create animation
* @param[out] handle The handle of animation encoder
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
* @see image_util_anim_encode_destroy()
*
*/
int image_util_anim_encode_create(image_util_anim_type_e image_type, image_util_anim_encode_h *handle);

/**
* @brief Sets a number of times to repeat the animation.
* @since_tizen 6.0
*
* @remarks This function should be called before image_util_anim_encode_add_frame() is called.
*          If not set, the default value is infinite repetition.
*
* @param[in] handle The handle of animation encoder
* @param[in] loop_count The number of times to repeat the animation
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
*/
int image_util_anim_encode_set_loop_count(image_util_anim_encode_h handle, unsigned int loop_count);

/**
* @brief Sets a background color of the animation.
* @since_tizen 6.0
*
* @remarks This function should be called before image_util_anim_encode_add_frame() is called.
*
* @param[in] handle The handle of animation encoder
* @param[in] r The red color of the background
* @param[in] g The green color of the background
* @param[in] b The blue color of the background
* @param[in] a The alpha color of the background
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
*/
int image_util_anim_encode_set_background_color(image_util_anim_encode_h handle, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

/**
* @brief Sets lossless compression of animation.
* @since_tizen 6.0
*
* @remarks This function should be called before image_util_anim_encode_add_frame() is called.
*
* @param[in] handle The handle of animation encoder
* @param[in] lossless The flag determining whether the compression is lossless or lossy: true for lossless, false for lossy
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
*/
int image_util_anim_encode_set_lossless(image_util_anim_encode_h handle, bool lossless);

/**
* @brief Encodes an image and adds image to the frame of the animated image.
* @since_tizen 6.0
*
* @remarks This function should be called for each @a image which you want to add to the animated WEBP or GIF. Each @a image should be the same size.\n
*          The supported colorspaces are #IMAGE_UTIL_COLORSPACE_RGBA8888, #IMAGE_UTIL_COLORSPACE_ARGB8888, #IMAGE_UTIL_COLORSPACE_BGRA8888 and #IMAGE_UTIL_COLORSPACE_RGBA8888 for animated WEBP.\n
*          The supported colorspace is #IMAGE_UTIL_COLORSPACE_RGBA8888 for animated GIF.\n
*          You should call image_util_anim_encode_save_to_file() or image_util_anim_encode_save_to_buffer() to save the animated WEBP.\n
*          If you call this function after image_util_anim_encode_save_to_file() or image_util_anim_encode_save_to_buffer() function is called,
*          this function will encode a new animated WEBP and GIF.
*
* @param[in] handle The handle of animation encoder
* @param[in] image The handle of the image for each frame
* @param[in] time_delay The time delay between @a image and the next image (in 1ms increments for WEBP and 10ms increments for GIF)
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
* @see image_util_anim_encode_save_to_file()
* @see image_util_anim_encode_save_to_buffer()
*
*/
int image_util_anim_encode_add_frame(image_util_anim_encode_h handle, image_util_image_h image, unsigned int time_delay);

/**
* @brief Saves the animated WEBP or GIF image to the file.
* @details After the data has been written to a file,
*          the file cannot be modified. In other words,\n
*          it is not possible to add frames to the file.\n
*          Saving animation flushes the frames that has been added to the encoder.\n
*          However, the encoder can be still used after the data is written.\n
*          For example, although you write the data to one file(B0),\n
*          you can do the following operations:\n
*
*          1. Add frames 1-10 to the encoder.\n
*          2. Save the data in the encoder to another file(B1). 10 frames will be saved.\n
*          3. Add more frames (11-20) to the same encoder.\n
*          4. Save the data in the encoder to the other file (B2)\n
*             It is not possible to save the changed data to B1 file used in step 4.
* @since_tizen 6.0
*
* @remarks %http://tizen.org/privilege/mediastorage is required if @a file_path value is media storage.\n
*          %http://tizen.org/privilege/externalstorage is required if @a file_path value is external storage.
*
* @param[in] handle The handle of encoding an animated WEBP or GIF
* @param[in] file_path The file path for saving the animated WEBP or GIF
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED Permission denied
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_NO_SUCH_FILE No such file
*
* @pre image_util_anim_encode_add_frame()
*
* @see image_util_anim_encode_add_frame()
*
*/
int image_util_anim_encode_save_to_file(image_util_anim_encode_h handle, const char *file_path);

/**
* @brief Saves the animation WEBP or GIF image to the buffer.
* @details After the data has been written to a buffer,
*          the file cannot be modified. In other words,\n
*          it is not possible to add frames to the buffer.\n
*          Saving animation flushes the frames that has been added to the encoder.\n
*          However, the encoder can be still used after the data is written.\n
*          For example, although you write the data to one buffer(B0),\n
*          you can do the following operations:\n
*
*          1. Add frames 1-10 to the encoder.\n
*          2. Save the data in the encoder to another buffer(B1). 10 frames will be saved.\n
*          3. Add more frames (11-20) to the same encoder.\n
*          4. Save the data in the encoder to the other buffer (B2)\n
*             It is not possible to save the changed data to B1 file used in step 4.
* @since_tizen 6.0
*
* @remarks The @a buffer should be released using free() after using it.
*
* @param[in] handle The handle of encoding an animated WEBP or GIF
* @param[out] buffer The buffer in which the animated WEBP or GIF is saved
* @param[out] buffer_size The size of the buffer
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_anim_encode_add_frame()
*
* @see image_util_anim_encode_add_frame()
*
*/
int image_util_anim_encode_save_to_buffer(image_util_anim_encode_h handle, unsigned char **buffer, size_t *buffer_size);

/**
* @brief Destroys the handle for encoding animation.
* @since_tizen 6.0
*
* @remarks Any created animated encode handle should be destroyed.
*
* @param[in] handle The handle of animation encoder
*
* @return @c 0 on success,
*                 otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
*
* @pre image_util_anim_encode_create()
*
* @see image_util_anim_encode_create()
*
*/
int image_util_anim_encode_destroy(image_util_anim_encode_h handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MULTIMEDIA_IMAGE_UTIL_ENCODE_H__ */
