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

#ifndef __MV_ML_EXCEPTION_H__
#define __MV_ML_EXCEPTION_H__

#include <exception>
#include <string>

#include <mv_common.h>

namespace mediavision
{
namespace machine_learning
{
namespace exception
{
class BaseException : public std::exception
{
private:
	std::string _msg;
	int _errorType;

public:
	BaseException(std::string msg, int errorType) : _msg(msg), _errorType(errorType)
	{}
	~BaseException() override = default;

	const char *what() const noexcept override
	{
		return _msg.c_str();
	}
	int getError() const
	{
		return _errorType;
	}
};

class InvalidParameter : public BaseException
{
public:
	InvalidParameter(std::string msg) : BaseException("Invalid parameter: " + msg, MEDIA_VISION_ERROR_INVALID_PARAMETER)
	{}
	~InvalidParameter() override = default;
};

class InvalidOperation : public BaseException
{
public:
	InvalidOperation(std::string msg, int errorType = MEDIA_VISION_ERROR_INVALID_OPERATION)
			: BaseException("Invalid operation: " + msg, errorType)
	{}
	~InvalidOperation() final = default;
};

class OutOfMemory : public BaseException
{
public:
	OutOfMemory(std::string msg) : BaseException("Out of memory: " + msg, MEDIA_VISION_ERROR_OUT_OF_MEMORY)
	{}
	~OutOfMemory() final = default;
};

class NoData : public BaseException
{
public:
	NoData(std::string msg) : BaseException("No Data: " + msg, MEDIA_VISION_ERROR_NO_DATA)
	{}
	~NoData() final = default;
};

}; // Exception
}; // MachineLearning
}; // Mediavision

#endif // __MV_ML_EXCEPTION_H__
