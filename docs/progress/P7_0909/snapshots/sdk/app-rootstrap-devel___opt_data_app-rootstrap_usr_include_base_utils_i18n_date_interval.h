/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <utils_i18n_types.h>

/**
 * @file utils_i18n_date_interval.h
 * @version 0.1
 * @brief utils_i18n_date_interval
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_DATE_INTERVAL_MODULE Date Interval
 * @brief Date Interval represents a date interval.
 * @section CAPI_BASE_UTILS_I18N_DATE_INTERVAL_MODULE_HEADER Required Header
 *          \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_DATE_INTERVAL_MODULE_OVERVIEW Overview
 * @details The module represents a date interval.
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_DATE_INTERVAL_MODULE
 * @{
 */

/**
 * @brief Creates a date interval object.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *                      i18n_date_interval_destroy() function.
 *
 * @param[in]    from_date      The start date of the date interval
 * @param[in]    to_date        The end date of the date interval
 * @param[out]   date_interval  The created date interval object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_date_interval_create(i18n_udate from_date, i18n_udate to_date, i18n_date_interval_h *date_interval);

/**
 * @brief Destroys the date interval object.
 * @since_tizen 5.0
 *
 * @param[in] date_interval     The date interval to be destroyed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_date_interval_destroy(i18n_date_interval_h date_interval);

/**
 * @brief Clones the date interval object.
 * @since_tizen 5.0
 * @remarks The cloned object should be released by the caller with the
 *                      i18n_date_interval_destroy() function.
 *
 * @param[in]    date_interval    The date interval to be cloned
 * @param[out]   clone            The clone of the given @a date_interval object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_date_interval_clone(i18n_date_interval_h date_interval, i18n_date_interval_h *clone);

/**
 * @brief Gets the 'from date' from a given date interval object.
 * @since_tizen 5.0
 *
 * @param[in]    date_interval    The date interval object
 * @param[out]   from_date        The 'from date' value
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_date_interval_get_from_date(i18n_date_interval_h date_interval, i18n_udate *from_date);

/**
 * @brief Gets the 'to date' from a given date interval object.
 * @since_tizen 5.0
 *
 * @param[in]    date_interval    The date interval object
 * @param[out]   to_date          The 'to date' value
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_date_interval_get_to_date(i18n_date_interval_h date_interval, i18n_udate *to_date);

/**
 * @brief Compares two date interval objects.
 * @since_tizen 5.0
 *
 * @param[in]    date_interval_1    The first date interval object
 * @param[in]    date_interval_2    The second date interval object
 * @param[out]   equals            A flag set to @c true if given objects are equal,
 *                                 or @c false otherwise.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_date_interval_equals(i18n_date_interval_h date_interval_1, i18n_date_interval_h date_interval_2,
                              i18n_ubool *equals);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
