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

#ifndef __MV_ML_TYPE_H__
#define __MV_ML_TYPE_H__

#include <mv_common.h>
#include <mv_inference_type.h>

namespace mediavision
{
namespace machine_learning
{
struct InputBaseType {
	mv_source_h inference_src {};
	InputBaseType(mv_source_h src = NULL) : inference_src(src)
	{}
	virtual ~InputBaseType()
	{}
};

struct OutputBaseType {
	unsigned long frame_number {};
	virtual ~OutputBaseType()
	{}
};

} // machine_learning
} // mediavision

#endif