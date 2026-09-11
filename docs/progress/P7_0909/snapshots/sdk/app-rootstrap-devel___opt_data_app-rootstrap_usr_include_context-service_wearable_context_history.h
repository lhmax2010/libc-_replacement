/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_CONTEXT_HISTORY_H__
#define __TIZEN_CONTEXT_HISTORY_H__


#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @deprecated Deprecated since 10.0
 * @addtogroup CAPI_CONTEXT_HISTORY_MODULE
 * @{
 */

/**
 * @deprecated Deprecated since 10.0
 * @brief	Definition for the attribute key denoting "application id".
 * @details	This can be used as a key of context_history_record_get_string().
 * @since_tizen 4.0
 */
#define CONTEXT_HISTORY_APP_ID "AppId"

/**
 * @deprecated Deprecated since 10.0
 * @brief	Definition for the attribute key denoting "total count".
 * @details	This can be used as a key of context_history_record_get_int().
 * @since_tizen 4.0
 */
#define CONTEXT_HISTORY_TOTAL_COUNT "TotalCount"

/**
 * @deprecated Deprecated since 10.0
 * @brief	Definition for the attribute key denoting "total amount".
 * @details	This can be used as a key of context_history_record_get_double().
 * @since_tizen 4.0
 */
#define CONTEXT_HISTORY_TOTAL_AMOUNT "TotalAmount"

/**
 * @deprecated Deprecated since 10.0
 * @brief	Definition for the attribute key denoting "total duration".
 * @details	This can be used as a key of context_history_record_get_int().
 * @since_tizen 4.0
 */
#define CONTEXT_HISTORY_TOTAL_DURATION "TotalDuration"

/**
 * @deprecated Deprecated since 10.0
 * @brief	Definition for the attribute key denoting "last time".
 * @details	This can be used as a key of context_history_record_get_int().
 * @since_tizen 4.0
 */
#define CONTEXT_HISTORY_LAST_TIME "LastTime"

/**
 * @deprecated Deprecated since 10.0
 * @brief		Enumeration for errors.
 * @since_tizen 4.0
 */
typedef enum {
	CONTEXT_HISTORY_ERROR_NONE = TIZEN_ERROR_NONE,								/**< Successful */
	CONTEXT_HISTORY_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,	/**< Invalid parameter */
	CONTEXT_HISTORY_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,	/**< Permission denied */
	CONTEXT_HISTORY_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,			/**< Not supported */
	CONTEXT_HISTORY_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,			/**< Memory allocation failed */
	CONTEXT_HISTORY_ERROR_NO_DATA = TIZEN_ERROR_NO_DATA,						/**< No Data */
	CONTEXT_HISTORY_ERROR_OUT_OF_RANGE = (TIZEN_ERROR_CONTEXT | 0x03),			/**< Out of range */
	CONTEXT_HISTORY_ERROR_OPERATION_FAILED = (TIZEN_ERROR_CONTEXT | 0x04),		/**< Operation failed */
} context_history_error_e;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Enumeration for data types of statistics and patterns.
 * @since_tizen 4.0
 */
typedef enum {
	CONTEXT_HISTORY_RECENTLY_USED_APP = 1,					/**< Recently used application @n Privilege : http://tizen.org/privilege/apphistory.read */
	CONTEXT_HISTORY_FREQUENTLY_USED_APP = 2,				/**< Frequently used application @n Privilege : http://tizen.org/privilege/apphistory.read */
	CONTEXT_HISTORY_BATTERY_USAGE = 11,						/**< Per-app battery usage in percentage
															@n Privilege : http://tizen.org/privilege/apphistory.read */
	CONTEXT_HISTORY_RECENT_BATTERY_USAGE = 12,				/**< Per-app battery usage in percentage since last charge
															@n Privilege : http://tizen.org/privilege/apphistory.read */
} context_history_data_e;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Enumeration for filters of statistics and patterns.
 * @since_tizen 4.0
 */
typedef enum {
	CONTEXT_HISTORY_FILTER_TIME_SPAN = 1,			/**< Time span of data in days */
	CONTEXT_HISTORY_FILTER_RESULT_SIZE = 2,			/**< Result size of data records */
	CONTEXT_HISTORY_FILTER_START_TIME = 5,			/**< Start time of data in epoch time */
	CONTEXT_HISTORY_FILTER_END_TIME = 6,			/**< End time of data in epoch time */
} context_history_filter_e;

/**
 * @deprecated Deprecated since 10.0
 * @brief Retrieving statistics and patterns using context history handle.
 * @since_tizen 4.0
 */
typedef struct _context_history_handle_s* context_history_h;

/**
 * @deprecated Deprecated since 10.0
 * @brief Computing statistics and patterns using history filter handle.
 * @since_tizen 4.0
 */
typedef struct _context_history_filter_handle_s* context_history_filter_h;

/**
 * @deprecated Deprecated since 10.0
 * @brief One or multiple records contained in history list handle.
 * @since_tizen 4.0
 */
typedef struct _context_history_list_handle_s* context_history_list_h;

/**
 * @deprecated Deprecated since 10.0
 * @brief One or multiple attributes contained in history list handle.
 * @since_tizen 4.0
 */
typedef struct _context_history_record_handle_s* context_history_record_h;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Creates a context history handle.
 * @since_tizen 4.0
 * @remarks		The @c handle must be released using context_history_destroy().
 *
 * @param[out] handle    Handle to be initialized
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 * @retval		#CONTEXT_HISTORY_ERROR_OUT_OF_MEMORY		Out of memory
 *
 * @see context_history_destroy()
 */
int context_history_create(context_history_h* handle) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Releases the resources occupied by a handle.
 * @details		This releases the memory allocated for the @c handle.
 *
 * @since_tizen 4.0
 *
 * @param[in]  handle   Handle to be released
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @pre	context_history_create()
 */
int context_history_destroy(context_history_h handle) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Creates a history filter.
 * @since_tizen 4.0
 * @remarks		The @c filter must be released using context_history_filter_destroy().
 *
 * @param[out]	filter		Filter handle to be initialized
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 * @retval		#CONTEXT_HISTORY_ERROR_OUT_OF_MEMORY		Memory allocation failed
 *
 * @see		context_history_filter_destroy()
 */
int context_history_filter_create(context_history_filter_h* filter) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Releases the resources occupied by a filter.
 * @details		This releases the memory allocated for the @c filter.
 * @since_tizen 4.0
 *
 * @param[in]	filter		Filter handle to be released
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 *
 * @pre		context_history_filter_create()
 */
int context_history_filter_destroy(context_history_filter_h filter) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Sets an integer value to a filter.
 * @details		This sets the value to a filter.
 * @since_tizen 4.0
 *
 * @param[in]	filter		The filter handle
 * @param[in]	filter_type	The filter type to set
 * @param[in]	value		The value to be set
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 *
 * @pre		context_history_filter_create()
  */
int context_history_filter_set_int(context_history_filter_h filter, context_history_filter_e filter_type, int value) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Checks whether a history data type is supported in the current device.
 * @since_tizen 4.0
 *
 * @param[in]	data_type		Type of history data
 * @param[out]	supported		If supported, @c true, otherwise @c false
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 * @retval		#CONTEXT_HISTORY_ERROR_OPERATION_FAILED		Operation failed
 */
int context_history_is_supported(context_history_data_e data_type, bool* supported) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Reads context statistics or patterns.
 * @details		Retrieves a given type of context statistics or patterns list.
 *				See the programming guide to find available filter type #context_history_filter_e@n
 *				for each context data type #context_history_data_e.
 * @since_tizen 4.0
 *
 * @remarks		The @c list must be released using context_history_list_destroy(). \n
 *              %http://tizen.org/privilege/apphistory.read is needed to retrieve data.
 *				It depends on context data type #context_history_data_e.
 *
 * @param[in]	handle      Handle for controlling the context history data requests
 * @param[in]	data_type	Type of the history data
 * @param[in]	filter		Filter to be used to compute statistics or patterns. This can not be @c NULL
 * @param[out]	list   History data retrieved. This should be freed after use
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_HISTORY_ERROR_PERMISSION_DENIED	Permission denied
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 * @retval		#CONTEXT_HISTORY_ERROR_OPERATION_FAILED		Operation failed
 * @retval		#CONTEXT_HISTORY_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval		#CONTEXT_HISTORY_ERROR_NO_DATA				Requested data does not exist
 *
 * @pre context_history_create()
 * @post context_history_list_destroy()
 */
int context_history_get_list(context_history_h handle, context_history_data_e data_type, context_history_filter_h filter, context_history_list_h* list) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Retrieves the number of records in a list.
 * @since_tizen 4.0
 *
 * @param[in]	list    The history data list handle
 * @param[out]	count   The count of the data list
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 * @retval		#CONTEXT_HISTORY_ERROR_OPERATION_FAILED		Operation failed
 *
 * @pre context_history_get_list()
 * @see context_history_list_move_first()
 * @see context_history_list_move_next()
 */
int context_history_list_get_count(context_history_list_h list, int* count) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Retrieves the current record from the history list.
 * @details		The default current record is the first record.
 * @since_tizen 4.0
 * @remarks		The @c record must be released using context_history_record_destroy().
 *
 * @param[in]	list      The context history list handle
 * @param[out]	record   History data record retrieved
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 * @retval		#CONTEXT_HISTORY_ERROR_OPERATION_FAILED		Operation failed
 * @retval		#CONTEXT_HISTORY_ERROR_OUT_OF_MEMORY		Out of memory
 *
 * @pre context_history_get_list()
 * @post context_history_record_destroy()
 */
int context_history_list_get_current(context_history_list_h list, context_history_record_h* record) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Moves a history data list to the first position.
 * @since_tizen 4.0
 *
 * @param[in]	list    The context history list handle
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 *
 * @pre context_history_get_list()
 * @see context_history_list_get_count()
 * @see context_history_list_move_next()
 */
int context_history_list_move_first(context_history_list_h list) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Moves a history data list to the next position.
 * @since_tizen 4.0
 *
 * @param[in]	list    The context history list handle
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 * @retval		#CONTEXT_HISTORY_ERROR_NO_DATA				Requested data does not exist
 *
 * @pre context_history_get_list()
 * @see context_history_list_get_count()
 * @see context_history_list_move_first()
 */
int context_history_list_move_next(context_history_list_h list) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Destroys a history list handle and release all its resources.
 * @since_tizen 4.0
 *
 * @param[in]	list    The context history data handle
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 *
 * @pre context_history_get_list()
 */
int context_history_list_destroy(context_history_list_h list) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Gets an integer value from a record.
 * @details		See the programming guide to find available attribute keys@n
 *				for each context data #context_history_data_e.
 * @since_tizen 4.0
 *
 * @param[in]	record		The record handle
 * @param[in]	key			The key of attribute to get
 * @param[out]	value		The result value
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 *
 * @pre		context_history_list_get_current()
 * @see		context_history_record_get_double()
 * @see		context_history_record_get_string()
 */
int context_history_record_get_int(context_history_record_h record, const char* key, int* value) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Gets a string from a record.
 * @details		See the programming guide to find available attribute keys@n
 *				for each context data #context_history_data_e.
 * @since_tizen 4.0
 * @remarks		@c value must be released using free().
 *
 * @param[in]	record		The record handle
 * @param[in]	key			The key of attribute to get
 * @param[out]	value		The result value
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 *
 * @pre		context_history_list_get_current()
 * @see		context_history_record_get_int()
 * @see		context_history_record_get_double()
 */
int context_history_record_get_string(context_history_record_h record, const char* key, char** value) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Gets a double-precision value from a record.
 * @details		See the programming guide to find available attribute keys@n
 *				for each context data #context_history_data_e.
 * @since_tizen 4.0
 *
 * @param[in]	record		The record handle
 * @param[in]	key			The key of the attribute to get
 * @param[out]	value		The result value
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 *
 * @pre		context_history_list_get_current()
 * @see		context_history_record_get_int()
 * @see		context_history_record_get_string()
 */
int context_history_record_get_double(context_history_record_h record, const char* key, double* value) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief		Destroys a record handle and releases all its resources.
 * @since_tizen 4.0
 *
 * @param[in]	record		The record handle
 *
 * @return		@c 0 on success, otherwise a negative error value
 * @retval		#CONTEXT_HISTORY_ERROR_NONE					Successful
 * @retval		#CONTEXT_HISTORY_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval		#CONTEXT_HISTORY_ERROR_NOT_SUPPORTED		Not supported
 *
 * @pre		context_history_list_get_current()
 */
int context_history_record_destroy(context_history_record_h record) TIZEN_DEPRECATED_API;

#ifdef __cplusplus
}
#endif /* __cplusplus */

/**
* @}
*/

#endif /* __TIZEN_CONTEXT_HISTORY_H__ */
