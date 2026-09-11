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

#ifndef __TIZEN_STC_RSTN_INTERNAL_H__
#define __TIZEN_STC_RSTN_INTERNAL_H__

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
 *  Macros and Typedefs
 *****************************************************************************/

/**
 * @addtogroup CAPI_NETWORK_STC_RESTRICTION_MODULE
 * @{
 */

/**
 * @brief The restriction rule handle.
 * @since_tizen 4.0
 */
typedef void *stc_restriction_rule_h;

/**
 * @brief The restriction list handle.
 * @since_tizen 4.0
 */
typedef void *stc_restriction_list_h;

/**
* @}
*/

/*****************************************************************************
 *  Enumeration
 *****************************************************************************/

/**
 * @addtogroup CAPI_NETWORK_STC_RESTRICTION_MODULE
 * @{
 */

/**
 * @brief Enumeration for network restriction type.
 * @since_tizen 5.0
 */
typedef enum {
	STC_RSTN_TYPE_UNKNOWN,
	STC_RSTN_TYPE_ACCEPT,
	STC_RSTN_TYPE_DROP,
	STC_RSTN_TYPE_LAST_ELEM,
} stc_restriction_type_e;

/**
* @}
*/

/*****************************************************************************
 *  Callback functions
 *****************************************************************************/

/*****************************************************************************
 *  Core API functions
 *****************************************************************************/

/**
 * @addtogroup CAPI_NETWORK_STC_RESTRICTION_MODULE
 * @{
 */

/**
 * @brief Creates the restriction rule handle.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a handle using stc_restriction_rule_destroy().
 *
 * @param[in] stc The stc handle
 * @param[out] rule The restriction rule handle
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
 * @see stc_restriction_rule_destroy()
 */
int stc_restriction_rule_create(stc_h stc, stc_restriction_rule_h *rule);

/**
 * @brief Destroys the restriction rule handle.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 */
int stc_restriction_rule_destroy(stc_restriction_rule_h rule);

/**
 * @brief Sets the application ID for restriction rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[in] app_id The application ID
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_get_app_id()
 */
int stc_restriction_rule_set_app_id(stc_restriction_rule_h rule,
		const char *app_id);

/**
 * @brief Sets the interface name for restriction rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[in] iface_name The interface name
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
 * @see #stc_restriction_rule_h
 * @see #stc_iface_type_e
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_get_iface_name()
 */
int stc_restriction_rule_set_iface_name(	stc_restriction_rule_h rule,
		const char *iface_name);

/**
 * @brief Sets the interface type for restriction rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[in] iface_type The interface type
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
 * @see #stc_restriction_rule_h
 * @see #stc_iface_type_e
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_get_iface_type()
 */
int stc_restriction_rule_set_iface_type(stc_restriction_rule_h rule,
		stc_iface_type_e iface_type);

/**
 * @brief Sets the type for restriction rule.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[in] type The restriction type
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
 * @see #stc_restriction_rule_h
 * @see #stc_restriction_type_e
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_get_type()
 */
int stc_restriction_rule_set_type(stc_restriction_rule_h rule,
		stc_restriction_type_e type);

/**
 * @brief Sets the limit for restriction rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[in] data_limit The data limit
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_get_limit()
 */
int stc_restriction_rule_set_limit(stc_restriction_rule_h rule,
				   int64_t data_limit);

/**
 * @brief Sets the warning limit for restriction rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[in] data_warn_limit The data warning limit
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_get_warning_limit()
 */
int stc_restriction_rule_set_warning_limit(stc_restriction_rule_h rule,
					   int64_t data_warn_limit);

/**
 * @brief Sets the monthly data limit for restriction rule.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[in] limit The data limit
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_get_monthly_limit()
 */
int stc_restriction_rule_set_monthly_limit(stc_restriction_rule_h rule,
					   int64_t limit);

/**
 * @brief Sets the weekly data limit for restriction rule.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[in] limit The data limit
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_get_weekly_limit()
 */
int stc_restriction_rule_set_weekly_limit(stc_restriction_rule_h rule,
					  int64_t limit);

/**
 * @brief Sets the daily data limit for restriction rule.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[in] limit The data limit
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_get_daily_limit()
 */
int stc_restriction_rule_set_daily_limit(stc_restriction_rule_h rule,
					 int64_t limit);

/**
 * @brief Sets the month start date for restriction rule.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[in] month_start_date The month start date
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_get_month_start_date()
 */
int stc_restriction_rule_set_month_start_date(stc_restriction_rule_h rule,
					      int month_start_date);

