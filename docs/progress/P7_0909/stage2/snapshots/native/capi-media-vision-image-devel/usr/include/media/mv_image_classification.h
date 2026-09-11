/**
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

#ifndef __TIZEN_MEDIAVISION_MV_IMAGE_CLASSIFICATION_H__
#define __TIZEN_MEDIAVISION_MV_IMAGE_CLASSIFICATION_H__

#include <mv_common.h>
#include <mv_image_classification_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file   mv_image_classification.h
 * @brief  This file contains the Inference based Media Vision API.
 */

/**
 * @addtogroup CAPI_MEDIA_VISION_IMAGE_CLASSIFICATION_MODULE
 * @{
 */

/**
 * @brief Creates image classification object handle.
 * @details Use this function to create an image classification object handle.
 *          After creation the handle has to be prepared with
 *          mv_image_classification_prepare() function to prepare
 *          an image classification object.
 *
 * @since_tizen 9.0
 *
 * @param[out] handle    The handle to the image classification object to be created
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INTERNAL Internal Error
 *
 * @post Release @a handle by using mv_image_classification_destroy() function when
 *       it is not needed anymore.
 *
 * @code
 * #include <mv_image_classification.h>
 * ...
 * mv_image_classification_h handle = NULL;
 * mv_image_classification_create(&handle);
 * ...
 * mv_image_classification_destroy(handle);
 * @endcode
 *
 * @see mv_image_classification_destroy()
 */
int mv_image_classification_create(mv_image_classification_h *handle);

/**
 * @brief Destroys image classification handle and releases all its resources.
 *
 * @since_tizen 9.0
 *
 * @param[in] handle    The handle to the image classification object to be destroyed.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create an image classification handle by using mv_image_classification_create()
 *
 * @see mv_image_classification_create()
 */
int mv_image_classification_destroy(mv_image_classification_h handle);

/**
 * @brief Configures the backend to the inference handle.
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the inference
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 */
int mv_image_classification_configure(mv_image_classification_h handle);

/**
 * @brief Prepares inference.
 * @details Use this function to prepare inference based on
 *          the configured network.
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the inference
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_DATA Invalid model data
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 */
int mv_image_classification_prepare(mv_image_classification_h handle);

/**
 *
 * @brief Performs inference with a given face on the @a source.
 * @details Use this function to inference with a given source.
 *
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the image classification object.
 * @param[in] source         The handle to the source of the media.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Source colorspace
 *                                                  isn't supported
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an image classification handle by calling mv_image_classification_create()
 * @pre Prepare an image classification by calling mv_image_classification_prepare()
 *
 * @par Inference Example
 * @snippet image_classification_sync.c IC sync
 */
int mv_image_classification_inference(mv_image_classification_h handle, mv_source_h source);

/**
 * @brief Performs asynchronously the image classification inference on the @a source.
 *
 * @since_tizen 9.0
 * @remarks This function operates asynchronously, so it returns immediately upon invocation.
 *          The inference results are inserted into the outgoing queue within the framework
 *          in the order of processing, and the results can be obtained through mv_image_classification_get_label().
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
 * @pre Create an inference handle by calling mv_image_classification_create()
 * @pre Prepare an inference by calling mv_image_classification_configure()
 * @pre Prepare an inference by calling mv_image_classification_prepare()
 *
 * @par Async Inference Example
 * @snippet image_classification_async.c IC async
 */
int mv_image_classification_inference_async(mv_image_classification_h handle, mv_source_h source);

/**
 * @brief Gets the image classification inference result count on the @a handle.
 *
 * @since_tizen 9.0
 *
 * @param[in] handle       The handle to the inference
 * @param[out] frame_number       A frame number inferenced.
 * @param[out] result_cnt  A number of results.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_image_classification_create()
 * @pre Prepare an inference by calling mv_image_classification_configure()
 * @pre Prepare an inference by calling mv_image_classification_prepare()
 * @pre Request an inference by calling mv_image_classification_inference()
 */
int mv_image_classification_get_result_count(mv_image_classification_h handle, unsigned long *frame_number,
											 unsigned int *result_cnt);

/**
 * @brief Gets the image classification inference result to a given index.
 *
 * @since_tizen 9.0
 *
 * @remarks The @a label should not be released.
 * @remarks The @a label is available until new inference is performed or @a handle is released.
 *
 * @param[in] handle              The handle to the inference
 * @param[in] index               A result index.
 * @param[out] label              A label name to a detected object.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_image_classification_create()
 * @pre Prepare an inference by calling mv_image_classification_configure()
 * @pre Prepare an inference by calling mv_image_classification_prepare()
 * @pre Request an inference by calling mv_image_classification_inference()
 * @pre Get result count by calling mv_image_classification_get_result_count()
 *
 * @code
 * #include <mv_image_classification.h>
 * #include <stdio.h>
 * ...
 * mv_image_classification_h handle = NULL;
 * mv_image_classification_create(&handle);
 * ...
 * // perform inference
 * ...
 * unsigned long frame_number;
 * unsigned int cnt;
 * mv_image_classification_get_result_count(handle, &frame_number, &cnt);
 * for (unsigned long idx = 0; idx < cnt; ++idx) {
 *      const char *label = NULL;
 *      mv_image_classification_get_label(handle, idx, &label);
 *      printf("frame number = %ld, label = %s\n", frame_number, label);
 * }
 * mv_image_classification_destroy(handle);
 * @endcode
 */
int mv_image_classification_get_label(mv_image_classification_h handle, unsigned int index, const char **label);
/**
 * @}
 */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_MV_IMAGE_CLASSIFICATION_H__ */
