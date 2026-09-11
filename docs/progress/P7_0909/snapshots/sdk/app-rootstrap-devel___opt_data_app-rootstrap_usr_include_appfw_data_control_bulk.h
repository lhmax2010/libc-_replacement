/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_APPFW_DATA_CONTROL_BULK_H__
#define __TIZEN_APPFW_DATA_CONTROL_BULK_H__

#include <data_control_types.h>
#include <bundle.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file data_control_bulk.h
 * @brief This is the header file for bulk data feature of the Data Control module. \n
 *        All callbacks are called in the main loop context, unless stated otherwise.
 */


/**
 * @addtogroup CAPI_DATA_CONTROL_CONSUMER_MODULE
 * @{
 */


/**
 * @brief Called when a response is received for a bulk operation from a provider application.
 * @since_tizen 3.0
 *
 * @param[in] request_id The request ID
 * @param[in] provider The provider handle. @a provider is valid only inside this function. \n
 *                     @a provider should not be destroyed.
 * @param[in] bulk_results The result data for each insert request. @a bulk_results is valid only inside this function. \n
 *                         @a bulk_results should not be freed.
 * @param[in] provider_result Set to @c true if the data control provider successfully processed, \n
 *                            otherwise set to @c false
 * @param[in] error The error message from the data control provider. @a error is valid only inside this function. \n
 *                  @a error should not be freed.
 * @param[in] user_data The user data passed from the register function
 * @see data_control_sql_register_insert_bulk_data_response_cb()
 * @see data_control_map_register_add_bulk_data_response_cb()
 */
typedef void (*data_control_bulk_cb)(int request_id, data_control_h provider, data_control_bulk_result_data_h bulk_results, bool provider_result, const char *error, void *user_data);


/**
 * @brief Gets the n-th bundle from bulk data.
 * @since_tizen 3.0
 * @remarks The @a data should not be released. \n
 *          It will be released when data_control_bulk_data_destroy() is called.
 * @param[in] bulk_data The bulk data handle
 * @param[in] idx The bundle index. Index starts at 0
 * @param[out] data The n-th bundle.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #DATA_CONTROL_ERROR_NONE Successful
 * @retval #DATA_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see data_control_bulk_data_add()
 * @see data_control_bulk_data_destroy()
 */
int data_control_bulk_data_get_data(data_control_bulk_data_h bulk_data, int idx, bundle **data);


/**
 * @brief Gets the bulk data bundle count.
 * @since_tizen 3.0
 * @param[in] bulk_data The bulk data handle
 * @param[out] count The bundle count
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #DATA_CONTROL_ERROR_NONE Successful
 * @retval #DATA_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int data_control_bulk_data_get_count(data_control_bulk_data_h bulk_data, int *count);


/**
 * @brief Adds a bundle to bulk data.
 * @since_tizen 3.0
 * @remarks The order in which bundles are added is preserved. Bundle indexes start from 0.
 * @param[in] bulk_data The bulk data handle
 * @param[in] data The bundle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #DATA_CONTROL_ERROR_NONE Successful
 * @retval #DATA_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DATA_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 */
int data_control_bulk_data_add(data_control_bulk_data_h bulk_data, bundle *data);


/**
 * @brief Creates bulk data.
 * @since_tizen 3.0
 * @remarks The @a bulk_data should be released using data_control_bulk_data_destroy().
 * @param[out] bulk_data The bulk data handle
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #DATA_CONTROL_ERROR_NONE Successful
 * @retval #DATA_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DATA_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 */
int data_control_bulk_data_create(data_control_bulk_data_h *bulk_data);


/**
 * @brief Destroys bulk data.
 * @since_tizen 3.0
 * @param[in] bulk_data The bulk data handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #DATA_CONTROL_ERROR_NONE Successful
 * @retval #DATA_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int data_control_bulk_data_destroy(data_control_bulk_data_h bulk_data);


/**
 * @brief Gets the n-th bundle from bulk result data.
 * @since_tizen 3.0
 * @remarks The @a data should not be released. \n
 *          It will be released when data_control_bulk_result_data_destroy() is called.
 * @param[in] result_data The bulk result data handle
 * @param[in] idx The bundle index. Index starts at 0
 * @param[out] data The n-th bundle
 * @param[out] result Bulk operation result. Possible values and their meaning are defined by the developer.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #DATA_CONTROL_ERROR_NONE Successful
 * @retval #DATA_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see data_control_bulk_result_data_add()
 * @see data_control_bulk_result_data_destroy()
 */
int data_control_bulk_result_data_get_result_data(data_control_bulk_result_data_h result_data, int idx, bundle **data, int *result);


/**
 * @brief Gets the bulk result data bundle count.
 * @since_tizen 3.0
 * @param[in] result_data The bulk result data handle
 * @param[out] count The bundle count
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #DATA_CONTROL_ERROR_NONE Successful
 * @retval #DATA_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int data_control_bulk_result_data_get_count(data_control_bulk_result_data_h result_data, int *count);


/**
 * @brief Adds bulk data.
 * @since_tizen 3.0
 * @remarks The order in which bundles are added is preserved. Bundle indexes start from 0.
 * @param[in] result_data The bulk result data handle
 * @param[in] data The bundle
 * @param[in] result Bulk operation result. Possible values and their meaning are defined by the developer.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #DATA_CONTROL_ERROR_NONE Successful
 * @retval #DATA_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DATA_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @see data_control_bulk_result_data_get_result_data()
 */
int data_control_bulk_result_data_add(data_control_bulk_result_data_h result_data, bundle *data, int result);


/**
 * @brief Creates bulk result data.
 * @since_tizen 3.0
 * @remarks The @a result_data should be released using data_control_bulk_result_data_destroy().
 * @param[out] result_data The bulk result data handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #DATA_CONTROL_ERROR_NONE Successful
 * @retval #DATA_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DATA_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 */
int data_control_bulk_result_data_create(data_control_bulk_result_data_h *result_data);


/**
 * @brief Destroys bulk result data.
 * @since_tizen 3.0
 * @param[in] result_data The bulk result data handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #DATA_CONTROL_ERROR_NONE Successful
 * @retval #DATA_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int data_control_bulk_result_data_destroy(data_control_bulk_result_data_h result_data);


/**
* @}
*/


#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_DATA_CONTROL_BULK_H__ */
