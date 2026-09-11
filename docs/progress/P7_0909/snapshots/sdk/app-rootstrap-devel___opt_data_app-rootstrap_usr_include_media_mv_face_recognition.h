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

#ifndef __TIZEN_MEDIAVISION_MV_FACE_RECOGNITION_H__
#define __TIZEN_MEDIAVISION_MV_FACE_RECOGNITION_H__

#include <mv_common.h>
#include <mv_face_recognition_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file   mv_face_recognition.h
 * @brief  This file contains the face recognition CAPI for the Mediavision.
 */

/**
 * @addtogroup CAPI_MEDIA_VISION_FACE_RECOGNITION_MODULE
 * @{
 */

/**
 * @brief Creates face recognition handle.
 * @details Use this function to create a face recognition handle. After the creation
 *          the handle has to be prepared with mv_face_recognition_prepare() function to prepare
 *          face recognition resources.
 *
 * @since_tizen 7.0
 *
 * @remarks The @a handle should be released using mv_face_recognition_destroy().
 *
 * @param[out] handle    The handle to the face recognition to be created
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mv_face_recognition_destroy()
 */
int mv_face_recognition_create(mv_face_recognition_h *handle);

/**
 * @brief Destroys face recognition handle and release all its resources.
 *
 * @since_tizen 7.0
 *
 * @param[in] handle    The handle to the face recognition object to be destroyed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create face recognition handle by using mv_face_recognition_create()
 *
 * @see mv_face_recognition_create()
 */
int mv_face_recognition_destroy(mv_face_recognition_h handle);

/**
 * @brief Prepares the resources for face recognition.
 * @details Use this function to prepare the resources for face recognition.
 *
 * @since_tizen 7.0
 *
 * @param[in] handle        The handle to the face recognition object.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_VISION_ERROR_KEY_NOT_AVAILABLE Attribute key isn't available
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create face recognition handle by using mv_face_recognition_create()
 */
int mv_face_recognition_prepare(mv_face_recognition_h handle);

/**
 * @brief Registers a new face on the @a source.
 * @details Use this function to register a new face.
 *          Each time when this function is called, a new face on the media source
 *          will be registered to internal database.
 *
 * @since_tizen 7.0
 *
 * @param[in] handle         The handle to the face recognition object.
 * @param[in] source         The handle to the source of the media.
 * @param[in] label          The label to be registered.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Source colorspace
 *                                                  isn't supported
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_VISION_ERROR_INTERNAL Internal error
  *
 * @pre Prepare an face recognition by calling mv_face_recognition_prepare()
 */
int mv_face_recognition_register(mv_face_recognition_h handle, mv_source_h source, const char *label);

/**
 * @brief Unregisters face data on the @a label.
 * @details Use this function to unregister a given label.
 *          Each time when this function is called, all data related to the label
 *          will be removed from internal database.
 *
 * @since_tizen 7.0
 *
 * @param[in] handle         The handle to the face recognition object.
 * @param[in] label          The label to be unregistered.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Register an new face by calling mv_face_recognition_register()
 */
int mv_face_recognition_unregister(mv_face_recognition_h handle, const char *label);

/**
 * @brief Inferences with a given face on the @a source.
 * @details Use this function to inference with a given source.
 *          This function returns n proper label string to a give source.
 *
 * @since_tizen 7.0
 *
 * @param[in] handle         The handle to the face recognition object.
 * @param[in] source         The handle to the source of the media.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_VISION_ERROR_INVALID_PATH Invalid path
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_VISION_ERROR_INTERNAL Internal error
 * @retval #MEDIA_VISION_ERROR_NO_DATA No data
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Source colorspace
 *                                                  isn't supported
 *
 * @pre Prepare an face recognition by calling mv_face_recognition_prepare()
 * @pre Register a new face by calling mv_face_recognition_register()
 */
int mv_face_recognition_inference(mv_face_recognition_h handle, mv_source_h source);

/**
 * @brief Gets the recognized face label name.
 * @details Use this function to get the recognized label name after calling mv_face_recognition_inference().
 *
 * @since_tizen 7.0
 *
 * @remarks The @a out_label must NOT be released using free()
 *
 * @param[in] handle         The handle to the face recognition object.
 * @param[out] out_label     The array pointer for the label name to be stored.
 *                           This function returns memory pointer containing actual label string to @a out_label.
 *                           So do not free @a out_label. And please note that @a out_label is valid only while handle is alive.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Request an inference by calling mv_face_recognition_inference()
 */
int mv_face_recognition_get_label(mv_face_recognition_h handle, const char **out_label);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_MV_FACE_RECOGNITION_H__ */
