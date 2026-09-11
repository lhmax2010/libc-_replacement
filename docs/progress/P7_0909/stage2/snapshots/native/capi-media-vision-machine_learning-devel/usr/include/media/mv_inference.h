/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MEDIAVISION_INFERENCE_H__
#define __TIZEN_MEDIAVISION_INFERENCE_H__

#include <mv_common.h>
#include <mv_inference_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @deprecated Deprecated since 9.0
 * @file   mv_inference.h
 * @brief  This file contains the Inference based Media Vision API.
 */

/**
 * @addtogroup CAPI_MEDIA_VISION_INFERENCE_MODULE
 * @{
 */

/**
 * @deprecated Deprecated since 9.0
 * @brief Defines #MV_INFERENCE_MODEL_CONFIGURATION_FILE_PATH to set inference
         model's configuration file attribute of the engine configuration.
 * @details Inference model's configuration can be changed to specify the path to the file
 *
 * @since_tizen 5.5
 * @see mv_engine_config_set_string_attribute()
 * @see mv_engine_config_get_string_attribute()
 */
#define MV_INFERENCE_MODEL_CONFIGURATION_FILE_PATH "MV_INFERENCE_MODEL_CONFIGURATION_FILE_PATH"

/**
 * @deprecated Deprecated since 9.0
 * @brief Defines #MV_INFERENCE_MODEL_WEIGHT_FILE_PATH to set inference
 *        model's weight file attribute of the engine configuration.
 * @details Inference model's weight can be changed to specify the path to the file
 *
 * @since_tizen 5.5
 * @see mv_engine_config_set_string_attribute()
 * @see mv_engine_config_get_string_attribute()
 */
#define MV_INFERENCE_MODEL_WEIGHT_FILE_PATH "MV_INFERENCE_MODEL_WEIGHT_FILE_PATH"

/**
 * @deprecated Deprecated since 9.0
 * @brief Defines #MV_INFERENCE_MODEL_USER_FILE_PATH to set inference
 *        model's category file attribute of the engine configuration.
 * @details Inference model's category can be changed to specify the path to the file
 *
 * @since_tizen 5.5
 * @see mv_engine_config_set_string_attribute()
 * @see mv_engine_config_get_string_attribute()
 */
#define MV_INFERENCE_MODEL_USER_FILE_PATH "MV_INFERENCE_MODEL_USER_FILE_PATH"

/**
 * @deprecated Deprecated since 9.0
 * @brief Defines #MV_INFERENCE_MODEL_META_FILE_PATH to set inference
 *        models's metadata file attribute of the engine configuration.
 * @details The file includes inference model's metadata such as input and output
 *          node names, input tensor's width and height,
 *          mean and standard deviation values for pre-processing.
 *
 * @since_tizen 6.5
 * @see mv_engine_config_set_string_attribute()
 * @see mv_engine_config_get_string_attribute()
 */
#define MV_INFERENCE_MODEL_META_FILE_PATH "MV_INFERENCE_MODEL_META_FILE_PATH"

/**
 * @deprecated Deprecated since 9.0
 * @deprecated Deprecated since 6.5. Use #MV_INFERENCE_MODEL_META_FILE_PATH instead.
 * @brief Defines #MV_INFERENCE_MODEL_MEAN_VALUE to set inference
 *        model's mean attribute of the engine configuration.
 *
 * @since_tizen 5.5
 * @see mv_engine_config_set_double_attribute()
 * @see mv_engine_config_get_double_attribute()
 */
#define MV_INFERENCE_MODEL_MEAN_VALUE "MV_INFERENCE_MODEL_MEAN_VALUE"

/**
 * @deprecated Deprecated since 9.0
 * @deprecated Deprecated since 6.5. Use #MV_INFERENCE_MODEL_META_FILE_PATH instead.
 * @brief Defines #MV_INFERENCE_MODEL_STD_VALUE to set an input image's
 *        standard deviation attribute of the engine configuration.
 *
 * @since_tizen 5.5
 * @see mv_engine_config_set_double_attribute()
 * @see mv_engine_config_get_double_attribute()
 */
#define MV_INFERENCE_MODEL_STD_VALUE "MV_INFERENCE_MODEL_STD_VALUE"

