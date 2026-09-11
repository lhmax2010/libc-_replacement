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
 * @file utils_i18n_ucnv.h
 * @version 0.1
 * @brief utils_i18n_ucnv
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UCNV_MODULE Ucnv
 * @brief Character Conversion C API.
 * @section CAPI_BASE_UTILS_I18N_UCNV_MODULE_HEADER Required Header
 *  \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UCNV_MODULE_OVERVIEW Overview
 * @details This function is used to convert codepage or character encoded data to and from UTF-16.
 *          You can create a converter with i18n_ucnv_create().
 *          With that converter, you can get its properties, set options, convert your data and destroy the converter.
 *
 *          Since many software programs recognize different converter names for different types of converters,
 *          there are other functions in this function to iterate over the converter aliases.
 *          The functions i18n_ucnv_get_available_name(), i18n_ucnv_get_alias() and i18n_ucnv_get_standard_name()
 *          are some of the more frequently used alias functions to get this information.
 *
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UCNV_MODULE
 * @{
 */

/**
 * @brief   Calculates the size of a buffer for conversion from Unicode to a charset.
 * @details The calculated size is guaranteed to be sufficient for this conversion.
 *
 *          It takes into account initial and final non-character bytes that are output by some converters.\n
 * @since_tizen 6.0
 * @param[in] length        The number of #i18n_uchar characters to be converted.
 * @param[in] max_char_size Return value from i18n_ucnv_get_max_char_size() for the converter that will be used.
 * @return                  Size (in bytes) of a buffer that will be large enough to hold the output bytes of converting length #i18n_uchar with the converter that returned the @a max_char_size.
 * @see i18n_ucnv_get_max_char_size()
 *
 */
int i18n_ucnv_get_max_bytes_for_string(int32_t length, int32_t max_char_size);

/**
 * @brief   Does a fuzzy compare of two converter/alias names.
 * @details The comparison is case-insensitive, ignores leading zeroes if they are not followed by further digits, and ignores all but letters and digits.
           Thus the strings "UTF-8", "utf_8", "u*T@f08" and "Utf 8" are exactly equivalent.
           See section 1.4, Charset Alias Matching in Unicode Technical Standard #22 at http://www.unicode.org/reports/tr22/.
 * @since_tizen 6.0
 * @param[in] name1  A converter name or alias, zero-terminated.
 * @param[in] name2  A converter name or alias, zero-terminated.
 * @return @c 0 if the names match, or a negative value if the name1 lexically precedes name2, or a positive value if the name1 lexically follows name2.
 * @exception #I18N_ERROR_NONE               Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 */
int i18n_ucnv_compare_names(const char *name1, const char *name2);

/**
 * @brief   Creates an #i18n_ucnv_h with the name of a coded character set specified as a C string.
 * @details The actual name will be resolved with the alias file using a case-insensitive string comparison that ignores leading zeroes and all non-alphanumeric characters.
 *          E.g., the names "UTF8", "utf-8", "u*T@f08" and "Utf 8" are all equivalent. (See also i18n_ucnv_compare_names().)
 *          If NULL is passed for the converter name, it will create one with the i18n_ucnv_get_default_name() return value.
 *
 *          A converter name for ICU 1.5 and above may contain options like a locale specification to control the specific behavior of the newly instantiated converter.
 *          The meaning of the options depends on the particular converter. If an option is not defined for or recognized by a given converter, then it is ignored.
 *
 *          Options are appended to the converter name string, with a "," between the name and the first option and also between adjacent options.
 *
 *
 *          The conversion behavior and names can vary between platforms. ICU may convert some characters differently from other platforms.
 * @since_tizen 6.0
 * @remarks The @a converter should be released using i18n_ucnv_destroy().
 * @param[in] converter_name    Name of the coded character set table. This may have options appended to the string.
 *                              IANA alias character set names, IBM CCSIDs starting with "ibm-", Windows codepage numbers starting with "windows-" are frequently used for this parameter.
 *                              See i18n_ucnv_get_available_name() and i18n_ucnv_get_alias() for a complete list that is available.
 *                              If this parameter is NULL, the default converter will be used.
 * @param[out] converter        The created #i18n_ucnv_h, or NULL if an error occurred.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_FILE_ACCESS        The requested file cannot be found
 * @retval #I18N_ERROR_OUT_OF_MEMORY      Out of memory
 * @see i18n_ucnv_create_unicode()
 * @see i18n_ucnv_create_ccsid()
 * @see i18n_ucnv_get_available_name()
 * @see i18n_ucnv_get_alias()
 * @see i18n_ucnv_get_default_name()
 * @see i18n_ucnv_destroy()
 * @see i18n_ucnv_compare_names()
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_ucnv_h converter;
 * 	i18n_error_code_e error_code;
 * 	error_code = i18n_ucnv_create("ibm-949", &converter);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucnv_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_ucnv_destroy(converter);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucnv_create(const char *converter_name, i18n_ucnv_h *converter);

/**
 * @brief   Creates a Unicode converter with the names specified as unicode string.
 * @details The name should be limited to the ASCII-7 alphanumerics range.
 *          The actual name will be resolved with the alias file using a case-insensitive string comparison that ignores leading zeroes and all non-alphanumeric characters.
 *          E.g., the names "UTF8", "utf-8", "u*T@f08" and "Utf 8" are all equivalent. (See also i18n_ucnv_compare_names().)
 *          If NULL is passed for the converter name, it will create one with the i18n_ucnv_get_default_name() return value.
 *
 *          See i18n_ucnv_create() for the complete details.
 * @since_tizen 6.0
 * @remarks The @a converter should be released using i18n_ucnv_destroy().
 * @param[in]  name         Name of the #i18n_ucnv_h table in a zero terminated Unicode string.
 * @param[out] converter    The created #i18n_ucnv_h, or NULL if an error occurred.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #I18N_ERROR_FILE_ACCESS        Requested file cannot be found
 * @see i18n_ucnv_create()
 * @see i18n_ucnv_create_ccsid()
 * @see i18n_ucnv_get_available_name()
 * @see i18n_ucnv_destroy()
 * @see i18n_ucnv_compare_names()
 *
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_ucnv_h converter;
 * 	i18n_error_code_e error_code;
 * 	i18n_uchar name[] = {0x0069, 0x0062, 0x006d, 0x002d, 0x0039, 0x0034, 0x0033, 0x0000};
 * 	error_code = i18n_ucnv_create_unicode(name, &converter);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucnv_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_ucnv_destroy(converter);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucnv_create_unicode(const i18n_uchar *name, i18n_ucnv_h *converter);

/**
 * @brief   Creates an #i18n_ucnv_h from a CCSID number and platform pair.
 * @details Note that the usefulness of this function is limited to platforms with numeric encoding IDs. Only IBM and Microsoft platforms use numeric (16-bit) identifiers for encodings.
 *
 *          In addition, IBM CCSIDs and Unicode conversion tables are not 1:1 related.
 *          For many IBM CCSIDs there are multiple (up to six) Unicode conversion tables, and for some Unicode conversion tables there are multiple CCSIDs.
 *          Some "alternate" Unicode conversion tables are provided by the IBM CDRA conversion table registry.
 *          The most prominent example of a systematic modification of conversion tables that is not provided in the form of conversion table files in the repository is that
 *          S/390 Unix System Services swaps the codes for Line Feed and New Line in all EBCDIC codepages, which requires such a swap in the Unicode conversion tables as well.
 *
 *          Only IBM default conversion tables are accessible with i18n_ucnv_create_ccsid(). i18n_ucnv_get_ccsid() will return the same CCSID
 *          for all conversion tables that are associated with that CCSID.
 *
 *          In summary, the use of CCSIDs and the associated function functions is not recommended.
 *
 *          In order to create a converter with the default IBM CDRA Unicode conversion table, you can use this function or use the prefix "ibm-":
 * @since_tizen 6.0
 * @remarks The @a converter should be released using i18n_ucnv_destroy().
 * @param[in] codepage      Codepage number to create
 * @param[in] platform      The platform in which the codepage number exists
 * @param[out] converter    The created #i18n_ucnv_h, or NULL if an error occurred.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #I18N_ERROR_FILE_ACCESS        Requested file cannot be found
 * @see i18n_ucnv_create()
 * @see i18n_ucnv_create_unicode()
 * @see i18n_ucnv_get_ccsid()
 * @see i18n_ucnv_get_platform()
 *
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_ucnv_h converter;
 * 	i18n_error_code_e error_code;
 * 	int32_t ccsids[] = {37, 850, 943, 949, 950, 1047, 1252, 1392, 33722};
 *
 * 	for (int i = 0; i < 9; ++i)
 * 	{
 * 		error_code = i18n_ucnv_create_ccsid(ccsids[i], I18N_UCNV_IBM, &converter);
 * 		if (error_code != I18N_ERROR_NONE) {
 * 			printf("Error i18n_ucnv_create_ccsid: %d\n", error_code);
 * 			return EXIT_FAILURE;
 * 		}
 * 		i18n_ucnv_destroy(converter);
 * 	}
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucnv_create_ccsid(int32_t codepage, i18n_ucnv_platform_e platform, i18n_ucnv_h *converter);

/**
 * @brief   Creates an #i18n_ucnv_h specified from a @a package_name and a @a converter_name.
 * @details The @a package_name and @a converter_name must point to an ICU udata object.
 *          Typically, @a package_name will refer to a (.dat) file. Using a full file or directory pathname for @a package_name is deprecated.
 *
 *          The name will NOT be looked up in the alias mechanism, nor will the converter be stored in the converter cache or the alias table.
 *          The only way to create further converters is call this function multiple times, or use the i18n_ucnv_safe_clone() function to clone a 'master' converter.
 *
 *          A future version may add alias table lookups and/or caching to this function.
 * @since_tizen 6.0
 * @remarks The @a converter should be released using i18n_ucnv_destroy().
 * @param[in] package_name      Name of the package.
 * @param[in] converter_name    Name of the data item to be used, without suffix.
 * @param[out] converter        The created #i18n_ucnv_h, or NULL if an error occurred.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #I18N_ERROR_FILE_ACCESS        Requested file cannot be found
 * @see i18n_ucnv_create()
 * @see i18n_ucnv_safe_clone()
 *
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_ucnv_h converter;
 * 	i18n_error_code_e error_code;
 * 	error_code = i18n_ucnv_create_package("", "", &converter);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucnv_create_package: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_ucnv_destroy(converter);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucnv_create_package(const char *package_name, const char *converter_name, i18n_ucnv_h *converter);

/**
 * @brief   Clones converter thread safely.
 * @since_tizen 6.0
 * @remarks The @a cloned_converter should be released using i18n_ucnv_destroy().
 * @param[in] converter           Converter to be cloned.
 * @param[out] cloned_converter   The created #i18n_ucnv_h, or NULL if an error occurred.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_ucnv_h converter;
 * 	i18n_ucnv_h cloned_converter;
 * 	i18n_error_code_e error_code;
 * 	error_code = i18n_ucnv_create("iso-8859-3", &converter);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucnv_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	error_code = i18n_ucnv_safe_clone(converter, &cloned_converter);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucnv_safe_clone: %d\n", error_code);
 * 		i18n_ucnv_destroy(converter);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_ucnv_destroy(cloned_converter);
 * 	i18n_ucnv_destroy(converter);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucnv_safe_clone(i18n_ucnv_h converter, i18n_ucnv_h *cloned_converter);

/**
 * @brief   Deletes the unicode converter and releases resources associated with just this instance.
 * @details Does not free up shared converter tables.
 * @since_tizen 6.0
 * @param[in] converter    The converter object to be deleted.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 */
int i18n_ucnv_destroy(i18n_ucnv_h converter);

/**
 * @brief   Fills in the output parameter, @a sub_chars, with the substitution characters as multiple bytes.
 * @details If i18n_ucnv_set_subst_chars() set a Unicode string because the converter is stateful, then @a sub_chars will be an empty string.
 * @since_tizen 6.0
 * @param[in]     converter An #i18n_ucnv_h
 * @param[out]     sub_chars The substitution characters
 * @param[in,out] len       On input the capacity of @a sub_chars, on output the number of bytes copied to it
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_INDEX_OUTOFBOUNDS  Trying to access the index that is out of bounds
 * @see i18n_ucnv_set_subst_chars()
 * @see i18n_ucnv_set_subst_string()
 *
 */
int i18n_ucnv_get_subst_chars(i18n_ucnv_h converter, char *sub_chars, int8_t *len);

/**
 * @brief   Sets the substitution chars when converting from unicode to a codepage.
 * @details The substitution is specified as a string of 1-4 bytes, and may contain NULL bytes.
 *          The @a sub_chars must represent a single character.
 *          The caller needs to know the byte sequence of a valid character in the converter's charset.
 *          For some converters, for example some ISO 2022 variants, only single-byte substitution characters may be supported.
 *          The newer i18n_ucnv_set_subst_string() function relaxes these limitations.
 * @since_tizen 6.0
 * @param[in] converter An #i18n_ucnv_h
 * @param[in] sub_chars The substitution character byte sequence we want set.
 * @param[in] len       The number of bytes in @a sub_chars
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_INDEX_OUTOFBOUNDS  Trying to access the index that is out of bounds
 * @see i18n_ucnv_get_subst_chars()
 * @see i18n_ucnv_set_subst_string()
 *
 */
int i18n_ucnv_set_subst_chars(i18n_ucnv_h converter, const char *sub_chars, int8_t len);

/**
 * @brief   Sets a substitution string for converting from Unicode to a charset.
 * @details The caller need not know the charset byte sequence for each charset.
 *
 *          Unlike i18n_ucnv_set_subst_chars() which is designed to set a charset byte sequence for a single character,
 *          this function takes a Unicode string with zero, one or more characters,
 *          and immediately verifies that the string can be converted to the charset.
 *          If not, or if the result is too long (more than 32 bytes), then the function returns with an error accordingly.
 *
 *          Also unlike i18n_ucnv_set_subst_chars(), this function works for stateful charsets
 *          by converting on the fly at the point of substitution rather than setting a fixed byte sequence.
 * @since_tizen 6.0
 * @param[in] converter An #i18n_ucnv_h
 * @param[in] s         The Unicode string.
 * @param[in] len       The number of #i18n_uchar characters in s, or -1 for a NUL-terminated string.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @see i18n_ucnv_get_subst_chars()
 * @see i18n_ucnv_set_subst_chars()
 *
 */
int i18n_ucnv_set_subst_string(i18n_ucnv_h converter, const i18n_uchar *s, int32_t len);

/**
 * @brief   Fills in the output parameter, @a err_bytes, with the error characters from the last failing conversion.
 * @since_tizen 6.0
 * @param[in]     converter An #i18n_ucnv_h
 * @param[in]     err_bytes The codepage bytes which were in error.
 * @param[in,out] len       Input the capacity of @a err_bytes, on output the number of bytes which were copied to it.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_INDEX_OUTOFBOUNDS  Trying to access the index that is out of bounds
 *
 */
int i18n_ucnv_get_invalid_chars(i18n_ucnv_h converter, char *err_bytes, int8_t *len);

/**
 * @brief   Fills in the output parameter, @a err_uchars, with the error characters from the last failing conversion.
 * @since_tizen 6.0
 * @param[in]     converter     An #i18n_ucnv_h
 * @param[in]     err_uchars    The #i18n_uchar which were in error.
 * @param[in,out] len           Input the capacity of @a err_uchars, on output the number of #i18n_uchar characters which were copied to it.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_INDEX_OUTOFBOUNDS  Trying to access the index that is out of bounds
 *
 */
int i18n_ucnv_get_invalid_uchars(i18n_ucnv_h converter, i18n_uchar *err_uchars, int8_t *len);

/**
 * @brief   Resets the state of a converter to the default state.
 * @details This is used in the case of an error, to restart a conversion from a known default state. It will also empty the internal output buffers.
 * @since_tizen 6.0
 * @param[in] converter     An #i18n_ucnv_h
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 */
int i18n_ucnv_reset(i18n_ucnv_h converter);

/**
 * @brief   Resets the to-Unicode part of a converter state to the default state.
 * @details This is used in the case of an error to restart a conversion to Unicode to a known default state.
 *          It will also empty the internal output buffers used for the conversion to Unicode codepoints.
 * @since_tizen 6.0
 * @param[in] converter     An #i18n_ucnv_h
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 */
int i18n_ucnv_reset_to_unicode(i18n_ucnv_h converter);

/**
 * @brief   Resets the from-Unicode part of a converter state to the default state.
 * @details This is used in the case of an error to restart a conversion from Unicode to a known default state.
 *          It will also empty the internal output buffers used for the conversion from Unicode codepoints.
 * @since_tizen 6.0
 * @param[in] converter     An #i18n_ucnv_h
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 */
int i18n_ucnv_reset_from_unicode(i18n_ucnv_h converter);

/**
 * @brief   Gets the maximum number of bytes that are output per #i18n_uchar in conversion from Unicode using this converter.
 * @details The returned number can be used with i18n_ucnv_get_max_bytes_for_string() to calculate the size of a target buffer for conversion from Unicode.
 *
 *          Note: Before ICU 2.8, this function did not return reliable numbers for some stateful converters (EBCDIC_STATEFUL, ISO-2022) and LMBCS.
 *
 *          This number may not be the same as the maximum number of bytes per "conversion unit".
 *          In other words, it may not be the intuitively expected number of bytes per character that would be published for a charset,
 *          and may not fulfill any other purpose than the allocation of an output buffer of guaranteed sufficient size for a given input length and converter.
 *
 *          Examples for special cases that are taken into account:
 *
 *          - Supplementary code points may convert to more bytes than BMP code points.
 *          This function returns bytes per #i18n_uchar (UTF-16 code unit), not per Unicode code point, for efficient buffer allocation.\n
 *          - State-shifting output (SI/SO, escapes, etc.) from stateful converters.
 *          - When m input #i18n_uchar are converted to n output bytes, then the maximum m/n is taken into account.
 *
 *          The number returned here does not take into account (see i18n_ucnv_get_max_bytes_for_string()):
 *
 *          - initial and final non-character bytes that are output by some converters (automatic BOMs, initial escape sequence, final SI, etc.)
 *
 *          Examples for returned values:
 *
 *          - SBCS charsets: 1
 *          - Shift-JIS: 2
 *          - UTF-16: 2 (2 per BMP, 4 per surrogate pair, BOM not counted)
 *          - UTF-8: 3 (3 per BMP, 4 per surrogate pair)]n
 *          - EBCDIC_STATEFUL (EBCDIC mixed SBCS/DBCS): 3 (SO + DBCS)
 *          - ISO-2022: 3 (always outputs UTF-8)
 *          - ISO-2022-JP: 6 (4-byte escape sequences + DBCS)
 *          - ISO-2022-CN: 8 (4-byte designator sequences + 2-byte SS2/SS3 + DBCS)
 * @since_tizen 6.0
 * @param[in] converter     An #i18n_ucnv_h
 * @param[out] size         The maximum number of bytes per #i18n_uchar (16 bit code unit) that are output by
 *                          i18n_ucnv_from_unicode(), to be used together with i18n_ucnv_get_max_bytes_for_string() for buffer allocation.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @see i18n_ucnv_get_max_bytes_for_string()
 * @see i18n_ucnv_get_min_char_size()
 *
 */
int i18n_ucnv_get_max_char_size(i18n_ucnv_h converter, int8_t *size);

/**
 * @brief   Gets the minimum byte length (per codepoint) for characters in this codepage.
 * @details This is usually either 1 or 2.
 * @since_tizen 6.0
 * @param[in] converter     An #i18n_ucnv_h
 * @param[out] size         The minimum number of bytes per codepoint allowed by this particular converter.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @see i18n_ucnv_get_max_char_size()
 *
 */
int i18n_ucnv_get_min_char_size(i18n_ucnv_h converter, int8_t *size);

/**
 * @brief   Gets the display name of the converter passed in based on the Locale passed in.
 * @details If the locale contains no display name, the internal ASCII name will be filled in.
 * @since_tizen 6.0
 * @param[in] converter             An #i18n_ucnv_h
 * @param[in] display_locale        The specific Locale we want to localized for.
 * @param[in] display_name          User provided buffer to be filled in.
 * @param[in] display_name_capacity Size of @a display_name Buffer.
 * @param[out] display_name_len     The number of #i18n_uchar characters needed in @a display_name.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @see i18n_ucnv_get_name()
 *
 */
int i18n_ucnv_get_display_name(i18n_ucnv_h converter, const char *display_locale, i18n_uchar *display_name, int32_t display_name_capacity, int32_t *display_name_len);

/**
 * @brief   Gets the internal, canonical name of the converter (zero-terminated).
 * @details The lifetime of the returned string will be that of the converter passed to this function.
 * @since_tizen 6.0
 * @remarks The returned @a name should be freed by the caller with free() function.
 * @param[in] converter An #i18n_ucnv_h
 * @param[out] name     The internal name of the converter
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @see i18n_ucnv_get_display_name()
 *
 */
int i18n_ucnv_get_name(i18n_ucnv_h converter, const char **name);

/**
 * @brief   Gets a codepage number associated with the converter.
 * @details This is not guaranteed to be the one used to create the converter.
 *          Some converters do not represent platform registered codepages and return zero for the codepage number.
 *          The error code fill-in parameter indicates if the codepage number is available.
 *          Does not check if the converter is NULL or if converter's data table is NULL.
 *
 *          Important: The use of CCSIDs is not recommended
 *          because it is limited to only two platforms in principle and only one (UCNV_IBM) in the current ICU converter API.
 *          Also, CCSIDs are insufficient to identify IBM Unicode conversion tables precisely. For more details see i18n_ucnv_create_ccsid().
 * @since_tizen 6.0
 * @param[in] converter        An #i18n_ucnv_h
 * @param[out] codepage_number If any error occurs, -1 will get otherwise, the codepage number will be returned
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @see i18n_ucnv_create_ccsid()
 * @see i18n_ucnv_get_platform()
 *
 */
int i18n_ucnv_get_ccsid(i18n_ucnv_h converter, int32_t *codepage_number);

/**
 * @brief   Gets a codepage platform associated with the converter.
 * @details Currently, only #I18N_UCNV_IBM will be returned. Does not test if the converter is NULL or if converter's data table is NULL.
 * @since_tizen 6.0
 * @param[in] converter    An #i18n_ucnv_h
 * @param[out] platform    The codepage platform
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 */
int i18n_ucnv_get_platform(i18n_ucnv_h converter, i18n_ucnv_platform_e *platform);

/**
 * @brief   Gets the type of the converter e.g.
 *          SBCS, MBCS, DBCS, UTF8, UTF16_BE, UTF16_LE, ISO_2022, EBCDIC_STATEFUL, LATIN_1.
 * @since_tizen 6.0
 * @param[in] converter    An #i18n_ucnv_h
 * @param[out] type        The type of the converter
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 */
int i18n_ucnv_get_type(i18n_ucnv_h converter, i18n_ucnv_type_e* type);

/**
 * @brief   Gets the "starter" (lead) bytes for converters of type MBCS.
 * @details Will fill in an #I18N_ERROR_INVALID_PARAMETER if converter passed in is not MBCS.
 *          Fills in an array of type #i18n_ubool, with the value of the byte as offset to the array.
 *          For example, if (starters[0x20] == TRUE) at return, it means that the byte 0x20 is a starter byte in this converter.
 *          Context pointers are always owned by the caller.
 * @since_tizen 6.0
 * @param[in] converter      An #i18n_ucnv_h of type MBCS
 * @param[in,out] starters   An array of size 256 to be filled in
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @see i18n_ucnv_get_type()
 *
 */
int i18n_ucnv_get_starters(i18n_ucnv_h converter, i18n_ubool starters[256]);

/**
 * @brief   Gets the set of Unicode code points that can be converted by an ICU converter.
 *
 *          Returns one of several kinds of set:
 *
 *          1. UCNV_ROUNDTRIP_SET\n
 *          The set of all Unicode code points that can be roundtrip-converted (converted without any data loss) with the converter.
 *          This set will not include code points that have fallback mappings or are only the result of reverse fallback mappings.
 *          This set will also not include PUA code points with fallbacks, although i18n_ucnv_from_unicode() will always uses those mappings
 *          despite i18n_ucnv_set_fallback().
 *
 *          This is useful for example for
 *
 *          - checking that a string or document can be roundtrip-converted with a converter, without/before actually performing the conversion
 *          - testing if a converter can be used for text for typical text for a certain locale,
 *            by comparing its roundtrip set with the set of ExemplarCharacters from ICU's locale data or other sources\n
 *
 *          2. UCNV_ROUNDTRIP_AND_FALLBACK_SET\n
 *          The set of all Unicode code points that can be converted with the converter (i18n_ucnv_from_unicode())
 *          when fallbacks are turned on (see i18n_ucnv_set_fallback()).
 *          This set includes all code points with roundtrips and fallbacks (but not reverse fallbacks).
 *
 *          In the future, there may be more #i18n_ucnv_unicode_set_e choices to select sets with different properties.
 * @since_tizen 6.0
 * @param[in] converter     An #i18n_ucnv_h for which a set is requested.
 * @param[out] set_fill_in  A valid #i18n_uset_h *. It will be cleared by this function before the converter's specific set is filled into the #i18n_uset_h.
 * @param[in] which_set     An #i18n_ucnv_unicode_set_e selector; currently #I18N_UCNV_ROUNDTRIP_SET is the only supported value.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @see i18n_ucnv_unicode_set_e
 * @see i18n_uset_create()
 * @see i18n_uset_destroy()
 *
 */
int i18n_ucnv_get_unicode_set(i18n_ucnv_h converter, i18n_uset_h set_fill_in, i18n_ucnv_unicode_set_e which_set);

/**
 * @brief   Converts an array of unicode characters to an array of codepage characters.
 * @details This function is optimized for converting a continuous stream of data in buffer-sized chunks,
 *          where the entire source and target does not fit in available buffers.
 *
 *          The source pointer is an in/out parameter.
 *          It starts out pointing where the conversion is to begin, and ends up pointing after the last #i18n_uchar consumed.
 *
 *          Target similarly starts out pointer at the first available byte in the output buffer,
 *          and ends up pointing after the last byte written to the output.
 *
 *          The converter always attempts to consume the entire source buffer, unless the target buffer is full.
 *          When a successful error status has been returned, it means that all of the source buffer has been consumed.
 *          At that point, the caller should reset the source and @a source_limit pointers to point to the next chunk.
 *
 *          At the end of the stream (flush==TRUE), the input is completely consumed when *source==source_limit and no error code is set.
 *          The converter object is then automatically reset by this function.
 *          (This means that a converter need not be reset explicitly between data streams if it finishes the previous stream without errors.)
 *
 *          This is a stateful conversion.
 *          Additionally, even when all source data has been consumed, some data may be in the converters' internal state.
 *          Call this function repeatedly, updating the target pointers with the next empty chunk of target in case of a #I18N_ERROR_BUFFER_OVERFLOW,
 *          and updating the source pointers with the next chunk of source when a successful error status is returned,
 *          until there are no more chunks of source data.
 * @since_tizen 6.0
 * @param[in] converter     An #i18n_ucnv_h
 * @param[in,out] target    I/O parameter. Input : Points to the beginning of the buffer to copy codepage characters to. Output : points to after the last codepage character copied to target.
 * @param[in] target_limit  The pointer just after last of the target buffer
 * @param[in,out] source    I/O parameter, pointer to pointer to the source Unicode character buffer.
 * @param[in] source_limit  The pointer just after the last of the source buffer
 * @param[in] offsets       If NULL is passed, nothing will happen to it, otherwise it needs to have the same number of allocated cells as target. Will fill in offsets from target to source pointer e.g: offsets[3] is equal to 6, it means that the target[3] was a result of transcoding source[6] For output data carried across calls, and other data without a specific source character (such as from escape sequences) -1 will be placed for offsets.
 * @param[in] flush         Set to TRUE if the current source buffer is the last available chunk of the source, FALSE otherwise. Note that if a failing status is returned, this function may have to be called multiple times with flush set to TRUE until the source buffer is consumed.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW    A result would not fit in the supplied buffer
 * @see i18n_ucnv_convert()
 * @see i18n_ucnv_get_min_char_size()
 *
 */
int i18n_ucnv_from_unicode(i18n_ucnv_h converter, char **target, const char *target_limit, const i18n_uchar **source, const i18n_uchar *source_limit, int32_t *offsets, i18n_ubool flush);

/**
 * @brief   Converts a buffer of codepage bytes into an array of unicode #i18n_uchar characters.
 * @details This function is optimized for converting a continuous stream of data in buffer-sized chunks,
 *          where the entire source and target does not fit in available buffers.
 *
 *          The source pointer is an in/out parameter.
 *          It starts out pointing where the conversion is to begin, and ends up pointing after the last byte of source consumed.
 *
 *          Target similarly starts out pointer at the first available #i18n_uchar in the output buffer,
 *          and ends up pointing after the last #i18n_uchar written to the output.
 *          It does NOT necessarily keep #i18n_uchar sequences together.
 *
 *          The converter always attempts to consume the entire source buffer, unless the target buffer is full.
 *          When a successful error status has been returned, it means that all of the source buffer has been consumed.
 *          At that point, the caller should reset the source and @a source_limit pointers to point to the next chunk.
 *
 *          At the end of the stream (flush==TRUE), the input is completely consumed when *source==source_limit and no error code is set.
 *          The converter object is then automatically reset by this function.
 *          (This means that a converter need not be reset explicitly between data streams if it finishes the previous stream without errors.)
 *
 *          This is a stateful conversion.
 *          Additionally, even when all source data has been consumed, some data may be in the converters' internal state.
 *          Call this function repeatedly, updating the target pointers with the next empty chunk of target in case of a #I18N_ERROR_BUFFER_OVERFLOW,
 *          and updating the source pointers with the next chunk of source when a successful error status is returned,
 *          until there are no more chunks of source data.
* @since_tizen 6.0
* @param[in] converter     An #i18n_ucnv_h
* @param[in,out] target    I/O parameter. Input : Points to the beginning of the buffer to copy #i18n_uchar into. Output : points to after the last #i18n_uchar copied
* @param[in] target_limit  the pointer just after the end of the target buffer
* @param[in,out] source    I/O parameter, pointer to pointer to the source codepage buffer.
* @param[in] source_limit  The pointer to the byte after the end of the source buffer
* @param[in] offsets       If NULL is passed, nothing will happen to it, otherwise it needs to have the same number of allocated cells as target. Will fill in offsets from target to source pointer e.g: offsets[3] is equal to 6, it means that the target[3] was a result of transcoding source[6] For output data carried across calls, and other data without a specific source character (such as from escape sequences) -1 will be placed for offsets
* @param[in] flush         Set to TRUE if the current source buffer is the last available chunk of the source, FALSE otherwise. Note that if a failing status is returned, this function may have to be called multiple times with flush set to TRUE until the source buffer is consumed.
* @return @c 0 on success, otherwise a negative error value.
* @retval #I18N_ERROR_NONE               Successful
* @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
* @retval #I18N_ERROR_BUFFER_OVERFLOW    A result would not fit in the supplied buffer
* @see i18n_ucnv_convert()
* @see i18n_ucnv_get_min_char_size()
* @see i18n_ucnv_get_next_uchar()
*
*/
int i18n_ucnv_to_unicode(i18n_ucnv_h converter, i18n_uchar **target, const i18n_uchar *target_limit, const char **source, const char *source_limit, int32_t *offsets, i18n_ubool flush);

/**
 * @brief   Converts the Unicode string into a codepage string using an existing #i18n_ucnv_h.
 * @details The output string is NUL-terminated if possible.
 *
 *          This function is a more convenient but less powerful version of i18n_ucnv_from_unicode(). It is only useful for whole strings, not for streaming conversion.
 *
 *          The maximum output buffer capacity required will be i18n_ucnv_get_max_bytes_for_string(src_length, i18n_ucnv_get_max_char_size(cnv)).
 * @since_tizen 6.0
 * @param[in] converter     An #i18n_ucnv_h to be used (i18n_ucnv_reset_from_unicode() will be called)
 * @param[out] dest         Destination string buffer, can be NULL if dest_capacity==0
 * @param[in] dest_capacity The number of chars available at dest
 * @param[in] src           The input Unicode string
 * @param[in] src_length    The input string length (the number of #i18n_uchar characters) or -1 if NUL-terminated
 * @param[out] length       The length (the number of bytes) of the output string, not counting the terminating NUL; if the length is greater than @a dest_capacity, then the string will not fit and a buffer of the indicated length would need to be passed in
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW    A result would not fit in the supplied buffer
 * @see i18n_ucnv_from_unicode()
 * @see i18n_ucnv_convert()
 * @see i18n_ucnv_get_max_bytes_for_string()
 *
 */
int i18n_ucnv_from_uchars(i18n_ucnv_h converter, char *dest, int32_t dest_capacity, const i18n_uchar *src, int32_t src_length, int32_t *length);

/**
 * @brief   Converts the codepage string into a Unicode string using an existing #i18n_ucnv_h.
 * @details The output string is NUL-terminated if possible.
 *
 *          This function is a more convenient but less powerful version of i18n_ucnv_to_unicode(). It is only useful for whole strings, not for streaming conversion.
 *
 *          The maximum output buffer capacity required will be 2*@a src_length (each char may be converted into a surrogate pair).
 * @since_tizen 6.0
 * @param[in] converter         An #i18n_ucnv_h to be used (i18n_ucnv_reset_to_unicode() will be called)
 * @param[out] dest             Destination string buffer, can be NULL if dest_capacity==0
 * @param[in] dest_capacity     The number of #i18n_uchar characters available at dest
 * @param[in] src               The input codepage string
 * @param[in] src_length        The input string length (the number of #i18n_uchar characters) or -1 if NUL-terminated
 * @param[out] output_length    The length (the number of #i18n_uchar characters) of the output string, not counting the terminating NUL; if the @a output_length is greater than @a dest_capacity, then the string will not fit and a buffer of the indicated length would need to be passed in
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW    A result would not fit in the supplied buffer
 * @see i18n_ucnv_to_unicode()
 * @see i18n_ucnv_convert()
 *
 */
int i18n_ucnv_to_uchars(i18n_ucnv_h converter, i18n_uchar *dest, int32_t dest_capacity, const char *src, int32_t src_length, int32_t *output_length);

/**
 * @brief   Converts a codepage buffer into Unicode one character at a time.
 * @details The input is completely consumed when the #I18N_ERROR_INDEX_OUTOFBOUNDS is set.
 *
 *          Advantage compared to i18n_ucnv_to_unicode() or i18n_ucnv_to_uchars():
 *
 *          - Faster for small amounts of data, for most converters, e.g., US-ASCII, ISO-8859-1, UTF-8/16/32, and most "normal" charsets.
 *          (For complex converters, e.g., SCSU, UTF-7 and ISO 2022 variants, it uses i18n_ucnv_to_unicode() internally.)\n
 *          - Convenient.
 *
 *          Limitations compared to i18n_ucnv_to_unicode():
 *
 *          - Always assumes flush=TRUE. This makes i18n_ucnv_get_next_uchar() unsuitable for "streaming" conversion,
 *          that is, for where the input is supplied in multiple buffers,
 *          because i18n_ucnv_get_next_uchar() will assume the end of the input at the end of the first buffer.\n
 *          - Does not provide offset output.
 *
 *          It is possible to "mix" i18n_ucnv_get_next_uchar() and i18n_ucnv_to_unicode()
 *          because i18n_ucnv_get_next_uchar() uses the current state of the converter
 *          (unlike i18n_ucnv_to_uchars() which always resets first).
 *          However, if i18n_ucnv_get_next_uchar() is called after i18n_ucnv_to_unicode() stopped in the middle of a character sequence (with flush=FALSE),
 *          then i18n_ucnv_get_next_uchar() will always use the slower i18n_ucnv_to_unicode() internally until the next character boundary.
 *          (This is new in ICU 2.6. In earlier releases, i18n_ucnv_get_next_uchar() had to start at a character boundary.)
 *
 *          Instead of using i18n_ucnv_get_next_uchar(), it is recommended to convert using i18n_ucnv_to_unicode() or i18n_ucnv_to_uchars()
 *          and then iterate over the text using U16_NEXT() or an #i18n_uchar_iter_h or a C++ CharacterIterator or similar.
 *          This allows streaming conversion and offset output, for example.
 *
 *          Handling of surrogate pairs and supplementary-plane code points:\n
 *          There are two different kinds of codepages that provide mappings for surrogate characters:
 *
 *          - Codepages like UTF-8, UTF-32, and GB 18030 provide direct representations for Unicode code points U+10000-U+10ffff
 *          as well as for single surrogates U+d800-U+dfff. Each valid sequence will result in exactly one returned code point.
 *          If a sequence results in a single surrogate, then that will be returned by itself, even if a neighboring sequence encodes the matching surrogate.\n
 *          - Codepages like SCSU and LMBCS (and UTF-16) provide direct representations only for BMP code points including surrogates.
 *          Code points in supplementary planes are represented with two sequences, each encoding a surrogate.
 *          For these codepages, matching pairs of surrogates will be combined into single code points for returning from this function.
 *          (Note that SCSU is actually a mix of these codepage types.)\n
 * @since_tizen 6.0
 * @param[in] converter     An #i18n_ucnv_h
 * @param[in] source        The address of a pointer to the codepage buffer, will be updated to point after the bytes consumed in the conversion call.
 * @param[in] source_limit  Points to the end of the input buffer
 * @param[out] result       An #i18n_uchar32 resulting from the partial conversion of source
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE             Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW      A result would not fit in the supplied buffer
 * @retval #I18N_ERROR_INDEX_OUTOFBOUNDS    Trying to access the index that is out of bounds
 * @see i18n_ucnv_to_unicode()
 * @see i18n_ucnv_to_uchars()
 * @see i18n_ucnv_convert()
 *
 */
int i18n_ucnv_get_next_uchar(i18n_ucnv_h converter, const char **source, const char *source_limit, i18n_uchar32 *result);

/**
 * @brief   Converts from one external charset to another using two existing #i18n_ucnv_h.
 * @details Internally, two conversions - i18n_ucnv_to_unicode() and i18n_ucnv_from_unicode() - are used, "pivoting" through 16-bit Unicode.
 *
 *          Important: For streaming conversion (multiple function calls for successive parts of a text stream),
 *          the caller must provide a pivot buffer explicitly, and must preserve the pivot buffer and associated pointers from one call to another.
 *          (The buffer may be moved if its contents and the relative pointer positions are preserved.)
 *
 *          There is a similar function, i18n_ucnv_convert(), which has the following limitations:
 *
 *          - it takes charset names, not converter objects, so that
 *              - two converters are created for each call
 *              - only single-string conversion is possible, not streaming operation
 *          - it does not provide enough information to find out, in case of failure, whether the toUnicode or the fromUnicode conversion failed
 *
 *          By contrast, i18n_ucnv_convert_ex()
 *
 *          - takes #i18n_ucnv_h parameters instead of charset names
 *          - fully exposes the pivot buffer for streaming conversion and complete error handling
 *
 *          i18n_ucnv_convert_ex() also provides further convenience:
 *
 *          - an option to reset the converters at the beginning (if reset==TRUE, see parameters; also sets *@a pivot_target=*@a pivot_source=@a pivot_start)
 *          - allow NUL-terminated input (only a single NUL byte, will not work for charsets with multi-byte NULs) (if @a source_limit ==NULL, see parameters)
 *          - terminate with a NUL on output (only a single NUL byte, not useful for charsets with multi-byte NULs),
 *          or set #I18N_WARNING_STRING_NOT_TERMINATED if the output exactly fills the target buffer\n
 *          - the pivot buffer can be provided internally; possible only for whole-string conversion, not streaming conversion;
 *          in this case, the caller will not be able to get details about where an error occurred (if @a pivot_start ==NULL, see below)\n
 *
 *          The function returns when one of the following is true:
 *
 *          - the entire source text has been converted successfully to the target buffer
 *          - a target buffer overflow occurred (#I18N_ERROR_BUFFER_OVERFLOW)
 *          - a conversion error occurred
 *
 *          Limitation compared to the direct use of i18n_ucnv_from_unicode() and i18n_ucnv_to_unicode(): i18n_ucnv_convert_ex() does not provide offset information.
 *
 *          Limitation compared to i18n_ucnv_from_uchars() and i18n_ucnv_to_uchars(): i18n_ucnv_convert_ex() does not support preflighting directly.
 * @since_tizen 6.0
 * @param[in] target_cnv        Output converter, used to convert from the UTF-16 pivot to the target using i18n_ucnv_from_unicode().
 * @param[in] source_cnv        Input converter, used to convert from the source to the UTF-16 pivot using i18n_ucnv_to_unicode().
 * @param[in,out] target        Input: *target points to the beginning of the target buffer. Output: *target points to the first unit after the last char written.
 * @param[in] target_limit      Pointer to the first unit after the target buffer.
 * @param[in,out] source        Input: *source points to the beginning of the source buffer. Output: *source points to the first unit after the last char read.
 * @param[in] source_limit      Pointer to the first unit after the source buffer.
 * @param[in] pivot_start       Pointer to the UTF-16 pivot buffer. If @a pivot_start ==NULL, then an internal buffer is used and the other pivot arguments are ignored and can be NULL as well.
 * @param[in,out] pivot_source  Conversion from the pivot buffer to the target buffer.
 * @param[in,out] pivot_target  Conversion from the source buffer to the pivot buffer. It must be @a pivot_start<=*@a pivot_source<=*@a pivot_target<=pivotLimit and @a pivot_start<pivotLimit (unless @a pivot_start ==NULL).
 * @param[in] pivot_limit       Pointer to the first unit after the pivot buffer.
 * @param[in] reset             If TRUE, then i18n_ucnv_reset_to_unicode(@a source_cnv) and i18n_ucnv_reset_from_unicode(@a target_cnv) are called, and the pivot pointers are reset (*@a pivot_target=*@a pivot_source=@a pivot_start).
 * @param[in] flush             If TRUE, indicates the end of the input. Passed directly to i18n_ucnv_to_unicode(), and carried over to i18n_ucnv_from_unicode() when the source is empty as well.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW    A result would not fit in the supplied buffer
 * @see i18n_ucnv_convert()
 * @see i18n_ucnv_from_algorithmic()
 * @see i18n_ucnv_to_algorithmic()
 * @see i18n_ucnv_from_unicode()
 * @see i18n_ucnv_to_unicode()
 * @see i18n_ucnv_from_uchars()
 * @see i18n_ucnv_to_uchars()
 *
 */
int i18n_ucnv_convert_ex(i18n_ucnv_h target_cnv, i18n_ucnv_h source_cnv, char **target, const char *target_limit, const char **source, const char *source_limit, i18n_uchar *pivot_start, i18n_uchar **pivot_source, i18n_uchar **pivot_target, const i18n_uchar *pivot_limit, i18n_ubool reset, i18n_ubool flush);

/**
 * @brief   Converts from one external charset to another.
 * @details Internally, two converters are created according to the name arguments,
 *          then the text is converted to and from the 16-bit Unicode "pivot" using i18n_ucnv_convert_ex(), then the converters are destroyed again.
 *
 *          This is a convenience function, not an efficient way to convert a lot of text: i18n_ucnv_convert()
 *
 *          - takes charset names, not converter objects, so that
 *              - two converters are created for each call
 *              - only single-string conversion is possible, not streaming operation
 *          - does not provide enough information to find out, in case of failure, whether the toUnicode or the fromUnicode conversion failed
 *          - allows NUL-terminated input (only a single NUL byte, will not work for charsets with multi-byte NULs) (if @a source_length ==-1, see parameters)
 *          terminate with a NUL on output (only a single NUL byte, not useful for charsets with multi-byte NULs),
 *          or set #I18N_WARNING_STRING_NOT_TERMINATED if the output exactly fills the target buffer\n
 *          - a pivot buffer is provided internally
 *
 *          The function returns when one of the following is true:
 *
 *          - the entire source text has been converted successfully to the target buffer
 *          and either the target buffer is terminated with a single NUL byte or the error code is set to #I18N_WARNING_STRING_NOT_TERMINATED\n
 *          - a target buffer overflow occurred (#I18N_ERROR_BUFFER_OVERFLOW) and the full output string length is returned ("preflighting")
 *          - a conversion error occurred
 * @since_tizen 6.0
 * @param[in] to_converter_name     The name of the converter that is used to convert from the UTF-16 pivot buffer to the target.
 * @param[in] from_converter_name   The name of the converter that is used to convert from the source to the UTF-16 pivot buffer.
 * @param[in] target                Pointer to the output buffer.
 * @param[in] target_capacity       Capacity of the target, in bytes.
 * @param[in] source                Pointer to the input buffer.
 * @param[in] source_length         Length of the input text (the number of #i18n_uchar characters) or -1 if NUL-terminated
 * @param[out] output_length        Length of the complete output text in bytes, even if it exceeds the @a target_capacity and a #I18N_ERROR_BUFFER_OVERFLOW is set.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW    A result would not fit in the supplied buffer
 * @see i18n_ucnv_convert_ex()
 * @see i18n_ucnv_from_algorithmic()
 * @see i18n_ucnv_to_algorithmic()
 * @see i18n_ucnv_from_unicode()
 * @see i18n_ucnv_to_unicode()
 * @see i18n_ucnv_from_uchars()
 * @see i18n_ucnv_to_uchars()
 * @see i18n_ucnv_get_next_uchar()
 *
 */
int i18n_ucnv_convert(const char *to_converter_name, const char *from_converter_name, char *target, int32_t target_capacity, const char *source, int32_t source_length, int32_t *output_length);

/**
 * @brief   Converts from one external charset to another.
 * @details Internally, the text is converted to and from the 16-bit Unicode "pivot"
 *          using i18n_ucnv_convert_ex(). i18n_ucnv_to_algorithmic() works exactly like i18n_ucnv_convert()
 *          except that the two converters need not be looked up and created completely.
 *
 *          The source-to-pivot conversion uses the @a converter i18n_ucnv_h.
 *          The pivot-to-target conversion uses a purely algorithmic converter according to the specified type, e.g., #I18N_UCNV_UTF8 for a UTF-8 converter.
 *
 *          Internally, the algorithmic converter is created and destroyed for each function call,
 *          which is more efficient than using the public i18n_ucnv_create() but somewhat less efficient than only resetting an existing converter and using i18n_ucnv_convert_ex().
 *
 *          This function is more convenient than i18n_ucnv_convert_ex() for single-string conversions,
 *          especially when "preflighting" is desired (returning the length of the complete output even if it does not fit into the target buffer).
 * @since_tizen 6.0
 * @param[in] algorithmic_type  An #i18n_ucnv_type_e constant identifying the desired target charset as a purely algorithmic converter.
 *                              Those are converters for Unicode charsets like UTF-8, BOCU-1, SCSU, UTF-7, IMAP-mailbox-name, etc., as well as US-ASCII and ISO-8859-1.
 * @param[in] converter         The converter that is used to convert from the source to the UTF-16 pivot buffer.
 * @param[in] target            Pointer to the output buffer.
 * @param[in] target_capacity   Capacity of the target, in bytes.
 * @param[in] source            Pointer to the input buffer.
 * @param[in] source_length     Length of the input text, in bytes
 * @param[out] output_length    Length of the complete output text in bytes, even if it exceeds the @a target_capacity and a #I18N_ERROR_BUFFER_OVERFLOW is set.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW    A result would not fit in the supplied buffer
 * @see i18n_ucnv_from_algorithmic()
 * @see i18n_ucnv_convert_ex()
 * @see i18n_ucnv_convert()
 * @see i18n_ucnv_from_unicode()
 * @see i18n_ucnv_to_unicode()
 * @see i18n_ucnv_from_uchars()
 * @see i18n_ucnv_to_uchars()
 *
 */
int i18n_ucnv_to_algorithmic(i18n_ucnv_type_e algorithmic_type, i18n_ucnv_h converter, char *target, int32_t target_capacity, const char *source, int32_t source_length, int32_t *output_length);

/**
 * @brief   Converts from one external charset to another.
 * @details Internally, the text is converted to and from the 16-bit Unicode "pivot"
 *          using i18n_ucnv_convert_ex(). i18n_ucnv_from_algorithmic() works exactly like i18n_ucnv_convert()
 *          except that the two converters need not be looked up and created completely.
 *
 *          The source-to-pivot conversion uses a purely algorithmic converter according to the specified type, e.g., #I18N_UCNV_UTF8 for a UTF-8 converter.
 *          The pivot-to-target conversion uses the @a converter i18n_ucnv_h.
 *
 *          Internally, the algorithmic converter is created and destroyed for each function call,
 *          which is more efficient than using the public i18n_ucnv_create() but somewhat less efficient than only resetting an existing converter and using i18n_ucnv_convert_ex().
 *
 *          This function is more convenient than i18n_ucnv_convert_ex() for single-string conversions,
 *          especially when "preflighting" is desired (returning the length of the complete output even if it does not fit into the target buffer).
 * @since_tizen 6.0
 * @param[in] converter         The converter that is used to convert from the UTF-16 pivot buffer to the target.
 * @param[in] algorithmic_type  An #i18n_ucnv_type_e constant identifying the desired source charset as a purely algorithmic converter.
 *                              Those are converters for Unicode charsets like UTF-8, BOCU-1, SCSU, UTF-7, IMAP-mailbox-name, etc., as well as US-ASCII and ISO-8859-1.
 * @param[in] target            Pointer to the output buffer.
 * @param[in] target_capacity   Capacity of the target, in bytes.
 * @param[in] source            Pointer to the input buffer.
 * @param[in] source_length     Length of the input text, in bytes
 * @param[out] output_length    Length of the complete output text in bytes, even if it exceeds the @a target_capacity and a #I18N_ERROR_BUFFER_OVERFLOW is set.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW    A result would not fit in the supplied buffer
 * @see i18n_ucnv_to_algorithmic()
 * @see i18n_ucnv_convert_ex()
 * @see i18n_ucnv_convert()
 * @see i18n_ucnv_from_unicode()
 * @see i18n_ucnv_to_unicode()
 * @see i18n_ucnv_from_uchars()
 * @see i18n_ucnv_to_uchars()
 *
 */
int i18n_ucnv_from_algorithmic(i18n_ucnv_h converter, i18n_ucnv_type_e algorithmic_type, char *target, int32_t target_capacity, const char *source, int32_t source_length, int32_t *output_length);

/**
 * @brief   Frees up memory occupied by unused, cached converter shared data.
 * @since_tizen 6.0
 * @return The number of cached converters successfully deleted
 * @see i18n_ucnv_destroy()
 *
 */
int32_t i18n_ucnv_flush_cache(void);

/**
 * @brief   Returns the number of available converters, as per the alias file.
 * @since_tizen 6.0
 * @return The number of available converters.
 * @see i18n_ucnv_get_available_name()
 *
 */
int32_t i18n_ucnv_count_available(void);

/**
 * @brief   Gets the canonical converter name of the specified converter from a list of all available converters contaied in the alias file.
 *          All converters in this list can be created.
 * @since_tizen 6.0
 * @remarks The returned @a name should be freed by the caller with free() function.
 * @param[in] index    The index to a converter available on the system (in the range [0..i18n_ucnv_count_available()-1])
 * @param[out] name    A pointer a string (library owned), or NULL if the index is out of bounds.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @see i18n_ucnv_count_available()
 *
 */
int i18n_ucnv_get_available_name(int32_t index, const char **name);

/**
 * @brief   Gets an #i18n_uenumeration_h to enumerate all of the canonical converter names, as per the alias file, regardless of the ability to create each converter.
 * @since_tizen 6.0
 * @remarks The @a enumeration should be released using i18n_uenumeration_destroy().
 * @param[out] enumeration    An #i18n_uenumeration_h for getting all the recognized canonical converter names.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @see i18n_ucnv_get_available_name()
 * @see i18n_uenumeration_destroy()
 * @see i18n_uenumeration_next()
 *
 */
int i18n_ucnv_create_all_names(i18n_uenumeration_h *enumeration);

/**
 * @brief   Gets the number of aliases for a given converter or alias name.
 * @details This method only enumerates the listed entries in the alias file.
 * @since_tizen 6.0
 * @param[in] alias    Alias name
 * @param[out] number  The number of names on alias list for given alias
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 */
int i18n_ucnv_count_aliases(const char *alias, uint16_t *number);

/**
 * @brief   Gets the name of the alias at given index of alias list.
 * @details This method only enumerates the listed entries in the alias file.
 * @since_tizen 6.0
 * @remarks The returned @a name should be freed by the caller with free() function.
 * @param[in] alias    Alias name
 * @param[in] index    Index in alias list
 * @param[out] name    The name of the alias at given index
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 */
int i18n_ucnv_get_alias(const char *alias, uint16_t index, const char **name);

/**
 * @brief   Gets the list of alias names for the given alias.
 * @details This method only enumerates the listed entries in the alias file.
 * @since_tizen 6.0
 * @remarks The @a aliases should be released using free().
 * @param[in] alias       Alias name
 * @param[out] aliases    Aliases is a pointer to an array of i18n_ucnv_count_aliases() string-pointers (const char *) that will be filled in.
 *                        The strings themselves are owned by the library.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 */
int i18n_ucnv_get_aliases(const char *alias, const char **aliases);

/**
 * @brief   Gets an #i18n_uenumeration_h for enumerating all the alias names for a given converter that are recognized by a standard.
 * @details This method only enumerates the listed entries in the alias file.
 *          The first result in this list is the same result given by i18n_ucnv_get_standard_name(), which is the default alias for the specified standard name.
 *          The returned object must be destroyed with i18n_uenumeration_destroy() when you are done with the object.
 * @since_tizen 6.0
 * @param[in] conv_name       The original converter name
 * @param[in] standard        The name of the standard governing the names; MIME and IANA are such standards
 * @param[out] enumeration    An #i18n_uenumeration_h for getting all aliases that are recognized by a standard. If any of the parameters are invalid, NULL is returned.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @see i18n_ucnv_get_standard_name()
 * @see i18n_uenumeration_destroy()
 * @see i18n_uenumeration_next()
 *
 */
int i18n_ucnv_create_standard_names(const char *conv_name, const char *standard, i18n_uenumeration_h *enumeration);

/**
 * @brief   Returns the number of standards associated to converter names.
 * @since_tizen 6.0
 * @return The number of standards.
 *
 */
uint16_t i18n_ucnv_count_standards(void);

/**
 * @brief   Gets the name of the standard at given index of standard list.
 * @since_tizen 6.0
 * @remarks The @a name should be freed by the caller with free() function.
 * @param[in] index    Index in standard list
 * @param[out] name    The name of the standard at given index. Owned by the library.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_INDEX_OUTOFBOUNDS    Trying to access the index that is out of bounds
 *
 */
int i18n_ucnv_get_standard(uint16_t index, const char **name);

/**
 * @brief   Gets a standard name for a given converter name.
 * @details Example alias table: conv alias1 { STANDARD1 } alias2 { STANDARD1* }
 *
 *          @a name of i18n_ucnv_get_standard_name("conv", "STANDARD1", &name) from example alias table: "alias2"
 * @since_tizen 6.0
 * @remarks The @a name should be freed by the caller with free() function.
 * @param[in] cnv_name  Original converter name
 * @param[in] standard  The name of the standard governing the names; MIME and IANA are such standards
 * @param[out] name     The standard converter name; if a standard converter name cannot be determined, then NULL is returned. Owned by the library.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 */
int i18n_ucnv_get_standard_name(const char *cnv_name, const char *standard, const char **name);

/**
 * @brief   Gets the internal canonical converter name of the tagged alias.
 * @details This is the opposite of i18n_ucnv_create_standard_names(), which returns the tagged alias given the canonical name.
 *
 *          Example alias table: conv alias1 { STANDARD1 } alias2 { STANDARD1* }
 *
 *          @a name of i18n_ucnv_get_canonical_name("alias1", "STANDARD1", &name) from example alias table: "conv"
 * @since_tizen 6.0
 * @remarks The @a name should be freed by the caller with free() function.
 * @param[in] alias     Alias name
 * @param[in] standard  The name of the standard governing the names; MIME and IANA are such standards
 * @param[out] name     The canonical converter name; if a standard or alias name cannot be determined, then NULL is returned. The returned string is owned by the library.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @see i18n_ucnv_get_standard_name()
 *
 */
int i18n_ucnv_get_canonical_name(const char *alias, const char *standard, const char **name);

/**
 * @brief   Returns the current default converter name.
 * @details If you want to create a default converter, you do not need to use this function.
 *          It is faster if you pass a NULL argument to i18n_ucnv_create() the default converter.
 * @since_tizen 6.0
 * @return The current default converter name. Storage owned by the library
 * @see i18n_ucnv_set_default_name()
 *
 */
const char* i18n_ucnv_get_default_name(void);

/**
 * @brief   Sets the current default converter name.
 * @details This function is not thread safe.
 *
 *          DO NOT call this function when ANY I18N function is being used from more than one thread!
 *          This function sets the current default converter name.
 *          If this function needs to be called, it should be called during application initialization.
 *          Most of the time, the results from i18n_ucnv_get_default_name() or i18n_ucnv_create() with a NULL string argument is sufficient for your application.
 * @since_tizen 6.0
 * @param[in] name     The converter name to be the default (must be known by ICU). In case of @a name is a NULL, It resets to the default codepage.
 * @see i18n_ucnv_get_default_name()
 *
 */
void i18n_ucnv_set_default_name(const char *name);

/**
 * @brief   Fixes the backslash character mismapping.
 * @details For example, in SJIS, the backslash character in the ASCII portion is also used to represent the yen currency sign.
 *          When mapping from Unicode character 0x005C, it's unclear whether to map the character back to yen or backslash in SJIS.
 *          This function will take the input buffer and replace all the yen sign characters with backslash.
 *          This is necessary when the user tries to create a file with the input buffer on Windows.
 *          This function will test the converter to see whether such mapping is required.
 *          You can sometimes avoid using this function by using the correct version of Shift-JIS.
 * @since_tizen 6.0
 * @param[in] converter   The converter representing the target codepage.
 * @param[in] source      The input buffer to be fixed
 * @param[in] source_len  The length (the number of #i18n_uchar characters) of the input buffer
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @see i18n_ucnv_is_ambiguous()
 *
 */
int i18n_ucnv_fix_file_separator(i18n_ucnv_h converter, i18n_uchar *source, int32_t source_len);

/**
 * @brief   Determines if the converter contains ambiguous mappings of the same character or not.
 * @since_tizen 6.0
 * @param[in] converter    The converter to be tested
 * @param[out] result      TRUE if the converter contains ambiguous mapping of the same character, FALSE otherwise.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 */
int i18n_ucnv_is_ambiguous(i18n_ucnv_h converter, i18n_ubool *result);

/**
 * @brief   Sets the converter to use fallback mappings or not.
 * @details Regardless of this flag, the converter will always use fallbacks from Unicode Private Use code points, as well as reverse fallbacks (to Unicode).
 * @since_tizen 6.0
 * @param[in] converter      The converter to set the fallback mapping usage on.
 * @param[in] uses_fallback  TRUE if the user wants the converter to take advantage of the fallback mapping, FALSE otherwise.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 */
int i18n_ucnv_set_fallback(i18n_ucnv_h converter, i18n_ubool uses_fallback);

/**
 * @brief   Determines if the converter uses fallback mappings or not.
 * @details This flag has restrictions, see i18n_ucnv_set_fallback().
 * @since_tizen 6.0
 * @param[in] converter    The converter to be tested.
 * @param[out] result      TRUE if the converter uses fallback, FALSE otherwise.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 */
int i18n_ucnv_uses_fallback(i18n_ucnv_h converter, i18n_ubool *result);

/**
 * @brief   Detects Unicode signature byte sequences at the start of the byte stream and returns the charset name of the indicated Unicode charset.
 * @details The caller can i18n_ucnv_create() a converter using the charset name.
 *          The first code unit (#i18n_uchar) from the start of the stream will be U+FEFF (the Unicode BOM/signature character) and can usually be ignored.
 *
 *          For most Unicode charsets it is also possible to ignore the indicated number of initial stream bytes and start converting after them.
 *          However, there are stateful Unicode charsets (UTF-7 and BOCU-1) for which this will not work.
 *          Therefore, it is best to ignore the first output #i18n_uchar instead of the input signature bytes.
 * @since_tizen 6.0
 * @remarks The returned @a name should be freed by the caller with free() function.
 * @param[in] source            The source string in which the signature should be detected.
 * @param[in] source_length     Length of the input string, or -1 if terminated with a NUL byte.
 * @param[out] signature_length A pointer to int32_t to receive the number of bytes that make up the signature of the detected UTF. 0 if not detected. Can be a NULL pointer.
 * @param[out] name             The name of the encoding detected. NULL if encoding is not detected.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucnv_detect_unicode_signature(const char *source, int32_t source_length, int32_t *signature_length, const char **name);

/**
 * @brief   Gets the number of #i18n_uchar characters held in the converter's internal state because more input is needed for completing the conversion.
 * @details This function is useful for mapping semantics of ICU's converter interface to those of iconv, and this information is not needed for normal conversion.
 * @since_tizen 6.0
 * @param[in] converter    The converter in which the input is held.
 * @param[out] number      The number of #i18n_uchar characters in the state. -1 if an error is encountered.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucnv_from_ucount_pending(i18n_ucnv_h converter, int32_t *number);

/**
 * @brief   Gets the number of char held in the converter's internal state because more input is needed for completing the conversion.
 * @details This function is useful for mapping semantics of ICU's converter interface to those of iconv, and this information is not needed for normal conversion.
 * @since_tizen 6.0
 * @param[in] converter    The converter in which the input is held as internal state.
 * @param[out] number      The number of chars in the state. -1 if an error is encountered.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucnv_to_ucount_pending(i18n_ucnv_h converter, int32_t *number);

/**
 * @brief   Gets whether or not the charset of the converter has a fixed number of bytes per charset character.
 * @details An example of this are converters that are of the type #I18N_UCNV_SBCS or #I18N_UCNV_DBCS.
 *          Another example is UTF-32 which is always 4 bytes per character.
 *          A Unicode code point may be represented by more than one UTF-8 or UTF-16 code unit but a UTF-32 converter encodes each code point with 4 bytes.
 *          Note: This method is not intended to be used to determine whether the charset has a fixed ratio of bytes to Unicode codes units for any particular Unicode encoding form.
 * @since_tizen 6.0
 * @param[in] converter        The converter to be tested.
 * @param[out] is_fixed_width  TRUE if the converter is fixed-width.
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 *
 */
int i18n_ucnv_is_fixed_width(i18n_ucnv_h converter, i18n_ubool *is_fixed_width);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
