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

#ifndef __TIZEN_STC_MGR_INTERNAL_H__
#define __TIZEN_STC_MGR_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file stc_internal.h
 */

/*****************************************************************************
 *  Standard headers
 *****************************************************************************/
#include <tizen.h>
#include <time.h>
#include <inttypes.h>

/*****************************************************************************
 *  Smart Traffic Control (STC) library headers
 *****************************************************************************/
#include "stc_stat_internal.h"
#include "stc_rstn_internal.h"

/*****************************************************************************
 *  Macros and Typedefs
 *****************************************************************************/

/*****************************************************************************
 *  Enumeration
 *****************************************************************************/

/*****************************************************************************
 *  Callback functions
 *****************************************************************************/

/**
 * @addtogroup CAPI_NETWORK_STC_MANAGER_MODULE
 * @{
 */

/**
 * @brief Called for enumerate restriction rule.
 * @since_tizen 5.0
 *
 * @param[in] result		The result
 * @param[in] rule			The restriction rule handle
 * @param[in] user_data		The user data passed from the callback registration function
 * @return	@c STC_CALLBACK_CONTINUE to continue with the next iteration of the loop,
 *			otherwise @c STC_CALLBACK_CANCEL to break out of the loop
 *
 * @pre	stc_get_restriction() will invoke this callback.
 * @pre	stc_foreach_restriction() will invoke this callback.
 * @see stc_restriction_rule_get_app_id()
 * @see stc_restriction_rule_get_iface_name()
 * @see stc_restriction_rule_get_subscriber_id()
 * @see stc_restriction_rule_get_type()
 * @see stc_restriction_rule_get_iface_type()
 * @see stc_restriction_rule_get_limit()
 * @see stc_restriction_rule_get_warning_limit()
 * @see stc_restriction_rule_get_monthly_limit()
 * @see stc_restriction_rule_get_weekly_limit()
 * @see stc_restriction_rule_get_daily_limit()
 * @see stc_restriction_rule_get_month_start_date()
 * @see stc_restriction_rule_get_roaming_type()
 * @see	stc_get_restriction()
 * @see	stc_foreach_restriction()
 */
typedef stc_callback_ret_e (*stc_restriction_rule_cb)(stc_error_e result,
		stc_restriction_rule_h rule, void *user_data);

typedef void (*stc_threshold_crossed_cb)(stc_restriction_rule_h rule,
		void *user_data);

/**
* @}
*/

/*****************************************************************************
 *  Core API functions
 *****************************************************************************/

/**
 * @addtogroup CAPI_NETWORK_STC_MANAGER_MODULE
 * @{
 */

/**
 * @brief Resets stats information.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] stc The stc handle
 * @param[in] rule The reset rule handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE Successful
 * @retval #STC_ERROR_OPERATION_FAILED General error
 * @retval #STC_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see #stc_h
 * @see #stc_reset_rule_h
 * @see stc_initialize()
 * @see stc_reset_rule_create()
 * @see stc_reset_rule_destroy()
 * @see stc_reset_rule_set_app_id()
 * @see stc_reset_rule_set_subscriber_id()
 * @see stc_reset_rule_set_iface_type()
 * @see stc_reset_rule_set_time_interval()
 * @see stc_reset_rule_get_app_id()
 * @see stc_reset_rule_get_subscriber_id()
 * @see stc_reset_rule_get_iface_type()
 * @see stc_reset_rule_get_time_interval()
 */
int stc_reset_stats(stc_h stc, stc_reset_rule_h rule);


/**
 * @brief Sets the restriction for application.
 * @details Sets and applies restriction for application.
 *			It will creates new restriction or modify existing.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] stc The stc handle
 * @param[in] rule The restriction rule handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE Successful
 * @retval #STC_ERROR_OPERATION_FAILED General error
 * @retval #STC_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see #stc_h
 * @see #stc_restriction_rule_h
 * @see stc_initialize()
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_app_id()
 * @see stc_restriction_rule_set_iface_name()
 * @see stc_restriction_rule_set_iface_type()
 * @see stc_restriction_rule_set_limit()
 * @see stc_restriction_rule_set_warning_limit()
 * @see stc_restriction_rule_set_roaming_type()
 * @see stc_restriction_rule_set_subscriber_id()
 * @see stc_restriction_rule_get_app_id()
 * @see stc_restriction_rule_get_iface_type()
 * @see stc_restriction_rule_get_limit()
 * @see stc_restriction_rule_get_warning_limit()
 * @see stc_restriction_rule_get_roaming_type()
 * @see stc_restriction_rule_get_subscriber_id()
 * @see stc_restriction_rule_get_process_state()
 * @see stc_get_restriction()
 * @see stc_foreach_restriction()
 * @see stc_get_restriction_type()
 * @see stc_unset_restriction()
 */
