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
 * Copyright (C) 1998-2012, International Business Machines
 * Corporation and others.	All Rights Reserved.
 */
#pragma once

#include <utils_i18n_types.h>

/**
 * @file utils_i18n_ustring.h
 * @version 0.1
 * @brief utils_i18n_ustring
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_USTRING_MODULE Ustring
 * @brief The Ustring module provides general unicode string handling information.
 *
 * @section CAPI_BASE_UTILS_I18N_USTRING_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_USTRING_MODULE_OVERVIEW Overview
 * @details The Ustring module provides general unicode string handling information.
 *
 * @section CAPI_BASE_UTILS_I18N_USTIRING_MODULE_SAMPLE_CODE_1 Sample Code 1
 * @brief It converts a byte string to a unicode string and then to uppercase letters.
 * @code
	char str_1[64] = {0,};
	i18n_uchar uchar_str_1[64] = {0,};
	i18n_uchar uchar_str_2[64] = {0,};
	int uchar_len = 0;
	i18n_uerror_code_e err_code = I18N_ERROR_NONE;

	strcpy(str_1, "tizen");
	dlog_print(DLOG_INFO, LOG_TAG, "str_1 is %s\n", str_1);	   // str_1 is tizen

	// converts a byte string to a unicode string
	i18n_ustring_copy_ua_n(uchar_str_1, str_1, strlen(str_1));

	// converts to uppercase letters
	i18n_ustring_to_upper(uchar_str_2, 64, uchar_str_1, i18n_ustring_get_length( uchar_str_1 ), "en_US", &err_code);

	i18n_ustring_copy_au(str_1, uchar_str_2);
	dlog_print(DLOG_INFO, LOG_TAG, "str_1 is %s\n", str_1);	   // str_1 is TIZEN
 * @endcode
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_USTRING_MODULE
 * @{
 */

/**
 * @brief Determines the length of an array of #i18n_uchar.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s		The array of #i18n_uchar characters, @c NULL (U+0000) terminated.
 *
 * @return The number of #i18n_uchar characters in @c chars, minus the terminator
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_get_length(const i18n_uchar *s);

/**
 * @brief Counts Unicode code points in the length #i18n_uchar code units of the string.
 * @details A code point may occupy either one or two #i18n_uchar code units.
 *			Counting code points involves reading all code units.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s			The input string.
 * @param[in] length	The number of #i18n_uchar code units to be checked, or @c -1 to count
 *						all code points before the first NULL (U+0000).
 *
 * @return The number of code points in the specified code units.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_count_char32(const i18n_uchar *s, int32_t length);

/**
 * @brief Checks if the string contains more Unicode code points than a certain number.
 * @details This is more efficient than counting all code points in the entire string and comparing that number with a threshold.
 *			This function may not need to scan the string at all if the length is known (not @c -1 for NULL-termination) and falls within a certain range,
 *			and never needs to count more than 'number+1' code points.
 *			Logically equivalent to ( i18n_ustring_count_char32 (s, length, &number_of_code_points); number_of_code_points > number ).
 *			A Unicode code point may occupy either one or two #i18n_uchar code units.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s			The input string.
 * @param[in] length	The length of the string, or @c -1 if it is NULL-terminated.
 * @param[in] number	The number of code points in the string is compared against the @a number parameter.
 *
 * @return Boolean value for whether the string contains more Unicode code points than @a number. Same as ( i18n_ustring_count_char32 (s, length, &number_of_code_points); number_of_code_points > number).
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_ubool i18n_ustring_has_more_char32_than(const i18n_uchar *s, int32_t length, int32_t number);

/**
 * @brief Concatenates two ustrings.
 * @details Appends a copy of @a src, including the NULL terminator, to @a dest.
 *			The initial copied character from @a src overwrites the NULL terminator in @a dest.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[out] dest		The destination string.
 * @param[in] src		The source string.
 *
 * @return A pointer to @a dest.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_uchar *i18n_ustring_cat(i18n_uchar *dest, const i18n_uchar *src);

/**
 * @brief Concatenate two ustrings.
 * @details Appends a copy of @a src, including the NULL terminator, to @a dest.
 *			The initial copied character from @a src overwrites the NULL terminator in @a dest.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[out] dest		The destination string.
 * @param[in] src		The source string.
 * @param[in] n			The maximum number of characters to append; no-op if <=0.
 *
 * @return A pointer to @a dest.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_uchar *i18n_ustring_cat_n(i18n_uchar *dest, const i18n_uchar *src, int32_t n);

/**
 * @brief Finds the first occurrence of a substring in a string.
 * @details The substring is found at code point boundaries. That means that if the substring begins with a trail surrogate
 *			or ends with a lead surrogate, then it is found only if these surrogates stand alone in the text.
 *			Otherwise, the substring edge units would be matched against halves of surrogate pairs.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s				The string to search (NULL-terminated).
 * @param[in] sub_string	The substring to find (NULL-terminated).
 *
 * @return A pointer to the first occurrence of @a sub_string in @a s,
 *		   or @a s itself if the @a sub_string is empty,
 *		   or @c NULL if @a sub_string is not in @a s.
 *
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_ustring_r_string()
 * @see i18n_ustring_find_first()
 * @see i18n_ustring_find_last()
 */
i18n_uchar *i18n_ustring_string(const i18n_uchar *s, const i18n_uchar *sub_string);

/**
 * @brief Finds the first occurrence of a substring in a string.
 * @details The substring is found at code point boundaries. That means that if the substring begins with a trail surrogate
 *			or ends with a lead surrogate, then it is found only if these surrogates stand alone in the text.
 *			Otherwise, the substring edge units would be matched against halves of surrogate pairs.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s				The string to search (NULL-terminated).
 * @param[in] length		The length of @a s (number of #i18n_uchar characters), or @c -1 if it is NULL-terminated.
 * @param[in] sub_string	The substring to find (NULL-terminated).
 * @param[in] sub_length	The length of substring (number of #i18n_uchar characters), or @c -1 if it is NULL-terminated.
 *
 * @return A pointer to the first occurrence of @a sub_string in @a s,
 *		   or @a s itself if the @a sub_string is empty,
 *		   or @c NULL if @a sub_string is not in @a s.
 *
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_ustring_string()
 * @see i18n_ustring_find_last()
 */
i18n_uchar *i18n_ustring_find_first(const i18n_uchar *s, int32_t length, const i18n_uchar *sub_string, int32_t sub_length);

/**
 * @brief Finds the first occurrence of a BMP code point in a string.
 * @details A surrogate code point is found only if its match in the text is not part of a surrogate pair. A NULL character is found at the string terminator.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s The string to search (NULL-terminated).
 * @param[in] c The BMP code point to find.
 *
 * @return A pointer to the first occurrence of @a c in @a s
 * or @c NULL if @a c is not in @a s.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_char32()
 * @see i18n_ustring_mem_char()
 * @see i18n_ustring_string()
 * @see i18n_ustring_find_first()
 */
