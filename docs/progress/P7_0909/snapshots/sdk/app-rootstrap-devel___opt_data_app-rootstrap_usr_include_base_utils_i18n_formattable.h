/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @file utils_i18n_formattable.h
 * @version 0.1
 * @brief utils_i18n_formattable
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE Formattable
 * @brief Formattable interconverts between the primitive numeric types
 *		  (double, long, etc.) as well as #i18n_udate and char string.
 *
 * @section CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE_OVERVIEW Overview
 * @details Internally, an #i18n_formattable_h handle holds an object
 *			that is a union of primitive types.
 *			As such, it can only store one flavor of data at a time.
 *			To determine what flavor of data it contains, use the
 *			#i18n_formattable_get_type function.
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE
 * @{
 */

/**
 * @brief Creates a new default #i18n_formattable_h.
 * @since_tizen 2.3.2
 * @remarks The created object should be released by the caller with the
 *			#i18n_formattable_destroy() function.
 *
 * @param[out] formattable	A pointer to a handle to the newly created formattable
 *							object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_formattable_create_default(i18n_formattable_h *formattable);

/**
 * @brief Creates a new #i18n_formattable_h handle with an #i18n_udate instance.
 * @since_tizen 2.3.2
 * @remarks The created object should be released by the caller with the
 *			#i18n_formattable_destroy() function.
 *
 * @param[in] date			The #i18n_udate instance
 * @param[out] formattable	A pointer to a handle to the newly created formattable
 *							object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_formattable_create_with_udate(i18n_udate date, i18n_formattable_h *formattable);

/**
 * @brief Creates a new #i18n_formattable_h handle with a double value.
 * @since_tizen 2.3.2
 * @remarks The created object should be released by the caller with the
 *			#i18n_formattable_destroy() function.
 *
 * @param[in] value			The double value to be used
 * @param[out] formattable	A pointer to a handle to the newly created formattable
 *							object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_formattable_create_with_double(double value, i18n_formattable_h *formattable);

/**
 * @brief Creates a new #i18n_formattable_h handle with a long value.
 * @since_tizen 2.3.2
 * @remarks The created object should be released by the caller with the
 *			#i18n_formattable_destroy() function.
 *
 * @param[in] value			The long value to be used
 * @param[out] formattable	A pointer to a handle to the newly created formattable
 *							object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_formattable_create_with_long(int32_t value, i18n_formattable_h *formattable);

/**
 * @brief Creates a new #i18n_formattable_h handle with an int64_t value.
 * @since_tizen 2.3.2
 * @remarks The created object should be released by the caller with the
 *			#i18n_formattable_destroy() function.
 *
 * @param[in] value			The int64_t value to be used
 * @param[out] formattable	A pointer to a handle to the newly created formattable
 *							object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_formattable_create_with_int64(int64_t value, i18n_formattable_h *formattable);

/**
 * @brief Creates a new #i18n_formattable_h handle with a char string pointer.
 * @details Assumes that the char string is null terminated.
 * @since_tizen 2.3.2
 * @remarks The created object should be released by the caller with the
 *			#i18n_formattable_destroy() function.
 *
 * @param[in] str_to_copy	The char string
 * @param[out] formattable	A pointer to a handle to the newly created formattable
 *							object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_formattable_create_with_char_string(const char *str_to_copy, i18n_formattable_h *formattable);

/**
 * @brief Creates a new #i18n_formattable_h handle with an array of
 *		  #i18n_formattable_h handles.
 * @since_tizen 2.3.2
 * @remarks The created object should be released by the caller with the
 *			#i18n_formattable_destroy() function.
 *
 * @param[in] array_to_copy	 An array with the #i18n_formattable_h handles
 * @param[in] count			 The number of the elements in the array
 * @param[out] formattable	  A pointer to a handle to the newly created
 *							  formattable object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_formattable_create_with_formattable_array(const i18n_formattable_h *array_to_copy, int32_t count, i18n_formattable_h *formattable);

/**
 * @brief Releases the given #i18n_formattable_h handle.
 * @since_tizen 2.3.2
 *
 * @param[in] formattable  A handle to the formattable object to be released
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_formattable_destroy(i18n_formattable_h formattable);

/**
 * @brief Clones the given formattable handle with the related object to the
 *		  @a clone handle.
 * @since_tizen 2.3.2
 * @details Clones can be used concurrently in multiple threads.
 * @remarks The cloned object should be released by the caller with the
 *			#i18n_formattable_destroy() function.
 *
 * @param[in] formattable  A handle to the formattable object
 * @param[out] clone		A pointer to a handle to the formattable object which
 *							will be filled with a copy of the @a formattable handle.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_formattable_clone(i18n_formattable_h formattable, i18n_formattable_h *clone);

/**
 * @brief Gets the array value and count of the given formattable object.
 * @since_tizen 2.3.2
 * @remarks If this object is not of type #I18N_FORMATTABLE_TYPE_ARRAY then the
 *			result is undefined. The obtained array should be released by the
 *			caller with the free() function.
 *
 * @param[in] formattable	A handle to the formattable object
 * @param[out] array		A pointer to an array of #i18n_formattable_h handles
 * @param[out] count		A pointer to an int32_t variable which will be filled
 *							with the number of array's elements
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_formattable_get_array(i18n_formattable_h formattable, i18n_formattable_h **array, int32_t *count);

/**
 * @brief Gets the date value of the given formattable object.
 * @since_tizen 2.3.2
 * @remarks If this object is not of type #I18N_FORMATTABLE_TYPE_DATE then the
 *			result is undefined.
 *
 * @param[in] formattable  A handle to the formattable object
 * @param[out] date			A pointer to an i18n_udate object which will be filled
 *							with the date obtained from the given formattable object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_formattable_get_date(i18n_formattable_h formattable, i18n_udate *date);

/**
 * @brief Gets the double value of the given formattable object.
 * @since_tizen 2.3.2
 * @remarks If this object is not of type #I18N_FORMATTABLE_TYPE_DOUBLE then the
 *			result is undefined.
 *
 * @param[in] formattable  A handle to the formattable object
 * @param[out] value		A pointer to a double variable which will be filled
 *							with the value obtained from the given formattable object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_formattable_get_double(i18n_formattable_h formattable, double *value);

/**
 * @brief Gets the int64 value of the given formattable object.
 * @since_tizen 2.3.2
 * @remarks If this object is not of type #I18N_FORMATTABLE_TYPE_INT64 then the
 *			result is undefined.
 *
 * @param[in] formattable  A handle to the formattable object
 * @param[out] value		A pointer to an int64_t variable which will be filled
 *							with the value obtained from the given formattable object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_formattable_get_int64(i18n_formattable_h formattable, int64_t *value);

/**
 * @brief Gets the long value of the given formattable object.
 * @details If the magnitude is too large to fit in a long, then the maximum or
 *			minimum long value, as appropriate, is set to @a value
 *			and the #I18N_ERROR_INVALID_FORMAT error code is returned by the
 *			function.
 * @since_tizen 2.3.2
 *
 * @param[in] formattable	A handle to the formattable object
 * @param[out] value		A pointer to a int32_t variable which will be filled
 *							with the value obtained from the given formattable object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_formattable_get_long(i18n_formattable_h formattable, int32_t *value);

/**
 * @brief Gets the string value of the given formattable object.
 * @since_tizen 2.3.2
 * @remarks If the type is not a string, the function returns the
 *			#I18N_ERROR_INVALID_FORMAT error code and the value is set to @c
 *			NULL. The @a value should be released by the caller with the free() function.
 *
 * @param[in] formattable  A handle to the formattable object
 * @param[out] value		A pointer to a char string variable which will be
 *							filled with the value obtained from the given formattable object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_INVALID_FORMAT The given formattable type is not a string
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #I18N_ERROR_UNKNOWN Unknown error
 */
int i18n_formattable_get_string(i18n_formattable_h formattable, char **value);

/**
 * @brief Gets the data type of the given formattable object.
 * @since_tizen 2.3.2
 *
 * @param[in] formattable  A handle to the formattable object
 * @param[out] type		   A pointer to an #i18n_formattable_type_e variable which
 *						   will be filled with the type obtained from the given formattable
 *						   object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_formattable_get_type(i18n_formattable_h formattable, i18n_formattable_type_e *type);

/**
 * @brief Sets the variable pointed by the @a is_numeric pointer to @c true if
 *		  the data type of the given formattable object is #I18N_FORMATTABLE_TYPE_DOUBLE,
 *		  #I18N_FORMATTABLE_TYPE_LONG or #I18N_FORMATTABLE_TYPE_INT64.
 * @since_tizen 2.3.2
 *
 * @param[in] formattable  A handle to the formattable object
 * @param[out] is_numeric  A pointer to a boolean variable which will be filled by
 *						   the function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_formattable_is_numeric(i18n_formattable_h formattable, bool *is_numeric);

/**
 * @brief Sets the variable pointed by the @a not_equal pointer to @c true if
 *		  the given @a formattable object is not equal to the given @a other
 *		  formattable object.
 * @since_tizen 2.3.2
 *
 * @param[in] formattable  A handle to the formattable object
 * @param[in] other		   A handle to the other formattable object
 * @param[out] not_equal   A pointer to a boolean variable which will be filled by
 *						   the function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_formattable_not_equal(i18n_formattable_h formattable, i18n_formattable_h other, bool *not_equal);

/**
 * @brief Sets the variable pointed by the @a equal pointer to @c true if the
 *		  given @a formattable object is equal to the given @a other
 *		  formattable object.
 * @since_tizen 2.3.2
 *
 * @param[in] formattable  A handle to the formattable object
 * @param[in] other		   A handle to the other formattable object
 * @param[out] equal		A pointer to a boolean variable which will be filled by
 *							the function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_formattable_equal(i18n_formattable_h formattable, i18n_formattable_h other, bool *equal);

/**
 * @brief Sets the object pointed by the @a element pointer to the element at
 *		  the @a index position in the array stored by the given formattable
 *		  object (if its type is #I18N_FORMATTABLE_TYPE_ARRAY).
 * @since_tizen 2.3.2
 * @remarks If this object is not of type #I18N_FORMATTABLE_TYPE_ARRAY then the
 *			result is undefined.
 *
 *			The obtained formattable element should not be released by the caller
 *			as it belongs to the given formattable object of type #I18N_FORMATTABLE_TYPE_ARRAY.
 *
 * @param[in] formattable  A handle to the formattable object
 * @param[in] index		   A number representing the position in the array from
 *						   which the element should be obtained
 * @param[out] element		A pointer to a handle to the formattable object which
 *							will be filled by the function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_INDEX_OUTOFBOUNDS Index value out of range
 * @retval #I18N_ERROR_INVALID_FORMAT The given formattable type is not an array
 */
int i18n_formattable_element_at(i18n_formattable_h formattable, int32_t index, i18n_formattable_h *element);

/**
 * @brief Sets the array value and count of the given formattable object and
 *		  changes the type to #I18N_FORMATTABLE_TYPE_ARRAY.
 * @since_tizen 2.3.2
 *
 * @param[in] formattable  A handle to the formattable object
 * @param[in] array		   An array of handles to the formattable objects
 * @param[in] count		   The number of array elements to be copied
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_formattable_set_array(i18n_formattable_h formattable, const i18n_formattable_h *array, int32_t count);

/**
 * @brief Sets the date value of the given formattable object and changes the
 *		  type to the #I18N_FORMATTABLE_TYPE_DATE.
 * @since_tizen 2.3.2
 *
 * @param[in] formattable  A handle to the formattable object
 * @param[in] date		   The new i18n_udate value to be set
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_formattable_set_date(i18n_formattable_h formattable, i18n_udate date);

/**
 * @brief Sets the double value of the given formattable object and changes
 *		  the type to the #I18N_FORMATTABLE_TYPE_DOUBLE.
 * @since_tizen 2.3.2
 *
 * @param[in] formattable  A handle to the formattable object
 * @param[in] value		   The new double value to be set
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_formattable_set_double(i18n_formattable_h formattable, double value);

/**
 * @brief Sets the int64 value of the given formattable object and changes the
 *		  type to the #I18N_FORMATTABLE_TYPE_INT64.
 * @since_tizen 2.3.2
 *
 * @param[in] formattable  A handle to the formattable object
 * @param[in] value		   The new int64_t value to be set
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_formattable_set_int64(i18n_formattable_h formattable, int64_t value);

/**
 * @brief Sets the long value of the given formattable object and changes the
 *		  type to the #I18N_FORMATTABLE_TYPE_LONG.
 * @since_tizen 2.3.2
 *
 * @param[in] formattable  A handle to the formattable object
 * @param[in] value		   The new int32_t value to be set
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_formattable_set_long(i18n_formattable_h formattable, int32_t value);

/**
 * @brief Sets the string value of the given formattable object and changes
 *		  the type to the #I18N_FORMATTABLE_TYPE_STRING.
 * @since_tizen 2.3.2
 *
 * @param[in] formattable	  A handle to the formattable object
 * @param[in] string_to_copy  The new string value to be set
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_formattable_set_string(i18n_formattable_h formattable, const char *string_to_copy);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
