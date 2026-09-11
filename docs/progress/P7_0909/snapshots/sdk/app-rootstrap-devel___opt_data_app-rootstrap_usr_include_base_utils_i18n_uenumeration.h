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
 * @file utils_i18n_uenumeration.h
 * @version 0.1
 * @brief utils_i18n_uenumeration
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UENUMERATION_MODULE UEnumeration
 * @brief UEnumeration defines functions for handling String Enumeration.
 *
 * @section CAPI_BASE_UTILS_I18N_UENUMERATION_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UENUMERATION_MODULE_OVERVIEW Overview
 * @details The UEnumeration module allows to create String Enumeration
 *			from chars and uchars.
 *
*/

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UENUMERATION_MODULE
 * @{
 */

/**
 * @brief Disposes of resources in use by the iterator.
 * @details If @c enumeration is <code>NULL</code>, does nothing. After this call, any <code>char*</code> or <code>i18n_uchar*</code> returned
 *			by i18n_uenumeration_unext() or i18n_uenumeration_next() is invalid.
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] enumeration	 An #i18n_uenumeration_h to destroy
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uenumeration_destroy(i18n_uenumeration_h enumeration);

/**
 * @brief Returns the number of elements that the iterator traverses.
 * @details If the iterator is out-of-sync with its service, error code is set to #I18N_ERROR_ENUM_OUT_OF_SYNC.
 *			This is a convenience function. It can end up being very expensive as all the items might have to be pre-fetched
 *			(depending on the type of data being traversed).
 *			Use with caution and only when necessary.
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exceptions section.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] enumeration	   An #i18n_uenumeration_h to count
 *
 * @return The number of elements in the iterator
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @exception #I18N_ERROR_ENUM_OUT_OF_SYNC The iterator is out of sync
 */
int32_t i18n_uenumeration_count(i18n_uenumeration_h enumeration);

/**
 * @brief Returns the next element in the iterator's list.
 * @details If there are no more elements, returns <code>NULL</code>.
 *			If the iterator is out-of-sync with its service, error code is set to #I18N_ERROR_ENUM_OUT_OF_SYNC and <code>NULL</code> is returned.
 *			If the native service string is a <code>char*</code> string, it is converted to <code>i18n_uchar*</code> with the invariant converter.
 *			The result is terminated by <code>(i18n_uchar)0</code>.
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exceptions section.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] enumeration		An #i18n_uenumeration_h
 * @param[out] result_length	A pointer to receive the length of the result (not including the terminating \\0).
 *								If the pointer is <code>NULL</code> it is ignored.
 *
 * @return A pointer to the string. The string will be zero-terminated.
 *		   The return pointer is owned by this iterator and must not be deleted by the caller.
 *		   The pointer is valid until the next call to any i18n_uenumeration_... method, including i18n_uenumeration_next() or i18n_uenumeration_unext().
 *		   When all strings have been traversed, returns <code>NULL</code>.
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @exception #I18N_ERROR_ENUM_OUT_OF_SYNC The iterator is out of sync
 */
const i18n_uchar *i18n_uenumeration_unext(i18n_uenumeration_h enumeration, int32_t *result_length);

/**
 * @brief Returns the next element in the iterator's list.
 * @details If there are no more elements, returns NULL. If the iterator is out-of-sync with its service,
 *			the #I18N_ERROR_ENUM_OUT_OF_SYNC error code is set and <code>NULL</code> is returned.
 *			If the native service string is a <code>i18n_uchar*</code> string, it is converted to <code>char*</code> with the invariant converter.
 *			The result is terminated by <code>(char)0</code>. If the conversion fails (because a character cannot be converted)
 *			then the error code is set to #I18N_ERROR_INVARIANT_CONVERSION and the return value is undefined (but non-NULL).
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exceptions section.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] enumeration		An #i18n_uenumeration_h
 * @param[out] result_length	A pointer to receive the length of the result (not including the terminating \\0).
 *								If the pointer is <code>NULL</code> it is ignored.
 *
 * @return A pointer to the string. The string will be zero-terminated.
 *		   The return pointer is owned by this iterator and must not be deleted by the caller.
 *		   The pointer is valid until the next call to any i18n_uenumeration_... method, including i18n_uenumeration_next() or i18n_uenumeration_unext().
 *		   When all strings have been traversed, returns <code>NULL</code>.
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @exception #I18N_ERROR_ENUM_OUT_OF_SYNC The iterator is out of sync
 * @exception #I18N_ERROR_INVARIANT_CONVERSION The underlying native string is <code>i18n_uchar*</code> and conversion to <code>char*</code> with the invariant converter fails.
 *											   This error pertains only to current string, so iteration might be able to continue successfully.
 */
const char *i18n_uenumeration_next(i18n_uenumeration_h enumeration, int32_t *result_length);

/**
 * @brief Resets the iterator to the current list of service IDs.
 * @details This re-establishes sync with the service and rewinds the iterator to start at the first element.
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] enumeration	   An #i18n_uenumeration_h
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uenumeration_reset(i18n_uenumeration_h enumeration);

/**
 * @brief Given an array of <code>const i18n_uchar*</code> strings, return an #i18n_uenumeration_h.
 * @details String pointers from 0..count-1 must not be <code>NULL</code>.
 *			Do not free or modify either the string array or the characters it points to until this object has been destroyed with i18n_uenumeration_destroy().
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] strings	  An array of const <code>i18n_uchar*</code> strings (each null terminated). All storage is owned by the caller.
 * @param[in] count		  The length of the array
 * @param[out] enumeration A pointer to the new #i18n_uenumeration_h. Caller is responsible for calling i18n_uenumeration_destroy() to free memory.
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_uenumeration_destroy()
 */
int i18n_uenumeration_uchar_strings_enumeration_create(const i18n_uchar *const strings[], int32_t count, i18n_uenumeration_h *enumeration);

/**
 * @brief Given an array of <code>const char*</code> strings (invariant chars only), return an #i18n_uenumeration_h.
 * @details String pointers from 0..count-1 must not be <code>NULL</code>.
 *			Do not free or modify either the string array or the characters it points to until this object has been destroyed with i18n_uenumeration_destroy().
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] strings	  A array of <code>char*</code> strings (each null terminated). All storage is owned by the caller.
 * @param[in] count		  The length of the array
 * @param[out] enumeration A pointer to the new #i18n_uenumeration_h. Caller is responsible for calling i18n_uenumeration_destroy() to free memory
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_uenumeration_destroy()
 */
int i18n_uenumeration_char_strings_enumeration_create(const char *const strings[], int32_t count, i18n_uenumeration_h *enumeration);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
