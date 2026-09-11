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
 * @file utils_i18n_ucnvsel.h
 * @version 0.1
 * @brief utils_i18n_ucnvsel
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UCONVERTER_SELECTOR_MODULE Ucnvsel
 * @brief C API: Charset Detection API.
 * @section CAPI_BASE_UTILS_I18N_UCONVERTER_SELECTOR_MODULE_HEADER Required Header
 *  \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UCONVERTER_SELECTOR_MODULE_OVERVIEW Overview
 * @details A converter selector is built with a set of encoding/charset names
 *          and given an input string returns the set of names of the corresponding converters which can convert the string.
 *
 *          A converter selector can be serialized into a buffer and created from the serialized form.
 *
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UCONVERTER_SELECTOR_MODULE
 * @{
 */

/**
 * @brief   Creates an #i18n_uconverter_selector_h.
 * @details If converter_list_size is 0, build for all available converters. If excluded_code_points is NULL, don't exclude any code points.
 * @since_tizen 6.0
 * @remarks The @a sel should be released using #i18n_ucnvsel_destroy().
 * @param[in] converter_list        A pointer to encoding names needed to be involved. Can be NULL if @a converter_list_size ==0.
 *                                  The list and the names will be cloned, and the caller retains ownership of the original.
 * @param[in] converter_list_size   Number of encodings in above list. If 0, builds a selector for all available converters.
 * @param[in] excluded_code_points  A set of code points to be excluded from consideration.
 *                                  That is, excluded code points in a string do not change the selection result. (They might be handled by a callback.)
 *                                  Use NULL to exclude nothing.
 * @param[in] which_set             What converter set to use? Use this to determine whether to consider only roundtrip mappings or also fallbacks.
 * @param[out] sel                  The new selector.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucnvsel_create(const char *const *converter_list, int32_t converter_list_size, const i18n_uset_h excluded_code_points, const i18n_ucnv_unicode_set_e which_set, i18n_uconverter_selector_h *sel);

/**
 * @brief   Destroys an #i18n_uconverter_selector_h.
 * @details If any enumerations were returned by i18n_ucnvsel*, they become invalid.
 *          They can be destroyed before or after calling #i18n_ucnv_destroy, but should never be used after the selector is destroyed.
 * @since_tizen 6.0
 * @param[in] sel    The selector to destroy.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucnvsel_destroy(i18n_uconverter_selector_h sel);

/**
 * @brief   Creates an #i18n_uconverter_selector_h from its serialized form.
 * @details The buffer must remain valid and unchanged for the lifetime of the selector.
 *          This is much faster than creating a selector from scratch. Using a serialized form from a different machine (endianness/charset) is supported.
 * @since_tizen 6.0
 * @remarks The @a sel should be released using #i18n_ucnvsel_destroy().
 * @param[in] buffer    Pointer to the serialized form of a converter selector; must be 32-bit-aligned
 * @param[in] length    The capacity of this buffer (can be equal to or larger than the actual data length)
 * @param[out] sel      The new selector.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucnvsel_create_from_serialized(const void *buffer, int32_t length, i18n_uconverter_selector_h *sel);

/**
 * @brief   Serializes a selector into a linear buffer.
 * @details The serialized form is portable to different machines.
 * @since_tizen 6.0
 * @param[in] sel               Selector to consider
 * @param[in] buffer            Pointer to 32-bit-aligned memory to be filled with the serialized form of this converter selector
 * @param[in] buffer_capacity   The capacity of this buffer
 * @param[out] capacity         The required buffer capacity to hold serialize data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucnvsel_serialize(const i18n_uconverter_selector_h sel, void *buffer, int32_t buffer_capacity, int32_t *capacity);

/**
 * @brief   Selects converters that can map all characters in a UTF-16 string, ignoring the excluded code points.
 * @since_tizen 6.0
 * @remarks The @a enumeration is valid until @a sel is released.
 * @param[in] sel          An #i18n_uconverter_selector_h
 * @param[in] string       UTF-16 string
 * @param[in] length       Length of the string, or -1 if NUL-terminated
 * @param[out] enumeration An enumeration containing encoding names. The returned encoding names and their order will be the same as supplied when building the selector.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @code
 * int main() {
 * 	i18n_uenumeration_h enumeration = NULL;
 * 	i18n_uconverter_selector_h sel;
 * 	i18n_uset_h excluded_set = NULL;
 * 	const char *char_str = "Hello World!";
 * 	const i18n_uchar string[20] = {0,};
 * 	i18n_error_code_e error_code;
 * 	int32_t length;
 *
 * 	i18n_ustring_from_UTF8((i18n_uchar *)string, I18N_UPRV_LENGTHOF(string), &length, char_str, 5, &error_code);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ustring_from_UTF8: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	error_code = i18n_uset_create_empty(&excluded_set);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_uset_create_empty: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	error_code = i18n_ucnvsel_create(NULL, 0, excluded_set, I18N_UCNV_ROUNDTRIP_AND_FALLBACK_SET, &sel);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucnvsel_create: %d\n", error_code);
 * 		i18n_uset_destroy(excluded_set);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	error_code = i18n_ucnvsel_select_for_string(sel, string, length, &enumeration);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucnvsel_select_for_string: %d\n", error_code);
 * 		i18n_uset_destroy(excluded_set);
 * 		i18n_ucnvsel_destroy(sel);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	i18n_uset_destroy(excluded_set);
 * 	i18n_ucnvsel_destroy(sel);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucnvsel_select_for_string(const i18n_uconverter_selector_h sel, const i18n_uchar *string, int32_t length, i18n_uenumeration_h *enumeration);

/**
 * @brief   Selects converters that can map all characters in a UTF-8 string, ignoring the excluded code points.
 * @since_tizen 6.0
 * @remarks The @a enumeration is valid until @a sel is released.
 * @param[in] sel          An #i18n_uconverter_selector_h
 * @param[in] string       UTF-8 string
 * @param[in] length       Length of the string, or -1 if NUL-terminated
 * @param[out] enumeration An enumeration containing encoding names. The returned encoding names and their order will be the same as supplied when building the selector.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @code
 * #include <stdio.h>
 * #include <string.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 *
 * int main() {
 * 	i18n_uenumeration_h enumeration = NULL;
 * 	i18n_uconverter_selector_h sel;
 * 	i18n_uset_h excluded_set = NULL;
 * 	const char *string = "Hello World!";
 * 	i18n_error_code_e error_code;
 *
 * 	error_code = i18n_uset_create_empty(&excluded_set);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_uset_create_empty: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	error_code = i18n_ucnvsel_create(NULL, 0, excluded_set, I18N_UCNV_ROUNDTRIP_AND_FALLBACK_SET, &sel);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucnvsel_create: %d\n", error_code);
 * 		i18n_uset_destroy(excluded_set);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	error_code = i18n_ucnvsel_select_for_utf8(sel, string, strlen(string), &enumeration);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucnvsel_select_for_utf8: %d\n", error_code);
 * 		i18n_ucnvsel_destroy(sel);
 * 		i18n_uset_destroy(excluded_set);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	i18n_ucnvsel_destroy(sel);
 * 	i18n_uset_destroy(excluded_set);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucnvsel_select_for_utf8(const i18n_uconverter_selector_h sel, const char *string, int32_t length, i18n_uenumeration_h *enumeration);


#ifdef __cplusplus
}
#endif

/**
 * @}
 */
