/*
 * Copyright (c) 2015 - 2017 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @file utils_i18n_measure_format.h
 * @version 0.1
 * @brief utils_i18n_measure_format
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE MeasureFormat
 * @brief The MeasureFormat is a formatter for Measure objects.
 * @section CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE_HEADER Required Header
 *         \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE_OVERVIEW Overview
 * @details
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE
 * @{
 */

/**
 * @deprecated Deprecated since 5.0. Use i18n_measure_format_create_from_locale_id() instead.
 * @brief Creates the measure format object using given locale.
 * @since_tizen 2.3.2
 * @remarks The created object should be released by the caller with the
 *            i18n_measure_format_destroy() function.
 *
 * @param[in]  language         The language of the locale
 * @param[in]  country          The country of the locale
 * @param[in]  width            The format width
 * @param[out] measure_format   The created measure format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_format_create(const char *language,
							   const char *country,
							   i18n_umeasure_format_width_e width,
							   i18n_measure_format_h *measure_format) TIZEN_DEPRECATED_API;

/**
 * @brief Destroys the measure format object.
 * @since_tizen 2.3.2
 *
 * @param[in] measure_format    The measure format object to destroy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_measure_format_destroy(i18n_measure_format_h measure_format);

/**
 * @brief Creates a polymorphic clone of the given @a measure_format object.
 * @since_tizen 2.3.2
 * @remarks The @a clone object should be released by the caller with the
 *          i18n_measure_format_destroy() function.
 *
 * @param[in]  measure_format   The measure format object to be cloned
 * @param[out] clone            The created measure format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_format_clone(i18n_measure_format_h measure_format, i18n_format_h *clone);

/**
 * @brief Formats an object to produce a string.
 * @since_tizen 2.3.2
 * @remarks The obtained @a append_to string is actually a concatenation of the given input string and
 *          the result of the function (appended to the string). Actually, the @a append_to
 *          buffer is being reallocated inside the function which means that the buffer is not
 *          at the same place in memory as it was on the input. Please note that the @a append_to
 *          buffer should be released by the caller with the free() function. @a append_to buffer
 *          should be allocated on the heap.
 *
 * @param[in]     measure_format    The format object
 * @param[in]     formattable       The object to format
 * @param[in,out] append_to         Input/output parameter to receive the result.
 *                                  The result is appended to the existing contents.
 * @param[in]     field_position    The #i18n_field_position_h object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_format_format(i18n_measure_format_h measure_format,
							   i18n_formattable_h formattable,
							   char **append_to,
							   i18n_field_position_h field_position);

/**
 * @brief Parses a string to produce an object.
 * @since_tizen 2.3.2
 * @remarks The obtained @a result object should be released by the caller
 *          with the i18n_formattable_destroy() function.
 *
 * @param[in]  measure_format       The format object
 * @param[in]  source               The string to be parsed into an object
 * @param[out] parse_position       The position to start parsing at. Upon return
 *                                  this parameter is set to the position after the
 *                                  last character successfully parsed. If the
 *                                  source is not parsed successfully, this parameter
 *                                  will remain unchanged.
 * @param[out] result               The formattable object to be set to the parse result.
 *                                  If parse fails, return contents are undefined.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_format_parse_object(i18n_measure_format_h measure_format,
									 const char *source,
									 i18n_parse_position_h parse_position,
									 i18n_formattable_h *result);

/**
 * @deprecated Deprecated since 5.0. Use i18n_measure_format_create_currency_format_from_locale_id() instead.
 * @brief Gets a formatter for currency amount objects in the given locale.
 * @since_tizen 2.3.2
 * @remarks The created object should be released by the caller with the
 *          i18n_measure_format_destroy() function.
 *
 * @param[in]  language         The language of the locale
 * @param[in]  country          The country of the locale
 * @param[out] measure_format   The measure format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_format_create_currency_format_from_locale(const char *language,
														   const char *country,
														   i18n_measure_format_h *measure_format) TIZEN_DEPRECATED_API;

/**
 * @brief Gets a formatter for currency amount objects in the default locale.
 * @since_tizen 2.3.2
 * @remarks The created object should be released by the caller with the
 *          i18n_measure_format_destroy() function.
 *
 * @param[out] measure_format   The measure format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_format_create_currency_format(i18n_measure_format_h *measure_format);

/**
 * @brief Formats an object to produce a string.
 * @since_tizen 4.0
 * @remarks If @a output_length is lower than @a append_to_size then formatted string is not
 *          appended to the @a append_to buffer and #I18N_ERROR_BUFFER_OVERFLOW is returned.
 *          Otherwise the obtained @a append_to string is a concatenation of the given input string and
 *          formatted string. If @a output_length is equal to @a append_to_size then output string
 *          is not NULL terminated.
 *
 * @param[in]     measure_format    The format object
 * @param[in]     formattable       The object to format
 * @param[in]     field_position    The #i18n_field_position_h object
 * @param[in]     append_to_size    The size of the @a append_to buffer.
 *                                  If <code>@a append_to_size <= 0</code> then output string is not
 *                                  appended to the @a append_to buffer and #I18N_ERROR_BUFFER_OVERFLOW
 *                                  is returned.
 * @param[in,out] append_to         The buffer to which formatted string will be appended.
 *                                  Can be NULL if <code>@a append_to_size <= 0</code>. Otherwise
 *                                  must be NULL terminated.
 * @param[out]    output_length     The length of the output string, which is the string in
 *                                  @a append_to buffer with formatted string.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW Buffer overflow
 */
