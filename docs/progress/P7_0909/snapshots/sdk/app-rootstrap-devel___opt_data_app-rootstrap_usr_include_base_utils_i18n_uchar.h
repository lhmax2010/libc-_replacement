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
 * @file utils_i18n_uchar.h
 * @version 0.1
 * @brief utils_i18n
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UCHAR_MODULE Uchar
 * @brief The Uchar module provides low-level access to the Unicode Character Database.
 *
 * @section CAPI_BASE_UTILS_I18N_UCHAR_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UCHAR_MODULE_OVERVIEW Overview
 * @details The Uchar module provides low-level access to the Unicode Character Database.
 *
 * @section CAPI_BASE_UTILS_I18N_UCHAR_MODULE_SAMPLE_CODE_1 Sample Code 1
 * @brief Gets the property value of 'east asian width' among an enumerated property,
 * and the unicode allocation block that contains the character.
 * @code
	int ret = I18N_ERROR_NONE;
	i18n_uchar32 code_point = 0;
	int property_value = 0;
	i18n_uchar_u_east_asian_width_e east_asian_width = I18N_UCHAR_U_EA_NEUTRAL;
	i18n_uchar_ublock_code_e block_code = I18N_UCHAR_UBLOCK_NO_BLOCK;

	// How to get the east asian width type for 's'
	code_point = 0x73;	  // 's'
	ret = i18n_uchar_get_int_property_value(code_point, I18N_UCHAR_EAST_ASIAN_WIDTH, &property_value);
	if (ret != I18N_ERROR_NONE) {
		dlog_print(DLOG_INFO, LOG_TAG, "Error occurred!!\n");
	} else {
		east_asian_width = (i18n_uchar_u_east_asian_width_e)property_value;
		dlog_print(DLOG_INFO, LOG_TAG, "East Asian Width Type for ( %.4x ) is ( %d )\n", code_point, east_asian_width);
		// East Asian Width Type for ( 0073 ) is ( 4 ) which is I18N_UCHAR_U_EA_NARROW
	}

	// How to get the block code for 's'
	ret = i18n_uchar_get_ublock_code(code_point, &block_code);
	if (ret != I18N_ERROR_NONE) {
		dlog_print(DLOG_INFO, LOG_TAG, "Error occurred!!\n");
	} else {
		dlog_print(DLOG_INFO, LOG_TAG, "block name for ( %.4x ) is ( %d )\n", code_point, block_code);
		// block code for ( 0073 ) is ( 1 ) which is I18N_UCHAR_UBLOCK_BASIC_LATIN
	}

	// How to get the east asian width type for 'sung' as ideographs
	code_point = 0x661F;	// 'sung' as ideographs
	ret = i18n_uchar_get_int_property_value(code_point, I18N_UCHAR_EAST_ASIAN_WIDTH, &property_value);
	if (ret != I18N_ERROR_NONE) {
		dlog_print(DLOG_INFO, LOG_TAG, "Error occurred!!\n");
	} else {
		east_asian_width = (i18n_uchar_u_east_asian_width_e)property_value;
		dlog_print(DLOG_INFO, LOG_TAG, "East Asian Width Type for ( %.4x ) is ( %d )\n", code_point, east_asian_width);
		// East Asian Width Type for ( 661f ) is ( 5 ) which is I18N_UCHAR_U_EA_WIDE
	}

	// How to get the block code for 'sung' as ideographs
	ret = i18n_uchar_get_ublock_code(code_point, &block_code);
	if (ret != I18N_ERROR_NONE) {
		dlog_print(DLOG_INFO, LOG_TAG, "Error occurred!!\n");
	} else {
		dlog_print(DLOG_INFO, LOG_TAG, "block name for ( %.4x ) is ( %d )\n", code_point, block_code);
		// block code for ( 661f ) is ( 71 ) which is I18N_UCHAR_UBLOCK_CJK_UNIFIED_IDEOGRAPHS
	}

	// How to get the east asian width type for 'sung' as hangul
	code_point = 0xC131;	// 'sung' as hangul
	ret = i18n_uchar_get_int_property_value(code_point, I18N_UCHAR_EAST_ASIAN_WIDTH, &property_value);
	if (ret != I18N_ERROR_NONE) {
		dlog_print(DLOG_INFO, LOG_TAG, "Error occurred!!\n");
	} else {
		east_asian_width = (i18n_uchar_u_east_asian_width_e)property_value;
		dlog_print(DLOG_INFO, LOG_TAG, "East Asian Width Type for ( %.4x ) is ( %d )\n", code_point, east_asian_width);
		// East Asian Width Type for ( c131 ) is ( 5 ) which is I18N_UCHAR_U_EA_WIDE
	}

	// How to get the block code for 'sung' as hangul
	ret = i18n_uchar_get_ublock_code(code_point, &block_code);
	if (ret != I18N_ERROR_NONE) {
		dlog_print(DLOG_INFO, LOG_TAG, "Error occurred!!\n");
	} else {
		dlog_print(DLOG_INFO, LOG_TAG, "block name for ( %.4x ) is ( %d )\n", code_point, block_code);
		// block code for ( c131 ) is ( 74 ) which is I18N_UCHAR_UBLOCK_HANGUL_SYLLABLES
	}

 * @endcode
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UCHAR_MODULE
 * @{
 */

/**
 * @brief Gets the property value for an enumerated property for a code point.
 * @details
 * @code
    int property_value;

    i18n_uchar_u_east_asian_width_e east_asian_width;
    i18n_uchar_get_int_property_value (c, I18N_UCHAR_EAST_ASIAN_WIDTH, &property_value);
    east_asian_width = (i18n_uchar_u_east_asian_width_e)property_value;

    bool is_ideographic;
    i18n_uchar_get_int_property_value(c, I18N_UCHAR_IDEOGRAPHIC, &property_value);
    is_ideographic = (bool)property_value;
 * @endcode
 * @since_tizen 2.3
 *
 * @param[in] c             The code point to test.
 * @param[in] which         The #i18n_uchar_uproperty_e selector constant, identifies which property
 *                          to check. \n Must be
 *                          <code>#I18N_UCHAR_BINARY_START <= @a which < #I18N_UCHAR_BINARY_LIMIT</code>
 *                          or <code>#I18N_UCHAR_INT_START <= @a which < #I18N_UCHAR_INT_LIMIT</code>
 *                          or <code>#I18N_UCHAR_MASK_START <= @a which < #I18N_UCHAR_MASK_LIMIT</code>.
 * @param[out] property_val The numeric value that is directly the property value or,
 *                          for enumerated properties, corresponds to the numeric value of the
 *                          enumerated constant of the respective property value enumeration type
 *                          (cast to enum type if necessary)\n
 *                          Returns @c 0 or @c 1 (for false/true) for binary Unicode properties\n
 *                          Returns a bit-mask for mask properties \n
 *                          Returns @c 0 if @a which is out of bounds or if the Unicode version does
 *                          not have data for the property at all, or not for this code point.
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_get_int_property_value(i18n_uchar32 c, i18n_uchar_uproperty_e which, int32_t *property_val);

/**
 * @brief Gets the Unicode allocation block that contains the character.
 * @since_tizen 2.3
 *
 * @param[in] c             The code point to test
 * @param[out] block_val    The block value for the code point
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_get_ublock_code(i18n_uchar32 c, i18n_uchar_ublock_code_e *block_val);

/**
 * @brief Checks a binary unicode property for a code point.
 * @details The properties APIs are intended to reflect unicode properties as defined in the
 *          Unicode Character Database (UCD) and Unicode Technical Reports (UTR). For details about
 *          the properties see <a href="http://www.unicode.org/ucd/">Unicode.org</a>.
 * @since_tizen 4.0
 *
 * @param[in]   c                   Code point to test
 * @param[in]   which               #i18n_uchar_uproperty_e selector constant, identifies which
 *                                  binary property to check. Must be
 *                                  <code>#I18N_UCHAR_BINARY_START <= @a which < #I18N_UCHAR_BINARY_LIMIT</code>.
 * @param[out]  has_binary_property @c true or @c false according to the binary Unicode property
 *                                  value for c. Also @c false if @a which is out of bounds or if
 *                                  the Unicode version does not have data for the property at all,
 *                                  or not for this code point.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_uproperty_e
 * @see i18n_uchar_get_int_property_value()
 * @see i18n_uchar_get_unicode_version()
 */
int i18n_uchar_has_binary_property(i18n_uchar32 c,
									i18n_uchar_uproperty_e which,
									i18n_ubool *has_binary_property);

/**
 * @brief Checks if a code point has the Alphabetic Unicode unicode property.
 * @details Same as i18n_uchar_has_binary_property(c, #I18N_UCHAR_ALPHABETIC). This is different
 *          from the i18n_uchar_is_alpha() function.
 * @since_tizen 4.0
 *
 * @param[in]   c                Code point to test
 * @param[out]  is_alphabetic    @c true if the code point has the #I18N_UCHAR_ALPHABETIC Unicode
 *                               property, @c false otherwise
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #I18N_UCHAR_ALPHABETIC
 * @see i18n_uchar_is_alpha()
 * @see i18n_uchar_has_binary_property()
 */
int i18n_uchar_is_alphabetic(i18n_uchar32 c, i18n_ubool *is_alphabetic);

/**
 * @brief Checks if a code point has the Lowercase Unicode property.
 * @details Same as i18n_uchar_has_binary_property(c, #I18N_UCHAR_LOWERCASE). This is different
 *          from i18n_uchar_is_lower() function.
 * @since_tizen 4.0
 *
 * @param[in]   c               Code point to test
 * @param[out]  is_lowercase    @c true if the code point has the #I18N_UCHAR_LOWERCASE Unicode
 *                              property, @c false otherwise
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #I18N_UCHAR_LOWERCASE
 * @see i18n_uchar_is_lower()
 * @see i18n_uchar_has_binary_property()
 */
int i18n_uchar_is_lowercase(i18n_uchar32 c, i18n_ubool *is_lowercase);

/**
 * @brief Checks if a code point has the Uppercase Unicode property.
 * @details Same as i18n_uchar_has_binary_property(c, #I18N_UCHAR_UPPERCASE). This is different
 *          from i18n_uchar_is_upper() function.
 * @since_tizen 4.0
 *
 * @param[in]   c               Code point to test
 * @param[out]  is_uppercase    @c true if the code point has the #I18N_UCHAR_UPPERCASE Unicode
 *                              property, @c false otherwise
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #I18N_UCHAR_UPPERCASE
 * @see i18n_uchar_is_upper()
 * @see i18n_uchar_has_binary_property()
 */
int i18n_uchar_is_uppercase(i18n_uchar32 c, i18n_ubool *is_uppercase);

/**
 * @brief Checks if a code point has the White_Space Unicode property.
 * @details Same as i18n_uchar_has_binary_property(c, #I18N_UCHAR_WHITE_SPACE). This is different
 *          from both i18n_uchar_is_space() and i18n_uchar_is_whitespace() functions.
 *          Note: There are several icu whitespace functions.
 * @since_tizen 4.0
 *
 * @param[in]   c               Code point to test
 * @param[out]  is_white_space  @c true if the code point has the #I18N_UCHAR_WHITE_SPACE Unicode
 *                              property, @c false otherwise.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #I18N_UCHAR_WHITE_SPACE
 * @see i18n_uchar_is_whitespace()
 * @see i18n_uchar_is_space()
 * @see i18n_uchar_is_java_space_char()
 * @see i18n_uchar_has_binary_property()
 */
int i18n_uchar_is_white_space(i18n_uchar32 c, i18n_ubool *is_white_space);

/**
 * @brief Gets the minimum value for an enumerated/integer/binary Unicode property.
 * @since_tizen 4.0
 *
 * @param[in]   which           #i18n_uchar_uproperty_e selector constant, identifies which binary
 *                              property to check. Must be
 *                              <code>#I18N_UCHAR_BINARY_START <= @a which < #I18N_UCHAR_BINARY_LIMIT</code> or
 *                              <code>#I18N_UCHAR_INT_START <= @a which < #I18N_UCHAR_INT_LIMIT</code>.
 * @param[out]  int_min_value   Minimum value returned by i18n_uchar_get_int_property_value() for
 *                              a Unicode property. @c 0 if the property selector is out of range.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_uproperty_e
 * @see i18n_uchar_has_binary_property()
 * @see i18n_uchar_get_unicode_version()
 * @see i18n_uchar_get_int_property_max_value()
 * @see i18n_uchar_get_int_property_value()
 */
int i18n_uchar_get_int_property_min_value(i18n_uchar_uproperty_e which, int32_t *int_min_value);

/**
 * @brief Gets the maximum value for an enumerated/integer/binary Unicode property.
 * @details Can be used together with i18n_uchar_get_int_property_min_value to allocate arrays of
 *          UnicodeSet or similar. Examples for min/max values (for unicode 3.2): for undefined
 *          #i18n_uchar_uproperty_e constant values, min/max values will be @c 0 / @c -1.
 * @since_tizen 4.0
 *
 * @param[in]   which           #i18n_uchar_uproperty_e selector constant, identifies which binary
 *                              property to check. Must be
 *                              <code>#I18N_UCHAR_BINARY_START <= @a which < #I18N_UCHAR_BINARY_LIMIT</code> or
 *                              <code>#I18N_UCHAR_INT_START <= @a which < #I18N_UCHAR_INT_LIMIT</code>.
 * @param[out]  int_max_value   Maximum value returned by i18n_uchar_get_int_property_value() for
 *                              a Unicode property. @a int_max_value will be set to value lower than
 *                              or equal to 0 if the property selector is out of range.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_uproperty_e
 * @see i18n_uchar_has_binary_property()
 * @see i18n_uchar_get_unicode_version()
 * @see i18n_uchar_get_int_property_min_value()
 * @see i18n_uchar_get_int_property_value()
 */
int i18n_uchar_get_int_property_max_value(i18n_uchar_uproperty_e which, int32_t *int_max_value);

/**
 * @brief Gets the numeric value for a Unicode code point as defined in the Unicode Character Database.
 * @details A "double" param type is necessary because some numeric values are fractions, negative,
 *          or too large for int32_t. For characters without any numeric values in the Unicode
 *          Character Database, @a numeric_val will be equal to #I18N_U_NO_NUMERIC_VALUE.
 *          Note: This is different from the Unicode Standard which specifies NaN as the default
 *          value. (NaN is not available on all platforms) This function is similar to
 *          java.lang.Character.getNumericValue(), but i18n_uchar_get_numeric_value() also supports
 *          negative values, large values, and fractions, while Java's getNumericValue() returns
 *          values 10..35 for ASCII letters.
 * @since_tizen 4.0
 *
 * @param[in] c             The code point to get the numeric value for
 * @param[out] numeric_val  The numeric value of @a c or #I18N_U_NO_NUMERIC_VALUE if none is defined
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #I18N_U_NO_NUMERIC_VALUE
 */
int i18n_uchar_get_numeric_value(i18n_uchar32 c, double *numeric_val);

/**
 * @brief Determines whether the specified code point has the general category "Ll" (lowercase
 *        letter).
 * @details Same as java.lang.Character.isLowerCase(). This misses some characters that are also
 *          lowercase but have a different general category value. In order to include those, use
 *          #I18N_UCHAR_LOWERCASE. In addition to being equivalent to a Java function, this also
 *          serves as a c/posix migration function. See the comments about c/posix character
 *          classification functions in the documentation at the top of this header file.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_lower        @c true if the code point is an Ll lowercase letter
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #I18N_UCHAR_LOWERCASE
 * @see i18n_uchar_is_upper()
 * @see i18n_uchar_is_title()
 */
int i18n_uchar_is_lower(i18n_uchar32 c, i18n_ubool *is_lower);

/**
 * @brief Determines whether the specified code point has the general category "Lu" (uppercase
 *        letter).
 * @details Same as java.lang.Character.isUpperCase(). This misses some characters that are also
 *          uppercase but have a different general category value. In order to include those, use
 *          #I18N_UCHAR_UPPERCASE. In addition to being equivalent to a Java function, this also
 *          serves as a c/posix migration function. See the comments about c/posix character
 *          classification functions in the documentation at the top of this header file.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_upper        @c true if the code point is an Lu uppercase letter
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #I18N_UCHAR_UPPERCASE
 * @see i18n_uchar_is_lower()
 * @see i18n_uchar_is_title()
 * @see i18n_uchar_to_lower()
 */
int i18n_uchar_is_upper(i18n_uchar32 c, i18n_ubool *is_upper);

/**
 * @brief Determines whether the specified code point is a titlecase letter.
 * @details True for general category "Lt" (titlecase letter). Same as java.lang.Character.isTitleCase().
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_title        @c true if the code point is an Lt titlecase letter
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_is_upper()
 * @see i18n_uchar_is_lower()
 * @see i18n_uchar_to_title()
 */
int i18n_uchar_is_title(i18n_uchar32 c, i18n_ubool *is_title);

/**
 * @brief Determines whether the specified code point is a digit character according to Java.
 * @details @c true for characters with general category "Nd" (decimal digit numbers). Beginning
 *          with Unicode 4, this is the same as testing for the #I18N_UCHAR_NUMERIC_TYPE of I18N_UCHAR_U_NT_DECIMAL.
 *          Same as java.lang.Character.isDigit(). In addition to being equivalent to a Java function,
 *          this also serves as a c/posix migration function. See the comments about c/posix character
 *          classification functions in the documentation at the top of this header file.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_digit        @c true if the code point is a digit character according to
 *                              Character.is_digit()
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_is_digit(i18n_uchar32 c, i18n_ubool *is_digit);

/**
 * @brief Determines whether the specified code point is a letter character.
 * @details @c true for general categories "L" (letters). Same as java.lang.Character.isLetter().
 *          In addition to being equivalent to a Java function, this also serves as a c/posix
 *          migration function. See the comments about c/posix character classification functions
 *          in the documentation at the top of this header file.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_alpha        @c true if the code point is a letter character
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_is_digit()
 * @see i18n_uchar_is_alnum()
 */
int i18n_uchar_is_alpha(i18n_uchar32 c, i18n_ubool *is_alpha);

/**
 * @brief Determines whether the specified code point is an alphanumeric character (letter or digit)
 *        according to Java.
 * @details @c true for characters with general categories "L" (letters) and "Nd" (decimal digit numbers).
 *          Same as java.lang.Character.isLetterOrDigit(). In addition to being equivalent to a Java
 *          function, this also serves as a c/posix migration function. See the comments about c/posix
 *          character classification functions in the documentation at the top of this header file.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_alnum        @c true if the code point is an alphanumeric character according to
 *                              Character.isLetterOrDigit()
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_is_alnum(i18n_uchar32 c, i18n_ubool *is_alnum);

/**
 * @brief Determines whether the specified code point is a hexadecimal digit.
 * @details This is equivalent to <code>i18n_uchar_digit(c, 16) >= 0</code>. @c true for characters
 *          with general category "Nd" (decimal digit numbers) as well as latin letters a-f and a-f
 *          in both ascii and fullwidth ascii. (that is, for letters with code points 0041..0046,
 *          0061..0066, ff21..ff26, ff41..ff46.)
 *
 *          In order to narrow the definition of hexadecimal digits to only ascii characters,
 *          use <code>(c <= 0x7f && i18n_uchar_is_xdigit(c))</code>. This is a c/posix migration function.
 *          See the comments about c/posix character classification functions in the documentation
 *          at the top of this header file.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_xdigit       @c true if the code point is a hexadecimal digit
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_is_xdigit(i18n_uchar32 c, i18n_ubool *is_xdigit);

/**
 * @brief Determines whether the specified code point is a punctuation character.
 * @details @c true for characters with general categories "P" (Punctuation).this is a c/posix
 *          migration function. See the comments about c/posix character classification functions
 *          in the documentation at the top of this header file.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_punct        @c true if the code point is a punctuation character
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_is_punct(i18n_uchar32 c, i18n_ubool *is_punct);

/**
 * @brief Determines whether the specified code point is a "graphic" character (printable, excluding,
 *        spaces).
 * @details @c true for all characters except those with general categories "Cc" (control codes),
 *          "Cf" (format controls), "Cs" (surrogates), "Cn" (unassigned), and "Z" (separators). This
 *          is a c/posix migration function. See the comments about c/posix character classification
 *          functions in the documentation at the top of this header file.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_graph        @c true if the code point is a "graphic" character
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_is_graph(i18n_uchar32 c, i18n_ubool *is_graph);

/**
 * @brief Determines whether the specified code point is a "blank" or "horizontal space", a character
 *        that visibly separates words on a line.
 * @details The following are equivalent definitions: @c true for unicode white_space characters
 *          except for "vertical space controls" where "vertical space controls" are the following
 *          characters: u+000_a (Lf), u+000_b (Vt), u+000_c (Ff), u+000_d (Cr), u+0085 (Nel),
 *          u+2028 (Ls), u+2029 (Ps). Same as @c true for u+0009 (tab) and characters with general
 *          category "Zs" (space separators) except zero width space (zwsp, u+200_b). Note: There
 *          are several ICU whitespace functions; please see the utils_i18n_uchar.h file documentation
 *          for a detailed comparison. This is a c/posix migration function. See the comments about
 *          c/posix character classification functions in the documentation at the top of this header
 *          file.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_blank        @c true if the code point is a "blank"
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_is_blank(i18n_uchar32 c, i18n_ubool *is_blank);

/**
 * @brief Determines whether the specified code point is "defined", which usually means that it is
 *        assigned a character.
 * @details @c true for general categories other than "Cn" (other, not assigned), i.e. Note that
 *          non-character code points (e.g., u+fdd0) are not "defined" (they are Cn), but surrogate
 *          code points are "defined" (Cs). Same as java.lang.Character.isDefined().
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_defined      @c true if the code point is assigned a character
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_is_digit()
 * @see i18n_uchar_is_alpha()
 * @see i18n_uchar_is_alnum()
 * @see i18n_uchar_is_upper()
 * @see i18n_uchar_is_lower()
 * @see i18n_uchar_is_title()
 */
int i18n_uchar_is_defined(i18n_uchar32 c, i18n_ubool *is_defined);

/**
 * @brief Determines if the specified character is a space character or not.
 * @details Note: there are several icu whitespace functions; please see the utils_i18n_uchar.h
 *          file documentation for a detailed comparison.this is a c/posix migration function.
 *          See the comments about c/posix character classification functions in the documentation
 *          at the top of this header file.
 * @since_tizen 4.0
 *
 * @param[in]   c               The character to be tested
 * @param[out]  is_space        @c true if the character is a space character; @c false otherwise.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_is_java_space_char()
 * @see i18n_uchar_is_whitespace()
 * @see i18n_uchar_is_white_space()
 */
int i18n_uchar_is_space(i18n_uchar32 c, i18n_ubool *is_space);

/**
 * @brief Determines if the specified code point is a space character according to Java.
 * @details @c true for characters with general categories "z" (separators), which does not include
 *          control codes (e.g., tab or line feed). Same as java.lang.Character.isSpaceChar().
 *          Note: There are several icu whitespace functions; please see the utils_i18n_uchar.h
 *          file documentation for a detailed comparison.
 * @since_tizen 4.0
 *
 * @param[in]   c                   The code point to be tested
 * @param[out]  is_java_space_char  @c true if the code point is a space character according to
 *                                  Character.is_spaceChar()
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_is_space()
 * @see i18n_uchar_is_whitespace()
 * @see i18n_uchar_is_white_space()
 */
int i18n_uchar_is_java_space_char(i18n_uchar32 c, i18n_ubool *is_java_space_char);

/**
 * @brief Determines if the specified code point is a whitespace character according to Java/ICU.
 * @details A character is considered to be a Java whitespace character if and only if it satisfies
 *          one of the following criteria:\n
 *
 *          It is a Unicode Separator character (categories "Z" = "Zs" or "Zl" or "Zp"), but is not
 *          also a non-breaking space (U+00A0 NBSP or U+2007 Figure Space or U+202F Narrow NBSP).\n
 *          It is U+0009 HORIZONTAL TABULATION.\n
 *          It is U+000A LINE FEED.\n
 *          It is U+000B VERTICAL TABULATION.\n
 *          It is U+000C FORM FEED.\n
 *          It is U+000D CARRIAGE RETURN.\n
 *          It is U+001C FILE SEPARATOR.\n
 *          It is U+001D GROUP SEPARATOR.\n
 *          It is U+001E RECORD SEPARATOR.\n
 *          It is U+001F UNIT SEPARATOR.\n
 *
 *          This api tries to sync with the semantics of Java's java.lang.Character.isWhitespace(),
 *          but it may not return the exact same results because of the Unicode version difference.
 *          Note: unicode 4.0.1 changed u+200_b zero width space from a space separator (Zs) to
 *          a format control (Cf). Since then, i18n_uchar_is_whitespace(0x200b) returns @c false.
 *          See <a href="http://www.unicode.org/versions/Unicode4.0.1/">Unicode 4.0.1</a>
 *          Note: there are several icu whitespace functions. Please see the uchar.h file documentation
 *          for a detailed comparison.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_whitespace   @c true if the code point is a whitespace character according to
 *                              Java/ICU
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_is_space()
 * @see i18n_uchar_is_java_space_char()
 * @see i18n_uchar_is_white_space()
 */
int i18n_uchar_is_whitespace(i18n_uchar32 c, i18n_ubool *is_whitespace);

/**
 * @brief Determines whether the specified code point is a control character (as defined by this
 *        function).
 * @details A control character is one of the following:\n
 *
 *          ISO 8-bit control character (U+0000..U+001f and U+007f..U+009f)\n
 *          #I18N_UCHAR_U_CONTROL_CHAR (Cc)\n
 *          #I18N_UCHAR_U_FORMAT_CHAR (Cf)\n
 *          #I18N_UCHAR_U_LINE_SEPARATOR (Zl)\n
 *          #I18N_UCHAR_U_PARAGRAPH_SEPARATOR (Zp)\n
 *
 *          This is a c/posix migration function. See the comments about c/posix character
 *          classification functions in the documentation at the top of this header file.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_control      @c true if the code point is a control character
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #I18N_UCHAR_DEFAULT_IGNORABLE_CODE_POINT
 * @see i18n_uchar_is_printable()
 */
int i18n_uchar_is_control(i18n_uchar32 c, i18n_ubool *is_control);

/**
 * @brief Determines whether the specified code point is an ISO control code.
 * @details @c true for u+0000..u+001f and u+007f..u+009f (general category "cc"). Same as
 *          java.lang.Character.isISOControl().
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_iso_control  @c true if the code point is an ISO control code
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_is_control()
 */
int i18n_uchar_is_iso_control(i18n_uchar32 c, i18n_ubool *is_iso_control);

/**
 * @brief Determines whether the specified code point is a printable character.
 * @details @c true for general categories other than "C" (controls).this is a c/posix migration function.
 *          See the comments about c/posix character classification functions in the documentation
 *          at the top of this header file.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_printable    @c true if the code point is a printable character
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #I18N_UCHAR_DEFAULT_IGNORABLE_CODE_POINT
 * @see i18n_uchar_is_control()
 */
int i18n_uchar_is_printable(i18n_uchar32 c, i18n_ubool *is_printable);

/**
 * @brief Determines whether the specified code point is a base character.
 * @details @c true for general categories "L" (letters), "N" (numbers), "Mc" (spacing combining
 *          marks), and "Me" (enclosing marks). Note that this is different from the unicode definition
 *          in chapter 3.5, conformance clause D13, which defines base characters to be all characters
 *          (not Cn) that do not graphically combine with preceding characters (M) and that are
 *          neither control (Cc) or format (Cf) characters.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_base         @c true if the code point is a base character according to this
 *                              function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_is_alpha()
 * @see i18n_uchar_is_digit()
 */
int i18n_uchar_is_base(i18n_uchar32 c, i18n_ubool *is_base);

/**
 * @brief Returns the bidirectional category value for the code point.
 * @details Which is used in the unicode bidirectional algorithm (UAX #9
 *          <a href="http://www.unicode.org/reports/tr9/">Unicode reports</a>). Note that some
 *          unassigned code points have bidi values of R or AL because they are in blocks that
 *          are reserved for right-to-left scripts. Same as java.lang.Character.getDirectionality()
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  direction       The bidirectional category (#i18n_uchar_direction_e) value
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #i18n_uchar_direction_e
 */
int i18n_uchar_char_direction(i18n_uchar32 c, i18n_uchar_direction_e *direction);

/**
 * @brief Determines whether the code point has the Bidi_Mirrored property.
 * @details This property is set for characters that are commonly used in right-to-left contexts
 *          and need to be displayed with a "mirrored" glyph. Same as java.lang.Character.isMirrored().
 *          Same as #I18N_UCHAR_BIDI_MIRRORED.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_mirrored     @c true if the character has the Bidi_Mirrored property
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #I18N_UCHAR_BIDI_MIRRORED
 */
int i18n_uchar_is_mirrored(i18n_uchar32 c, i18n_ubool *is_mirrored);

/**
 * @brief Maps the specified character to a "mirror-image" character.
 * @details For characters with the bidi_mirrored property, implementations sometimes need
 *          a "poor man's" mapping to another unicode character (code point) such that
 *          the default glyph may serve as the mirror-image of the default glyph of the specified
 *          character. This is useful for text conversion to and from codepages with visual order,
 *          and for displays without glyph selection capabilities.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be mapped
 * @param[out]  char_mirror     Another Unicode code point that may serve as a mirror-image
 *                              substitute, or @a c itself if there is no such mapping or @a c
 *                              does not have the Bidi_Mirrored property
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #I18N_UCHAR_BIDI_MIRRORED
 * @see i18n_uchar_is_mirrored()
 */
int i18n_uchar_char_mirror(i18n_uchar32 c, i18n_uchar32 *char_mirror);

/**
 * @brief Maps the specified character to its paired bracket character.
 * @details For bidi_paired_bracket_type!=none, this is the same as u_char_mirror(). Otherwise @a c
 *          itself is returned. See <a href="http://www.unicode.org/reports/tr9/">Unicode report</a>
 * @since_tizen 4.0
 *
 * @param[in]   c                   The code point to be mapped
 * @param[out]  bidi_paired_bracket The paired bracket code point, or @a c itself if there is no
 *                                  such mapping (Bidi_Paired_Bracket_Type=None)
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #I18N_UCHAR_BIDI_PAIRED_BRACKET
 * @see #I18N_UCHAR_BIDI_PAIRED_BRACKET_TYPE
 * @see i18n_uchar_char_mirror()
 */
int i18n_uchar_get_bidi_paired_bracket(i18n_uchar32 c, i18n_uchar32 *bidi_paired_bracket);

/**
 * @brief Returns the general category value for the code point.
 * @details Same as java.lang.character.getType().
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  char_type       The general category (#i18n_uchar_category_e) value
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_category_e
 */
int i18n_uchar_char_type(i18n_uchar32 c, int8_t *char_type);

/**
 * @brief Enumerates efficiently all code points with their unicode general categories.
 * @details This is useful for building data structures (e.g., unicode_set's), for enumerating
 *          all assigned code points (<code> type != #I18N_UCHAR_U_UNASSIGNED</code>),
 *          etc.for each contiguous range of code points with a given general category
 *          ("character type"), the i18n_uchar_enum_char_type_cb() function is called.
 *          Adjacent ranges have different types. The unicode standard guarantees that the numeric
 *          value of the type is 0..31.
 * @since_tizen 4.0
 *
 * @param[in]   cb              A pointer to a function that is called for each contiguous
 *                              range of code points with the same general category
 * @param[in]   user_data         An opaque pointer that is passed on to the callback function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #i18n_uchar_enum_char_type_cb
 * @see i18n_uchar_category_e
 */
int i18n_uchar_enum_char_types(i18n_uchar_enum_char_type_cb cb,
							   const void *user_data);

/**
 * @brief Returns the combining class of the code point.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point of the character
 * @param[out]  combining_class The combining class of the character
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_get_combining_class(i18n_uchar32 c, uint8_t *combining_class);

/**
 * @brief Returns the decimal digit value of a decimal digit character.
 * @details Such characters have the general category "Nd" (decimal digit numbers) and
 *          a #i18n_uchar_u_numeric_type_e of #I18N_UCHAR_U_NT_DECIMAL. Unlike ICU releases
 *          before 2.6, no digit values are returned for any Han characters because Han number
 *          characters are often used with a special chinese-style number format (with characters
 *          for powers of 10 in between) instead of in decimal-positional notation. Unicode 4
 *          explicitly assigns Han number characters the #i18n_uchar_u_numeric_type_e
 *          #I18N_UCHAR_U_NT_NUMERIC instead of #I18N_UCHAR_U_NT_DECIMAL. See jitterbug 1483 for
 *          more details. Use i18n_uchar_get_int_property_value() and i18n_uchar_get_numeric_value()
 *          for complete numeric unicode properties.
 * @since_tizen 4.0
 *
 * @param[in]   c                   The code point for which to get the decimal digit value
 * @param[out]  char_digit_value    The decimal digit value of @a c, or @c -1 if @a c is not
 *                                  a decimal digit character
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_get_numeric_value()
 */
int i18n_uchar_char_digit_value(i18n_uchar32 c, int32_t *char_digit_value);

/**
 * @brief Retrieves the name of a unicode character.
 * @details Depending on name_choice, the character name written into the buffer is the "modern"
 *          name or the name that was defined in unicode version 1.0. The name contains only
 *          "invariant" characters like a-z, 0-9, space, and '-'. unicode 1.0 names are only
 *          retrieved if they are different from the modern names and if the data file contains
 *          the data for them.
 * @since_tizen 4.0
 *
 * @param[in]   code            The character (code point) for which to get the name. It must be
 *                              <code>0 <= @a code <= 0x10ffff</code>.
 * @param[in]   name_choice     Selector for which name to get.
 * @param[in]   buffer          Destination buffer for copying the name. The name will always be
 *                              zero-terminated. If there is no name, then the buffer will be set
 *                              to the empty string.
 * @param[in]   buffer_length   The length of the buffer
 * @param[out]  name_length     The length of the name, or 0 if there is no name for this character.
 *                              If the @a bufferLength is less than or equal to the length, then the
 *                              buffer contains the truncated name and the returned length indicates
 *                              the full length of the name. The length does not include the zero-termination.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_char_from_name()
 * @see i18n_uchar_enum_char_names()
 * @see i18n_uchar_u_char_name_choice_e
 */
int i18n_uchar_char_name(i18n_uchar32 code,
						 i18n_uchar_u_char_name_choice_e name_choice,
						 char *buffer,
						 int32_t buffer_length,
						 int32_t *name_length);

/**
 * @brief Finds a unicode character by its name and return its code point value.
 * @details The name is matched exactly and completely. If the name does not correspond to a code
 *          point, #I18N_ERROR_INVALID_CHAR_FOUND is returned. A unicode 1.0 name is matched only
 *          if it differs from the modern name. Unicode names are all uppercase. Extended names are
 *          lowercase followed by an uppercase hexadecimal number, and within angle brackets.
 * @since_tizen 4.0
 *
 * @param[in]   name_choice     Selector for which name to match
 * @param[in]   name            The name to match
 * @param[out]  char_from_name  The Unicode value of the code point with the given name, or an
 *                              undefined value if there is no such code point
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_enum_char_names()
 * @see i18n_uchar_char_name()
 * @see i18n_uchar_u_char_name_choice_e
 */
int i18n_uchar_char_from_name(i18n_uchar_u_char_name_choice_e name_choice,
							  const char *name,
							  i18n_uchar32 *char_from_name);

/**
 * @brief Enumerates all assigned unicode characters between the @a start and @a limit code points
 *        (@a start inclusive, @a limit exclusive) and call a function for each, passing the code
 *        point value and the character name.
 * @details For unicode 1.0 names, only those are enumerated that differ from the modern names.
 * @since_tizen 4.0
 *
 * @param[in]   start           The first code point in the enumeration range
 * @param[in]   limit           One more than the last code point in the enumeration range
 *                              (the first one after the range)
 * @param[in]   cb              The function that is to be called for each character name
 * @param[in]   user_data         An arbitrary pointer that is passed to the function
 * @param[in]   name_choice     Selector for which kind of names to enumerate
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_char_name()
 * @see #i18n_uchar_u_char_name_choice_e
 */
int i18n_uchar_enum_char_names(i18n_uchar32 start,
							   i18n_uchar32 limit,
							   i18n_uchar_enum_char_name_cb cb,
							   void *user_data,
							   i18n_uchar_u_char_name_choice_e name_choice);

/**
 * @brief Returns the unicode name for a given property, as given in the unicode database file.
 * @details In addition, this function maps the property #I18N_UCHAR_GENERAL_CATEGORY_MASK to the
 *          synthetic names "gcm" / "General_Category_Mask".
 * @since_tizen 4.0
 * @remarks The @a name should not be released.
 *
 * @param[in]   property        #18n_uchar_uproperty_e selector other than #I18N_UCHAR_INVALID_CODE.
 *                              If out of range, NULL is returned.
 * @param[in]   name_choice     Selector for which name to get. If out of range, NULL is returned.
 *                              All properties have a long name. Most have a short name, but some
 *                              do not. Unicode allows for additional names; if present these will
 *                              be returned by <code> #I18N_UCHAR_U_LONG_PROPERTY_NAME + i</code>,
 *                              where <code>i = 1, 2,..</code>.
 * @param[out]  name            A pointer to the name, or NULL if either the property or the
 *                              @a name_choice is out of range. If a given @a name_choice returns
 *                              NULL, then all larger values of @a name_choice will return NULL,
 *                              with one exception: if NULL is returned for #I18N_UCHAR_U_SHORT_PROPERTY_NAME,
 *                              then #I18N_UCHAR_U_LONG_PROPERTY_NAME (and higher) may still return
 *                              a non-NULL value. The returned pointer is valid until i18n_uclean_cleanup()
 *                              (not implemented) is called.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #i18n_uchar_uproperty_e
 * @see #i18n_uchar_u_property_name_choice_e
 */
int i18n_uchar_get_property_name(i18n_uchar_uproperty_e property,
								 i18n_uchar_u_property_name_choice_e name_choice,
								 const char **name);

/**
 * @brief Returns the #i18n_uchar_uproperty_e enum for a given property name, as specified in the
 *        unicode database.
 * @details Short, long, and any other variants are recognized. In addition, this function maps
 *          the synthetic names "gcm" / "General_Category_Mask" to the property #I18N_UCHAR_GENERAL_CATEGORY_MASK.
 * @since_tizen 4.0
 *
 * @param[in]   alias           The property name to be matched. The name is compared using
 *                              "loose matching"
 * @param[out]  property_enum   A #i18n_uchar_uproperty_e enum, or #I18N_UCHAR_INVALID_CODE if
 *                              the given name does not match any property
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #i18n_uchar_uproperty_e
 */
int i18n_uchar_get_property_enum(const char *alias, i18n_uchar_uproperty_e *property_enum);

/**
 * @brief Returns the unicode name for a given property value.
 * @details Note: some of the names can only be retrieved using #I18N_UCHAR_GENERAL_CATEGORY_MASK,
 *          not #I18N_UCHAR_GENERAL_CATEGORY. These include: "C" / "Other", "L" / "Letter", "Lc" /
 *          "Cased_Letter", "M" / "Mark", "N" / "Number", "P" / "Punctuation", "S" / "Symbol",
 *          and "Z" / "Separator".
 * @since_tizen 4.0
 * @remarks The @a name should not be released.
 *
 * @param[in]   property        #i18n_uchar_uproperty_e selector constant. Must be
 *                              <code>#I18N_UCHAR_BINARY_START <= @a property < #I18N_UCHAR_BINARY_LIMIT</code> or
 *                              <code>#I18N_UCHAR_INT_START <= @a property < #I18N_UCHAR_INT_LIMIT</code> or
 *                              <code>#I18N_UCHAR_MASK_START <= @a property < #I18N_UCHAR_MASK_LIMIT</code>.
 *                              If out of range, NULL is returned.
 * @param[in]   value           Selector for a value for the given property. If out of range, NULL
 *                              is returned. In general, valid values range from @c 0 up to some maximum.
 *                              There are a few exceptions:\n
 *                              #I18N_UCHAR_BLOCK values begin at the non-zero value
 *                              #I18N_UCHAR_UBLOCK_BASIC_LATIN.\n
 *                              #I18N_UCHAR_CANONICAL_COMBINING_CLASS values are not contiguous
 *                              and range from 0..240.\n
 *                              #I18N_UCHAR_GENERAL_CATEGORY_MASK values are not values of
 *                              #i18n_uchar_category_e, but rather mask values produced by
 *                              I18N_U_GET_GC_MASK().\n
 *                              This allows grouped categories such as [:L:] to be represented.
 *                              Mask values range non-contiguously from 1..I18N_U_GC_P_MASK.
 * @param[in]   name_choice     Selector for which name to get. If out of range, NULL is returned.
 *                              All values have a long name. Most have a short name, but some do not.
 *                              Unicode allows for additional names; if present these will be
 *                              returned by <code> #I18N_UCHAR_U_LONG_PROPERTY_NAME + i</code>,
 *                              where <code>i = 1, 2,..</code>.
 * @param[out]  name            A pointer to the name, or NULL if either the property or the
 *                              @a name_choice is out of range. If a given @a name_choice returns NULL,
 *                              then all larger values of @a name_choice will return NULL, with one
 *                              exception: if NULL is returned for #I18N_UCHAR_U_SHORT_PROPERTY_NAME,
 *                              then #I18N_UCHAR_U_LONG_PROPERTY_NAME (and higher) may still return
 *                              a non-NULL value. The returned pointer is valid until 18n_uclean_cleanup()
 *                              (not implemented) is called.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #i18n_uchar_u_property_name_choice_e
 * @see #i18n_uchar_uproperty_e
 */
int i18n_uchar_get_property_value_name(i18n_uchar_uproperty_e property,
									   int32_t value,
									   i18n_uchar_u_property_name_choice_e name_choice,
									   const char **name);

/**
 * @brief Returns the property value integer for a given value name, as specified in the unicode database.
 * @details Short, long, and any other variants are recognized. Some of the names will only be recognized
 *          with #I18N_UCHAR_GENERAL_CATEGORY_MASK, not #I18N_UCHAR_GENERAL_CATEGORY. these include: "C" /
 *          "Other", "L" / "Letter", "Lc" / "Cased_Letter", "M" / "Mark", "N" / "Number", "P" /
 *          "Punctuation", "S" / "Symbol", and "Z" / "Separator".
 * @since_tizen 4.0
 *
 * @param[in]   property        #i18n_uchar_uproperty_e selector constant. Must be
 *                              <code>#I18N_UCHAR_BINARY_START <= @a property < #I18N_UCHAR_BINARY_LIMIT</code> or
 *                              <code>#I18N_UCHAR_INT_START <= @a property < #I18N_UCHAR_INT_LIMIT</code> or
 *                              <code>#I18N_UCHAR_MASK_START <= @a property < #I18N_UCHAR_MASK_LIMIT</code>.
 *                              If out of range, #I18N_UCHAR_INVALID_CODE is returned.
 * @param[in]   alias           The value name to be matched. The name is compared using "loose matching"
 * @param[out]  value_enum      A value integer or #I18N_UCHAR_INVALID_CODE if the given name does not
 *                              match any value of the given property, or if the property is invalid.
 *                              Note: #I18N_UCHAR_GENERAL_CATEGORY_MASK values are not values of
 *                              #i18n_uchar_category_e, but rather mask values produced by #I18N_U_GET_GC_MASK().
 *                              This allows grouped categories such as [:L:] to be represented.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #i18n_uchar_uproperty_e
 */
int i18n_uchar_get_property_value_enum(i18n_uchar_uproperty_e property,
									   const char *alias,
									   int32_t *value_enum);

/**
 * @brief Determines if the specified character is permissible as the first character in an identifier
 *        according to unicode (the unicode standard, version 3.0, chapter 5.16 identifiers).
 * @details @c true for characters with general categories "L" (letters) and "Nl" (letter numbers).
 *          Same as java.lang.Character.isUnicodeIdentifierStart(). Same as #I18N_UCHAR_ID_START
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_id_start     @c true if the code point may start an identifier
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_is_alpha()
 * @see i18n_uchar_is_id_part()
 * @see #I18N_UCHAR_ID_START
 */
int i18n_uchar_is_id_start(i18n_uchar32 c, i18n_ubool *is_id_start);

/**
 * @brief Determines if the specified character is permissible in an identifier according to Java.
 * @details @c true for characters with general categories "L" (letters), "Nl" (letter numbers),
 *          "Nd" (decimal digits), "Mc" and "Mn" (combining marks), "Pc" (connecting punctuation),
 *          and i18n_uchar_is_id_ignorable().
 *          Same as java.lang.Character.isUnicodeIdentifierPart(). Almost the same as Unicode's
 *          ID_Continue (#I18N_UCHAR_ID_CONTINUE) except that unicode recommends to ignore Cf which
 *          is less than i18n_uchar_is_id_ignorable().
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_id_part      @c true if the code point may occur in an identifier according to Java
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_is_id_start()
 * @see i18n_uchar_is_id_ignorable()
 * @see #I18N_UCHAR_ID_CONTINUE
 */
int i18n_uchar_is_id_part(i18n_uchar32 c, i18n_ubool *is_id_part);

/**
 * @brief Determines if the specified character should be regarded as an ignorable character in an
 *        identifier, according to java.
 * @details @c true for characters with general category "Cf" (format controls) as well as
 *          non-whitespace ISO controls (u+0000..u+0008, u+000_e..u+001_b, u+007_f..u+009_f).
 *          Same as java.lang.Character.isIdentifierIgnorable(). Note that unicode just recommends
 *          to ignore Cf (format controls).
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_id_ignorable @c true if the code point is ignorable in identifiers according to Java
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #I18N_UCHAR_DEFAULT_IGNORABLE_CODE_POINT
 * @see i18n_uchar_is_id_start()
 * @see i18n_uchar_is_id_part()
 */
int i18n_uchar_is_id_ignorable(i18n_uchar32 c, i18n_ubool *is_id_ignorable);

/**
 * @brief Determines if the specified character is permissible as the first character in a java
 *        identifier.
 * @details In addition to i18n_uchar_is_id_start(), @c true for characters with general categories
 *          "Sc" (currency symbols) and "Pc" (connecting punctuation).
 *          Same as java.lang.Character.isJavaIdentifierStart().
 * @since_tizen 4.0
 *
 * @param[in]   c                   The code point to be tested
 * @param[out]  is_java_id_start    @c true if the code point may start a Java identifier
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_is_java_id_part()
 * @see i18n_uchar_is_alpha()
 * @see i18n_uchar_is_id_start()
 */
int i18n_uchar_is_java_id_start(i18n_uchar32 c, i18n_ubool *is_java_id_start);

/**
 * @brief Determines if the specified character is permissible in a java identifier.
 * @details In addition to i18n_uchar_is_id_part(), @c true for characters with general category
 *          "Sc" (currency symbols).
 *          Same as java.lang.Character.isJavaIdentifierPart().
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be tested
 * @param[out]  is_java_id_part @c true if the code point may occur in a Java identifier
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_is_id_ignorable()
 * @see i18n_uchar_is_java_id_start()
 * @see i18n_uchar_is_alpha()
 * @see i18n_uchar_is_digit()
 * @see i18n_uchar_is_id_part()
 */
int i18n_uchar_is_java_id_part(i18n_uchar32 c, i18n_ubool *is_java_id_part);

/**
 * @brief Maps the given character to its lowercase equivalent.
 * @details If the character has no lowercase equivalent, the character itself is returned. This
 *          function only returns the simple, single-code point case mapping. Full case mappings
 *          should be used whenever possible because they produce better results by working on whole
 *          strings. They take into account the string context and the language and can map to a
 *          result string with a different length as appropriate. Full case mappings are applied by
 *          the string case mapping functions, see utils_i18n_ustring.h.
 *          See also the user guide chapter on c/posix migration:
 *          <a href="http://icu-project.org/userguide/posix.html#casemappings">User Guide: Case Mappings</a>
 *          Same as java.lang.Character.toLowerCase().
 * @since_tizen 4.0

 * @param[in]   c               The code point to be mapped
 * @param[out]  lower           The #I18N_UCHAR_SIMPLE_LOWERCASE_MAPPING of the code point, if any;
 *                              otherwise the code point itself.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_to_lower(i18n_uchar32 c, i18n_uchar32 *lower);

/**
 * @brief Maps the given character to its uppercase equivalent.
 * @details If the character has no uppercase equivalent, the character itself is returned. This
 *          function only returns the simple, single-code point case mapping. Full case mappings
 *          should be used whenever possible because they produce better results by working on whole
 *          strings. They take into account the string context and the language and can map to
 *          a result string with a different length as appropriate. Full case mappings are applied
 *          by the string case mapping functions, see utils_i18n_ustring.h.
 *          See also the user guide chapter on c/posix migration:
 *          <a href="http://icu-project.org/userguide/posix.html#casemappings">User Guide: Case Mappings</a>
 *          Same as java.lang.Character.toUpperCase().
 * @since_tizen 4.0

 * @param[in]   c               The code point to be mapped
 * @param[out]  upper           The #I18N_UCHAR_SIMPLE_UPPERCASE_MAPPING of the code point, if any;
 *                              otherwise the code point itself.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_to_upper(i18n_uchar32 c, i18n_uchar32 *upper);

/**
 * @brief Maps the given character to its titlecase equivalent.
 * @details If none is defined, the character itself is returned. This function only returns
 *          the simple, single-code point case mapping. Full case mappings should be used whenever
 *          possible because they produce better results by working on whole strings. They take
 *          into account the string context and the language and can map to a result string with
 *          a different length as appropriate. Full case mappings are applied by the string case
 *          mapping functions, see utils_i18n_ustring.h. See also the user guide chapter on c/posix
 *          migration:
 *          <a href="http://icu-project.org/userguide/posix.html#casemappings">User Guide: Case Mappings</a>
 *          Same as java.lang.Character.toTitleCase().
 * @details
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be mapped
 * @param[out]  title           The #I18N_UCHAR_SIMPLE_TITLECASE_MAPPING of the code point, if any;
 *                              otherwise the code point itself.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_to_title(i18n_uchar32 c, i18n_uchar32 *title);

/**
 * @brief Maps the given character to its case folding equivalent.
 * @details If the character has no case folding equivalent, the character itself is returned.
 *          This function only returns the simple, single-code point case mapping. Full case
 *          mappings should be used whenever possible because they produce better results by
 *          working on whole strings. They take into account the string context and the language
 *          and can map to a result string with a different length as appropriate. Full case
 *          mappings are applied by the string case mapping functions, see utils_i18n_ustring.h.
 *          See also the user guide chapter on c/posix migration:
 *          <a href="http://icu-project.org/userguide/posix.html#casemappings">User Guide: Case Mappings</a>
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point to be mapped
 * @param[in]   options         Either #I18N_U_FOLD_CASE_DEFAULT or #I18N_U_FOLD_CASE_EXCLUDE_SPECIAL_I
 * @param[out]  code_point      The #I18N_UCHAR_SIMPLE_CASE_FOLDING of the code point, if any; otherwise
 *                              the code point itself.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_fold_case(i18n_uchar32 c, uint32_t options, i18n_uchar32 *code_point);

/**
 * @brief Returns the decimal digit value of the code point in the specified radix.
 * @details If the radix is not in the range <code> @c 2<= @a radix <= @c 36 </code> or if the
 *          value of @a c is not a valid digit in the specified radix, @c -1 is returned.
 *          A character is a valid digit if at least one of the following is true:\n
 *
 *          The character has a decimal digit value. Such characters have the general category "Nd"
 *          (decimal digit numbers) and a #I18N_UCHAR_NUMERIC_TYPE of #I18N_UCHAR_U_NT_DECIMAL.
 *          In this case the value is the character's decimal digit value.\n
 *          The character is one of the uppercase Latin letters @c 'A' through @c 'Z'. In this case
 *          the value is <code> @a c - @c 'A' + @c 10</code>.\n
 *          The character is one of the lowercase Latin letters @c 'a' through @c 'z'. In this case
 *          the value is <code> @a ch - @c 'a' + @c 10</code>.\n
 *          Latin letters from both the ASCII range (0061..007A, 0041..005A) as well as from
 *          the Fullwidth ASCII range (FF41..FF5A, FF21..FF3A) are recognized.\n
 *
 *          Same as java.lang.Character.digit().
 * @since_tizen 4.0

 * @param[in]   ch              The code point to be tested
 * @param[in]   radix           The radix
 * @param[out]  numerical_value The numeric value represented by the character in the specified
 *                              @a radix, or @c -1 if there is no value or if the value exceeds
 *                              the @a radix
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #I18N_UCHAR_NUMERIC_TYPE
 * @see i18n_uchar_for_digit()
 * @see i18n_uchar_char_digit_value()
 * @see i18n_uchar_is_digit()
 */
int i18n_uchar_digit(i18n_uchar32 ch, int8_t radix, int32_t *numerical_value);

/**
 * @brief Determines the character representation for a specific digit in the specified radix.
 * @details If the value of radix is not a valid radix, or the value of digit is not a valid digit
 *          in the specified radix, the null character (@c u+0000) is returned. The radix argument
 *          is valid if it is greater than or equal to @c 2 and less than or equal to @c 36.
 *          The digit argument is valid if <code> @c 0 <= @a digit < @a radix</code>. If the digit
 *          is less than @c 10, then <code> @c '0' + @a digit</code> is returned.
 *          Otherwise, the value <code> @c 'a' + @a digit - @c 10</code> is returned.
 *          Same as java.lang.Character.forDigit().
 * @since_tizen 4.0
 *
 * @param[in]   digit           The number to convert to a character
 * @param[in]   radix           The radix
 * @param[out]  representation  The char representation of the specified digit in the specified radix
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uchar_digit()
 * @see i18n_uchar_char_digit_value()
 * @see i18n_uchar_is_digit()
 */
int i18n_uchar_for_digit(int32_t digit, int8_t radix, i18n_uchar32 *representation);

/**
 * @brief Gets the "age" of the code point.
 * @details The "age" is the Unicode version when the code point was first designated
 *          (as a non-character or for private use) or assigned a character. This can be useful
 *          to avoid emitting code points to receiving processes that do not accept newer characters.
 * @since_tizen 4.0
 *
 * @param[in]   c               The code point
 * @param[in]   version_array   The Unicode version number array, to be filled in
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_char_age(i18n_uchar32 c, i18n_uversion_info version_array);

/**
 * @brief Gets the unicode version information.
 * @details The version array is filled in with the version information for the unicode standard
 *          that is currently used by icu. for example, unicode version 3.1.1 is represented as
 *          an array with the values { 3, 1, 1, 0 }.
 * @since_tizen 4.0

 * @param[in]   version_array   An output array that will be filled in with the Unicode version number

 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_get_unicode_version(i18n_uversion_info version_array);

/**
 * @brief Gets the FC_NFKC_Closure property string for a character.
 * @details See unicode standard annex #15 for details, search for "FC_NFKC_Closure" or for "FNC":
 *          <a href="http://www.unicode.org/reports/tr15/"> Unicode reports </a>.
 * @since_tizen 4.0
 *
 * @param[in]   c               The character (code point) for which to get the FC_NFKC_Closure
 *                              string. It must be <code>@c 0 <= @a c <= @c 0x10ffff</code>.
 * @param[in]   dest            The destination buffer for copying the string. The string will be
 *                              zero-terminated if possible. If there is no FC_NFKC_Closure string,
 *                              then the buffer will be set to the empty string.
 * @param[in]   dest_capacity   The capacity of the destination buffer
 * @param[out]  length          The length of the string, or 0 if there is no FC_NFKC_Closure string
 *                              for this character. If the @a dest_capacity is less than or equal to
 *                              the @a length, then the buffer contains the truncated name and the
 *                              @a length indicates the full length of the name. The length does not
 *                              include the zero-termination.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uchar_get_fc_nfkc_closure(i18n_uchar32 c,
								   i18n_uchar *dest,
								   int32_t dest_capacity,
								   int32_t *length);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
