/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @file utils_i18n_uscript.h
 * @version 0.1
 * @brief utils_i18n_uscript
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_USCRIPT_MODULE UScript
 * @brief Abstract Unicode Text API.
 * @section CAPI_BASE_UTILS_I18N_USCRIPT_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_USCRIPT_MODULE_OVERVIEW Overview
 * @details Unicode Script Information.
 *
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_USCRIPT_MODULE
 * @{
 */

/**
 * @brief Gets the script codes associated with the specified language.
 * @details  The language is described using either locale, ISO 15924 name or ISO 15924 abbreviation.
 *            Example: If "Malayam" or "Mlym" is given, the expected result is #I18N_USCRIPT_MALAYALAM.
 *            Note: To search by short or long script alias only,
 *                use #i18n_uchar_get_property_value_enum() instead.
 *                That does a fast lookup with no access of the locale data.
 * @since_tizen 6.0
 * @remarks @a codes must be allocated before calling the function.
 *			If the required capacity is greater than the capacity of @a codes, then the @a length is set to the required capacity and #I18N_ERROR_BUFFER_OVERFLOW is returned.
 * @param[in]     language    The language, for which the script codes are to be retrieved
 * @param[in,out] codes       The array of codes associated with the specified language
 * @param[in]     capacity    Capacity of the @a codes array
 * @param[out]    length      The number of items written to the array, or the required capacity if the array's capacity is insufficient
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #I18N_ERROR_BUFFER_OVERFLOW The supplied array @a codes is of insufficient capacity
 *
 */
int i18n_uscript_get_codes(const char* language, i18n_uscript_code_e* codes, int32_t capacity, int32_t *length);

/**
 * @brief	Gets the script name for the specified script code.
 * @details	Returns the long unicode script name, if there is one. Otherwise returns the 4-letter ISO 15924 script code
 *			Example: If #I18N_USCRIPT_MALAYALAM is given, the expected output is "Malayam".
 * @since_tizen 6.0
 * @param[in] script_code	Uscript code enum
 * @return	long script name as given in PropertyValueAliases.txt, or the 4-letter code, or NULL if #i18n_uscript_code_e is invalid
 *
 */
const char* i18n_uscript_get_name(i18n_uscript_code_e script_code);

/**
 * @brief	Gets the short script name for the specified script code.
 * @details	Returns the 4-letter ISO 15924 script code, which is the same as the short Unicode script name if Unicode has names for the script.
 *			Example: If #I18N_USCRIPT_MALAYALAM is given, the expected output is "Mlym".
 * @since_tizen 6.0
 * @param[in] script_code	Uscript code enum
 * @return	short script name (4-letter code), or NULL if @a script_code is invalid
 *
 */
const char* i18n_uscript_get_short_name(i18n_uscript_code_e script_code);

/**
 * @brief	Gets the script code associated with the given @a codepoint.
 * @details	If the specified @a codepoint is invalid, the script code returned is equal to 0.
 *			Example: If 0x0D02 is given, the expected output is #I18N_USCRIPT_MALAYALAM.
 * @since_tizen 6.0
 * @param[in]	codepoint	#i18n_uchar32 @a codepoint
 * @param[out]	script_code The code of the script, that the specified @a codepoint belongs to.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_uscript_get_script(i18n_uchar32 codepoint, i18n_uscript_code_e *script_code);

/**
 * @brief	Gets a value indicating whether the Script Extensions of the specified @a codepoint contain the specified script.
 * @details	If @a codepoint does not have explicit Script Extensions, then this tests whether @a codepoint has the Script property value @a script_code.
 * 			Some characters are commonly used in multiple scripts. For more information, see UAX #24: http://www.unicode.org/reports/tr24/.
 * @since_tizen 6.0
 * @param[in]	codepoint	Code point
 * @param[in]	script_code	Script code
 * @return	TRUE if @a script_code is in Script Extensions (@a codepoint)
 *
 */
i18n_ubool i18n_uscript_has_script(i18n_uchar32 codepoint, i18n_uscript_code_e script_code);

/**
 * @brief    Gets the Script Extensions for the specified @a codepoint.
 * @details
 *            - If @a codepoint does have Script Extensions, then the Script property value
 *              (normally Common or Inherited) is not included.
 *            - If @a codepoint does not have Script Extensions, then the one Script code is written to the output array.
 *            - If @a codepoint is not a valid code point, then the one #I18N_USCRIPT_UNKNOWN code is written.
 *
 *             Some characters are commonly used in multiple scripts.
 *             For more information, see UAX #24: http://www.unicode.org/reports/tr24/.
 *
 * @since_tizen 6.0
 * @remarks @a scripts must be allocated before calling the function.
 *          If the required capacity is greater than the capacity of @a scripts, then the @a length is set to the required capacity and #I18N_ERROR_BUFFER_OVERFLOW is returned.
 * @param[in]      codepoint            Code point
 * @param[in,out]  scripts              The array of Script Extensions for the specified @a codepoint
 * @param[in]      capacity             Capacity of the @a scripts array
 * @param[out]     length               The number of items written to the array, or the required capacity if the array's capacity is insufficient
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #I18N_ERROR_BUFFER_OVERFLOW The supplied array @a scripts is of insufficient capacity
 *
 */
int i18n_uscript_get_script_extensions(i18n_uchar32 codepoint, i18n_uscript_code_e *scripts, int32_t capacity, int32_t *length);

/**
 * @brief    Gets the script sample character string.
 * @details    This string normally consists of one code point but might be longer. The string is empty if the script is not encoded.
 * @since_tizen 6.0
 * @remarks @a sample must be allocated before calling the function.
 *          If the required capacity is greater than the capacity of @a sample, then the @a length is set to the required capacity and #I18N_ERROR_BUFFER_OVERFLOW is returned.
 * @param[in]     script               Script code
 * @param[in,out] sample               The sample string for the specified @a codepoint
 * @param[in]     capacity             The number of #i18n_uchar characters that @a sample can hold
 * @param[out]    length               The number of #i18n_uchar characters written to the string, or the required capacity if the string's capacity is insufficient
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #I18N_ERROR_BUFFER_OVERFLOW The supplied array @a sample is of insufficient capacity
 *
 */
int i18n_uscript_get_sample_string(i18n_uscript_code_e script, i18n_uchar *sample, int32_t capacity, int32_t *length);


/**
 * @brief	Returns the script usage according to UAX #31 Unicode Identifier and Pattern Syntax.
 * @details	Returns #I18N_USCRIPT_USAGE_NOT_ENCODED if the script is not encoded in Unicode.
 * @since_tizen 6.0
 * @param[in]	script	Script code
 * @return	script usage
 *
 */
i18n_uscript_usage_e i18n_uscript_get_usage(i18n_uscript_code_e script);

/**
 * @brief	Gets a value indicating whether the script is written right-to-left.
 * @details	For example, Arab and Hebr.
 * @since_tizen 6.0
 * @param[in]	script	Script code
 * @return	TRUE if the script is right-to-left
 *
 */
i18n_ubool i18n_uscript_is_right_to_left(i18n_uscript_code_e script);

/**
 * @brief	Gets a value indicating whether the script allows line breaks between letters (excluding hyphenation).
 * @details	Such a script typically requires dictionary-based line breaking. For example, Hani and Thai.
 * @since_tizen 6.0
 * @param[in]	script	Script code
 * @return	TRUE if the script allows line breaks between letters
 *
 */
i18n_ubool i18n_uscript_breaks_between_letters(i18n_uscript_code_e script);

/**
 * @brief	Gets a value indicating whether the script case distinctions in modern usage are customary.
 * @details	For example, Latn and Cyrl.
 * @since_tizen 6.0
 * @param[in]	script	Script code
 * @return	TRUE if the script is cased
 *
 */
i18n_ubool i18n_uscript_is_cased(i18n_uscript_code_e script);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
