/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef __RUA_TYPES_H__
#define __RUA_TYPES_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration for RUA error.
 * @since_tizen 5.5
 */
typedef enum {
	RUA_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	RUA_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	RUA_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	RUA_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	RUA_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< IO error */
	RUA_ERROR_NO_SUCH_APP = TIZEN_ERROR_NO_SUCH_FILE, /**< No such application */
} rua_error_e;

#ifdef __cplusplus
}
#endif

#endif /*__RUA_TYPES_H__*/
