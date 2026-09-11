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

#ifndef __MV_ML_PREPROCESS_H__
#define __MV_ML_PREPROCESS_H__

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <inference_engine_type.h>
#include <mv_inference_type.h>

#include "mv_common.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

/**
 * @file MvMlPreprocess.h
 * @brief This file contains the Preprocess class definition which
 *        provides Preprocess before running inference.
 */

namespace mediavision
{
namespace machine_learning
{
struct PreprocessConfig {
	bool skip_csc {}; /**< It indicates whether color space conversion operation should be skipped or not. */
	mv_colorspace_e output_format {}; /**< The pixel format of output tensor to be converted. */
	mv_inference_data_type_e output_data_type {}; /**< The data type of output tensor to be converted. */
	int output_channel {}; /** The channel size of output tensor te be converted. */
	int output_width {}; /** The width size of output tensor to be converted. */
	int output_height {}; /** The height size of output tensor to be converted. */
	bool normalize {}; /**< It indicates whether normalization to input data should be performed or not. */
	std::vector<double> mean;
	std::vector<double> std;
	bool quantize {}; /**< It indicates whether quantization to input data should be performed or not. */
	std::vector<double> scale;
	std::vector<double> zeropoint;
};

class Preprocess
{
public:
	Preprocess()
	{}
	~Preprocess() = default;

	void setConfig(const PreprocessConfig &config);
	template<typename T> void run(mv_source_h &mv_src, std::vector<T> &inputVector);

	std::vector<unsigned int> &getImageWidth()
	{
		return _vImageWidth;
	}
	std::vector<unsigned int> &getImageHeight()
	{
		return _vImageHeight;
	}

private:
	std::vector<unsigned int> _vImageWidth;
	std::vector<unsigned int> _vImageHeight;
	PreprocessConfig _config;

	int convertToCv(int given_type, int ch);
	void colorConvert(cv::Mat &source, cv::Mat &dest, int sType, int dType);
	void convertToCvSource(std::vector<mv_source_h> &mv_srcs, std::vector<cv::Mat> &cv_srcs);
	void normalize(cv::Mat &source, cv::Mat &dest, bool signedInt8 = false);
	void quantize(cv::Mat &source, cv::Mat &dest, bool signedInt8 = false);
};

} /* machine_learning */
} /* mediavision */

#endif /* __MV_ML_PREPROCESS_H__ */
