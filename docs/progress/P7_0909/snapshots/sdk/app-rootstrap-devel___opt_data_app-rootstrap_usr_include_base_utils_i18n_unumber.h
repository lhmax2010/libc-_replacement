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
 * @file utils_i18n_unumber.h
 * @version 0.1
 * @brief utils_i18n_unumber
 */

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UNUMBER_MODULE Unumber
 * @brief The Unumber module helps you format and parse numbers for any locale.
 *
 * @section CAPI_BASE_UTILS_I18N_UNUMBER_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UNUMBER_MODULE_OVERVIEW Overview
 * @details The Unumber module helps you format and parse numbers for any locale.
 * Your code can be completely independent of the locale conventions for decimal
 * points, thousands-separators, or even the particular decimal digits used,
 * or whether the number format is even decimal. There are different number format
 * styles like decimal, currency, percent and spellout.
 *
 * @section CAPI_BASE_UTILS_I18N_UNUMBER_MODULE_SAMPLE_CODE_1 Sample Code 1
 * @brief Gets a currency symbol according to a given locale.
 * @code
	int buf_len;
	i18n_uchar u_buffer[64];
	char a_buffer[64];
	i18n_unumber_format_h num_format;

	// creates and returns a new unumber_format
	i18n_unumber_create(I18N_UNUMBER_CURRENCY, NULL, -1, "en_US", NULL, &num_format);

	// gets a symbol associated with i18n_unumber_format
	i18n_unumber_get_symbol(num_format, I18N_UNUMBER_CURRENCY_SYMBOL, u_buffer, 64, &buf_len);

	i18n_ustring_copy_au(a_buffer, u_buffer);
	// en_US currency symbol: $
	dlog_print(DLOG_INFO, LOG_TAG, "en_US currency symbol: %s \n", a_buffer);

	// destroys i18n_unumber_format
	i18n_unumber_destroy(num_format);
 * @endcode
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UNUMBER_MODULE
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates and returns a new unumber_format_h for formatting and parsing numbers.
 * @details A unumber_format_style_e may be used to format numbers by calling i18n_unumber_create().
 * The caller must call i18n_unumber_destroy() when done to release resources used by this object.
 * @since_tizen 2.3
 * @remarks Must release @a num_format using i18n_unumber_destroy().
 *
 * @param[in] style The type of number format to create: one of
 * #I18N_UNUMBER_DECIMAL,
 * #I18N_UNUMBER_CURRENCY,
 * #I18N_UNUMBER_PERCENT,
 * #I18N_UNUMBER_SCIENTIFIC,
 * #I18N_UNUMBER_CURRENCY_ISO,
 * #I18N_UNUMBER_CURRENCY_PLURAL,
 * #I18N_UNUMBER_SPELLOUT,
 * #I18N_UNUMBER_ORDINAL,
 * #I18N_UNUMBER_DURATION,
 * #I18N_UNUMBER_NUMBERING_SYSTEM,
 * #I18N_UNUMBER_PATTERN_RULEBASED,
 * or #I18N_UNUMBER_DEFAULT \n
 * If #I18N_UNUMBER_PATTERN_DECIMAL or #I18N_UNUMBER_PATTERN_RULEBASED is passed then the number format is created using the given pattern, which must conform
 * to the syntax described in DecimalFormat or RuleBasedNumberFormat, respectively.
 * @param[in] pattern		A pattern specifying the format to use \n This parameter is ignored unless the style is #I18N_UNUMBER_PATTERN_DECIMAL or #I18N_UNUMBER_PATTERN_RULEBASED.
 * @param[in] pattern_len	The number of characters in the pattern, otherwise @c -1 if NULL-terminated\n
 *							This parameter is ignored unless the style is I18N_UNUMBER_PATTERN_*.
 * @param[in] locale		A locale identifier to use to determine formatting
 *							and parsing conventions, otherwise @c NULL to use the default locale.
 * @param[in] parse_err		A pointer to a #i18n_uparse_error_s structure to receive the
 *							details of any parsing errors, otherwise @c NULL if no parsing error details are desired.
 * @param[out] num_format	A pointer to a newly created #i18n_unumber_format_h, otherwise @c NULL if an error occurs.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_unumber_create(i18n_unumber_format_style_e style, const i18n_uchar *pattern, int32_t pattern_len, const char *locale,
						i18n_uparse_error_s *parse_err, i18n_unumber_format_h *num_format);

