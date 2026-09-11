/*
 * Smart Traffic Control (STC)
 *
 * Copyright (c) 2017 Samsung Electronics Co., Ltd. All rights reserved.
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
 *
 */

#ifndef __TIZEN_STC_H__
#define __TIZEN_STC_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file stc.h
 */

/*****************************************************************************
 *  Standard headers
 *****************************************************************************/
#include <tizen.h>
#include <time.h>
#include <inttypes.h>

/*****************************************************************************
 *  Macros and Typedefs
 *****************************************************************************/

/**
 * @addtogroup CAPI_NETWORK_STC_MODULE
 * @{
 */

/**
 * @brief The smart traffic control handle.
 * @since_tizen 4.0
 */
typedef void *stc_h;

/**
 * @}
 */

/**
 * @addtogroup CAPI_NETWORK_STC_STATISTICS_MODULE
 * @{
 */

/**
 * @brief The statistics rule handle.
 * @since_tizen 4.0
 */
typedef void *stc_stats_rule_h;

/**
 * @brief The handle of statistical information about network traffic.
 * @since_tizen 4.0
 */
typedef void *stc_stats_info_h;

/**
 * @brief The handle of all statistical information about network traffic.
 * @since_tizen 5.5
 */
typedef void *stc_all_stats_info_h;

/**
 * @}
 */

/*****************************************************************************
 *  Enumeration
 *****************************************************************************/

/**
 * @addtogroup CAPI_NETWORK_STC_MODULE
 * @{
 */

/**
 * @brief Enumeration for Smart Traffic Control (STC) error type.
 * @since_tizen 4.0
 */
typedef enum {
	STC_ERROR_NONE = TIZEN_ERROR_NONE,                           /**< Successful */
	STC_ERROR_NOT_PERMITTED = TIZEN_ERROR_NOT_PERMITTED,         /**< Operation not permitted(1) */
	STC_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,         /**< Out of memory(12) */
	STC_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied(13) */
	STC_ERROR_RESOURCE_BUSY = TIZEN_ERROR_RESOURCE_BUSY,         /**< Device or resource busy(16) */
	STC_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION, /**< Invalid operation(38) */
	STC_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid function parameter(22) */
	STC_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,         /**< Not supported */

	STC_ERROR_OPERATION_FAILED = TIZEN_ERROR_STC|0x01,           /**< Operation failed */
	STC_ERROR_NOT_INITIALIZED = TIZEN_ERROR_STC|0x02,            /**< Not initialized */
	STC_ERROR_ALREADY_INITIALIZED = TIZEN_ERROR_STC|0x03,        /**< Already initialized */
	STC_ERROR_IN_PROGRESS = TIZEN_ERROR_STC|0x04,                /**< In progress */
} stc_error_e;


/**
 * @}
 */

/**
 * @addtogroup CAPI_NETWORK_STC_MANAGER_MODULE
 * @{
 */

/**
 * @brief Enumeration for time period.
 * @since_tizen 4.0
 */
typedef enum {
	STC_TIME_PERIOD_UNKNOWN = 0,          /**< Unknown */
	STC_TIME_PERIOD_HOUR = 3600,          /**< Hour */
	STC_TIME_PERIOD_DAY = 86400,          /**< Day */
	STC_TIME_PERIOD_WEEK = 604800,        /**< Week */
	STC_TIME_PERIOD_MONTH = 2419200,      /**< Month */
} stc_time_period_e;

/**
 * @brief Enumeration for network interface type.
 * @since_tizen 4.0
 */
typedef enum {
	STC_IFACE_UNKNOWN,      /**< Unknown interface */
	STC_IFACE_DATACALL,     /**< Mobile data */
	STC_IFACE_WIFI,         /**< Wifi data */
	STC_IFACE_WIRED,        /**< Wired interface */
	STC_IFACE_BLUETOOTH,    /**< Bluetooth interface */
	STC_IFACE_ALL,          /**< Enumerate all network interface types */
} stc_iface_type_e;