/**
 * @deprecated Deprecated since 9.0
 * @brief Defines #MV_INFERENCE_BACKEND_TYPE to set the type used
 *        for inference attribute of the engine
 *        configuration.
 * @details Switches between two types of the type used for
 *          neural network model inference. Possible values of the
 *          attribute are:\n
 *          #MV_INFERENCE_BACKEND_OPENCV,\n
 *          #MV_INFERENCE_BACKEND_TFLITE.\n
 *          The default type is #MV_INFERENCE_BACKEND_OPENCV.
 *
 * @since_tizen 5.5
 * @see mv_engine_config_set_int_attribute()
 * @see mv_engine_config_get_int_attribute()
 */
#define MV_INFERENCE_BACKEND_TYPE "MV_INFERENCE_BACKEND_TYPE"

/**
 * @deprecated Deprecated since 9.0
 * @brief Defines #MV_INFERENCE_TARGET_DEVICE_TYPE to set the type used
 *        for device running attribute of the engine configuration.
 * @details Switches between CPU, GPU, or Custom:\n
 *          #MV_INFERENCE_TARGET_DEVICE_CPU,\n
 *          #MV_INFERENCE_TARGET_DEVICE_GPU,\n
 *          #MV_INFERENCE_TARGET_DEVICE_CUSTOM.\n
 *
 *          The default type is CPU.
 *
 * @since_tizen 6.0
 * @see mv_engine_config_set_int_attribute()
 * @see mv_engine_config_get_int_attribute()
 */
#define MV_INFERENCE_TARGET_DEVICE_TYPE "MV_INFERENCE_TARGET_DEVICE_TYPE"

/**
 * @deprecated Deprecated since 6.5. Use #MV_INFERENCE_MODEL_META_FILE_PATH instead.
 * @brief Defines #MV_INFERENCE_INPUT_TENSOR_WIDTH to set the width
 *        of input tensor.
 *
 * @since_tizen 5.5
 * @see mv_engine_config_set_int_attribute()
 * @see mv_engine_config_get_int_attribute()
 */
#define MV_INFERENCE_INPUT_TENSOR_WIDTH "MV_INFERENCE_INPUT_TENSOR_WIDTH"

/**
 * @deprecated Deprecated since 6.5. Use #MV_INFERENCE_MODEL_META_FILE_PATH instead.
 * @brief Defines #MV_INFERENCE_INPUT_TENSOR_HEIGHT to set the height
 *        of input tensor.
 *
 * @since_tizen 5.5
 * @see mv_engine_config_set_int_attribute()
 * @see mv_engine_config_get_int_attribute()
 */
#define MV_INFERENCE_INPUT_TENSOR_HEIGHT "MV_INFERENCE_INPUT_TENSOR_HEIGHT"

/**
 * @deprecated Deprecated since 6.5. Use #MV_INFERENCE_MODEL_META_FILE_PATH instead.
 * @brief Defines #MV_INFERENCE_INPUT_TENSOR_CHANNELS to set the channels,
 *        for example 3 in case of RGB colorspace, of input tensor.
 *
 * @since_tizen 5.5
 * @see mv_engine_config_set_int_attribute()
 * @see mv_engine_config_get_int_attribute()
 */
#define MV_INFERENCE_INPUT_TENSOR_CHANNELS "MV_INFERENCE_INPUT_TENSOR_CHANNELS"

/**
 * @deprecated Deprecated since 6.5. Use #MV_INFERENCE_MODEL_META_FILE_PATH instead.
 * @brief Defines #MV_INFERENCE_INPUT_DATA_TYPE to set data type of input tensor.
 * @details Data type of input tensor can be changed according to a given weight file.
 *          Switches between Float32 or UInt8:\n
 *          #MV_INFERENCE_DATA_FLOAT32,\n
 *          #MV_INFERENCE_DATA_UINT8,\n
 *
 *          The default type is MV_INFERENCE_DATA_FLOAT32.
 *
 * @since_tizen 6.0
 * @see mv_engine_config_set_int_attribute()
 * @see mv_engine_config_get_int_attribute()
 */
#define MV_INFERENCE_INPUT_DATA_TYPE "MV_INFERENCE_INPUT_DATA_TYPE"

/**
 * @deprecated Deprecated since 6.5. Use #MV_INFERENCE_MODEL_META_FILE_PATH instead.
 * @brief Defines #MV_INFERENCE_INPUT_NODE_NAME to set the input node name.
 *
 * @since_tizen 5.5
 * @see mv_engine_config_set_string_attribute()
 * @see mv_engine_config_get_string_attribute()
 */
#define MV_INFERENCE_INPUT_NODE_NAME "MV_INFERENCE_INPUT_NODE_NAME"

/**
 * @deprecated Deprecated since 6.5. Use #MV_INFERENCE_MODEL_META_FILE_PATH instead.
 * @brief Defines #MV_INFERENCE_OUTPUT_NODE_NAMES to set the output node names.
 *
 * @since_tizen 5.5
 * @see mv_engine_config_set_array_string_attribute()
 * @see mv_engine_config_get_array_string_attribute()
 */
#define MV_INFERENCE_OUTPUT_NODE_NAMES "MV_INFERENCE_OUTPUT_NODE_NAMES"

/**
 * @deprecated Deprecated since 6.5. Use #MV_INFERENCE_MODEL_META_FILE_PATH instead.
 * @brief Defines #MV_INFERENCE_OUTPUT_MAX_NUMBER
 *        to set the maximum number of output attributes
 *        of the engine configuration.
 * @details Default value is 5 and a value over 10 will be set to 10.
 *          A value under 1 will be set to 1.
 *
 * @since_tizen 5.5
 * @see mv_engine_config_set_int_attribute()
 * @see mv_engine_config_get_int_attribute()
 */
#define MV_INFERENCE_OUTPUT_MAX_NUMBER "MV_INFERENCE_OUTPUT_MAX_NUMBER"

/**
 * @deprecated Deprecated since 6.5. Use #MV_INFERENCE_MODEL_META_FILE_PATH instead.
 * @brief Defines #MV_INFERENCE_CONFIDENCE_THRESHOLD
 *        to set the threshold value for the confidence of inference results.
 * @details Default value is 0.6 and its range is between 0.0 and 1.0.
 *
 * @since_tizen 5.5
 * @see mv_engine_config_set_double_attribute()
 * @see mv_engine_config_get_double_attribute()
 */
#define MV_INFERENCE_CONFIDENCE_THRESHOLD "MV_INFERENCE_CONFIDENCE_THRESHOLD"

/*************/
/* Inference */
/*************/
/**
 * @deprecated Deprecated since 9.0
 * @brief Creates inference handle.
 * @details Use this function to create an inference. After the creation
 *          the inference has to be prepared with
 *          mv_inference_prepare() function to prepare a network
 *          for the inference.
 *
 * @since_tizen 5.5
 * @remarks If the app sets #MV_INFERENCE_MODEL_CONFIGURATION_FILE_PATH,
 *          #MV_INFERENCE_MODEL_WEIGHT_FILE_PATH, and #MV_INFERENCE_MODEL_USER_FILE_PATH
 *          to media storage, then the media storage privilege
 *          %http://tizen.org/privilege/mediastorage is needed.\n
 *          If the app sets any of the paths mentioned in the previous sentence
 *          to external storage, then the external storage privilege
 *          %http://tizen.org/privilege/externalstorage is needed.\n
 *          If the required privileges aren't set properly, mv_inference_prepare() will returned
 *          #MEDIA_VISION_ERROR_PERMISSION_DENIED.
 *
 * @remarks The @a infer should be released using mv_inference_destroy().
 *
 * @param[out] infer    The handle to the inference to be created
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mv_inference_destroy()
 * @see mv_inference_prepare()
 */
int mv_inference_create(mv_inference_h *infer);

/**
 * @deprecated Deprecated since 9.0
 * @brief Destroys inference handle and releases all its resources.
 *
 * @since_tizen 5.5
 *
 * @param[in] infer    The handle to the inference to be destroyed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create inference handle by using mv_inference_create()
 *
 * @see mv_inference_create()
 */
int mv_inference_destroy(mv_inference_h infer);

