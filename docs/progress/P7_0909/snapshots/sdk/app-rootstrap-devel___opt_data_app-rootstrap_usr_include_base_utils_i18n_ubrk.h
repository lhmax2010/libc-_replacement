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
 * Copyright (C) 1996-2013, International Business Machines Corporation and others.
 * All Rights Reserved.
 */
#pragma once

#include <utils_i18n_types.h>

/**
 * @file utils_i18n_ubrk.h
 * @version 0.1
 * @brief utils_i18n_ubrk
 */

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UBRK_MODULE Ubrk
 * @brief Ubrk defines methods for finding the location of boundaries in text.
 *
 * @section CAPI_BASE_UTILS_I18N_UBRK_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UBRK_MODULE_OVERVIEW Overview
 * @details Pointer to a #i18n_ubreak_iterator_h maintains a current position and scans over text returning the index of characters where boundaries occur.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UBRK_MODULE
 * @{
 */

/**
 * @brief Opens a new #i18n_ubreak_iterator_h for locating text boundaries for a specified locale.
 * @details A #i18n_ubreak_iterator_h may be used for detecting character, line, word,
 *		and sentence breaks in text.
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 * @param[in] type The type of #i18n_ubreak_iterator_h to open: one of #I18N_UBRK_CHARACTER, #I18N_UBRK_WORD,
 * #I18N_UBRK_LINE, #I18N_UBRK_SENTENCE
 * @param[in] locale		The locale specifying the text-breaking conventions.
 *							If @c NULL, the default locale will be used.
 * @param[in] text			The text to be iterated over. May be @c NULL, then the iterator will be created without any text.
 *							The text can be set later with i18n_ubrk_set_text() function.
 * @param[in] text_length	The number of characters in text, or -1 if NULL-terminated.
 * @param[out] break_iter	A pointer to the #i18n_ubreak_iterator_h for the specified locale.
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubrk_create_rules()
 */
int i18n_ubrk_create(i18n_ubreak_iterator_type_e type, const char *locale, const i18n_uchar *text, int32_t text_length, i18n_ubreak_iterator_h *break_iter);

/**
 * @brief Opens a new #i18n_ubreak_iterator_h for locating text boundaries using specified breaking rules.
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 * @remarks The @a break_iter and @a parse_err should not be released.
 * @param[in] rules			A set of rules specifying the text breaking conventions.
 * @param[in] rules_length	The number of characters in rules, or -1 if NULL-terminated.
 * @param[in] text			The text to be iterated over.  May be @c NULL, in which case i18n_ubrk_set_text() is
 *							used to specify the text to be iterated.
 * @param[in] text_length	The number of characters in text, or -1 if NULL-terminated.
 * @param[out] break_iter	A pointer to the #i18n_ubreak_iterator_h for the specified rules.
 * @param[out] parse_err	Receives position and context information for any syntax errors
 *							detected while parsing the rules.
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubrk_create()
 */
int i18n_ubrk_create_rules(const i18n_uchar *rules, int32_t rules_length, const i18n_uchar *text, int32_t text_length, i18n_ubreak_iterator_h *break_iter, i18n_uparse_error_s *parse_err);

/**
 * @brief Thread safe cloning operation.
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 * @remarks The @a break_iter_clone should not be released.
 * @param[in] break_iter iterator to be cloned. Must not be @c NULL.
 * @param[in] stack_buffer	(Deprecated Since 3.0. Use NULL instead.) User allocated space for the new clone. If @c NULL new memory will be allocated.
 *							If buffer is not large enough, new memory will be allocated.
 *							Clients can use the #I18N_U_BRK_SAFECLONE_BUFFERSIZE. This will probably be enough to avoid memory allocations.
 * @param[in] p_buffer_size (Deprecated Since 3.0. Use NULL instead.) A pointer to size of allocated space.
 *							If <code>*p_buffer_size == 0</code>, a sufficient size for use in cloning will
 *							be returned ('pre-flighting')
 *							If *p_buffer_size is not enough for a stack-based safe clone,
 *							new memory will be allocated.
 * @param[out] break_iter_clone	  A pointer to the cloned #i18n_ubreak_iterator_h.
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ubrk_safe_clone(const i18n_ubreak_iterator_h break_iter, void *stack_buffer, int32_t *p_buffer_size, i18n_ubreak_iterator_h *break_iter_clone);

/**
 * @brief Closes a #i18n_ubreak_iterator_h.
 * @details Once closed, a #i18n_ubreak_iterator_h may no longer be used.
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 * @param[in] break_iter The break iterator to close. Must not be @c NULL.
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
*/
int i18n_ubrk_destroy(i18n_ubreak_iterator_h break_iter);

/**
 * @brief Sets an existing iterator to point to a new piece of text.
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 * @param[in] break_iter			The iterator to use. Must not be @c NULL.
 * @param[in] text			The text to be set. Must not be @c NULL.
 * @param[in] text_length	The length of the text.
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ubrk_set_text(i18n_ubreak_iterator_h break_iter, const i18n_uchar *text, int32_t text_length);

/**
 * @brief Determines the most recently-returned text boundary.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 * @param[in] break_iter The break iterator to use. Must not be @c NULL.
 * @return The character index most recently returned by, i18n_ubrk_next(), i18n_ubrk_previous(),
 *	i18n_ubrk_first(), or i18n_ubrk_last().
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ubrk_current(const i18n_ubreak_iterator_h break_iter);

/**
 * @brief Advances the iterator to the boundary following the current boundary.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 * @param[in] break_iter The break iterator to use. Must not be @c NULL.
 * @return The character index of the next text boundary, or #I18N_UBRK_DONE
 *		if all text boundaries have been returned.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubrk_previous()
 */
int32_t i18n_ubrk_next(i18n_ubreak_iterator_h break_iter);

/**
 * @brief Sets the iterator position to the boundary preceding the current boundary.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 * @param[in] break_iter The break iterator to use. Must not be @c NULL.
 * @return The character index of the preceding text boundary, or #I18N_UBRK_DONE
 *		if all text boundaries have been returned.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubrk_next()
 */
int32_t i18n_ubrk_previous(i18n_ubreak_iterator_h break_iter);

/**
 * @brief Sets the iterator position to zero, the start of the text being scanned.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 * @param[in] break_iter The break iterator to use. Must not be @c NULL.
 * @return The new iterator position (zero).
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubrk_last()
 */
int32_t i18n_ubrk_first(i18n_ubreak_iterator_h break_iter);

/**
 * @brief Sets the iterator position to the index immediately <EM>beyond</EM> the last character in the text being scanned.
 * @details This is not the same as the last character.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 * @param[in] break_iter The break iterator to use. Must not be @c NULL.
 * @return The character offset immediately <EM>beyond</EM> the last character in the
 * text being scanned.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubrk_first()
 */
int32_t i18n_ubrk_last(i18n_ubreak_iterator_h break_iter);

/**
 * @brief Sets the iterator position to the first boundary preceding the specified @a offset.
 * @details The new position is always smaller than @a offset, or #I18N_UBRK_DONE.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 * @param[in] break_iter		The break iterator to use. Must not be @c NULL.
 * @param[in] offset	The offset to begin scanning.
 * @return The text boundary preceding offset, or #I18N_UBRK_DONE.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubrk_following()
 */
int32_t i18n_ubrk_preceding(i18n_ubreak_iterator_h break_iter, int32_t offset);

/**
 * @brief Advances the iterator to the first boundary following the specified @a offset.
 * @details The value returned is always greater than @a offset, or #I18N_UBRK_DONE.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 * @param[in] break_iter		The break iterator to use. Must not be @c NULL.
 * @param[in] offset	The offset to begin scanning.
 * @return The text boundary following offset, or #I18N_UBRK_DONE.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubrk_preceding()
 */
int32_t i18n_ubrk_following(i18n_ubreak_iterator_h break_iter, int32_t offset);

/**
 * @brief Gets a locale for which text breaking information is available.
 * @details A #i18n_ubreak_iterator_h in a locale returned by this function will perform the correct
 *		text breaking for the locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 * @param[in] index The index of the desired locale.
 * @return A locale for which number text breaking information is available, or @c 0 if none.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubrk_count_available()
 */
const char *i18n_ubrk_get_available(int32_t index);

/**
 * @brief Determines how many locales have text breaking information available.
 * @details This function is most useful as determining the loop ending condition for
 *		calls to i18n_ubrk_get_available().
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 * @return The number of locales for which text breaking information is available.
 * @exception #I18N_ERROR_NONE Successful
 * @see i18n_ubrk_get_available()
 */
int32_t i18n_ubrk_count_available(void);

/**
 * @brief Returns true if the specified position is a boundary position.
 * @details As a side effect, leaves the iterator pointing to the first boundary position at
 *		or after @c offset.
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are
 *		described in Exceptions section.
 * @since_tizen 2.3.1
 * @param[in] break_iter The break iterator to use. Must not be @c NULL.
 * @param[in] offset The offset to check.
 * @return True if "offset" is a boundary position.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_ubool i18n_ubrk_is_boundary(i18n_ubreak_iterator_h break_iter, int32_t offset);

/**
 * @brief Returns the status from the break rule that determined the most recently
 *		returned break position.
 * @details The values appear in the rule source
 *		within brackets, {123}, for example. For rules that do not specify a
 *		status, a default value of 0 is returned.
 *		<p>
 *		For word break iterators, the possible values are defined in enum #i18n_uchar_u_word_break_values_e.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 * @param[in] break_iter The break iterator to use. Must not be @c NULL.
 * @return The status from the break rule that determined the most recently returned break position.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ubrk_get_rule_status(i18n_ubreak_iterator_h break_iter);

/**
 * @brief Gets the statuses from the break rules that determined the most recently
 *		returned break position.
 * @details The values appear in the rule source within brackets, {123}, for example. The default status value for rules
 *		that do not explicitly provide one is zero.
 *		<p>
 *		For word break iterators, the possible values are defined in enum #i18n_uchar_u_word_break_values_e.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and in #i18n_error_code_e description.
 *
 * @since_tizen 2.3.1
 * @param[in] break_iter	The break iterator to use. Must not be @c NULL.
 * @param[out] fill_in_vec	An array to be filled in with the status values.
 * @param[in] capacity		The length of the supplied vector. A length of zero causes
 *							the function to return the number of status values, in the
 *							normal way, without attempting to store any values.
 * @return The number of rule status values from rules that determined
 *		the most recent boundary returned by the break iterator.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ubrk_get_rule_status_vec(i18n_ubreak_iterator_h break_iter, int32_t *fill_in_vec, int32_t capacity);

/**
 * @brief Returns the locale of the break iterator. You can choose between the valid and
 * the actual locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and in #i18n_error_code_e description.
 *
 * @since_tizen 2.3.1
 * @param[in] break_iter	Break iterator. Must not be @c NULL.
 * @param[in] type			Locale type (valid or actual).
 * @return locale string
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
const char *i18n_ubrk_get_locale_by_type(const i18n_ubreak_iterator_h break_iter, i18n_ulocale_data_locale_type_e type);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