/**
 * @brief Destroys an #i18n_unumber_format_h.
 * @details Once destroyed, an #i18n_unumber_format may no longer be used.
 * @since_tizen 2.3
 *
 * @param[in] fmt The formatter to destroy
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_unumber_destroy(i18n_unumber_format_h fmt);

/**
 * @brief Gets a symbol associated with an #i18n_unumber_format_h.
 * @details An #i18n_unumber_format_h uses symbols to represent the special locale-dependent characters in a number,
 * for example the percent sign. This API is not supported for rule-based formatters.
 * @since_tizen 2.3
 *
 * @param[in] fmt The formatter to query.
 * @param[in] symbol The unumber_format_symbol_e constant for the symbol to get
 * @param[out] buffer The string buffer that will receive the symbol string\n
 * If it is @c NULL, then only the length of the symbol is returned.
 * @param[in] size The size of the string buffer
 * @param[out] len_symbol The length of the symbol\n
 * The buffer is not modified if <code>length &gt;= size</code>
 *
 * @retval #I18N_ERROR_NONE Successful.
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_unumber_get_symbol(const i18n_unumber_format_h fmt, i18n_unumber_format_symbol_e symbol, i18n_uchar *buffer, int32_t size, int32_t *len_symbol);


/* Newly Added APIs */


/**
 * @brief Creates a copy of an #i18n_unumber_format_h.
 * @details This function performs a deep copy.
 * @remarks Error codes are described in #i18n_error_code_e description.\n
 * The @a fmt_clone should be released using i18n_unumber_destroy().
 * @since_tizen 2.3.1
 *
 * @param[in] fmt	The format to copy.
 * @param[out] fmt_clone A pointer to clone of @a fmt.
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful.
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_error_code_e error_code;
 * 	i18n_unumber_format_h fmt = NULL;
 * 	i18n_unumber_format_h fmt_clone = NULL;
 *
 * 	error_code = i18n_unumber_create(I18N_UNUMBER_DECIMAL, NULL, -1, I18N_ULOCALE_US, NULL, &fmt);
 * 	if(error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_unumber_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	error_code = i18n_unumber_clone(fmt, &fmt_clone);
 * 	if(error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_unumber_clone: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_unumber_destroy(fmt_clone);
 * 	i18n_unumber_destroy(fmt);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_unumber_clone(const i18n_unumber_format_h fmt, i18n_unumber_format_h *fmt_clone);

/**
* @brief Formats an integer using a i18n_unumber_format_h.
* @details The integer will be formatted according to the i18n_unumber_format_h's locale.
* @since_tizen 2.3.1
*
* @param[in] fmt		   The formatter to use.
* @param[in] number		   The number to format.
* @param[out] result	   A pointer to a buffer to receive the NULL-terminated formatted number.
*						   If the formatted number fits into dest but cannot be NULL-terminated (length == resultLength)
*						   then the error code is set to #I18N_WARNING_STRING_NOT_TERMINATED.
*						   If the formatted number doesn't fit into result then the error code is set to #I18N_ERROR_BUFFER_OVERFLOW.
* @param[in] result_length The maximum size of result.
* @param[in] pos		   A pointer to a i18n_ufield_position. On input, position->field is read.
*						   On output, position->beginIndex and position->endIndex indicate the beginning and ending indices of field number position->field, if such a field exists.
*						   This parameter may be NULL, in which case no field
* @param[out] status	   A pointer to an i18n_error_code_e to receive any errors
*
* @return The total buffer size needed; if greater than result_length, the output was truncated.
*/
int32_t i18n_unumber_format(const i18n_unumber_format_h fmt, int32_t number, i18n_uchar *result, int32_t result_length, i18n_ufield_position_s *pos, i18n_error_code_e *status);