i18n_uchar *i18n_ustring_char(const i18n_uchar *s, i18n_uchar c);

/**
 * @brief Finds the first occurrence of a code point in a string.
 * @details A surrogate code point is found only if its match in the text is not part of a surrogate pair. A NULL character is found at the string terminator.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s The string to search (NULL-terminated).
 * @param[in] c The code point to find.
 *
 * @return A pointer to the first occurrence of @a c in @a s
 * or @c NULL if @a c is not in @a s.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_char()
 * @see i18n_ustring_mem_char32()
 * @see i18n_ustring_string()
 * @see i18n_ustring_find_first()
 */
i18n_uchar *i18n_ustring_char32(const i18n_uchar *s, i18n_uchar32 c);

/**
 * @brief Finds the last occurrence of a substring in a string.
 * @details The substring is found at code point boundaries. That means that if the substring begins with a trail surrogate
 * or ends with a lead surrogate, then it is found only if these surrogates stand alone in the text.
 * Otherwise, the substring edge units would be matched against halves of surrogate pairs.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s The string to search (NULL-terminated).
 * @param[in] sub_string The substring to find (NULL-terminated).
 *
 * @return A pointer to the last occurrence of @a substring in @a s,
 * or @a s itself if the @a sub_string is empty,
 * or @c NULL if @a sub_string is not in @a s.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_string()
 * @see i18n_ustring_find_first()
 * @see i18n_ustring_find_last()
 */
i18n_uchar *i18n_ustring_r_string(const i18n_uchar *s, const i18n_uchar *sub_string);

/**
 * @brief Finds the last occurrence of a substring in a string.
 * @details The substring is found at code point boundaries. That means that if the substring begins with a trail surrogate
 * or ends with a lead surrogate, then it is found only if these surrogates stand alone in the text.
 * Otherwise, the substring edge units would be matched against halves of surrogate pairs.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s The string to search.
 * @param[in] length The length of s (number of #i18n_uchar), or @c -1 if it is NULL-terminated.
 * @param[in] sub_string The sub_string to find (NULL-terminated).
 * @param[in] sub_length The length of sub_string (number of #i18n_uchar), or @c -1 if it is NULL-terminated.
 *
 * @return A pointer to the last occurrence of @a sub_string in @a s,
 * or @a s itself if the @a substring is empty,
 * or @c NULL if @a sub_string is not in @a s.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_string()
 * @see i18n_ustring_find_first()
 */
i18n_uchar *i18n_ustring_find_last(const i18n_uchar *s, int32_t length, const i18n_uchar *sub_string, int32_t sub_length);

/**
 * @brief Finds the last occurrence of a BMP code point in a string.
 * @details A surrogate code point is found only if its match in the text is not part of a surrogate pair. A NULL character is found at the string terminator.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s The string to search (NULL-terminated).
 * @param[in] c The BMP code point to find.
 *
 * @return A pointer to the last occurrence of @a c in @a s
 * or @c NULL if @a c is not in @a s.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_char32()
 * @see i18n_ustring_mem_char()
 * @see i18n_ustring_string()
 * @see i18n_ustring_find_first()
 */
i18n_uchar *i18n_ustring_r_char(const i18n_uchar *s, i18n_uchar c);

/**
 * @brief Finds the last occurrence of a code point in a string.
 * @details A surrogate code point is found only if its match in the text is not part of a surrogate pair. A NULL character is found at the string terminator.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s The string to search (NULL-terminated).
 * @param[in] c The code point to find.
 *
 * @return A pointer to the last occurrence of @a c in @a s
 * or @c NULL if @a c is not in @a s.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_char()
 * @see i18n_ustring_mem_char32()
 * @see i18n_ustring_string()
 * @see i18n_ustring_find_first()
 */
i18n_uchar *i18n_ustring_r_char32(const i18n_uchar *s, i18n_uchar32 c);

/**
 * @brief Locates the first occurrence in the string of any of the characters in the string matchSet.
 * @details Works just like C's strpbrk but with Unicode.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] string The string in which to search, NULL-terminated.
 * @param[in] match_set A NULL-terminated string defining a set of code points for which to search in the text string.
 *
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @return A pointer to the	 character in @a string that matches one of the
 * characters in @a match_set, or NULL if no such character is found.
 */
i18n_uchar *i18n_ustring_pbrk(const i18n_uchar *string, const i18n_uchar *match_set);

/**
 * @brief Returns the number of consecutive characters in string, beginning with the first, that do not occur somewhere in match_set.
 * @details Works just like C's strcspn but with Unicode.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] string The string in which to search, NULL-terminated.
 * @param[in] match_set A NULL-terminated string defining a set of code points for which to search in the text string.
 *
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @return The number of initial characters in @a string that do not
 * occur in @a match_set.
 * @see i18n_ustring_spn()
 */
int32_t i18n_ustring_cspn(const i18n_uchar *string, const i18n_uchar *match_set);

/**
 * @brief Returns the number of consecutive characters in string, beginning with the first, that occur somewhere in match_set.
 * @details Works just like C's strspn but with Unicode.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] string The string in which to search, NULL-terminated.
 * @param[in] match_set A NULL-terminated string defining a set of code points for which to search in the text string.
 * @return The number of initial characters in @a string that do
 * occur in @a match_set.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_cspn()
 */
int32_t i18n_ustring_spn(const i18n_uchar *string, const i18n_uchar *match_set);

