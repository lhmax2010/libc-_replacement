/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_APPFW_JOB_ERROR_H__
#define __TIZEN_APPFW_JOB_ERROR_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_JOB_SCHEDULER_MODULE
 * @{
 */

/**
 * @brief Enumeration for job error.
 * @since_tizen 4.0
 */
typedef enum {
	JOB_ERROR_NONE = TIZEN_ERROR_NONE,                              /**< Successful */
	JOB_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,    /**< Invalid parameter */
	JOB_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,            /**< Out of memory */
	JOB_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR,                      /**< IO error */
	JOB_ERROR_NOT_INITIALIZED = TIZEN_ERROR_JOB_SCHEDULER | 0x21,   /**< Not initialized */
	JOB_ERROR_NO_SUCH_JOB = TIZEN_ERROR_JOB_SCHEDULER | 0x22,       /**< No such job */
	JOB_ERROR_ALREADY_EXIST = TIZEN_ERROR_JOB_SCHEDULER | 0x23,     /**< Already exist */
	JOB_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,    /**< Permission denied */
	JOB_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED             /**< Not supported */
} job_error_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_JOB_ERROR_H__ */