/**
 * @brief Formats an int64 using an #i18n_unumber_format_h.
 * @details The int64 will be formatted according to the #i18n_unumber_format_h's locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt				The formatter to use.
 * @param[in] number			The number to format.
 * @param[out] result			A pointer to a buffer to receive the NULL-terminated formatted number.
 *								If the formatted number fits into @a result but cannot be NULL-terminated (<code>length == result_length</code>)
 *								then the error code is set to #I18N_WARNING_STRING_NOT_TERMINATED.
 *								If the formatted number doesn't fit into result then the error code is set to #I18N_ERROR_BUFFER_OVERFLOW.
 * @param[in] result_length		The maximum size of @a result.
 * @param[in,out] pos			An #i18n_ufield_position_h handle. On input, position->field is read.
 *								On output, position->beginIndex and position->endIndex indicate the beginning and ending indices of field number position->field, if such a field exists.
 *								This parameter may be NULL, in which case no field
 *
 * @return The total buffer size needed; if greater than @a result_length, the output was truncated.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @exception #I18N_WARNING_STRING_NOT_TERMINATED If the formatted number fits into @a result but cannot be NULL-terminated (<code>length == result_length</code>)
 * @exception #I18N_ERROR_BUFFER_OVERFLOW If the formatted number doesn't fit into the @a result buffer
 */
int32_t i18n_unumber_format_int64(const i18n_unumber_format_h fmt, int64_t number, i18n_uchar *result, int32_t result_length, i18n_ufield_position_h pos);

/**
 * @brief Formats a double using an #i18n_unumber_format_h.
 * @details The double will be formatted according to the #i18n_unumber_format_h's locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt				The formatter to use.
 * @param[in] number			The number to format.
 * @param[out] result			A pointer to a buffer to receive the NULL-terminated formatted number.
 *								If the formatted number fits into @a result but cannot be NULL-terminated (<code>length == result_length</code>)
 *								then the error code is set to #I18N_WARNING_STRING_NOT_TERMINATED.
 *								If the formatted number doesn't fit into result then the error code is set to #I18N_ERROR_BUFFER_OVERFLOW.
 * @param[in] result_length		The maximum size of @a result.
 * @param[in,out] pos			An #i18n_ufield_position_h handle. On input, position->field is read.
 *								On output, position->beginIndex and position->endIndex indicate the beginning
 *								and ending indices of field number position->field, if such a field exists.
 *								This parameter may be NULL, in which case no field
 *
 * @return The total buffer size needed; if greater than @a result_length, the output was truncated.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @exception #I18N_WARNING_STRING_NOT_TERMINATED If the formatted number fits into @a result but cannot be NULL-terminated (<code>length == result_length</code>)
 * @exception #I18N_ERROR_BUFFER_OVERFLOW If the formatted number doesn't fit into the @a result buffer
 */
int32_t i18n_unumber_format_double(const i18n_unumber_format_h fmt, double number, i18n_uchar *result, int32_t result_length, i18n_ufield_position_h pos);

/**
 * @brief Formats a decimal number using an #i18n_unumber_format_h.
 * @details The number will be formatted according to the #i18n_unumber_format_h's locale. The syntax of the input number
 *		is a "numeric string" as defined in the Decimal Arithmetic Specification, available at http://speleotrove.com/decimal
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt				The formatter to use.
 * @param[in] number			The number to format.
 * @param[in] length			The length of the input @a number, or -1 if the input is NULL-terminated.
 * @param[out] result			A pointer to a buffer to receive the NULL-terminated formatted number.
 *								If the formatted number fits into @a result but cannot be NULL-terminated (<code>length == result_length</code>) then
 *								the error code is set to #I18N_WARNING_STRING_NOT_TERMINATED.
 *								If the formatted number doesn't fit into result then the error code is set to
 *								#I18N_ERROR_BUFFER_OVERFLOW.
 * @param[in] result_length		The maximum size of @a result.
 * @param[in,out] pos			An #i18n_ufield_position_h handle. On input, position->field is read.
 *								On output, position->beginIndex and position->endIndex indicate the beginning and ending
 *								indices of field number position->field, if such a field exists.
 *								This parameter may be NULL, in which case it is ignored.
 *
 * @return The total buffer size needed; if greater than @a result_length, the output was truncated.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @exception #I18N_WARNING_STRING_NOT_TERMINATED If the formatted number fits into @a result but cannot be NULL-terminated (<code>length == result_length</code>)
 * @exception #I18N_ERROR_BUFFER_OVERFLOW If the formatted number doesn't fit into the @a result buffer
 */
int32_t i18n_unumber_format_decimal(const i18n_unumber_format_h fmt, const char *number, int32_t length, i18n_uchar *result, int32_t result_length, i18n_ufield_position_h pos);