/**
 * @brief The string tokenizer API allows an application to break a string into tokens.
 * @details Works just like C's strspn but with Unicode.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @remarks The @a save_state should not be released.
 * @since_tizen 2.3
 *
 * @param[in] src String containing token(s). This string will be modified. After the first call to #i18n_ustring_tokenizer_r(), this argument must be NULL to get to the next token.
 * @param[in] delim Set of delimiter characters (Unicode code points).
 * @param[out] save_state The current pointer within the original string, which is set by this function.
 * The save_state parameter should the address of a local variable of type #i18n_uchar *.
 * @return A pointer to the next token found in src, or NULL
 * when there are no more tokens.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_uchar *i18n_ustring_tokenizer_r(i18n_uchar *src, const i18n_uchar *delim, i18n_uchar **save_state);

/**
 * @brief Compares two Unicode strings for bitwise equality (code unit order).
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s1 A string to compare.
 * @param[in] s2 A string to compare.
 * @return 0 if @a s1 and @a s2 are bitwise equal; a negative
 * value if @a s1 is bitwise less than @a s2; a positive
 * value if @a s1 is bitwise greater than @a s2.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_compare(const i18n_uchar *s1, const i18n_uchar *s2);

/**
 * @brief Compare two Unicode strings in code point order.
 * @details See #i18n_ustring_compare() for details.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s1 A string to compare.
 * @param[in] s2 A string to compare.
 * @return a negative/zero/positive integer corresponding to whether
 * the first string is less than/equal to/greater than the second one
 * in code point order
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_compare_code_point_order(const i18n_uchar *s1, const i18n_uchar *s2);

/**
 * @brief Compare two Unicode strings (binary order).
 * @details The comparison can be done in code unit order or in code point order. They differ only in UTF-16 when comparing supplementary code points
 * (U+10000..U+10ffff) to BMP code points near the end of the BMP (i.e., U+e000..U+ffff). In code unit order, high BMP code points sort after
 * supplementary code points because they are stored as pairs of surrogates which are at U+d800..U+dfff.\n
 * This functions works with strings of different explicitly specified lengths unlike the ANSI C-like #i18n_ustring_compare() and #i18n_ustring_mem_compare() etc.
 * NULL-terminated strings are possible with length arguments of -1.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s1 First source string.
 * @param[in] length1 Length of first source string, or @c -1 if NULL-terminated.
 * @param[in] s2 Second source string.
 * @param[in] length2 Length of second source string, or @c -1 if NULL-terminated.
 * @param[in] code_point_order Choose between code unit order (false) and code point order (true).
 * @return < 0, 0 or > 0 as usual for string comparisons
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_compare_binary_order(const i18n_uchar *s1, int32_t length1, const i18n_uchar *s2, int32_t length2, i18n_ubool code_point_order);

/**
 * @brief Compare two strings case-insensitively using full case folding.
 * @details The comparison can be done in UTF-16 code unit order or in code point order. They differ only when comparing
 * supplementary code points (U+10000..U+10ffff) to BMP code points near the end of the BMP (i.e., U+e000..U+ffff).
 * In code unit order, high BMP code points sort after supplementary code points because they are stored as pairs of surrogates which are at U+d800..U+dfff.\n
 * This functions works with strings of different explicitly specified lengths unlike the ANSI C-like #i18n_ustring_compare() and i18n_ustring_mem_compare() etc.
 * NULL-terminated strings are possible with length arguments of -1.
 * @since_tizen 2.3
 *
 * @param[in] s1 First source string.
 * @param[in] length1 Length of first source string, or @c -1 if NULL-terminated.
 * @param[in] s2 Second source string.
 * @param[in] length2 Length of second source string, or @c -1 if NULL-terminated.
 * @param[in] options A bit set of options:\n
 * - #I18N_USTRING_U_FOLD_CASE_DEFAULT or 0 is used for default options: Comparison in code unit order with default case folding.\n
 * - #I18N_USTRING_U_COMPARE_CODE_POINT_ORDER Set to choose code point order instead of code unit order (see i18n_ustring_compare_code_pointer_order() for details).\n
 * - #I18N_USTRING_U_FOLD_CASE_EXCLUDE_SPECIAL_I
 * @param[out] error_code Must be a valid pointer to an error code value,
 * which must not indicate a failure before the function call.
 * @return <0 or 0 or >0 as usual for string comparisons
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_case_compare_with_length(const i18n_uchar *s1, int32_t length1, const i18n_uchar *s2, int32_t length2, uint32_t options, i18n_error_code_e *error_code);

/**
 * @brief Compare two ustrings for bitwise equality.
 * @details Compares at most n characters.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s1 A string to compare (can be NULL/invalid if n<=0).
 * @param[in] s2 A string to compare (can be NULL/invalid if n<=0).
 * @param[in] n The maximum number of characters to compare; always returns 0 if n<=0.
 * @return 0 if @a s1 and @a s2 are bitwise equal; a negative
 * value if @a s1 is bitwise less than @a s2; a positive
 * value if @a s1 is bitwise greater than @a s2.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_compare_n(const i18n_uchar *s1, const i18n_uchar *s2, int32_t n);

/**
 * @brief Compare two Unicode strings in code point order.
 * @details This is different in UTF-16 from #i18n_ustring_compare_n() if supplementary characters are present. For details, see #i18n_ustring_compare_binary_order().
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s1 A string to compare.
 * @param[in] s2 A string to compare.
 * @param[in] n The maximum number of characters to compare.
 * @return a negative/zero/positive integer corresponding to whether
 * the first string is less than/equal to/greater than the second one
 * in code point order
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_compare_n_code_point_order(const i18n_uchar *s1, const i18n_uchar *s2, int32_t n);

/**
 * @brief Compare two strings case-insensitively using full case folding.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s1 A string to compare.
 * @param[in] s2 A string to compare.
 * @param[in] options  bit set of options:\n
 *	- #I18N_USTRING_U_FOLD_CASE_DEFAULT or 0 is used for default options: Comparison in code unit order with default case folding.
 *	- #I18N_USTRING_U_COMPARE_CODE_POINT_ORDER Set to choose code point order instead of code unit order (see u_strCompare for details).
 *	- #I18N_USTRING_U_FOLD_CASE_EXCLUDE_SPECIAL_I
 * @return A negative, zero, or positive integer indicating the comparison result.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_case_compare(const i18n_uchar *s1, const i18n_uchar *s2, uint32_t options);

/**
 * @brief Compare two strings case-insensitively using full case folding.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s1 A string to compare.
 * @param[in] s2 A string to compare.
 * @param[in] n The maximum number of characters each string to case-fold and then compare.
 * @param[in] options A bit set of options:\n
 *	- #I18N_USTRING_U_FOLD_CASE_DEFAULT or 0 is used for default options: Comparison in code unit order with default case folding.
 *	- #I18N_USTRING_U_COMPARE_CODE_POINT_ORDER Set to choose code point order instead of code unit order (see u_strCompare for details).
 *	- #I18N_USTRING_U_FOLD_CASE_EXCLUDE_SPECIAL_I
 * @return A negative, zero, or positive integer indicating the comparison result.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_case_compare_n(const i18n_uchar *s1, const i18n_uchar *s2, int32_t n, uint32_t options);

/**
 * @brief Compare two strings case-insensitively using full case folding.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s1 A string to compare.
 * @param[in] s2 A string to compare.
 * @param[in] length The number of characters in each string to case-fold and then compare.
 * @param[in] options A bit set of options:\n
 *	- #I18N_USTRING_U_FOLD_CASE_DEFAULT or 0 is used for default options: Comparison in code unit order with default case folding.
 *	- #I18N_USTRING_U_COMPARE_CODE_POINT_ORDER Set to choose code point order instead of code unit order (see u_strCompare for details).
 *	- #I18N_USTRING_U_FOLD_CASE_EXCLUDE_SPECIAL_I
 * @return A negative, zero, or positive integer indicating the comparison result.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_mem_case_compare(const i18n_uchar *s1, const i18n_uchar *s2, int32_t length, uint32_t options);

/**
 * @brief Copies a ustring. Adds a NULL terminator.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[out]	dest The destination string
 * @param[in]	src The source string
 *
 * @return A pointer to @a dest.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_uchar *i18n_ustring_copy(i18n_uchar *dest, const i18n_uchar *src);

/**
 * @brief Copies a ustring.
 * @details Copies at most @a n characters. The result will be NULL terminated
 * if the length of @a src is less than @a n.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[out] dest The destination string
 * @param[in] src The source string
 * @param[in] n The maximum number of characters to copy
 *
 * @return A pointer to @a dest.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_uchar *i18n_ustring_copy_n(i18n_uchar *dest, const i18n_uchar *src, int32_t n);

/**
 * @brief Copies a byte string encoded in the default codepage to a ustring.
 * @details Adds a NULL terminator. Performs a host byte to #i18n_uchar conversion.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[out] dest The destination string
 * @param[in] src The source string
 *
 * @return A pointer to @a dest.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_uchar *i18n_ustring_copy_ua(i18n_uchar *dest, const char *src);

/**
 * @brief Copies a byte string encoded in the default codepage to a ustring.
 * @details Copies at most @a n characters.	 The result will be NULL terminated
 * if the length of @a src is less than @a n.
 * Performs a host byte to #i18n_uchar conversion.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[out] dest The destination string
 * @param[in] src The source string
 * @param[in] n The maximum number of characters to copy
 *
 * @return A pointer to @a dest.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_uchar *i18n_ustring_copy_ua_n(i18n_uchar *dest, const char *src, int32_t n);

/**
 * @brief Copies a ustring to a byte string encoded in the default codepage.
 * @details Adds a NULL terminator. Performs an #i18n_uchar to host byte conversion.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[out] dest The destination string
 * @param[in] src The source string
 *
 * @return A pointer to @a dest.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
char *i18n_ustring_copy_au(char *dest, const i18n_uchar *src);

/**
 * @brief Copies a ustring to a byte string encoded in the default codepage.
 * @details Copies at most @a n characters.	 The result will be NULL terminated
 * if the length of @a src is less than @a n.
 * Performs an #i18n_uchar to host byte conversion.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[out] dest The destination string
 * @param[in] src The source string
 * @param[in] n The maximum number of characters to copy
 *
 * @return A pointer to @a dest.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
char *i18n_ustring_copy_au_n(char *dest, const i18n_uchar *src, int32_t n);

/**
 * @brief Synonym for memcpy(), but with #i18n_uchar characters only.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[out] dest The destination string
 * @param[in] src The source string (can be NULL/invalid if count<=0)
 * @param[in] count The number of characters to copy; no-op if <=0
 *
 * @return A pointer to @a dest
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_uchar *i18n_ustring_mem_copy(i18n_uchar *dest, const i18n_uchar *src, int32_t count);

/**
 * @brief Synonym for memmove(), but with #i18n_uchar characters only.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[out] dest The destination string
 * @param[in] src The source string (can be NULL/invalid if count<=0)
 * @param[in] count The number of characters to copy; no-op if <=0
 *
 * @return A pointer to @a dest
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_uchar *i18n_ustring_mem_move(i18n_uchar *dest, const i18n_uchar *src, int32_t count);

/**
 * @brief Initialize count characters of dest to c.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[out] dest The destination string
 * @param[in] c The character to initialize the string.
 * @param[in] count The maximum number of characters to set.
 *
 * @return A pointer to @a dest.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_uchar *i18n_ustring_mem_set(i18n_uchar *dest, const i18n_uchar c, int32_t count);

/**
 * @brief Compare the first count #i18n_uchar characters of each buffer.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] buf1 The first string to compare.
 * @param[in] buf2 The second string to compare.
 * @param[in] count The maximum number of #i18n_uchar characters to compare.
 * @return When buf1 < buf2, a negative number is returned.
 * When buf1 == buf2, 0 is returned.
 * When buf1 > buf2, a positive number is returned.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_mem_compare(const i18n_uchar *buf1, const i18n_uchar *buf2, int32_t count);

/**
 * @brief Compare two Unicode strings in code point order.
 * @details This is different in UTF-16 from #i18n_ustring_mem_compare() if supplementary characters are present. For details, see #i18n_ustring_compare_binary_order().
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s1 A string to compare.
 * @param[in] s2 A string to compare.
 * @param[in] count The maximum number of characters to compare.
 * @return a negative/zero/positive integer corresponding to whether
 * the first string is less than/equal to/greater than the second one
 * in code point order
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_mem_compare_code_point_order(const i18n_uchar *s1, const i18n_uchar *s2, int32_t count);

/**
 * @brief Finds the first occurrence of a BMP code point in a string.
 * @details A surrogate code point is found only if its match in the text is not part of a surrogate pair. A NULL character is found at the string terminator.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s The string to search (contains count #i18n_uchar characters).
 * @param[in] c The BMP code point to find.
 * @param[in] count The length of the string.
 * @return A pointer to the first occurrence of @a c in @a s
 * or @c NULL if @a c is not in @a s.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_char()
 * @see i18n_ustring_mem_char32()
 * @see i18n_ustring_find_first()
 */