int stc_set_restriction(stc_h stc, stc_restriction_rule_h rule_h);

/**
 * @brief Unsets the existing restriction for application.
 * @details Unsets the existing restriction for application.
 *			It will delete restriction rule in kernel.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] stc The stc handle
 * @param[in] rule The restriction rule handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE Successful
 * @retval #STC_ERROR_OPERATION_FAILED General error
 * @retval #STC_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see #stc_h
 * @see #stc_restriction_rule_h
 * @see stc_initialize()
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_app_id()
 * @see stc_restriction_rule_set_iface_name()
 * @see stc_restriction_rule_set_iface_type()
 * @see stc_restriction_rule_set_limit()
 * @see stc_restriction_rule_set_warning_limit()
 * @see stc_restriction_rule_set_roaming_type()
 * @see stc_restriction_rule_set_subscriber_id()
 * @see stc_restriction_rule_get_app_id()
 * @see stc_restriction_rule_get_iface_type()
 * @see stc_restriction_rule_get_limit()
 * @see stc_restriction_rule_get_warning_limit()
 * @see stc_restriction_rule_get_roaming_type()
 * @see stc_restriction_rule_get_subscriber_id()
 * @see stc_restriction_rule_get_process_state()
 * @see stc_set_restriction()
 * @see stc_get_restriction()
 * @see stc_foreach_restriction()
 * @see stc_get_restriction_type()
 */
int stc_unset_restriction(stc_h stc, stc_restriction_rule_h rule_h);

/**
 * @brief Gets the restriction rule per application.
 * @details The callback is called for each application that applied restriction.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] stc The stc handle
 * @param[in] rule The restriction rule handle
 * @param[in] rule_cb The callback is called for each application
 *			that applied restriction in interface specified
 * @param[in] user_data User data will be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE Successful
 * @retval #STC_ERROR_OPERATION_FAILED General error
 * @retval #STC_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see #stc_h
 * @see #stc_restriction_rule_h
 * @see stc_initialize()
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_app_id()
 * @see stc_restriction_rule_set_iface_name()
 * @see stc_restriction_rule_set_iface_type()
 * @see stc_restriction_rule_set_limit()
 * @see stc_restriction_rule_set_warning_limit()
 * @see stc_restriction_rule_set_roaming_type()
 * @see stc_restriction_rule_set_subscriber_id()
 * @see stc_restriction_rule_get_app_id()
 * @see stc_restriction_rule_get_iface_type()
 * @see stc_restriction_rule_get_limit()
 * @see stc_restriction_rule_get_warning_limit()
 * @see stc_restriction_rule_get_roaming_type()
 * @see stc_restriction_rule_get_subscriber_id()
 * @see stc_restriction_rule_get_process_state()
 * @see stc_restriction_rule_cb()
 * @see stc_set_restriction()
 * @see stc_foreach_restriction()
 * @see stc_get_restriction_type()
 * @see stc_unset_restriction()
 */
int stc_get_restriction(stc_h stc, stc_restriction_rule_h rule,
		stc_restriction_rule_cb rule_cb, void *user_data);

int stc_set_restriction_threshold_crossed_cb(stc_h stc,
		stc_threshold_crossed_cb crossed_cb, void *user_data);

int stc_unset_restriction_threshold_crossed_cb(stc_h stc);

int stc_set_warn_threshold_crossed_cb(stc_h stc,
		stc_threshold_crossed_cb crossed_cb, void *user_data);

int stc_unset_warn_threshold_crossed_cb(stc_h stc);

/**
 * @brief Gets the restriction rule of all application.
 * @details The callback is called for each application that applied restriction.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] stc The stc handle
 * @param[in] rule The restriction rule handle
 * @param[in] rule_cb The callback is called for each application
 *			that applied restriction in interface specified
 * @param[in] user_data User data will be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE Successful
 * @retval #STC_ERROR_OPERATION_FAILED General error
 * @retval #STC_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see #stc_h
 * @see #stc_restriction_rule_h
 * @see stc_initialize()
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_app_id()
 * @see stc_restriction_rule_set_iface_name()
 * @see stc_restriction_rule_set_iface_type()
 * @see stc_restriction_rule_set_limit()
 * @see stc_restriction_rule_set_warning_limit()
 * @see stc_restriction_rule_set_roaming_type()
 * @see stc_restriction_rule_set_subscriber_id()
 * @see stc_restriction_rule_get_app_id()
 * @see stc_restriction_rule_get_iface_type()
 * @see stc_restriction_rule_get_limit()
 * @see stc_restriction_rule_get_warning_limit()
 * @see stc_restriction_rule_get_roaming_type()
 * @see stc_restriction_rule_get_subscriber_id()
 * @see stc_restriction_rule_get_process_state()
 * @see stc_restriction_rule_cb()
 * @see stc_set_restriction()
 * @see stc_get_restriction()
 * @see stc_get_restriction_type()
 * @see stc_unset_restriction()
 */