/**
 * @brief Formats a double currency amount using an #i18n_unumber_format_h.
 * @details The double will be formatted according to the #i18n_unumber_format_h's locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt				The formatter to use.
 * @param[in] number			The number to format.
 * @param[in] currency			The 3-letter NULL-terminated ISO 4217 currency code.
 * @param[out] result			A pointer to a buffer to receive the NULL-terminated formatted number.
 *								If the formatted number fits into @a result but cannot be NULL-terminated (<code>length == result_length</code>)
 *								then the error code is set to #I18N_WARNING_STRING_NOT_TERMINATED.
 *								If the formatted number doesn't fit into result then the error code is set to
 *								#I18N_ERROR_BUFFER_OVERFLOW.
 * @param[in] result_length		The maximum size of @a result.
 * @param[in,out] pos			An #i18n_ufield_position_h handle. On input, position->field is read.
 *								On output, position->beginIndex and position->endIndex indicate the beginning and ending indices
 *								of field number position->field, if such a field exists.
 *								This parameter may be NULL, in which case it is ignored.
 *
 * @return The total buffer size needed; if greater than @a result_length, the output was truncated.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @exception #I18N_WARNING_STRING_NOT_TERMINATED If the formatted number fits into @a result but cannot be NULL-terminated (<code>length == result_length</code>)
 * @exception #I18N_ERROR_BUFFER_OVERFLOW If the formatted number doesn't fit into the @a result buffer
 */
int32_t i18n_unumber_format_double_currency(const i18n_unumber_format_h fmt, double number, i18n_uchar *currency, i18n_uchar *result, int32_t result_length, i18n_ufield_position_h pos);