i18n_uchar *i18n_ustring_mem_char(const i18n_uchar *s, i18n_uchar c, int32_t count);

/**
 * @brief Finds the first occurrence of a code point in a string.
 * @details A surrogate code point is found only if its match in the text is not part of a surrogate pair. A NULL character is found at the string terminator.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s The string to search (contains count #i18n_uchar characters).
 * @param[in] c The code point to find.
 * @param[in] count The length of the string.
 * @return A pointer to the first occurrence of @a c in @a s
 * or @c NULL if @a c is not in @a s.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_uchar *i18n_ustring_mem_char32(const i18n_uchar *s, i18n_uchar32 c, int32_t count);

/**
 * @brief Finds the last occurrence of a BMP code point in a string.
 * @details A surrogate code point is found only if its match in the text is not part of a surrogate pair. A NULL character is found at the string terminator.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s The string to search (contains count #i18n_uchar characters).
 * @param[in] c The BMP code point to find.
 * @param[in] count The length of the string.
 * @return A pointer to the last occurrence of @a c in @a s
 * or @c NULL if @a c is not in @a s.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #i18n_ustring_r_char
 * @see #i18n_ustring_mem_r_char32
 * @see #i18n_ustring_find_last
 */
i18n_uchar *i18n_ustring_mem_r_char(const i18n_uchar *s, i18n_uchar c, int32_t count);

/**
 * @brief Finds the last occurrence of a code point in a string.
 * @details A surrogate code point is found only if its match in the text is not part of a surrogate pair. A NULL character is found at the string terminator.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] s The string to search (contains count #i18n_uchar characters).
 * @param[in] c The code point to find.
 * @param[in] count The length of the string.
 * @return A pointer to the last occurrence of @a c in @a s
 * or @c NULL if @a c is not in @a s.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #i18n_ustring_r_char32
 * @see #i18n_ustring_mem_r_char
 * @see #i18n_ustring_find_last
 */