/**
 * @deprecated Deprecated since 9.0
 * @brief Configures the network of the inference.
 * @details Use this function to configure the network of the inference
 *          which is set to @a engine_config.
 *
 * @since_tizen 5.5
 *
 * @param[in] infer         The handle to the inference
 * @param[in] engine_config The handle to the configuration of
 *                           engine.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *                                               in @a engine_config
 * @retval #MEDIA_VISION_ERROR_INVALID_PATH Invalid path of model data
 *                                          in @a engine_config
 */
int mv_inference_configure(mv_inference_h infer, mv_engine_config_h engine_config);

/**
 * @deprecated Deprecated since 9.0
 * @brief Prepares inference.
 * @details Use this function to prepare inference based on
 *          the configured network.
 *
 * @since_tizen 5.5
 *
 * @param[in] infer         The handle to the inference
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
int mv_inference_prepare(mv_inference_h infer);

/**
* @deprecated Deprecated since 9.0
* @brief Called to provide information for supported engines for inference.
*
* @since_tizen 5.5
*
* @param[in] engine The supported engine.
*                   The @a engine can be used only in the callback.
*                   To use outside, make a copy.
* @param[in] supported The flag whether the engine
*                         is supported or not
* @param[in] user_data The user data passed from
*                       mv_inference_foreach_supported_engine()
* @return @c true to continue with the next iteration of the loop,
*         otherwise @c false to break out of the loop
*
* @pre mv_inference_foreach_supported_engine()
*/
typedef bool (*mv_inference_supported_engine_cb)(const char *engine, bool supported, void *user_data);

/**
* @deprecated Deprecated since 9.0
* @brief Traverses the list of supported engines for inference.
* @details Using this function the supported engines can be obtained.
*          The names can be used with #mv_engine_config_h related
*          getters and setters to get/set MV_INFERENCE_BACKEND_TYPE attribute
*          value.
*
* @since_tizen 5.5
* @param[in] infer The handle to the inference
* @param[in] callback The iteration callback function
* @param[in] user_data The user data to be passed to the callback function
* @return @c 0 on success, otherwise a negative error value
* @retval #MEDIA_VISION_ERROR_NONE Successful
* @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
* @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
*
* @see mv_inference_supported_engine_cb()
*/
int mv_inference_foreach_supported_engine(mv_inference_h infer, mv_inference_supported_engine_cb callback,
										  void *user_data);

/************************/
/* Image classification */
/************************/
/**
 * @deprecated Deprecated since 9.0
 * @brief Called when @a source is classified.
 * @details This callback is invoked each time when
 *          mv_inference_image_classify() is called to provide the results of
 *          image classification.
 *
 * @since_tizen 5.5
 * @remarks The @a indices, @a names, and @a confidences should not be released by the app.
 *          They can be used only in the callback. The number of elements in @a indices, @a names,
 *          and @a confidences is equal to @a number_of_classes.
 *
 * @param[in] source               The handle to the source of the media where
 *                                 an image was classified. @a source is the same object
 *                                 for which mv_inference_image_classify() was called.
 *                                 It should be released by calling mv_destroy_source()
 *                                 when it's not needed anymore.
 * @param[in] number_of_classes    The number of classes
 * @param[in] indices              The indices of the classified image.
 * @param[in] names                Names corresponding to the indices.
 * @param[in] confidences          Each element is the confidence that the corresponding image belongs to the corresponding class.
 * @param[in] user_data            The user data passed from callback invoking code
 *
 * @pre Call mv_inference_image_classify() function to perform classification of the image
 *      and to invoke this callback as a result
 *
 * @see mv_inference_image_classify()
 */
typedef void (*mv_inference_image_classified_cb)(mv_source_h source, int number_of_classes, const int *indices,
												 const char **names, const float *confidences, void *user_data);

/**
 * @deprecated Deprecated since 9.0
 * @brief Performs image classification on the @a source.
 * @details Use this function to launch image classification.
 *          Each time when mv_inference_image_classify() is
 *          called, @a classified_cb will receive classes
 *          which the media source may belong to.
 *
 * @since_tizen 5.5
 * @remarks This function is synchronous and may take considerable time to run.
 *
 * @param[in] source         The handle to the source of the media
 * @param[in] infer          The handle to the inference
 * @param[in] roi            Rectangular area in the @a source which will be analyzed.
 *                           If NULL, then the whole source will be analyzed.
 * @param[in] classified_cb  The callback which will be called for
 *                           classification on @a source.
 *                           This callback will receive classification results.
 * @param[in] user_data      The user data passed from the code where
 *                           mv_inference_image_classify() is invoked. This data will
 *                           be accessible in @a classified_cb callback.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_VISION_ERROR_INTERNAL          Internal error
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Source colorspace
 *                                                  isn't supported
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_inference_create()
 * @pre Configure an inference handle by calling mv_inference_configure()
 * @pre Prepare an inference by calling mv_inference_prepare()
 * @post @a classified_cb will be called to provide classification results
 *
 * @see mv_inference_image_classified_cb()
 */