/**
 * @brief Enumeration for network roaming type.
 * @since_tizen 4.0
 */
typedef enum {
	STC_ROAMING_UNKNOWN,     /**< Roaming unknown */
	STC_ROAMING_ENABLED,     /**< In roaming */
	STC_ROAMING_DISABLED,	 /**< Not in roaming */
} stc_roaming_type_e;

/**
 * @brief Enumeration for hardware network protocol type.
 * @since_tizen 4.0
 */
typedef enum {
	STC_PROTOCOL_UNKNOWN,               /**< Network unknown */
	STC_PROTOCOL_DATACALL_NOSVC,        /**< Network no service */
	STC_PROTOCOL_DATACALL_EMERGENCY,    /**< Network emergency */
	STC_PROTOCOL_DATACALL_SEARCH,       /**< Network search 1900 */
	STC_PROTOCOL_DATACALL_2G,           /**< Network 2G */
	STC_PROTOCOL_DATACALL_2_5G,         /**< Network 2.5G */
	STC_PROTOCOL_DATACALL_2_5G_EDGE,    /**< Network EDGE */
	STC_PROTOCOL_DATACALL_3G,           /**< Network UMTS */
	STC_PROTOCOL_DATACALL_HSDPA,        /**< Network HSDPA */
	STC_PROTOCOL_DATACALL_LTE,          /**< Network LTE */
} stc_protocol_type_e;

/**
 * @brief Enumeration for monitored process state.
 * @since_tizen 4.0
 */
typedef enum {
	STC_PROCESS_STATE_UNKNOWN,         /**< Unknown state */
	STC_PROCESS_STATE_FOREGROUND = 1 << 1,      /**< Foreground state */
	STC_PROCESS_STATE_BACKGROUND = 1 << 2,      /**< Background state */
} stc_process_state_e;

/**
 * @brief Enumeration for network connection period type.
 * @details Last received/sent mean counting data from the first connection of each interface.
 * @since_tizen 4.0
 */
typedef enum {
	STC_CONN_PERIOD_UNKNOWN,                /**< Unknown period */
	STC_CONN_PERIOD_LAST_RECEIVED_DATA,     /**< Last received data */
	STC_CONN_PERIOD_LAST_SENT_DATA,         /**< Last sent data */
	STC_CONN_PERIOD_TOTAL_RECEIVED_DATA,    /**< Total received data */
	STC_CONN_PERIOD_TOTAL_SENT_DATA,        /**< Total sent data */
} stc_conn_period_e;

/**
 * @brief Enumeration for return type of the callback.
 * @since_tizen 4.0
 */
typedef enum {
	STC_CALLBACK_CANCEL,        /**< Cancel */
	STC_CALLBACK_CONTINUE,      /**< Continue */
} stc_callback_ret_e;

/**
 * @}
 */

/*****************************************************************************
 *  Callback functions
 *****************************************************************************/

/**
 * @addtogroup CAPI_NETWORK_STC_MANAGER_MODULE
 * @{
 */

/**
 * @brief Called whenever there is available statistical information.
 * @details One of the following errors is delivered as a result.
 *   #STC_ERROR_NONE                Successful \n
 *   #STC_ERROR_INVALID_OPERATION   No reply \n
 *   #STC_ERROR_OPERATION_FAILED    Operation failed \n
 *   #STC_ERROR_PERMISSION_DENIED   Access or permission denied
 * @since_tizen 4.0
 *
 * @param[in] result        The result of statistics information getting
 * @param[in] info          The statistics information handle
 * @param[in] user_data     The user data passed from the statistics provider function
 * @return  #STC_CALLBACK_CONTINUE to continue with the next iteration of the loop,
 * otherwise #STC_CALLBACK_CANCEL to break out of the loop
 *
 * @pre stc_foreach_stats() will invoke this callback.
 * @see stc_stats_info_get_app_id()
 * @see stc_stats_info_get_iface_name()
 * @see stc_stats_info_get_time_interval()
 * @see stc_stats_info_get_iface_type()
 * @see stc_stats_info_get_counter()
 * @see stc_stats_info_get_roaming_type()
 * @see stc_stats_info_get_protocol_type()
 * @see stc_stats_info_get_process_state()
 */
