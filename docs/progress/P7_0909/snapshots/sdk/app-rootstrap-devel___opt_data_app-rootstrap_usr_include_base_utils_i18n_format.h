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
 * @file utils_i18n_format.h
 * @version 0.1
 * @brief utils_i18n_format
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_FORMAT_MODULE Format
 * @brief The Format module represents the base module for all formats.
 * @section CAPI_BASE_UTILS_I18N_FORMAT_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_FORMAT_MODULE_OVERVIEW Overview
 * @details The Format module specifies the protocol for classes which convert other objects or values,\n
 * such as numeric values and dates, and their string representations.\n
 * In some cases these representations may be localized or contain localized characters or strings.\n
 * Please note, that there is no function for creating an #i18n_format_h object as this module uses a mechanism
 * similar to inheritance, known in object-oriented languages. All of the functions that take the
 * #i18n_format_h handle as a parameter can actually take the handle to the more specific format object related
 * to the corresponding derived classes.
 * Please also note that at this point the only derived class that is supported by the base-utils i18n module is the
 * ICU's MeasureFormat class, which is why it is possible to use handles of type #i18n_measure_format_h
 * in place of the parameters of type #i18n_format_h.
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_FORMAT_MODULE
 * @{
 */

/**
 * @brief Destroys the format object.
 * @since_tizen 2.3.2
 *
 * @param[in] format  The format object to destroy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_format_destroy(i18n_format_h format);

/**
 * @brief Creates a polymorphic clone of the given @a format object.
 * @since_tizen 2.3.2
 * @remarks The cloned object should be released by the caller with the
 *			i18n_format_destroy() function.
 *
 * @param[in]  format  The format object to be cloned
 * @param[out] clone   The clone of the given @a format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_format_clone(i18n_format_h format, i18n_format_h *clone);

/**
 * @brief Formats an object to produce a string.
 * @since_tizen 2.3.2
 * @remarks The @a append_to parameter should be released by the caller with the
 *			free() function.
 *
 * @param[in] format		 The format object
 * @param[in] formattable	 The object to format
 * @param[out] append_to	 An input/output parameter to receive the result.
 *							 The result is appended to the existing contents.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_format_format(i18n_format_h format, i18n_formattable_h formattable, char **append_to);

/**
 * @brief Formats an object to produce a string.
 * @since_tizen 2.3.2
 * @remarks The @a append_to parameter should be released by the caller with the
 *			free() function.
 *
 * @param[in] format			  The format object
 * @param[in] formattable		  The object to format
 * @param[out] append_to		  Input/output parameter to receive the result.
 *								  The result is appended to the existing contents.
 * @param[out] field_position	  On input: an alignment field, if desired.
 *								  On output: the offsets of the alignment field.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_format_format_with_field_position(i18n_format_h format, i18n_formattable_h formattable, char **append_to, i18n_field_position_h field_position);

/**
 * @brief Parses a string to produce an object.
 * @since_tizen 2.3.2
 * @remarks The obtained @a result formattable object should be released by the caller with the
 *			i18n_formattable_destroy() function.
 *
 * @param[in] format   The format object
 * @param[in] source   The string to be parsed into an object
 * @param[out] result  The formattable object to be set to the parse result.
 *					   If parse fails, return contents are undefined.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_format_parse_object(i18n_format_h format, const char *source, i18n_formattable_h *result);

/**
 * @brief Parses a string to produce an object.
 * @since_tizen 2.3.2
 * @remarks The obtained @a result formattable object should be released by the caller with the
 *			i18n_formattable_destroy() function.
 *
 * @param[in] format			  The format object
 * @param[in] source			  The string to be parsed into an object
 * @param[out] parse_position	  The position to start parsing at. Upon return this parameter is set
 *								  to the position after the last character successfully parsed. If the source
 *								  is not parsed successfully, this parameter will remain unchanged.
 * @param[out] result			  The formattable object to be set to the parse result.
 *								  If parse fails, return contents are undefined.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_format_parse_object_with_parse_position(i18n_format_h format, const char *source, i18n_parse_position_h parse_position, i18n_formattable_h *result);

/**
 * @deprecated Deprecated since 5.0. Use i18n_format_get_locale_id() instead.
 * @brief Gets the locale for the given format object.
 * @details You can choose between valid and actual locale.
 * @since_tizen 2.3.2
 * @remarks Both @a language and @a country should be released by the caller with the free() function.
 *
 * @param[in] format	 The format object
 * @param[in] type		 The type of the locale we're looking for (valid or actual)
 * @param[out] language	 The obtained locale's ISO-639 language code
 * @param[out] country	 The obtained locale's ISO-3166 country code
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_format_get_locale(i18n_format_h format,
						   i18n_ulocale_data_locale_type_e type,
						   char **language, char **country) TIZEN_DEPRECATED_API;

/**
 * @brief Gets the locale for the given format object.
 * @details You can choose between valid and actual locale.
 * @since_tizen 5.0
 * @remarks @a locale_id should be released by the caller with the free() function.
 *
 * @param[in]  format		The format object
 * @param[in]  type			The type of the locale we're looking for (valid or actual)
 * @param[out] locale_id	The locale identifier
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_format_get_locale_id(i18n_format_h format,
							  i18n_ulocale_data_locale_type_e type, char **locale_id);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif
