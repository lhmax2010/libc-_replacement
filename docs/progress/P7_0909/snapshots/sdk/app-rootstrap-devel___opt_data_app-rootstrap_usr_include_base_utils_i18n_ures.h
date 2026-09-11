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
 * @file utils_i18n_ures.h
 * @version 0.1
 * @brief utils_i18n_uclean
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_URES_MODULE Ures
 * @brief Ures module provides API for Resource Bundle.
 *
 * @section CAPI_BASE_UTILS_I18N_URES_MODULE_HEADER Required Header
 *  \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_URES_MODULE_OVERVIEW Overview
 * @details C API representing a collection of resource information pertaining to a given
 *          locale. A resource bundle provides a way of accessing locale- specific information in
 *          a data file. You create a resource bundle that manages the resources for a given
 *          locale and then ask it for individual resources.
 *          <P>
 *          Resource bundles in ICU4C are currently defined using text files which conform to the following
 *          <a href="http://source.icu-project.org/repos/icu/icuhtml/trunk/design/bnf_rb.txt">BNF definition</a>.
 *          More on resource bundle concepts and syntax can be found in the
 *          <a href="http://icu-project.org/userguide/ResourceManagement.html">Users Guide</a>.
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_URES_MODULE
 * @{
 */

/**
 * @brief Creates a #i18n_ures_h, from which users can extract strings by using their corresponding keys.
 * @since_tizen 6.0
 * @remarks The @a ures should be released using i18n_ures_destroy().
 * @param[in]   package_name  The @a package_name and @a locale together point to an #i18n_ures_h.
 *                            Typically, @a package_name will refer to a (.dat) file.
 *                            Using a full file or directory pathname for @a package_name is deprecated. If NULL, ICU data will be used.
 * @param[in]   locale        Specifies the locale for which we want to create the resource
 *                            If NULL, the default locale will be used. If strlen(locale) == 0 root locale will be used.
 * @param[out]  ures          A new #i18n_ures_h.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @see i18n_ures_destroy()
 */
int i18n_ures_create(const char *package_name, const char *locale, i18n_ures_h *ures);

/**
 * @brief Creates a #i18n_ures_h and does not care what kind of localeID is passed in.
 * @details It simply creates a resource bundle with @a package_name. Fallback mechanism is disabled for the new resource bundle.
 *          If the requested bundle contains an %ALIAS directive, the results are undefined.
 * @since_tizen 6.0
 * @remarks The @a ures should be released using i18n_ures_destroy().
 * @param[in]   package_name  The @a package_name and @a locale together point to an #i18n_ures_h.
 *                            Typically, @a package_name will refer to a (.dat) file.
 *                            Using a full file or directory pathname for @a package_name is deprecated. If NULL, ICU data will be used.
 * @param[in]   locale        Specifies the locale for which we want to create the resource
 *                            If NULL, the default locale will be used. If strlen(locale) == 0 root locale will be used.
 * @param[out]  ures          A new #i18n_ures_h.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #I18N_ERROR_MISSING_RESOURCE The requested resource cannot be found
 *
 */
int i18n_ures_create_direct(const char *package_name, const char *locale, i18n_ures_h *ures);

/**
 * @brief Creates a #i18n_ures_h.
 * @details @a package_name will be converted to char * using the default converter, then i18n_ures_create() is called.
 * @since_tizen 6.0
 * @remarks The @a ures should be released using i18n_ures_destroy().
 * @param[in]   package_name  The @a package_name and @a locale together point to an #i18n_ures_h.
 *                            Typically, @a package_name will refer to a (.dat) file.
 *                            Using a full file or directory pathname for @a package_name is deprecated. If NULL, ICU data will be used.
 * @param[in]   locale        Specifies the locale for which we want to create the resource
 *                            If NULL, the default locale will be used. If strlen(locale) == 0 root locale will be used.
 * @param[out]  ures          A new #i18n_ures_h.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @see i18n_ures_create()
 *
 */
int i18n_ures_create_unicode(const i18n_uchar *package_name, const char *locale, i18n_ures_h *ures);

/**
 * @brief Destroys an #i18n_ures_h, all pointers returned from the various i18n_ures_getXXX calls on this particular resource bundle should be considered invalid henceforth.
 * @since_tizen 6.0
 * @param[in] ures An #i18n_ures_h.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ures_create()
 *
 */
int i18n_ures_destroy(i18n_ures_h ures);

/**
 * @brief Gets the version number associated with the #i18n_ures_h as an #i18n_uversion_info array.
 * @since_tizen 6.0
 * @param[in]  ures            The resource bundle for which the version is checked.
 * @param[out] version_array   A #i18n_uversion_info array that is filled with the version number as specified in the resource bundle or its parent.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ures_get_version(i18n_ures_h ures, i18n_uversion_info version_array);

/**
 * @brief Gets the name of the locale associated with @a ures.
 *        You can choose between requested, valid and real locale.
 * @since_tizen 6.0
 * @remarks The @a locale is available until @a ures is released. The @a locale should not be released.
 * @param[in]   ures     An #i18n_ures_h.
 * @param[in]   type     You can choose between requested, valid and actual locale.
 *                       For description see the definition of #i18n_ulocale_data_locale_type_e
 * @param[out]  locale   A Locale name
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ures_get_locale_by_type(i18n_ures_h ures, i18n_ulocale_data_locale_type_e type, const char **locale);

/**
 * @brief Gets a string from an #i18n_ures_h.
 * @since_tizen 6.0
 * @remarks The @a str_result is available until @a ures is released. The @a str_result should not be released.
 * @param[in]   ures        An #i18n_ures_h.
 * @param[out]  len         The length of resulting string
 * @param[out]  str_result  The pointer to a zero-terminated #i18n_uchar array which lives in a memory mapped/DLL file.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                       Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER          Invalid function parameter
 * @retval #I18N_ERROR_RESOURCE_TYPE_MISMATCH     An operation is requested over a resource that does not support it.
 * @see i18n_ures_get_binary()
 * @see i18n_ures_get_int_vector()
 * @see i18n_ures_get_int()
 * @see i18n_ures_get_uint()
 *
 */
int i18n_ures_get_string(i18n_ures_h ures, int32_t *len, const i18n_uchar **str_result);

/**
 * @brief Gets a UTF-8 string from a string resource.
 * @details The UTF-8 string may be returnable directly as a pointer, or it may need to be copied,
 *          or transformed from UTF-16 using i18n_ustring_to_UTF8() or equivalent.
 *          If @a force_copy==TRUE, then the string is always written to the dest buffer and dest is returned.
 *          If @a force_copy==FALSE, then the string is returned as a pointer if possible, without needing a dest buffer (it can be NULL).
 *          If the string needs to be copied or transformed, then it may be placed into dest at an arbitrary offset.
 *          If the string is to be written to dest, then #I18N_ERROR_BUFFER_OVERFLOW is set if appropriate, as usual.
 *          If the string is transformed from UTF-16, then a conversion error may occur if an unpaired surrogate is encountered.
 *          If the function is successful, then the output UTF-8 string is always well-formed.
 * @since_tizen 6.0
 * @remarks The @a str_result is available until @a ures is released. The @a str_result should not be released.
 * @param[in]       ures        Resource bundle.
 * @param[out]      dest        Destination buffer. Can be NULL only if capacity=*length==0.
 * @param[in,out]   len         Input: Capacity of destination buffer.
 *                              Output: Actual length of the UTF-8 string, not counting the terminating NUL,
 *                              even in case of #I18N_ERROR_BUFFER_OVERFLOW. Can be NULL, meaning capacity=0 and the string length is not returned to the caller.
 * @param[in]       force_copy  If TRUE, then the output string will always be written to dest,
 *                              with #I18N_ERROR_BUFFER_OVERFLOW set if appropriate.
 *                              If FALSE, then the dest buffer may or may not contain a copy of the string. dest may or may not be modified.
 *                              If a copy needs to be written, then retval indicates overflow etc. as usual.
 * @param[out]      str_result  The pointer to the UTF-8 string. It may be dest, or at some offset from dest (only if !force_copy), or in unrelated memory.
 *                              Always NUL-terminated unless the string was written to dest and length==capacity.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                       Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER          Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW            A result would not fit in the supplied buffer
 * @retval #I18N_ERROR_RESOURCE_TYPE_MISMATCH     An operation is requested over a resource that does not support it.
 * @see i18n_ures_get_string()
 * @see i18n_ustring_to_UTF8()
 *
 */
int i18n_ures_get_UTF8_string(i18n_ures_h ures, char *dest, int32_t *len, i18n_ubool force_copy, const char **str_result);

/**
 * @brief Gets a binary data from a binary resource.
 * @since_tizen 6.0
 * @remarks The @a binary_result is available until @a ures is released. The @a binary_result should not be released.
 * @param[in]   ures           Resource bundle.
 * @param[out]  len            The length of resulting byte chunk
 * @param[out]  binary_result  A pointer to a chunk of unsigned bytes which live in a memory mapped/DLL file.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_RESOURCE_TYPE_MISMATCH     An operation is requested over a resource that does not support it.
 * @see i18n_ures_get_string()
 * @see i18n_ures_get_int_vector()
 * @see i18n_ures_get_int()
 * @see i18n_ures_get_uint()
 *
 */
int i18n_ures_get_binary(i18n_ures_h ures, int32_t *len, const uint8_t **binary_result);

/**
 * @brief Gets a 32 bit integer array from a resource.
 * @since_tizen 6.0
 * @remarks The @a int_vector_result is available until @a ures is released. The @a int_vector_result should not be released.
 * @param[in]   ures               Resource bundle.
 * @param[out]  len                The length of resulting byte chunk
 * @param[out]  int_vector_result  A pointer to a chunk of integers which live in a memory mapped/DLL file.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_RESOURCE_TYPE_MISMATCH     An operation is requested over a resource that does not support it.
 * @see i18n_ures_get_string()
 * @see i18n_ures_get_binary()
 * @see i18n_ures_get_int()
 * @see i18n_ures_get_uint()
 *
 */
int i18n_ures_get_int_vector(i18n_ures_h ures, int32_t *len, const int32_t **int_vector_result);

/**
 * @brief Gets an unsigned integer from a resource. This integer is originally 28 bits.
 * @since_tizen 6.0
 * @param[in]   ures           Resource bundle.
 * @param[out]  uint_result    An integer value.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_RESOURCE_TYPE_MISMATCH     An operation is requested over a resource that does not support it.
 * @see i18n_ures_get_string()
 * @see i18n_ures_get_binary()
 * @see i18n_ures_get_int()
 * @see i18n_ures_get_int_vector()
 *
 */
int i18n_ures_get_uint(i18n_ures_h ures, uint32_t *uint_result);

/**
 * @brief Gets a signed integer from a resource.
 *        This integer is originally 28 bit and the sign gets propagated.
 * @since_tizen 6.0
 * @param[in]   ures           Resource bundle.
 * @param[out]  int_result     An integer value.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_RESOURCE_TYPE_MISMATCH     An operation is requested over a resource that does not support it.
 * @see i18n_ures_get_string()
 * @see i18n_ures_get_binary()
 * @see i18n_ures_get_uint()
 * @see i18n_ures_get_int_vector()
 *
 */
int i18n_ures_get_int(i18n_ures_h ures, int32_t *int_result);

/**
 * @brief    Gets the size of a resource.
 * @details  Size for scalar types is always 1, and for vector/table types is the number of child resources.
 * @since_tizen 6.0
 * @warning  Integer array is treated as a scalar type. There are no APIs to access individual members of an integer array. It is always returned as a whole.
 * @param[in]   ures           Resource bundle.
 * @param[out]  size_result    The number of resources in a given resource.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ures_get_size(i18n_ures_h ures, int32_t *size_result);

/**
 * @brief Gets the type of a resource. Available types are defined in enum #i18n_ures_type_e.
 * @since_tizen 6.0
 * @param[in]   ures           Resource bundle.
 * @param[out]  type_result    Type of the given resource.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ures_type_e
 *
 */
int i18n_ures_get_type(i18n_ures_h ures, i18n_ures_type_e *type_result);

/**
 * @brief Gets the key associated with a given resource.
 *        Not all the resources have a key - only those that are members of a table.
 * @since_tizen 6.0
 * @remarks The @a key_result is available until @a ures is released. The @a key_result should not be released.
 * @param[in]   ures           Resource bundle.
 * @param[out]  key_result     A key associated to this resource, or NULL if it doesn't have a key
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ures_get_key(i18n_ures_h ures, const char **key_result);

/**
 * @brief Resets the internal context of a resource so that iteration starts from the first element.
 * @since_tizen 6.0
 * @param[in]   ures           Resource bundle.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ures_reset_iterator(i18n_ures_h ures);

/**
 * @brief Checks whether the given resource has another element to iterate over.
 * @since_tizen 6.0
 * @param[in]   ures      Resource bundle.
 * @param[out]  has_next  TRUE if there are more elements, FALSE if there is no more elements
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ures_has_next(i18n_ures_h ures, bool *has_next);

/**
 * @brief Gets the next resource in a given resource or NULL if there are no more resources to iterate over. Features a fill-in parameter.
 * @details If @a fill_in is not NULL, it will be filled with the next resource, and a new #i18n_ures_h in @a result will not be created.
 *          If @a fill_in is NULL, then @a result will be set to a new #i18n_ures_h containing the next resource.
 *
 *          If there are no more resources to iterate over, @a result will be set to NULL.
 *
 *          This function may set @a result to a non-NULL value even if an error occurs. This function's return value should be checked.
 * @since_tizen 6.0
 * @remarks If @a fill_in is NULL, the @a result should be released using i18n_ures_destroy().
 * @param[in]      ures        Resource bundle.
 * @param[in,out]  fill_in     A #i18n_ures_h handle. It can be NULL.
 * @param[out]     result      Cannot be NULL. Will be set to a new #i18n_ures_h if @a fill_in is NULL. Will be set to NULL if there are no more resources to iterate over.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ures_get_next_resource(i18n_ures_h ures, i18n_ures_h fill_in, i18n_ures_h *result);

/**
 * @brief Gets the next string in a given resource
 *         or NULL if there are no more resources to iterate over.
 * @since_tizen 6.0
 * @remarks The @a key is available until @a ures is released. The @a key should not be released.
 * @remarks The @a str_result is available until @a ures is released. The @a str_result should not be released.
 * @param[in]   ures        Resource bundle.
 * @param[out]  len         Fill in length of the string
 * @param[out]  key         Fill in for key associated with this string. NULL if no key
 * @param[out]  str_result  A pointer to a zero-terminated #i18n_uchar array which lives in a memory mapped/DLL file.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ures_get_next_string(i18n_ures_h ures, int32_t *len, const char **key, const i18n_uchar **str_result);

/**
 * @brief Gets the resource in a given resource at the specified index. Features a fill-in parameter.
 * @details If @a fill_in is not NULL, it will be filled with output data, and @a result will not be created. @a result can be NULL in this case.
 *          If @a fill_in is NULL, then a new handle will be created in @a result. @a result cannot be NULL in this case.
 *          This function may set @a ures_result to a non-NULL value even if an error occurs. This function's return value should be checked.
 * @since_tizen 6.0
 * @remarks If @a fill_in is NULL, the @a result should be released using i18n_ures_destroy().
 * @param[in]      ures         Resource bundle.
 * @param[in]      index_r      An index to the wanted resource.
 * @param[in,out]  fill_in      A #i18n_ures_h handle. It can be NULL.
 * @param[out]     ures_result  A new #i18n_ures_h if @a fill_in is NULL. Otherwise no changes are made to this parameter.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ures_get_by_index(i18n_ures_h ures, int32_t index_r, i18n_ures_h fill_in, i18n_ures_h *ures_result);

/**
 * @brief Gets the string in a given resource at the specified index.
 * @since_tizen 6.0
 * @remarks The @a str_result is available until @a ures is released. The @a str_result should not be released.
 * @param[in]  ures        Resource bundle.
 * @param[in]  index_s     An index to the wanted string
 * @param[out] len         Fill in length of the string
 * @param[out] str_result  A pointer to a zero-terminated #i18n_uchar array which lives in a memory mapped/DLL file.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                       Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER          Invalid function parameter
 * @retval #I18N_ERROR_RESOURCE_TYPE_MISMATCH     An operation is requested over a resource that does not support it.
 *
 *
 */
int i18n_ures_get_string_by_index(i18n_ures_h ures, int32_t index_s, int32_t *len, const i18n_uchar **str_result);

/**
 * @brief Gets a UTF-8 string from a resource at the specified index.
 * @details The UTF-8 string may be returnable directly as a pointer, or it may need to be copied,
 *          or transformed from UTF-16 using #i18n_ustring_to_UTF8() or equivalent.
 *          If force_copy==TRUE, then the string is always written to the dest buffer and dest is returned.
 *          If force_copy==FALSE, then the string is returned as a pointer if possible, without needing a dest buffer (it can be NULL).
 *          If the string needs to be copied or transformed, then it may be placed into dest at an arbitrary offset.
 *          If the string is to be written to dest, then #I18N_ERROR_BUFFER_OVERFLOW is set if appropriate, as usual.
 *          If the string is transformed from UTF-16, then a conversion error may occur if an unpaired surrogate is encountered.
 *          If the function is successful, then the output UTF-8 string is always well-formed.
 * @since_tizen 6.0
 * @remarks The @a str_result is available until @a ures is released. The @a str_result should not be released.
 * @param[in]     ures        Resource bundle.
 * @param[in]     index_s     An index to the wanted string
 * @param[out]    dest        Destination buffer. Can be NULL only if capacity=*length==0.
 * @param[in,out] len         Input: Capacity of destination buffer.
 *                            Output: Actual length of the UTF-8 string, not counting the terminating NUL,
 *                            even in case of #I18N_ERROR_BUFFER_OVERFLOW. Can be NULL, meaning capacity=0 and the string length is not returned to the caller.
 * @param[in]     force_copy  If TRUE, then the output string will always be written to dest,
 *                            with #I18N_ERROR_BUFFER_OVERFLOW set if appropriate.
 *                            If FALSE, then the dest buffer may or may not contain a copy of the string. dest may or may not be modified.
 *                            If a copy needs to be written, then the I18N_ERROR parameter indicates overflow etc. as usual.
 * @param[out]    str_result  The pointer to the UTF-8 string. It may be dest, or at some offset from dest (only if !force_copy), or in unrelated memory.
 *                            Always NUL-terminated unless the string was written to dest and length==capacity.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                       Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER          Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW            A result would not fit in the supplied buffer
 * @retval #I18N_ERROR_RESOURCE_TYPE_MISMATCH     An operation is requested over a resource that does not support it.
 * @see i18n_ures_get_string_by_index()
 * @see i18n_ustring_to_UTF8()
 *
 */
int i18n_ures_get_UTF8_string_by_index(i18n_ures_h ures, int32_t index_s, char *dest, int32_t *len, i18n_ubool force_copy, const char **str_result);

/**
 * @brief Gets a resource in a given resource that has a given key.
 *        This procedure works only with table resources. Features a fill-in parameter.
 * @details If @a fill_in is not NULL, it will be filled with output data, and @a result will not be created. @a result can be NULL in this case.
 *          If @a fill_in is NULL, then a new handle will be created in @a result. @a result cannot be NULL in this case.
 * @since_tizen 6.0
 * @remarks If @a fill_in is NULL, the @a result should be released using i18n_ures_destroy().
 * @param[in]      ures         Resource bundle.
 * @param[in]      key          A key associated with the wanted resource
 * @param[in,out]  fill_in      A #i18n_ures_h handle. It can be NULL.
 * @param[out]     ures_result  A new #i18n_ures_h if @a fill_in is NULL. Otherwise no changes are made to this parameter.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ures_get_by_key(i18n_ures_h ures, const char *key, i18n_ures_h fill_in, i18n_ures_h *ures_result);

/**
 * @brief Gets a string in a given resource that has a given key.
 *        This procedure works only with table resources.
 * @since_tizen 6.0
 * @remarks The @a str_result is available until @a ures is released. The @a str_result should not be released.
 * @param[in]  ures        Resource bundle.
 * @param[in]  key         A key associated with the wanted resource
 * @param[out] len         Fills in length of the string
 * @param[out] str_result  A pointer to a zero-terminated #i18n_uchar array which lives in a memory mapped/DLL file.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ures_get_string_by_key(i18n_ures_h ures, const char *key, int32_t *len, const i18n_uchar **str_result);

/**
 * @brief Gets a UTF-8 string from a resource and a key. This function works only with table resources.
 * @details The UTF-8 string may be returnable directly as a pointer, or it may need to be copied,
 *          or transformed from UTF-16 using #i18n_ustring_to_UTF8() or equivalent.
 *          If force_copy==TRUE, then the string is always written to the dest buffer and dest is returned.
 *          If force_copy==FALSE, then the string is returned as a pointer if possible, without needing a dest buffer (it can be NULL).
 *          If the string needs to be copied or transformed, then it may be placed into dest at an arbitrary offset.
 *          If the string is to be written to dest, then #I18N_ERROR_BUFFER_OVERFLOW is set if appropriate, as usual.
 *          If the string is transformed from UTF-16, then a conversion error may occur if an unpaired surrogate is encountered.
 *          If the function is successful, then the output UTF-8 string is always well-formed.
 * @since_tizen 6.0
 * @remarks The @a str_result is available until @a ures is released.
 * @param[in]     ures        Resource bundle.
 * @param[in]     key         A key associated with the wanted resource
 * @param[out]    dest        Destination buffer. Can be NULL only if capacity=*length==0.
 * @param[in,out] len         Input: Capacity of destination buffer.
 *                            Output: Actual length of the UTF-8 string, not counting the terminating NUL,
 *                            even in case of #I18N_ERROR_BUFFER_OVERFLOW. Can be NULL, meaning capacity=0 and the string length is not returned to the caller.
 * @param[in]     force_copy  If TRUE, then the output string will always be written to dest,
 *                            with #I18N_ERROR_BUFFER_OVERFLOW set if appropriate.
 *                            If FALSE, then the dest buffer may or may not contain a copy of the string. dest may or may not be modified.
 *                            If a copy needs to be written, then the I18N_ERROR parameter indicates overflow etc. as usual.
 * @param[out]    str_result  The pointer to the UTF-8 string. It may be dest, or at some offset from dest (only if !force_copy), or in unrelated memory.
 *                            Always NUL-terminated unless the string was written to dest and length==capacity.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ures_get_string_by_key()
 * @see i18n_ustring_to_UTF8()
 *
 */
int i18n_ures_get_UTF8_string_by_key(i18n_ures_h ures, const char *key, char *dest, int32_t *len, i18n_ubool force_copy, const char **str_result);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif
