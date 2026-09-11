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

#ifndef __TIZEN_MEDIAVISION_INFERENCE_TYPE_H__
#define __TIZEN_MEDIAVISION_INFERENCE_TYPE_H__

#include <mv_common.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @deprecated Deprecated since 9.0
 * @file  mv_inference_type.h
 * @brief This file contains enumerations and a handle required by
 *        MediaVision inference API.
 */

/**
 * @addtogroup CAPI_MEDIA_VISION_INFERENCE_MODULE
 * @{
 */

/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for inference backend.
 * #MV_INFERENCE_BACKEND_OPENCV An open source computer vision and machine learning
 *                              software library.
 *                              (https://opencv.org/about/)
 * #MV_INFERENCE_BACKEND_TFLITE Google-introduced open source inference engine for embedded systems,
 *                              which runs Tensorflow Lite model.
 *                              (https://www.tensorflow.org/lite/guide/get_started)
 * #MV_INFERENCE_BACKEND_ARMNN ARM-introduced open source inference engine for CPUs, GPUs and NPUs, which
 *                             enables efficient translation of existing neural network frameworks
 *                             such as TensorFlow, TensorFlow Lite and Caffes, allowing them to
 *                             run efficiently without modification on Embedded hardware.
 *                             (https://developer.arm.com/ip-products/processors/machine-learning/arm-nn)
 * #MV_INFERENCE_BACKEND_MLAPI Samsung-introduced open source ML single API framework of NNStreamer, which
 *                             runs various NN models via tensor filters of NNStreamer. (Deprecated since 7.0)
 *                             (https://github.com/nnstreamer/nnstreamer)
 * #MV_INFERENCE_BACKEND_ONE Samsung-introduced open source inference engine called On-device Neural Engine, which
 *                           performs inference of a given NN model on various devices such as CPU, GPU, DSP and NPU.
 *                           (https://github.com/Samsung/ONE)
 *
 * @since_tizen 5.5
 *
 * @see mv_inference_prepare()
 */
typedef enum {
	MV_INFERENCE_BACKEND_NONE = -1, /**< None */
	MV_INFERENCE_BACKEND_OPENCV, /**< OpenCV */
	MV_INFERENCE_BACKEND_TFLITE, /**< TensorFlow-Lite */
	MV_INFERENCE_BACKEND_ARMNN, /**< @deprecated ARMNN (Since 6.0) (Deprecated since 8.0) */
	MV_INFERENCE_BACKEND_MLAPI, /**< @deprecated ML Single API of NNStreamer (Deprecated since 7.0) */
	MV_INFERENCE_BACKEND_ONE, /**< On-device Neural Engine (Since 6.0) */
	MV_INFERENCE_BACKEND_NNTRAINER, /**< NNTrainer (Since 7.0) */
	MV_INFERENCE_BACKEND_SNPE, /**< SNPE Engine (Since 7.0) */
	MV_INFERENCE_BACKEND_MAX /**< @deprecated Backend MAX (Deprecated since 7.0) */
} mv_inference_backend_type_e;

/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for inference target.
 *
 * @since_tizen 6.0
 *
 */
typedef enum {
	MV_INFERENCE_TARGET_DEVICE_NONE = 0, /**< None */
	MV_INFERENCE_TARGET_DEVICE_CPU = 1 << 0, /**< CPU */
	MV_INFERENCE_TARGET_DEVICE_GPU = 1 << 1, /**< GPU*/
	MV_INFERENCE_TARGET_DEVICE_CUSTOM = 1 << 2, /**< CUSTOM*/
	MV_INFERENCE_TARGET_DEVICE_MAX = 1 << 3 /**< Target MAX */
} mv_inference_target_device_e;

/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for input data type.
 *
 * @since_tizen 6.0
 *
 */
typedef enum {
	MV_INFERENCE_DATA_FLOAT32 = 0, /**< Data type of a given pre-trained model is float. */
	MV_INFERENCE_DATA_UINT8, /**< Data type of a given pre-trained model is unsigned char. */
	MV_INFERENCE_DATA_INT8 /**< Data type of a given pre-trained model is signed char. (Since 10.0) */
} mv_inference_data_type_e;

/**
  * @deprecated Deprecated since 9.0
  * @brief Enumeration for human pose landmark.
  *
  * @since_tizen 6.0
  *
  */
typedef enum {
	MV_INFERENCE_HUMAN_POSE_HEAD = 1, /**< Head of human pose */
	MV_INFERENCE_HUMAN_POSE_NECK, /**< Neck of human pose */
	MV_INFERENCE_HUMAN_POSE_THORAX, /**< Thorax of human pose */
	MV_INFERENCE_HUMAN_POSE_RIGHT_SHOULDER, /**< Right shoulder of human pose */
	MV_INFERENCE_HUMAN_POSE_RIGHT_ELBOW, /**< Right elbow of human pose */
	MV_INFERENCE_HUMAN_POSE_RIGHT_WRIST, /**< Right wrist of human pose */
	MV_INFERENCE_HUMAN_POSE_LEFT_SHOULDER, /**< Left shoulder of human pose */
	MV_INFERENCE_HUMAN_POSE_LEFT_ELBOW, /**< Left elbow of human pose */
	MV_INFERENCE_HUMAN_POSE_LEFT_WRIST, /**< Left wrist of human pose */
	MV_INFERENCE_HUMAN_POSE_PELVIS, /**< Pelvis of human pose */
	MV_INFERENCE_HUMAN_POSE_RIGHT_HIP, /**< Right hip of human pose */
	MV_INFERENCE_HUMAN_POSE_RIGHT_KNEE, /**< Right knee of human pose */
	MV_INFERENCE_HUMAN_POSE_RIGHT_ANKLE, /**< Right ankle of human pose */
	MV_INFERENCE_HUMAN_POSE_LEFT_HIP, /**< Left hip of human pose */
	MV_INFERENCE_HUMAN_POSE_LEFT_KNEE, /**< Left knee of human pose */
	MV_INFERENCE_HUMAN_POSE_LEFT_ANKLE /**< Left ankle of human pose */
} mv_inference_human_pose_landmark_e;

/**
  * @deprecated Deprecated since 9.0
  * @brief Enumeration for human body parts.
  *
  * @since_tizen 6.0
  *
  */
typedef enum {
	MV_INFERENCE_HUMAN_BODY_PART_HEAD = 1 << 0, /**< HEAD, NECK, and THORAX */
	MV_INFERENCE_HUMAN_BODY_PART_ARM_RIGHT = 1 << 1, /**< RIGHT SHOULDER, ELBOW, and WRIST */
	MV_INFERENCE_HUMAN_BODY_PART_ARM_LEFT = 1 << 2, /**< LEFT SHOULDER, ELBOW, and WRIST */
	MV_INFERENCE_HUMAN_BODY_PART_BODY = 1 << 3, /**< THORAX, PELVIS, RIGHT HIP, and LEFT HIP */
	MV_INFERENCE_HUMAN_BODY_PART_LEG_RIGHT = 1 << 4, /**< RIGHT HIP, KNEE, and ANKLE */
	MV_INFERENCE_HUMAN_BODY_PART_LEG_LEFT = 1 << 5 /**< LEFT HIP, KNEE, and ANKLE */
} mv_inference_human_body_part_e;

/**
 * @deprecated Deprecated since 9.0
 * @brief The inference handle.
 * @details Contains information about location of
 *          detected landmarks for one or more poses.
 * @since_tizen 5.5
 */
typedef void *mv_inference_h;

/**
 * @deprecated Deprecated since 9.0
 * @brief The inference pose result handle.
 *
 * @since_tizen 6.0
 */
typedef void *mv_inference_pose_result_h;

/**
 * @deprecated Deprecated since 9.0
 * @brief The pose handle.
 *
 * @since_tizen 6.0
 */
typedef void *mv_pose_h;
/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_INFERENCE_TYPE_H__ */
