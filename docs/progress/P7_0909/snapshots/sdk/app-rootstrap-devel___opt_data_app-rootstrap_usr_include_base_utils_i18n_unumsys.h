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
 * @file utils_i18n_unumsys.h
 * @version 0.1
 * @brief utils_i18n_unumsys
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UNUMSYS_MODULE UNumsys
 * @brief C API: UNumberingSystem, information about numbering systems.
 * @section CAPI_BASE_UTILS_I18N_UNUMSYS_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UNUMSYS_MODULE_OVERVIEW Overview
 * @details Defines numbering systems. A numbering system describes the scheme by which numbers are to be presented to the end user.
 *			In its simplest form, a numbering system describes the set of digit characters that are to be used to display numbers,
 *			such as Western digits, Thai digits, Arabic-Indic digits, etc.,
 *			in a positional numbering system with a specified radix (typically 10).
 *			More complicated numbering systems are algorithmic in nature, and require use of an RBNF formatter (rule based number formatter),
 *			in order to calculate the characters to be displayed for a given number.
 *			Examples of algorithmic numbering systems include Roman numerals, Chinese numerals, and Hebrew numerals.
 *			Formatting rules for many commonly used numbering systems are included in the ICU package,
 *			based on the numbering system rules defined in CLDR.
 *			Alternate numbering systems can be specified to a locale by using the numbers locale keyword.
 *			Formatting numbers is done using the @ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE API.
 *
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UNUMSYS_MODULE
 * @{
 */

/**
 * @brief	Creates an #i18n_unumsys_h using the numbering system for the specified locale.
 * @since_tizen 6.0
 * @param[in] locale	The locale for which the default numbering system should be created.
 * @param[out] unumsys	An #i18n_unumsys_h handle for the specified locale, or NULL if an error occurred.
 * @remarks The @a unumsys should be released using i18n_unumsys_destroy().
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_NOT_SUPPORTED A locale such as "en@numbers=xyz" that specifies a numbering system unknown to ICU.
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_unumsys_create(const char *locale, i18n_unumsys_h *unumsys);

/**
 * @brief	Creates an #i18n_unumsys_h using the name of one of the predefined numbering systems specified by CLDR and known to ICU,
 *			such as "latn", "arabext", or "hanidec"; the full list is returned by #i18n_unumsys_get_available_names.
 * @since_tizen 6.0
 * @remarks The @a unumsys should be released using i18n_unumsys_destroy().
 * @param[in] name	The name of the numbering system for which an #i18n_unumsys_h should be created.
 * @param[out] unumsys	An #i18n_unumsys_h handle for the specified name, or NULL if an error occurred.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_NOT_SUPPORTED A numbering system unknown to ICU.
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_unumsys_h unumsys;
 * 	i18n_error_code_e error_code;
 * 	error_code = i18n_unumsys_create("latn", &unumsys);
 * 	if(error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_unumsys_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_unumsys_destroy(unumsys);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 *
 */
int i18n_unumsys_create_by_name(const char *name, i18n_unumsys_h *unumsys);

/**
 * @brief	Destroys an #i18n_unumsys_h.
 * @details Once closed it may no longer be used.
 * @since_tizen 6.0
 * @param[in] unumsys	The #i18n_unumsys_h to destroy.
 *
 */
void i18n_unumsys_destroy(i18n_unumsys_h unumsys);

/**
 * @brief	Returns an enumeration over the names of all of the predefined numbering systems known to ICU.
 * @details The numbering system names will be in alphabetical (invariant) order.
 * @since_tizen 6.0
 * @remarks The @a uenum should be released using i18n_uenumeration_destroy().
 * @param[out] uenum	A pointer to a #i18n_uenumeration_h that must be closed with #i18n_uenumeration_destroy(), or NULL if an error occurred.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 *  int main() {
 * 	i18n_error_code_e error_code;
 * 	i18n_uenumeration_h uenum;
 * 	error_code = i18n_unumsys_get_available_names(&uenum);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_unumsys_get_available_names: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_uenumeration_destroy(uenum);
 * 	return EXIT_SUCCESS;
 *  }
 * @endcode
 */
int i18n_unumsys_get_available_names(i18n_uenumeration_h *uenum);

/**
 * @brief	Returns the name of the specified #i18n_unumsys_h.
 * @since_tizen 6.0
 * @remarks The @a name should not be released.
 * @param[in] unumsys	The #i18n_unumsys_h whose name is desired.
 * @param[out] name		A pointer to the name of the specified #i18n_unumsys_h, or NULL if the name is not one of the ICU predefined names.
 *         				The pointer is only valid for the lifetime of the #i18n_unumsys_h.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unumsys_get_name(i18n_unumsys_h unumsys, const char **name);

/**
 * @brief	Returns whether the given #i18n_unumsys_h is for an algorithmic (not purely positional) system.
 * @since_tizen 6.0
 * @param[in] unumsys			The #i18n_unumsys_h whose algorithmic status is desired.
 * @param[out] is_algorithmic	TRUE if the specified #i18n_unumsys_h is for an algorithmic system.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unumsys_is_algorithmic(i18n_unumsys_h unumsys, i18n_ubool *is_algorithmic);

/**
 * @brief	Returns the radix of the specified #i18n_unumsys_h.
 * @details	Simple positional numbering systems typically have radix 10, but might have a radix of e.g. 16 for hexadecimal.
 *			The radix is less well-defined for non-positional algorithmic systems.
 * @since_tizen 6.0
 * @param[in] unumsys	The #i18n_unumsys_h whose radix is desired.
 * @param[out] radix	The radix of the specified #i18n_unumsys_h.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unumsys_get_radix(i18n_unumsys_h unumsys, int32_t *radix);

/**
 * @brief	Gets the description string of the specified #i18n_unumsys_h.
 * @details For simple positional systems this is the ordered string of digits (with length matching the radix),
 *			e.g. "\u3007\u4E00\u4E8C\u4E09\u56DB\u4E94\u516D\u4E03\u516B\u4E5D" for "hanidec";
 *			it would be "0123456789ABCDEF" for hexadecimal.
 *
 *			For algorithmic systems this is the name of the RBNF ruleset used for formatting,
 *			e.g. "zh/SpelloutRules/%spellout-cardinal" for "hans" or "%greek-upper" for "grek".
 * @since_tizen 6.0
 * @param[in] unumsys			The #i18n_unumsys_h whose description string is desired.
 * @param[in] result			A pointer to a buffer to receive the description string.
 * @param[in] result_max_len	The maximum size of result.
 * @param[out] ulen				The total buffer size needed; if greater than result_length, the output was truncated.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unumsys_get_description(i18n_unumsys_h unumsys, i18n_uchar *result, int32_t result_max_len, int32_t *ulen);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