int mv_inference_image_classify(mv_source_h source, mv_inference_h infer, mv_rectangle_s *roi,
								mv_inference_image_classified_cb classified_cb, void *user_data);

/************************/
/* Object Detection     */
/************************/
/**
 * @deprecated Deprecated since 9.0
 * @brief Called when objects in @a source are detected.
 * @details This callback is invoked each time when
 *          mv_inference_object_detect() is called to provide the results of
 *          object detection.
 *
 * @since_tizen 5.5
 * @remarks The @a indices, @a names, @a confidences, and @a locations should not be released by app.
 *          They can be used only in the callback. The number of elements in @a indices, @a names,
 *          @a confidences, and @a locations is equal to @a number_of_objects.
 *
 * @param[in] source               The handle to the source of the media where
 *                                 an image was classified. @a source is the same object
 *                                 for which mv_inference_object_detect() was called.
 *                                 It should be released by calling mv_destroy_source()
 *                                 when it's not needed anymore.
 * @param[in] number_of_objects    The number of objects
 * @param[in] indices              The indices of objects.
 * @param[in] names                Names corresponding to the indices.
 * @param[in] confidences          Confidences of the detected objects.
 * @param[in] locations            Locations of the detected objects.
 * @param[in] user_data            The user data passed from callback invoking code
 *
 * @pre Call mv_inference_object_detect() function to perform detection of the objects
 *      in @a source and to invoke this callback as a result
 *
 * @see mv_inference_object_detect()
 */
typedef void (*mv_inference_object_detected_cb)(mv_source_h source, int number_of_objects, const int *indices,
												const char **names, const float *confidences,
												const mv_rectangle_s *locations, void *user_data);

/**
 * @deprecated Deprecated since 9.0
 * @brief Performs object detection on the @a source.
 * @details Use this function to launch object detection.
 *          Each time when mv_inference_object_detect() is
 *          called, @a detected_cb will receive a list of objects and their locations
 *          in the media source.
 *
 * @since_tizen 5.5
 * @remarks This function is synchronous and may take considerable time to run.
 *
 * @param[in] source         The handle to the source of the media
 * @param[in] infer          The handle to the inference
 * @param[in] detected_cb    The callback which will be called for
 *                           detecting objects in the media source.
 *                           This callback will receive the detection results.
 * @param[in] user_data      The user data passed from the code where
 *                           mv_inference_object_detect() is invoked. This data will
 *                           be accessible in @a detected_cb callback.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INTERNAL          Internal error
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Source colorspace
 *                                                  isn't supported
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_inference_create()
 * @pre Configure an inference handle by calling mv_inference_configure()
 * @pre Prepare an inference by calling mv_inference_prepare()
 * @post @a detected_cb will be called to provide detection results
 *
 * @see mv_inference_object_detected_cb()
 */
int mv_inference_object_detect(mv_source_h source, mv_inference_h infer, mv_inference_object_detected_cb detected_cb,
							   void *user_data);

/*************************************/
/* Face and its landmark detection   */
/*************************************/
/**
 * @deprecated Deprecated since 9.0
 * @brief Called when faces in @a source are detected.
 * @details This callback is invoked each time when
 *          mv_inference_face_detect() is called to provide the results of
 *          face detection.
 *
 * @since_tizen 5.5
 * @remarks The @a confidences and @a locations should not be released by app.
 *          They can be used only in the callback. The number of elements in @a confidences
 *          and @a locations is equal to @a number_of_faces.
 *
 * @param[in] source               The handle to the source of the media where
 *                                 faces were detected. @a source is the same object
 *                                 for which mv_inference_face_detect() was called.
 *                                 It should be released by calling mv_destroy_source()
 *                                 when it's not needed anymore.
 * @param[in] number_of_faces      The number of faces
 * @param[in] confidences          Confidences of the detected faces.
 * @param[in] locations            Locations of the detected faces.
 * @param[in] user_data            The user data passed from callback invoking code
 *
 * @pre Call mv_inference_face_detect() function to perform detection of the faces
 *      in @a source and to invoke this callback as a result
 *
 * @see mv_inference_face_detect()
 */
