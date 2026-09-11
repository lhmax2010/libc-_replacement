/**
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

#ifndef __GAZE_TRACKINGION_TYPE_H__
#define __GAZE_TRACKINGION_TYPE_H__

#include <opencv2/core.hpp>

#include "mv_ml_types.h"
#include <mv_common.h>
#include <mv_inference_type.h>

namespace mediavision
{
namespace machine_learning
{
struct GazeTrackingInput : public InputBaseType {
	GazeTrackingInput(mv_source_h src = nullptr) : InputBaseType(src)
	{}
};

/**
 * @brief The gaze_tracking result structure.
 * @details Contains gaze_tracking result.
 */
struct GazeTrackingResult : public OutputBaseType {
	unsigned int number_of_faces {};
	std::vector<unsigned int> indices;
	std::vector<float> confidences;
	std::vector<float> yaws;
	std::vector<float> pitches;
	std::vector<float> x_pos;
	std::vector<float> y_pos;
};

enum class GazeTrackingTaskType {
	GZE_TRACKING_TASK_NONE = 0,
	GZE_L2CS_NET,
	GZE_GENERAL_GAZE_NET,
	GZE_TINY_TRACKER,
	GZE_L2CS_NET_MOBILENETV2
	// TODO
};

}
}

#endif