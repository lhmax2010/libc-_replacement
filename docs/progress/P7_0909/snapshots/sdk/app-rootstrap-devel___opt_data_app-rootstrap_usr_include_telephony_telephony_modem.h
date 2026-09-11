/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __CAPI_TELEPHONY_MODEM_H__
#define __CAPI_TELEPHONY_MODEM_H__

/**
 * @file telephony_modem.h
 * @brief This file contains modem APIs.
 */

#include "telephony_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_TELEPHONY_INFORMATION_MODEM
 * @{
 */

/**
 * @deprecated Deprecated since 10.0
 * @brief Enumeration for Modem Power Status.
 * @since_tizen 2.4
 */
typedef enum {
	TELEPHONY_MODEM_POWER_STATUS_UNKNOWN = -1,	/**< Unknown */
	TELEPHONY_MODEM_POWER_STATUS_ON,		/**< Modem power ON */
	TELEPHONY_MODEM_POWER_STATUS_OFF,		/**< Modem power OFF */
	TELEPHONY_MODEM_POWER_STATUS_RESET,		/**< Modem power RESET */
	TELEPHONY_MODEM_POWER_STATUS_LOW,		/**< Modem power LOW */
} telephony_modem_power_status_e;

/**
 * @deprecated Deprecated since 10.0
 * @partner
 * @brief Gets the IMEI (International Mobile Station Equipment Identity) of a mobile phone.
 * @details The IMEI number is used by a GSM network to identify valid devices
 *          and therefore can be used for stopping a stolen phone from accessing that network.
 *
 * @since_tizen 2.3
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/securesysteminfo
 *
 * @remarks You must release @c imei using free() on success case.
 * @remarks Since 5.5, this function requires partner level privilege.
 * @remarks Before 5.5, this function required the %http://tizen.org/privilege/telephony privilege. Since 5.5, it requires the %http://tizen.org/privilege/securesysteminfo privilege.
 * @remarks To avoid unexpected behavior of old version applications which have the %http://tizen.org/privilege/telephony privilege, there is an exception in handling the 'permission denied' error.
 * @remarks For an application with api-version 5.5 or higher, if the application doesn't have %http://tizen.org/privilege/securesysteminfo privilege, this function returns #TELEPHONY_ERROR_PERMISSION_DENIED.
 * @remarks For an application with api-version lower than 5.5, if the application has %http://tizen.org/privilege/telephony privilege, this function returns a pseudo value with #TELEPHONY_ERROR_NONE instead of #TELEPHONY_ERROR_PERMISSION_DENIED. The returned value is not reliable data, it can't confirm the device's identification.
 *
 * @param[in] handle The handle from telephony_init()
 * @param[out] imei The International Mobile Station Equipment Identity
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #TELEPHONY_ERROR_NONE              Successful
 * @retval #TELEPHONY_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TELEPHONY_ERROR_PERMISSION_DENIED Permission denied
 * @retval #TELEPHONY_ERROR_NOT_SUPPORTED     Not supported
 * @retval #TELEPHONY_ERROR_OPERATION_FAILED  Operation failed
 */
int telephony_modem_get_imei(telephony_h handle, char **imei) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Gets the power status of the modem.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from telephony_init()
 * @param[out] status The Modem power status (0=on,1=off,2=reset,3=low)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #TELEPHONY_ERROR_NONE              Successful
 * @retval #TELEPHONY_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TELEPHONY_ERROR_PERMISSION_DENIED Permission denied
 * @retval #TELEPHONY_ERROR_NOT_SUPPORTED     Not supported
 * @retval #TELEPHONY_ERROR_OPERATION_FAILED  Operation failed
 */
int telephony_modem_get_power_status(telephony_h handle, telephony_modem_power_status_e *status) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 10.0
 * @partner
 * @brief Gets the MEID (Mobile Equipment Identifier) of a mobile phone (for CDMA).
 * @since_tizen 3.0
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/securesysteminfo
 *
 * @remarks You must release @c meid using free() on success case.
 * @remarks Since 5.5, this function requires partner level privilege.
 * @remarks Before 5.5, this function required the %http://tizen.org/privilege/telephony privilege. Since 5.5, it requires the %http://tizen.org/privilege/securesysteminfo privilege.
 * @remarks To avoid unexpected behavior of old version applications which have the %http://tizen.org/privilege/telephony privilege, there is an exception in handling the 'permission denied' error.
 * @remarks For an application with api-version 5.5 or higher, if the application doesn't have %http://tizen.org/privilege/securesysteminfo privilege, this function returns #TELEPHONY_ERROR_PERMISSION_DENIED.
 * @remarks For an application with api-version lower than 5.5, if the application has %http://tizen.org/privilege/telephony privilege, this function returns a pseudo value with #TELEPHONY_ERROR_NONE instead of #TELEPHONY_ERROR_PERMISSION_DENIED. The returned value is not reliable data, it can't confirm the device's identification.
 *
 * @param[in] handle The handle from telephony_init()
 * @param[out] meid The Mobile Equipment Identifier
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #TELEPHONY_ERROR_NONE              Successful
 * @retval #TELEPHONY_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TELEPHONY_ERROR_PERMISSION_DENIED Permission denied
 * @retval #TELEPHONY_ERROR_NOT_SUPPORTED     Not supported
 * @retval #TELEPHONY_ERROR_OPERATION_FAILED  Operation failed
 */
int telephony_modem_get_meid(telephony_h handle, char **meid) TIZEN_DEPRECATED_API;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __CAPI_TELEPHONY_MODEM_H__ */
