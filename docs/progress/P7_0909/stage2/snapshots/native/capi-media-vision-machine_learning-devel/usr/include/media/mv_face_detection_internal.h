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

#ifndef __TIZEN_MEDIAVISION_FACE_DETECT_INTERNAL_H__
#define __TIZEN_MEDIAVISION_FACE_DETECT_INTERNAL_H__

#include <mv_common.h>
#include <mv_face_detection_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file   mv_face_detection.h
 * @internal
 * @brief  This file contains the Inference based Media Vision API.
 */

/**
 * @addtogroup CAPI_MEDIA_VISION_INFERENCE_MODULE
 * @{
 */

/**
 * @internal
 * @brief Sets user-given model information.
 * @details Use this function to change the model information instead of default one after calling @ref mv_face_detection_create().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle        The handle to the face detection object.
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
 * @pre Create a face detection handle by calling @ref mv_face_detection_create()
 */
int mv_face_detection_set_model(mv_face_detection_h handle, const char *model_file, const char *meta_file,
								const char *label_file, const char *model_name);

/**
 * @internal
 * @brief Sets user-given inference engine and device types for inference.
 * @details Use this function to change the inference engine and device types for inference instead of default ones after calling @ref mv_face_detection_create().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle        The handle to the face detection object.
 * @param[in] engine_type  A string of inference engine type.
 * @param[in] device_type   A string of device type.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a face detection handle by calling @ref mv_face_detection_create()
 */
int mv_face_detection_set_engine(mv_face_detection_h handle, const char *engine_type, const char *device_type);

/**
 * @internal
 * @brief Gets a number of inference engines available for face detection task API.
 * @details Use this function to get how many inference engines are supported for face detection after calling @ref mv_face_detection_create().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the face detection object.
 * @param[out] engine_count  A number of inference engines available for face detection API.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a face detection handle by calling @ref mv_face_detection_create()
 */
int mv_face_detection_get_engine_count(mv_face_detection_h handle, unsigned int *engine_count);

/**
 * @internal
 * @brief Gets engine type to a given inference engine index.
 * @details Use this function to get inference engine type with a given engine index after calling @ref mv_face_detection_get_engine_count().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle        The handle to the face detection object.
 * @param[in] engine_index  A inference engine index for getting the inference engine type.
 * @param[out] engine_type  A string to inference engine.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Get a number of inference engines available for face detection task API by calling @ref mv_face_detection_get_engine_count()
 */
int mv_face_detection_get_engine_type(mv_face_detection_h handle, const unsigned int engine_index, char **engine_type);

/**
 * @internal
 * @brief Gets a number of device types available to a given inference engine.
 * @details Use this function to get how many device types are supported for a given inference engine after calling @ref mv_face_detection_create().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the face detection object.
 * @param[in] engine_type    A inference engine string.
 * @param[out] device_count  A number of device types available for a given inference engine.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a face detection handle by calling @ref mv_face_detection_create()
 */
int mv_face_detection_get_device_count(mv_face_detection_h handle, const char *engine_type, unsigned int *device_count);

/**
 * @internal
 * @brief Gets device type list available.
 * @details Use this function to get what device types are supported for current inference engine type after calling @ref mv_face_detection_configure().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the face detection object.
 * @param[in] engine_type    A inference engine string.
 * @param[in] device_index   A device index for getting the device type.
 * @param[out] device_type   A string to device type.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a face detection handle by calling @ref mv_face_detection_create()
 * @pre Configure face detection task by calling @ref mv_face_detection_configure()
 */
int mv_face_detection_get_device_type(mv_face_detection_h handle, const char *engine_type,
									  const unsigned int device_index, char **device_type);
/**
 * @}
 */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_FACE_DETECT_INTERNAL_H__ */