typedef void (*mv_inference_face_detected_cb)(mv_source_h source, int number_of_faces, const float *confidences,
											  const mv_rectangle_s *locations, void *user_data);

/**
 * @deprecated Deprecated since 9.0
 * @brief Performs face detection on the @a source.
 * @details Use this function to launch face detection.
 *          Each time when mv_inference_face_detect() is
 *          called, @a detected_cb will receive a list of faces and their locations
 *          in the media source.
 *
 * @since_tizen 5.5
 * @remarks This function is synchronous and may take considerable time to run.
 *
 * @param[in] source         The handle to the source of the media
 * @param[in] infer          The handle to the inference
 * @param[in] detected_cb    The callback which will be called for
 *                           detecting faces on media source.
 *                           This callback will receive the detection results.
 * @param[in] user_data      The user data passed from the code where
 *                           mv_inference_face_detect() is invoked. This data will
 *                           be accessible in @a detected_cb callback.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INTERNAL          Internal error
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Source colorspace
 *                                                  isn't supported
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_inference_create()
 * @pre Configure an inference handle by calling mv_inference_configure()
 * @pre Prepare an inference by calling mv_inference_prepare()
 * @post @a detected_cb will be called to provide detection results
 *
 * @see mv_inference_face_detected_cb()
 */
int mv_inference_face_detect(mv_source_h source, mv_inference_h infer, mv_inference_face_detected_cb detected_cb,
							 void *user_data);

/**
 * @deprecated Deprecated since 9.0
 * @brief Called when facial landmarks in @a source are detected.
 * @details This type callback is invoked each time when
 *          mv_inference_facial_landmark_detect() is called to provide
 *          the results of the landmarks detection.
 *
 * @since_tizen 5.5
 * @remarks The @a locations should not be released by app. They can be used only in the callback.
 *          The number of elements in @a locations is equal to @a number_of_landmarks.
 *
 * @param[in] source               The handle to the source of the media where
 *                                 landmarks were detected. @a source is the same object
 *                                 for which mv_inference_facial_landmark_detect() was called.
 *                                 It should be released by calling mv_destroy_source()
 *                                 when it's not needed anymore.
 * @param[in] number_of_landmarks  The number of landmarks
 * @param[in] locations            Locations of the detected facial landmarks.
 * @param[in] user_data            The user data passed from callback invoking code
 *
 * @pre Call mv_inference_face_detect() function to perform detection of the faces
 *      in @a source and to invoke this callback as a result
 *
 * @see mv_inference_face_detect()
 */
typedef void (*mv_inference_facial_landmark_detected_cb)(mv_source_h source, int number_of_landmarks,
														 const mv_point_s *locations, void *user_data);

/**
 * @deprecated Deprecated since 9.0
 * @brief Performs facial landmarks detection on the @a source.
 * @details Use this function to launch facial landmark detection.
 *          Each time when mv_inference_facial_landmark_detect() is
 *          called, @a detected_cb will receive a list facial landmark's locations
 *          in the media source.
 *
 * @since_tizen 5.5
 * @remarks This function is synchronous and may take considerable time to run.
 *
 * @param[in] source         The handle to the source of the media
 * @param[in] infer          The handle to the inference
 * @param[in] roi            Rectangular area including a face in @a source which
 *                           will be analyzed. If NULL, then the whole source will be
 *                           analyzed.
 * @param[in] detected_cb    The callback which will receive the detection results.
 * @param[in] user_data      The user data passed from the code where
 *                           mv_inference_facial_landmark_detect() is invoked.
 *                           This data will be accessible in @a detected_cb callback.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INTERNAL          Internal error
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Source colorspace
 *                                                  isn't supported
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_inference_create()
 * @pre Configure an inference handle by calling mv_inference_configure()
 * @pre Prepare an inference by calling mv_inference_prepare()
 * @post @a detected_cb will be called to provide detection results
 *
 * @see mv_inference_facial_landmark_detected_cb()
 */