int i18n_measure_format_format_buff(const i18n_measure_format_h measure_format,
									const i18n_formattable_h formattable,
									i18n_field_position_h field_position,
									int32_t append_to_size,
									i18n_uchar *append_to,
									int32_t *output_length);

/**
 * @brief Formats measure objects to produce a string.
 * @details #i18n_measure_h objects appear in the formatted string in the same order they appear in
 *          the @a measures array.
 * @since_tizen 4.0
 * @remarks If @a output_length is lower than @a append_to_size then formatted string is not
 *          appended to the @a append_to buffer and #I18N_ERROR_BUFFER_OVERFLOW is returned.
 *          Otherwise the obtained @a append_to string is a concatenation of the given input string
 *          and formatted string. If @a output_length is equal to @a append_to_size then output
 *          string is not NULL terminated.
 *
 * @param[in]     measure_format    The format object
 * @param[in]     measures          The array of the #i18n_measure_h objects to format
 * @param[in]     measures_count    The number of the #i18n_measure_h objects in the @a measures
 *                                  array
 * @param[in]     field_position    The #i18n_field_position_h object
 * @param[in]     append_to_size    The size of the @a append_to buffer.
 *                                  If <code>@a append_to_size <= 0</code> then output string is not
 *                                  appended to the @a append_to buffer and #I18N_ERROR_BUFFER_OVERFLOW
 *                                  is returned.
 * @param[in,out] append_to         The buffer to which formatted string will be appended.
 *                                  Can be NULL if <code>@a append_to_size <= 0</code>. Otherwise
 *                                  must be NULL terminated.
 * @param[out]    output_length     The length of the output string, which is the string in
									@a append_to buffer with formatted string.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW Buffer overflow
 */
int i18n_measure_format_format_measures(const i18n_measure_format_h measure_format,
										const i18n_measure_h *measures,
										int32_t measures_count,
										i18n_field_position_h field_position,
										int32_t append_to_size,
										i18n_uchar *append_to,
										int32_t *output_length);

/**
 * @brief Formats a single measure per unit.
 * @since_tizen 4.0
 * @remarks If @a output_length is lower than @a append_to_size then formatted string is not
 *          appended to the @a append_to buffer and #I18N_ERROR_BUFFER_OVERFLOW is returned.
 *          Otherwise the obtained @a append_to string is a concatenation of the given input string
 *          and formatted string. If @a output_length is equal to @a append_to_size then output
 *          string is not NULL terminated.
 *
 * @param[in]     measure_format    The format object
 * @param[in]     measure           The #i18n_measure_h object to format
 * @param[in]     measure_unit      The #i18n_measure_unit_h object
 * @param[in]     field_position    The #i18n_field_position_h object
 * @param[in]     append_to_size    The size of the @a append_to buffer.
 *                                  If <code>@a append_to_size <= 0</code> then output string is not
 *                                  appended to the @a append_to buffer and #I18N_ERROR_BUFFER_OVERFLOW
 *                                  is returned.
 * @param[in,out] append_to         The buffer to which formatted string will be appended.
 *                                  Can be NULL if <code>@a append_to_size <= 0</code>. Otherwise
 *                                  must be NULL terminated.
 * @param[out]    output_length     The length of the output string, which is the string in
									@a append_to buffer with formatted string.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW Buffer overflow
 */
int i18n_measure_format_format_measure_per_unit(const i18n_measure_format_h measure_format,
												const i18n_measure_h measure,
												const i18n_measure_unit_h measure_unit,
												i18n_field_position_h field_position,
												int32_t append_to_size,
												i18n_uchar *append_to,
												int32_t *output_length);

/**
 * @brief Gets the display name of the #i18n_measure_unit_h corresponding to the current locale and format width.
 * @since_tizen 5.0
 *
 * @param[in]   measure_format  The measure format object
 * @param[in]   measure_unit    The unit for which to get the display name
 * @param[in]   buffer_size     The size of the @a buffer.
 *                              If <code>@a buffer_size <= 0</code> then the output string is not
 *                              inserted to the @a buffer and #I18N_ERROR_BUFFER_OVERFLOW
 *                              is returned.
 * @param[out]  buffer          The buffer to which the formatted string will be inserted.
 *                              Can be @c NULL if <code>@a buffer_size <= 0</code>.
 * @param[out]  output_length   The length of the output string
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW   Buffer overflow
 */
int i18n_measure_format_get_unit_display_name(const i18n_measure_format_h measure_format,
											  const i18n_measure_unit_h measure_unit,
											  int32_t buffer_size,
											  i18n_uchar *buffer,
											  int32_t *output_length);

/**
 * @brief Creates the measure format object using given locale.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *            i18n_measure_format_destroy() function.
 *
 * @param[in]  locale_id        The locale string containing language and country code
 * @param[in]  width            The format width
 * @param[out] measure_format   The created measure format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_format_create_from_locale_id(const char *locale_id,
											  i18n_umeasure_format_width_e width,
											  i18n_measure_format_h *measure_format);

/**
 * @brief Gets a formatter for currency amount objects in the given locale.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *          i18n_measure_format_destroy() function.
 *
 * @param[in]  locale_id        The locale string containing language and country code
 * @param[out] measure_format   The measure format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_measure_format_create_currency_format_from_locale_id(const char *locale_id,
															  i18n_measure_format_h *measure_format);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
