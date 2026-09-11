/*
 * Phonenumber Utils
 *
 * Copyright (c) 2015 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef __TIZEN_TELEPHONY_PHONE_NUMBER_UTILS_H__
#define __TIZEN_TELEPHONY_PHONE_NUMBER_UTILS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <tizen.h>

#include <phone_number_types.h>
#include <phone_number_errors.h>
#include <phone_number_blocking_rule.h>

/**
 * @file phone_number.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_PHONE_NUMBER_UTILS_MODULE
 *
 * @{
 */


/**
 * @brief Connects to the phonenumber-utils service.
 * @since_tizen 3.0
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE Successful
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 * @see phone_number_disconnect()
 */
int phone_number_connect(void);


/**
 * @brief Disconnects from the phonenumber-utils service.
 * @since_tizen 3.0
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE Successful
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 * @see phone_number_connect()
 */
int phone_number_disconnect(void);


/**
 * @brief Gets the location string from number, region, and language.
 * @since_tizen 2.4
 * @remarks You must release @a location using free().
 * @remarks The phonenumber-utils is changed to client/server architecture for performance improvement since 3.0.
 *          phone_number_connect()/phone_number_disconnect() should be called.
 * @param[in]  number    The number
 * @param[in]  region    The region of number
 * @param[in]  language  The language of location
 * @param[out] location  The location string to be returned
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE Successful
 * @retval #PHONE_NUMBER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PHONE_NUMBER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PHONE_NUMBER_ERROR_FILE_NO_SPACE_ON_DEVICE FS Full
 * @retval #PHONE_NUMBER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_NO_DATA Requested data does not exist
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 * @pre phone_number_connect() should be called to open a connection to the phonenumber-utils service. (Since 3.0)
 * @post phone_number_disconnect() should be called to close a connection to the phonenumber-utils service. (Since 3.0)
 */
int phone_number_get_location_from_number(const char *number,
		phone_number_region_e region, phone_number_lang_e language, char **location);


/**
 * @brief Gets the formatted number.
 * @since_tizen 2.4
 * @remarks You must release @a formatted_number using free().
 * @remarks The phonenumber-utils is changed to client/server architecture for performance improvement since 3.0.
 *          phone_number_connect()/phone_number_disconnect() should be called.
 * @param[in]  number           The number
 * @param[in]  region           The region of number
 * @param[out] formatted_number The formatted number string to be returned
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE Successful
 * @retval #PHONE_NUMBER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PHONE_NUMBER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PHONE_NUMBER_ERROR_FILE_NO_SPACE_ON_DEVICE FS Full
 * @retval #PHONE_NUMBER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_NO_DATA Requested data does not exist
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 * @pre phone_number_connect() should be called to open a connection to the phonenumber-utils service. (Since 3.0)
 * @post phone_number_disconnect() should be called to close a connection to the phonenumber-utils service. (Since 3.0)
 */
int phone_number_get_formatted_number(const char *number, phone_number_region_e region,
		char **formatted_number);


/**
 * @brief Gets the normalized number.
 * @details Normalized number starts with plus('+') and country code, and excludes the separators such
 *          as dash or space. It is a format of E.164 standard including the country code based on current network.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 * @remarks You must release @a normalized_number using free().
 * @remarks The phonenumber-utils is changed to client/server architecture for performance improvement since 3.0.
 *          phone_number_connect()/phone_number_disconnect() should be called.
 * @param[in]  number            The number
 * @param[out] normalized_number The normalized number string to be returned
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE Successful
 * @retval #PHONE_NUMBER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PHONE_NUMBER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PHONE_NUMBER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_NO_DATA Requested data does not exist
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 * @pre phone_number_connect() should be called to open a connection to the phonenumber-utils service. (Since 3.0)
 * @post phone_number_disconnect() should be called to close a connection to the phonenumber-utils service. (Since 3.0)
 */
int phone_number_get_normalized_number(const char *number, char **normalized_number);


/**
 * @partner
 * @brief Adds a blocking rule to the phone number database.
 *
 * @since_tizen 4.0
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/blocknumber.write
 *
 * @param[in] rule     The blocking rule handle
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE                Successful
 * @retval #PHONE_NUMBER_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #PHONE_NUMBER_ERROR_DB_FAILED           Database operation failure
 * @retval #PHONE_NUMBER_ERROR_PERMISSION_DENIED   Permission denied. This application does not have the privilege to call this method.
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_IPC                 Unknown IPC error
 * @retval #PHONE_NUMBER_ERROR_OUT_OF_MEMORY       Out of memory
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 * @pre phone_number_connect() should be called to open a connection to the phonenumber-utils service.
 * @post phone_number_disconnect() should be called to close a connection to the phonenumber-utils service.
 * @see phone_number_remove_blocking_rule()
 */