typedef stc_callback_ret_e (*stc_stats_info_cb)(
		stc_error_e result, stc_stats_info_h info, void *user_data);

/**
 * @brief Called when getting all statistical information is finished.
 * @details One of the following errors is delivered as a result.
 *   #STC_ERROR_NONE                Successful \n
 *   #STC_ERROR_INVALID_OPERATION   No reply \n
 *   #STC_ERROR_OPERATION_FAILED    Operation failed \n
 *   #STC_ERROR_PERMISSION_DENIED   Access or permission denied
 * @since_tizen 5.5
 * @remarks @a info will be released by the platform after the callback exits.
 *
 * @param[in] result        The result of statistics gathering completion
 * @param[in] info          The handle of all statistical information
 * @param[in] user_data     The user data passed from the statistics provider function
 *
 * @pre stc_get_all_stats() will invoke this callback.
 * @see stc_get_all_stats()
 */
typedef void (*stc_get_all_stats_finished_cb)(stc_error_e result,
		stc_all_stats_info_h info, void *user_data);

/**
 * @}
 */


/*****************************************************************************
 *  Core API functions
 *****************************************************************************/


/**
 * @addtogroup CAPI_NETWORK_STC_MODULE
 * @{
 */

/**
 * @deprecated Deprecated since 10.0.
 * @brief Initializes STC (Smart Traffic Control).
 * @details This function initializes the Smart Traffic
 *   Control (STC) handle and prepares it for use. It is mandatory
 *   to call this function before using any other STC functions.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a stc using stc_deinitialize().
 *
 * @param[out] stc The STC handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED       Permission denied
 *
 * @see stc_h
 * @see stc_deinitialize()
 */
int stc_initialize(stc_h *stc) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Deinitializes STC.
 * @details This function releases the resources associated
 *   with the given STC handle. It should be called after all STC
 *   operations are completed to clean up the system resources.
 * @since_tizen 4.0
 *
 * @param[in] stc The STC handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_h
 * @see stc_initialize()
 */
int stc_deinitialize(stc_h stc) TIZEN_DEPRECATED_API;


/**
 * @}
 */

/**
 * @addtogroup CAPI_NETWORK_STC_MANAGER_MODULE
 * @{
 */

/**
 * @deprecated Deprecated since 5.5. Use stc_get_all_stats() and stc_foreach_all_stats() instead.
 * @brief Gets the statistics information an application matched rule asynchronously.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] stc           The STC handle
 * @param[in] rule          The statistics rule handle
 * @param[in] info_cb       The callback is called for each application
 * that used network in between timestamps specified
 * @param[in] user_data     The user data passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED       Permission denied
 *
 * @see stc_h
 * @see stc_stats_rule_h
 * @see stc_initialize()
 * @see stc_stats_rule_create()
 * @see stc_stats_rule_destroy()
 * @see stc_stats_rule_set_app_id()
 * @see stc_stats_rule_set_time_interval()
 * @see stc_stats_rule_set_iface_type()
 * @see stc_stats_rule_set_time_period()
 * @see stc_stats_rule_get_app_id()
 * @see stc_stats_rule_get_time_interval()
 * @see stc_stats_rule_get_iface_type()
 * @see stc_stats_rule_get_time_period()
 * @see stc_stats_info_cb()
 */
