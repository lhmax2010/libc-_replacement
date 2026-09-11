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
 * @file utils_i18n_plural_format.h
 * @version 0.1
 * @brief utils_i18n_plural_format
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE PluralFormat
 * @brief The Plural Format is a formatter for Plural objects.
 * @section CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE_HEADER Required Header
 *         \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE_OVERVIEW Overview
 * @details Plural Format supports the creation of internationalized messages with plural inflection.
 *          It is based on plural selection, i.e. the caller specifies messages for each plural case
 *          that can appear in the user's language and the Plural Format selects the appropriate message
 *          based on the number.\n
 *
 *          <b>The Problem of Plural Forms in Internationalized Messages:</b>\n
 *
 *          Different languages have different ways to inflect plurals. Creating internationalized messages
 *          that include plural forms is only feasible when the framework is able to handle plural forms
 *          of all languages correctly. In some languages, like Polish, one plural case applies to infinitely many intervals
 *          (e.g., the plural case applies to numbers ending with 2, 3, or 4 except those ending with 12, 13, or 14).
 *
 *          Plural Format deals with this by breaking the problem into two parts:\n
 *          <ul>
 *           <li> It uses Plural Rules that can define more complex conditions for a plural case than just a single interval.
 *                These plural rules define both what plural cases exist in a language,
 *                and to which numbers these cases apply.
 *           </li>
 *           <li> It provides predefined plural rules for many languages. Thus, the programmer need not worry
 *                about the plural cases of a language and does not have to define the plural cases;
 *                they can simply use the predefined keywords. The whole plural formatting of messages can be done
 *                using localized patterns from resource bundles. For predefined plural rules,
 *                see the CLDR Language Plural Rules page at
 *                http://unicode.org/repos/cldr-tmp/trunk/diff/supplemental/language_plural_rules.html
 *           </li>
 *          </ul>
 *          <b>Usage of Plural Format:</b>\n
 *
 *          This discussion assumes that you use Plural Format with a predefined set of plural rules.
 *          You can create one using one of the constructors that takes a locale object.
 *          To specify the message pattern, you can either pass it to the constructor or set it explicitly
 *          using the i18n_plural_format_apply_pattern() function.
 *          The i18n_plural_format_format_* functions takes a number object and selects the message
 *          of the matching plural case. This message will be returned.\n
 *
 *          <b>Patterns and Their Interpretation:</b>\n
 *
 *          The pattern text defines the message output for each plural case of the specified locale.
 *          Syntax:\n
 *
 *          @code
 *          pluralStyle = [offsetValue] (selector '{' message '}')+
 *          offsetValue = "offset:" number
 *          selector = explicitValue | keyword
 *          explicitValue = '=' number  // adjacent, no white space in between
 *          keyword = [^[[:Pattern_Syntax:][:Pattern_White_Space:]]]+
 *          message: message = messageText (argument messageText)*
 *          argument = noneArg | simpleArg | complexArg
 *          complexArg = pluralArg | selectordinalArg
 *          noneArg = '{' argNameOrNumber '}'
 *          simpleArg = '{' argNameOrNumber ',' argType [',' argStyle] '}'
 *          pluralArg = '{' argNameOrNumber ',' "plural" ',' pluralStyle '}'
 *          selectordinalArg = '{' argNameOrNumber ',' "selectordinal" ',' pluralStyle '}'
 *          argNameOrNumber = argName | argNumber
 *          argName = [^[[:Pattern_Syntax:][:Pattern_White_Space:]]]+
 *          argNumber = '0' | ('1'..'9' ('0'..'9')*)
 *          argType = "number" | "date" | "time" | "spellout" | "ordinal" | "duration"
 *          argStyle = "short" | "medium" | "long" | "full" | "integer" | "currency" | "percent" | argStyleText
 *          @endcode
 *
 *          Examples:\n
 *          @code
 *          one{dog} other{dogs}                        // English plural format for dog
 *          one{pes} two{psa} few{psi} other{psov}      //Slovenian translation of dog in plural format
 *          @endcode
 *
 *          Pattern_White_Space between syntax elements is ignored, except between the {curly braces}
 *          and their sub-message, and between the '=' and the number of an explicitValue.
 *
 *          There are 6 predefined casekeyword in CLDR/ICU - 'zero', 'one', 'two', 'few', 'many' and 'other'.
 *          You always have to define a message text for the default plural case other
 *          which is contained in every rule set. If you do not specify a message text for a particular plural case,
 *          the message text of the plural case other gets assigned to this plural case.
 *
 *          When formatting, the input number is first matched against the explicitValue clauses.
 *          If there is no exact-number match, then a keyword is selected by calling the Plural Rules
 *          with the input number minus the offset. (The offset defaults to 0 if it is omitted from the pattern string.)
 *          If there is no clause with that keyword, then the "other" clauses is returned.
 *
 *          An unquoted pound sign (#) in the selected sub-message itself
 *          (i.e., outside of arguments nested in the sub-message) is replaced by the input number minus the offset.
 *          The number-minus-offset value is formatted using a Number Format for the Plural Format's locale.\n
 *
 *          Note: That argument is formatting without subtracting the offset!
 *          If you need a custom format and have a non-zero offset, then you need to pass the
 *          number-minus-offset value as a separate parameter.
 *
 *          <b>Defining Custom Plural Rules:</b>\n
 *
 *          If you need to use Plural Format with custom rules, you can create a Plural Rules object
 *          and pass it to Plural Format's constructor. If you also specify a locale in this constructor,
 *          this locale will be used to format the number in the message texts.
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE
 * @{
 */

/**
 * @brief Creates a new cardinal-number plural format object for the default locale.
 * @details This locale will be used to get the set of plural rules and for standard number formatting.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *          i18n_plural_format_destroy() function.
 *
 * @param[out] plural_format    The created plural format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_plural_format_create(i18n_plural_format_h *plural_format);

 /**
  * @brief Creates a new cardinal-number plural format object for a given locale.
  * @since_tizen 5.0
  * @remarks The created object should be released by the caller with the
  *          i18n_plural_format_destroy() function.
  *
  * @param[in]  locale           The locale string containing language and country code
  * @param[out] plural_format    The created plural format object
  *
  * @return @c 0 on success, otherwise a negative error value
  * @retval #I18N_ERROR_NONE                Successful
  * @retval #I18N_ERROR_INVALID_PARAMETER   Invalid function parameter
  * @retval #I18N_ERROR_OUT_OF_MEMORY       Out of memory
  */
int i18n_plural_format_create_from_locale(const char *locale,
										  i18n_plural_format_h *plural_format);

/**
 * @brief Creates a new plural format object for a given set of rules, a pattern and a default locale.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *          i18n_plural_format_destroy() function.
 *
 * @param[in]  plural_rules     Plural rules which defines the behavior of the plural format object
 * @param[in]  pattern          The pattern for this plural format object
 * @param[out] plural_format    The created plural format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_plural_format_create_from_rules_pattern(i18n_plural_rules_h plural_rules,
												 const char *pattern,
												 i18n_plural_format_h *plural_format);

/**
 * @brief Creates a new plural format object for a plural type, a pattern and a locale.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *          i18n_plural_format_destroy() function.
 *
 * @param[in]  locale           The locale string containing language and country code
 * @param[in]  type             The plural type
 * @param[in]  pattern          The pattern for this plural format object
 * @param[out] plural_format    The created plural format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_plural_format_create_from_locale_type_pattern(const char *locale,
													   i18n_uplural_type_e type,
													   const char *pattern,
													   i18n_plural_format_h *plural_format);

/**
 * @brief Creates a new plural format object for the plural type.
 * @details The standard number formatting will be done using the given locale.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *          i18n_plural_format_destroy() function.
 *
 * @param[in]  locale           The locale string containing language and country code
 * @param[in]  type             The plural type
 * @param[out] plural_format    The created plural format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_plural_format_create_from_locale_type(const char *locale,
											   i18n_uplural_type_e type,
											   i18n_plural_format_h *plural_format);

/**
 * @brief Creates a new cardinal-number plural format object for a given pattern string and locale.
 * @details The locale will be used to get the set of plural rules and for standard number formatting.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *          i18n_plural_format_destroy() function.
 *
 * @param[in]  locale           The locale string containing language and country code
 * @param[in]  pattern          The pattern for this plural format object
 * @param[out] plural_format    The created plural format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_plural_format_create_from_locale_pattern(const char *locale,
												  const char *pattern,
												  i18n_plural_format_h *plural_format);

/**
 * @brief Creates a new plural format object for a given set of rules, a pattern and a locale.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *          i18n_plural_format_destroy() function.
 *
 * @param[in]  locale           The locale string containing language and country code
 * @param[in]  plural_rules     Plural rules which defines the behavior of the plural format object
 * @param[in]  pattern          The pattern for this plural format object
 * @param[out] plural_format    The created plural format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_plural_format_create_from_locale_rules_pattern(const char *locale,
														i18n_plural_rules_h plural_rules,
														const char *pattern,
														i18n_plural_format_h *plural_format);

/**
 * @brief Destroys the plural format object.
 * @since_tizen 5.0
 *
 * @param[in] plural_format    The plural format object to destroy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_plural_format_destroy(i18n_plural_format_h plural_format);

/**
 * @brief Creates a polymorphic clone of the given @a plural_format object.
 * @since_tizen 5.0
 * @remarks The @a clone object should be released by the caller with the
 *          i18n_plural_format_destroy() function.
 *
 * @param[in]  plural_format    The plural format object to be cloned
 * @param[out] clone            The created plural format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_plural_format_clone(i18n_plural_format_h plural_format, i18n_format_h *clone);

/**
 * @brief Sets the pattern used by this plural format.
 * @details The method parses the pattern and creates a map of format strings for the plural rules.
 *          Patterns and their interpretation are specified in the module overview.
 * @since_tizen 5.0
 *
 * @param[in] plural_format The plural format object to set the pattern
 * @param[in] pattern       The pattern to set in plural format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 */
int i18n_plural_format_apply_pattern(i18n_plural_format_h plural_format, const char *pattern);

/**
 * @brief Formats a plural message for a given int32_t number.
 * @since_tizen 5.0
 *
 * @param[in]     plural_format     The format object for which number will be formatted
 * @param[in]     number            Number to format
 * @param[in]     field_position    The #i18n_field_position_h object
 * @param[in]     append_to_size    The size of the @a append_to buffer.
 *                                  If <code>@a append_to_size <= 0</code> then output string is not
 *                                  appended to the @a append_to buffer and #I18N_ERROR_BUFFER_OVERFLOW
 *                                  is returned.
 * @param[in,out] append_to         The buffer to which formatted string will be appended.
 *                                  Can be @c NULL if <code>@a append_to_size <= 0</code>. Otherwise
 *                                  must be @c NULL terminated.
 * @param[out]    output_length     The length of the output string, which is the string in
 *                                  @a append_to buffer with formatted string.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW      Buffer overflow
 */
int i18n_plural_format_format_int32(i18n_plural_format_h plural_format,
	 	 	 	 	 	 	 	 	int32_t number,
									i18n_field_position_h field_position,
									int32_t append_to_size,
									i18n_uchar *append_to,
									int32_t *output_length);

/**
 * @brief Formats a plural message for a given double number.
 * @since_tizen 5.0
 *
 * @param[in]     plural_format     The format object for which number will be formatted
 * @param[in]     number            Number to format
 * @param[in]     field_position    The #i18n_field_position_h object
 * @param[in]     append_to_size    The size of the @a append_to buffer.
 *                                  If <code>@a append_to_size <= 0</code> then output string is not
 *                                  appended to the @a append_to buffer and #I18N_ERROR_BUFFER_OVERFLOW
 *                                  is returned.
 * @param[in,out] append_to         The buffer to which formatted string will be appended.
 *                                  Can be @c NULL if <code>@a append_to_size <= 0</code>. Otherwise
 *                                  must be @c NULL terminated.
 * @param[out]    output_length     The length of the output string, which is the string in
 *                                  @a append_to buffer with formatted string.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW      Buffer overflow
 */
int i18n_plural_format_format_double(i18n_plural_format_h plural_format,
		 	 	 	 	 	 	 	 double number,
									 i18n_field_position_h field_position,
									 int32_t append_to_size,
									 i18n_uchar *append_to,
									 int32_t *output_length);

/**
 * @brief Formats a plural message for a number taken from a i18n_formattable_h object.
 * @since_tizen 5.0
 *
 * @param[in]     plural_format     The format object for which number will be formatted
 * @param[in]     formattable       The object containing a number for which the plural message
 *                                  should be formatted. The object must be of a numeric type.
 * @param[in]     field_position    The #i18n_field_position_h object
 * @param[in]     append_to_size    The size of the @a append_to buffer.
 *                                  If <code>@a append_to_size <= 0</code> then output string is not
 *                                  appended to the @a append_to buffer and #I18N_ERROR_BUFFER_OVERFLOW
 *                                  is returned.
 * @param[in,out] append_to         The buffer to which formatted string will be appended.
 *                                  Can be @c NULL if <code>@a append_to_size <= 0</code>. Otherwise
 *                                  must be @c NULL terminated.
 * @param[out]    output_length     The length of the output string, which is the string in
 *                                  @a append_to buffer with formatted string.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW      Buffer overflow
 */
int i18n_plural_format_format_formattable(i18n_plural_format_h plural_format,
										  i18n_formattable_h formattable,
										  i18n_field_position_h field_position,
										  int32_t append_to_size,
										  i18n_uchar *append_to,
										  int32_t *output_length);

/**
 * @brief Sets the number format used by this formatter.
 * @details You only need to call this if you want a different number format than the default formatter for the locale.
 * @since_tizen 5.0
 *
 * @param[in] plural_format     The format object to set
 * @param[in] number_format     The number format to use
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 */
int i18n_plural_format_set_number_format(i18n_plural_format_h plural_format, i18n_unumber_format_h number_format);

/**
 * @brief Returns the pattern from i18n_plural_format_apply_pattern() or i18n_plural_format_create_pattern().
 * @since_tizen 5.0
 *
 * @param[in]     plural_format     The format object to get pattern from
 * @param[in]     field_position    The #i18n_field_position_h object
 * @param[in]     append_to_size    The size of the @a append_to buffer.
 *                                  If <code>@a append_to_size <= 0</code> then output string is not
 *                                  appended to the @a append_to buffer and #I18N_ERROR_BUFFER_OVERFLOW
 *                                  is returned.
 * @param[in,out] append_to         The buffer to which formatted string will be appended.
 *                                  Can be @c NULL if <code>@a append_to_size <= 0</code>. Otherwise
 *                                  must be @c NULL terminated.
 * @param[out]    output_length     The length of the output string, which is the string in
 *                                  @a append_to buffer with formatted string.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW      Buffer overflow
 */
int i18n_plural_format_to_pattern(i18n_plural_format_h plural_format,
								  i18n_field_position_h field_position,
								  int32_t append_to_size,
								  i18n_uchar *append_to,
								  int32_t *output_length);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
