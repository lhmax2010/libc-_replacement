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

#ifndef __MV_ML_CONFIG_H__
#define __MV_ML_CONFIG_H__

#include "EngineConfig.h"
#include "mv_private.h"
#include <mv_common.h>

#include "MetaParser.h"

namespace mediavision
{
namespace machine_learning
{
class Config
{
private:
	std::unique_ptr<MetaParser> _parser;
	std::string _defaultModelName;
	std::string _modelFilePath;
	std::string _modelMetaFilePath;
	std::string _modelDefaultPath;
	std::string _modelLabelFilePath;
	std::string _pluginFileName;
	bool _usePlugin {};
	int _backendType { MV_INFERENCE_BACKEND_NONE };
	int _targetDeviceType { MV_INFERENCE_TARGET_DEVICE_NONE };
	double _confidence_threshold {};

public:
	Config();
	virtual ~Config() = default;

	void setUserModel(const std::string &model_file, const std::string &meta_file, const std::string &label_file);
	void parseConfigFile(const std::string &configFilePath);
	void parsePluginConfigFile(const std::string &pluginConfigFilePath);
	void parseMetaFile();
	void setBackendType(int backend_type);
	void setTargetDeviceType(int device_type);
	const std::string &getDefaultModelName() const;
	const std::string &getModelFilePath() const;
	const std::string &getLabelFilePath() const;
	const std::string &getPluginFileName() const;
	MetaMap &getInputMetaMap() const;
	MetaMap &getOutputMetaMap() const;
	double getConfidenceThreshold() const;
	int getBackendType() const;
	int getTargetDeviceType() const;
	bool isPluginUsed() const;
	void loadMetaFile(std::unique_ptr<MetaParser> parser);
};

} // machine_learning
} // mediavision

#endif