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

#ifndef __TIZEN_MEDIAVISION_MV_FACE_RECOGNITION_INTERNAL_H__
#define __TIZEN_MEDIAVISION_MV_FACE_RECOGNITION_INTERNAL_H__

#include <mv_common.h>
#include <mv_face_recognition_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @internal
 * @brief Gets confidences after a given face image is recognized.
 * @details Use this function to get the confidences calling @ref mv_face_recognition_inference().
 *
 * @since_tizen 8.0
 *
 * @remarks The @a num_of_confidences and @a confidences must NOT be released using free()
 *
 * @param[in] handle              The handle to the face recognition object.
 * @param[out] confidences        The array pointer to the confidence table which contains a confidence value of each class.
 *                                This function returns memory pointer containing actual confidence values to @a confidences.
 *                                And please note that @a confidences is valid only while handle is alive.
 * @param[out] num_of_confidences A number of confidences to the classes registered.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Request an inference by calling @ref mv_face_recognition_inference()
 */
int mv_face_recognition_get_confidence(mv_face_recognition_h handle, const float **confidences,
									   size_t *num_of_confidences);

/**
 * @internal
 * @brief Gets a label name corresponding to a given index.
 * @details Use this function to get the label name calling @ref mv_face_recognition_get_confidence().
 *
 * @since_tizen 8.0
 *
 * @remarks The @a num_of_confidences and @a confidences must NOT be released using free()
 *
 * @param[in] handle   The handle to the face recognition object.
 * @param[in] index    A label index pointing to the offset of label file.
 * @param[out] label   The array pointer for the label name to be stored.
 *                     This function returns memory pointer containing actual label string to @a label.
 *                     So do not free @a label. And please note that @a label is valid only while handle is alive.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Request an inference by calling @ref mv_face_recognition_get_confidence()
 */
int mv_face_recognition_get_label_with_index(mv_face_recognition_h handle, const unsigned int index,
											 const char **label);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_MV_FACE_RECOGNITION_INTERNAL_H__ */