int stc_get_stats(stc_h stc, stc_stats_rule_h rule,
		  stc_stats_info_cb info_cb, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 5.5. Use stc_get_all_stats() and stc_foreach_all_stats() instead.
 * @brief Gets the statistics information of each application asynchronously.
 * @details The callback is called for each application that used network
 * in between timestamps specified.
 * If interface name is not specified, each application will only appear
 * once with the total traffic used over all interfaces.
 *
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] stc           The STC handle
 * @param[in] rule          The statistics rule handle
 * @param[in] info_cb The callback is called for each application
 * that used network in between timestamps specified
 * @param[in] user_data     The user data passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED       Permission denied
 *
 * @see stc_h
 * @see stc_stats_rule_h
 * @see stc_initialize()
 * @see stc_stats_rule_create()
 * @see stc_stats_rule_destroy()
 * @see stc_stats_rule_set_app_id()
 * @see stc_stats_rule_set_time_interval()
 * @see stc_stats_rule_set_iface_type()
 * @see stc_stats_rule_set_time_period()
 * @see stc_stats_rule_get_app_id()
 * @see stc_stats_rule_get_time_interval()
 * @see stc_stats_rule_get_iface_type()
 * @see stc_stats_rule_get_time_period()
 * @see stc_stats_info_cb()
 */
int stc_foreach_stats(stc_h stc, stc_stats_rule_h rule,
		stc_stats_info_cb info_cb, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Starts to get statistical information based on the rules asynchronously.
 * @details This function initiates the process of gathering
 *   statistical information based on the specified rules. It runs
 *   in an asynchronous manner, meaning that the function returns
 *   immediately and the results are provided later via the
 *   callback function.
 *
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] stc           The STC handle
 * @param[in] rule          The statistics rule handle
 * @param[in] finished_cb   The callback is called when information gathering is completed
 * @param[in] user_data     The user data passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED       Permission denied
 *
 * @post This function invokes stc_get_all_stats_finished_cb().
 * @see stc_h
 * @see stc_stats_rule_h
 * @see stc_initialize()
 * @see stc_stats_rule_create()
 * @see stc_stats_rule_destroy()
 * @see stc_stats_rule_set_app_id()
 * @see stc_stats_rule_set_time_interval()
 * @see stc_stats_rule_set_iface_type()
 * @see stc_stats_rule_set_time_period()
 * @see stc_stats_rule_get_app_id()
 * @see stc_stats_rule_get_time_interval()
 * @see stc_stats_rule_get_iface_type()
 * @see stc_stats_rule_get_time_period()
 * @see stc_get_all_stats_finished_cb()
 */
