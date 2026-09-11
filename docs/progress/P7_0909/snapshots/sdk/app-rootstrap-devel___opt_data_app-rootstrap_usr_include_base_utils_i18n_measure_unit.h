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
 * @file utils_i18n_measure_unit.h
 * @version 0.1
 * @brief utils_i18n_measure_unit
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE MeasureUnit
 * @brief The MeasureUnit unit such as length, mass, volume, currency, etc. coupled with a numeric amount to produce a Measure.
 * @section CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE_OVERVIEW Overview
 * @details
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE
 * @{
 */

/**
 * @brief Creates the default measure unit object.
 * @since_tizen 2.3.2
 * @remarks The created object should be released by the caller with the
 *			#i18n_measure_unit_destroy() function.
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create(i18n_measure_unit_h *measure_unit);

/**
 * @brief Creates a polymorphic clone of the given @a clone object.
 * @since_tizen 2.3.2
 * @remarks The created object should be released by the caller with the
 *			#i18n_measure_unit_destroy() function.
 *
 * @param[in]  measure_unit	 The measure unit object to be cloned.
 * @param[out] clone		 The clone of the given @a measure_unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_clone(i18n_measure_unit_h measure_unit, i18n_measure_unit_h *clone);

/**
 * @brief Destroys the measure unit object
 * @since_tizen 2.3.2
 *
 * @param[in] measure_unit	The measure unit object to destroy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_measure_unit_destroy(i18n_measure_unit_h measure_unit);

/**
 * @brief Destroys the given array of the measure unit objects
 * @since_tizen 2.3.2
 *
 * @param[in] array		  The array of measure unit objects to destroy
 * @param[in] array_size  The capacity of the @a dest_array
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_measure_unit_array_destroy(i18n_measure_unit_h *array, int32_t array_size);

/**
 * @brief Gets the measure unit object type.
 * @since_tizen 2.3.2
 * @remarks The obtained type should be released by the caller with the free() function.
 *
 * @param[in]  measure_unit	 The measure unit object
 * @param[out] type			 The measure unit object type
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_get_type(i18n_measure_unit_h measure_unit, char **type);

/**
 * @brief Gets the measure unit object sub type.
 * @since_tizen 2.3.2
 * @remarks The obtained subtype should be released by the caller with the free() function.
 *
 * @param[in]  measure_unit	 The measure unit object
 * @param[out] subtype		 The measure unit object subtype
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_get_subtype(i18n_measure_unit_h measure_unit, char **subtype);

/**
 * @brief Gets the available units.
 * @details The function creates the array with the results.
 *			If there are more units available than the given @a max_count
 *			then the returned array is truncated to the first @a max_count units
 *			and the returned error code is #I18N_ERROR_BUFFER_OVERFLOW.
 *
 * @since_tizen 2.3.2
 * @remarks The obtained array of measure unit objects should be released by the caller
 *			with the #i18n_measure_unit_array_destroy() function.
 *
 * @param[in]  max_count	  The maximal number of units that will be obtained
 * @param[out] out_array	  The output array
 * @param[out] available	  The actual number of available units
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #I18N_ERROR_BUFFER_OVERFLOW Buffer overflow
 */
int i18n_measure_unit_get_available(int32_t max_count, i18n_measure_unit_h **out_array, int32_t *available);

/**
 * @brief Gets the available units for a specific type.
 * @details The function creates the array with the results.
 *			If there are more units available than the given @a max_count
 *			then the returned array is truncated to the first @a max_count units
 *			and the returned error code is #I18N_ERROR_BUFFER_OVERFLOW.
 *
 * @since_tizen 2.3.2
 * @remarks The obtained array of measure unit objects should be released by the caller
 *			with the #i18n_measure_unit_array_destroy() function.
 *
 * @param[in]  max_count	  The maximal number of units that will be obtained
 * @param[in]  type			  The type of the obtained units
 * @param[out] out_array	  The output array
 * @param[out] available	  The actual number of available units
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #I18N_ERROR_BUFFER_OVERFLOW Buffer overflow
 */
int i18n_measure_unit_get_available_with_type(int32_t max_count, const char *type,
											  i18n_measure_unit_h **out_array, int32_t *available);

/**
 * @brief Invokes the given callback function for every available measure unit type.
 * @since_tizen 2.3.2
 *
 * @param[in] cb		 The callback function invoked for every available measure unit type
 * @param[in] user_data	 The user data passed to the callback function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_measure_unit_foreach_available_type(i18n_measure_unit_types_cb cb, void *user_data);

/**
 * @brief Gets unit of acceleration: g-force.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_g_force(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of acceleration: meter-per-second-squared.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_meter_per_second_squared(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of angle: arc-minute.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_arc_minute(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of angle: arc-second.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_arc_second(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of angle: degree.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_degree(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of angle: radian.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_radian(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of area: acre.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_acre(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of area: hectare.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_hectare(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit area: square-centimeter.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_square_centimeter(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit area: square-foot.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_square_foot(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit area: square-inch.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_square_inch(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit area: square-kilometer.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_square_kilometer(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit area: square-meter.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_square_meter(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit area: square-mile.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_square_mile(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit area: square-yard.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_square_yard(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of consumption: liter-per-kilometer.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_liter_per_kilometer(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of consumption: mile-per-gallon.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_mile_per_gallon(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of digital: bit.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_bit(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of digital: byte.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_byte(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of digital: gigabit.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_gigabit(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of digital: gigabyte.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_gigabyte(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of digital: kilobit.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_kilobit(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of digital: kilobyte.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_kilobyte(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of digital: megabit.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_megabit(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of digital: megabyte.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_megabyte(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of digital: terabit.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_terabit(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of digital: terabyte.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_terabyte(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of duration: day.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_day(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of duration: hour.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_hour(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of duration: microsecond.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_microsecond(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of duration: millisecond.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_millisecond(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of duration: minute.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_minute(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of duration: month.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_month(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of duration: nanosecond.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_nanosecond(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of duration: second.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_second(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of duration: week.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_week(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of duration: year.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_year(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of electric: ampere.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_ampere(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of electric: milliampere.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_milliampere(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of electric: ohm.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_ohm(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of electric: volt.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_volt(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of energy: calorie.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_calorie(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of energy: foodcalorie.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_foodcalorie(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of energy: joule.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_joule(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of energy: kilocalorie.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_kilocalorie(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of energy: kilojoule.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_kilojoule(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of energy: kilowatt-hour.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_kilowatt_hour(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of frequency: gigahertz.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_gigahertz(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of frequency: hertz.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_hertz(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of frequency: kilohertz.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_kilohertz(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of frequency: megahertz.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_megahertz(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: astronomical-unit.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_astronomical_unit(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: centimeter.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_centimeter(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: decimeter.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_decimeter(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: fathom.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_fathom(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: foot.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_foot(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: furlong.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_furlong(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: inch.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_inch(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: kilometer.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_kilometer(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: light-year.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_light_year(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: meter.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_meter(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: micrometer.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_micrometer(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: mile.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_mile(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: millimeter.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_millimeter(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: nanometer.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_nanometer(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: nautical-mile.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_nautical_mile(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: parsec.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_parsec(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: picometer.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_picometer(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of length: yard.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_yard(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of illuminance: lux.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_lux(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of mass: carat.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_carat(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of mass: gram.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_gram(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of mass: kilogram.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_kilogram(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of mass: metric-ton.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_metric_ton(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of mass: microgram.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_microgram(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of mass: milligram.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_milligram(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of mass: ounce.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_ounce(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of mass: ounce-troy.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_ounce_troy(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of mass: pound.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_pound(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of mass: stone.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_stone(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of mass: ton.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_ton(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of power: gigawatt.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_gigawatt(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of power: horsepower.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_horsepower(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of power: kilowatt.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_kilowatt(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of power: megawatt.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_megawatt(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of power: milliwatt.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_milliwatt(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of power: watt.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_watt(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of pressure: hectopascal.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_hectopascal(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of pressure: inch-hg.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_inch_hg(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of pressure: millibar.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_millibar(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of pressure: millimeter-of-mercury.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_millimeter_of_mercury(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of pressure: pound-per-square-inch.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_pound_per_square_inch(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of proportion: karat.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_karat(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of speed: kilometer-per-hour.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_kilometer_per_hour(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of speed: meter-per-second.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_meter_per_second(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of speed: mile-per-hour.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_mile_per_hour(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of temperature: celsius.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_celsius(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of temperature: fahrenheit.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_fahrenheit(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of temperature: kelvin.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_kelvin(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: acre-foot.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_acre_foot(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: bushel.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_bushel(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: centiliter.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_centiliter(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: cubic-centimeter.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_cubic_centimeter(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: cubic-foot.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_cubic_foot(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: cubic-inch.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_cubic_inch(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: cubic-kilometer.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_cubic_kilometer(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: cubic-meter.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_cubic_meter(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: cubic-mile.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_cubic_mile(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: cubic-yard.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_cubic_yard(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: cup.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_cup(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: deciliter.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_deciliter(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: fluid-ounce.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_fluid_ounce(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: gallon.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_gallon(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: hectoliter.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_hectoliter(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: liter.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_liter(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: megaliter.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_megaliter(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: milliliter.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_milliliter(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: pint.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_pint(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: quart.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_quart(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: tablespoon.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_tablespoon(i18n_measure_unit_h *measure_unit);

/**
 * @brief Gets unit of volume: teaspoon.
 * @since_tizen 2.3.2
 * @remarks The returned @a measure_unit should be freed by the caller
 *			with i18n_measure_unit_destroy().
 *
 * @param[out] measure_unit	 The created measure unit object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_unit_create_teaspoon(i18n_measure_unit_h *measure_unit);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
