/*
 * Copyright (c) 2011 - 2015 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_SYSTEM_SYSTEM_INFO_H__
#define __TIZEN_SYSTEM_SYSTEM_INFO_H__

#include "system_info_type.h"

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @addtogroup CAPI_SYSTEM_SYSTEM_INFO_MODULE
 * @{
 */


/**
 * @brief Gets the boolean type value of the @a platform feature.
 * @details The keys for the platform functions are specified by the Tizen platform and can be seen <a href="https://docs.tizen.org/application/native/guides/device/system">here.</a>
 * @since_tizen 2.3
 * @param[in] key The name of the platform feature to get
 * @param[out] value The value of the given platform feature
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SYSTEM_INFO_ERROR_NONE Successful
 * @retval #SYSTEM_INFO_ERROR_INVALID_PARAMETER Cannot find the @a key in the model config file
 * @retval #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred while reading the @a value from the model config file
 * @retval #SYSTEM_INFO_ERROR_PERMISSION_DENIED No permission to use the API
 */
int system_info_get_platform_bool(const char *key, bool *value);


/**
 * @brief Gets the integer type value of the @a platform feature.
 * @details The keys for the platform functions are specified by the Tizen platform and can be seen <a href="https://docs.tizen.org/application/native/guides/device/system">here.</a>
 * @since_tizen 2.3
 * @param[in] key The name of the platform feature to get
 * @param[out] value The value of the given platform feature
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SYSTEM_INFO_ERROR_NONE Successful
 * @retval #SYSTEM_INFO_ERROR_INVALID_PARAMETER Cannot find the @a key in the model config file
 * @retval #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred while reading the @a value from the model config file
 * @retval #SYSTEM_INFO_ERROR_PERMISSION_DENIED No permission to use the API
 */
int system_info_get_platform_int(const char *key, int *value);


/**
 * @brief Gets the double type value of the @a platform feature.
 * @details The keys for the platform functions are specified by the Tizen platform and can be seen <a href="https://docs.tizen.org/application/native/guides/device/system">here.</a>
 * @since_tizen 2.3
 * @param[in] key The name of the platform feature to get
 * @param[out] value The value of the given platform feature
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SYSTEM_INFO_ERROR_NONE Successful
 * @retval #SYSTEM_INFO_ERROR_INVALID_PARAMETER Cannot find the @a key in the model config file
 * @retval #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred while reading the @a value from the model config file
 * @retval #SYSTEM_INFO_ERROR_PERMISSION_DENIED No permission to use the API
 */
int system_info_get_platform_double(const char *key, double *value);


/**
 * @brief Gets the string type value of the @a platform feature.
 * @details The keys for the platform functions are specified by the Tizen platform and can be seen <a href="https://docs.tizen.org/application/native/guides/device/system">here.</a>
 * @since_tizen 2.3
 * @remarks You must release the @a value using free().
 * @param[in] key The name of the platform feature to get
 * @param[out] value The value of the given platform feature
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SYSTEM_INFO_ERROR_NONE Successful
 * @retval #SYSTEM_INFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SYSTEM_INFO_ERROR_INVALID_PARAMETER Cannot find the @a key in the model config file
 * @retval #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred while reading the @a value from the model config file
 * @retval #SYSTEM_INFO_ERROR_PERMISSION_DENIED No permission to use the API
 */
int system_info_get_platform_string(const char *key, char **value);


/**
 * @brief Gets the boolean type value of the @a custom feature.
 * @details This custom function uses a custom key which is provided by OEM's.
 * @since_tizen 2.3
 * @param[in] key The name of the custom feature to get.
 * @param[out] value The value of the given custom feature
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SYSTEM_INFO_ERROR_NONE Successful
 * @retval #SYSTEM_INFO_ERROR_INVALID_PARAMETER Cannot find the @a key in the model config file
 * @retval #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred while reading the @a value from the model config file
 * @retval #SYSTEM_INFO_ERROR_PERMISSION_DENIED No permission to use the API
 */
int system_info_get_custom_bool(const char *key, bool *value);


/**
 * @brief Gets the integer type value of the @a custom feature.
 * @details This custom function uses a custom key which is provided by OEM's.
 * @since_tizen 2.3
 * @param[in] key The name of the custom feature to get.
 * @param[out] value The value of the given custom feature
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SYSTEM_INFO_ERROR_NONE Successful
 * @retval #SYSTEM_INFO_ERROR_INVALID_PARAMETER Cannot the find @a key in the model config file
 * @retval #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred while reading the @a value from the model config file
 * @retval #SYSTEM_INFO_ERROR_PERMISSION_DENIED No permission to use the API
 */
int system_info_get_custom_int(const char *key, int *value);


/**
 * @brief Gets the double type value of the @a custom feature.
 * @details This custom function uses a custom key which is provided by OEM's.
 * @since_tizen 2.3
 * @param[in] key The name of the custom feature to get.
 * @param[out] value The value of the given custom feature
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SYSTEM_INFO_ERROR_NONE Successful
 * @retval #SYSTEM_INFO_ERROR_INVALID_PARAMETER Cannot find the @a key in the model config file
 * @retval #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred while reading the @a value from the model config file
 * @retval #SYSTEM_INFO_ERROR_PERMISSION_DENIED No permission to use the API
 */
int system_info_get_custom_double(const char *key, double *value);


/**
 * @brief Gets the string type value of the @a custom feature.
 * @details This custom function uses a custom key which is provided by OEM's.
 * @since_tizen 2.3
 * @remarks You must release the @a value using free().
 * @param[in] key The name of the custom feature to get.
 * @param[out] value The value of the given custom feature
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SYSTEM_INFO_ERROR_NONE Successful
 * @retval #SYSTEM_INFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SYSTEM_INFO_ERROR_INVALID_PARAMETER Cannot find the @a key in the model config file
 * @retval #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred while reading the @a value from the model config file
 * @retval #SYSTEM_INFO_ERROR_PERMISSION_DENIED No permission to use the API
 */
int system_info_get_custom_string(const char *key, char **value);


/**
 * @internal
 * @brief Do not use it. It will be removed.
 */
int system_info_get_value_int(system_info_key_e key, int *value);


/**
 * @internal
 * @brief Do not use it. It will be removed.
 */
int system_info_get_value_bool(system_info_key_e key, bool *value);


/**
 * @internal
 * @brief Do not use it. It will be removed.
 */
int system_info_get_value_double(system_info_key_e key, double *value);


/**
 * @internal
 * @brief Do not use it. It will be removed.
 */
int system_info_get_value_string(system_info_key_e key, char **value);


/**
 * @internal
 * @brief It is not decided if it should be opened to public.
 */
int system_info_get_platform_type(const char *key, system_info_type_e *type);


/**
 * @internal
 * @brief It is not decided if it should be opened to public.
 */
int system_info_get_custom_type(const char *key, system_info_type_e *type);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_SYSTEM_INFO_H__ */
