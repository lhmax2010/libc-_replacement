/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MEDIAVISION_SELFIE_SEGMENTATION_INTERNAL_H__
#define __TIZEN_MEDIAVISION_SELFIE_SEGMENTATION_INTERNAL_H__

#include <mv_common.h>
#include <mv_selfie_segmentation_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file   mv_selfie_segmentation_internal.h
 * @internal
 * @brief  This file contains the Inference based Media Vision API.
 */

/**
 * @addtogroup CAPI_MEDIA_VISION_INFERENCE_MODULE
 * @{
 */

/**
 * @internal
 * @brief Creates a inference handle for selfie segmentation object.
 * @details Use this function to create a inference handle. After the creation
 *          the selfie segmentation task has to be prepared with
 *          mv_selfie_segmentation_prepare() function to prepare a network
 *          for the inference.
 *
 * @since_tizen 9.0
 *
 * @remarks The @a handle should be released using mv_selfie_segmentation_destroy().
 *
 * @param[out] handle    The handle to the inference to be created.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INTERNAL Internal Error
 *
 * @see mv_selfie_segmentation_destroy()
 * @see mv_selfie_segmentation_prepare()
 */
int mv_selfie_segmentation_create(mv_selfie_segmentation_h *handle);

/**
 * @internal
 * @brief Destroys inference handle and releases all its resources.
 *
 * @since_tizen 9.0
 *
 * @param[in] handle    The handle to the inference to be destroyed.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create inference handle by using mv_selfie_segmentation_create()
 *
 * @see mv_selfie_segmentation_create()
 */
int mv_selfie_segmentation_destroy(mv_selfie_segmentation_h handle);

/**
 * @internal
 * @brief Set user-given model information.
 * @details Use this function to change the model information instead of default one after calling @ref mv_selfie_segmentation_create().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle        The handle to the selfie segmentation object.
 * @param[in] model_file    Model file name.
 * @param[in] meta_file     Model meta file name.
 * @param[in] label_file    Label file name.
 * @param[in] model_name    Model name.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a selfie segmentation handle by calling @ref mv_selfie_segmentation_create()
 */
int mv_selfie_segmentation_set_model(mv_selfie_segmentation_h handle, const char *model_file, const char *meta_file,
									 const char *label_file, const char *model_name);

/**
 * @internal
 * @brief Configures the backend for the selfie segmentation inference.
 *
 * @since_tizen 9.0
 *
 * @param [in] handle         The handle to the inference
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 */
int mv_selfie_segmentation_configure(mv_selfie_segmentation_h handle);

/**
 * @internal
 * @brief Prepares the selfie segmentation inference
 * @details Use this function to prepare the selfie segmentation inference based on
 *          the configured network.
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the inference.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_DATA Invalid model data
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Not supported format
 */
int mv_selfie_segmentation_prepare(mv_selfie_segmentation_h handle);

/**
 * @internal
 * @brief Performs the selfie segmentation inference on the @a source.
 *
 * @since_tizen 9.0
 * @remarks This function is synchronous and may take considerable time to run.
 *
 * @param[in] source         The handle to the source of the media
 * @param[in] handle          The handle to the inference
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Source colorspace
 *                                                  isn't supported
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_object_detect_create()
 * @pre Prepare an inference by calling mv_object_detect_configure()
 * @pre Prepare an inference by calling mv_object_detect_prepare()
 */
int mv_selfie_segmentation_inference(mv_selfie_segmentation_h handle, mv_source_h source);

/**
 * @internal
 * @brief Performs asynchronously the selfie segmentation inference on the @a source.
 *
 * @since_tizen 9.0
 * @remarks This function operates asynchronously, so it returns immediately upon invocation.
 *          The inference results are inserted into the outgoing queue within the framework
 *          in the order of processing, and the results can be obtained through mv_selfie_segmentation_get_result()
 *          and mv_selfie_segmentation_get_label().
 *
 * @param[in] handle         The handle to the inference
 * @param[in] source         The handle to the source of the media
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Source colorspace
 *                                                  isn't supported
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_object_detect_create()
 * @pre Prepare an inference by calling mv_object_detect_configure()
 * @pre Prepare an inference by calling mv_object_detect_prepare()
 */