int stc_foreach_restriction(stc_h stc, stc_restriction_rule_h rule,
		stc_restriction_rule_cb rule_cb, void *user_data);

/**
 * @brief Gets the restriction type.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] stc The stc handle
 * @param[in] rule The restriction rule handle
 * @param[out] type The restriction type
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE Successful
 * @retval #STC_ERROR_OPERATION_FAILED General error
 * @retval #STC_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see #stc_h
 * @see #stc_restriction_rule_h
 * @see #stc_restriction_type_e
 * @see stc_initialize()
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_app_id()
 * @see stc_restriction_rule_set_iface_name()
 * @see stc_restriction_rule_set_iface_type()
 * @see stc_restriction_rule_set_limit()
 * @see stc_restriction_rule_set_warning_limit()
 * @see stc_restriction_rule_set_roaming_type()
 * @see stc_restriction_rule_set_subscriber_id()
 * @see stc_restriction_rule_get_app_id()
 * @see stc_restriction_rule_get_iface_type()
 * @see stc_restriction_rule_get_limit()
 * @see stc_restriction_rule_get_warning_limit()
 * @see stc_restriction_rule_get_roaming_type()
 * @see stc_restriction_rule_get_subscriber_id()
 * @see stc_restriction_rule_get_process_state()
 * @see stc_set_restriction()
 * @see stc_get_restriction()
 * @see stc_foreach_restriction()
 * @see stc_unset_restriction()
 */
int stc_get_restriction_type(stc_h stc, stc_restriction_rule_h rule,
		stc_restriction_type_e *type);

/**
 * @brief Adds the restriction to list.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] list_h The restriction list handle
 * @param[in] rule_h The restriction rule handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE Successful
 * @retval #STC_ERROR_OPERATION_FAILED General error
 * @retval #STC_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see #stc_restriction_list_h
 * @see #stc_restriction_rule_h
 * @see stc_initialize()
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_app_id()
 * @see stc_restriction_rule_set_iface_name()
 * @see stc_restriction_rule_set_iface_type()
 * @see stc_restriction_rule_set_limit()
 * @see stc_restriction_rule_set_warning_limit()
 * @see stc_restriction_rule_set_roaming_type()
 * @see stc_restriction_rule_set_subscriber_id()
 * @see stc_restriction_rule_get_app_id()
 * @see stc_restriction_rule_get_iface_type()
 * @see stc_restriction_rule_get_limit()
 * @see stc_restriction_rule_get_warning_limit()
 * @see stc_restriction_rule_get_roaming_type()
 * @see stc_restriction_rule_get_subscriber_id()
 * @see stc_restriction_rule_get_process_state()
 * @see stc_restriction_list_create()
 * @see stc_remove_restriction_rule_from_list()
 * @see stc_set_restriction_list()
 * @see stc_unset_restriction_list()
 */
int stc_add_restriction_rule_to_list(stc_restriction_list_h list_h,
			stc_restriction_rule_h rule_h);

/**
 * @brief Removes the restriction from list.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] list_h The restriction list handle
 * @param[in] rule_h The restriction rule handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE Successful
 * @retval #STC_ERROR_OPERATION_FAILED General error
 * @retval #STC_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see #stc_restriction_list_h
 * @see #stc_restriction_rule_h
 * @see stc_initialize()
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_app_id()
 * @see stc_restriction_rule_set_iface_name()
 * @see stc_restriction_rule_set_iface_type()
 * @see stc_restriction_rule_set_limit()
 * @see stc_restriction_rule_set_warning_limit()
 * @see stc_restriction_rule_set_roaming_type()
 * @see stc_restriction_rule_set_subscriber_id()
 * @see stc_restriction_rule_get_app_id()
 * @see stc_restriction_rule_get_iface_type()
 * @see stc_restriction_rule_get_limit()
 * @see stc_restriction_rule_get_warning_limit()
 * @see stc_restriction_rule_get_roaming_type()
 * @see stc_restriction_rule_get_subscriber_id()
 * @see stc_restriction_rule_get_process_state()
 * @see stc_restriction_list_create()
 * @see stc_add_restriction_rule_to_list()
 * @see stc_set_restriction_list()
 * @see stc_unset_restriction_list()
 */
