/*
 * Copyright (c) 2025 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef API_ACTION_ERROR_H_
#define API_ACTION_ERROR_H_

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration for action framework errors.
 * @since_tizen 10.0
 */
typedef enum _action_error {
	ACTION_ERROR_NONE = TIZEN_ERROR_NONE, /**< Success */
	ACTION_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< I/O error */
	ACTION_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	ACTION_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	ACTION_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	ACTION_ERROR_INVALID_MODEL = TIZEN_ERROR_INVALID_OPERATION, /**< Invalid model */
	ACTION_ERROR_CONNECTION_REFUSED = TIZEN_ERROR_CONNECTION_REFUSED, /**< Connection refused */
} action_error_e;

#ifdef __cplusplus
}
#endif

#endif  // API_ACTION_ERROR_H_
