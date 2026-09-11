/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @file utils_i18n_measure.h
 * @version 0.1
 * @brief utils_i18n_measure
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_MEASURE_MODULE Measure
 * @brief Measure object contains an amount of a specified unit, consisting of a number and an unit.
 * @section CAPI_BASE_UTILS_I18N_MEASURE_MODULE_HEADER Required Header
 *		 \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_MEASURE_MODULE_OVERVIEW Overview
 * @details
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_MEASURE_MODULE
 * @{
 */

/**
 * @brief Creates an object with the given numeric amount and the given unit.
 * @details After this call, the caller must not delete the given measure unit object.
 * @since_tizen 2.3.2 and 3.0
 * @remarks The created object should be released by the caller with the
 *			#i18n_measure_destroy() function.
 *
 * @param[in] formattable	A numeric object; The #i18n_formattable_is_numeric()
 *							function must return @c true for this object.
 * @param[in] measure_unit	The unit object, which must not be @c NULL
 * @param[out] measure		The created measure object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_create(const i18n_formattable_h formattable, i18n_measure_unit_h measure_unit, i18n_measure_h *measure);

/**
 * @brief Creates a polymorphic clone of the given @a clone object.
 * @since_tizen 2.3.2 and 3.0
 * @remarks The @a clone object should be released by the caller with the
 *			#i18n_measure_destroy() function.
 *
 * @param[in]  measure	The measure object to be cloned
 * @param[out] clone	The created measure object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_clone(i18n_measure_h measure, i18n_measure_h *clone);

/**
 * @brief Destroys the measure object.
 * @since_tizen 2.3.2 and 3.0
 *
 * @param[in] measure  The measure object to destroy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_measure_destroy(i18n_measure_h measure);

/**
 * @brief Gets a reference to the numeric value of the measure object.
 * @since_tizen 2.3.2 and 3.0
 * @remarks The obtained formattable object should be released by the caller
 *			with the #i18n_formattable_destroy() function.
 *
 * @param[in]  measure		The measure object
 * @param[out] formattable	The numeric value of the measure object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_get_number(i18n_measure_h measure, i18n_formattable_h *formattable);

/**
 * @brief Gets a reference to the unit of the measure object.
 * @since_tizen 2.3.2 and 3.0
 * @remarks The obtained measure unit object should be released by the caller
 *			with the #i18n_measure_unit_destroy() function.
 *
 * @param[in]  measure		 The measure object
 * @param[out] measure_unit	 The unit of the measure object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_get_unit(i18n_measure_h measure, i18n_measure_unit_h *measure_unit);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
