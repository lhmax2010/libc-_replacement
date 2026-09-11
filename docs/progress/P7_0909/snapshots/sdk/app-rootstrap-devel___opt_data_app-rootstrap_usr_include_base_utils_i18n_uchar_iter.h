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
 * @file utils_i18n_uchar_iter.h
 * @version 0.1
 * @brief utils_i18n_uchar_iterator
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE UChar Iterator
 * @brief The UChar Iterator module provides API for code unit iteration.
 * @section CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE_HEADER Required Header
 *          \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE_OVERVIEW Overview
 * @details C API for code unit iteration.
 *          This can be implemented using simple strings, etc.
 *          The current() and next() functions only check the current index against the limit,
 *          and previous() only checks the current index against the start, to see if the iterator
 *          already reached the end of the iteration range.
 *          The assumption - in all iterators - is that the index is moved via the API, which means
 *          it won't go out of bounds, or the index is modified by user code that knows enough
 *          about the iterator implementation to set valid index values.
 *          UCharIterator functions return code unit values 0..0xffff.
 *          Before any functions operating on strings are called, the string must be set with
 *          i18n_uchar_iter_set_string(), i18n_uchar_iter_set_UTF16BE() or i18n_uchar_iter_set_UTF8().
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE
 * @{
 */

/**
 * @brief Creates an #i18n_uchar_iter_h object.
 * @since_tizen 4.0
 * @remarks To delete this object call i18n_uchar_iter_destroy() function.
 *
 * @param[out]	iter				The #i18n_uchar_iter_h handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE					Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER	Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 *
 * int main() {
 * 	i18n_error_code_e error_code;
 * 	i18n_iter_h iter = NULL;
 * 	error_code = i18n_iter_create(&iter);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_iter_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_iter_destroy(iter);
 * 	return EXIT_SUCCESS;
 * }
 *
 * @endcode
 */
int i18n_uchar_iter_create(i18n_uchar_iter_h *iter);

/**
 * @brief Deletes an #i18n_uchar_iter_h object.
 * @since_tizen 4.0
 *
 * @param[in]	iter				The #i18n_uchar_iter_h object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE					Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER	Invalid function parameter
 */
int i18n_uchar_iter_destroy(i18n_uchar_iter_h iter);

/**
 * @brief Sets up an #i18n_uchar_iter_h to iterate over a string.
 * @details Sets the #i18n_uchar_iter_h function pointers for iteration over the string @a s with
 *          iteration boundaries (start == index == 0) and (length == limit == string length).
 *          The "provider" may set the start, index, and limit values at any time within the range 0..@a length.
 * @since_tizen 4.0
 * @remarks The string @a s will not be copied or reallocated.
 *
 * @param[in]	iter			The #i18n_uchar_iter_h structure to be set for iteration
 * @param[in]	s				String to iterate over
 * @param[in]	length			Length of @a s, or @c -1 if @c NULL-terminated
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE					Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER	Invalid function parameter
 */
int i18n_uchar_iter_set_string(i18n_uchar_iter_h iter, const i18n_uchar *s, int32_t length);

/**
 * @brief Sets up an #i18n_uchar_iter_h to iterate over a UTF-16BE string (byte vector with a
 *        big-endian pair of bytes per #i18n_uchar).
 * @details Everything works just like with a normal #i18n_uchar iterator, except that #i18n_uchar characters
 *          are assembled from byte pairs, and that the @a length argument here indicates an even number
 *          of bytes.
 * @since_tizen 4.0
 *
 * @param[in]	iter			#i18n_uchar_iter_h structure to be set for iteration
 * @param[in]	s				UTF-16BE string to iterate over
 * @param[in]	length			Length of @a s as an even number of bytes, or @c -1 if
 *								@c NULL-terminated (@c NULL means pair of 0 bytes at even
 *								index from @a s)
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE					Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER	Invalid function parameter
 */
int i18n_uchar_iter_set_utf16be(i18n_uchar_iter_h iter, const char *s, int32_t length);

/**
 * @brief Sets up an #i18n_uchar_iter_h to iterate over a UTF-8 string.
 * @details Sets the #i18n_uchar_iter_h function pointers for iteration over the UTF-8 string @a s
 *          with UTF-8 iteration boundaries 0 and @a length. The implementation counts the UTF-16
 *          index on the fly and lazily evaluates the UTF-16 length of the text.
 * @since_tizen 4.0
 * @remarks The string @a s will not be copied or reallocated.
 *          i18n_uchar_iter_get_state() returns a state value consisting of the current UTF-8 source
 *          byte index (bits 31..1) a flag (bit 0) that indicates whether the UChar position is in the
 *          middle of a surrogate pair (from a 4-byte UTF-8 sequence for the corresponding supplementary code point).
 *          i18n_uchar_iter_get_state() cannot also encode the UTF-16 index in the state value.
 *
 * @param[in]	iter				#i18n_uchar_iter_h structure to be set for iteration
 * @param[in]	s					UTF-8 string to iterate over
 * @param[in]	length				Length of @a s, or @c -1 if @c NULL-terminated
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE					Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER	Invalid function parameter
 * @pre		The string must be set with one of: i18n_uchar_iter_set_string(), i18n_uchar_iter_set_UTF16BE(),
 *			i18n_uchar_iter_set_UTF8().
 */
int i18n_uchar_iter_set_utf8(i18n_uchar_iter_h iter, const char *s, int32_t length);

/**
 * @brief Gets the current position, or the start or limit of the iteration range.
 * @details This function may perform slowly for #I18N_UCHAR_ITER_CURRENT after i18n_uchar_iter_set_state()
 *          was called, or for #I18N_UCHAR_ITER_LENGTH, because an iterator implementation may have to count
 *          UChars if the underlying storage is not UTF-16.
 * @since_tizen 4.0
 *
 * @param[in]	iter				The #i18n_uchar_iter_h object
 * @param[in]	origin				Origin defining action to perform
 * @param[out]	index				The requested index, or #I18N_SENTINEL in an error condition
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE					Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER	Invalid function parameter
 * @pre		The string must be set with one of: i18n_uchar_iter_set_string(), i18n_uchar_iter_set_UTF16BE(),
 *			i18n_uchar_iter_set_UTF8().
 */
int i18n_uchar_iter_get_index(i18n_uchar_iter_h iter, i18n_uchar_iter_origin_e origin, int32_t *index);

/**
 * @brief Moves the current position relative to the start or limit of the iteration range, or relative to the
 *        current position itself. The movement is expressed in numbers of code units forward or backward by specifying
 *        a positive or negative delta. Out of bounds movement will be pinned to the start or limit.
 * @details This function may perform slowly for moving relative to #I18N_UCHAR_ITER_LENGTH because an iterator
 *          implementation may have to count the rest of the UChars if the native storage is not UTF-16.
 *          When moving relative to the limit or length, or relative to the current position after i18n_uchar_iter_set_state()
 *          was called, i18n_uchar_iter_move() may return #I18N_UCHAR_ITER_UNKNOWN_INDEX to avoid an inefficient
 *          determination of the actual UTF-16 index. The actual index can be determined with
 *          i18n_uchar_iter_get_index(#I18N_UCHAR_ITER_CURRENT) which will count the UChars if necessary.
 * @since_tizen 4.0
 *
 * @param[in]	iter				The #i18n_uchar_iter_h object
 * @param[in]	delta				Movement
 * @param[in]	origin				Origin defining action to perform
 * @param[out]	new_index			The new index or #I18N_UCHAR_ITER_UNKNOWN_INDEX when the index is not known,
 *									or #I18N_SENTINEL on an error condition
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE					Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER	Invalid function parameter
 * @pre		The string must be set with one of: i18n_uchar_iter_set_string(), i18n_uchar_iter_set_UTF16BE(),
 *			i18n_uchar_iter_set_UTF8().
 */
int i18n_uchar_iter_move(i18n_uchar_iter_h iter, int32_t delta, i18n_uchar_iter_origin_e origin, int32_t *new_index);

/**
 * @brief Checks if i18n_uchar_iter_current() and i18n_uchar_iter_next() can still return another code unit.
 * @since_tizen 4.0
 *
 * @param[in]	iter			The #i18n_uchar_iter_h object
 * @param[out]	has_next		@c true if another code unit can be returned, @c false otherwise
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE					Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER	Invalid function parameter
 * @pre		The string must be set with one of: i18n_uchar_iter_set_string(), i18n_uchar_iter_set_UTF16BE(),
 *			i18n_uchar_iter_set_UTF8().
 */
int i18n_uchar_iter_has_next(i18n_uchar_iter_h iter, bool *has_next);

/**
 * @brief Checks if i18n_uchar_iter_previous() can still return another code unit.
 * @since_tizen 4.0
 *
 * @param[in]	iter			The #i18n_uchar_iter_h object
 * @param[out]	has_previous	@c true if another code unit can be returned, @c false otherwise
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE					Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER	Invalid function parameter
 * @pre		The string must be set with one of: i18n_uchar_iter_set_string(), i18n_uchar_iter_set_UTF16BE(),
 *			i18n_uchar_iter_set_UTF8().
 */
int i18n_uchar_iter_has_previous(i18n_uchar_iter_h iter, bool *has_previous);

/**
 * @brief Returns the code unit at the current position, or #I18N_SENTINEL if there is none
 *			(index is at the limit).
 * @since_tizen 4.0
 *
 * @param[in]	iter			The #i18n_uchar_iter_h object
 * @param[out]	current			The current code unit
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE					Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER	Invalid function parameter
 * @pre		The string must be set with one of: i18n_uchar_iter_set_string(), i18n_uchar_iter_set_UTF16BE(),
 *			i18n_uchar_iter_set_UTF8().
 */
int i18n_uchar_iter_current(i18n_uchar_iter_h iter, i18n_uchar32 *current);

/**
 * @brief Returns the code unit at the current index and increments the index (post-increment, like s[i++]),
 *			or returns #I18N_SENTINEL if there is none (index is at the limit).
 * @since_tizen 4.0
 *
 * @param[in]	iter			The #i18n_uchar_iter_h object
 * @param[out]	current			The current code unit
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE					Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER	Invalid function parameter
 * @pre		The string must be set with one of: i18n_uchar_iter_set_string(), i18n_uchar_iter_set_UTF16BE(),
 *			i18n_uchar_iter_set_UTF8().
 */
int i18n_uchar_iter_next(i18n_uchar_iter_h iter, i18n_uchar32 *current);

/**
 * @brief Decrements the index and returns the code unit from there (pre-decrement, like s[--i]),
 *			or returns #I18N_SENTINEL if there is none (index is at the start).
 * @since_tizen 4.0
 *
 * @param[in]	iter			The #i18n_uchar_iter_h object
 * @param[out]	previous		The previous code unit
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE					Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER	Invalid function parameter
 * @pre		The string must be set with one of: i18n_uchar_iter_set_string(), i18n_uchar_iter_set_UTF16BE(),
 *			i18n_uchar_iter_set_UTF8().
 */
int i18n_uchar_iter_previous(i18n_uchar_iter_h iter, i18n_uchar32 *previous);

/**
 * @brief Gets the "state" of the iterator in the form of a single 32-bit word.
 * @details It is recommended that the state value be calculated to be as small as is feasible. For strings
 *          with limited lengths, fewer than 32 bits may be sufficient.
 *
 *          This is used together with i18n_uchar_iter_set_state() to save and restore the iterator position
 *          more efficiently than with i18n_uchar_iter_get_index() or i18n_uchar_iter_move().
 *
 *          The iterator state is defined as a @c uint32_t value because it is designed for use in
 *          i18n_ucol_next_sort_key_part() which provides 32 bits to store the state of the character iterator.
 *
 *          With some UCharIterator implementations (e.g., UTF-8), getting and setting the UTF-16 index with existing
 *          functions (i18n_uchar_iter_get_index(#I18N_UCHAR_ITER_CURRENT) followed by
 *          i18n_uchar_iter_move(pos, #I18N_UCHAR_ITER_ZERO)) is possible but relatively slow because the iterator
 *          has to "walk" from a known index to the requested one. This takes more time the farther it needs to go.
 *
 *          An opaque state value allows an iterator implementation to provide an internal index (UTF-8: the source
 *          byte array index) for fast, constant-time restoration.
 *
 *          After calling i18n_uchar_iter_set_state(), i18n_uchar_iter_get_index(#I18N_UCHAR_ITER_CURRENT) calls may be
 *          slow because the UTF-16 index may not be restored as well, but the iterator can deliver the correct text
 *          contents and move relative to the current position without performance degradation.
 *
 *          Some UCharIterator implementations may not be able to return a valid state for each position, in which case
 *          they return #I18N_UCHAR_ITER_NO_STATE instead.
 * @since_tizen 4.0
 *
 * @param[in]	iter			The #i18n_uchar_iter_h object
 * @param[out]	state			The state word
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE					Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER	Invalid function parameter
 * @pre		The string must be set with one of: i18n_uchar_iter_set_string(), i18n_uchar_iter_set_UTF16BE(),
 *			i18n_uchar_iter_set_UTF8().
 */
int i18n_uchar_iter_get_state(const i18n_uchar_iter_h iter, uint32_t *state);

/**
 * @brief Restores the "state" of the iterator using a state word from a i18n_uchar_iter_get_state() call.
 *        The iterator object need not be the same one as for which i18n_uchar_iter_get_state() was called,
 *        but it must be of the same type (set up using the same i18n_uchar_iter_set_* function) and it must iterate
 *        over the same string (binary identical regardless of memory address).
 * @details After calling i18n_uchar_iter_set_state(), an i18n_uchar_iter_get_index(#I18N_UCHAR_ITER_CURRENT)
 *          may be slow because the UTF-16 index may not be restored as well, but the iterator can deliver
 *          the correct text contents and move relative to the current position without performance degradation.
 * @since_tizen 4.0
 *
 * @param[in]	iter				The #i18n_uchar_iter_h object
 * @param[in]	state				The state word from an i18n_uchar_iter_get_state() call on a same-type,
 *									same-string iterator
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE					Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER	Invalid function parameter
 * @pre		The string must be set with one of: i18n_uchar_iter_set_string(), i18n_uchar_iter_set_UTF16BE(),
 *			i18n_uchar_iter_set_UTF8().
 */
int i18n_uchar_iter_set_state(i18n_uchar_iter_h iter, uint32_t state);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
