/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MEDIAVISION_MV_GAZE_TRACKING_H__
#define __TIZEN_MEDIAVISION_MV_GAZE_TRACKING_H__

#include <mv_common.h>
#include <mv_gaze_tracking_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file   mv_gaze_tracking.h
 * @brief  This file contains the Inference based Media Vision API.
 */

/**
 * @addtogroup CAPI_MEDIA_VISION_GAZE_TRACKING_MODULE
 * @{
 */

/**
 * @brief Creates a inference handle for gaze tracking object.
 * @details Use this function to create a inference handle. After the creation
 *          the gaze tracking 3d task has to be prepared with
 *          mv_gaze_tracking_prepare() function to prepare a network
 *          for the inference.
 *
 * @since_tizen 9.0
 *
 * @remarks The @a infer should be released using mv_gaze_tracking_destroy().
 *
 * @param[out] infer    The handle to the inference to be created.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @code
 * #include <mv_gaze_tracking.h>
 * ...
 * mv_gaze_tracking_h handle = NULL;
 * mv_gaze_tracking_create(&handle);
 * ...
 * mv_gaze_tracking_destroy(handle);
 * @endcode
 *
 * @see mv_gaze_tracking_destroy()
 * @see mv_gaze_tracking_prepare()
 */
int mv_gaze_tracking_create(mv_gaze_tracking_h *infer);

/**
 * @brief Destroys inference handle and releases all its resources.
 *
 * @since_tizen 9.0
 *
 * @param[in] infer    The handle to the inference to be destroyed.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create inference handle by using mv_gaze_tracking_create()
 *
 * @see mv_gaze_tracking_create()
 */
int mv_gaze_tracking_destroy(mv_gaze_tracking_h infer);

/**
 * @brief Configures the backend for the gaze tracking inference.
 *
 * @since_tizen 9.0
 *
 * @param[in] infer         The handle to the inference
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 */
int mv_gaze_tracking_configure(mv_gaze_tracking_h infer);

/**
 * @brief Prepares the gaze tracking inference.
 * @details Use this function to prepare the gaze tracking inference based on
 *          the configured network.
 *
 * @since_tizen 9.0
 *
 * @param[in] infer         The handle to the inference.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_DATA Invalid model data
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Not supported format
 */
int mv_gaze_tracking_prepare(mv_gaze_tracking_h infer);

/**
 * @brief Performs the gaze tracking inference on the @a source.
 *
 * @since_tizen 9.0
 * @remarks This function is synchronous and may take considerable time to run.
 *
 * @param[in] infer          The handle to the inference
 * @param[in] source         The handle to the face image data source
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INTERNAL          Internal error
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Source colorspace
 *                                                  isn't supported
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_gaze_tracking_create()
 * @pre Prepare an inference by calling mv_gaze_tracking_configure()
 * @pre Prepare an inference by calling mv_gaze_tracking_prepare()
 *
 * @par Inference Example
 * @snippet gaze_tracking_sync.c OD sync
 */
int mv_gaze_tracking_inference(mv_gaze_tracking_h infer, mv_source_h source);

/**
 * @brief Performs asynchronously the gaze tracking inference on the @a source.
 *
 * @since_tizen 9.0
 * @remarks This function operates asynchronously, so it returns immediately upon invocation.
 *          The inference results are inserted into the outgoing queue within the framework
 *          in the order of processing, and the results can be obtained through
 *          mv_gaze_tracking_get_result_count() and mv_gaze_tracking_get_label().
 *
 * @param[in] handle         The handle to the inference
 * @param[in] source         The handle to the source of the media
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INTERNAL          Internal error
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Source colorspace
 *                                                  isn't supported
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_gaze_tracking_create()
 * @pre Prepare an inference by calling mv_gaze_tracking_configure()
 * @pre Prepare an inference by calling mv_gaze_tracking_prepare()
 *
 * @par Async Inference Example
 * @snippet gaze_tracking_async.c OD async
 */
int mv_gaze_tracking_inference_async(mv_gaze_tracking_h handle, mv_source_h source);

/**
 * @brief Gets the gaze tracking inference result on the @a handle.
 *
 * @since_tizen 9.0
 *
 * @param[in] handle          The handle to the inference
 * @param[out] frame_number   A frame number inferenced.
 * @param[out] result_cnt     A number of results.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INTERNAL          Internal error
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_gaze_tracking_create()
 * @pre Prepare an inference by calling mv_gaze_tracking_configure()
 * @pre Prepare an inference by calling mv_gaze_tracking_prepare()
 * @pre Request an inference by calling mv_gaze_tracking_inference()
 */
int mv_gaze_tracking_get_result_count(mv_gaze_tracking_h handle, unsigned long *frame_number, unsigned int *result_cnt);

/**
 * @brief Gets x and y-axis values to gazed position.
 *
 * @since_tizen 9.0
 *
 * @param[in] handle   The handle to the inference
 * @param[in] index    A result index.
 * @param[out] x       x-axis value array to gazed position.
 * @param[out] y       y-axis value array to gazed position.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INTERNAL          Internal error
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_gaze_tracking_create()
 * @pre Prepare an inference by calling mv_gaze_tracking_configure()
 * @pre Prepare an inference by calling mv_gaze_tracking_prepare()
 * @pre Prepare an inference by calling mv_gaze_tracking_inference()
 */
int mv_gaze_tracking_get_pos(mv_gaze_tracking_h handle, unsigned int index, float *x, float *y);

/**
 * @brief Gets a raw data to gazed position.
 *
 * @since_tizen 9.0
 *
 * @param[in] handle   The handle to the inference
 * @param[in] index    A result index.
 * @param[out] yaw     yaw value to gazed position.
 * @param[out] pitch   pitch value to gazed position.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INTERNAL          Internal error
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_gaze_tracking_create()
 * @pre Prepare an inference by calling mv_gaze_tracking_configure()
 * @pre Prepare an inference by calling mv_gaze_tracking_prepare()
 * @pre Prepare an inference by calling mv_gaze_tracking_inference()
 */
int mv_gaze_tracking_get_raw_data(mv_gaze_tracking_h handle, unsigned int index, float *yaw, float *pitch);
/**
 * @}
 */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_MV_GAZE_TRACKING_H__ */
