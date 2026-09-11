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

#ifndef __TIZEN_MEDIAVISION_MV_FACE_DETECTION_H__
#define __TIZEN_MEDIAVISION_MV_FACE_DETECTION_H__

#include <mv_common.h>
#include <mv_face_detection_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file   mv_face_detection.h
 * @brief  This file contains the Inference based Media Vision API.
 */

/**
 * @addtogroup CAPI_MEDIA_VISION_FACE_DETECTION_MODULE
 * @{
 */

/**
 * @brief Creates a inference handle for face detection object.
 * @details Use this function to create a inference handle. After the creation
 *          the face detection task has to be prepared with
 *          mv_face_detection_prepare() function to prepare a network
 *          for the inference.
 *
 * @since_tizen 9.0
 *
 * @remarks The @a handle should be released using mv_face_detection_destroy().
 *
 * @param[out] handle    The handle to the inference to be created.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INTERNAL Internal Error
 *
 * @code
 * #include <mv_face_detection.h>
 * ...
 * mv_face_detection_h handle = NULL;
 * mv_face_detection_create(&handle);
 * ...
 * mv_face_detection_destroy(handle);
 * @endcode
 *
 * @see mv_face_detection_destroy()
 * @see mv_face_detection_prepare()
 */
int mv_face_detection_create(mv_face_detection_h *handle);

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
 * @pre Create inference handle by using mv_face_detection_create()
 *
 * @see mv_face_detection_create()
 */
int mv_face_detection_destroy(mv_face_detection_h handle);

/**
 * @brief Configures the backend for the face detection inference.
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
 *
 * @pre Create an inference handle by calling mv_face_detection_create()
 */
int mv_face_detection_configure(mv_face_detection_h handle);

/**
 * @brief Prepares the face detection inference.
 * @details Use this function to prepare the face detection inference based on
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
 *
 * @pre Prepare an inference by calling mv_face_detection_configure()
 */
int mv_face_detection_prepare(mv_face_detection_h handle);

/**
 * @brief Performs the face detection inference on the @a source.
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
 * @pre Create an inference handle by calling mv_face_detection_create()
 * @pre Prepare an inference by calling mv_face_detection_configure()
 * @pre Prepare an inference by calling mv_face_detection_prepare()
 *
 * @par Inference Example
 * @snippet face_detection_sync.c FD sync
 */
int mv_face_detection_inference(mv_face_detection_h handle, mv_source_h source);

/**
 * @brief Performs asynchronously the face detection inference on the @a source.
 *
 * @since_tizen 9.0
 * @remarks This function operates asynchronously, so it returns immediately upon invocation.
 *          The inference results are inserted into the outgoing queue within the framework
 *          in the order of processing, and the results can be obtained through mv_face_detection_get_result_count()
 *          and mv_face_detection_get_bound_box().
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
 * @pre Create an inference handle by calling mv_face_detection_create()
 * @pre Prepare an inference by calling mv_face_detection_configure()
 * @pre Prepare an inference by calling mv_face_detection_prepare()
 *
 * @par Async Inference Example
 * @snippet face_detection_async.c FD async
 */
int mv_face_detection_inference_async(mv_face_detection_h handle, mv_source_h source);

/**
 * @brief Gets the face detection inference result on the @a handle.
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
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_face_detection_create()
 * @pre Prepare an inference by calling mv_face_detection_configure()
 * @pre Prepare an inference by calling mv_face_detection_prepare()
 * @pre Request an inference by calling mv_face_detection_inference()
 */
int mv_face_detection_get_result_count(mv_face_detection_h handle, unsigned long *frame_number,
									   unsigned int *result_cnt);

/**
 * @brief Gets a bound box to detected face region.
 *
 * @since_tizen 9.0
 *
 * @param[in] handle              The handle to the inference
 * @param[in] index               A result index.
 * @param[out] left               An left position of bound box.
 * @param[out] top                An top position of bound box.
 * @param[out] right              An right position of bound box.
 * @param[out] bottom             An bottom position of bound box.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_face_detection_create()
 * @pre Prepare an inference by calling mv_face_detection_configure()
 * @pre Prepare an inference by calling mv_face_detection_prepare()
 * @pre Request an inference by calling mv_face_detection_inference()
 * @pre Get result count by calling mv_face_detection_get_result_count()
 */
int mv_face_detection_get_bound_box(mv_face_detection_h handle, unsigned int index, int *left, int *top, int *right,
									int *bottom);
/**
 * @}
 */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_MV_FACE_DETECTION_H__ */