i18n_uchar *i18n_ustring_mem_r_char32(const i18n_uchar *s, i18n_uchar32 c, int32_t count);

/**
 * @brief Unescape a string of characters and write the resulting Unicode characters to the destination buffer.
 * @details The following escape sequences are recognized:\n
 * \\uhhhh 4 hex digits; h in [0-9A-Fa-f] \\Uhhhhhhhh 8 hex digits \\xhh 1-2 hex digits \\x{h...} 1-8 hex digits \\ooo 1-3 octal digits; o in [0-7] \\cX control-X; X is masked with 0x1F\n
 * as well as the standard ANSI C escapes:\n
 * \\a => U+0007, \\b => U+0008, \\t => U+0009, \\n => U+000A, \\v => U+000B, \\f => U+000C, \\r => U+000D, \\e => U+001B, \\&quot; => U+0022, \\' => U+0027, \\? => U+003F, \\\\ => U+005C\n
 * Anything else following a backslash is generically escaped. For example, "[a\-z]" returns "[a-z]".\n
 * If an escape sequence is ill-formed, this method returns an empty string. An example of an ill-formed sequence is "\\u" followed by fewer than 4 hex digits.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] src a zero-terminated string of invariant characters
 * @param[in] dest pointer to buffer to receive converted and unescaped text and, if there is room, a zero terminator. May be NULL for preflighting, in which case no #i18n_uchar characters will be written,
 * but the return value will still be valid. On error, an empty string is stored here (if possible).
 * @param[in] dest_capacity the number of #i18n_uchar characters that may be written at dest. Ignored if dest == NULL.
 * @return the length of unescaped string.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_unescape_at()
 */
int32_t i18n_ustring_unescape(const char *src, i18n_uchar *dest, int32_t dest_capacity);

/**
 * @brief Unescape a single sequence.
 * @details The character at offset-1 is assumed (without checking) to be a backslash. This method takes a callback pointer to a function that returns the #i18n_uchar at a given offset.
 * By varying this callback, I18N functions are able to unescape char* strings, and UnicodeString objects.\n
 * If offset is out of range, or if the escape sequence is ill-formed, (i18n_uchar32)0xFFFFFFFF is returned.
 * See documentation of #i18n_ustring_unescape() for a list of recognized sequences.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] char_at callback function that returns a #i18n_uchar of the source text given an offset and a context pointer.
 * @param[in] offset pointer to the offset that will be passed to char_at. The offset value will be updated upon return to point after the last parsed character of the escape sequence.
 * On error the offset is unchanged.
 * @param[in] length the number of #i18n_uchar characters that may be written at dest. Ignored if dest == NULL.
 * @param[in] context an opaque pointer passed directly into char_at.
 *
 * @return the character represented by the escape sequence at
 * offset, or (i18n_uchar32)0xFFFFFFFF on error.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_unescape()
 */
i18n_uchar32 i18n_ustring_unescape_at(i18n_ustring_unescape_char_at_cb char_at, int32_t *offset, int32_t length, void *context);