int stc_remove_restriction_rule_from_list(stc_restriction_list_h list_h,
			stc_restriction_rule_h rule_h);

/**
 * @brief Gets the restriction rule from list.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] list_h The restriction list handle
 * @param[in] rule_cb The callback is called for each rule from list
 * @param[in] user_data User data will be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE Successful
 * @retval #STC_ERROR_OPERATION_FAILED General error
 * @retval #STC_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see #stc_restriction_list_h
 * @see #stc_restriction_rule_h
 * @see stc_initialize()
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_app_id()
 * @see stc_restriction_rule_set_iface_name()
 * @see stc_restriction_rule_set_iface_type()
 * @see stc_restriction_rule_set_limit()
 * @see stc_restriction_rule_set_warning_limit()
 * @see stc_restriction_rule_set_roaming_type()
 * @see stc_restriction_rule_set_subscriber_id()
 * @see stc_restriction_rule_get_app_id()
 * @see stc_restriction_rule_get_iface_type()
 * @see stc_restriction_rule_get_limit()
 * @see stc_restriction_rule_get_warning_limit()
 * @see stc_restriction_rule_get_roaming_type()
 * @see stc_restriction_rule_get_subscriber_id()
 * @see stc_restriction_rule_get_process_state()
 * @see stc_restriction_list_create()
 * @see stc_remove_restriction_rule_from_list()
 * @see stc_set_restriction_list()
 * @see stc_unset_restriction_list()
 */
int stc_foreach_restriction_rule_from_list(stc_restriction_list_h list_h,
			stc_restriction_rule_cb rule_cb, void *user_data);

/**
 * @brief Sets the restriction list.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] stc The stc handle
 * @param[in] list_h The restriction list handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE Successful
 * @retval #STC_ERROR_OPERATION_FAILED General error
 * @retval #STC_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see #stc_restriction_list_h
 * @see #stc_restriction_rule_h
 * @see stc_initialize()
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_app_id()
 * @see stc_restriction_rule_set_iface_name()
 * @see stc_restriction_rule_set_iface_type()
 * @see stc_restriction_rule_set_limit()
 * @see stc_restriction_rule_set_warning_limit()
 * @see stc_restriction_rule_set_roaming_type()
 * @see stc_restriction_rule_set_subscriber_id()
 * @see stc_restriction_rule_get_app_id()
 * @see stc_restriction_rule_get_iface_type()
 * @see stc_restriction_rule_get_limit()
 * @see stc_restriction_rule_get_warning_limit()
 * @see stc_restriction_rule_get_roaming_type()
 * @see stc_restriction_rule_get_subscriber_id()
 * @see stc_restriction_rule_get_process_state()
 * @see stc_restriction_list_create()
 * @see stc_add_restriction_rule_to_list()
 * @see stc_remove_restriction_rule_from_list()
 * @see stc_unset_restriction_list()
 */
int stc_set_restriction_list(stc_h stc, stc_restriction_list_h list_h);

/**
 * @brief Unsets the restriction list.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] stc The stc handle
 * @param[in] list_h The restriction list handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE Successful
 * @retval #STC_ERROR_OPERATION_FAILED General error
 * @retval #STC_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED Not initialized
 * @retval #STC_ERROR_NOT_SUPPORTED Not supported
 * @retval #STC_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see #stc_restriction_list_h
 * @see #stc_restriction_rule_h
 * @see stc_initialize()
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_app_id()
 * @see stc_restriction_rule_set_iface_name()
 * @see stc_restriction_rule_set_iface_type()
 * @see stc_restriction_rule_set_limit()
 * @see stc_restriction_rule_set_warning_limit()
 * @see stc_restriction_rule_set_roaming_type()
 * @see stc_restriction_rule_set_subscriber_id()
 * @see stc_restriction_rule_get_app_id()
 * @see stc_restriction_rule_get_iface_type()
 * @see stc_restriction_rule_get_limit()
 * @see stc_restriction_rule_get_warning_limit()
 * @see stc_restriction_rule_get_roaming_type()
 * @see stc_restriction_rule_get_subscriber_id()
 * @see stc_restriction_rule_get_process_state()
 * @see stc_restriction_list_create()
 * @see stc_add_restriction_rule_to_list()
 * @see stc_remove_restriction_rule_from_list()
 * @see stc_set_restriction_list()
 */
int stc_unset_restriction_list(stc_h stc, stc_restriction_list_h list_h);

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_STC_MGR_INTERNAL_H__ */