/**
 * @brief Parses a string into an integer using an #i18n_unumber_format_h.
 * @details The string will be parsed according to the #i18n_unumber_format_h's locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt			 The formatter to use.
 * @param[in] text			 The text to parse.
 * @param[in] text_length	 The length of @a text, or -1 if NULL-terminated.
 * @param[in,out] parse_pos	 If not NULL, on input a pointer to an integer specifying the offset at which to begin parsing.
 *							 If not NULL, on output the offset at which parsing ended.
 *
 * @return The value of the parsed integer
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_unumber_parse(const i18n_unumber_format_h fmt, const i18n_uchar *text, int32_t text_length, int32_t *parse_pos);

/**
 * @brief Parses a string into an int64 using an #i18n_unumber_format_h.
 * @details The string will be parsed according to the #i18n_unumber_format_h's locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt			 The formatter to use.
 * @param[in] text			 The text to parse.
 * @param[in] text_length	 The length of @a text, or -1 if NULL-terminated.
 * @param[in,out] parse_pos	 If not NULL, on input a pointer to an integer specifying the offset at which to begin parsing.
 *							 If not NULL, on output the offset at which parsing ended.
 *
 * @return The value of the parsed integer
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int64_t i18n_unumber_parse_int64(const i18n_unumber_format_h fmt, const i18n_uchar *text, int32_t text_length, int32_t *parse_pos);

/**
 * @brief Parses a string into a double using an #i18n_unumber_format_h.
 * @details The string will be parsed according to the #i18n_unumber_format_h's locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt			 The formatter to use.
 * @param[in] text			 The text to parse.
 * @param[out] text_length	 The length of @a text, or -1 if NULL-terminated.
 * @param[in,out] parse_pos	 If not NULL, on input a pointer to an integer specifying the offset at which to begin parsing.
 *							 If not NULL, on output the offset at which parsing ended.
 *
 * @return The value of the parsed double
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
double i18n_unumber_parse_double(const i18n_unumber_format_h fmt, const i18n_uchar *text, int32_t text_length, int32_t *parse_pos);

/**
 * @brief Parses a number from a string into an unformatted numeric string using an #i18n_unumber_format_h.
 * @details The input string will be parsed according to the #i18n_unumber_format_h's locale.
 *			The syntax of the output is a "numeric string" as defined in the Decimal Arithmetic Specification, available at
 *			http://speleotrove.com/decimal
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt				 The formatter to use.
 * @param[in] text				 The text to parse.
 * @param[in] text_length		 The length of @a text, or -1 if NULL-terminated.
 * @param[in,out] parse_pos		 If not NULL, on input a pointer to an integer specifying the offset at which to begin parsing.
 *								 If not NULL, on output the offset at which parsing ended.
 * @param[out] out_buf			 A (char *) buffer to receive the parsed number as a string.
 *								 The output string will be NULL-terminated if there is sufficient space.
 * @param[out] out_buf_length	 The size of the output buffer. May be zero, in which case the @a out_buf pointer may be NULL,
 *								 and the function will return the size of the output string.
 *
 * @return The length of the output string, not including any terminating NULL.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_unumber_parse_decimal(const i18n_unumber_format_h fmt, const i18n_uchar *text, int32_t text_length, int32_t *parse_pos, char *out_buf, int32_t out_buf_length);

/**
 * @brief Parses a string into a double and a currency using an #i18n_unumber_format_h.
 * @details The string will be parsed according to the #i18n_unumber_format_h's locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt			 The formatter to use.
 * @param[in] text			 The text to parse.
 * @param[in] text_length	 The length of @a text, or -1 if NULL-terminated.
 * @param[in,out] parse_pos	 A pointer to an offset index into text at which to begin parsing. On output, @a parse_pos will
 *							 point after the last parsed character. This parameter may be NULL, in which case parsing begins
 *							 at offset 0.
 *							 If not NULL, on output the offset at which parsing ended.
 * @param[out] currency		 A pointer to the buffer to receive the parsed NULL- terminated currency.
 *							 This buffer must have a capacity of at least 4 #i18n_uchar characters.
 *
 * @return The parsed double
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
double i18n_unumber_parse_double_currency(const i18n_unumber_format_h fmt, const i18n_uchar *text, int32_t text_length, int32_t *parse_pos, i18n_uchar *currency);

/**
 * @brief Sets the pattern used by an #i18n_unumber_format_h.
 * @details This can only be used on a DecimalFormat, other formats return #I18N_ERROR_NOT_SUPPORTED in the status.
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] format			The formatter to set.
 * @param[in] localized			true if the pattern is localized, false otherwise.
 * @param[in] pattern			The new pattern
 * @param[in] pattern_length	The length of @a pattern, or -1 if NULL-terminated.
 * @param[out] parse_error		 A pointer to #i18n_uparse_error_s to receive information about errors occurred during parsing,
 *								 or NULL if no parse error information is desired.
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful.
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_unumber_apply_pattern(i18n_unumber_format_h format, i18n_ubool localized, const i18n_uchar *pattern, int32_t pattern_length, i18n_uparse_error_s *parse_error);

/**
 * @brief Gets a locale for which decimal formatting patterns are available.
 * @details An #i18n_unumber_format_h in a locale returned by this function will perform the correct formatting and parsing for the locale.
 *			The results of this call are not valid for rule-based number formats.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_index	The index of the desired locale.
 *
 * @return A locale for which number formatting patterns are available, or 0 if none.
 *
 * @exception #I18N_ERROR_NONE Successful
 */
const char *i18n_unumber_get_available(int32_t locale_index);

/**
 * @brief Determines how many locales have decimal formatting patterns available.
 * @details The results of this call are not valid for rule-based number formats.
 *			This function is useful for determining the loop ending condition for calls to i18n_unumber_get_available().
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @return The number of locales for which decimal formatting patterns are available.
 *
 * @exception #I18N_ERROR_NONE Successful
 */
int32_t i18n_unumber_count_available(void);