/**
 * @brief Uppercases the characters in a string.
 * @details Casing is locale-dependent and context-sensitive.
 * The result may be longer or shorter than the original.
 * The source string and the destination buffer are allowed to overlap.
 * @since_tizen 2.3
 *
 * @param[out] dest A buffer for the result string\n The result will be zero-terminated if
 * the buffer is large enough.
 * @param[in] dest_capacity The size of the buffer (number of #i18n_uchar characters)\n
 * If it is @c 0, then @a dest may be @c NULL and the function will only return the length of the result
 * without writing any of the result string.
 * @param[in] src The original string
 * @param[in] src_len The length of the original string\n
 * If @c -1, then @a src must be zero-terminated.
 * @param[in]  locale The locale to consider, or "" for the root locale or @c NULL for the default locale.
 * @param[out] error_code Must be a valid pointer to an error code value,
 * which must not indicate a failure before the function call.
 * @return The length of the result string. It may be greater than destCapacity. In that case,
 * only some of the result was written to the destination buffer.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_to_upper(i18n_uchar *dest, int32_t dest_capacity, const i18n_uchar *src, int32_t src_len, const char *locale, i18n_error_code_e *error_code);

/**
 * @brief Lowercase the characters in a string.
 * @details Casing is locale-dependent and context-sensitive. The result may be longer or shorter than the original. The source string and the destination buffer are allowed to overlap.
 * The result may be longer or shorter than the original.
 * The source string and the destination buffer are allowed to overlap.
 * @since_tizen 2.3
 *
 * @param[out] dest A buffer for the result string. The result will be zero-terminated if the buffer is large enough.
 * @param[in] dest_capacity The size of the buffer (number of #i18n_uchar characters)\n
 * If it is 0, then dest may be NULL and the function will only return the length of the result without writing any of the result string.
 * @param[in] src The original string
 * @param[in] src_len The length of the original string.\n
 * If @c -1, then @a src must be zero-terminated.
 * @param[in] locale The locale to consider, or "" for the root locale or @c NULL for the default locale.
 * @param[out] error_code Must be a valid pointer to an error code value,
 * which must not indicate a failure before the function call.
 * @return The length of the result string. It may be greater than destCapacity. In that case,
 * only some of the result was written to the destination buffer.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_to_lower(i18n_uchar *dest, int32_t dest_capacity, const i18n_uchar *src, int32_t src_len, const char *locale, i18n_error_code_e *error_code);

/**
 * @brief Titlecases a string.
 * @details Casing is locale-dependent and context-sensitive.
 * Titlecasing uses a break iterator to find the first characters of words
 * that are to be titlecased. It titlecases those characters and lowercases
 * all others.
 *
 * @remarks The specific error code can be obtained using the get_last_result() method.
 * Error codes are described in Exceptions section and in #i18n_error_code_e description.
 *
 * The titlecase break iterator can be provided to customize arbitrary
 * styles, using rules and dictionaries beyond the standard iterators.
 * It may be more efficient to always provide an iterator to avoid
 * opening and closing one for each string.
 * The standard titlecase iterator for the root locale implements the
 * algorithm of Unicode TR 21.\n
 * The result may be longer or shorter than the original.
 * The source string and the destination buffer are allowed to overlap.
 * @since_tizen 2.3.1
 *
 * @param[out] dest A buffer for the result string.\n
 * The result will be zero-terminated if the buffer is large enough.
 * @param[in] dest_capacity The size of the buffer (number of #i18n_uchar characters.\n
 * If it is @c 0, then @a dest may be @c NULL and the function will only return the length of the result
 * without writing any of the result string.
 * @param[in] src The original string
 * @param[in] src_len The length of the original string.\n	If @c -1, then @a src must be zero-terminated.
 * @param[in] title_iter A break iterator to find the first characters of words
 * that are to be titlecased.\n
 * If none are provided (@c NULL), then a standard titlecase
 * break iterator is opened.
 * @param[in] locale The locale to consider, or "" for the root locale or @c NULL for the default locale.
 * @return The length of the result string. It may be greater than dest_capacity. In that case,
 * only some of the result were written to the destination buffer.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_to_title_new(i18n_uchar *dest, int32_t dest_capacity, const i18n_uchar *src, int32_t src_len, i18n_ubreak_iterator_h title_iter, const char *locale);

/**
 * @brief Case-folds the characters in a string.
 * @details Case-folding is locale-independent and not context-sensitive,
 * but there is an option for whether to include or exclude mappings for dotted I and dotless i.\n
 * The result may be longer or shorter than the original.
 * The source string and the destination buffer are allowed to overlap.
 * @since_tizen 2.3
 *
 * @param[out] dest A buffer for the result string\n
 * The result will be zero-terminated if the buffer is large enough.
 * @param[in] dest_capacity The size of the buffer (number of #i18n_uchar characters)\n
 * If it is @c 0, then @a dest may be @c NULL and the function will only return the length of the result
 * without writing any of the result string.
 * @param[in] src The original string
 * @param[in] src_len The length of the original string.\n	If @c -1, then @a src must be zero-terminated.
 * @param[in] options Either #I18N_USTRING_U_FOLD_CASE_DEFAULT or #I18N_USTRING_U_FOLD_CASE_EXCLUDE_SPECIAL_I
 * @param[out] error_code Must be a valid pointer to an error code value,
 * which must not indicate a failure before the function call.
 * @return The length of the result string. It may be greater than destCapacity. In that case,
 * only some of the result was written to the destination buffer.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ustring_fold_case(i18n_uchar *dest, int32_t dest_capacity, const i18n_uchar *src, int32_t src_len, uint32_t options, i18n_error_code_e *error_code);

/**
 * @brief Convert a UTF-16 string to a wchar_t string.
 * @details If it is known at compile time that wchar_t strings are in UTF-16 or UTF-32, then this function simply calls the fast, dedicated function for that.
 * Otherwise, two conversions UTF-16 -> default charset -> wchar_t* are performed.
 * @since_tizen 2.3
 *
 * @param[out] dest A buffer for the result string. The result will be zero-terminated if the buffer is large enough.
 * @param[in] dest_capacity The size of the buffer (number of wchar_t's).\n
 * If it is @c 0, then @a dest may be @c NULL and the function will only return the length of the result
 * without writing any of the result string (pre-flighting).
 * @param[out] dest_len A pointer to receive the number of units written to the destination.\n
 * If dest_len!=NULL then *dest_len is always set to the number of output units corresponding to the transformation of all the input units, even in case of a buffer overflow.
 * @param[in] src The original source string.
 * @param[in] src_len The length of the original string.\n	If @c -1, then @a src must be zero-terminated.
 * @param[out] error_code Must be a valid pointer to an error code value,
 * which must not indicate a failure before the function call.
 * @return The pointer to destination buffer.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
wchar_t *i18n_ustring_to_WCS(wchar_t *dest, int32_t dest_capacity, int32_t *dest_len, const i18n_uchar *src, int32_t src_len, i18n_error_code_e *error_code);

/**
 * @brief Convert a wchar_t string to UTF-16.
 * @details If it is known at compile time that wchar_t strings are in UTF-16 or UTF-32, then this function simply calls the fast, dedicated function for that.
 * Otherwise, two conversions wchar_t* -> default charset -> UTF-16 are performed.
 * @since_tizen 2.3
 *
 * @param[out] dest A buffer for the result string. The result will be zero-terminated if the buffer is large enough.
 * @param[in] dest_capacity The size of the buffer (number of #i18n_uchar characters).\n
 * If it is @c 0, then @a dest may be @c NULL and the function will only return the length of the result
 * without writing any of the result string (pre-flighting).
 * @param[out] dest_len A pointer to receive the number of units written to the destination.\n
 * If dest_len!=NULL then *dest_len is always set to the number of output units corresponding to the transformation of all the input units, even in case of a buffer overflow.
 * @param[in] src The original source string.
 * @param[in] src_len The length of the original string.\n	If @c -1, then @a src must be zero-terminated.
 * @param[out] error_code Must be a valid pointer to an error code value,
 * which must not indicate a failure before the function call.
 * @return The pointer to destination buffer.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_uchar *i18n_ustring_from_WCS(i18n_uchar *dest, int32_t dest_capacity, int32_t *dest_len, const wchar_t *src, int32_t src_len, i18n_error_code_e *error_code);

/**
 * @brief Converts a UTF-16 string to UTF-8.
 * @details If the input string is not well-formed, then the #I18N_ERROR_INVALID_CHAR_FOUND error code is set.
 * @since_tizen 2.3
 *
 * @param[out] dest A buffer for the result string.\n
 * The result will be zero-terminated if the buffer is large enough.
 * @param[in] dest_capacity	 The size of the buffer (number of chars)\n
 * If it is @c 0, then @a dest may be @c NULL and the function will only return the length of the
 * result without writing any of the result string (pre-flighting).
 * @param[out] dest_len A pointer to receive the number of units written to the destination.\n
 * If dest_len!=NULL then *dest_len is always set to the
 * number of output units corresponding to the transformation of
 * all the input units, even in case of a buffer overflow.
 * @param[in] src The original source string
 * @param[in] src_len The length of the original string.\n
 * If @c -1, then @a src must be zero-terminated.
 * @param[out] error_code Must be a valid pointer to an error code value,
 * which must not indicate a failure before the function call.
 * @return The pointer to destination buffer.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_from_UTF8()
 */
char *i18n_ustring_to_UTF8(char *dest, int32_t dest_capacity, int32_t *dest_len, const i18n_uchar *src, int32_t src_len, i18n_error_code_e *error_code);