/**
 * @brief Sets the roaming type for restriction rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[in] roaming_type The roaming type
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
 * @see #stc_restriction_rule_h
 * @see #stc_roaming_type_e
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_get_roaming_type()
 */
int stc_restriction_rule_set_roaming_type(stc_restriction_rule_h rule,
		stc_roaming_type_e roaming_type);

/**
 * @brief Sets the subscriber ID for restriction rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[in] subscriber_id The subscriber ID
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_get_subscriber_id()
 */
int stc_restriction_rule_set_subscriber_id(stc_restriction_rule_h rule, const char *subscriber_id);

/**
 * @brief Gets the application ID for restriction rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[out] app_id The application ID
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_app_id()
 */
int stc_restriction_rule_get_app_id(stc_restriction_rule_h rule, char **app_id);

/**
 * @brief Gets the interface name for restriction rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[out] iface_name The interface name
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
 * @see #stc_restriction_rule_h
 * @see #stc_iface_type_e
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_iface_name()
 */
int stc_restriction_rule_get_iface_name(stc_restriction_rule_h rule,
		char **iface_name);

/**
 * @brief Gets the interface type for restriction rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[out] iface_type The interface type
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
 * @see #stc_restriction_rule_h
 * @see #stc_iface_type_e
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_iface_type()
 */
int stc_restriction_rule_get_iface_type(stc_restriction_rule_h rule,
		stc_iface_type_e *iface_type);

/**
 * @brief Gets the type for restriction rule.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
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
 * @see #stc_restriction_rule_h
 * @see #stc_restriction_type_e
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_type()
 */
int stc_restriction_rule_get_type(stc_restriction_rule_h rule,
		stc_restriction_type_e *type);

/**
 * @brief Gets the limit for restriction rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[out] data_limit The data limit
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_limit()
 */
int stc_restriction_rule_get_limit(stc_restriction_rule_h rule,
				   int64_t *data_limit);

/**
 * @brief Gets the warning limit for restriction rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[out] data_warn_limit The data warning limit
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_warning_limit()
 */
int stc_restriction_rule_get_warning_limit(stc_restriction_rule_h rule,
					   int64_t *data_warn_limit);

/**
 * @brief Gets the montlhy data limit for restriction rule.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[out] limit The data limit
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_monthly_limit()
 */
int stc_restriction_rule_get_monthly_limit(stc_restriction_rule_h rule,
					   int64_t *limit);

/**
 * @brief Gets the weekly data limit for restriction rule.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[out] limit The data limit
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_weekly_limit()
 */
int stc_restriction_rule_get_weekly_limit(stc_restriction_rule_h rule,
					  int64_t *limit);

/**
 * @brief Gets the daily data limit for restriction rule.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[out] limit The data limit
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_daily_limit()
 */
int stc_restriction_rule_get_daily_limit(stc_restriction_rule_h rule,
					 int64_t *limit);

/**
 * @brief Gets the month start date for restriction rule.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[out] month_start_date The month start date
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_month_start_date()
 */
int stc_restriction_rule_get_month_start_date(stc_restriction_rule_h rule,
					      int *month_start_date);

/**
 * @brief Gets the roaming type for restriction rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[out] roaming The roaming type
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
 * @see #stc_restriction_rule_h
 * @see #stc_roaming_type_e
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_roaming_type()
 */
int stc_restriction_rule_get_roaming_type(stc_restriction_rule_h rule,
		stc_roaming_type_e *roaming_type);

/**
 * @brief Gets the subscriber ID for restriction rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The restriction rule handle
 * @param[out] subscriber_id The subscriber ID
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
 * @see #stc_restriction_rule_h
 * @see stc_restriction_rule_create()
 * @see stc_restriction_rule_destroy()
 * @see stc_restriction_rule_set_subscriber_id()
 */
int stc_restriction_rule_get_subscriber_id(stc_restriction_rule_h rule, char **subscriber_id);

/**
 * @brief Creates the restriction list handle.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a handle using stc_restriction_list_destroy().
 *
 * @param[in] stc The stc handle
 * @param[out] list The restriction list handle
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
 * @see #stc_restriction_list_h
 * @see stc_initialize()
 * @see stc_restriction_list_destroy()
 */
int stc_restriction_list_create(stc_h stc, stc_restriction_list_h *list_h);

/**
 * @brief Destroys the restriction list handle.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] list The restriction list handle
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
 * @see stc_restriction_list_create()
 */
int stc_restriction_list_destroy(stc_restriction_list_h list_h);
/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_STC_RSTN_INTERNAL_H__ */