int mv_inference_facial_landmark_detect(mv_source_h source, mv_inference_h infer, mv_rectangle_s *roi,
										mv_inference_facial_landmark_detected_cb detected_cb, void *user_data);

/**
 * @deprecated Deprecated since 9.0
 * @brief Called when poses in @a source are detected.
 * @details This type callback is invoked each time when
 *          mv_inference_pose_landmark_detect() is called to provide
 *          the results of the pose landmark detection.
 *
 * @since_tizen 6.0
 * @remarks The @a locations should not be released by app. They can be used only in the callback.
 *
 * @param[in] source               The handle to the source of the media where
 *                                 landmarks were detected. @a source is the same object
 *                                 for which mv_inference_pose_landmark_detect() was called.
 *                                 It should be released by calling mv_destroy_source()
 *                                 when it's not needed anymore.
 * @param[in] locations            Locations of the detected pose landmarks.
 * @param[in] user_data            The user data passed from callback invoking code
 *
 * @see mv_inference_pose_landmark_detect()
 */
typedef void (*mv_inference_pose_landmark_detected_cb)(mv_source_h source, mv_inference_pose_result_h locations,
													   void *user_data);

/**
 * @deprecated Deprecated since 9.0
 * @brief Performs pose landmarks detection on the @a source.
 * @details Use this function to launch pose landmark detection.
 *          Each time when mv_inference_pose_landmark_detect() is
 *          called, @a detected_cb will receive a list of pose landmark's locations
 *          in the media source.
 *
 * @since_tizen 6.0
 * @remarks This function is synchronous and may take considerable time to run.
 *
 * @param[in] source         The handle to the source of the media
 * @param[in] infer          The handle to the inference
 * @param[in] roi            Rectangular area including a face in @a source which
 *                           will be analyzed. If NULL, then the whole source will be
 *                           analyzed.
 * @param[in] detected_cb    The callback which will receive the detection results.
 * @param[in] user_data      The user data passed from the code where
 *                           mv_inference_pose_landmark_detect() is invoked.
 *                           This data will be accessible in @a detected_cb callback.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INTERNAL          Internal error
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Source colorspace isn't supported
 *
 * @pre Create a source handle by calling mv_create_source()
 * @pre Create an inference handle by calling mv_inference_create()
 * @pre Configure an inference handle by calling mv_inference_configure()
 * @pre Prepare an inference by calling mv_inference_prepare()
 * @post @a detected_cb will be called to provide detection results
 *
 * @see mv_inference_pose_landmark_detected_cb()
 */
int mv_inference_pose_landmark_detect(mv_source_h source, mv_inference_h infer, mv_rectangle_s *roi,
									  mv_inference_pose_landmark_detected_cb detected_cb, void *user_data);

/**
 * @deprecated Deprecated since 9.0
 * @brief Gets the number of poses.
 *
 * @since_tizen 6.0
 *
 * @param[in] result           The handle to inference result
 * @param[out] number_of_poses The pointer to the number of poses
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mv_inference_pose_landmark_detected_cb()
 * @see mv_inference_pose_result_h
 */
int mv_inference_pose_get_number_of_poses(mv_inference_pose_result_h result, int *number_of_poses);

/**
 * @deprecated Deprecated since 9.0
 * @brief Gets the number of landmarks per a pose.
 *
 * @since_tizen 6.0
 *
 * @param[in] result               The handle to inference result
 * @param[out] number_of_landmarks The pointer to the number of landmarks
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mv_inference_pose_landmark_detected_cb()
 * @see mv_inference_pose_result_h
 */
int mv_inference_pose_get_number_of_landmarks(mv_inference_pose_result_h result, int *number_of_landmarks);

/**
 * @deprecated Deprecated since 9.0
 * @brief Gets landmark location of a part of a pose.
 *
 * @since_tizen 6.0
 *
 * @param[in] result         The handle to inference result
 * @param[in] pose_index     The pose index between 0 and
 *                           the number of poses which can be gotten by
 *                           mv_inference_pose_get_number_of_poses()
 * @param[in] pose_part      The landmark index between 0 and
 *                           the number of landmarks which can be gotten by
 *                           mv_inference_pose_get_number_of_landmarks()
 * @param[out] location      The location of a landmark
 * @param[out] score         The score of a landmark
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mv_inference_pose_get_number_of_poses()
 * @see mv_inference_pose_get_number_of_landmarks()
 * @see mv_inference_pose_landmark_detected_cb()
 * @see mv_inference_pose_result_h
 */
int mv_inference_pose_get_landmark(mv_inference_pose_result_h result, int pose_index, int pose_part,
								   mv_point_s *location, float *score);

/**
 * @deprecated Deprecated since 9.0
 * @brief Gets a label of a pose.
 *
 * @since_tizen 6.0
 *
 * @param[in] result         The handle to inference result
 * @param[in] pose_index     The pose index between 0 and
 *                           the number of poses which can be gotten by
 *                           mv_inference_pose_get_number_of_poses()
 * @param[out] label         The label of a pose
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mv_inference_pose_get_number_of_poses()
 * @see mv_inference_pose_get_number_of_landmarks()
 * @see mv_inference_pose_landmark_detected_cb()
 * @see mv_inference_pose_result_h
 */
int mv_inference_pose_get_label(mv_inference_pose_result_h result, int pose_index, int *label);

/**
 * @deprecated Deprecated since 9.0
 * @brief Creates pose handle.
 * @details Use this function to create a pose.
 *
 * @since_tizen 6.0
 *
 * @remarks The @a pose should be released using mv_pose_destroy().
 *
 * @param[out] pose    The handle to the pose to be created
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mv_pose_destroy()
 */
int mv_pose_create(mv_pose_h *pose);

/**
 * @deprecated Deprecated since 9.0
 * @brief Destroys pose handle and releases all its resources.
 *
 * @since_tizen 6.0
 *
 * @param[in] pose    The handle to the pose to be destroyed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create pose handle by using mv_pose_create()
 *
 * @see mv_pose_create()
 */
int mv_pose_destroy(mv_pose_h pose);

/**
 * @deprecated Deprecated since 9.0
 * @brief Sets a motion capture file and its pose mapping file to the pose.
 * @details Use this function to set a motion capture file and
 *          its pose mapping file. These are used by mv_pose_compare()
 *          to compare a given pose by mv_inference_pose_landmark_estimation().
 *
 *
 * @since_tizen 6.0
 * @remarks If the app sets paths to media storage,
 *          then the media storage privilege
 *          %http://tizen.org/privilege/mediastorage is needed.\n
 *          If the app sets the paths to external storage,
 *          then the external storage privilege
 *          %http://tizen.org/privilege/externalstorage is needed.\n
 *          If the required privileges aren't set properly,
 *          mv_pose_set_from_file() will returned #MEDIA_VISION_ERROR_PERMISSION_DENIED.
 *
 * @param[in] pose                  The handle to the pose
 * @param[in] motion_capture_file_path The file path to the motion capture file
 * @param[in] motion_mapping_file_path The file path to the motion mapping file
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_PATH Invalid path of capture or mapping file
 * @retval #MEDIA_VISION_ERROR_INTERNAL Internal error
 */
int mv_pose_set_from_file(mv_pose_h pose, const char *motion_capture_file_path, const char *motion_mapping_file_path);

/**
 * @deprecated Deprecated since 9.0
 * @brief Compares an action pose with the pose which is set by mv_pose_set_from_file().
 * @details Use this function to compare action pose with the pose
 *          which is set by mv_pose_set_from_file().
 *          Parts to be compared can be selected by #mv_inference_human_body_part_e.
 *          Their similarity will be given by the score between 0 ~ 1.
 *
 * @since_tizen 6.0
 * @remarks If @a action contains multiple poses, the first pose is used for comparison.
 *
 * @param[in] pose   The handle to the pose
 * @param[in] action The action pose
 * @param[in] parts  The parts to be compared
 * @param[out] score The similarity score
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Sets the pose by using mv_pose_set_from_file()
 * @pre Detects the pose by using mv_inference_pose_landmark_detect()
 */
int mv_pose_compare(mv_pose_h pose, mv_inference_pose_result_h action, int parts, float *score);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_INFERENCE_H__ */
