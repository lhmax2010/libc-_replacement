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

#ifndef __TIZEN_STC_PCAP_INTERNAL_H__
#define __TIZEN_STC_PCAP_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

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
 * @brief The pcap handle.
 * @since_tizen 5.0
 */
typedef void *stc_pcap_h;

/**
* @}
*/

/*****************************************************************************
 *  Enumeration
 *****************************************************************************/

/*****************************************************************************
 *  Callback functions
 *****************************************************************************/

typedef stc_callback_ret_e (*stc_pcap_info_cb)(stc_pcap_h info_h, void *user_data);

/*****************************************************************************
 *  Core API functions
 *****************************************************************************/

/**
 * @addtogroup CAPI_NETWORK_STC_PCAP_MODULE
 * @{
 */

/**
 * @brief Creates the pcap handle.
 * @since_tizen 5.0
 * @remarks You must release @a handle using stc_pcap_destroy().
 *
 * @param[in] stc        The stc handle
 * @param[out] pcap_h    The pcap handle
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
 * @see #stc_pcap_h
 * @see stc_initialize()
 * @see stc_pcap_destroy()
 */
int stc_pcap_create(stc_h stc, stc_pcap_h *pcap_h);

/**
 * @brief Destroys the pcap handle.
 * @since_tizen 5.0
 *
 * @param[in] pcap_h    The pcap handle
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
 * @see #stc_pcap_h
 * @see stc_pcap_create()
 */
int stc_pcap_destroy(stc_pcap_h pcap_h);

/**
 * @brief Starts packet capture.
 * @since_tizen 5.0
 *
 * @param[in] stc       The stc handle
 * @param[in] pcap_h    The pcap handle
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
 * @see #stc_pcap_h
 * @see stc_pcap_create()
 * @see stc_pcap_destroy()
 */
int stc_pcap_start(stc_h stc, stc_pcap_h pcap_h);

/**
 * @brief Stops packet capture.
 * @since_tizen 5.0
 *
 * @param[in] stc       The stc handle
 * @param[in] pcap_h    The pcap handle
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
 * @see #stc_pcap_h
 * @see stc_pcap_create()
 * @see stc_pcap_destroy()
 */
int stc_pcap_stop(stc_h stc, stc_pcap_h pcap_h);

/**
 * @brief Gets all packet capture.
 * @since_tizen 5.0
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
 * @see #stc_pcap_h
 * @see stc_pcap_create()
 * @see stc_pcap_destroy()
 */
int stc_pcap_foreach(stc_h stc, stc_pcap_info_cb callback, void *user_data);

/**
 * @brief Finds all devices.
 * @since_tizen 5.0
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
 * @see #stc_pcap_h
 * @see stc_pcap_create()
 * @see stc_pcap_destroy()
 */
int stc_pcap_find_all_devs(stc_h stc, stc_pcap_info_cb callback, void *user_data);

/**
 * @brief Sets interface name to pcap.
 * @since_tizen 5.0
 *
 * @param[in] pcap_h    The pcap handle
 * @param[in] ifname    The interface name
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
 * @see #stc_pcap_h
 * @see stc_pcap_create()
 * @see stc_pcap_destroy()
 * @see stc_pcap_get_ifname()
 */
int stc_pcap_set_ifname(stc_pcap_h pcap_h, char *ifname);

/**
 * @brief Sets group of nflog to pcap.
 * @since_tizen 5.0
 *
 * @param[in] pcap_h    The pcap handle
 * @param[in] group     The group of nflog
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
 * @see #stc_pcap_h
 * @see stc_pcap_create()
 * @see stc_pcap_destroy()
 * @see stc_pcap_get_nflog_group()
 */
int stc_pcap_set_nflog_group(stc_pcap_h pcap_h, int group);

/**
 * @brief Gets interface name to pcap.
 * @since_tizen 5.0
 *
 * @param[in] pcap_h    The pcap handle
 * @param[out] ifname   The interface name
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
 * @see #stc_pcap_h
 * @see stc_pcap_create()
 * @see stc_pcap_destroy()
 * @see stc_pcap_set_ifname()
 */
int stc_pcap_get_ifname(stc_pcap_h pcap_h, char **ifname);

/**
 * @brief Gets group of nflog to pcap.
 * @since_tizen 5.0
 *
 * @param[in] pcap_h    The pcap handle
 * @param[out] group    The group of nflog
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
 * @see #stc_pcap_h
 * @see stc_pcap_create()
 * @see stc_pcap_destroy()
 * @see stc_pcap_set_nflog_group()
 */
int stc_pcap_get_nflog_group(stc_pcap_h pcap_h, int *group);

/**
 * @brief Gets name of device.
 * @since_tizen 5.0
 *
 * @param[in] pcap_h    The pcap handle
 * @param[out] name     The name of device
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
 * @see #stc_pcap_h
 * @see stc_pcap_create()
 * @see stc_pcap_destroy()
 */
int stc_pcap_get_dev_name(stc_pcap_h pcap_h, char **name);

/**
 * @brief Gets description of device.
 * @since_tizen 5.0
 *
 * @param[in] pcap_h    The pcap handle
 * @param[out] decs     The description of device
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
 * @see #stc_pcap_h
 * @see stc_pcap_create()
 * @see stc_pcap_destroy()
 */
int stc_pcap_get_dev_decs(stc_pcap_h pcap_h, char **decs);

/**
 * @brief Gets net of device.
 * @since_tizen 5.0
 *
 * @param[in] pcap_h    The pcap handle
 * @param[out] net      The net of device
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
 * @see #stc_pcap_h
 * @see stc_pcap_create()
 * @see stc_pcap_destroy()
 */
int stc_pcap_get_dev_net(stc_pcap_h pcap_h, char **net);

/**
 * @brief Gets mask of device.
 * @since_tizen 5.0
 *
 * @param[in] pcap_h    The pcap handle
 * @param[out] mask     The mask of device
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
 * @see #stc_pcap_h
 * @see stc_pcap_create()
 * @see stc_pcap_destroy()
 */
int stc_pcap_get_dev_mask(stc_pcap_h pcap_h, char **mask);

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_STC_PCAP_INTERNAL_H__ */
