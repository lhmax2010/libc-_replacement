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
 *
 * Copyright (C) 1999-2013, International Business Machines Corporation
 * and others. All Rights Reserved.
 */
#pragma once

#include <utils_i18n_types.h>

/**
 * @file utils_i18n_uset.h
 * @version 0.1
 * @brief utils_i18n_uset
 */


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_USET_MODULE Uset
 * @brief Uset module allows to specify a subset of character used in strings.
 *
 * @section CAPI_BASE_UTILS_I18N_USET_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_USET_MODULE_OVERVIEW Overview
 * @details Uset module allows to specify a subset of character used in strings.
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_USET_MODULE
 * @{
 */

/**
 * @brief Creates an empty #i18n_uset_h object.
 * @details Equivalent to i18n_uset_create(1, 0).
 *
 * @since_tizen 2.3.1
 * @remarks The @a set should be released using i18n_uset_destroy().
 * @param[out] set A pointer to the newly created #i18n_uset_h. The caller must call i18n_uset_destroy() on
 * it when done.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_uset_h uset = NULL;
 * 	i18n_error_code_e error_code;
 * 	error_code = i18n_uset_create_empty(&uset);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_uset_create_empty: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_uset_destroy(uset);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 * @see i18n_uset_destroy()
 */
int i18n_uset_create_empty(i18n_uset_h *set);

/**
 * @brief Creates an #i18n_uset_h object that contains the range of characters
 * start..end, inclusive.
 * @details If <code>start > end</code>
 * then an empty set is created (same as using i18n_uset_empty_create()).
 *
 * @since_tizen 2.3.1
 * @remarks The @a set should be released using i18n_uset_destroy().
 * @param[in] start First character of the range, inclusive
 * @param[in] end Last character of the range, inclusive
 * @param[out] set A pointer to the newly created #i18n_uset_h object. The caller must call i18n_uset_destroy() on
 * it when done.
 * * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uset_destroy()
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_uset_h set = NULL;
 * 	i18n_uchar32 start = 0x41;
 * 	i18n_uchar32 end = 0x50;
 *
 * 	i18n_error_code_e error_code;
 * 	error_code = i18n_uset_create(start, end, &set);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_uset_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_uset_destroy(set);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_uset_create(i18n_uchar32 start, i18n_uchar32 end, i18n_uset_h *set);

/**
 * @brief Creates a set based on a given pattern.
 * @details See the UnicodeSet class description for the syntax
 * of the pattern language.
 *
 * @since_tizen 2.3.1
 * @remarks The @a set should be released using i18n_uset_destroy().
 * @param[in] pattern A string specifying what characters are in the set
 * @param[in] pattern_length The length of the pattern, >= 0, or -1 if NULL-terminated.
 * @param[out] set A pointer to the newly created #i18n_uset_h object. The caller must call i18n_uset_destroy() on
 * it when done.
 * * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uset_destroy()
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	const char *pattern_ascii = "[:Hyphen:]";
 * 	i18n_uchar *pattern = NULL;
 * 	i18n_uset_h set = NULL;
 * 	i18n_error_code_e error_code;
 *
 * 	pattern = (i18n_uchar *)calloc(strlen(pattern_ascii), sizeof(i18n_uchar));
 * 	i18n_ustring_copy_ua(pattern, pattern_ascii);
 *
 * 	error_code = i18n_uset_create_pattern(pattern, 10, &set);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_uset_create_pattern: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	free(pattern);
 * 	i18n_uset_destroy(set);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_uset_create_pattern(const i18n_uchar *pattern, int32_t pattern_length, i18n_uset_h *set);

/**
 * @brief Creates a set based on a given pattern.
 * @details See the UnicodeSet class description for the syntax of the pattern language.
 *
 * @since_tizen 2.3.1
 * @remarks The @a set should be released using i18n_uset_destroy().
 * @param[in] pattern A string specifying what characters are in the set
 * @param[in] pattern_length The length of the pattern, >= 0, or -1 if NULL-terminated
 * @param[in] options Bitmask for options to apply to the pattern.
 * Valid options are #I18N_USET_IGNORE_SPACE and #I18N_USET_CASE_INSENSITIVE.
 * @param[out] set A pointer to the newly created #i18n_uset_h object.	The caller must call i18n_uset_destroy() on
 * it when done.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uset_destroy()
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <string.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	const char *pattern_ascii = "[:Hyphen:]";
 * 	i18n_uchar *pattern = NULL;
 * 	i18n_uset_h set = NULL;
 * 	i18n_error_code_e error_code;
 *
 * 	pattern = (i18n_uchar *)calloc(strlen(pattern_ascii) + 1, sizeof(i18n_uchar));
 * 	i18n_ustring_copy_ua(pattern, pattern_ascii);
 *
 * 	error_code = i18n_uset_create_pattern_options(pattern, strlen(pattern_ascii), I18N_USET_IGNORE_SPACE, &set);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_uset_create_pattern: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	free(pattern);
 * 	i18n_uset_destroy(set);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_uset_create_pattern_options(const i18n_uchar *pattern, int32_t pattern_length, uint32_t options, i18n_uset_h *set);

/**
 * @brief Disposes of the storage used by a #i18n_uset_h object.
 * @details This function should be called exactly once for objects returned by i18n_uset_create().
 * @since_tizen 2.3.1
 *
 * @param[in] set The object to dispose of
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uset_destroy(i18n_uset_h set);

/**
 * @brief Returns a copy of this object.
 * @details If this set is frozen, then the clone will be frozen as well.
 * Uses i18n_uset_clone_as_thawed() for a mutable clone of a frozen set.
 *
 * @since_tizen 2.3.1
 * @remarks The @a set_clone should be released using i18n_uset_destroy().
 * @param[in] set The original set. Must not be @c NULL.
 * @param[out] set_clone The newly allocated copy of the set
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uset_clone_as_thawed()
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_uset_h set = NULL;
 * 	i18n_uset_h set_clone = NULL;
 * 	i18n_uchar32 start = 0x41;
 * 	i18n_uchar32 end = 0x50;
 * 	i18n_error_code_e error_code;
 *
 * 	error_code = i18n_uset_create(start, end, &set);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_uset_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	error_code = i18n_uset_clone(set, &set_clone);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_uset_clone: %d\n", error_code);
 * 		i18n_uset_destroy(set);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_uset_destroy(set_clone);
 * 	i18n_uset_destroy(set);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_uset_clone(const i18n_uset_h set, i18n_uset_h *set_clone);

/**
 * @brief Determines whether the set has been frozen (made immutable) or not.
 * @details See the ICU4J Freezable interface for details.
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @since_tizen 2.3.1
 * @param[in] set The set. Must not be @c NULL.
 * @return true/false for whether the set has been frozen
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uset_freeze()
 * @see i18n_uset_clone_as_thawed()
 */
i18n_ubool i18n_uset_is_frozen(const i18n_uset_h set);

/**
 * @brief Freezes the set (make it immutable).
 * @details Once frozen, it cannot be unfrozen and is therefore thread-safe
 * until it is deleted.
 * See the ICU4J Freezable interface for details.
 * Freezing the set may also make some operations faster, for example
 * i18n_uset_contains() and i18n_uset_span().
 * A frozen set will not be modified. (It remains frozen.)
 *
 * @since_tizen 2.3.1
 * @param[in] set The set. Must not be @c NULL.
 * @return the same set, now frozen
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uset_is_frozen()
 * @see i18n_uset_clone_as_thawed()
 */
int i18n_uset_freeze(i18n_uset_h set);

/**
 * @brief Clones the set and make the clone mutable.
 * @details See the ICU4J Freezable interface for details.
 * @since_tizen 2.3.1
 * @remarks The @a set_copy should be released using i18n_uset_destroy().
 * @param[in] set The set. Must not be @c NULL.
 * @param[out] set_copy The mutable clone
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uset_freeze()
 * @see i18n_uset_is_frozen()
 * @see i18n_uset_clone()
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_uset_h set = NULL;
 * 	i18n_uset_h set_copy = NULL;
 * 	i18n_uchar32 start = 0x41;
 * 	i18n_uchar32 end = 0x50;
 * 	i18n_error_code_e error_code;
 *
 * 	error_code = i18n_uset_create(start, end, &set);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_uset_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	error_code = i18n_uset_clone_as_thawed(set, &set_copy);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_uset_clone_as_thawed: %d\n", error_code);
 * 		i18n_uset_destroy(set);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_uset_destroy(set_copy);
 * 	i18n_uset_destroy(set);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_uset_clone_as_thawed(const i18n_uset_h set, i18n_uset_h *set_copy);

/**
 * @brief Causes the #i18n_uset_h object to represent the range <code>start - end</code>.
 * @details If <code>start > end</code> then this #i18n_uset_h is set to an empty range.
 * A frozen set will not be modified.
 *
 * @since_tizen 2.3.1
 * @param[in] set The object to set to the given range. Must not be @c NULL.
 * @param[in] start First character in the set, inclusive
 * @param[in] end Last character in the set, inclusive
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uset_set(i18n_uset_h set, i18n_uchar32 start, i18n_uchar32 end);

/**
 * @brief Modifies the set to represent the set specified by the given
 * pattern.
 * @details See the UnicodeSet class description for the syntax of
 * the pattern language. See also the User Guide chapter about UnicodeSet.
 * <em>Empties the set passed before applying the pattern.</em>
 * A frozen set will not be modified.
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and in #i18n_error_code_e description.
 *
 * @since_tizen 2.3.1
 * @param[in] set			 The set to which the pattern is to be applied. Must not be @c NULL.
 * @param[in] pattern		 A pointer to #i18n_uchar string specifying what characters are in the set.
 *							 The character at pattern[0] must be a '['.
 * @param[in] pattern_length The length of the #i18n_uchar string, >= 0, or -1 if NULL terminated.
 * @param[in] options		 A bitmask for options to apply to the pattern.
 *							 Valid options are #I18N_USET_IGNORE_SPACE and #I18N_USET_CASE_INSENSITIVE.
 * @return					 Upon successful parse, the value is either
 *							 the index of the character after the closing ']'
 *							 of the parsed pattern.
 *							 If the status code indicates failure, then the return value
 *							 is the index of the error in the source.
 *							 If @a set is NULL, 0 is returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_uset_apply_pattern(i18n_uset_h set, const i18n_uchar *pattern, int32_t pattern_length, uint32_t options);

/**
 * @brief Modifies the set to contain those code points which have the given value
 *		for the given binary or enumerated property, as returned by
 *		i18n_uchar_get_int_property_value().
 * @details Prior contents of this set are lost.
 *		A frozen set will not be modified.
 *
 * @since_tizen 2.3.1
 * @param[in] set The object to contain the code points defined by the property. Must not be @c NULL.
 * @param[in] prop A property in the range #I18N_UCHAR_INT_START..#I18N_UCHAR_INT_LIMIT-1
 *		or #I18N_UCHAR_MASK_START..#I18N_UCHAR_MASK_LIMIT-1.
 * @param[in] value A value in the range i18n_uchar_get_int_property_min_value(prop)..
 *		i18n_uchar_get_int_property_max_value(prop), with one exception. If prop is
 *		#I18N_UCHAR_GENERAL_CATEGORY_MASK, then value should not be a #i18n_uchar_category_e, but
 *		rather a mask value produced by I18N_U_GET_GC_MASK(). This allows grouped
 *		categories such as [:L:] to be represented.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uset_apply_int_property_value(i18n_uset_h set, i18n_uchar_uproperty_e prop, int32_t value);

/**
 * @brief Modifies the set to contain those code points which have the
 *		given value for the given property.
 * @details Prior contents of this set are lost. A frozen set will not be modified.
 * @since_tizen 2.3.1
 *
 * @param[in] set The object to contain the code points defined by the given
 *		property and value alias. Must not be @c NULL.
 * @param[in] prop A string specifying a property alias, either short or long.
 *		The name is matched loosely. See PropertyAliases.txt for names and a
 *		description of loose matching. If the value string is empty, then this
 *		string is interpreted as either a General_Category value alias, a Script
 *		value alias, a binary property alias, or a special ID. Special IDs are
 *		matched loosely and correspond to the following sets:
 *
 * "ANY" = [\\u0000-\\U0010FFFF],
 * "ASCII" = [\\u0000-\\u007F],
 * "Assigned" = [:^Cn:].
 *
 * @param[in] prop_length The length of the @a prop, >= 0, or @c -1 if @c NULL.
 * @param[in] value A string specifying a value alias, either short or long.
 *		The name is matched loosely. See PropertyValueAliases.txt for names
 *		and a description of loose matching. In addition to aliases listed,
 *		numeric values and canonical combining classes may be expressed
 *		numerically, e.g., ("nv", "0.5") or ("ccc", "220"). The value string
 *		may also be empty.
 * @param[in] value_length The length of the value, >= 0, or -1 if NULL.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uset_apply_property_alias(i18n_uset_h set, const i18n_uchar *prop, int32_t prop_length, const i18n_uchar *value, int32_t value_length);

/**
 * @brief Return true if the given position, in the given pattern, appears
 *		to be the start of a UnicodeSet pattern.
 * @since_tizen 2.3.1
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @param[in] pattern A string specifying the pattern.
 * @param[in] pattern_length The length of the pattern, >= 0, or @c -1 if @c NULL.
 * @param[in] pos The given position, >= 0.
 *
 * @return @c true if the given position, in the given pattern, appears to be the start of a UnicodeSet pattern.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_ubool i18n_uset_resembles_pattern(const i18n_uchar *pattern, int32_t pattern_length, int32_t pos);

/**
 * @brief Returns a string representation of the given @a set.
 * @details If the result of calling this function is passed to an i18n_uset_create_pattern(),
 *			it will produce another set that is equal to this one.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] set					The set. Must not be @c NULL.
 * @param[in,out] result			The string to receive the rules, may be @c NULL.
 * @param[in] result_capacity		The capacity of @a result, >= 0, may be @c 0 if result is @c NULL.
 * @param[in] escape_unprintable	If true then convert unprintable character to their hex escape representations,
 *									\\uxxxx or \\Uxxxxxxxx. Unprintable characters are those other than U+000A, U+0020..U+007E.
 *
 * @return Length of string, >= 0, possibly larger than @a result_capacity. If @a set is NULL, 0 is returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_uset_create_pattern()
 */
int32_t i18n_uset_to_pattern(const i18n_uset_h set, i18n_uchar *result, int32_t result_capacity, i18n_ubool escape_unprintable);

/**
 * @brief Adds the given character to the given #i18n_uset_h.
 * @details After this call, i18n_uset_contains(set, character) will return true.
 *			A frozen set will not be modified.
 * @since_tizen 2.3.1
 *
 * @param[in] set			The object to which to add the @a character. Must not be @c NULL.
 * @param[in] character		The character to add.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_uset_contains()
 */
int i18n_uset_add(i18n_uset_h set, i18n_uchar32 character);


/**
 * @brief Adds all of the elements in the specified set to this set if
 *		they are not already present.
 * @details This operation effectively modifies this set so that its value is the <i>union</i> of the two
 *		sets. The behavior of this operation is unspecified if the specified
 *		collection is modified while the operation is in progress.
 *		A frozen set will not be modified.
 *
 * @since_tizen 2.3.1
 * @param[in] set The object to which to add the set. Must not be @c NULL.
 * @param[in] additional_set The source set whose elements are to be added to this set.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uset_add_all(i18n_uset_h set, const i18n_uset_h additional_set);

/**
 * @brief Adds the given range of characters to the given #i18n_uset_h. After this call,
 * i18n_uset_contains(set, start, end) will return true.
 * A frozen set will not be modified.
 *
 * @since_tizen 2.3.1
 * @param[in] set The object to which to add the character. Must not be @c NULL.
 * @param[in] start The first character of the range to add, inclusive
 * @param[in] end The last character of the range to add, inclusive
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uset_contains()
 */
int i18n_uset_add_range(i18n_uset_h set, i18n_uchar32 start, i18n_uchar32 end);

/**
 * @brief Adds the given string to the given #i18n_uset_h.
 * @details After this call, i18n_uset_contains_string(set, str, str_len) will return true.
 * A frozen set will not be modified.
 *
 * @since_tizen 2.3.1
 * @param[in] set The object to which to add the character. Must not be @c NULL.
 * @param[in] str The string to add.
 * @param[in] str_len The length of the string, >= 0, or -1 if NULL terminated.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uset_contains_string()
 */
int i18n_uset_add_string(i18n_uset_h set, const i18n_uchar *str, int32_t str_len);

/**
 * @brief Adds each of the characters in this string to the set. Thus "ch" => {"c", "h"}
 * @details If this set already any particular character, it has no effect on that character.
 * A frozen set will not be modified.
 *
 * @since_tizen 2.3.1
 * @param[in] set The object to which to add the character. Must not be @c NULL.
 * @param[in] str The source string.
 * @param[in] str_len The length of the string, >= 0, or -1 if NULL terminated.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uset_add_all_code_points(i18n_uset_h set, const i18n_uchar *str, int32_t str_len);

/**
 * @brief Removes the given @a character from the given #i18n_uset_h.
 * @details After this call, i18n_uset_contains(set, character) will return @c false.
 *			A frozen set will not be modified.
 * @since_tizen 2.3.1
 *
 * @param[in] set			the object from which to remove the @a character. Must not be @c NULL.
 * @param[in] character		the character to remove
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_uset_contains()
 */
int i18n_uset_remove(i18n_uset_h set, i18n_uchar32 character);

/**
 * @brief Removes the given range of characters from the given #i18n_uset_h.
 * @details After this call, i18n_uset_contains(set, start, end) will return false.
 * A frozen set will not be modified.
 *
 * @since_tizen 2.3.1
 * @param[in] set The object to which to add the character. Must not be @c NULL.
 * @param[in] start The first character of the range to remove, inclusive
 * @param[in] end The last character of the range to remove, inclusive
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uset_contains()
 */
int i18n_uset_remove_range(i18n_uset_h set, i18n_uchar32 start, i18n_uchar32 end);

/**
 * @brief Removes the given string to the given #i18n_uset_h.
 * @details After this call, i18n_uset_contains_string(set, str, str_len) will return false.
 * A frozen set will not be modified.
 *
 * @since_tizen 2.3.1
 * @param[in] set The object to which to add the character. Must not be @c NULL.
 * @param[in] str The string to remove.
 * @param[in] str_len The length of the string, >= 0, or -1 if NULL terminated.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uset_contains_string()
 */
int i18n_uset_remove_string(i18n_uset_h set, const i18n_uchar *str, int32_t str_len);

/**
 * @brief Removes from this set all of its elements that are contained in the specified set.
 * @details This operation effectively modifies this set so that its value is the <i>asymmetric set difference</i> of
 * the two sets.
 * A frozen set will not be modified.
 *
 * @since_tizen 2.3.1
 * @param[in] set The object from which the elements are to be removed. Must not be @c NULL.
 * @param[in] remove_set The object that defines which elements will be
 * removed from this set. Must not be @c NULL.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uset_remove_all(i18n_uset_h set, const i18n_uset_h remove_set);

/**
 * @brief Retains only the elements in this set that are contained in the specified range.
 * @details If <code>start > end</code> then an empty range is	retained, leaving the set empty.  This is equivalent to
 * a boolean logic AND, or a set INTERSECTION.
 * A frozen set will not be modified.
 *
 * @since_tizen 2.3.1
 * @param[in] set The object for which to retain only the specified range. Must not be @c NULL.
 * @param[in] start First character, inclusive, of range to be retained
 * to this set.
 * @param[in] end Last character, inclusive, of range to be retained
 * to this set.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uset_retain(i18n_uset_h set, i18n_uchar32 start, i18n_uchar32 end);

/**
 * @brief Retains only the elements in this set that are contained in the
 * specified set.
 * @details In other words, removes from this set all of
 * its elements that are not contained in the specified set.  This
 * operation effectively modifies this set so that its value is
 * the <i>intersection</i> of the two sets.
 * A frozen set will not be modified.
 *
 * @since_tizen 2.3.1
 * @param[in] set The object on which to perform the retain. Must not be @c NULL.
 * @param[in] retain Set that defines which elements this set will retain. Must not be @c NULL.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uset_retain_all(i18n_uset_h set, const i18n_uset_h retain);

/**
 * @brief Reallocates this objects internal structures to take up the least
 * possible space, without changing this object's value.
 *
 * @details A frozen set will not be modified.
 * @since_tizen 2.3.1
 * @param[in] set The object on which to perfrom the compact. Must not be @c NULL.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uset_compact(i18n_uset_h set);

/**
 * @brief Inverts this set.	 This operation modifies this set so that
 * its value is its complement.
 * @details This operation does not affect
 * the multicharacter strings, if any.
 * A frozen set will not be modified.
 *
 * @since_tizen 2.3.1
 * @param[in] set The set. Must not be @c NULL.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uset_complement(i18n_uset_h set);

/**
 * @brief Complements in this set all elements contained in the specified set.
 * @details Any character in the other set will be removed if it is
 * in this set, or will be added if it is not in this set.
 * A frozen set will not be modified.
 *
 * @since_tizen 2.3.1
 * @param[in] set The set with which to complement. Must not be @c NULL.
 * @param[in] complement Set that defines which elements will be xor'ed
 * from this set. Must not be @c NULL.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uset_complement_all(i18n_uset_h set, const i18n_uset_h complement);

/**
 * @brief Removes all of the elements from this set.
 * @details This set will be empty after this call returns.
 * A frozen set will not be modified.
 *
 * @since_tizen 2.3.1
 * @param[in] set The set. Must not be @c NULL.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uset_clear(i18n_uset_h set);

/**
 * @brief Closes this set over the given attribute.
 * @details For the attribute
 * #I18N_USET_CASE_INSENSITIVE, the result is to modify this set so that:
 *
 * 1. For each character or string 'a' in this set, all strings or
 * characters 'b' such that foldCase(a) == foldCase(b) are added
 * to this set.
 *
 * 2. For each string 'e' in the resulting set, if e !=
 * foldCase(e), 'e' will be removed.
 *
 * Example: [aq\\u00DF{Bc}{bC}{Fi}] => [aAqQ\\u00DF\\uFB01{ss}{bc}{fi}]
 *
 * (Here foldCase(x) refers to the operation i18n_ustring_fold_case(), and a
 * == b denotes that the contents are the same, not pointer
 * comparison.)
 *
 * A frozen set will not be modified.
 *
 * @since_tizen 2.3.1
 * @param[in] set The set. Must not be @c NULL.
 * @param[in]  attributes Bitmask for attributes to close over.
 * Currently only the #I18N_USET_CASE_INSENSITIVE bit is supported. Any undefined bits
 * are ignored.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_fold_case()
 */
int i18n_uset_destroy_over(i18n_uset_h set, int32_t attributes);

/**
 * @brief Removes all strings from this set.
 *
 * @since_tizen 2.3.1
 * @param[in] set The set. Must not be @c NULL.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uset_remove_all_strings(i18n_uset_h set);

/**
 * @brief Returns true if the given #i18n_uset_h contains no characters and no
 * strings.
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @since_tizen 2.3.1
 * @param[in] set The set. Must not be @c NULL.
 * @return true if set is empty
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_ubool i18n_uset_is_empty(const i18n_uset_h set);

/**
 * @brief Returns @c true if the given #i18n_uset_h contains the given @a character.
 * @details This function works faster with a frozen set.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] set			the set. Must not be @c NULL.
 * @param[in] character		the codepoint to check for within the @a set
 *
 * @return @c true if @a set contains the given @a character
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_ubool i18n_uset_contains(const i18n_uset_h set, i18n_uchar32 character);

/**
 * @brief Returns true if the given #i18n_uset_h contains all characters c
 * where start <= c && c <= end.
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @since_tizen 2.3.1
 * @param[in] set The set. Must not be @c NULL.
 * @param[in] start The first character of the range to test, inclusive
 * @param[in] end The last character of the range to test, inclusive
 * @return true if set contains the range
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_ubool i18n_uset_contains_range(const i18n_uset_h set, i18n_uchar32 start, i18n_uchar32 end);

/**
 * @brief Returns true if the given #i18n_uset_h contains the given string.
 * @since_tizen 2.3.1
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @param[in] set The set. Must not be @c NULL.
 * @param[in] str The string.
 * @param[in] str_len The length of the string, >= 0, or -1 if NULL terminated
 * @return true if set contains str
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_ubool i18n_uset_contains_string(const i18n_uset_h set, const i18n_uchar *str, int32_t str_len);

/**
 * @brief Returns the index of the given @a character within this @a set, where
 *		  the @a set is ordered by ascending code point.
 * @details If the @a character is not in this @a set, return @c -1.
 *			The inverse of this function is <code>i18n_uset_char_at()</code>.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] set			the set. Must not be @c NULL.
 * @param[in] character		the character to obtain the index for
 *
 * @return An index from 0..size()-1, or @c -1
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_uset_index_of(const i18n_uset_h set, i18n_uchar32 character);

/**
 * @brief Returns the character at the given index within this set, where
 * the set is ordered by ascending code point.
 * @details If the index is out of range, return (i18n_uchar32)-1.	The inverse of this function is
 * <code>i18n_uset_index_of()</code>.
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @since_tizen 2.3.1
 * @param[in] set The set. Must not be @c NULL.
 * @param[in] char_index An index from 0..size()-1 to obtain the char for
 * @return The character at the given index, or (i18n_uchar32)-1.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_uchar32 i18n_uset_char_at(const i18n_uset_h set, int32_t char_index);

/**
 * @brief Returns the number of characters and strings contained in the given #i18n_uset_h.
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @since_tizen 2.3.1
 * @param[in] set The set. Must not be @c NULL.
 * @return A non-negative integer counting the characters and strings
 * contained in set.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_uset_size(const i18n_uset_h set);

/**
 * @brief Returns the number of items in this set.
 * @details An item is either a range of characters or a single multicharacter string.
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @since_tizen 2.3.1
 * @param[in] set The set. Must not be @c NULL.
 * @return A non-negative integer counting the character ranges
 * and/or strings contained in set.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_uset_get_item_count(const i18n_uset_h set);

/**
 * @brief Returns an item of this set.
 * @details An item is either a range of characters or a single multicharacter string.
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @since_tizen 2.3.1
 * @param[in] set The set. Must not be @c NULL.
 * @param[in] item_index A non-negative integer in the range [0; i18n_uset_get_item_count(set)-1].
 * @param[in] start Pointer to variable to receive first character
 * in range, inclusive
 * @param[in] end Pointer to variable to receive last character in range,
 * inclusive
 * @param[out] str Buffer to receive the string, may be NULL
 * @param[in] str_capacity Capacity of @a str, or 0 if str is NULL
 * @return The length of the string (>= 2), or 0 if the item is a
 * range, in which case it is the range *start..*end, or -1 if
 * item_index is out of range
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_uset_get_item(const i18n_uset_h set, int32_t item_index, i18n_uchar32 *start, i18n_uchar32 *end, i18n_uchar *str, int32_t str_capacity);

/**
 * @brief Returns true if set1 contains all the characters and strings of set2. It answers the question, 'Is set1 a superset of set2?'
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @since_tizen 2.3.1
 * @param[in] set1 Set to be checked for containment. Must not be @c NULL.
 * @param[in] set2 Set to be checked for containment. Must not be @c NULL.
 * @return true if the test condition is met
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_ubool i18n_uset_contains_all(const i18n_uset_h set1, const i18n_uset_h set2);

/**
 * @brief Returns true if this set contains all the characters of the given string.
 * @details This is does not check containment of grapheme
 * clusters, like i18n_uset_contains_string().
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @since_tizen 2.3.1
 * @param[in] set Set of characters to be checked for containment. Must not be @c NULL.
 * @param[in] str String containing codepoints to be checked for containment
 * @param[in] str_len The length of the string, >= 0, or -1 if NULL terminated.
 * @return true if the test condition is met.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_ubool i18n_uset_contains_all_code_points(const i18n_uset_h set, const i18n_uchar *str, int32_t str_len);

/**
 * @brief Returns true if set1 contains none of the characters and strings of set2.
 * @details It answers the question, 'Is set1 a disjoint set of set2?'
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @since_tizen 2.3.1
 * @param[in] set1 Set to be checked for containment. Must not be @c NULL.
 * @param[in] set2 Set to be checked for containment. Must not be @c NULL.
 * @return true if the test condition is met
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_ubool i18n_uset_contains_none(const i18n_uset_h set1, const i18n_uset_h set2);

/**
 * @brief Returns true if set1 contains some of the characters and strings of set2.
 * @details It answers the question, 'Does set1 and set2 have an intersection?'
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @since_tizen 2.3.1
 * @param[in] set1 Set to be checked for containment. Must not be @c NULL.
 * @param[in] set2 Set to be checked for containment. Must not be @c NULL.
 * @return true if the test condition is met
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_ubool i18n_uset_contains_some(const i18n_uset_h set1, const i18n_uset_h set2);

/**
 * @brief Returns the length of the initial substring of the input string which
 *		  consists only of characters and strings that are contained in this set
 *		  (#I18N_USET_SPAN_CONTAINED, #I18N_USET_SPAN_SIMPLE),
 *		  or only of characters and strings that are not contained
 *		  in this set (#I18N_USET_SPAN_NOT_CONTAINED).
 * @details See #i18n_uset_span_condition_e for details.
 *			Similar to the strspn() C library function.
 *			Unpaired surrogates are treated according to contains() of their surrogate code points.
 *			This function works faster with a frozen set and with a non-negative string length argument.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] set				the set. Must not be @c NULL.
 * @param[in] str				start of the input string.
 * @param[in] length			length of the @a string; >= 0, can be @c -1 for NULL-terminated
 * @param[in] span_condition	specifies the containment condition
 *
 * @return The length of the initial substring according to the @a span_condition;
 *		   @c 0 if the start of the string does not fit the @a span_condition
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see #i18n_uset_span_condition_e
 */
int32_t i18n_uset_span(const i18n_uset_h set, const i18n_uchar *str, int32_t length, i18n_uset_span_condition_e span_condition);

/**
 * @brief Returns the start of the trailing substring of the input string which
 *		  consists only of characters and strings that are contained in this set
 *		  (#I18N_USET_SPAN_CONTAINED, #I18N_USET_SPAN_SIMPLE),
 *		  or only of characters and strings that are not contained
 *		  in this set (#I18N_USET_SPAN_NOT_CONTAINED).
 * @details See #i18n_uset_span_condition_e for details.
 *			Unpaired surrogates are treated according to contains() of their surrogate code points.
 *			This function works faster with a frozen set and with a non-negative string length argument.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] set				the set. Must not be @c NULL.
 * @param[in] str				start of the input string
 * @param[in] length			length of the @a str; >= 0, can be @c -1 for NULL-terminated
 * @param[in] span_condition	specifies the containment condition
 *
 * @return the start of the trailing substring according to the @a span_condition;
 *		   the string length if the end of the string does not fit the @a span_condition
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see #i18n_uset_span_condition_e
 */
int32_t i18n_uset_span_back(const i18n_uset_h set, const i18n_uchar *str, int32_t length, i18n_uset_span_condition_e span_condition);

/**
 * @brief Returns the length of the initial substring of the input string which
 *		  consists only of characters and strings that are contained in this set
 *		  (#I18N_USET_SPAN_CONTAINED, #I18N_USET_SPAN_SIMPLE),
 *		  or only of characters and strings that are not contained
 *		  in this set (#I18N_USET_SPAN_NOT_CONTAINED).
 * @details See #i18n_uset_span_condition_e for details.
 *			Similar to the strspn() C library function.
 *			Malformed byte sequences are treated according to contains(0xfffd).
 *			This function works faster with a frozen set and with a non-negative string length argument.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] set				the set. Must not be @c NULL.
 * @param[in] str				start of the string (UTF-8)
 * @param[in] length			length of the string; >= 0, can be @c -1 for NULL-terminated
 * @param[in] span_condition	specifies the containment condition
 *
 * @return the length of the initial substring according to the @a span_condition;
 *		   @c 0 if the start of the string does not fit the @a span_condition
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see #i18n_uset_span_condition_e
 */
int32_t i18n_uset_span_utf8(const i18n_uset_h set, const char *str, int32_t length, i18n_uset_span_condition_e span_condition);

/**
 * @brief Returns the start of the trailing substring of the input string which
 *		  consists only of characters and strings that are contained in this set
 *		  (#I18N_USET_SPAN_CONTAINED, #I18N_USET_SPAN_SIMPLE),
 *		  or only of characters and strings that are not contained
 *		  in this set (#I18N_USET_SPAN_NOT_CONTAINED).
 * @details See #i18n_uset_span_condition_e for details.
 *			Malformed byte sequences are treated according to contains(0xfffd).
 *			This function works faster with a frozen set and with a non-negative string length argument.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] set				the set. Must not be @c NULL.
 * @param[in] str				start of the string (UTF-8)
 * @param[in] length			length of the string; >= 0, can be @c -1 for NULL-terminated
 * @param[in] span_condition	specifies the containment condition
 *
 * @return the start of the trailing substring according to the @a span_condition;
 *		   the string length if the end of the string does not fit the @a span_condition
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see #i18n_uset_span_condition_e
 */
int32_t i18n_uset_span_back_utf8(const i18n_uset_h set, const char *str, int32_t length, i18n_uset_span_condition_e span_condition);

/**
 * @brief Returns true if set1 contains all of the characters and strings
 * of set2, and vice versa. It answers the question, 'Is set1 equal to set2?'
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @since_tizen 2.3.1
 * @param[in] set1 Set to be checked for containment. Must not be @c NULL.
 * @param[in] set2 Set to be checked for containment. Must not be @c NULL.
 * @return true if the test condition is met
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_ubool i18n_uset_equals(const i18n_uset_h set1, const i18n_uset_h set2);

/*********************************************************************
 * Serialized set API
 *********************************************************************/

/**
 * @brief Serializes this @a set into an array of 16-bit integers.
 * @details Serialization (currently) only records the characters in the set;
 *			multicharacter strings are ignored.
 *
 *			The array has following format (each line is one 16-bit integer):
 *
 *			length	   = (n+2*m) | (m!=0?0x8000:0)
 *			bmpLength  = n;
 *			present if m!=0
 *			bmp[0]
 *			bmp[1]
 *			...
 *			bmp[n-1]
 *			supp-high[0]
 *			supp-low[0]
 *			supp-high[1]
 *			supp-low[1]
 *			...
 *			supp-high[m-1]
 *			supp-low[m-1]
 *
 *			The array starts with a header. After the header are n bmp
 *			code points, then m supplementary code points. Either n or m
 *			or both may be zero. n+2*m is always <= 0x7FFF.
 *
 *			If there are no supplementary characters (if m==0) then the
 *			header is one 16-bit integer, 'length', with value n.
 *
 *			If there are supplementary characters (if m!=0) then the header
 *			is two 16-bit integers. The first, 'length', has value
 *			(n+2*m)|0x8000.	 The second, 'bmpLength', has value n.
 *
 *			After the header the code points are stored in ascending order.
 *			Supplementary code points are stored as most significant 16
 *			bits followed by least significant 16 bits.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] set				the set. Must not be @c NULL.
 * @param[out] dest				pointer to buffer of @a dest_capacity 16-bit integers
 *								May be NULL only if @a dest_capacity is zero.
 * @param[in] dest_capacity		size of @a dest, or zero
 *								Must not be negative.
 *
 * @return the total length of the serialized format, including
 *		   the header, that is, <code>n+2*m+(m != 0 ? 2 : 1)</code>,
 *		   or @c 0 on error other than #I18N_ERROR_BUFFER_OVERFLOW.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @exception #I18N_ERROR_INDEX_OUTOFBOUNDS If <code>n+2*m > 0x7FFF</code>
 * @exception #I18N_ERROR_BUFFER_OVERFLOW If <code>n+2*m+(m != 0 ? 2 : 1) > dest_capacity</code>.
 */
int32_t i18n_uset_serialize(const i18n_uset_h set, uint16_t *dest, int32_t dest_capacity);

/**
 * @brief Given a serialized array, fill in the given serialized set object.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] src			pointer to start of array. Must not be @c NULL.
 * @param[in] src_length	length of @a src array, >= 0.
 * @param[out] fill_set		the serialized set to be filled
 *
 * @return @c true if the given array is valid, otherwise @c false
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see #i18n_userialized_set_s
 */
i18n_ubool i18n_uset_get_serialized_set(const uint16_t *src, int32_t src_length, i18n_userialized_set_s *fill_set);

/**
 * @brief Sets the #i18n_userialized_set_s to contain the given @a character (and nothing else).
 * @since_tizen 2.3.1
 *
 * @param[in] character		the code point to set
 * @param[out] fill_set		the serialized set to be filled
 *
 * @return Error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see #i18n_userialized_set_s
 */
int i18n_uset_set_serialized_to_one(i18n_uchar32 character, i18n_userialized_set_s *fill_set);

/**
 * @brief Returns @c true if the given #i18n_userialized_set_s contains the given @a character.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] set			the serialized set. Must not be @c NULL.
 * @param[in] character		the code point to check for within the @a set
 *
 * @return @c true if @a set contains @a character
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #i18n_userialized_set_s
 */
i18n_ubool i18n_uset_serialized_contains(const i18n_userialized_set_s *set, i18n_uchar32 character);

/**
 * @brief Returns the number of disjoint ranges of characters contained in
 * the given serialized set.
 * @details Ignores any strings contained in the set.
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @since_tizen 2.3.1
 * @param[in] set The serialized set. Must not be @c NULL.
 * @return A non-negative integer counting the character ranges contained in set
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #i18n_userialized_set_s
 */
int32_t i18n_uset_get_serialized_range_count(const i18n_userialized_set_s *set);

/**
 * @brief Returns a range of characters contained in the given serialized set.
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @since_tizen 2.3.1
 * @param[in] set The serialized set. Must not be @c NULL.
 * @param[in] range_index A non-negative integer in the range 0..
 * i18n_uset_get_serialized_range_count(set)-1
 * @param[out] p_start Pointer to variable to receive first character
 * in range, inclusive
 * @param[out] p_end Pointer to variable to receive last character in range,
 * inclusive
 * @return true if range_index is valid, otherwise false
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #i18n_userialized_set_s
 */
i18n_ubool i18n_uset_get_serialized_range(const i18n_userialized_set_s *set, int32_t range_index, i18n_uchar32 *p_start, i18n_uchar32 *p_end);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