/**
 * @brief Gets a numeric attribute associated with an #i18n_unumber_format_h.
 * @details An example of a numeric attribute is the number of integer digits a formatter will produce.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt		The formatter to query.
 * @param[in] attr		The attribute to query; one of #I18N_UNUMBER_PARSE_INT_ONLY, #I18N_UNUMBER_GROUPING_USED,
 *						#I18N_UNUMBER_DECIMAL_ALWAYS_SHOWN, #I18N_UNUMBER_MAX_INTEGER_DIGITS, #I18N_UNUMBER_MIN_INTEGER_DIGITS,
 *						#I18N_UNUMBER_INTEGER_DIGITS, #I18N_UNUMBER_MAX_FRACTION_DIGITS, #I18N_UNUMBER_MIN_FRACTION_DIGITS,
 *						#I18N_UNUMBER_FRACTION_DIGITS, #I18N_UNUMBER_MULTIPLIER, #I18N_UNUMBER_GROUPING_SIZE,
 *						#I18N_UNUMBER_ROUNDING_MODE, #I18N_UNUMBER_FORMAT_WIDTH, #I18N_UNUMBER_PADDING_POSITION,
 *						#I18N_UNUMBER_SECONDARY_GROUPING_SIZE, #I18N_UNUM_SCALE.
 *
 * @return The value of @a attr or @c -1 if the given attribute is not supported.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_unumber_get_attribute(const i18n_unumber_format_h fmt, i18n_unumber_format_attribute_e attr);

/**
 * @brief Sets a numeric attribute associated with an #i18n_unumber_format_h.
 * @details An example of a numeric attribute is the number of integer digits a formatter will produce.
 *			If the formatter does not understand the attribute, the call is ignored. Rule-based formatters only understand
 *			the lenient-parse attribute. The #I18N_UNUMBER_ROUNDING_INCREMENT attribute is not supported.
 *
 *			Note that the value for the #I18N_UNUMBER_PADDING_POSITION attribute should be one of the #i18n_unumber_pad_position_e
 *			enumeration values.
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt		 The formatter to set.
 * @param[in] attr		 The attribute to query; one of #I18N_UNUMBER_PARSE_INT_ONLY, #I18N_UNUMBER_GROUPING_USED,
 *						 #I18N_UNUMBER_DECIMAL_ALWAYS_SHOWN, #I18N_UNUMBER_MAX_INTEGER_DIGITS, #I18N_UNUMBER_MIN_INTEGER_DIGITS,
 *						 #I18N_UNUMBER_INTEGER_DIGITS, #I18N_UNUMBER_MAX_FRACTION_DIGITS, #I18N_UNUMBER_MIN_FRACTION_DIGITS,
 *						 #I18N_UNUMBER_FRACTION_DIGITS, #I18N_UNUMBER_MULTIPLIER, #I18N_UNUMBER_GROUPING_SIZE,
 *						 #I18N_UNUMBER_ROUNDING_MODE, #I18N_UNUMBER_FORMAT_WIDTH, #I18N_UNUMBER_PADDING_POSITION,
 *						 #I18N_UNUMBER_SECONDARY_GROUPING_SIZE, #I18N_UNUMBER_SIGNIFICANT_DIGITS_USED, #I18N_UNUMBER_MIN_SIGNIFICANT_DIGITS,
 *						 #I18N_UNUMBER_MAX_SIGNIFICANT_DIGITS, #I18N_UNUMBER_LENIENT_PARSE, #I18N_UNUM_SCALE,
 *						 #I18N_UNUM_FORMAT_FAIL_IF_MORE_THAN_MAX_DIGITS, #I18N_UNUM_PARSE_NO_EXPONENT.
 * @param[in] new_value	 The new value of @a attr.
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful.
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_NOT_SUPPORTED Not supported
 */
int i18n_unumber_set_attribute(i18n_unumber_format_h fmt, i18n_unumber_format_attribute_e attr, int32_t new_value);

