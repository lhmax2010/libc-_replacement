/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MEDIAVISION_INFERENCE_PRIVATE_H__
#define __TIZEN_MEDIAVISION_INFERENCE_PRIVATE_H__

#include <mv_common.h>

#define MAX_NUMBER_OF_POSE 5
#define MAX_NUMBER_OF_LANDMARKS_PER_POSE 500

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file  mv_inference_private.h
 * @brief This file contains internal structure for pose.
 */

/**
  * @brief Position, score, and label for describing a landmark.
  *
  * @since_tizen 6.0
  *
  */
typedef struct mv_inference_landmark_s {
	bool isAvailable; /**< Availability of landmark */
	mv_point_s point; /**< 2D position of landmark */
	int label; /**< Label of landmark */
	float score; /**< Score of landmark */

	mv_inference_landmark_s()
	{
		isAvailable = false;
		point.x = -1;
		point.y = -1;
		label = -1;
		score = -1.0f;
	}
} mv_inference_landmark_s;

/**
  * @brief Landmarks of a pose.
  *
  * @since_tizen 6.0
  *
  */
typedef struct {
	int number_of_poses;
	int number_of_landmarks_per_pose; /**< The number of landmarks*/
	mv_inference_landmark_s landmarks[MAX_NUMBER_OF_POSE]
									 [MAX_NUMBER_OF_LANDMARKS_PER_POSE]; /**< A set of landmarks describing pose */
} mv_inference_pose_s;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_INFERENCE_TYPE_H__ */
