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

#ifndef __TIZEN_STC_FW_INTERNAL_H__
#define __TIZEN_STC_FW_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file stc_fw_internal.h
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
 * @addtogroup CAPI_NETWORK_STC_FIREWALL_MODULE
 * @{
 */

/**
 * @brief The firewall chain handle.
 * @since_tizen 5.0
 */
typedef void *stc_firewall_chain_h;

/**
 * @brief The firewall rule handle.
 * @since_tizen 5.0
 */
typedef void *stc_firewall_rule_h;

/**
* @}
*/

/*****************************************************************************
 *  Enumeration
 *****************************************************************************/

/**
 * @addtogroup CAPI_NETWORK_STC_FIREWALL_MODULE
 * @{
 */

typedef enum {
	STC_FIREWALL_UNKNOWN,
	STC_FIREWALL_UNLOCKED,
	STC_FIREWALL_LOCKED
} stc_firewall_lock_e;

/**
 * @brief Enumeration for firewall direction.
 * @since_tizen 5.0
 */
typedef enum {
	STC_FIREWALL_DIRECTION_NONE,
	STC_FIREWALL_DIRECTION_IN,
	STC_FIREWALL_DIRECTION_OUT
} stc_firewall_direction_e;

/**
 * @brief Enumeration for firewall IP type.
 * @since_tizen 5.0
 */
typedef enum {
	STC_FIREWALL_IP_NONE,
	STC_FIREWALL_IP_SINGLE,
	STC_FIREWALL_IP_MASK,
	STC_FIREWALL_IP_RANGE
} stc_firewall_ip_type_e;

/**
 * @brief Enumeration for firewall port type.
 * @since_tizen 5.0
 */
typedef enum {
	STC_FIREWALL_PORT_NONE,
	STC_FIREWALL_PORT_SINGLE,
	STC_FIREWALL_PORT_RANGE
} stc_firewall_port_type_e;

/**
 * @brief Enumeration for firewall protocol type.
 * @since_tizen 5.0
 */
typedef enum {
	STC_FIREWALL_PROTOCOL_NONE,
	STC_FIREWALL_PROTOCOL_TCP,
	STC_FIREWALL_PROTOCOL_UDP,
	STC_FIREWALL_PROTOCOL_ICMP,
	STC_FIREWALL_PROTOCOL_ALL,
} stc_firewall_protocol_type_e;

/**
 * @brief Enumeration for firewall family type.
 * @since_tizen 5.0
 */
typedef enum {
	STC_FIREWALL_FAMILY_NONE,
	STC_FIREWALL_FAMILY_V4,
	STC_FIREWALL_FAMILY_V6
} stc_firewall_family_type_e;

/**
 * @brief Enumeration for firewall chain target.
 * @since_tizen 5.0
 */
typedef enum {
	STC_FIREWALL_CHAIN_TARGET_NONE,
	STC_FIREWALL_CHAIN_TARGET_INPUT,
	STC_FIREWALL_CHAIN_TARGET_OUTPUT,
	STC_FIREWALL_CHAIN_TARGET_MAX
} stc_firewall_chain_target_e;

/**
 * @brief Enumeration for firewall rule target.
 * @since_tizen 5.0
 */
typedef enum {
	STC_FIREWALL_RULE_TARGET_NONE,
	STC_FIREWALL_RULE_TARGET_ACCEPT,
	STC_FIREWALL_RULE_TARGET_DROP,
	STC_FIREWALL_RULE_TARGET_LOG,
	STC_FIREWALL_RULE_TARGET_NFLOG,
	STC_FIREWALL_RULE_TARGET_MAX,
} stc_firewall_rule_target_e;

/**
* @}
*/


/*****************************************************************************
 *  Callback functions
 *****************************************************************************/

/**
 * @addtogroup CAPI_NETWORK_STC_FIREWALL_MODULE
 * @{
 */

/**
 * @brief Called for enumerate firewall chain information.
 * @since_tizen 5.0
 *
 * @param[in] info          The firewall chain handle
 * @param[in] user_data		The user data passed from the callback firewall chain function
 * @return	@c STC_CALLBACK_CONTINUE to continue with the next iteration of the loop,
 *			otherwise @c STC_CALLBACK_CANCEL to break out of the loop
 *
 * @pre	stc_firewall_chain_foreach() will invoke this callback.
 * @see stc_firewall_chain_get_name()
 * @see stc_firewall_chain_get_target()
 * @see stc_firewall_chain_get_priority()
 * @see	stc_firewall_chain_foreach()
 */
typedef stc_callback_ret_e (*stc_firewall_chain_info_cb)(stc_firewall_chain_h info_h,
			void *user_data);

/**
 * @brief Called for enumerate firewall rule information.
 * @since_tizen 5.0
 *
 * @param[in] info          The firewall rule handle
 * @param[in] user_data		The user data passed from the callback firewall rule function
 * @return	@c STC_CALLBACK_CONTINUE to continue with the next iteration of the loop,
 *			otherwise @c STC_CALLBACK_CANCEL to break out of the loop
 *
 * @pre	stc_firewall_rule_foreach() will invoke this callback.
 * @see stc_firewall_rule_get_chain()
 * @see stc_firewall_rule_get_direction()
 * @see stc_firewall_rule_get_src_ip_type()
 * @see stc_firewall_rule_get_dst_ip_type()
 * @see stc_firewall_rule_get_src_port_type()
 * @see stc_firewall_rule_get_dst_port_type()
 * @see stc_firewall_rule_get_protocol_type()
 * @see stc_firewall_rule_get_family_type()
 * @see stc_firewall_rule_get_src_ip()
 * @see stc_firewall_rule_get_dst_ip()
 * @see stc_firewall_rule_get_src_port()
 * @see stc_firewall_rule_get_dst_port()
 * @see stc_firewall_rule_get_ifname()
 * @see stc_firewall_rule_get_target()
 * @see	stc_firewall_rule_foreach()
 */
typedef stc_callback_ret_e (*stc_firewall_rule_info_cb)(stc_firewall_rule_h info,
			void *user_data);

/**
* @}
*/

/*****************************************************************************
 *  Core API functions
 *****************************************************************************/

/**
 * @addtogroup CAPI_NETWORK_STC_FIREWALL_MODULE
 * @{
 */

/**
 * @brief Locks the firewall.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/firewall.admin
 *
 * @param[in] stc        The stc handle
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
 * @see stc_initialize()
 * @see stc_firewall_unlock()
 * @see stc_firewall_get_lock()
 */
int stc_firewall_lock(stc_h stc);

/**
 * @brief Unlocks the firewall.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/firewall.admin
 *
 * @param[in] stc        The stc handle
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
 * @see stc_initialize()
 * @see stc_firewall_lock()
 * @see stc_firewall_get_lock()
 */
int stc_firewall_unlock(stc_h stc);

/**
 * @brief Gets the firewall lock state.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/firewall.admin
 *
 * @param[in] stc        The stc handle
 * @param[out] state     The firewall lock state
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
 * @see stc_initialize()
 * @see stc_firewall_lock()
 * @see stc_firewall_unlock()
 */
int stc_firewall_get_lock(stc_h stc, stc_firewall_lock_e *state);

/**
 * @brief Creates the firewall chain handle.
 * @since_tizen 5.0
 * @remarks You must release @a handle using stc_firewall_chain_destroy().
 *
 * @param[in] stc        The stc handle
 * @param[in] name       The chain name
 * @param[out] chain     The firewall chain handle
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
 * @see #stc_firewall_chain_h
 * @see stc_initialize()
 * @see stc_firewall_chain_destroy()
 */
int stc_firewall_chain_create(stc_h stc, const char *name,
				stc_firewall_chain_h *chain_h);

/**
 * @brief Clones the firewall chain handle.
 * @since_tizen 5.0
 * @remarks You must release @a handle using stc_firewall_chain_destroy().
 *
 * @param[in] origin        The origin firewall chain handle
 * @param[out] cloned_chain The cloned firewall chain handle
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
 * @see #stc_firewall_chain_h
 * @see stc_initialize()
 * @see stc_firewall_chain_create()
 * @see stc_firewall_chain_destroy()
 */
int stc_firewall_chain_clone(stc_firewall_chain_h origin_h,
			stc_firewall_chain_h *cloned_chain_h);

/**
 * @brief Destroys the firewall chain handle.
 * @since_tizen 5.0
 *
 * @param[in] chain      The firewall chain handle
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
 * @see #stc_firewall_chain_h
 * @see stc_firewall_chain_create()
 */
int stc_firewall_chain_destroy(stc_firewall_chain_h chain_h);

/**
 * @brief Gets the name for chain.
 * @since_tizen 5.0
 *
 * @param[in] chain      The firewall chain handle
 * @param[out] name      The chain name
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
 * @see #stc_firewall_chain_h
 * @see stc_firewall_chain_create()
 * @see stc_firewall_chain_destroy()
 */
int stc_firewall_chain_get_name(stc_firewall_chain_h chain, char **name);

/**
 * @brief Gets the target type for chain.
 * @since_tizen 5.0
 *
 * @param[in] chain      The firewall chain handle
 * @param[out] target    The target type
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
 * @see #stc_firewall_chain_h
 * @see stc_firewall_chain_create()
 * @see stc_firewall_chain_destroy()
 */
int stc_firewall_chain_get_target(stc_firewall_chain_h chain_h,
			stc_firewall_chain_target_e *target);

/**
 * @brief Gets the priority for chain.
 * @since_tizen 5.0
 *
 * @param[in] chain      The firewall chain handle
 * @param[out] priority  The priority
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
 * @see #stc_firewall_chain_h
 * @see stc_firewall_chain_create()
 * @see stc_firewall_chain_destroy()
 */
int stc_firewall_chain_get_priority(stc_firewall_chain_h chain_h,
			unsigned int *priority);

/**
 * @brief Adds the chain.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/firewall.common
 *
 * @param[in] stc        The stc handle
 * @param[in] chain      The firewall chain handle
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
 * @see #stc_firewall_chain_h
 * @see stc_firewall_chain_create()
 * @see stc_firewall_chain_destroy()
 */
int stc_firewall_chain_add(stc_h stc, stc_firewall_chain_h chain_h);

/**
 * @brief Removes the chain.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/firewall.common
 *
 * @param[in] stc        The stc handle
 * @param[in] chain      The firewall chain handle
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
 * @see #stc_firewall_chain_h
 * @see stc_firewall_chain_create()
 * @see stc_firewall_chain_destroy()
 */
int stc_firewall_chain_remove(stc_h stc, stc_firewall_chain_h chain_h);

/**
 * @brief Flushs the chain.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/firewall.common
 *
 * @param[in] stc        The stc handle
 * @param[in] chain      The firewall chain handle
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
 * @see #stc_firewall_chain_h
 * @see stc_firewall_chain_create()
 * @see stc_firewall_chain_destroy()
 */
int stc_firewall_chain_flush(stc_h stc, stc_firewall_chain_h chain_h);

/**
 * @brief Gets all chains.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/firewall.common
 *
 * @param[in] stc        The stc handle
 * @param[in] callback   The callback to be called
 * @param[in] user_data  The user data passed to the callback function
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
 * @see #stc_firewall_chain_h
 * @see stc_firewall_chain_create()
 * @see stc_firewall_chain_destroy()
 */
int stc_firewall_chain_foreach(stc_h stc,
			stc_firewall_chain_info_cb callback, void *user_data);

/**
 * @brief Sets the chain to target.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/firewall.common
 * @remarks You should set only one chain.
 *
 * @param[in] stc        The stc handle
 * @param[in] chain      The firewall chain handle
 * @param[in] target     The target type
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
 * @see #stc_firewall_chain_h
 * @see stc_firewall_chain_create()
 * @see stc_firewall_chain_destroy()
 */
int stc_firewall_chain_set(stc_h stc, stc_firewall_chain_h chain_h,
			stc_firewall_chain_target_e target);

/**
 * @brief Unsets the chain.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/firewall.common
 *
 * @param[in] stc        The stc handle
 * @param[in] chain      The firewall chain handle
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
 * @see #stc_firewall_chain_h
 * @see stc_firewall_chain_create()
 * @see stc_firewall_chain_destroy()
 */
int stc_firewall_chain_unset(stc_h stc, stc_firewall_chain_h chain_h);

/**
 * @brief Creates the firewall rule handle.
 * @since_tizen 5.0
 * @remarks You must release @a handle using stc_firewall_rule_destroy().
 *
 * @param[in] stc        The stc handle
 * @param[out] rule      The firewall rule handle
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
 * @see #stc_firewall_rule_h
 * @see stc_initialize()
 * @see stc_firewall_rule_destroy()
 */
int stc_firewall_rule_create(stc_h stc, stc_firewall_rule_h *rule_h);

/**
 * @brief Clones the firewall rule handle.
 * @since_tizen 5.0
 * @remarks You must release @a handle using stc_firewall_rule_destroy().
 *
 * @param[in] origin        The origin firewall rule handle
 * @param[out] cloned_rule  The cloned firewall rule handle
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
 * @see #stc_firewall_rule_h
 * @see stc_initialize()
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 */
int stc_firewall_rule_clone(stc_firewall_rule_h origin_h,
			stc_firewall_rule_h *cloned_rule_h);

/**
 * @brief Destroys the firewall rule handle.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall chain handle
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 */
int stc_firewall_rule_destroy(stc_firewall_rule_h rule_h);

/**
 * @brief Sets the chain name to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] chain      The chain name
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_chain()
 */
int stc_firewall_rule_set_chain(stc_firewall_rule_h rule_h, char *chain);

/**
 * @brief Sets the direction to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] direction  The direction
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_direction()
 */
int stc_firewall_rule_set_direction(stc_firewall_rule_h rule_h,
				stc_firewall_direction_e direction);

/**
 * @brief Sets the source IP type to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] type       The IP type
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_src_ip_type()
 */
int stc_firewall_rule_set_src_ip_type(stc_firewall_rule_h rule_h,
				stc_firewall_ip_type_e type);

/**
 * @brief Sets the destination IP type to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] type       The IP type
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_dst_ip_type()
 */
int stc_firewall_rule_set_dst_ip_type(stc_firewall_rule_h rule_h,
				stc_firewall_ip_type_e type);

/**
 * @brief Sets the source port type to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] type       The port type
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_src_port_type()
 */
int stc_firewall_rule_set_src_port_type(stc_firewall_rule_h rule_h,
				stc_firewall_port_type_e type);

/**
 * @brief Sets the destination port type to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] type       The port type
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_dst_port_type()
 */
int stc_firewall_rule_set_dst_port_type(stc_firewall_rule_h rule_h,
				stc_firewall_port_type_e type);

/**
 * @brief Sets the protocol type to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] type       The protocol type
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_protocol_type()
 */
int stc_firewall_rule_set_protocol_type(stc_firewall_rule_h rule_h,
				stc_firewall_protocol_type_e type);

/**
 * @brief Sets the address family type to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] type       The address family type
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_family_type()
 */
int stc_firewall_rule_set_family_type(stc_firewall_rule_h rule_h,
				stc_firewall_family_type_e type);

/**
 * @brief Sets source IP addresses to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] ip1        The address 1
 * @param[in] ip2        The address 2 which will be IP address or mask
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_src_ip_type()
 * @see stc_firewall_rule_get_src_ip()
 */
int stc_firewall_rule_set_src_ip(stc_firewall_rule_h rule_h,
			char *ip1, char *ip2);

/**
 * @brief Sets destination IP addresses to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] ip1        The address 1
 * @param[in] ip2        The address 2 which will be IP address or mask
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_dst_ip_type()
 * @see stc_firewall_rule_get_dst_ip()
 */
int stc_firewall_rule_set_dst_ip(stc_firewall_rule_h rule_h,
			char *ip1, char *ip2);

/**
 * @brief Sets source ports to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] port1      The port 1
 * @param[in] port2      The port 2
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_src_port_type()
 * @see stc_firewall_rule_get_src_port()
 */
int stc_firewall_rule_set_src_port(stc_firewall_rule_h rule_h,
			unsigned int port1, unsigned int port2);

/**
 * @brief Sets destination ports to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] port1      The port 1
 * @param[in] port2      The port 2
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_dst_port_type()
 * @see stc_firewall_rule_get_dst_port()
 */
int stc_firewall_rule_set_dst_port(stc_firewall_rule_h rule_h,
			unsigned int port1, unsigned int port2);

/**
 * @brief Sets interface name to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] ifname     The interface name
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_ifname()
 */
int stc_firewall_rule_set_ifname(stc_firewall_rule_h rule_h, char *ifname);

/**
 * @brief Sets target type to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] target     The target type
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_target()
 */
int stc_firewall_rule_set_target(stc_firewall_rule_h rule_h,
			stc_firewall_rule_target_e target);

/**
 * @brief Sets log level to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] level      The level
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_log_level()
 */
int stc_firewall_rule_set_log_level(stc_firewall_rule_h rule_h,
			unsigned char level);

/**
 * @brief Sets log prefix to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] prefix     The prefix
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_log_prefix()
 */
int stc_firewall_rule_set_log_prefix(stc_firewall_rule_h rule_h,
			char *prefix);

/**
 * @brief Sets nflog group to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] group      The group
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_nflog_group()
 */
int stc_firewall_rule_set_nflog_group(stc_firewall_rule_h rule_h,
			unsigned int group);

/**
 * @brief Sets nflog prefix to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] prefix     The prefix
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_nflog_prefix()
 */
int stc_firewall_rule_set_nflog_prefix(stc_firewall_rule_h rule_h,
			char *prefix);

/**
 * @brief Sets nflog range to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] range      The range
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_nflog_range()
 */
int stc_firewall_rule_set_nflog_range(stc_firewall_rule_h rule_h,
			unsigned int range);

/**
 * @brief Sets nflog threshold to rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[in] threshold  The threshold
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_nflog_threshold()
 */
int stc_firewall_rule_set_nflog_threshold(stc_firewall_rule_h rule_h,
			unsigned int threshold);

/**
 * @brief Gets the chain name from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] chain     The chain name
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_chain()
 */
int stc_firewall_rule_get_chain(stc_firewall_rule_h rule_h, char **chain);

/**
 * @brief Gets the direction from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] direction The direction
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_direction()
 */
int stc_firewall_rule_get_direction(stc_firewall_rule_h rule_h,
				stc_firewall_direction_e *direction);

/**
 * @brief Gets the source IP type from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] type      The IP type
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_src_ip_type()
 */
int stc_firewall_rule_get_src_ip_type(stc_firewall_rule_h rule_h,
				stc_firewall_ip_type_e *type);

/**
 * @brief Gets the destination IP type from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] type      The IP type
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_dst_ip_type()
 */
int stc_firewall_rule_get_dst_ip_type(stc_firewall_rule_h rule_h,
				stc_firewall_ip_type_e *type);

/**
 * @brief Gets the source port type from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] type      The port type
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_src_port_type()
 */
int stc_firewall_rule_get_src_port_type(stc_firewall_rule_h rule_h,
				stc_firewall_port_type_e *type);

/**
 * @brief Gets the destination port type from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] type      The port type
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_dst_port_type()
 */
int stc_firewall_rule_get_dst_port_type(stc_firewall_rule_h rule_h,
				stc_firewall_port_type_e *type);

/**
 * @brief Gets the protocol type from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] type      The protocol type
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_protocol_type()
 */
int stc_firewall_rule_get_protocol_type(stc_firewall_rule_h rule_h,
				stc_firewall_protocol_type_e *type);

/**
 * @brief Gets the address family type from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] type      The address family type
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_family_type()
 */
int stc_firewall_rule_get_family_type(stc_firewall_rule_h rule_h,
				stc_firewall_family_type_e *type);

/**
 * @brief Gets the source IP addresses from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] ip1       The address 1
 * @param[out] ip2       The address 2 which will be IP address or mask
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_src_ip_type()
 * @see stc_firewall_rule_set_src_ip()
 */
int stc_firewall_rule_get_src_ip(stc_firewall_rule_h rule_h,
			char **ip1, char **ip2);

/**
 * @brief Gets the destination IP addresses from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] ip1       The address 1
 * @param[out] ip2       The address 2 which will be IP address or mask
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_dst_ip_type()
 * @see stc_firewall_rule_set_dst_ip()
 */
int stc_firewall_rule_get_dst_ip(stc_firewall_rule_h rule_h,
			char **ip1, char **ip2);

/**
 * @brief Gets the source ports from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] port1     The port 1
 * @param[out] port2     The port 2
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_src_port_type()
 * @see stc_firewall_rule_set_src_port()
 */
int stc_firewall_rule_get_src_port(stc_firewall_rule_h rule_h,
				unsigned int *port1, unsigned int *port2);

/**
 * @brief Gets the destination ports from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] port1     The port 1
 * @param[out] port2     The port 2
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_get_dst_port_type()
 * @see stc_firewall_rule_set_dst_port()
 */
int stc_firewall_rule_get_dst_port(stc_firewall_rule_h rule_h,
				unsigned int *port1, unsigned int *port2);

/**
 * @brief Gets the interface name from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] ifname    The interface name
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_ifname()
 */
int stc_firewall_rule_get_ifname(stc_firewall_rule_h rule_h, char **ifname);

/**
 * @brief Gets the target type from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] target    The target type
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_target()
 */
int stc_firewall_rule_get_target(stc_firewall_rule_h rule_h,
			stc_firewall_rule_target_e *target);

/**
 * @brief Gets log level from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] level     The level
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_log_level()
 */
int stc_firewall_rule_get_log_level(stc_firewall_rule_h rule_h,
			unsigned char *level);

int stc_firewall_rule_get_log_prefix(stc_firewall_rule_h rule_h,
			char **prefix);

/**
 * @brief Gets nflog group from rule.
 * @since_tizen 5.0
 *
 * @param[in] rule       The firewall rule handle
 * @param[out] group     The group
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 * @see stc_firewall_rule_set_nflog_group()
 */
int stc_firewall_rule_get_nflog_group(stc_firewall_rule_h rule_h,
			unsigned int *group);

int stc_firewall_rule_get_nflog_prefix(stc_firewall_rule_h rule_h,
			char **prefix);

int stc_firewall_rule_get_nflog_range(stc_firewall_rule_h rule_h,
			unsigned int *range);

int stc_firewall_rule_get_nflog_threshold(stc_firewall_rule_h rule_h,
			unsigned int *threshold);

/**
 * @brief Adds the rule.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/firewall.common
 *
 * @param[in] stc        The stc handle
 * @param[in] rule       The firewall rule handle
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 */
int stc_firewall_rule_add(stc_h stc, stc_firewall_rule_h rule_h);

/**
 * @brief Removes the rule.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/firewall.common
 *
 * @param[in] stc        The stc handle
 * @param[in] rule       The firewall rule handle
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 */
int stc_firewall_rule_remove(stc_h stc, stc_firewall_rule_h rule_h);

/**
 * @brief Updates the rule.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/firewall.common
 *
 * @param[in] stc        The stc handle
 * @param[in] rule       The firewall rule handle
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 */
int stc_firewall_rule_update(stc_h stc, stc_firewall_rule_h rule_h);

/**
 * @brief Gets all rules.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/firewall.common
 *
 * @param[in] stc        The stc handle
 * @param[in] callback   The callback to be called
 * @param[in] user_data  The user data passed to the callback function
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
 * @see #stc_firewall_rule_h
 * @see stc_firewall_rule_create()
 * @see stc_firewall_rule_destroy()
 */
int stc_firewall_rule_foreach(stc_h stc,
			stc_firewall_rule_info_cb callback, void *user_data);

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_STC_FW_INTERNAL_H__ */