/**
 * @brief Converts a UTF-8 string to UTF-16.
 * @details If the input string is not well-formed, then the #I18N_ERROR_INVALID_CHAR_FOUND error code is set.
 * @since_tizen 2.3
 *
 * @param[out] dest A buffer for the result string.\n
 * The result will be zero-terminated if the buffer is large enough.
 * @param[in] dest_capacity	 The size of the buffer (number of #i18n_uchar characters)\n
 * If it is @c 0, then @a dest may be @c NULL and the function will only return the length of the
 * result without writing any of the result string (pre-flighting).
 * @param[out] dest_len A pointer to receive the number of units written to the destination.\n
 * If dest_len!=NULL then *dest_len is always set to the
 * number of output units corresponding to the transformation of
 * all the input units, even in case of a buffer overflow.
 * @param[in] src The original source string
 * @param[in] src_len The length of the original string.\n
 * If @c -1, then @a src must be zero-terminated.
 * @param[out] error_code Must be a valid pointer to an error code value,
 * which must not indicate a failure before the function call.
 * @return The pointer to destination buffer.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_uchar *i18n_ustring_from_UTF8(i18n_uchar *dest, int32_t dest_capacity, int32_t *dest_len, const char *src, int32_t src_len, i18n_error_code_e *error_code);

/**
 * @brief Convert a UTF-16 string to UTF-8.
 * Same as #i18n_ustring_to_UTF8() except for the additional sub_char which is output for illegal input sequences, instead of stopping with the #I18N_ERROR_INVALID_CHAR_FOUND error code.
 * @since_tizen 2.3
 *
 * @param[out] dest A buffer for the result string.\n
 * The result will be zero-terminated if the buffer is large enough.
 * @param[in] dest_capacity	 The size of the buffer (number of chars)\n
 * If it is @c 0, then @a dest may be @c NULL and the function will only return the length of the
 * result without writing any of the result string (pre-flighting).
 * @param[out] dest_len A pointer to receive the number of units written to the destination.\n
 * If dest_len!=NULL then *dest_len is always set to the
 * number of output units corresponding to the transformation of
 * all the input units, even in case of a buffer overflow.
 * @param[in] src The original source string
 * @param[in] src_len The length of the original string.\n
 * If @c -1, then @a src must be zero-terminated.
 * @param[in] sub_char The substitution character to use in place of an illegal input sequence, or U_SENTINEL if the function is to return with #I18N_ERROR_INVALID_CHAR_FOUND instead.
 * A substitution character can be any valid Unicode code point (up to U+10FFFF) except for surrogate code points (U+D800..U+DFFF).
 * The recommended value is U+FFFD "REPLACEMENT CHARACTER".
 * @param[out] num_substitutions Output parameter receiving the number of substitutions if sub_char>=0. Set to 0 if no substitutions occur or sub_char<0. num_substitutions can be NULL.
 * @param[out] error_code Must be a valid pointer to an error code value,
 * which must not indicate a failure before the function call.
 * @return The pointer to destination buffer.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_to_UTF8()
 * @see i18n_ustring_from_UTF8_with_sub()
 */
char *i18n_ustring_to_UTF8_with_sub(char *dest, int32_t dest_capacity, int32_t *dest_len, const i18n_uchar *src, int32_t src_len, i18n_uchar32 sub_char, int32_t *num_substitutions, i18n_error_code_e *error_code);

/**
 * @brief Convert a UTF-8 string to UTF-16.
 * @since_tizen 2.3
 *
 * @param[out] dest A buffer for the result string.\n
 * The result will be zero-terminated if the buffer is large enough.
 * @param[in] dest_capacity	 The size of the buffer (number of #i18n_uchar characters)\n
 * If it is @c 0, then @a dest may be @c NULL and the function will only return the length of the
 * result without writing any of the result string (pre-flighting).
 * @param[out] dest_len A pointer to receive the number of units written to the destination.\n
 * If dest_len!=NULL then *dest_len is always set to the
 * number of output units corresponding to the transformation of
 * all the input units, even in case of a buffer overflow.
 * @param[in] src The original source string
 * @param[in] src_len The length of the original string.\n
 * If @c -1, then @a src must be zero-terminated.
 * @param[in] sub_char The substitution character to use in place of an illegal input sequence, or U_SENTINEL if the function is to return with #I18N_ERROR_INVALID_CHAR_FOUND instead.
 * A substitution character can be any valid Unicode code point (up to U+10FFFF) except for surrogate code points (U+D800..U+DFFF).
 * The recommended value is U+FFFD "REPLACEMENT CHARACTER".
 * @param[out] num_substitutions Output parameter receiving the number of substitutions if sub_char>=0. Set to 0 if no substitutions occur or sub_char<0. num_substitutions can be NULL.
 * @param[out] error_code Must be a valid pointer to an error code value,
 * which must not indicate a failure before the function call.
 * @return The pointer to destination buffer.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_from_UTF8()
 * @see i18n_ustring_from_UTF8_lenient()
 * @see i18n_ustring_to_UTF8_with_sub()
 */
i18n_uchar *i18n_ustring_from_UTF8_with_sub(i18n_uchar *dest, int32_t dest_capacity, int32_t *dest_len, const char *src, int32_t src_len, i18n_uchar32 sub_char,
											int32_t *num_substitutions, i18n_error_code_e *error_code);

/**
 * @brief Convert a UTF-8 string to UTF-16.
 * @details Same as i18n_ustring_from_UTF8() except that this function is designed to be very fast, which it achieves by being lenient about malformed UTF-8 sequences.
 * This function is intended for use in environments where UTF-8 text is expected to be well-formed.\n
 * Its semantics are:
 * - Well-formed UTF-8 text is correctly converted to well-formed UTF-16 text.\n
 * - The function will not read beyond the input string, nor write beyond the dest_capacity.\n
 * - Malformed UTF-8 results in "garbage" 16-bit Unicode strings which may not be well-formed UTF-16. The function will resynchronize to valid code point boundaries within a small number of code points after an illegal sequence.\n
 * - Non-shortest forms are not detected and will result in "spoofing" output.\n
 * For further performance improvement, if src_len is given (>=0), then it must be dest_capacity>=src_len.\n
 * There is no inverse i18n_ustring_to_UTF8_lenient() function because there is practically no performance gain from not checking that a UTF-16 string is well-formed.
 * @since_tizen 2.3
 *
 * @param[out] dest A buffer for the result string.\n
 * The result will be zero-terminated if the buffer is large enough.
 * @param[in] dest_capacity	 The size of the buffer (number of #i18n_uchar characters)\n
 * If it is @c 0, then @a dest may be @c NULL and the function will only return the length of the
 * result without writing any of the result string (pre-flighting).
 * Unlike for other I18N functions, if src_len>=0 then it must be dest_capacity>=src_len.
 * @param[out] dest_len A pointer to receive the number of units written to the destination.\n
 * If dest_len!=NULL then *dest_len is always set to the number of output units corresponding
 * to the transformation of all the input units, even in case of a buffer overflow.
 * Unlike for other I18N functions, if src_len>=0 but dest_capacity<src_len, then *dest_len will be
 * set to src_len (and I18N_U_BUFFER_OVERFLOW_ERROR will be set) regardless of the actual result length.
 * @param[in] src The original source string
 * @param[in] src_len The length of the original string.\n
 * If @c -1, then @a src must be zero-terminated.
 * @param[out] error_code Must be a valid pointer to an error code value,
 * which must not indicate a failure before the function call.
 * @return The pointer to destination buffer.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_from_UTF8()
 * @see i18n_ustring_to_UTF8_with_sub()
 * @see i18n_ustring_from_UTF8_with_sub()
 */
i18n_uchar *i18n_ustring_from_UTF8_lenient(i18n_uchar *dest, int32_t dest_capacity, int32_t *dest_len, const char *src, int32_t src_len, i18n_error_code_e *error_code);

