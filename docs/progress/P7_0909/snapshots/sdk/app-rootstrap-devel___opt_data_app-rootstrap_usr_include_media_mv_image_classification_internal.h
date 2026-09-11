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

#ifndef __MEDIA_VISION_IMAGE_CLASSIFICATION_INTERNAL_H__
#define __MEDIA_VISION_IMAGE_CLASSIFICATION_INTERNAL_H__

#include <mv_common.h>
#include <mv_image_classification_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
	 * @brief Set user-given model information.
	 * @details Use this function to change the model information instead of default one after calling @ref mv_image_classification_create().
	 *
	 * @since_tizen 9.0
	 *
	 * @param[in] handle        The handle to the image classification object.
	 * @param[in] model_file    Model file name.
	 * @param[in] meta_type     Model meta file name.
	 * @param[in] label_file    Label file name.
	 * @param[in] model_name    Model name.
	 *
	 * @return @c 0 on success, otherwise a negative error value
	 * @retval #MEDIA_VISION_ERROR_NONE Successful
	 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
	 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
	 *
	 * @pre Create a image classification handle by calling @ref mv_image_classification_create()
	 */
int mv_image_classification_set_model(mv_image_classification_h handle, const char *model_file, const char *meta_file,
									  const char *label_file, const char *model_name);

/**
	 * @brief Set user-given inference engine and device types for inference.
	 * @details Use this function to change the inference engine and device types for inference instead of default ones after calling @ref mv_image_classification_create().
	 *
	 * @since_tizen 9.0
	 *
	 * @param[in] handle        The handle to the image classification object.
	 * @param[in] engine_type  A string of inference engine type.
	 * @param[in] device_type   A string of device type.
	 *
	 * @return @c 0 on success, otherwise a negative error value
	 * @retval #MEDIA_VISION_ERROR_NONE Successful
	 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
	 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
	 *
	 * @pre Create a image classification handle by calling @ref mv_image_classification_create()
	 */
int mv_image_classification_set_engine(mv_image_classification_h handle, const char *engine_type,
									   const char *device_type);

/**
	 * @brief Get a number of inference engines available for image classification task API.
	 * @details Use this function to get how many inference engines are supported for image classification after calling @ref mv_image_classification_create().
	 *
	 * @since_tizen 9.0
	 *
	 * @param[in] handle         The handle to the image classification object.
	 * @param[out] engine_count  A number of inference engines available for image classification API.
	 *
	 * @return @c 0 on success, otherwise a negative error value
	 * @retval #MEDIA_VISION_ERROR_NONE Successful
	 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
	 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
	 *
	 * @pre Create a image classification handle by calling @ref mv_image_classification_create()
	 */
int mv_image_classification_get_engine_count(mv_image_classification_h handle, unsigned int *engine_count);

/**
	 * @brief Get engine type to a given inference engine index.
	 * @details Use this function to get inference engine type with a given engine index after calling @ref mv_image_classification_get_engine_count().
	 *
	 * @since_tizen 9.0
	 *
	 * @param[in] handle        The handle to the image classification object.
	 * @param[in] engine_index  A inference engine index for getting the inference engine type.
	 * @param[out] engine_type  A string to inference engine.
	 *
	 * @return @c 0 on success, otherwise a negative error value
	 * @retval #MEDIA_VISION_ERROR_NONE Successful
	 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
	 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
	 *
	 * @pre Get a number of inference engines available for image classification task API by calling @ref mv_image_classification_get_engine_count()
	 */
int mv_image_classification_get_engine_type(mv_image_classification_h handle, const unsigned int engine_index,
											char **engine_type);

/**
	 * @brief Get a number of device types available to a given inference engine.
	 * @details Use this function to get how many device types are supported for a given inference engine after calling @ref mv_image_classification_create().
	 *
	 * @since_tizen 9.0
	 *
	 * @param[in] handle         The handle to the image classification object.
	 * @param[in] engine_type    A inference engine string.
	 * @param[out] device_count  A number of device types available for a given inference engine.
	 *
	 * @return @c 0 on success, otherwise a negative error value
	 * @retval #MEDIA_VISION_ERROR_NONE Successful
	 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
	 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
	 *
	 * @pre Create a image classification handle by calling @ref mv_image_classification_create()
	 */
int mv_image_classification_get_device_count(mv_image_classification_h handle, const char *engine_type,
											 unsigned int *device_count);

/**
	 * @brief Get device type list available.
	 * @details Use this function to get what device types are supported for current inference engine type after calling @ref mv_image_classification_configure().
	 *
	 * @since_tizen 9.0
	 *
	 * @param[in] handle         The handle to the image classification object.
	 * @param[in] engine_type    A inference engine string.
	 * @param[in] device_index   A device index for getting the device type.
	 * @param[out] device_type   A string to device type.
	 *
	 * @return @c 0 on success, otherwise a negative error value
	 * @retval #MEDIA_VISION_ERROR_NONE Successful
	 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
	 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
	 *
	 * @pre Create a image classification handle by calling @ref mv_image_classification_create()
	 * @pre Configure image classification task by calling @ref mv_image_classification_configure()
	 */
int mv_image_classification_get_device_type(mv_image_classification_h handle, const char *engine_type,
											const unsigned int device_index, char **device_type);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MEDIA_VISION_IMAGE_CLASSIFICATION_H__ */
