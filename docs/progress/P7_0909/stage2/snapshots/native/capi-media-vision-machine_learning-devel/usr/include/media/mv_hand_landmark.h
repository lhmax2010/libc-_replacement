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

#ifndef __TIZEN_MEDIAVISION_MV_HAND_LANDMARK_H__
#define __TIZEN_MEDIAVISION_MV_HAND_LANDMARK_H__

#include <mv_common.h>
#include <mv_hand_landmark_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file   mv_hand_landmark.h
 * @internal
 * @brief  This file contains the Inference based Media Vision API.
 */

/**
 * @addtogroup CAPI_MEDIA_VISION_HAND_LANDMARK_MODULE
 * @{
 */

/**
 * @internal
 * @brief Creates a inference handle for hand landmark object.
 * @details Use this function to create a inference handle. After the creation
 *          the hand landmark task has to be prepared with
 *          mv_hand_landmark_prepare() function to prepare a network
 *          for the inference.
 *
 * @since_tizen 9.0
 *
 * @remarks The @a handle should be released using mv_hand_landmark_destroy().
 *
 * @param[out] handle    The handle to the inference to be created.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @code
 * #include <mv_hand_landmark.h>
 * ...
 * mv_hand_landmark_h handle = NULL;
 * mv_hand_landmark_create(&handle);
 * ...
 * mv_hand_landmark_destroy(handle);
 * @endcode
 *
 * @see mv_hand_landmark_destroy()
 * @see mv_hand_landmark_prepare()
 */
int mv_hand_landmark_create(mv_hand_landmark_h *handle);

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
 * @pre Create inference handle by using mv_hand_landmark_create()
 *
 * @see mv_hand_landmark_create()
 */
int mv_hand_landmark_destroy(mv_hand_landmark_h handle);

/**
 * @internal
 * @brief Configures the backend for the hand landmark inference.
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the inference
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 */
int mv_hand_landmark_configure(mv_hand_landmark_h handle);

/**
 * @internal
 * @brief Prepares the hand landmark inference.
 * @details Use this function to prepare the hand landmark inference based on
 *          the configured network.
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the inference.
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
int mv_hand_landmark_prepare(mv_hand_landmark_h handle);

/**
 * @internal
 * @brief Performs the hand landmark inference on the @a source.
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
 * @retval #MEDIA_VISION_ERROR_INTERNAL          Internal error
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Source colorspace
 *                                                  isn't supported
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_hand_landmark_create()
 * @pre Prepare an inference by calling mv_hand_landmark_configure()
 * @pre Prepare an inference by calling mv_hand_landmark_prepare()
 *
 * @par Inference Example
 * @snippet hand_landmark_sync.c FLD sync
 */
int mv_hand_landmark_inference(mv_hand_landmark_h handle, mv_source_h source);

/**
 * @internal
 * @brief Performs asynchronously the hand landmark inference on the @a source.
 *
 * @since_tizen 9.0
 * @remarks This function operates asynchronously, so it returns immediately upon invocation.
 *          The inference results are inserted into the outgoing queue within the framework
 *          in the order of processing, and the results can be obtained through mv_hand_landmark_get_position().
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
 * @pre Create an inference handle by calling mv_hand_landmark_create()
 * @pre Prepare an inference by calling mv_hand_landmark_configure()
 * @pre Prepare an inference by calling mv_hand_landmark_prepare()
 *
 * @par Async Inference Example
 * @snippet hand_landmark_async.c FLD async
 */
int mv_hand_landmark_inference_async(mv_hand_landmark_h handle, mv_source_h source);

/**
 * @internal
 * @brief Gets the result count to objects.
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
 * @retval #MEDIA_VISION_ERROR_INTERNAL          Internal error
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_hand_landmark_create()
 * @pre Prepare an inference by calling mv_hand_landmark_configure()
 * @pre Prepare an inference by calling mv_hand_landmark_prepare()
 * @pre Request an inference by calling mv_hand_landmark_inference()
 */
int mv_hand_landmark_get_result_count(mv_hand_landmark_h handle, unsigned long *frame_number, unsigned int *result_cnt);

/**
 * @internal
 * @brief Gets the hand landmark position values to a given index.
 *
 * @since_tizen 9.0
 * @remarks pos_x and pos_y arrays are allocated internally by the framework and will remain valid
 *          until the handle is released.
 *          Please do not deallocate them directly, and if you want to use them after the handle is released,
 *          please copy them to user memory and use the copy.
 *
 *          This function operates differently depending on the inference request method.
 *          - After mv_hand_landmark_inference() calls, this function returns hand landmark positions immediately.
 *          - After mv_hand_landmark_inference_async() calls, this function can be blocked until the asynchronous inference request is completed
 *            or the timeout occurs if no result within 3 seconds.
 *
 *          Additionally, after calling the mv_hand_landmark_inference_async() function, the function operates
 *          in asynchronous mode until the handle is released.
 *
 * @param[in] handle               The handle to the inference
 * @param[in] index                A result index.
 * @param[out] pos_x               An array containing x-coordinate values.
 * @param[out] pos_y               An array containing y-coordinate values.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INTERNAL          Internal error
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_hand_landmark_create()
 * @pre Prepare an inference by calling mv_hand_landmark_configure()
 * @pre Prepare an inference by calling mv_hand_landmark_prepare()
 * @pre Prepare an inference by calling mv_hand_landmark_inference()
 * @pre Get result count by calling mv_hand_landmark_get_result_count()
 */
int mv_hand_landmark_get_position(mv_hand_landmark_h handle, unsigned int index, unsigned int *pos_x,
								  unsigned int *pos_y);
/**
 * @}
 */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_MV_HAND_LANDMARK_H__ */
