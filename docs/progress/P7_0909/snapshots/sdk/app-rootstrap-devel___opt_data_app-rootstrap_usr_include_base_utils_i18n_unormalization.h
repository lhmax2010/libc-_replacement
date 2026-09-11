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
 * @file utils_i18n_unormalization.h
 * @version 0.1
 * @brief utils_i18n_unormaliztion
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE Unormalization
 * @brief The Unormalization module provides Unicode normalization functionality for standard unicode normalization.
 *
 * @section CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE_OVERVIEW Overview
 * @details The Unormalization module provides Unicode normalization functionality for standard unicode normalization.
 * All instances of i18n_unormalizer_h are unmodifiable/immutable.
 * Instances returned by i18n_unormalization_get_instance() are singletons that must not be deleted by the caller.
 *
 * @section CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE_SAMPLE_CODE_1 Sample Code 1
 * @brief Creates a normalizer and normalizes a unicode string
 * @code
	i18n_unormalizer_h normalizer = NULL;
	i18n_uchar src = 0xAC00;
	i18n_uchar dest[4] = {0,};
	int dest_str_len = 0;
	int i = 0;

	// gets instance for normalizer
	i18n_unormalization_get_instance( NULL, "nfc", I18N_UNORMALIZATION_DECOMPOSE, &normalizer );

	// normalizes a unicode string
	i18n_unormalization_normalize( normalizer, &src, 1, dest, 4, &dest_str_len );
	dlog_print(DLOG_INFO, LOG_TAG, "src is 0x%x\n", src );	  // src is 0xAC00 (0xAC00: A Korean character combined with consonant and vowel)

	for ( i = 0; i < dest_str_len; i++ ) {
		dlog_print(DLOG_INFO, LOG_TAG, "dest[%d] is 0x%x\t", i + 1, dest[i] );	  // dest[1] is 0x1100	dest[2] is 0x1161 (0x1100: consonant, 0x1161: vowel)
	}
 * @endcode
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE
 * @{
 */

/**
 * @brief Gets a i18n_unormalizer_h which uses the specified data file and composes or decomposes text according to the specified mode.
 * @since_tizen 2.3
 * @remarks The @a normalizer should not be released.
 *
 * @param[in] package_name  @c NULL for ICU built-in data, otherwise application data package name.
 * @param[in] name 			"nfc" or "nfkc" or "nfkc_cf" or the name of the custom data file.
 * @param[in] mode 			The normalization mode (compose or decompose).
 * @param[out] normalizer 	The requested normalizer on success.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_unormalization_get_instance(const char *package_name, const char *name, i18n_unormalization_mode_e mode,
									 i18n_unormalizer_h *normalizer);

/**
 * @brief Writes the normalized form of the source string to the destination string(replacing its contents).
 * @details The source and destination strings must be different buffers.
 * @since_tizen 2.3
 *
 * @param[in] normalizer	An #i18n_unormalizer_h handle.
 * @param[in] src			The source string.
 * @param[in] len			The length of the source string, otherwise @c -1 if NULL-terminated.
 * @param[out] dest 		The destination string\n
 * 							Its contents are replaced with normalized @a src.
 * @param[in] capacity		The number of string_uchar that can be written to @a dest
 * @param[out] len_deststr 	The length of the destination string
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_unormalization_normalize(i18n_unormalizer_h normalizer, const i18n_uchar *src, int32_t len, i18n_uchar *dest, int32_t capacity, int32_t *len_deststr);

/**
 * @brief	Returns an #i18n_unormalizer_h instance for Unicode NFC normalization.
 * @details Same as #i18n_unormalization_get_instance(NULL, "nfc", I18N_UNORM2_COMPOSE, n2).
 *			Returns an unmodifiable singleton instance. Do not delete it.
 * @since_tizen 6.0
 * @remarks The @a normalizer should not be released.
 * @param[out] normalizer	The requested #i18n_unormalizer_h, if successful
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_get_nfc_instance(i18n_unormalizer_h* normalizer);

/**
 * @brief	Returns an #i18n_unormalizer_h instance for Unicode NFD normalization.
 * @details Same as #i18n_unormalization_get_instance(NULL, "nfc", I18N_UNORMALIZATION_DECOMPOSE, n2).
 *			Returns an unmodifiable singleton instance. Do not delete it.
 * @since_tizen 6.0
 * @remarks The @a normalizer should not be released.
 * @param[out] normalizer	The requested #i18n_unormalizer_h, if successful
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_get_nfd_instance(i18n_unormalizer_h* normalizer);

/**
 * @brief	Returns an #i18n_unormalizer_h instance for Unicode NFKC normalization.
 * @details Same as #i18n_unormalization_get_instance(NULL, "nfkc", I18N_UNORM2_COMPOSE, n2).
 *			Returns an unmodifiable singleton instance. Do not delete it.
 * @since_tizen 6.0
 * @remarks The @a normalizer should not be released.
 * @param[out] normalizer	The requested #i18n_unormalizer_h, if successful
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_get_nfkc_instance(i18n_unormalizer_h* normalizer);

/**
 * @brief	Returns an #i18n_unormalizer_h instance for Unicode NFKD normalization.
 * @details Same as #i18n_unormalization_get_instance(NULL, "nfkc", I18N_UNORMALIZATION_DECOMPOSE, n2).
 *			Returns an unmodifiable singleton instance. Do not delete it.
 * @since_tizen 6.0
 * @remarks The @a normalizer should not be released.
 * @param[out] normalizer	The requested #i18n_unormalizer_h, if successful
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_get_nfkd_instance(i18n_unormalizer_h* normalizer);

/**
 * @brief	Returns an #i18n_unormalizer_h instance for Unicode NFKC_Casefold normalization.
 * @details Same as #i18n_unormalization_get_instance(NULL, "nfkc_cf", I18N_UNORM2_COMPOSE, n2).
 *			Returns an unmodifiable singleton instance. Do not delete it.
 * @since_tizen 6.0
 * @remarks The @a normalizer should not be released.
 * @param[out] normalizer	The requested #i18n_unormalizer_h, if successful
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_get_nfkc_casefold_instance(i18n_unormalizer_h* normalizer);


/**
 * @brief	Constructs a filtered normalizer wrapping any #i18n_unormalizer_h and a filter set.
 * @details Both are aliased and @a normalizer must not be modified or deleted while this object is used.
 *			The filter set should be frozen; otherwise the performance will suffer greatly.
 * @since_tizen 6.0
 * @param[in] normalizer	Wrapped #i18n_unormalizer_h
 * @param[in] filter_set	An #i18n_uset_h which determines the characters to be normalized
 * @param[out] normalizer2	The requested Normalizer2, if successful
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_create_filtered(i18n_unormalizer_h normalizer, const i18n_uset_h *filter_set, i18n_unormalizer_h *normalizer2);

/**
 * @brief	Closes a #i18n_unormalizer_h from #i18n_unormalization_create_filtered().
 * @details Do not destroy instances from #i18n_unormalization_get_instance()
 * @since_tizen 6.0
 * @param[in] normalizer	An #i18n_unormalizer_h to be closed
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_destroy(i18n_unormalizer_h normalizer);

/**
 * @brief	Appends the normalized form of the second string to the first string (merging them at the boundary)
 *			and returns the length of the first string.
 * @details The result is normalized if the first string was normalized. The first and second strings must be different buffers.
 * @since_tizen 6.0
 * @param[in] normalizer		An #i18n_unormalizer_h
 * @param[in,out] first			A normalized string
 * @param[in] first_length		Length of the first string, or -1 if NUL-terminated
 * @param[in] first_capacity	Number of #i18n_uchar that can be written to @a first
 * @param[in] second			String, will be normalized
 * @param[in] second_length		Length of the source string, or -1 if NUL-terminated
 * @param[out] len				The length of the @a first string after merging
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_normalize_second_and_append(i18n_unormalizer_h normalizer, i18n_uchar *first, int32_t first_length, int32_t first_capacity, const i18n_uchar *second, int32_t second_length, int32_t *len);

/**
 * @brief	Appends the second string to the first string (merging them at the boundary) and returns the length of the first string.
 * @details The result is normalized if both the strings were normalized. The first and second strings must be different buffers.
 * @since_tizen 6.0
 * @param[in] normalizer		An #i18n_unormalizer_h
 * @param[in,out] first			A normalized string.
 * @param[in] first_length		Length of the first string, or -1 if NUL-terminated
 * @param[in] first_capacity	Number of #i18n_uchar that can be written to @a first
 * @param[in] second			String, should be normalized
 * @param[in] second_length		Length of the source string, or -1 if NUL-terminated
 * @param[out] len				The length of the @a first string after merging
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_append(i18n_unormalizer_h normalizer, i18n_uchar *first, int32_t first_length, int32_t first_capacity, const i18n_uchar *second, int32_t second_length, int32_t *len);

/**
 * @brief	Gets the decomposition mapping of @a code_point.
 * @details Roughly equivalent to normalizing the String form of @a code_point on an #I18N_UNORMALIZATION_DECOMPOSE #i18n_unormalizer_h,
 *			but much faster, and except that this function returns a negative value and does not write a string
 *			if @a code_point does not have a decomposition mapping in this instance's data.
 *			This function is independent of the mode of the UNormalizer2.
 * @since_tizen 6.0
 * @param[in] normalizer	An #i18n_unormalizer_h
 * @param[in] code_point	Code point
 * @param[in] decomposition	String buffer which will be set to @a code_point's decomposition mapping, if there is one.
 * @param[in] capacity		Number of #i18n_uchar that can be written to decomposition
 * @param[out] len			The non-negative length of @a code_point's decomposition, if there is one; otherwise a negative value
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_get_decomposition(i18n_unormalizer_h normalizer, i18n_uchar32 code_point, i18n_uchar *decomposition, int32_t capacity, int32_t *len);

/**
 * @brief	Gets the raw decomposition mapping of @a code_point.
 * @details This is similar to the #i18n_unormalization_get_decomposition() function
 *			but returns the raw decomposition mapping as specified in UnicodeData.txt
 *			or (for custom data) in the mapping files processed by the gennorm2 tool.
 *			By contrast, #i18n_unormalization_get_decomposition() returns the processed, recursively-decomposed version of this mapping.
 *
 *			When used on a standard NFKC Normalizer2 instance, #i18n_unormalization_get_raw_decomposition() returns the Unicode Decomposition_Mapping (dm) property.
 *
 *			When used on a standard NFC Normalizer2 instance, it returns the Decomposition_Mapping
 *			only if the Decomposition_Type (dt) is Canonical (Can); in this case, the result contains either one or two code points (=1..4 #i18n_uchar).
 *
 *			This function is independent of the mode of the UNormalizer2.
 * @since_tizen 6.0
 * @param[in] normalizer	An #i18n_unormalizer_h
 * @param[in] code_point	Code point.
 * @param[in] decomposition	String buffer which will be set to @a code_point's decomposition mapping, if there is one.
 * @param[in] capacity		Number of #i18n_uchar that can be written to decomposition
 * @param[out] len			The non-negative length of @a code_point's raw decomposition, if there is one; otherwise a negative value
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_get_raw_decomposition(i18n_unormalizer_h normalizer, i18n_uchar32 code_point, i18n_uchar *decomposition, int32_t capacity, int32_t *len);

/**
 * @brief	Performs pairwise composition of two code points and returns the composite if there is one.
 * @details Returns a composite code point c only if c has a two-way mapping to a+b.
 *			In standard Unicode normalization, this means that c has a canonical decomposition
 *			to a+b and c does not have the Full_Composition_Exclusion property.
 *
 *			This function is independent of the mode of the UNormalizer2.
 * @since_tizen 6.0
 * @param[in] normalizer	An #i18n_unormalizer_h
 * @param[in] a				A (normalization starter) code point.
 * @param[in] b				Another code point.
 * @param[out] c			The non-negative composite code point if there is one; otherwise a negative value.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_compose_pair(i18n_unormalizer_h normalizer, i18n_uchar32 a, i18n_uchar32 b, i18n_uchar32 *c);

/**
 * @brief	Gets the combining class of @a code_point.
 * @details The default implementation returns 0 but all standard implementations return the Unicode Canonical_Combining_Class value.
 * @since_tizen 6.0
 * @param[in] normalizer	An #i18n_unormalizer_h
 * @param[in] code_point	Code point
 * @return 					@a code_point's combining class

 *
 */
uint8_t i18n_unormalization_get_combining_class(i18n_unormalizer_h normalizer, i18n_uchar32 code_point);

/**
 * @brief	Tests if the string is normalized.
 * @details Internally, in cases where the #i18n_unormalization_quick_check() method would return "maybe"
 *			(which is only possible for the two COMPOSE modes)
 *			this method resolves to "yes" or "no" to provide a definitive result,
 *			at the cost of doing more work in those cases.
 * @since_tizen 6.0
 * @param[in] normalizer	An #i18n_unormalizer_h
 * @param[in] s				Input string
 * @param[in] length		Length of the string, or -1 if NUL-terminated
 * @param[out] ret			TRUE if s is normalized
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_is_normalized(i18n_unormalizer_h normalizer, const i18n_uchar *s, int32_t length, i18n_ubool *ret);

/**
 * @brief	Tests if the string is normalized.
 * @details For the two COMPOSE modes, the result could be "maybe" in cases that would take a little more work to resolve definitively.
 *			Use #i18n_unormalization_span_quick_check_yes() and #i18n_unormalization_normalize_second_and_append()
 *			for a faster combination of quick check + normalization, to avoid re-checking the "yes" prefix.
 * @since_tizen 6.0
 * @param[in] normalizer	An #i18n_unormalizer_h
 * @param[in] s				Input string
 * @param[in] length		Length of the string, or -1 if NUL-terminated
 * @param[out] result		Normalization check result
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_quick_check(i18n_unormalizer_h normalizer, const i18n_uchar *s, int32_t length, i18n_unormalization_check_result_e *result);

/**
 * @brief	Gets the end of the normalized substring of the input string.
 * @details The returned end index is usually one or more characters before the "no" or "maybe" character:
 *			The end index is at a normalization boundary. (See the class documentation for more about normalization boundaries.)
 *
 *			When the goal is a normalized string and most input strings are expected to be normalized already,
 *			then call this method, and if it returns a prefix shorter than the input string,
 *			copy that prefix and use #i18n_unormalization_normalize_second_and_append() for the remainder.
 * @since_tizen 6.0
 * @param[in] normalizer	An #i18n_unormalizer_h
 * @param[in] s				Input string.
 * @param[in] length		Length of the string, or -1 if NUL-terminated.
 * @param[out] result		End index of the normalized beginning of the @a s string.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_span_quick_check_yes(i18n_unormalizer_h normalizer, const i18n_uchar *s, int32_t length, int32_t *result);

/**
 * @brief	Tests if the character always has a normalization boundary before it, regardless of context.
 * @since_tizen 6.0
 * @param[in] normalizer	An #i18n_unormalizer_h
 * @param[in] c				Character to test
 * @param[out] result		TRUE if c has a normalization boundary before it
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalizer_has_boundary_before(i18n_unormalizer_h normalizer, i18n_uchar32 c, i18n_ubool *result);

/**
 * @brief	Tests if the character always has a normalization boundary after it, regardless of context.
 * @details For details see the Normalizer2 base class documentation.
 * @since_tizen 6.0
 * @param[in] normalizer	An #i18n_unormalizer_h
 * @param[in] c				Character to test
 * @param[out] result		TRUE if c has a normalization boundary after it
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalizer_has_boundary_after(i18n_unormalizer_h normalizer, i18n_uchar32 c, i18n_ubool *result);

/**
 * @brief	Tests if the character is normalization-inert.
 * @details For details see the Normalizer2 base class documentation.
 * @since_tizen 6.0
 * @param[in] normalizer	An #i18n_unormalizer_h
 * @param[in] c				Character to test
 * @param[out] result		TRUE if c is normalization-inert
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_unormalization_is_inert(i18n_unormalizer_h normalizer, i18n_uchar32 c, i18n_ubool *result);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
