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

#ifndef __TIZEN_STC_IPT_INTERNAL_H__
#define __TIZEN_STC_IPT_INTERNAL_H__

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
 * @addtogroup CAPI_NETWORK_STC_IPTABLES_MODULE
 * @{
 */

/**
 * @brief Commits iptables rule.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/firewall.common
 * @remarks You must release @a err_str using free().
 *
 * @param[in] stc        The stc handle
 * @param[in] option     The option of iptables rule
 * @param[out] err_num   The error number
 * @param[out] err_str   The error string
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
 * @see stc_initialize()
 */
int stc_iptables_commit(stc_h stc, const char *option,
			int *err_num, char **err_str);

/**
 * @brief Commits ip6tables rule.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/firewall.common
 * @remarks You must release @a err_str using free().
 *
 * @param[in] stc        The stc handle
 * @param[in] option     The option of iptables rule
 * @param[out] err_num   The error number
 * @param[out] err_str   The error string
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
 * @see stc_initialize()
 */
int stc_ip6tables_commit(stc_h stc, const char *option,
			int *err_num, char **err_str);

 /**
 * @brief Initializes STC iptables rule management module.
 * @details This function initializes the resources required for managing
 *          iptables firewall rules. It must be called before any other
 *          iptables operations. Initializes D-Bus connection and internal state.
 * @since_tizen 10.0
 * @remarks You must release @a stc using stc_iptables_deinitialize().
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_OUT_OF_MEMORY           Out of memory
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 *
 * @see stc_iptables_deinitialize()
 */
int stc_iptables_initialize();

/**
 * @brief Deinitializes STC iptables rule management module.
 * @details Releases all resources allocated by stc_iptables_initialize().
 *          Should be called when iptables operations are no longer needed.
 *          Cleans up D-Bus connection and internal state.
 * @since_tizen 10.0
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        General error
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 *
 * @see stc_iptables_initialize()
 */
int stc_iptables_deinitialize();

/**
 * @brief Executes iptables command via D-Bus
 * @details Sends iptables command to system service through D-Bus interface
 * @since_tizen 10.0
 * @remarks Caller must free the error string if provided
 *
 * @param[in] option     The iptables command and options to execute
 * @param[out] err_num   Will contain error code if operation fails
 * @param[out] err_str   Will contain error message (must be freed by caller)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        D-Bus communication failed
 * @retval #STC_ERROR_OUT_OF_MEMORY           Memory allocation failed
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameters
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 *
 * @see stc_iptables_initialize()
 */
int stc_iptables_exec(const char *option, int *err_num, char **err_str);

/**
 * @brief Executes ip6tables command via D-Bus
 * @details Sends ip6tables command to system service through D-Bus interface
 * @since_tizen 10.0
 * @remarks Caller must free the error string if provided
 *
 * @param[in] option     The ip6tables command and options to execute
 * @param[out] err_num   Will contain error code if operation fails
 * @param[out] err_str   Will contain error message (must be freed by caller)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #STC_ERROR_NONE                    Successful
 * @retval #STC_ERROR_OPERATION_FAILED        D-Bus communication failed
 * @retval #STC_ERROR_OUT_OF_MEMORY           Memory allocation failed
 * @retval #STC_ERROR_INVALID_PARAMETER       Invalid parameters
 * @retval #STC_ERROR_INVALID_OPERATION       Invalid operation
 * @retval #STC_ERROR_NOT_INITIALIZED         Not initialized
 *
 * @see stc_iptables_initialize()
 */
int stc_ip6tables_exec(const char *option, int *err_num, char **err_str);

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_STC_IPT_INTERNAL_H__ */