int stc_get_all_stats(stc_h stc, stc_stats_rule_h rule,
		stc_get_all_stats_finished_cb finished_cb, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets all statistical information.
 * @details This function retrieves all statistical
 *    information stored in the given handle and iterates over each
 *    entry, invoking the provided callback function for each item.
 *    The callback function can then process the individual
 *    statistics as needed.
 *
 * @since_tizen 5.5
 *
 * @param[in] info          All statistics information handle
 * @param[in] info_cb       The callback is called for each application
 * @param[in] user_data     The user data passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @pre stc_get_all_stats() needs to be called and data needs to be received
 *      in stc_get_stats_finished_cb() before calling this function.
 * @post This function invokes stc_stats_info_cb().
 * @see stc_get_all_stats()
 * @see stc_stats_info_cb()
 */
int stc_foreach_all_stats(stc_all_stats_info_h info,
		stc_stats_info_cb info_cb, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 5.5. Use stc_get_all_stats() and stc_foreach_all_stats() instead.
 * @brief Gets the total statistics information by interface type asynchronously.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] stc           The STC handle
 * @param[in] rule          The statistics rule handle
 * @param[in] info_cb       The callback is called for each application
 * that used network in between timestamps specified
 * @param[in] user_data     The user data passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED       Permission denied
 *
 * @see stc_h
 * @see stc_stats_rule_h
 * @see stc_initialize()
 * @see stc_stats_rule_create()
 * @see stc_stats_rule_destroy()
 * @see stc_stats_rule_set_app_id()
 * @see stc_stats_rule_set_time_interval()
 * @see stc_stats_rule_set_iface_type()
 * @see stc_stats_rule_set_time_period()
 * @see stc_stats_rule_get_app_id()
 * @see stc_stats_rule_get_time_interval()
 * @see stc_stats_rule_get_iface_type()
 * @see stc_stats_rule_get_time_period()
 * @see stc_stats_info_cb()
 */
int stc_get_total_stats(stc_h stc, stc_stats_rule_h rule,
		stc_stats_info_cb info_cb, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @}
 */

/**
 * @addtogroup CAPI_NETWORK_STC_STATISTICS_MODULE
 * @{
 */


/**
 * @deprecated Deprecated since 10.0.
 * @brief Creates the statistics rule handle.
 * @details This function creates a new statistics rule handle
 *    which can be used to specify criteria for gathering
 *    statistical information. The handle is initially empty and
 *    must be configured with the desired rules before being used
 *    with other STC functions.
 * @since_tizen 4.0
 * @remarks You must release @a rule using stc_stats_rule_destroy().
 *
 * @param[in] stc           The STC handle
 * @param[out] rule         The statistics rule handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_h
 * @see stc_stats_rule_h
 * @see stc_initialize()
 * @see stc_stats_rule_destroy()
 */
int stc_stats_rule_create(stc_h stc, stc_stats_rule_h *rule) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Destroys the statistics rule handle.
 * @details This function releases the resources associated
 *    with the given statistics rule handle. It should be called
 *    after the handle is no longer needed to clean up the system
 *    resources.
 * @since_tizen 4.0
 *
 * @param[in] rule          The statistics rule handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_rule_h
 * @see stc_stats_rule_create()
 */
int stc_stats_rule_destroy(stc_stats_rule_h rule) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Sets the application ID for statistics rule.
 * @details This function sets the application ID for the
 *    given statistics rule handle. The application ID specifies
 *    which application's traffic statistics should be gathered
 *    according to the rule.
 * @since_tizen 4.0
 *
 * @param[in] rule          The statistics rule handle
 * @param[in] app_id        The application ID
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_rule_h
 * @see stc_stats_rule_create()
 * @see stc_stats_rule_destroy()
 * @see stc_stats_rule_get_app_id()
 */
int stc_stats_rule_set_app_id(stc_stats_rule_h rule,
		const char *app_id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Sets the time interval for statistics rule.
 * @details This function sets the time interval for the given
 *    statistics rule handle. The time interval specifies the range
 *    of time during which the traffic statistics should be gathered
 *    according to the rule.
 * @since_tizen 4.0
 *
 * @param[in] rule          The statistics rule handle
 * @param[in] from          The beginning of the time interval
 * @param[in] to            The end of the time interval
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_rule_h
 * @see stc_stats_rule_create()
 * @see stc_stats_rule_destroy()
 * @see stc_stats_rule_get_time_interval()
 */
int stc_stats_rule_set_time_interval(stc_stats_rule_h rule,
		time_t from, time_t to) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Sets the interface type for statistics rule.
 * @details This function sets the interface type for the
 *    given statistics rule handle. The interface type specifies
 *    which network interfaces' traffic statistics should be
 *    gathered according to the rule.
 * @since_tizen 4.0
 *
 * @param[in] rule          The stats rule handle
 * @param[in] iface_type    The interface type
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_rule_h
 * @see stc_iface_type_e
 * @see stc_stats_rule_create()
 * @see stc_stats_rule_destroy()
 * @see stc_stats_rule_get_iface_type()
 */
int stc_stats_rule_set_iface_type(stc_stats_rule_h rule,
		stc_iface_type_e iface_type) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Sets the time period for statistics rule.
 * @details This function sets the time period for the given
 *    statistics rule handle. The time period specifies the
 *    frequency at which the traffic statistics should be gathered
 *    according to the rule.
 * @since_tizen 4.0
 *
 * @param[in] rule          The statistics rule handle
 * @param[in] time_period   The time period
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_rule_h
 * @see stc_time_period_e
 * @see stc_stats_rule_create()
 * @see stc_stats_rule_destroy()
 * @see stc_stats_rule_get_time_period()
 */
int stc_stats_rule_set_time_period(stc_stats_rule_h rule,
		stc_time_period_e time_period) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the application ID for statistics rule.
 * @details This function retrieves the application ID that
 *    has been set for the given statistics rule handle. The
 *    application ID specifies which application's traffic
 *    statistics should be gathered according to the rule.
 * @since_tizen 4.0
 * @remarks You must release @a app_id using free().
 *
 * @param[in] rule          The statistics rule handle
 * @param[out] app_id       The application ID
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_rule_h
 * @see stc_stats_rule_create()
 * @see stc_stats_rule_destroy()
 * @see stc_stats_rule_set_app_id()
 */
int stc_stats_rule_get_app_id(stc_stats_rule_h rule, char **app_id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the time interval for statistics rule.
 * @since_tizen 4.0
 *
 * @param[in] rule          The statistics rule handle
 * @param[out] from         The beginning of the time interval
 * @param[out] to           The end of the time interval
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_rule_h
 * @see stc_stats_rule_create()
 * @see stc_stats_rule_destroy()
 * @see stc_stats_rule_set_time_interval()
 */
int stc_stats_rule_get_time_interval(stc_stats_rule_h rule,
		time_t *from, time_t *to) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the interface type for statistics rule.
 * @details This function retrieves the interface type that
 *    has been set for the given statistics rule handle. The
 *    interface type specifies which network interfaces' traffic
 *    statistics should be gathered according to the rule.
 * @since_tizen 4.0
 *
 * @param[in] rule          The statistics rule handle
 * @param[out] iface_type   The interface type
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_rule_h
 * @see stc_iface_type_e
 * @see stc_stats_rule_create()
 * @see stc_stats_rule_destroy()
 * @see stc_stats_rule_set_iface_type()
 */
int stc_stats_rule_get_iface_type(stc_stats_rule_h rule,
		stc_iface_type_e *iface_type) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the time period for statistics rule.
 * @details This function retrieves the time period that has been
 *    set for the given statistics rule handle. The time period
 *    specifies the frequency at which the traffic statistics should
 *    be gathered according to the rule.
 * @since_tizen 4.0
 *
 * @param[in] rule          The statistics rule handle
 * @param[out] time_period  The time period
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_rule_h
 * @see stc_time_period_e
 * @see stc_stats_rule_create()
 * @see stc_stats_rule_destroy()
 * @see stc_stats_rule_set_time_period()
 */
int stc_stats_rule_get_time_period(stc_stats_rule_h rule,
		stc_time_period_e *time_period) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Clones the statistics info handle.
 * @details This function creates a new statistics info handle
 *    that is a copy of the original statistics info handle. The
 *    cloned handle can be used independently of the original handle.
 * @since_tizen 5.5
 * @remarks You must release @a cloned using stc_stats_info_destroy().
 *
 * @param[in] info            The origin statistics info handle
 * @param[out] cloned         The cloned statistics info handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_h
 * @see stc_stats_info_h
 * @see stc_initialize()
 * @see stc_stats_info_destroy()
 */
int stc_stats_info_clone(stc_stats_info_h info, stc_stats_info_h *cloned) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Destroys the statistics info handle.
 * @details This function releases all resources allocated for
 *    the given statistics info handle. After calling this function,
 *    the handle becomes invalid and cannot be used further.
 * @since_tizen 5.5
 *
 * @param[in] info          The statistics info handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_info_h
 * @see stc_stats_info_clone()
 */
int stc_stats_info_destroy(stc_stats_info_h info) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the application ID from statistics information.
 * @details This function retrieves the application ID that
 *    corresponds to the given statistics information handle. The
 *    application ID indicates which application's traffic
 *    statistics are contained within the handle.
 * @since_tizen 4.0
 * @remarks You must release @a app_id using free().
 *
 * @param[in] info          The statistics information handle
 * @param[out] app_id       The application ID
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_info_h
 * @see stc_stats_info_cb()
 */
int stc_stats_info_get_app_id(stc_stats_info_h info, char **app_id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the interface name from statistics information.
 * @details This function retrieves the interface name that
 *    corresponds to the given statistics information handle. The
 *    interface name indicates which network interface's traffic
 *    statistics are contained within the handle.
 * @since_tizen 4.0
 * @remarks You must release @a iface_name using free().
 *
 * @param[in] info          The statistics information handle
 * @param[out] iface_name   The interface name
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_info_h
 * @see stc_stats_info_cb()
 */
int stc_stats_info_get_iface_name(stc_stats_info_h info, char **iface_name) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the time interval from statistics information.
 * @details This function retrieves the time interval that
 *    corresponds to the given statistics information handle. The
 *    time interval indicates the range of time during which the
 *    traffic statistics were gathered.
 * @since_tizen 4.0
 *
 * @param[in] info          The statistics information handle
 * @param[out] from         The beginning of the time interval
 * @param[out] to           The end of the time interval
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_info_h
 * @see stc_stats_info_cb()
 */
int stc_stats_info_get_time_interval(stc_stats_info_h info,
		time_t *from, time_t *to) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the interface type from statistics information.
 * @details This function retrieves the interface type that
 *    corresponds to the given statistics information handle. The
 *    interface type indicates the type of network interface whose
 *    traffic statistics are contained within the handle.
 * @since_tizen 4.0
 *
 * @param[in] info          The statistics information handle
 * @param[out] iface_type   The interface type
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_info_h
 * @see stc_iface_type_e
 * @see stc_stats_info_cb()
 */
int stc_stats_info_get_iface_type(stc_stats_info_h info,
		stc_iface_type_e *iface_type) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the counters from statistics information.
 * @details This function retrieves the traffic counters that
 *    correspond to the given statistics information handle. The
 *    traffic counters indicate the amount of data transmitted and
 *    received over the network interface during the specified time
 *    interval.
 * @since_tizen 4.0
 *
 * @param[in] info          The statistics information handle
 * @param[out] incoming     The incoming counter
 * @param[out] outgoing     The outgoing counter
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_info_h
 * @see stc_stats_info_cb()
 */
int stc_stats_info_get_counter(stc_stats_info_h info,
		int64_t *incoming, int64_t *outgoing) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the roaming type from statistics information.
 * @details This function retrieves the roaming type that
 *    corresponds to the given statistics information handle. The
 *    roaming type indicates whether the traffic statistics were
 *    gathered while the device was connected to a cellular network
 *    or not.
 * @since_tizen 4.0
 *
 * @param[in] info          The statistics information handle
 * @param[out] roaming_type The roaming type
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_info_h
 * @see stc_roaming_type_e
 * @see stc_stats_info_cb()
 */
int stc_stats_info_get_roaming_type(stc_stats_info_h info,
		stc_roaming_type_e *roaming_type) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the protocol type from statistics information.
 * @details This function retrieves the protocol type that
 *    corresponds to the given statistics information handle. The
 *    protocol type indicates the specific network protocol whose
 *    traffic statistics are contained within the handle.
 * @since_tizen 4.0
 *
 * @param[in] info          The statistics information handle
 * @param[out] protocol     The protocol type
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_info_h
 * @see stc_protocol_type_e
 * @see stc_stats_info_cb()
 */
int stc_stats_info_get_protocol_type(stc_stats_info_h info,
		stc_protocol_type_e *protocol) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the process state from statistics information.
 * @details This function retrieves the process state that
 *    corresponds to the given statistics information handle. The
 *    process state indicates the current status of the application
 *    whose traffic statistics are contained within the handle.
 * @since_tizen 4.0
 *
 * @param[in] info          The statistics information handle
 * @param[out] state        The process state
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED           Not supported
 *
 * @see stc_stats_info_h
 * @see stc_process_state_e
 * @see stc_stats_info_cb()
 */
int stc_stats_info_get_process_state(stc_stats_info_h info,
		stc_process_state_e *state) TIZEN_DEPRECATED_API;


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_STC_H__ */
