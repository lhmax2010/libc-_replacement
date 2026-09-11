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

#ifndef __TIZEN_STC_STAT_INTERNAL_H__
#define __TIZEN_STC_STAT_INTERNAL_H__

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
 * @addtogroup CAPI_NETWORK_STC_STATISTICS_MODULE
 * @{
 */

/**
 * @brief The reset rule handle.
 * @since_tizen 4.0
 */
typedef void *stc_reset_rule_h;


/**
* @}
*/

/*****************************************************************************
 *  Enumeration
 *****************************************************************************/

/*****************************************************************************
 *  Callback functions
 *****************************************************************************/

/*****************************************************************************
 *  Core API functions
 *****************************************************************************/

/**
 * @addtogroup CAPI_NETWORK_STC_STATISTICS_MODULE
 * @{
 */

/**
 * @brief Creates the reset rule handle.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a handle using stc_reset_rule_destroy().
 *
 * @param[in] stc The stc handle
 * @param[out] rule The reset rule handle
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
 * @see stc_reset_rule_destroy()
 */
int stc_reset_rule_create(stc_h stc, stc_reset_rule_h *rule);

/**
 * @brief Destroys the reset rule handle.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
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
 * @see #stc_reset_rule_h
 * @see stc_reset_rule_create()
 */
int stc_reset_rule_destroy(stc_reset_rule_h rule);

/**
 * @brief Sets the application ID for reset rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The reset rule handle
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
 * @see #stc_reset_rule_h
 * @see stc_reset_rule_create()
 * @see stc_reset_rule_destroy()
 * @see stc_reset_rule_get_app_id()
 */
int stc_reset_rule_set_app_id(stc_reset_rule_h rule, const char *app_id);

/**
 * @brief Sets the subscriber ID for reset rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The reset rule handle
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
 * @see #stc_reset_rule_h
 * @see stc_reset_rule_create()
 * @see stc_reset_rule_destroy()
 * @see stc_reset_rule_get_subscriber_id()
 */
int stc_reset_rule_set_subscriber_id(stc_reset_rule_h rule, const char *subscriber_id);

/**
 * @brief Sets the interface type for reset rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The reset rule handle
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
 * @see #stc_reset_rule_h
 * @see #stc_iface_type_e
 * @see stc_reset_rule_create()
 * @see stc_reset_rule_destroy()
 * @see stc_reset_rule_get_iface_type()
 */
int stc_reset_rule_set_iface_type(stc_reset_rule_h rule,
		stc_iface_type_e iface_type);

/**
 * @brief Sets the time interval for reset rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The reset rule handle
 * @param[in] from The time interval from
 * @param[in] to The time interval to
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
 * @see #stc_reset_rule_h
 * @see stc_reset_rule_create()
 * @see stc_reset_rule_destroy()
 * @see stc_reset_rule_get_time_interval()
 */
int stc_reset_rule_set_time_interval(stc_reset_rule_h rule,
		time_t from, time_t to);

/**
 * @brief Gets the application ID for reset.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The reset rule handle
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
 * @see #stc_reset_rule_h
 * @see stc_reset_rule_create()
 * @see stc_reset_rule_destroy()
 * @see stc_reset_rule_set_app_id()
 */
int stc_reset_rule_get_app_id(stc_reset_rule_h rule, char **app_id);

/**
 * @brief Gets the subscriber ID for reset rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The reset rule handle
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
 * @see #stc_reset_rule_h
 * @see stc_reset_rule_create()
 * @see stc_reset_rule_destroy()
 * @see stc_reset_rule_set_subscriber_id()
 */
int stc_reset_rule_get_subscriber_id(stc_reset_rule_h rule, char **subscriber_id);

/**
 * @brief Gets the interface type for reset rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The reset rule handle
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
 * @see #stc_reset_rule_h
 * @see #stc_iface_type_e
 * @see stc_reset_rule_create()
 * @see stc_reset_rule_destroy()
 * @see stc_reset_rule_set_iface_type()
 */
int stc_reset_rule_get_iface_type(stc_reset_rule_h rule,
		stc_iface_type_e *iface_type);

/**
 * @brief Gets the time interval for reset rule.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] rule The reset rule handle
 * @param[out] from The time interval from
 * @param[out] to The time interval to
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
 * @see #stc_reset_rule_h
 * @see stc_reset_rule_create()
 * @see stc_reset_rule_destroy()
 * @see stc_reset_rule_set_time_interval()
 */
int stc_reset_rule_get_time_interval(stc_reset_rule_h rule,
		time_t *from, time_t *to);

/**
 * @brief Gets the subscriber ID from statistics information.
 * @since_tizen 4.0
 * @remarks You must release @a subscriber ID using free().
 *
 * @param[in] info The statistics information handle
 * @param[out] subscriber_id The subscriber ID
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
 * @see stc_stats_info_h
 * @see stc_stats_info_cb()
 */
int stc_stats_info_get_subscriber_id(stc_stats_info_h info, char **subscriber_id);

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_STC_STAT_INTERNAL_H__ */