/**
 * @brief Convert a UTF-16 string to UTF-32.
 * @details If the input string is not well-formed, then the #I18N_ERROR_INVALID_CHAR_FOUND error code is set.
 * @since_tizen 2.3
 *
 * @param[out] dest A buffer for the result string.\n
 * The result will be zero-terminated if the buffer is large enough.
 * @param[in] dest_capacity	 The size of the buffer (number of #i18n_uchar32 characters)\n
 * If it is @c 0, then @a dest may be @c NULL and the function will only return the length of the
 * result without writing any of the result string (pre-flighting).
 * @param[out] dest_len A pointer to receive the number of units written to the destination.\n
 * If dest_len!=NULL then *dest_len is always set to the number of output units corresponding
 * to the transformation of all the input units, even in case of a buffer overflow.
 * @param[in] src The original source string
 * @param[in] src_len The length of the original string.\n
 * If @c -1, then @a src must be zero-terminated.
 * @param[out] error_code Must be a valid pointer to an error code value,
 * which must not indicate a failure before the function call.
 * @return The pointer to destination buffer.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_to_UTF32_with_sub()
 * @see i18n_ustring_from_UTF32()
 */
i18n_uchar32 *i18n_ustring_to_UTF32(i18n_uchar32 *dest, int32_t dest_capacity, int32_t *dest_len, const i18n_uchar *src, int32_t src_len, i18n_error_code_e *error_code);

/**
 * @brief Convert a UTF-32 string to UTF-16.
 * @details If the input string is not well-formed, then the #I18N_ERROR_INVALID_CHAR_FOUND error code is set.
 * @since_tizen 2.3
 *
 * @param[out] dest A buffer for the result string.\n
 * The result will be zero-terminated if the buffer is large enough.
 * @param[in] dest_capacity	 The size of the buffer (number of #i18n_uchar characters)\n
 * If it is @c 0, then @a dest may be @c NULL and the function will only return the length of the
 * result without writing any of the result string (pre-flighting).
 * @param[out] dest_len A pointer to receive the number of units written to the destination.\n
 * If dest_len!=NULL then *dest_len is always set to the number of output units corresponding
 * to the transformation of all the input units, even in case of a buffer overflow.
 * @param[in] src The original source string
 * @param[in] src_len The length of the original string.\n
 * If @c -1, then @a src must be zero-terminated.
 * @param[out] error_code Must be a valid pointer to an error code value,
 * which must not indicate a failure before the function call.
 * @return The pointer to destination buffer.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_from_UTF32_with_sub()
 * @see i18n_ustring_to_UTF32()
 */
i18n_uchar *i18n_ustring_from_UTF32(i18n_uchar *dest, int32_t dest_capacity, int32_t *dest_len, const i18n_uchar32 *src, int32_t src_len, i18n_error_code_e *error_code);

/**
 * @brief Convert a UTF-16 string to UTF-32.
 * @details Same as #i18n_ustring_to_UTF32() except for the additional sub_char which is output for illegal input sequences, instead of stopping with the #I18N_ERROR_INVALID_CHAR_FOUND error code.
 * @since_tizen 2.3
 *
 * @param[out] dest A buffer for the result string.\n
 * The result will be zero-terminated if the buffer is large enough.
 * @param[in] dest_capacity	 The size of the buffer (number of i18n_char32s)\n
 * If it is @c 0, then @a dest may be @c NULL and the function will only return the length of the
 * result without writing any of the result string (pre-flighting).
 * @param[out] dest_len A pointer to receive the number of units written to the destination.\n
 * If dest_len!=NULL then *dest_len is always set to the number of output units corresponding
 * to the transformation of all the input units, even in case of a buffer overflow.
 * @param[in] src The original source string
 * @param[in] src_len The length of the original string.\n
 * If @c -1, then @a src must be zero-terminated.
 * @param[in] sub_char The substitution character to use in place of an illegal input sequence, or U_SENTINEL if the function is to return with #I18N_ERROR_INVALID_CHAR_FOUND instead.
 * A substitution character can be any valid Unicode code point (up to U+10FFFF) except for surrogate code points (U+D800..U+DFFF).
 * The recommended value is U+FFFD "REPLACEMENT CHARACTER".
 * @param[out] num_substitutions Output parameter receiving the number of substitutions if sub_char>=0. Set to 0 if no substitutions occur or sub_char<0. num_substitutions can be NULL.
 * @param[out] error_code Must be a valid pointer to an error code value,
 * which must not indicate a failure before the function call.
 * @return The pointer to destination buffer.
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_to_UTF32()
 * @see i18n_ustring_from_UTF32_with_sub()
 */
i18n_uchar32 *i18n_ustring_to_UTF32_with_sub(i18n_uchar32 *dest, int32_t dest_capacity, int32_t *dest_len, const i18n_uchar *src, int32_t src_len,
											 i18n_uchar32 sub_char, int32_t *num_substitutions, i18n_error_code_e *error_code);

/**
 * @brief Convert a UTF-32 string to UTF-16.
 * Same as #i18n_ustring_from_UTF32() except for the additional sub_char which is output for illegal input sequences, instead of stopping with the #I18N_ERROR_INVALID_CHAR_FOUND error code.
 * @since_tizen 2.3
 *
 * @param[out] dest A buffer for the result string.\n
 * The result will be zero-terminated if the buffer is large enough.
 * @param[in] dest_capacity	 The size of the buffer (number of i18n_chars)\n
 * If it is @c 0, then @a dest may be @c NULL and the function will only return the length of the
 * result without writing any of the result string (pre-flighting).
 * @param[out] dest_len A pointer to receive the number of units written to the destination.\n
 * If dest_len!=NULL then *dest_len is always set to the number of output units corresponding
 * to the transformation of all the input units, even in case of a buffer overflow.
 * @param[in] src The original source string
 * @param[in] src_len The length of the original string.\n
 * If @c -1, then @a src must be zero-terminated.
 * @param[in] sub_char The substitution character to use in place of an illegal input sequence, or U_SENTINEL if the function is to return with #I18N_ERROR_INVALID_CHAR_FOUND instead.
 * A substitution character can be any valid Unicode code point (up to U+10FFFF) except for surrogate code points (U+D800..U+DFFF).
 * The recommended value is U+FFFD "REPLACEMENT CHARACTER".
 * @param[out] num_substitutions Output parameter receiving the number of substitutions if sub_char>=0. Set to 0 if no substitutions occur or sub_char<0. num_substitutions can be NULL.
 * @param[out] error_code Must be a valid pointer to an error code value,
 * which must not indicate a failure before the function call.
 * @return The pointer to destination buffer.
 *
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ustring_from_UTF32()
 * @see i18n_ustring_to_UTF32_with_sub()
 */
i18n_uchar *i18n_ustring_from_UTF32_with_sub(i18n_uchar *dest, int32_t dest_capacity, int32_t *dest_len, const i18n_uchar32 *src, int32_t src_len, i18n_uchar32 sub_char, int32_t *num_substitutions, i18n_error_code_e *error_code);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
