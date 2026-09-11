/**
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

#ifndef __OBJECT_DETECTION_TYPE_H__
#define __OBJECT_DETECTION_TYPE_H__

#include <opencv2/core.hpp>

#include "mv_ml_types.h"
#include <mv_common.h>
#include <mv_inference_type.h>

namespace mediavision
{
namespace machine_learning
{
struct ObjectDetectionInput : public InputBaseType {
	ObjectDetectionInput(mv_source_h src = nullptr) : InputBaseType(src)
	{}
};

/**
 * @brief The object detection result structure.
 * @details Contains object detection result.
 */
struct ObjectDetectionResult : public OutputBaseType {
	unsigned int number_of_objects {};
	std::vector<unsigned int> indices;
	std::vector<std::string> names;
	std::vector<float> confidences;
	std::vector<int> left;
	std::vector<int> top;
	std::vector<int> right;
	std::vector<int> bottom;
};

enum class ObjectDetectionTaskType {
	OBJECT_DETECTION_TASK_NONE = 0,
	MOBILENET_V1_SSD,
	MOBILENET_V2_SSD,
	FD_MOBILENET_V1_SSD,
	OD_TRIV2,
	FD_TRIV2,
	HD_PALM
	// TODO
};

struct Box {
	int index; /**< index of box belonging to a category */
	float score; /**< score of box belonging to the index */
	cv::Rect2f location; /**< location of a box */
};

}
}

#endif