int mv_selfie_segmentation_inference_async(mv_selfie_segmentation_h handle, mv_source_h source);

/**
 * @internal
 * @brief Gets the selfie segmentation inference result on the @a source.
 *
 * @since_tizen 9.0
 *
 * @param[in] infer         The handle to the inference
 * @param[out] width        Width size of output image.
 * @param[out] height       Height size of output image.
 * @param[out] pixel_size   Pixel size of output image in bytes.
 * @param[out] data         A pointer to output image data.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_object_detect_create()
 * @pre Prepare an inference by calling mv_object_detect_configure()
 * @pre Prepare an inference by calling mv_object_detect_prepare()
 * @pre Prepare an inference by calling mv_object_detect_inference()
 */
int mv_selfie_segmentation_get_result(mv_selfie_segmentation_h handle, unsigned int *width, unsigned int *height,
									  unsigned int *pixel_size, const unsigned char **data);

/**
 * @internal
 * @brief Set user-given inference engine and device types for inference.
 * @details Use this function to change the inference engine and device types for inference instead of default ones after calling @ref mv_selfie_segmentation_create().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle        The handle to the selfie segmentation object.
 * @param[in] engine_type  A string of inference engine type.
 * @param[in] device_type   A string of device type.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a selfie segmentation handle by calling @ref mv_selfie_segmentation_create()
 */
int mv_selfie_segmentation_set_engine(mv_selfie_segmentation_h handle, const char *engine_type,
									  const char *device_type);

/**
 * @internal
 * @brief Get a number of inference engines available for selfie segmentation task API.
 * @details Use this function to get how many inference engines are supported for selfie segmentation after calling @ref mv_selfie_segmentation_create().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the selfie segmentation object.
 * @param[out] engine_count  A number of inference engines available for selfie segmentation API.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a selfie segmentation handle by calling @ref mv_selfie_segmentation_create()
 */
int mv_selfie_segmentation_get_engine_count(mv_selfie_segmentation_h handle, unsigned int *engine_count);

/**
 * @internal
 * @brief Get engine type to a given inference engine index.
 * @details Use this function to get inference engine type with a given engine index after calling @ref mv_selfie_segmentation_get_engine_count().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle        The handle to the selfie segmentation object.
 * @param[in] engine_index  A inference engine index for getting the inference engine type.
 * @param[out] engine_type  A string to inference engine.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Get a number of inference engines available for selfie segmentation task API by calling @ref mv_selfie_segmentation_get_engine_count()
 */
int mv_selfie_segmentation_get_engine_type(mv_selfie_segmentation_h handle, const unsigned int engine_index,
										   char **engine_type);

/**
 * @internal
 * @brief Get a number of device types available to a given inference engine.
 * @details Use this function to get how many device types are supported for a given inference engine after calling @ref mv_selfie_segmentation_create().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the selfie segmentation object.
 * @param[in] engine_type    A inference engine string.
 * @param[out] device_count  A number of device types available for a given inference engine.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a selfie segmentation handle by calling @ref mv_selfie_segmentation_create()
 */
int mv_selfie_segmentation_get_device_count(mv_selfie_segmentation_h handle, const char *engine_type,
											unsigned int *device_count);

/**
 * @internal
 * @brief Get device type list available.
 * @details Use this function to get what device types are supported for current inference engine type after calling @ref mv_selfie_segmentation_configure().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the selfie segmentation object.
 * @param[in] engine_type    A inference engine string.
 * @param[in] device_index   A device index for getting the device type.
 * @param[out] device_type   A string to device type.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a selfie segmentation handle by calling @ref mv_selfie_segmentation_create()
 * @pre Configure selfie segmentation task by calling @ref mv_selfie_segmentation_configure()
 */
int mv_selfie_segmentation_get_device_type(mv_selfie_segmentation_h handle, const char *engine_type,
										   const unsigned int device_index, char **device_type);
/**
 * @}
 */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_SELFIE_SEGMENTATION_INTERNAL_H__ */
