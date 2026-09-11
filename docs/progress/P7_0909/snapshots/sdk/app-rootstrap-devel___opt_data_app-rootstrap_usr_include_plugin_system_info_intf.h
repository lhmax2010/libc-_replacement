/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
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

/**
 * @file system_info_intf.h
 * @brief This file contains the system information plugin interface definitions.
 * @details This file defines the interface for external system information plugins.
 *          Plugins can implement custom system information keys and register them
 *          with the system information framework.
 */

#ifndef __TIZEN_SYSTEM_SYSTEM_INFO_INTF_H__
#define __TIZEN_SYSTEM_SYSTEM_INFO_INTF_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

/**
 * @addtogroup CAPI_SYSTEM_SYSTEM_INFO_INTERNAL_MODULE
 * @{
 */

/**
 * @addtogroup CAPI_SYSTEM_SYSTEM_INFO_PLUGIN_MODULE
 * @{
 */

/**
 * @brief Tag name for custom system information
 */
#define TAG_CUSTOM	"custom"

/**
 * @brief Type string for boolean values
 */
#define TYPE_BOOL	"bool"

/**
 * @brief Type string for integer values
 */
#define TYPE_INT	"int"

/**
 * @brief Type string for double values
 */
#define TYPE_DBL	"double"

/**
 * @brief Type string for string values
 */
#define TYPE_STR	"string"

/**
 * @brief Structure containing function pointers for system information external plugin interface
 * @details This structure defines the interface that external system information plugins
 *          must implement. Plugins should provide implementations for both get_value_external
 *          and get_type_external function pointers.
 * @since_tizen 2.3
 */
typedef struct {
	/**
	 * @brief Function pointer to get the value of an external system information
	 * @details This function is called by the system information framework to retrieve
	 *          the value of a custom system information key. The value must be stored
	 *          as a string in the provided buffer, regardless of the actual data type.
	 *          For boolean values, only "true" or "TRUE" are recognized as true values.
	 * @param[in] tag The tag name of the system information (e.g., "custom")
	 * @param[in] key The key name of the system information
	 * @param[in] type The type string of the value (e.g., "bool", "int", "double", "string")
	 * @param[out] buf The buffer to store the value
	 * @param[in] len The length of the buffer
	 * @return @c 0 on success,
	 *         otherwise a negative error value
	 * @retval #SYSTEM_INFO_ERROR_NONE Successful
	 * @retval #SYSTEM_INFO_ERROR_INVALID_PARAMETER Invalid parameter
	 * @retval #SYSTEM_INFO_ERROR_OUT_OF_MEMORY Out of memory
	 */
	int (*get_value_external)(const char *tag,
			const char *key, const char *type,
			char *buf, unsigned int len);

	/**
	 * @brief Function pointer to get the type of an external system information
	 * @details This function is called by the system information framework to retrieve
	 *          the type string of a custom system information key. The type string should
	 *          be one of the predefined type constants: TYPE_BOOL, TYPE_INT, TYPE_DBL, or TYPE_STR.
	 * @param[in] tag The tag name of the system information (e.g., "custom")
	 * @param[in] key The key name of the system information
	 * @param[out] buf The buffer to store the type string
	 * @param[in] len The length of the buffer
	 * @return @c 0 on success,
	 *         otherwise a negative error value
	 * @retval #SYSTEM_INFO_ERROR_NONE Successful
	 * @retval #SYSTEM_INFO_ERROR_INVALID_PARAMETER Invalid parameter
	 * @retval #SYSTEM_INFO_ERROR_OUT_OF_MEMORY Out of memory
	 */
	int (*get_type_external)(const char *tag,
			const char *key, char *buf, unsigned int len);

} system_info_external_plugin_interface;

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_SYSTEM_INFO_PRIVATE_H__ */
