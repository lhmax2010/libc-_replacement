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

#ifndef __TIZEN_MEDIAVISION_OBJECT_DETECT_3D_INTERNAL_H__
#define __TIZEN_MEDIAVISION_OBJECT_DETECT_3D_INTERNAL_H__

#include <mv_common.h>
#include <mv_object_detection_3d_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file   mv_object_detection_3d.h
 * @brief  This file contains the Inference based Media Vision API.
 */

/**
 * @addtogroup CAPI_MEDIA_VISION_INFERENCE_MODULE
 * @{
 */

/**
 * @brief Creates a inference handle for object detection 3d object.
 * @details Use this function to create a inference handle. After the creation
 *          the object detection 3d task has to be prepared with
 *          mv_object_detection_3d_prepare() function to prepare a network
 *          for the inference.
 *
 * @since_tizen 9.0
 *
 * @remarks The @a infer should be released using mv_object_detection_3d_destroy().
 *
 * @param[out] handle    The handle to the inference to be created.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INTERNAL Internal Error
 *
 * @see mv_object_detection_3d_destroy()
 * @see mv_object_detection_3d_prepare()
 */
int mv_object_detection_3d_create(mv_object_detection_3d_h *handle);

/**
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
 * @pre Create inference handle by using mv_object_detection_3d_create()
 *
 * @see mv_object_detection_3d_create()
 */
int mv_object_detection_3d_destroy(mv_object_detection_3d_h handle);

/**
	 * @brief Set user-given model information.
	 * @details Use this function to change the model information instead of default one after calling @ref mv_object_detection_3d_create().
	 *
	 * @since_tizen 9.0
	 *
	 * @param[in] handle        The handle to the object detection 3d object.
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
	 * @pre Create a object detection 3d handle by calling @ref mv_object_detection_3d_create()
	 */
int mv_object_detection_3d_set_model(mv_object_detection_3d_h handle, const char *model_file, const char *meta_file,
									 const char *label_file, const char *model_name);

/**
 * @brief Configures the backend for the object detection inference.
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
int mv_object_detection_3d_configure(mv_object_detection_3d_h handle);

/**
 * @brief Prepares the object detection inference
 * @details Use this function to prepare the object detection inference based on
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
int mv_object_detection_3d_prepare(mv_object_detection_3d_h handle);

/**
 * @brief Performs the object detection 3d inference on the @a source.
 *
 * @since_tizen 9.0
 * @remarks This function is synchronous and may take considerable time to run.
 *
 * @param[in] handle          The handle to the inference
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
 * @pre Create an inference handle by calling mv_object_detect_3d_create()
 * @pre Prepare an inference by calling mv_object_detect_3d_prepare()
 * @post
 *
 * @see mv_object_detect_result_s structure
 */
int mv_object_detection_3d_inference(mv_object_detection_3d_h handle, mv_source_h source);

/**
 * @internal
 * @brief Gets the object detection 3d inference result count.
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the inference
 * @param[out] frame_number  A frame number inferenced.
 * @param[out] result_cnt    A number of results.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_object_detect_3d_create()
 * @pre Prepare an inference by calling mv_object_detect_3d_configure()
 * @pre Prepare an inference by calling mv_object_detect_3d_prepare()
 * @pre Request an inference by calling mv_object_detect_3d_inference()
 */
int mv_object_detection_3d_get_result_count(mv_object_detection_3d_h handle, unsigned long *frame_number,
											unsigned int *result_cnt);

/**
 * @internal
 * @brief Gets a point count to detected object corresponding to a given object index
 *
 * @since_tizen 9.0
 *
 * @param[in] handle        The handle to the inference
 * @param[in] object_index  A object index
 * @param[out] confidence   Probability to detected object.
 * @param[out] point_cnt    A number of points to detected object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_object_detect_3d_create()
 * @pre Prepare an inference by calling mv_object_detect_3d_configure()
 * @pre Prepare an inference by calling mv_object_detect_3d_prepare()
 * @pre Request an inference by calling mv_object_detect_3d_inference()
 * @pre Get result count by calling mv_object_detection_3d_get_result_count()
 */
int mv_object_detection_3d_get_point_count(mv_object_detection_3d_h handle, unsigned int object_index,
										   float *confidence, unsigned int *point_cnt);

/**
 * @internal
 * @brief Gets result to detected object corresponding to a given index
 *
 * @since_tizen 9.0
 *
 * @param[in] handle              The handle to the inference
 * @param[in] point_index         A point index.
 * @param[out] pos_x              x coordinate.
 * @param[out] pos_y              y coordinate.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_object_detect_3d_create()
 * @pre Prepare an inference by calling mv_object_detect_3d_configure()
 * @pre Prepare an inference by calling mv_object_detect_3d_prepare()
 * @pre Prepare an inference by calling mv_object_detect_3d_inference()
 * @pre Get result count by calling mv_object_detection_3d_get_result_count()
 * @pre Get object count by calling mv_object_detection_3d_get_object_count()
 */
