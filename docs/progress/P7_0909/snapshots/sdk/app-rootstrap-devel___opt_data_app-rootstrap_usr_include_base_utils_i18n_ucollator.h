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
 * @file utils_i18n_ucollator.h
 * @version 0.1
 * @brief utils_i18n_ucollator
 */

#ifdef __cplusplus
extern "C" {
# endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE Ucollator
 * @brief The Ucollator module performs locale-sensitive string comparison.
 *
 * @section CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE_HEADER Required Header
 *     \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE_OVERVIEW Overview
 * @details The Ucollator module performs locale-sensitive string comparison.
 *          It builds searching and sorting routines for natural language text
 *          and provides correct sorting orders for most locales supported.
 *
 * @section CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE_SAMPLE_CODE_1 Sample Code 1
 * @brief Converts two different byte strings to two different unicode strings
 *        and compares the unicode strings to check if the strings are equal to each other.
 * @code
	i18n_uchar uchar_src[64] = {0,};
	i18n_uchar uchar_target[64] = {0,};
	char *src = "tizen";
	char *target = "bada";
	int uchar_src_len = 0;
	int uchar_target_len = 0;
	i18n_ucollator_h coll = NULL;
	i18n_ubool result = NULL;

	i18n_ustring_from_UTF8( uchar_src, 64, NULL, src, -1 );
	i18n_ustring_from_UTF8( uchar_target, 64, NULL, target, -1 );

	// creates a collator
	i18n_ucollator_create( "en_US", &coll );

	// sets strength for coll
	i18n_ucollator_set_strength( coll, I18N_UCOLLATOR_PRIMARY );

	// compares uchar_src with uchar_target
	i18n_ustring_get_length( uchar_src, &uchar_src_len );
	i18n_ustring_get_length( uchar_target, &uchar_target_len );
	i18n_ucollator_equal( coll, uchar_src, uchar_src_len, uchar_target, uchar_target_len, &result );
	dlog_print(DLOG_INFO, LOG_TAG, "%s %s %s\n",
	src, result == 1 ? "is equal to" : "is not equal to", target );	// tizen is not equal to bada

	// destroys the collator
	i18n_ucollator_destroy( coll );
 * @endcode
 * @section CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE_SAMPLE_CODE_2 Sample Code 2
 * @brief Sorts in ascending order on the given data using string_ucollator
 * @code
	i18n_ucollator_h coll = NULL;
	char *src[3] = { "cat", "banana", "airplane" };
	char *tmp = NULL;
	i18n_uchar buf_01[16] = {0,};
	i18n_uchar buf_02[16] = {0,};
	i18n_ucollator_result_e result = I18N_UCOLLATOR_EQUAL;
	int i = 0, j = 0;
	int ret = I18N_ERROR_NONE;
	int buf_01_len = 0, buf_02_len = 0;

	for (i = 0; i < sizeof(src) / sizeof(src[0]); i++) {
		dlog_print(DLOG_INFO, LOG_TAG, "%s\n", src[i]);
	}	 // cat	   banana	 airplane

	// creates a collator
	ret = i18n_ucollator_create("en_US", &coll);

	// compares and sorts in ascending order
	if (ret == I18N_ERROR_NONE) {
		i18n_ucollator_set_strength(coll, I18N_UCOLLATOR_TERTIARY);
		for (i = 0; i < 2; i++) {
			for (j = 0; j < 2 - i; j++) {
				i18n_ustring_copy_ua(buf_01, src[j]);
				i18n_ustring_copy_ua(buf_02, src[j+1]);
				i18n_ustring_get_length(buf_01, &buf_01_len);
				i18n_ustring_get_length(buf_02, &buf_02_len);
				// compares buf_01 with buf_02
				i18n_ucollator_str_collator(coll, buf_01, buf_01_len, buf_02, buf_02_len, &result);
				if (result == I18N_UCOLLATOR_GREATER) {
					tmp = src[j];
					src[j] = src[j+1];
					src[j+1] = tmp;
				}
			}
		}
	}
	// destroys the collator
	i18n_ucollator_destroy( coll );	   // deallocate memory for collator

	for (i = 0; i < sizeof(src) / sizeof(src[0]); i++) {
		dlog_print(DLOG_INFO, LOG_TAG, "%s\n", src[i]);
	}	 // ariplane	banana	  cat
 * @endcode
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE
 * @{
 */

/**
 * @brief Creates a #i18n_ucollator_h for comparing strings.
 * @details For some languages, multiple collation types are available;
 * for example, "de@collation=phonebook".
 * Collation attributes can be specified via locale keywords as well,
 * in the old locale extension syntax ("el@colCaseFirst=upper")
 * or in language tag syntax ("el-u-kf-upper").
 * See <a href="http://userguide.icu-project.org/collation/api">User Guide: Collation API</a>.
 *
 * The #i18n_ucollator_h is used in all the calls to the Collation service.\n
 * After finished, collator must be disposed off by calling {@link #i18n_ucollator_destroy()}.
 * @since_tizen 2.3
 * @remarks Must release @a collator using i18n_ucollator_destroy().
 *
 * @param[in] locale The locale containing the required collation rules\n
 * Special values for locales can be passed in - if @c NULL is passed for the locale,
 * the default locale collation rules will be used \n
 * If empty string ("") or "root" is passed, UCA rules will be used.
 * @param[out] collator #i18n_ucollator_h, otherwise @c 0 if an error occurs
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_destroy()
 */
int i18n_ucollator_create(const char *locale, i18n_ucollator_h *collator);

/**
 * @brief Produces an #i18n_ucollator_h instance according to the rules supplied.
 * @details The rules are used to change the default ordering, defined in the UCA in a process
 *          called tailoring. The resulting #i18n_ucollator_h pointer can be used in the same way as
 *          the one obtained by i18n_ucollator_str_collator().
 * @since_tizen 4.0
 * @remarks The @a collator should be released using i18n_ucollator_destroy().
 *
 * @param[in]   rules               A string describing the collation rules. For the syntax of the rules
 *                                  please see users guide
 * @param[in]   rules_length        The length of @a rules, or @c -1 if null-terminated
 * @param[in]   normalization_mode  The normalization mode, one of:\n
 *                                  #I18N_UCOLLATOR_OFF (expect the text to not need normalization),\n
 *                                  #I18N_UCOLLATOR_ON (normalize)\n
 *                                  #I18N_UCOLLATOR_DEFAULT (set the mode according to the rules)\n
 * @param[in]   strength            The default collation strength: One of\n
 *                                  #I18N_UCOLLATOR_PRIMARY\n,
 *                                  #I18N_UCOLLATOR_SECONDARY\n,
 *                                  #I18N_UCOLLATOR_TERTIARY\n,
 *                                  #I18N_UCOLLATOR_IDENTICAL\n,
 *                                  #I18N_UCOLLATOR_DEFAULT_STRENGTH - can be also set in the rules\n
 * @param[out]  parse_error         A pointer to #i18n_uparse_error_s to receive information about errors
 *                                  occurred during parsing. This argument can currently be set to @c NULL,
 *                                  but at users own risk. Please provide a real structure.
 * @param[out]  collator            A pointer to a #i18n_ucollator_h. It is not guaranteed that @c NULL be returned
 *                                  in case of error - please use status argument to check for errors.
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_create()
 * @see i18n_ucollator_safe_clone()
 * @see i18n_ucollator_destroy()
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <string.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_error_code_e error_code;
 * 	i18n_ucollator_h collator = NULL;
 * 	i18n_uparse_error_s parse_error;
 * 	const char *test_rules = "&9 < a, A < b, B < c, C; ch, cH, Ch, CH < d, D, e, E";
 * 	i18n_uchar rules[128];
 *
 * 	i18n_ustring_copy_ua(rules, test_rules);
 * 	error_code = i18n_ucollator_create_rules(rules, strlen(test_rules), I18N_UCOLLATOR_ON, I18N_UCOLLATOR_DEFAULT_STRENGTH,
 * 										&parse_error, &collator);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucollator_create_rules: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_ucollator_destroy(collator);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucollator_create_rules(const i18n_uchar *rules,
								int32_t rules_length,
								i18n_ucollator_attribute_value_e normalization_mode,
								i18n_ucollator_strength_e strength,
								i18n_uparse_error_s *parse_error,
								i18n_ucollator_h *collator);

/**
 * @brief Gets a set containing the expansions defined by the collator.
 * @details The set includes both the root collator's expansions and the expansions defined
 *          by the tailoring.
 * @since_tizen 4.0
 *
 * @param[in]    collator        The collator
 * @param[in]    add_prefixes    Add the prefix contextual elements to contractions
 * @param[out]   contractions    If not @c NULL, the set to hold the contractions
 * @param[out]   expansions      If not @c NULL, the set to hold the expansions
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_get_contractions_and_expansions(const i18n_ucollator_h collator,
												   i18n_ubool add_prefixes,
												   i18n_uset_h contractions,
												   i18n_uset_h expansions);

/**
 * @brief Destroys a #i18n_ucollator_h.
 * @details Once destroyed, the #i18n_ucollator_h should not be used. Every created collator
 *          should be destroyed.
 * @since_tizen 2.3
 *
 * @param[in]   collator   The #i18n_ucollator_h to close
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_create()
 */
int i18n_ucollator_destroy(i18n_ucollator_h collator);

/**
 * @brief Compares two strings.
 * @details The strings will be compared using the options already specified.
 * @since_tizen 2.3
 *
 * @param[in]    collator    The #i18n_ucollator_h containing the comparison rules
 * @param[in]    src         The source string
 * @param[in]    src_len     The length of the source, otherwise @c -1 if null-terminated
 * @param[in]    target      The target string
 * @param[in]    target_len  The length of the target, otherwise @c -1 if null-terminated
 * @param[out]   result      The result of comparing the strings \n
 *                           One of #I18N_UCOLLATOR_EQUAL, #I18N_UCOLLATOR_GREATER, or #I18N_UCOLLATOR_LESS
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_equal()
 */
int i18n_ucollator_str_collator(const i18n_ucollator_h collator,
								const i18n_uchar *src,
								int32_t src_len,
								const i18n_uchar *target,
								int32_t target_len,
								i18n_ucollator_result_e *result);

/**
 * @brief Compares two strings in UTF-8.
 * @details The strings will be compared using the options already specified.
 * @remarks Note: When input string contains a malformed UTF-8 byte sequence, this function treats
 *          these bytes as REPLACEMENT CHARACTER (U+FFFD).
 * @since_tizen 4.0
 *
 * @param[in]    collator    The #i18n_ucollator_h containing the comparison rules
 * @param[in]    src         The source UTF-8 string
 * @param[in]    src_len     The length of the @a src, otherwise @c -1 if null-terminated
 * @param[in]    target      The target UTF-8 string
 * @param[in]    target_len  The length of the @a target, otherwise @c -1 if null-terminated
 * @param[out]   result      The result of comparing the strings \n
 *                           One of #I18N_UCOLLATOR_EQUAL, #I18N_UCOLLATOR_GREATER, or #I18N_UCOLLATOR_LESS
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_greater()
 * @see i18n_ucollator_greater_or_equal()
 * @see i18n_ucollator_equal()
 */
int i18n_ucollator_str_collator_utf8(const i18n_ucollator_h collator,
									 const char *src, int32_t src_len,
									 const char *target, int32_t target_len,
									 i18n_ucollator_result_e *result);

/**
 * @brief Determines if one string is greater than another.
 * @details This function is equivalent to i18n_ucollator_str_collator() == #I18N_UCOLLATOR_GREATER
 * @since_tizen 4.0
 *
 * @param[in]     collator     The #i18n_ucollator_h containing the comparison rules
 * @param[in]     src          The source string
 * @param[in]     src_len      The length of the @a src, otherwise @c -1 if null-terminated
 * @param[in]     target       The target string
 * @param[in]     target_len   The length of the @a target, otherwise @c -1 if null-terminated
 * @param[out]    is_greater   TRUE if source is greater than @a target, FALSE otherwise
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_str_collator()
 * @see i18n_ucollator_greater_or_equal()
 * @see i18n_ucollator_equal()
 */
int i18n_ucollator_greater(const i18n_ucollator_h collator, const i18n_uchar *src,
						   int32_t src_len, const i18n_uchar *target,
						   int32_t target_len, i18n_ubool *is_greater);

/**
 * @brief Determines if one string is greater than or equal to another.
 * @details This function is equivalent to i18n_ucollator_str_collator() != #I18N_UCOLLATOR_LESS
 * @since_tizen 4.0
 *
 * @param[in]    collator      The #i18n_ucollator_h containing the comparison rules
 * @param[in]    src           The source string
 * @param[in]    src_len       The length of the @a src, otherwise @c -1 if null-terminated
 * @param[in]    target        The target string
 * @param[in]    target_len    The length of the @a target, otherwise @c -1 if null-terminated
 * @param[out]   result        TRUE if source string is greater than or equal to @a target, FALSE otherwise
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_str_collator()
 * @see i18n_ucollator_greater()
 * @see i18n_ucollator_equal()
 */
int i18n_ucollator_greater_or_equal(const i18n_ucollator_h collator, const i18n_uchar *src,
									int32_t src_len, const i18n_uchar *target,
									int32_t target_len, i18n_ubool *result);

/**
 * @brief Compares two strings for equality.
 * @details This function is equivalent to {@link #i18n_ucollator_str_collator()}.
 * @since_tizen 2.3
 *
 * @param[in]    collator     The #i18n_ucollator_h containing the comparison rules
 * @param[in]    src          The source string
 * @param[in]    src_len      The length of the source, otherwise @c -1 if null-terminated
 * @param[in]    target       The target string
 * @param[in]    target_len   The length of the target, otherwise @c -1 if null-terminated
 * @param[out]   equal        If @c true source is equal to target, otherwise @c false
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_str_collator()
 */
int i18n_ucollator_equal(const i18n_ucollator_h collator, const i18n_uchar *src,
						 int32_t src_len, const i18n_uchar *target,
						 int32_t target_len, i18n_ubool *equal);

/**
 * @brief Compares two UTF-8 encoded strings.
 * @details The strings will be compared using the options already specified.
 * @since_tizen 4.0
 *
 * @param[in]    collator    The #i18n_ucollator_h containing the comparison rules
 * @param[in]    src_iter    The source string iterator
 * @param[in]    dest_iter   The dest string iterator
 * @param[out]   result      The result of comparing the strings \n
 *                           One of #I18N_UCOLLATOR_EQUAL, #I18N_UCOLLATOR_GREATER, or #I18N_UCOLLATOR_LESS
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_equal()
 */
int i18n_ucollator_str_collator_iter(const i18n_ucollator_h collator,
									 const i18n_uchar_iter_h *src_iter,
									 const i18n_uchar_iter_h *dest_iter,
									 i18n_ucollator_result_e *result);

/**
 * @brief Gets the collation strength used in an #i18n_ucollator_h.
 * @details The strength influences how strings are compared.
 * @since_tizen 4.0
 *
 * @param[in]    collator       The #i18n_ucollator_h to query
 * @param[out]   strength       The collation strength; One of #I18N_UCOLLATOR_PRIMARY, #I18N_UCOLLATOR_SECONDARY,
 *                              #I18N_UCOLLATOR_TERTIARY, #I18N_UCOLLATOR_QUATERNARY, #I18N_UCOLLATOR_IDENTICAL
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_set_strength()
 */
int i18n_ucollator_get_strength(const i18n_ucollator_h collator, i18n_ucollator_strength_e *strength);

/**
 * @brief Sets the collation strength used in a collator.
 * @details The strength influences how strings are compared.
 * @since_tizen 2.3
 *
 * @param[in]     collator    The i18n_ucollator_h to set.
 * @param[in]     strength    The desired collation strength.\n
 *                            One of #i18n_ucollator_strength_e
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_set_strength(i18n_ucollator_h collator, i18n_ucollator_strength_e strength);

/**
 * @brief Retrieves the reordering codes for this collator.
 * @details These reordering codes are a combination of #i18n_uscript_code_e codes and
 *          #i18n_ucollator_reorder_code_e entries.
 * @since_tizen 4.0
 *
 * @param[in]    collator    The #i18n_ucollator_h to query
 * @param[in]    dest_size   The length of @a dest. If it is @c 0, then dest may be @c NULL and the function
 *                           will only set the @a n_codes parameter to the length of the result without
 *                           writing any codes (pre-flighting).
 * @param[out]   dest        The array to fill with the script ordering
 * @param[out]   n_codes     The number of reordering codes written to the @a dest array
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_set_reorder_codes()
 * @see i18n_ucollator_get_equivalent_reorder_codes()
 * @see #i18n_uscript_code_e
 * @see #i18n_ucollator_reorder_code_e
 */
int i18n_ucollator_get_reorder_codes(const i18n_ucollator_h collator, int32_t dest_size,
									 int32_t *dest, int32_t *n_codes);

/**
 * @brief Sets the reordering codes for this collator.
 * @details Collation reordering allows scripts and some other groups of characters to be moved
 *          relative to each other. This reordering is done on top of the DUCET/CLDR standard
 *          collation order. Reordering can specify groups to be placed at the start and/or the
 *          end of the collation order. These groups are specified using #i18n_uscript_code_e codes
 *          and #i18n_ucollator_reorder_code_e entries.
 *
 *          By default, reordering codes specified for the start of the order are placed in the order
 *          given after several special non-script blocks. These special groups of characters are
 *          space, punctuation, symbol, currency, and digit. These special groups are represented
 *          with #i18n_ucollator_reorder_code_e entries. Script groups can be intermingled with these
 *          special non-script groups if those special groups are explicitly specified in the reordering.
 *
 *          The special code OTHERS stands for any script that is not explicitly mentioned in the
 *          list of reordering codes given.
 *
 *          The special reorder code DEFAULT will reset the reordering for this collator to
 *          the default for this collator. The default reordering may be the DUCET/CLDR order
 *          or may be a reordering that was specified when this collator was created from resource
 *          data or from rules. The DEFAULT code must be the sole code supplied when it is used.
 *          If not, then #I18N_ERROR_INVALID_PARAMETER will be set.
 *
 *          The special reorder code NONE will remove any reordering for this collator. The result
 *          of setting no reordering will be to have the DUCET/CLDR ordering used.
 * @since_tizen 4.0
 *
 * @param[in]    collator        The #i18n_ucollator_h to query
 * @param[in]    reorder_codes   An array of script codes in the new order.
 *                               This can be @c NULL if the @a codes_length is also set to @c 0.
 *                               An empty array will clear any reordering codes on the collator.
 * @param[in]    codes_length    The length of @a reorder_codes
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_get_reorder_codes()
 * @see i18n_ucollator_get_equivalent_reorder_codes()
 * @see #i18n_uscript_code_e
 * @see #i18n_ucollator_reorder_code_e
 */
int i18n_ucollator_set_reorder_codes(i18n_ucollator_h collator, const int32_t *reorder_codes,
									 int32_t codes_length);

/**
 * @brief Retrieves the reorder codes that are grouped with the given reorder code.
 * @details Some reorder codes will be grouped and must reorder together. Beginning with ICU 55,
 *          scripts only reorder together if they are primary-equal, for example Hiragana and Katakana.
 * @since_tizen 4.0
 *
 * @param[in]    reorder_code     The reorder code to determine equivalence for
 * @param[in]    dest_size        The length of @a dest. If it is @c 0, then dest may be @c NULL and the function will
 *                                only return the length of the result without writing any codes (pre-flighting).
 * @param[out]   dest             The array to fill with the script ordering
 * @param[out]   n_codes          The number of reordering codes written to the @a dest array.
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_set_reorder_codes()
 * @see i18n_ucollator_get_reorder_codes()
 * @see #i18n_uscript_code_e
 * @see #i18n_ucollator_reorder_code_e
 */
int i18n_ucollator_get_equivalent_reorder_codes(int32_t reorder_code, int32_t dest_size,
												int32_t *dest, int32_t *n_codes);

/**
 * @brief Gets the display name for an #i18n_ucollator_h.
 * @details The display name is suitable for presentation to a user.
 * @since_tizen 4.0
 *
 * @param[in]    obj_locale     The locale of the collator in question
 * @param[in]    disp_locale    The locale for display
 * @param[in]    dest_size      The maximum size of @a dest
 * @param[out]   dest           A pointer to a buffer to receive the attribute
 * @param[out]   display_size   The total buffer size needed; if greater than @a dest_size, the output was
 *                              truncated
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_get_display_name(const char *obj_locale, const char *disp_locale,
									int32_t dest_size, i18n_uchar *dest,
									int32_t *display_size);

/**
 * @brief Gets a locale for which collation rules are available.
 * @details An #i18n_ucollator_h in a locale returned by this function will perform the correct
 *          collation for the locale.
 * @since_tizen 4.0
 * @remarks The @a locale should not be released.
 *
 * @param[in]    locale_index     The index of the desired locale
 * @param[out]   locale           A locale for which collation rules are available, or @c 0 if none
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * 	i18n_error_code_e error_code;
 * 	const char *locale = NULL;
 * 	int32_t locale_index = 0;
 *
 * 	i18n_ucollator_get_available(locale_index, &locale);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucollator_get_available: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * @endcode
 */
int i18n_ucollator_get_available(int32_t locale_index, const char **locale);

/**
 * @brief Determines how many locales have collation rules available.
 * @details This function is most useful for determining the loop ending condition for calls
 *          to i18n_ucollator_get_available().
 * @since_tizen 4.0
 *
 * @param[out]    n_available    The number of locales for which collation rules are available
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_count_available(int32_t *n_available);

/**
 * @brief Creates a string enumerator of all locales for which a valid collator may be created.
 * @since_tizen 4.0
 * @remarks The @a locales should be released using i18n_uenumeration_destroy().
 *
 * @param[out]    locales    A string enumeration over locale strings. The caller is responsible for
 *                           releasing the result.
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_error_code_e error_code;
 * 	i18n_uenumeration_h locales;
 *
 * 	error_code = i18n_ucollator_create_available_locales(&locales);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucollator_create_available_locales: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_uenumeration_destroy(locales);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucollator_create_available_locales(i18n_uenumeration_h *locales);

/**
 * @brief Creates a string enumerator of all possible keywords that are relevant to collation.
 * @details At this point, the only recognized keyword for this service is "collation".
 * @since_tizen 4.0
 * @remarks The @a keywords should be released using i18n_uenumeration_destroy().
 *
 * @param[out]    keywords    A string enumeration over locale strings. The caller is responsible for
 *                            releasing the result.
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_error_code_e error_code;
 * 	i18n_uenumeration_h keywords;
 *
 * 	error_code = i18n_ucollator_get_keywords(&keywords);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucollator_get_keywords: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_uenumeration_destroy(keywords);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucollator_get_keywords(i18n_uenumeration_h *keywords);

/**
 * @brief Given a keyword, create a string enumeration of all values for that keyword that are
 *        currently in use.
 * @since_tizen 4.0
 * @remarks The @a keywords should be released using i18n_uenumeration_destroy().
 *
 * @param[in]    keyword     A particular keyword as enumerated by i18n_ucollator_get_keywords().
 *                           If any other keyword is passed in, returns #I18N_ERROR_INVALID_PARAMETER
 * @param[out]   keywords    A string enumeration over collation keyword values, or @c NULL upon error.
 *                           The caller is responsible for releasing the result.
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_error_code_e error_code;
 * 	i18n_uenumeration_h keyword;
 *
 * 	error_code = i18n_ucollator_get_keyword_values("collation", &keyword);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucollator_get_keyword_values: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_uenumeration_destroy(keyword);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucollator_get_keyword_values(const char *keyword, i18n_uenumeration_h *keywords);

/**
 * @brief Given a key and a locale, returns an array of string values in a preferred order that
 *        would make a difference.
 * @details These are all and only those values where the creation of the service with
 *          the locale formed from the input locale plus input keyword and that value has different
 *          behavior than creation with the input locale alone.
 * @since_tizen 4.0
 * @remarks The @a keywords should be released using i18n_uenumeration_destroy().
 *
 * @param[in]    key             One of the keys supported by this service. For now, only "collation" is
 *                               supported
 * @param[in]    locale          The locale
 * @param[in]    commonly_used   If set to true it will return only commonly used values with the given
 *                               locale in preferred order. Otherwise, it will return all the available
 *                               values for the locale
 * @param[out]   keywords        A string enumeration over keyword values for the given key and the locale
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_error_code_e error_code;
 * 	i18n_uenumeration_h keywords;
 *
 * 	error_code = i18n_ucollator_get_keyword_values_for_locale("collation", "en_US", false, &keywords);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucollator_get_keyword_values_for_locale: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_uenumeration_destroy(keywords);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucollator_get_keyword_values_for_locale(const char *key, const char *locale,
												  i18n_ubool commonly_used,
												  i18n_uenumeration_h *keywords);

/**
 * @brief Returns the functionally equivalent locale for the specified input locale, with respect
 *        to given keyword, for the collation service.
 * @details If two different input locale + keyword combinations produce the same result locale,
 *          then collators instantiated for these two different input locales will behave equivalently.
 *          The converse is not always true; two collators may in fact be equivalent, but return
 *          different results, due to internal details. The return result has no other meaning than
 *          that stated above, and implies nothing as to the relationship between the two locales.
 *          This is intended for use by applications who wish to cache collators, or otherwise
 *          reuse collators when possible. The functional equivalent may change over time.
 * @since_tizen 4.0
 *
 * @param[in]    keyword        A particular keyword as enumerated by i18n_ucollator_get_keywords()
 * @param[in]    locale         The specified input locale
 * @param[in]    dest_size      Capacity of the dest buffer
 * @param[out]   dest           The functionally equivalent result locale
 * @param[out]   is_available   If non-NULL, indicates whether the specified input locale was
 *                              'available' to the collation service. A locale is defined as
 *                              'available' if it physically exists within the collation locale data.
 * @param[out]   buffer_size    The actual buffer size needed for the locale. If greater than @a dest_size,
 *                              the returned full name will be truncated and an error code will be returned.
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_get_functional_equivalent(const char *keyword, const char *locale,
											 int32_t dest_size, char *dest,
											 i18n_ubool *is_available, int32_t *buffer_size);

/**
 * @brief Gets the collation tailoring rules from a #i18n_ucollator_h.
 * @details The rules will follow the rule syntax.
 * @since_tizen 4.0
 * @remarks The @a rules should not be released.
 *
 * @param[in]    collator      The #i18n_ucollator_h to query
 * @param[out]   length        The length of the rules
 * @param[out]   rules         The collation tailoring rules
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_get_rules(const i18n_ucollator_h collator, int32_t *length,
							 const i18n_uchar **rules);

/**
 * @brief Gets a sort key for a string from a #i18n_ucollator_h.
 * @details Sort keys may be compared using strcmp. Note that sort keys are often less efficient
 *          than simply doing comparison. For more details, see the ICU User Guide. Like ICU
 *          functions that write to an output buffer, the buffer contents is undefined if the buffer
 *          capacity (dest_size parameter) is too small. Unlike ICU functions that write a string
 *          to an output buffer, the terminating zero byte is counted in the sort key length.
 * @since_tizen 4.0
 *
 * @param[in]    collator        The #i18n_ucollator_h containing the collation rules
 * @param[in]    src             The string to transform
 * @param[in]    src_length      The length of source, or -1 if null-terminated
 * @param[in]    dest_size       The maximum size of dest buffer
 * @param[out]   dest            A pointer to a buffer to receive the attribute
 * @param[out]   result_length   The size needed to fully store the sort key. If there was an internal
 *                               error generating the sort key, a zero value is returned.
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_get_sort_key(const i18n_ucollator_h collator, const i18n_uchar *src,
								int32_t src_length, int32_t dest_size,
								uint8_t *dest, int32_t *result_length);

/**
 * @brief Gets the next count bytes of a sort key.
 * @details Caller needs to preserve state array between calls and to provide the same type of
 *          #i18n_ucollator_h set with the same string. The destination buffer provided must be
 *          big enough to store the number of requested bytes.
 *          The generated sort key may or may not be compatible with sort keys generated
 *          using i18n_ucollator_get_sort_key().
 * @since_tizen 4.0
 *
 * @param[in]    collator       The #i18n_ucollator_h containing the collation rules
 * @param[in]    iter           #i18n_uchar_iter_h containing the string we need the sort key to be calculated for
 * @param[in]    state          Opaque state of sortkey iteration
 * @param[in]    dest           Buffer to hold the resulting sortkey part
 * @param[out]   count          The number of sort key bytes required
 * @param[out]   result_length  The actual number of bytes of a sortkey. It can be smaller than
 *                              count if we have reached the end of the sort key.
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_next_sort_key_part(const i18n_ucollator_h collator,
									  i18n_uchar_iter_h *iter, uint32_t state[2],
									  uint8_t *dest, int32_t count,
									  int32_t *result_length);

/**
 * @brief Produces a bound for a given sortkey and a number of levels.
 * @details Return value is always the number of bytes needed, regardless of whether the result
 *          buffer was big enough or even valid.
 *
 *          Resulting bounds can be used to produce a range of strings that are between upper and
 *          lower bounds. For example, if bounds are produced for a sortkey of string "smith",
 *          strings between upper and lower bounds with one level would include "Smith", "SMITH",
 *          "sMiTh".
 *
 *          There are two upper bounds that can be produced. If #I18N_UCOLLATOR_BOUND_UPPER is
 *          produced, strings matched would be as above. However, if bound produced using
 *          UCOL_BOUND_UPPER_LONG is used, the above example will also match "Smithsonian" and similar.
 *
 *          Sort keys may be compared using strcmp.
 * @since_tizen 4.0
 *
 * @param[in]    src            The source sortkey
 * @param[in]    src_length     The length of source, or -1 if null-terminated. (If an unmodified sortkey
 *                              is passed, it is always null terminated).
 * @param[in]    mode           Type of bound required. It can be #I18N_UCOLLATOR_BOUND_LOWER, which produces
 *                              a lower inclusive bound, #I18N_UCOLLATOR_BOUND_UPPER, that produces upper bound
 *                              that matches strings of the same length or #I18N_UCOLLATOR_BOUND_UPPER_LONG
 *                              that matches strings that have the same starting substring as the source string.
 * @param[in]    n_levels       Number of levels required in the resulting bound (for most uses, the
 *                              recommended value is 1). See users guide for explanation on number of
 *                              levels a sortkey can have.
 * @param[in]    dest           A pointer to a buffer to receive the resulting sortkey.
 * @param[out]   dest_length    The maximum size of result
 * @param[out]   needed_size    The size needed to fully store the bound
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_get_bound(const uint8_t *src, int32_t src_length,
							 i18n_ucollator_bound_mode_e mode, uint32_t n_levels,
							 uint8_t *dest, int32_t dest_length, int32_t *needed_size);

/**
 * @brief Gets the version information for a #i18n_ucollator_h.
 * @details Version is currently an opaque 32-bit number which depends, among other things,
 *          on major versions of the collator tailoring and UCA.
 * @since_tizen 4.0
 *
 * @param[in]    collator     The #i18n_ucollator_h to query
 * @param[in]    info         The version information, the result will be filled in
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_get_version(const i18n_ucollator_h collator, i18n_uversion_info info);

/**
 * @brief Gets the UCA version information for a #i18n_ucollator_h.
 * @since_tizen 4.0
 *
 * @param[in]    collator      The #i18n_ucollator_h to query
 * @param[in]    info          The version information, the result will be filled in
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_get_uca_version(const i18n_ucollator_h collator, i18n_uversion_info info);

/**
 * @brief Merges two sort keys.
 * @details The levels are merged with their corresponding counterparts (primaries with primaries,
 *          secondaries with secondaries etc.). Between the values from the same level a separator
 *          is inserted.
 *
 *          This is useful, for example, for combining sort keys from first and last
 *          names to sort such pairs. See http://www.unicode.org/reports/tr10/#Merging_Sort_Keys
 *
 *          The recommended way to achieve "merged" sorting is by concatenating strings with U+FFFE
 *          between them. The concatenation has the same sort order as the merged sort keys,
 *          but merge(getSortKey(str1), getSortKey(str2)) may differ from getSortKey(str1 + '' + str2).
 *          Using strings with U+FFFE may yield shorter sort keys.
 *
 *          For details about Sort Key Features see http://userguide.icu-project.org/collation/api#TOC-Sort-Key-Features
 *
 *          It is possible to merge multiple sort keys by consecutively merging another one
 *          with the intermediate result.
 *
 *          The length of the merge result is the sum of the lengths of the input sort keys.
 *
 *          Example (uncompressed):\n
 *          191B1D 01 050505 01 910505 00\n
 *          1F2123 01 050505 01 910505 00\n
 *
 *          will be merged as
 *
 *          191B1D 02 1F2123 01 050505 02 050505 01 910505 02 910505 00
 *
 *          If the destination buffer is not big enough, then its contents are undefined.
 *          If any of source lengths are zero or any of the source pointers are @c NULL / undefined,
 *          the result is of size zero.
 * @since_tizen 4.0
 *
 * @param[in]    src1           The first sort key
 * @param[in]    src1_length    The length of the first sort key, including the zero byte at the end;
 *                              can be -1 if the function is to find the length.
 * @param[in]    src2           The second sort key
 * @param[in]    src2_length    The length of the second sort key, including the zero byte at the end;
 *                              can be -1 if the function is to find the length.
 * @param[in]    dest_size      The number of bytes in the dest buffer
 * @param[out]   dest           The buffer where the merged sort key is written, can be @c NULL if dest_size==0
 * @param[out]   merged_length  The length of the merged sort key, @a src1_length+ @a src2_length; can be
 *                              larger than @a dest_size, or 0 if an error occurs (only for illegal
 *                              arguments), in which cases the contents of dest is undefined .
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_merge_sort_keys(const uint8_t *src1, int32_t src1_length,
								   const uint8_t *src2, int32_t src2_length,
								   int32_t dest_size, uint8_t *dest,
								   int32_t *merged_length);

/**
 * @brief Sets an attribute's value.
 * @since_tizen 2.3
 *
 * @param[in]    collator    The i18n_ucollator_h containing attributes to be changed
 * @param[in]    attr        The attribute type
 * @param[in]    val         The attribute value
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_set_attribute(i18n_ucollator_h collator, i18n_ucollator_attribute_e attr,
								 i18n_ucollator_attribute_value_e val);


/**
 * @brief Gets an attribute's value.
 * @since_tizen 4.0
 *
 * @param[in]     collator     The collator
 * @param[in]     attr         The attribute type
 * @param[out]    val          The attribute value
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_get_attribute(i18n_ucollator_h collator, i18n_ucollator_attribute_e attr,
								 i18n_ucollator_attribute_value_e *val);

/**
 * @brief Sets the variable top to the top of the specified reordering group.
 * @details The variable top determines the highest-sorting character which is affected by
 *          #I18N_UCOLLATOR_ALTERNATE_HANDLING. If that attribute is set to
 *          #I18N_UCOLLATOR_NON_IGNORABLE, then the variable top has no effect.
 * @since_tizen 4.0
 *
 * @param[in]    collator     The collator
 * @param[in]    group        One of:\n
 *                            #I18N_UCOLLATOR_REORDER_CODE_SPACE\n,
 *                            #I18N_UCOLLATOR_REORDER_CODE_PUNCTUATION\n,
 *                            #I18N_UCOLLATOR_REORDER_CODE_SYMBOL\n,
 *                            #I18N_UCOLLATOR_REORDER_CODE_CURRENCY\n,
 *                            #I18N_UCOLLATOR_REORDER_CODE_DEFAULT to restore the default max variable group\n
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_set_max_variable(i18n_ucollator_h collator, i18n_ucollator_reorder_code_e group);

/**
 * @brief Returns the maximum reordering group whose characters are affected by
 *        #I18N_UCOLLATOR_ALTERNATE_HANDLING.
 * @since_tizen 4.0
 *
 * @param[in]     collator     The collator
 * @param[out]    group        The maximum variable reordering group
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_get_max_variable(i18n_ucollator_h collator, i18n_ucollator_reorder_code_e *group);

/**
 * @brief Gets the variable top value of a #i18n_ucollator_h.
 * @since_tizen 4.0
 *
 * @param[in]     collator     The collator, which variable top needs to be retrieve
 * @param[out]    weight       The variable top primary weight
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_get_max_variable()
 * @see i18n_ucollator_get_variable_top()
 */
int i18n_ucollator_get_variable_top(i18n_ucollator_h collator, uint32_t *weight);

/**
 * @brief Clones the given collator, the cloning is thread-safe.
 * @since_tizen 4.0
 * @remarks The @a clone should be released using i18n_ucollator_destroy().
 *
 * @param[in]     collator      The collator to be cloned
 * @param[out]    clone         The pointer to the new clone
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_create()
 * @see i18n_ucollator_create_rules()
 * @see i18n_ucollator_destroy()
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_error_code_e error_code;
 * 	i18n_ucollator_h collator = NULL;
 * 	i18n_ucollator_h clone = NULL;
 *
 *  error_code = i18n_ucollator_create("en_US", &collator);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucollator_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	error_code = i18n_ucollator_safe_clone(collator, &clone);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucollator_safe_clone: %d\n", error_code);
 * 		i18n_ucollator_destroy(collator);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_ucollator_destroy(clone);
 * 	i18n_ucollator_destroy(collator);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucollator_safe_clone(i18n_ucollator_h collator, i18n_ucollator_h *clone);

/**
 * @brief Returns the current rules.
 * @details Delta defines whether full rules are returned or just the tailoring. Returns number of
 *          #i18n_uchar needed to store rules. If @a dest is @c NULL or @a dest_size is not enough to
 *          store rules, will store up to available space.
 *          See http://userguide.icu-project.org/collation/customization#TOC-Building-on-Existing-Locales
 * @remarks i18n_ucollator_get_rules() should normally be used instead.
 * @since_tizen 4.0
 *
 * @param[in]     collator        The collator to get the rules from
 * @param[in]     option          The first sort key
 * @param[in]     dest_size       The length of buffer to store rules in. If less than needed you'll get
 *                                only the part that fits in.
 * @param[out]    dest            The buffer to store the result in. If @c NULL, you'll get no rules.
 * @param[out]    current_rules   The current rules.
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see #I18N_UCOLLATOR_FULL_RULES
 */
int i18n_ucollator_get_rules_ex(i18n_ucollator_h collator, i18n_ucollator_rule_option_e option,
								int32_t dest_size, i18n_uchar *dest,
								int32_t *current_rules);

/**
 * @brief Gets the locale name of the collator.
 * @details If the collator is instantiated from the rules, then this function returns @c NULL.
 * @since_tizen 4.0
 * @remarks The @a locale should not be released.
 *
 * @param[in]      collator     The #i18n_ucollator_h for which the locale is needed
 * @param[in]      type         You can choose between requested, valid and actual locale. For description see
 *                              the definition of #i18n_ulocale_data_locale_type_e.
 * @param[out]     locale       The real locale name from which the collation data comes. If the collator
 *                              was instantiated from rules, returns @c NULL.
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucollator_get_locale_by_type(i18n_ucollator_h collator, i18n_ulocale_data_locale_type_e type,
									  const char **locale);

/**
 * @brief Gets a Unicode set that contains all the characters and sequences tailored in this collator.
 * @details The result must be disposed of by using i18n_uset_destroy().
 * @since_tizen 4.0
 * @remarks The @a uset should not be released.
 *
 * @param[in]     collator       The #i18n_ucollator_h for which we want to get tailored chars
 * @param[out]    uset           A pointer to the newly created #i18n_uset_h. Must be be disposed by using
 *                               i18n_uset_destroy().
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_create_rules()
 * @see i18n_ucollator_destroy()
 */
int i18n_ucollator_get_tailored_set(i18n_ucollator_h collator, i18n_uset_h *uset);

/**
 * @brief Creates a binary image of a collator.
 * @details This binary image can be stored and later used to instantiate a collator using
 *          i18n_ucollator_create_binary(). This API supports preflighting.
 * @since_tizen 4.0
 *
 * @param[in]    collator    The collator
 * @param[in]    dest_size   Capacity of the dest buffer
 * @param[out]   dest        A fill-in buffer to receive the binary image
 * @param[out]   image_size  The size of the image
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_create_binary()
 */
int i18n_ucollator_clone_binary(i18n_ucollator_h collator, int32_t dest_size,
								uint8_t *dest, int32_t *image_size);

/**
 * @brief Creates a collator from a collator binary image created using i18n_ucollator_create_binary().
 * @details Binary image used in instantiation of the collator remains owned by the user
 *          and should stay around for the lifetime of the collator. The API also takes a base
 *          collator which must be the root collator.
 * @since_tizen 4.0
 *
 * @param[in]    bin       The binary image owned by the user and required through the lifetime of the collator
 * @param[in]    length    The size of the image. If negative, the API will try to figure out
 *                         the length of the image.
 * @param[in]    base      Base collator, for lookup of untailored characters.
 *                         Must be the root collator, must not be @c NULL.
 *                         The base is required to be present through the lifetime of the collator.
 * @param[in]    collator  The newly created collator
 *
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucollator_clone_binary()
 */
int i18n_ucollator_create_binary(const uint8_t *bin, int32_t length,
								 i18n_ucollator_h base, i18n_ucollator_h *collator);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
