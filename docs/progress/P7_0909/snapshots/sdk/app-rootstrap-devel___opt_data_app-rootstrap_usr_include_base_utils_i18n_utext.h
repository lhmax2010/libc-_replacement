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
 * @file utils_i18n_utext.h
 * @version 0.1
 * @brief utils_i18n_utext
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UTEXT_MODULE Utext
 * @brief Abstract Unicode Text API.
 * @section CAPI_BASE_UTILS_I18N_UTEXT_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UTEXT_MODULE_OVERVIEW Overview
 * @details The Text Access API provides a means to allow text that is stored in alternative formats to work with ICU services.
 * ICU normally operates on text that is stored in UTF-16 format, in (#i18n_uchar *) arrays for the C APIs or as type UnicodeString for C++ APIs.
 * ICU Text Access allows other formats, such as UTF-8 or non-contiguous UTF-16 strings, to be placed in an #i18n_utext_h wrapper and then passed to ICU services.
 *
 * These are typical classes of usage for #i18n_utext_h:
 *
 * Application Level Use - applications would use one of the @ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE functions
 * on their input text, and pass the resulting #i18n_utext_h to the desired ICU service.
 *
 * ICU Services use - services (eg. break iteration) would operate on input
 * presented to them as an #i18n_utext_h. These implementations will need to use the iteration
 * and related #i18n_utext_h functions to gain access to the actual text.
 *
 * Text providers use - text providers are the #i18n_utext_h implementations
 * for the various text storage formats.
 * The Text Access API offers 2 built in providers - for UTF8 and UTF16 encoded strings.
 * Implementing custom #i18n_utext_h providers is not supported.
 *
 * <em>Iterating over text</em>
 *
 * Here is sample code for a forward iteration over the contents of an #i18n_utext_h
 *
 * @code
 *    i18n_uchar32  c;
 *    i18n_utext    *ut = i18n_utext_create_for_UTF8(SOME_UTF8_ENCODED_STRING);
 *
 *    for (c=i18n_utext_next32_from(ut, 0); c>=0; c=i18n_utext_next32(ut)) {
 *       // do whatever with the codepoint c here.
 *    }
 * @endcode
 *
 * And here is similar code to iterate in the reverse direction, from the end
 * of the text towards the beginning.
 *
 * @code
 *    i18n_uchar32  c;
 *    i18n_utext    *ut = i18n_utext_create_for_UTF8(SOME_UTF8_ENCODED_STRING);
 *    int      textLength = i18n_utext_native_length(ut);
 *    for (c=i18n_utext_previous32_from(ut, textLength); c>=0; c=i18n_utext_previous32(ut)) {
 *       // do whatever with the codepoint c here.
 *    }
 * @endcode
 *
 * <em>Characters and Indexing</em>
 *
 * Indexing into text by #i18n_utext_h functions is nearly always in terms of the native
 * indexing of the underlying text storage.  The storage format could be UTF-8
 * or UTF-32, for example.  When coding to the #i18n_utext_h access API, no assumptions
 * can be made regarding the size of characters, or how far an index
 * may move when iterating between characters.
 *
 * All indices supplied to #i18n_utext_h functions are pinned to the length of the
 * text.  An out-of-bounds index is not considered to be an error, but is
 * adjusted to be in the range  0 <= index <= length of input text.
 *
 *
 * When an index position is returned from an #i18n_utext_h function, it will be
 * a native index to the underlying text.  In the case of multi-unit characters,
 * it will  always refer to the first position of the character,
 * never to the interior.  This is essentially the same thing as saying that
 * a returned index will always point to a boundary between characters.
 *
 * When a native index is supplied to an #i18n_utext_h function, all indices that
 * refer to any part of a multi-unit character representation are considered
 * to be equivalent.  In the case of multi-unit characters, an incoming index
 * will be logically normalized to refer to the start of the character.
 *
 * It is possible to test whether a native index is on a code point boundary
 * by doing an #i18n_utext_set_native_index() followed by an #i18n_utext_get_native_index().
 * If the index is returned unchanged, it was on a code point boundary.  If
 * an adjusted index is returned, the original index referred to the
 * interior of a character.
 *
 * <em>Conventions for calling #i18n_utext_h functions</em>
 *
 * Most #i18n_utext_h access functions have as their first parameter a (#i18n_utext_h) pointer,
 * which specifies the #i18n_utext_h to be used.  Unless otherwise noted, the
 * pointer must refer to a valid, created #i18n_utext_h.  Attempting to
 * use a destroyed #i18n_utext_h or passing a NULL pointer is a programming error and
 * will produce undefined results or crashes.
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UTEXT_MODULE
 * @{
 */

/**
 * @brief Destroys the #i18n_utext_h handle and release its resources.
 * @details An #i18n_utext_h can be reset to refer to new string by using one of the i18n_utext_create_for_UTF8() or i18n_utext_create_for_uchars() functions without first closing the #i18n_utext_h.
 * @since_tizen 6.0
 * @param[in] ut The #i18n_utext_h handle to be released.
 */
void i18n_utext_destroy(i18n_utext_h ut);

/**
 * @brief Creates a read-only #i18n_utext_h implementation for UTF-8 strings.
 * @details Any invalid UTF-8 in the input will be handled in this way:
 * 			a sequence of bytes that has the form of a truncated, but otherwise valid,
 * 			UTF-8 sequence will be replaced by a single unicode replacement character, @htmlonly \uFFFD @endhtmlonly.
 * 			Any other illegal bytes will each be replaced by a @htmlonly \uFFFD @endhtmlonly.
 * @since_tizen 6.0
 * @remarks If @a utb is created (@a uta was NULL), then the created @a utb needs to be released using #i18n_utext_destroy().
 * @param[in] uta	An #i18n_utext_h handle. If NULL, a new #i18n_utext_h will be created. If non-NULL,
 *	 				must refer to an initialized #i18n_utext_h handle, which will then be reset to reference the specified UTF-8 string.
 * @param[in] s		A UTF-8 string. Must not be NULL.
 * @param[in] length	The length of the UTF-8 string in bytes, or -1 if the string is zero terminated.
 * @param[out] utb	A pointer to an #i18n_utext_h handle. If a pre-allocated #i18n_utext_h was provided, it will always be used and returned.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_utext_create_for_UTF8(i18n_utext_h uta, const char *s, int64_t length, i18n_utext_h *utb);

/**
 * @brief Creates a read-only #i18n_utext_h for #i18n_uchar * string.
 * @since_tizen 6.0
 * @remarks If @a utb is created (@a uta was NULL), then the created @a utb needs to be released using #i18n_utext_destroy().
 * @param[in] uta	An #i18n_utext_h handle. If NULL, a new #i18n_utext_h will be created. If non-NULL,
 *					must refer to an initialized #i18n_utext_h handle, which will then be reset to reference the specified #i18n_uchar string.
 * @param[in] s		An #i18n_uchar (UTF-16) string.
 * @param[in] length	The length of the i18n_uchars in the input string, or -1 if the string is zero terminated.
 * @param[out] utb	A pointer to an #i18n_utext_h handle. If a pre-allocated #i18n_utext_h was provided, it will always be used and returned.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_utext_h utb;
 * 	i18n_error_code_e error_code;
 * 	i18n_uchar uString[] = {0x41, 0x42, 0x43, 0};
 * 	error_code = i18n_utext_create_for_uchars(NULL, uString, -1, &utb);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_uset_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_utext_destroy(utb);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_utext_create_for_uchars(i18n_utext_h uta, const i18n_uchar *s, int64_t length, i18n_utext_h *utb);

/**
 * @brief Clones an #i18n_utext_h.
 * @details	This is much like creating an #i18n_utext_h where the source text is itself another #i18n_utext_h.
 *			A deep clone will copy both the #i18n_utext_h handle and the underlying text.
 *			The original and cloned #i18n_utext_h will operate completely independently;
 *			modifications made to the text in one will not affect the other.
 *			Text providers are not required to support deep clones.
 *			The user of clone() must check the return value and be prepared to handle failures.
 *
 *			The built-in #i18n_utext_h implementations for UTF8 and i18n_uchar * support deep cloning.
 *			The #i18n_utext_h returned from a deep clone will be writable, assuming that the text provider is able to support writing,
 *			even if the source #i18n_utext_h had been made non-writable by means of i18n_utext_freeze().
 *
 *			A shallow clone replicates only the #i18n_utext_h handle data; it does not make a copy of the underlying text.
 *			Shallow clones can be used as an efficient way to have multiple iterators active in a single text string that is not being modified.
 *			A shallow clone operation will not fail, barring truly exceptional conditions such as memory allocation failures.
 *			Shallow #i18n_utext_h clones should be avoided if the #i18n_utext_h functions that modify the text are expected to be used,
 *			either on the original or the cloned #i18n_utext_h. Any such modifications can cause unpredictable behavior.
 *
 *			Read Only shallow clones provide some protection against errors of this type by disabling text modification via the cloned #i18n_utext_h.
 *			A shallow clone made with the read_only parameter == FALSE will preserve the i18n_utext_is_writable() state of the source object.
 *			Note, however, that write operations must be avoided while more than one #i18n_utext_h exists that refer to the same underlying text.
 *
 *			An #i18n_utext_h and its clone may be safely concurrently accessed by separate threads.
 *			This is true for read access only with shallow clones, and for both read and write access with deep clones.
 * @since_tizen 6.0
 * @param[in] dest	An #i18n_utext_h handle to be filled in with the result of the clone operation,
 *					or NULL if the clone function should heap-allocate a new #i18n_utext_h handle.
 *					If non-NULL, must refer to an already existing #i18n_utext_h, which will then be reset to become the clone.
 * @param[in] src	The #i18n_utext_h to be cloned.
 * @param[in] deep	TRUE to request a deep clone, FALSE for a shallow clone.
 * @param[in] read_only	TRUE to request that the cloned #i18n_utext_h have read only access to the underlying text.
 * @param[out] ut	The newly created clone, or NULL if the clone operation failed.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int i18n_utext_clone(i18n_utext_h dest, const i18n_utext_h src, i18n_ubool deep, i18n_ubool read_only, i18n_utext_h *ut);

/**
 * @brief Compares two #i18n_utext_h objects for equality.
 * @details	i18n_utexts are equal if they are iterating over the same text, and have the same iteration position within the text.
 *			If either or both of the parameters are NULL, the comparison is FALSE.
 * @since_tizen 6.0
 * @param[in] a	The first of the two #i18n_utext_h to compare.
 * @param[in] b	The other #i18n_utext_h to be compared.
 * @return TRUE if the two i18n_utexts are equal.
 *
 */
i18n_ubool i18n_utext_equals(const i18n_utext_h a, const i18n_utext_h b);

/**
 * @brief	Gets the length of the text.
 * @details	Depending on the characteristics of the underlying text representation, this may be expensive.
 * @since_tizen 6.0
 * @param[in] ut	The text to be accessed.
 * @return the length of the text, expressed in native units.
 *
 * @see i18n_utext_is_length_expensive()
 *
 */
int64_t i18n_utext_native_length(i18n_utext_h ut);

/**
 * @brief	Returns TRUE if calculating the length of the text could be expensive.
 * @details	Finding the length of NUL terminated strings is considered to be expensive.
 *			Note that the value of this function may change as the result of other operations on an #i18n_utext_h.
 *			Once the length of a string has been discovered, it will no longer be expensive to report it.
 * @since_tizen 6.0
 * @param[in] ut	The text to be accessed.
 * @return TRUE if determining the length of the text could be time consuming.
 *
 */
i18n_ubool i18n_utext_is_length_expensive(const i18n_utext_h ut);

/**
 * @brief	Returns the code point at the requested index, or #I18N_SENTINEL (-1) if it is out of bounds.
 * @details	If the specified index points to the interior of a multi-unit character
 *			- one of the trail bytes of a UTF-8 sequence, for example - the complete code point will be returned.
 *
 *			The iteration position will be set to the start of the returned code point.
 *
 *			This function is roughly equivalent to the sequence i18n_utext_set_native_index(index);
 *			i18n_utext_current32(); (There is a subtle difference if the index is out of bounds by being less than zero
 *			- i18n_utext_set_native_index(negative value) sets the index to zero,
 *			after which i18n_utext_current() will return the char at zero.
 *			i18n_utext_char32_at(negative index), on the other hand, will return the #I18N_SENTINEL value of -1.)
 * @since_tizen 6.0
 * @param[in] ut			The text to be accessed.
 * @param[in] native_index	The native index of the character to be accessed.
 *							If the index points to other than the first unit of a multi-unit character,
 *							it will be adjusted to the start of the character.
 * @return the code point at the specified index.
 *
 */
i18n_uchar32 i18n_utext_char32_at(i18n_utext_h ut, int64_t native_index);

/**
 * @brief	Gets the code point at the current iteration position, or #I18N_SENTINEL (-1) if the iteration has reached the end of the input text.
 * @since_tizen 6.0
 * @param[in] ut	The text to be accessed.
 * @return the Unicode code point at the current iterator position.
 *
 */
i18n_uchar32 i18n_utext_current32(i18n_utext_h ut);

/**
 * @brief	Gets the code point at the current iteration position of the #i18n_utext_h, and advance the position to the first index following the character.
 * @details	If the position is at the end of the text (the index following the last character, which is also the length of the text), return #I18N_SENTINEL (-1) and do not advance the index.
 *			This is a post-increment operation.
 * @since_tizen 6.0
 * @param[in] ut	The text to be accessed.
 * @return the Unicode code point at the iteration position.
 *
 */
i18n_uchar32 i18n_utext_next32(i18n_utext_h ut);

/**
 * @brief	Moves the iterator position to the character (code point) whose index precedes the current position, and return that character.
 * @details	This is a pre-decrement operation. If the initial position is at the start of the text (index of 0) return #I18N_SENTINEL (-1), and leave the position unchanged.
 * @since_tizen 6.0
 * @param[in] ut	The text to be accessed.
 * @return the previous i18n_uchar32 code point, or #I18N_SENTINEL (-1) if the iteration has reached the start of the text.
 *
 */
i18n_uchar32 i18n_utext_previous32(i18n_utext_h ut);

/**
 * @brief	Sets the iteration index and return the code point at that index.
 * @details	Leave the iteration index at the start of the following code point.
 *			This function is the most efficient and convenient way to begin a forward iteration. The results are identical to the those from the sequence
 *			@code
 *			i18n_utext_set_native_index();
 *			i18n_utext_next32();
 *			@endcode
 * @since_tizen 6.0
 * @param[in] ut			The text to be accessed.
 * @param[in] native_index	Iteration index, in the native units of the text provider.
 * @return Code point which starts at or before index, or #I18N_SENTINEL (-1) if it is out of bounds.
 *
 */
i18n_uchar32 i18n_utext_next32_from(i18n_utext_h ut, int64_t native_index);

/**
 * @brief	Sets the iteration index, and return the code point preceding the one specified by the initial index.
 * @details	Leave the iteration position at the start of the returned code point
 *			This function is the most efficient and convenient way to begin a backwards iteration.
 * @since_tizen 6.0
 * @param[in] ut			The text to be accessed.
 * @param[in] native_index	Iteration index, in the native units of the text provider.
 * @return Code point which starts at or before index, or #I18N_SENTINEL (-1) if it is out of bounds.
 *
 */
i18n_uchar32 i18n_utext_previous32_from(i18n_utext_h ut, int64_t native_index);

/**
 * @brief	Gets the current iterator position, which can range from 0 to the length of the text.
 * @details	The position is a native index into the input text, in whatever format it may have (possibly UTF-8 for example),
 *			and may not always be the same as the corresponding #i18n_uchar (UTF-16) index.
 *			The returned position will always be aligned to a code point boundary.
 * @since_tizen 6.0
 * @param[in] ut			The text to be accessed.
 * @return the current index position, in the native units of the text provider.
 *
 */
int64_t i18n_utext_get_native_index(const i18n_utext_h ut);

/**
 * @brief	Sets the current iteration position to the nearest code point boundary at or preceding the specified index.
 * @details	The index is in the native units of the original input text. If the index is out of range,
 * 			it will be pinned to be within the range of the input text.
 * 			It will usually be more efficient to begin an iteration using the functions
 * 			#i18n_utext_next32_from() or #i18n_utext_previous32_from() rather than #i18n_utext_set_native_index().
 *
 * 			Moving the index position to an adjacent character is best done with
 * 			i18n_utext_next32(), i18n_utext_previous32() or i18n_utext_move_index32().
 * 			Attempting to do direct arithmetic on the index position is complicated by the fact that
 * 			the size (in native units) of a character depends on the underlying representation of the character (UTF-8, UTF-16, UTF-32, arbitrary codepage),
 * 			and is not easily knowable.
 * @since_tizen 6.0
 * @param[in] ut			The text to be accessed.
 * @param[in] native_index	The native unit index of the new iteration position.
 *
 */
void i18n_utext_set_native_index(i18n_utext_h ut, int64_t native_index);

/**
 * @brief	Moves the iterator position by delta code points.
 * @details	The number of code points is a signed number; a negative delta will move the iterator backwards, towards the start of the text.
 * 			The index is moved by delta code points forward or backward, but no further backward than to 0 and
 * 			no further forward than to i18n_utext_native_length().
 * 			The resulting index value will be in between 0 and length, inclusive.
 * @since_tizen 6.0
 * @param[in] ut			The text to be accessed.
 * @param[in] delta			The signed number of code points to move the iteration position.
 * @return TRUE if the position could be moved the requested number of positions while staying within the range [0 - text length].
 *
 */
i18n_ubool i18n_utext_move_index32(i18n_utext_h ut, int32_t delta);

/**
 * @brief	Gets the native index of the character preceding the current position.
 * @details	If the iteration position is already at the start of the text, zero is returned.
 *			The value returned is the same as that obtained from the following sequence,
 *			but without the side effect of changing the iteration position.
 *			@code
 *			i18n_utext_h ut = whatever;
 *			i18n_utext_previous(ut);
 *			i18n_utext_get_native_index(ut);
 *			@endcode
 * 			This function is most useful during forwards iteration, where it will get the native index of
 *			the character most recently returned from i18n_utext_next().
 * @since_tizen 6.0
 * @param[in] ut			The text to be accessed.
 * @return the native index of the character preceding the current index position, or zero if the current position is at the start of the text.
 *
 */
int64_t i18n_utext_get_previous_native_index(i18n_utext_h ut);

/**
 * @brief	Extracts text from an #i18n_utext_h into an #i18n_uchar buffer.
 * @details	The range of text to be extracted is specified in the native indices of the #i18n_utext_h provider.
 *			These may not necessarily be UTF-16 indices.
 *			The size (number of 16 bit i18n_uchars) of the data to be extracted is returned.
 *			The full number of i18n_uchars is returned, even when the extracted text is truncated because the specified buffer size is too small.
 *
 *			The extracted string will be NUL-terminated if there is sufficient space in the destination buffer.
 *			This terminating NUL is not included in the returned length.
 *			The iteration index is left at the position following the last extracted character.
 * @since_tizen 6.0
 * @param[in] ut			The #i18n_utext_h from which to extract data.
 * @param[in] native_start	The native index of the first character to extract.
 *							If the specified index is out of range, it will be pinned to be within 0 <= index <= textLength
 * @param[in] native_limit	The native string index of the position following the last character to extract.
 *							If the specified index is out of range, it will be pinned to be within 0 <= index <= textLength.
 * 							native_limit must be >= native_start.
 * @param[in] dest			The i18n_uchars (UTF-16) buffer into which the extracted text is placed.
 * @param[in] dest_capacity	The size, in i18n_uchars, of the destination buffer. May be zero for precomputing the required size.
 * @param[out] length		Number of i18n_uchars in the data to be extracted. Does not include a trailing NUL.
 * @return receives any error status.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #I18N_ERROR_BUFFER_OVERFLOW the extracted text was truncated because the buffer was too small.
 *
 */
int i18n_utext_extract(i18n_utext_h ut, int64_t native_start, int64_t native_limit, i18n_uchar *dest, int32_t dest_capacity, int32_t *length);

/**
 * @brief	Returns TRUE if the text can be written (modified) with i18n_utext_replace() or i18n_utext_copy().
 * @details	For the text to be writable, the text provider must be of a type that supports writing and the #i18n_utext_h must not be frozen.
 *			Attempting to modify text when i18n_utext_is_writeable() is FALSE will fail
 * 			- the text will not be modified, and an error will be returned from the function that attempted the modification.
 * @since_tizen 6.0
 * @param[in] ut			The #i18n_utext_h to be tested.
 * @return TRUE if the text is modifiable.
 * @see i18n_utext_freeze()
 * @see i18n_utext_replace()
 * @see i18n_utext_copy()
 *
 */
i18n_ubool i18n_utext_is_writable(const i18n_utext_h ut);

/**
 * @brief	Tests whether there is meta data associated with the text.
 * @since_tizen 6.0
 * @param[in] ut			The #i18n_utext_h to be tested.
 * @return TRUE if the underlying text includes meta data.
 *
 */
i18n_ubool i18n_utext_has_meta_data(const i18n_utext_h ut);

/**
 * @brief	Replaces a range of the original text with a replacement text.
 * @details	Leaves the current iteration position at the position following the newly inserted replacement text.
 *			This function is only available on #i18n_utext_h types that support writing, that is, ones where #i18n_utext_is_writable() returns TRUE.
 *
 *			When using this function, there should be only a single #i18n_utext_h created for the underlying native text string.
 *			Behavior after a replace operation on an #i18n_utext_h is undefined for any other additional i18n_utexts that refer to the modified string.
 * @since_tizen 6.0
 * @param[in] ut			The #i18n_utext_h representing the text to be operated on.
 * @param[in] native_start	The native index of the start of the region to be replaced.
 * @param[in] native_limit	The native index of the character following the region to be replaced.
 * @param[in] replacement_text	Pointer to the replacement text.
 * @param[in] replacement_length	Length of the replacement text, or -1 if the text is NUL terminated.
 * @param[out] number		The signed number of (native) storage units by which the length of the text expanded or contracted.
 * @return receives any error status.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #I18N_ERROR_NO_WRITE_PERMISSION	Attempt to modify read-only or constant data
 *
 */
int i18n_utext_replace(i18n_utext_h ut, int64_t native_start, int64_t native_limit, const i18n_uchar *replacement_text, int32_t replacement_length, int32_t *number);

/**
 * @brief	Copies or moves a substring from one position to another within the text, while retaining any metadata associated with the text.
 * @details	This function is used to duplicate or reorder substrings. The destination index must not overlap the source range.
 *			The text to be copied or moved is inserted at dest_index; it does not replace or overwrite any existing text.
 *			The iteration position is left following the newly inserted text at the destination position.
 *
 *			This function is only available on #i18n_utext_h types that support writing, that is, ones where #i18n_utext_is_writable() returns TRUE.
 *			When using this function, there should be only a single #i18n_utext_h created for the underlying native text string.
 *			Behavior after a copy operation on an #i18n_utext_h is undefined in any other additional i18n_utexts that refer to the modified string.
 * @since_tizen 6.0
 * @param[in] ut			The #i18n_utext_h representing the text to be operated on.
 * @param[in] native_start	The native index of the start of the region to be copied or moved.
 * @param[in] native_limit	The native index of the character position following the region to be copied.
 * @param[in] dest_index	The native destination index to which the source substring is copied or moved.
 * @param[in] move			If TRUE, then the substring is moved, not copied/duplicated.
 * @return receives any error status.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #I18N_ERROR_NO_WRITE_PERMISSION	Attempt to modify read-only or constant data
 *
 */
int i18n_utext_copy(i18n_utext_h ut, int64_t native_start, int64_t native_limit, int64_t dest_index, i18n_ubool move);

/**
 * @brief	Freezes an #i18n_utext_h. This prevents any modification to the underlying text itself by means of functions operating on this #i18n_utext_h.
 * @details	Once frozen, an #i18n_utext_h can not be unfrozen. The intent is to ensure that
 *			a the text underlying a frozen #i18n_utext_h wrapper cannot be modified via that #i18n_utext_h.
 *
 *			Caution: freezing an #i18n_utext_h will disable changes made via the specific frozen #i18n_utext_h wrapper only;
 *			it will not have any effect on the ability to directly modify the text by bypassing the #i18n_utext_h.
 *			Any such backdoor modifications are always an error while #i18n_utext_h access is occurring
 *			because the underlying text can get out of sync with #i18n_utext_h's buffering.
 * @since_tizen 6.0
 * @param[in] ut	The #i18n_utext_h to be frozen.
 * @see	i18n_utext_is_writable()
 *
 */
void i18n_utext_freeze(i18n_utext_h ut);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
