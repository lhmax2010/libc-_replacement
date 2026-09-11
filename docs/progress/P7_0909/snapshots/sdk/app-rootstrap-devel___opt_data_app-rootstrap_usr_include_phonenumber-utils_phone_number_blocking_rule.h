/*
 * Copyright (c) 2015 - 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_TELEPHONY_PHONE_NUMBER_UTILS_RULE_H__
#define __TIZEN_TELEPHONY_PHONE_NUMBER_UTILS_RULE_H__

#include <phone_number_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file phone_number_blocking_rule.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_PHONE_NUMBER_UTILS_MODULE
 *
 * @{
 */

/**
 * @brief Creates a blocking rule.
 *
 * @since_tizen 4.0
 *
 * @remarks You must release @a rule using phone_number_blocking_rule_destroy().
 *
 * @param[out] rule      The blocking rule handle
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE                Successful
 * @retval #PHONE_NUMBER_ERROR_OUT_OF_MEMORY       Out of memory
 * @retval #PHONE_NUMBER_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 *
 * @pre     phone_number_connect() should be called to initialize.
 *
 * @see phone_number_blocking_rule_destroy()
 */
int phone_number_blocking_rule_create(phone_number_blocking_rule_h *rule);


/**
 * @brief Destroys a blocking rule and releases all its resources.
 *
 * @since_tizen 4.0
 *
 * @param[in] rule          The blocking rule handle
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE                 Successful
 * @retval #PHONE_NUMBER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 *
 * @see phone_number_blocking_rule_create()
 */
int phone_number_blocking_rule_destroy(phone_number_blocking_rule_h rule);


/**
 * @brief Gets a blocked number from a rule.
 *
 * @since_tizen 4.0
 *
 * @remarks You must release @a number using free().
 *
 * @param[in]   rule        The blocking rule handle
 * @param[out]  number       The blocked number
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE                Successful
 * @retval #PHONE_NUMBER_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 *
 * @see phone_number_blocking_rule_set_number()
 */
int phone_number_blocking_rule_get_number(phone_number_blocking_rule_h rule, char **number);


/**
 * @brief Gets a rule's match type.
 *
 * @since_tizen 4.0
 *
 * @param[in]   rule         The blocking rule handle
 * @param[out]  match_type        The match type of blocked number
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE                Successful
 * @retval #PHONE_NUMBER_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 *
 * @see phone_number_blocking_rule_set_match_type()
 */
int phone_number_blocking_rule_get_match_type(phone_number_blocking_rule_h rule, phone_number_blocking_rule_match_type_e *match_type);


/**
 * @brief Sets a blocked number to a rule.
 *
 * @since_tizen 4.0
 *
 * @param[in]   rule          The blocking rule handle
 * @param[in]  number       The blocked number
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE                Successful
 * @retval #PHONE_NUMBER_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 *
 * @see phone_number_blocking_rule_get_number()
 */
int phone_number_blocking_rule_set_number(phone_number_blocking_rule_h rule, const char *number);


/**
 * @brief Sets a rule's match type.
 *
 * @since_tizen 4.0
 *
 * @param[in] rule          The blocking rule handle
 * @param[in] match_type        The match type of blocked number
 *
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #PHONE_NUMBER_ERROR_NONE                  Successful
 * @retval #PHONE_NUMBER_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #PHONE_NUMBER_ERROR_NOT_SUPPORTED Not supported
 * @retval #PHONE_NUMBER_ERROR_SYSTEM Internal error
 *
 * @see phone_number_blocking_rule_get_match_type()
 */
int phone_number_blocking_rule_set_match_type(phone_number_blocking_rule_h rule, phone_number_blocking_rule_match_type_e match_type);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /*__TIZEN_TELEPHONY_PHONE_NUMBER_UTILS_RULE_H__*/

