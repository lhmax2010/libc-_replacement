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

#ifndef __TIZEN_MEDIAVISION_GAZE_TRACKING_INTERNAL_H__
#define __TIZEN_MEDIAVISION_GAZE_TRACKING_INTERNAL_H__

#include <mv_common.h>
#include <mv_gaze_tracking_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file   mv_gaze_tracking.h
 * @internal
 * @brief  This file contains the Inference based Media Vision API.
 */

/**
 * @addtogroup CAPI_MEDIA_VISION_INFERENCE_MODULE
 * @{
 */

/**
 * @internal
 * @brief Set user-given model information.
 * @details Use this function to change the model information instead of default one after calling @ref mv_gaze_tracking_create().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle        The handle to the gaze tracking object.
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
 * @pre Create a gaze tracking handle by calling @ref mv_gaze_tracking_create()
 */
int mv_gaze_tracking_set_model(mv_gaze_tracking_h handle, const char *model_file, const char *meta_file,
							   const char *label_file, const char *model_name);

/**
 * @internal
 * @brief Set user-given inference engine and device types for inference.
 * @details Use this function to change the inference engine and device types for inference instead of default ones after calling @ref mv_gaze_tracking_create().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle        The handle to the gaze tracking object.
 * @param[in] engine_type  A string of inference engine type.
 * @param[in] device_type   A string of device type.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a gaze tracking handle by calling @ref mv_gaze_tracking_create()
 */
int mv_gaze_tracking_set_engine(mv_gaze_tracking_h handle, const char *engine_type, const char *device_type);

/**
 * @internal
 * @brief Get a number of inference engines available for gaze tracking task API.
 * @details Use this function to get how many inference engines are supported for gaze tracking after calling @ref mv_gaze_tracking_create().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the gaze tracking object.
 * @param[out] engine_count  A number of inference engines available for gaze tracking API.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a gaze tracking handle by calling @ref mv_gaze_tracking_create()
 */
int mv_gaze_tracking_get_engine_count(mv_gaze_tracking_h handle, unsigned int *engine_count);

/**
 * @internal
 * @brief Get engine type to a given inference engine index.
 * @details Use this function to get inference engine type with a given engine index after calling @ref mv_gaze_tracking_get_engine_count().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle        The handle to the gaze tracking object.
 * @param[in] engine_index  A inference engine index for getting the inference engine type.
 * @param[out] engine_type  A string to inference engine.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Get a number of inference engines available for gaze tracking task API by calling @ref mv_gaze_tracking_get_engine_count()
 */
int mv_gaze_tracking_get_engine_type(mv_gaze_tracking_h handle, const unsigned int engine_index, char **engine_type);

/**
 * @internal
 * @brief Get a number of device types available to a given inference engine.
 * @details Use this function to get how many device types are supported for a given inference engine after calling @ref mv_gaze_tracking_create().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the gaze tracking object.
 * @param[in] engine_type    A inference engine string.
 * @param[out] device_count  A number of device types available for a given inference engine.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a gaze tracking handle by calling @ref mv_gaze_tracking_create()
 */
int mv_gaze_tracking_get_device_count(mv_gaze_tracking_h handle, const char *engine_type, unsigned int *device_count);

/**
 * @internal
 * @brief Get device type list available.
 * @details Use this function to get what device types are supported for current inference engine type after calling @ref mv_gaze_tracking_configure().
 *
 * @since_tizen 9.0
 *
 * @param[in] handle         The handle to the gaze tracking object.
 * @param[in] engine_type    A inference engine string.
 * @param[in] device_index   A device index for getting the device type.
 * @param[out] device_type   A string to device type.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a gaze tracking handle by calling @ref mv_gaze_tracking_create()
 * @pre Configure gaze tracking task by calling @ref mv_gaze_tracking_configure()
 */
int mv_gaze_tracking_get_device_type(mv_gaze_tracking_h handle, const char *engine_type,
									 const unsigned int device_index, char **device_type);
/**
 * @}
 */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_GAZE_TRACKING_INTERNAL_H__ */
