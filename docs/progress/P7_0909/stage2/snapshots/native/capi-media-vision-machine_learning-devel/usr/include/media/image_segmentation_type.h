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

#ifndef __IMAGE_SEGMENTATION_TYPE_H__
#define __IMAGE_SEGMENTATION_TYPE_H__

#include <opencv2/core.hpp>

#include "mv_ml_types.h"
#include <mv_common.h>
#include <mv_inference_type.h>

namespace mediavision
{
namespace machine_learning
{
struct ImageSegmentationInput : public InputBaseType {
	ImageSegmentationInput(mv_source_h src = nullptr) : InputBaseType(src)
	{}
};

/**
 * @brief The object detection result structure.
 * @details Contains object detection result.
 */
struct ImageSegmentationResult : public OutputBaseType {
	unsigned int width {};
	unsigned int height {};
	unsigned int pixel_size {};
	std::vector<unsigned char> data;
};

enum class ImageSegmentationTaskType {
	IMAGE_SEGMENTATION_TASK_NONE = 0,
	SELFIE_SEGMENTATION,
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