int phone_number_add_blocking_rule(phone_number_blocking_rule_h rule);


/**
 * @partner
 * @brief Removes a blocking rule from the phone number database.
 *
 * @since_tizen 4.0
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/blocknumber.write
 *
 * @remarks The blocking rule to remove should have been gotten from the phone number database using phone_number_get_blocking_rules().
 *
 * @param[in] rule     The blocking rule handle
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE                Successful
 * @retval #PHONE_NUMBER_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #PHONE_NUMBER_ERROR_DB_FAILED           Database operation failure
 * @retval #PHONE_NUMBER_ERROR_PERMISSION_DENIED   Permission denied. This application does not have the privilege to call this method.
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_IPC                 Unknown IPC error
 * @retval #PHONE_NUMBER_ERROR_OUT_OF_MEMORY       Out of memory
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 * @pre phone_number_connect() should be called to open a connection to the phonenumber-utils service.
 * @post phone_number_disconnect() should be called to close a connection to the phonenumber-utils service.
 * @see phone_number_add_blocking_rule()
 */
int phone_number_remove_blocking_rule(phone_number_blocking_rule_h rule);


/**
 * @partner
 * @brief Retrieves blocking rules as an array.
 *
 * @since_tizen 4.0
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/blocknumber.read
 *
 * @remarks You must release each blocking rule in the array with phone_number_blocking_rule_destroy() and @a rules using free().
 *
 * @param[in]  offset     The index from which to get results
 * @param[in]  limit      The number to limit results (value 0 is used for all blocking rules)
 * @param[out] rules      The array of blocking rule handles
 * @param[out] length     The number of blocking rule handles in the array
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE                Successful
 * @retval #PHONE_NUMBER_ERROR_OUT_OF_MEMORY       Out of memory
 * @retval #PHONE_NUMBER_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #PHONE_NUMBER_ERROR_DB_FAILED           Database operation failure
 * @retval #PHONE_NUMBER_ERROR_PERMISSION_DENIED   Permission denied. This application does not have the privilege to call this method.
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_IPC                 Unknown IPC error
 * @retval #PHONE_NUMBER_ERROR_NO_DATA             Data does not exist
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 * @pre phone_number_connect() should be called to open a connection to the phonenumber-utils service.
 * @post phone_number_disconnect() should be called to close a connection to the phonenumber-utils service.
 */
int phone_number_get_blocking_rules(int offset, int limit, phone_number_blocking_rule_h **rules, int *length);


/**
 * @partner
 * @brief Retrieves blocking rules that block a specific number.
 *
 * @since_tizen 4.0
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/blocknumber.read
 *
 * @remarks You must release each blocking rule in the array with phone_number_blocking_rule_destroy() and @a rules using free().
 *
 * @param[in]  number     The number to get blocking rules related with
 * @param[out] rules      The array of blocking rule handles
 * @param[out] length     The number of blocking rule handles in the array
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE                Successful
 * @retval #PHONE_NUMBER_ERROR_OUT_OF_MEMORY       Out of memory
 * @retval #PHONE_NUMBER_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #PHONE_NUMBER_ERROR_DB_FAILED           Database operation failure
 * @retval #PHONE_NUMBER_ERROR_PERMISSION_DENIED   Permission denied. This application does not have the privilege to call this method.
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_IPC                 Unknown IPC error
 * @retval #PHONE_NUMBER_ERROR_NO_DATA             Data does not exist
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 * @pre phone_number_connect() should be called to open a connection to the phonenumber-utils service.
 * @post phone_number_disconnect() should be called to close a connection to the phonenumber-utils service.
 */
int phone_number_get_blocking_rules_related(const char *number, phone_number_blocking_rule_h **rules, int *length);


/**
 * @partner
 * @brief Checks if a specific number is blocked or not.
 *
 * @since_tizen 4.0
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/blocknumber.read
 *
 * @param[in]  number     The number to check blocking
 * @param[out] is_blocked The blocking result
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE                Successful
 * @retval #PHONE_NUMBER_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #PHONE_NUMBER_ERROR_DB_FAILED           Database operation failure
 * @retval #PHONE_NUMBER_ERROR_PERMISSION_DENIED   Permission denied. This application does not have the privilege to call this method.
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_IPC                 Unknown IPC error
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 * @pre phone_number_connect() should be called to open a connection to the phonenumber-utils service.
 * @post phone_number_disconnect() should be called to close a connection to the phonenumber-utils service.
 */
int phone_number_check_blocking(const char *number, bool *is_blocked);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_TELEPHONY_PHONE_NUMBER_UTILS_H__ */