int mv_object_detection_3d_get_points(mv_object_detection_3d_h handle, unsigned int point_index, int *pos_x,
									  int *pos_y);

/**
	 * @brief Set user-given inference engine and device types for inference.
	 * @details Use this function to change the inference engine and device types for inference instead of default ones after calling @ref mv_object_detection_3d_create().
	 *
	 * @since_tizen 9.0
	 *
	 * @param[in] handle        The handle to the object detection 3d object.
	 * @param[in] engine_type  A string of inference engine type.
	 * @param[in] device_type   A string of device type.
	 *
	 * @return @c 0 on success, otherwise a negative error value
	 * @retval #MEDIA_VISION_ERROR_NONE Successful
	 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
	 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
	 *
	 * @pre Create a object detection 3d handle by calling @ref mv_object_detection_3d_create()
	 */
int mv_object_detection_3d_set_engine(mv_object_detection_3d_h handle, const char *engine_type,
									  const char *device_type);

/**
	 * @brief Get a number of inference engines available for object detection 3d task API.
	 * @details Use this function to get how many inference engines are supported for object detection 3d after calling @ref mv_object_detection_3d_create().
	 *
	 * @since_tizen 9.0
	 *
	 * @param[in] handle         The handle to the object detection 3d object.
	 * @param[out] engine_count  A number of inference engines available for object detection 3d API.
	 *
	 * @return @c 0 on success, otherwise a negative error value
	 * @retval #MEDIA_VISION_ERROR_NONE Successful
	 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
	 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
	 *
	 * @pre Create a object detection 3d handle by calling @ref mv_object_detection_3d_create()
	 */
int mv_object_detection_3d_get_engine_count(mv_object_detection_3d_h handle, unsigned int *engine_count);

/**
	 * @brief Get engine type to a given inference engine index.
	 * @details Use this function to get inference engine type with a given engine index after calling @ref mv_object_detection_3d_get_engine_count().
	 *
	 * @since_tizen 9.0
	 *
	 * @param[in] handle        The handle to the object detection 3d object.
	 * @param[in] engine_index  A inference engine index for getting the inference engine type.
	 * @param[out] engine_type  A string to inference engine.
	 *
	 * @return @c 0 on success, otherwise a negative error value
	 * @retval #MEDIA_VISION_ERROR_NONE Successful
	 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
	 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
	 *
	 * @pre Get a number of inference engines available for object detection 3d task API by calling @ref mv_object_detection_get_engine_count()
	 */
int mv_object_detection_3d_get_engine_type(mv_object_detection_3d_h handle, const unsigned int engine_index,
										   char **engine_type);

/**
	 * @brief Get a number of device types available to a given inference engine.
	 * @details Use this function to get how many device types are supported for a given inference engine after calling @ref mv_object_detection_3d_create().
	 *
	 * @since_tizen 9.0
	 *
	 * @param[in] handle         The handle to the object detection 3d object.
	 * @param[in] engine_type    A inference engine string.
	 * @param[out] device_count  A number of device types available for a given inference engine.
	 *
	 * @return @c 0 on success, otherwise a negative error value
	 * @retval #MEDIA_VISION_ERROR_NONE Successful
	 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
	 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
	 *
	 * @pre Create a object detection 3d handle by calling @ref mv_object_detection_3d_create()
	 */
int mv_object_detection_3d_get_device_count(mv_object_detection_3d_h handle, const char *engine_type,
											unsigned int *device_count);

/**
	 * @brief Get device type list available.
	 * @details Use this function to get what device types are supported for current inference engine type after calling @ref mv_object_detection_3d_configure().
	 *
	 * @since_tizen 9.0
	 *
	 * @param[in] handle         The handle to the object detection 3d object.
	 * @param[in] engine_type    A inference engine string.
	 * @param[in] device_index   A device index for getting the device type.
	 * @param[out] device_type   A string to device type.
	 *
	 * @return @c 0 on success, otherwise a negative error value
	 * @retval #MEDIA_VISION_ERROR_NONE Successful
	 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
	 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
	 *
	 * @pre Create a object detection 3d handle by calling @ref mv_object_detection_3d_create()
	 * @pre Configure object detection 3d task by calling @ref mv_object_detection_3d_configure()
	 */
int mv_object_detection_3d_get_device_type(mv_object_detection_3d_h handle, const char *engine_type,
										   const unsigned int device_index, char **device_type);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_OBJECT_DETECT_3D_INTERNAL_H__ */