/**
 * @brief Gets a numeric attribute associated with an #i18n_unumber_format_h.
 * @details An example of a numeric attribute is the number of integer digits a formatter will produce.
 *			If the formatter does not understand the attribute, -1 is returned.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt	 The formatter to query.
 * @param[in] attr	 The attribute to query; only #I18N_UNUMBER_ROUNDING_INCREMENT is supported.
 *
 * @return The value of @a attr.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
double i18n_unumber_get_double_attribute(const i18n_unumber_format_h fmt, i18n_unumber_format_attribute_e attr);

/**
 * @brief Sets a numeric attribute associated with an #i18n_unumber_format_h.
 * @details An example of a numeric attribute is the number of integer digits a formatter will produce.
 *			If the formatter does not understand the attribute, this call is ignored.
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt		 The formatter to set.
 * @param[in] attr		 The attribute to query; Only #I18N_UNUMBER_ROUNDING_INCREMENT is supported.
 * @param[in] new_value	 The new value of @a attr.
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful.
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_unumber_set_double_attribute(i18n_unumber_format_h fmt, i18n_unumber_format_attribute_e attr, double new_value);

/**
 * @brief Gets a text attribute associated with an #i18n_unumber_format_h.
 * @details An example of a text attribute is the suffix for positive numbers. If the formatter does not understand the attribute,
 *			#I18N_ERROR_NOT_SUPPORTED error code is set.
 *			Rule-based formatters only understand #I18N_UNUMBER_DEFAULT_RULESET and #I18N_UNUMBER_PUBLIC_RULESETS.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt				The formatter to query.
 * @param[in] tag				The attribute to query; one of #I18N_UNUMBER_POSITIVE_PREFIX, #I18N_UNUMBER_POSITIVE_SUFFIX,
 *								#I18N_UNUMBER_NEGATIVE_PREFIX, #I18N_UNUMBER_NEGATIVE_SUFFIX, #I18N_UNUMBER_PADDING_CHARACTER,
 *								#I18N_UNUMBER_CURRENCY_CODE, #I18N_UNUMBER_DEFAULT_RULESET, or #I18N_UNUMBER_PUBLIC_RULESETS.
 * @param[out] result			A pointer to a buffer to receive the attribute.
 * @param[in] result_length		The maximum size of @a result.
 *
 * @return The total buffer size needed; if greater than @a result_length, the output was truncated.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_unumber_get_text_attribute(const i18n_unumber_format_h fmt, i18n_unumber_format_text_attribute_e tag, i18n_uchar *result, int32_t result_length);

/**
 * @brief Sets a text attribute associated with an #i18n_unumber_format_h.
 * @details An example of a text attribute is the suffix for positive numbers. Rule-based formatters only understand
 *			#I18N_UNUMBER_DEFAULT_RULESET. The #I18N_UNUMBER_PUBLIC_RULESETS tag is not supported.
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt				 The formatter to set.
 * @param[in] tag				 The attribute to set; one of #I18N_UNUMBER_POSITIVE_PREFIX, #I18N_UNUMBER_POSITIVE_SUFFIX,
 *								 #I18N_UNUMBER_NEGATIVE_PREFIX, #I18N_UNUMBER_NEGATIVE_SUFFIX, #I18N_UNUMBER_PADDING_CHARACTER,
 *								 #I18N_UNUMBER_CURRENCY_CODE, #I18N_UNUMBER_DEFAULT_RULESET.
 * @param[in] new_value			 The new value of @a tag.
 * @param[in] new_value_length	 The length of new_value, or -1 if NULL-terminated.
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful.
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_unumber_set_text_attribute(const i18n_unumber_format_h fmt, i18n_unumber_format_text_attribute_e tag, const i18n_uchar *new_value, int32_t new_value_length);

/**
 * @brief Extracts the pattern from an #i18n_unumber_format_h.
 * @details The pattern will follow the DecimalFormat pattern syntax.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt					 The formatter to query.
 * @param[in] is_pattern_localized	 true if the pattern should be localized, false otherwise.
 *									 This is ignored if the formatter is a rule-based formatter.
 * @param[out] result				 A pointer to a buffer to receive the pattern.
 * @param[in] result_length			 The maximum size of @a result.
 *
 * @return The total buffer size needed; if greater than @a result_length, the output was truncated.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_unumber_to_pattern(const i18n_unumber_format_h fmt, i18n_ubool is_pattern_localized, i18n_uchar *result, int32_t result_length);

/**
 * @brief Sets a symbol associated with an #i18n_unumber_format_h.
 * @details An #i18n_unumber_format_h uses symbols to represent the special locale-dependent characters in a number, for example the percent sign.
 *			This API is not supported for rule-based formatters.
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt		 The formatter to set.
 * @param[in] symbol	 The #i18n_unumber_format_symbol_e constant for the symbol to set
 * @param[in] value		 The string to set the symbol to
 * @param[in] length	 The length of the input string, or -1 for a zero-terminated string
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful.
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_unumber_set_symbol(i18n_unumber_format_h fmt, i18n_unumber_format_symbol_e symbol, const i18n_uchar *value, int32_t length);

/**
 * @brief Gets the locale for this number format object.
 * @details You can choose between valid and actual locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] fmt		 The formatter to get the locale from.
 * @param[in] type		 Type of the locale we're looking for (valid or actual)
 *
 * @return The locale name
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
const char *i18n_unumber_get_locale_by_type(const i18n_unumber_format_h fmt, i18n_ulocale_data_locale_type_e type);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif
