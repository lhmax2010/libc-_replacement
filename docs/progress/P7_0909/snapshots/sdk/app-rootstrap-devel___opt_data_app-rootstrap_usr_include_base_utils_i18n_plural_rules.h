/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @file utils_i18n_plural_rules.h
 * @version 0.1
 * @brief utils_i18n_plural_rules
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE PluralRules
 * @brief Defines rules for mapping non-negative numeric values onto a small set of keywords.
 * @section CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE_HEADER Required Header
 *         \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE_OVERVIEW Overview
 * @details Rules are constructed from a text description, consisting of a series of
 *          keywords and conditions. The select method examines each condition in order and returns
 *          the keyword for the first condition that matches the number.
 *          If none match, default rule(other) is returned.
 *          For more information, details, and tips for writing rules, see the LDML spec,
 *          C.11 Language Plural Rules: https://www.unicode.org/reports/tr35/tr35-53/tr35-numbers.html#Language_Plural_Rules \n\n
 *
 *          Examples:\n
 *
 *          "one: n is 1; few: n in 2..4" \n
 *          This defines two rules, for 'one' and 'few'. The condition for 'one' is "n is 1"
 *          which means that the number must be equal to 1 for this condition to pass.
 *          The condition for 'few' is "n in 2..4" which means that the number must be
 *          between 2 and 4 inclusive for this condition to pass.
 *          All other numbers are assigned the keyword "other" by the default rule.
 *
 *          "zero: n is 0; one: n is 1; zero: n mod 100 in 1..19" \n
 *          This illustrates that the same keyword can be defined multiple times.
 *          Each rule is examined in order, and the first keyword whose condition passes is the one returned.
 *          Also notes that a modulus is applied to n in the last rule.
 *          Thus its condition holds for 119, 219, 319...
 *
 *          "one: n is 1; few: n mod 10 in 2..4 and n mod 100 not in 12..14" \n
 *          This illustrates conjunction and negation. The condition for 'few' has two parts, both of which must be met:
 *          "n mod 10 in 2..4" and "n mod 100 not in 12..14". The first part applies a modulus to n
 *          before the test as in the previous example. The second part applies a different modulus
 *          and also uses negation, thus it matches all numbers not in 12, 13, 14, 112, 113, 114, 212, 213, 214...
 *
 *          Syntax: \n
 *
 *          @code
 *          rules           = rule (';' rule)*
 *          rule            = keyword ':' condition
 *          keyword         = <identifier>
 *          condition       = and_condition ('or' and_condition)*
 *          and_condition   = relation ('and' relation)*
 *          relation        = is_relation | in_relation | within_relation | 'n' <EOL>
 *          is_relation     = expr 'is' ('not')? value
 *          in_relation     = expr ('not')? 'in' range_list
 *          within_relation = expr ('not')? 'within' range
 *          expr            = ('n' | 'i' | 'f' | 'v' | 'j') ('mod' value)?
 *          range_list      = (range | value) (',' range_list)*
 *          value           = digit+  ('.' digit+)?
 *          digit           = 0|1|2|3|4|5|6|7|8|9
 *          range           = value'..'value
 *          @endcode
 *
 *          The i, f, and v values are defined as follows: \n
 *          <ul>
 *              <li> i to be the integer digits. </li>
 *              <li> f to be the visible fractional digits, as an integer. </li>
 *              <li> v to be the number of visible fraction digits. </li>
 *              <li> j is defined to only match integers. That is "j is 3" fails if v != 0 (eg for 3.1 or 3.0). </li>
 *          </ul>
 *
 *          Examples are in the following table: \n
 *
 *          @code
 *          n       1.0     1.00    1.3     1.03    1.23
 *
 *          i       1       1       1       1       1
 *
 *          f       0       0       3       3       23
 *
 *          v       1       2       2       2       2
 *          @endcode
 *
 *          The difference between 'in' and 'within' is that 'in' only includes integers in the specified range,
 *          while 'within' includes all values. Using 'within' with a range_list consisting entirely of values
 *          is the same as using 'in' (it's not an error). An "identifier" is a sequence of characters
 *          that do not have the Unicode Pattern_Syntax or Pattern_White_Space properties.
 *          The difference between 'in' and 'within' is that 'in' only includes integers in the specified range,
 *          while 'within' includes all values. Using 'within' with a range_list consisting entirely of values
 *          is the same as using 'in' (it's not an error). Keywords could be defined by users or from ICU locale data.
 *          There are 6 predefined values in ICU - 'zero', 'one', 'two', 'few', 'many' and 'other'.
 *          Callers need to check the value of keyword returned by select method. \n\n
 *
 *          Example:
 *
 *          @code
 *          char keyword[BUFSIZE] = { 0 };
 *          int32_t number = 10
 *          int32_t output_length = -1;
 *          i18n_plural_rules_select_int32(plural_rules, number, BUFSIZE, keyword, &output_length);
 *          if (strncmp(keyword, "one", output_length) == 0) {
 *               ...
 *          }
 *          else if ( ... )
 *          @endcode
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE
 * @{
 */

/**
 * @brief Creates a plural rules object.
 * @since_tizen 4.0
 * @remarks The created object should be released by the caller with the
 *            i18n_plural_rules_destroy() function.
 *
 * @param[out] plural_rules   The created plural rules object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_plural_rules_create(i18n_plural_rules_h *plural_rules);

/**
 * @brief Destroys the plural rules object.
 * @since_tizen 4.0
 *
 * @param[in] plural_rules    The plural rules object to destroy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_plural_rules_destroy(i18n_plural_rules_h plural_rules);

/**
 * @brief Creates a polymorphic clone of the given @a plural_rules object.
 * @since_tizen 4.0
 * @remarks The @a clone object should be released by the caller with the
 *          i18n_plural_rules_destroy() function.
 *
 * @param[in]  plural_rules   The plural rules object to be cloned
 * @param[out] clone          The created plural rules object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY     Out of memory
 */
int i18n_plural_rules_clone(i18n_plural_rules_h plural_rules, i18n_plural_rules_h *clone);

/**
 * @brief Creates a plural rules object from a description.
 * @details If the description is not parsable, plural_rules will be @c NULL.
 * @since_tizen 4.0
 * @remarks The created object should be released by the caller with the
 *          i18n_plural_rules_destroy() function.
 *
 * @param[in]  description   Rule description
 * @param[out] plural_rules  The created plural rules object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_plural_rules_create_rules_from_descr(const char *description, i18n_plural_rules_h *plural_rules);

/**
 * @deprecated Deprecated since 5.0. Use i18n_plural_rules_for_locale_id() instead.
 * @brief Provides access to the predefined plural rules object for a given locale and the plural type.
 * @details If there's no predefined rules for this locale, the rules for the closest parent
 *          in the locale hierarchy that has one will be returned.
 *          The final fallback always returns the default 'other' rules.
 * @since_tizen 4.0
 * @remarks The created object should be released by the caller with the
 *          i18n_plural_rules_destroy() function.
 *
 * @param[in]  language         The language of the locale
 * @param[in]  country          The country of the locale
 * @param[in]  type             The plural type (e.g., cardinal or ordinal)
 * @param[out] plural_rules     The pointer to the plural rules object for given locale
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY     Out of memory
 */
int i18n_plural_rules_for_locale(const char *language,
								 const char *country,
								 i18n_uplural_type_e type,
								 i18n_plural_rules_h *plural_rules) TIZEN_DEPRECATED_API;

/**
 * @brief Returns the keyword of the first rule that applies to the given number.
 * @details This function can be used with i18n_plural_rules_is_keyword() function to determine
 *          the keyword for default plural rules.
 * @since_tizen 4.0
 *
 * @param[in]     plural_rules          The plural rules object
 * @param[in]     number                The number for which the rule has to be determined
 * @param[in]     output_buffer_size    The size of the @a output_buffer buffer.
 *                                      If <code>@a output_buffer_size <= 0</code> then output string is not
 *                                      insert to the @a output_buffer buffer and #I18N_ERROR_BUFFER_OVERFLOW
 *                                      is returned.
 * @param[out]    output_buffer         The buffer to which formatted string will be inserted.
 *                                      Can be @c NULL if <code>@a output_buffer_size <= 0</code>.
 * @param[out]    output_length         The length of the output string
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW   Buffer overflow
 */
int i18n_plural_rules_select_int32(i18n_plural_rules_h plural_rules,
								   int32_t number,
								   int32_t output_buffer_size,
								   i18n_uchar *output_buffer,
								   int32_t *output_length);

/**
 * @brief Returns the keyword of the first rule that applies to the given number.
 * @details This function can be used with i18n_plural_rules_is_keyword() function to determine
 *          the keyword for default plural rules.
 * @since_tizen 4.0
 *
 * @param[in]     plural_rules          The plural rules object
 * @param[in]     number                The number for which the rule has to be determined
 * @param[in]     output_buffer_size    The size of the @a output_buffer buffer.
 *                                      If <code>@a output_buffer_size <= 0</code> then output string is not
 *                                      insert to the @a output_buffer buffer and #I18N_ERROR_BUFFER_OVERFLOW
 *                                      is returned.
 * @param[out]    output_buffer         The buffer to which formatted string will be inserted.
 *                                      Can be @c NULL if <code>@a output_buffer_size <= 0</code>.
 * @param[out]    output_length         The length of the output string
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW   Buffer overflow
 */
int i18n_plural_rules_select_double(i18n_plural_rules_h plural_rules,
									double number,
									int32_t output_buffer_size,
									i18n_uchar *output_buffer,
									int32_t *output_length);

/**
 * @brief Returns a list of all rule keywords used in this plural rules object.
 * @details The rule 'other' is always present by default.
 * @since_tizen 4.0
 * @remarks The obtained @a string_enum object should be released by the caller
 *          with the i18n_uenumeration_destroy() function.
 *
 * @param[in]  plural_rules  The plural rules object
 * @param[out] string_enum   String enumeration containing keywords
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_plural_rules_get_keywords(i18n_plural_rules_h plural_rules, i18n_uenumeration_h *string_enum);

/**
 * @brief Gets sample values for which i18n_plural_rules_select_* functions would return the keyword.
 * @details If the keyword is unknown, returns no values, but this is not an error.
 *          The number of returned values is typically small.
 * @since_tizen 4.0
 *
 * @param[in]  plural_rules     The plural rules object
 * @param[in]  keyword          The keyword
 * @param[out] dest             Array into which to put the returned values. May be @c NULL if @a dest_capacity is 0
 * @param[in]  dest_capacity    The capacity of the array, must be at least 0
 * @param[out] count            The count of values written. If more than @a dest_capacity samples are available,
 *                              then only @a dest_capacity are written, and @a dest_capacity is returned as the count,
 *                              rather than setting a #I18N_ERROR_BUFFER_OVERFLOW.
 *                              (The actual number of keyword values could be unlimited.)
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_plural_rules_get_samples(i18n_plural_rules_h plural_rules, const char *keyword, double *dest, int32_t dest_capacity, int32_t *count);

/**
 * @brief Checks if the given keyword is defined in a plural rules object.
 * @since_tizen 4.0
 *
 * @param[in]  plural_rules  The plural rules object
 * @param[in]  keyword       The input keyword
 * @param[out] is_keyword    Boolean value indicating if the given keyword is defined in @a plural_rules
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_plural_rules_is_keyword(i18n_plural_rules_h plural_rules, const char *keyword, i18n_ubool *is_keyword);

/**
 * @brief Returns keyword for default plural form.
 * @since_tizen 4.0
 *
 * @param[in]     plural_rules          The plural rules object
 * @param[in]     output_buffer_size    The size of the @a output_buffer buffer.
 *                                      If <code>@a output_buffer_size <= 0</code> then output string is not
 *                                      insert to the @a output_buffer buffer and #I18N_ERROR_BUFFER_OVERFLOW
 *                                      is returned.
 * @param[out]    output_buffer         The buffer to which formatted string will be inserted.
 *                                      Can be @c NULL if <code>@a output_buffer_size <= 0</code>.
 * @param[out]    output_length         The length of the output string
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW   Buffer overflow
 */
int i18n_plural_rules_get_keyword_other(i18n_plural_rules_h plural_rules,
										int32_t output_buffer_size,
										i18n_uchar *output_buffer,
										int32_t *output_length);

/**
 * @brief Provides access to the predefined plural rules object for a given locale and the plural type.
 * @details If there's no predefined rules for this locale, the rules for the closest parent
 *          in the locale hierarchy that has one will be returned.
 *          The final fallback always returns the default 'other' rules.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *          i18n_plural_rules_destroy() function.
 *
 * @param[in]  locale_id        The locale string containing language and country code
 * @param[in]  type             The plural type (e.g., cardinal or ordinal)
 * @param[out] plural_rules     The pointer to the plural rules object for given locale
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY     Out of memory
 */
int i18n_plural_rules_for_locale_id(const char *locale_id,
								 	i18n_uplural_type_e type,
									i18n_plural_rules_h *plural_rules);



/**
 * @}
 */

#ifdef __cplusplus
}
#endif
