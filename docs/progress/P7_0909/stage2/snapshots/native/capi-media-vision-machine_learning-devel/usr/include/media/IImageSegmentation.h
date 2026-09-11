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

#ifndef __IIMAGE_SEGMENTATION_H__
#define __IIMAGE_SEGMENTATION_H__

#include <mv_common.h>

#include "image_segmentation_type.h"

namespace mediavision
{
namespace machine_learning
{
class IImageSegmentation
{
public:
	virtual ~IImageSegmentation() {};

	virtual void preDestroy() = 0;
	virtual void setUserModel(std::string model_file, std::string meta_file, std::string label_file) = 0;
	virtual void setEngineInfo(std::string engine_type, std::string device_type) = 0;
	virtual unsigned int getNumberOfEngines() = 0;
	virtual const std::string &getEngineType(unsigned int engine_index) = 0;
	virtual unsigned int getNumberOfDevices(const std::string &engine_type) = 0;
	virtual const std::string &getDeviceType(const std::string &engine_type, const unsigned int device_index) = 0;
	virtual void configure() = 0;
	virtual void prepare() = 0;
	virtual void perform(mv_source_h &mv_src) = 0;
	virtual void performAsync(ImageSegmentationInput &input) = 0;
	virtual ImageSegmentationResult &getOutput() = 0;
	virtual ImageSegmentationResult &getOutputCache() = 0;
};

} // machine_learning
} // mediavision

#endif