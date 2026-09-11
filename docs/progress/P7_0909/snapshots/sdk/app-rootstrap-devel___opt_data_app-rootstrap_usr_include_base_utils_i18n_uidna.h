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
 * @file utils_i18n_uidna.h
 * @version 0.1
 * @brief utils_i18n_uidna
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UIDNA_MODULE Uidna
 * @brief Uidna module provides Internationalizing Domain Names in Applications.
 * @section CAPI_BASE_UTILS_I18N_UIDNA_MODULE_HEADER Required Header
 *  \#include <utils_i18n.h>
 *
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UIDNA_MODULE
 * @{
 */

/**
 * @brief Returns a UIDNA instance which implements UTS #46.
 * @details Returns an unmodifiable instance, owned by the caller. Cache it for multiple operations,
 *          and i18n_uidna_destroy() it when done. The instance is thread-safe, that is, it can be used concurrently.
 * @since_tizen 6.0
 * @remarks The @a uidna should be released using i18n_uidna_destroy().
 * @param[in] options   Bit set to modify the processing and error checking. See #i18n_uidna_option_e for options.
 * @param[out] uidna    The UTS #46 UIDNA instance, if successful
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @see i18n_uidna_destroy
 */
int i18n_uidna_create(uint32_t options, i18n_uidna_h *uidna);

/**
 * @brief Destroys a UIDNA instance.
 * @since_tizen 6.0
 *
 * @param[in] uidna UIDNA instance to be destroyed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_uidna_create
 */
int i18n_uidna_destroy(i18n_uidna_h uidna);

/**
 * @brief Converts a single domain name label into its ASCII form for DNS lookup.
 * @details The label might be modified according to the types of errors. Labels with severe errors will be left in (or turned into) their Unicode form.
 *          The #i18n_error_code_e indicates an error only in exceptional cases, such as a #I18N_ERROR_OUT_OF_MEMORY.
 * @since_tizen 6.0
 * @remarks @a info should be released with i18n_uidna_info_destroy().
 *
 * @param[in] uidna     A UIDNA instance
 * @param[in] label     Input domain name label
 * @param[in] length    Label length, or -1 if NUL-terminated
 * @param[in] dest      Destination string buffer
 * @param[in] capacity  Destination buffer capacity
 * @param[out] info     Output container of IDNA processing details.
 * @param[out] len      Destination string length
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_uidna_label_to_ascii(i18n_uidna_h uidna, const i18n_uchar *label, int32_t length, i18n_uchar *dest, int32_t capacity, i18n_uidna_info_h *info, int32_t *len);

/**
 * @brief Converts a single domain name label into its Unicode form for human-readable display.
 * @details The #i18n_error_code_e indicates an error only in exceptional cases, such as a #I18N_ERROR_OUT_OF_MEMORY.
 * @since_tizen 6.0
 * @remarks @a info should be released with i18n_uidna_info_destroy().
 *
 * @param[in] uidna     A UIDNA instance
 * @param[in] label     Input domain name label
 * @param[in] length    Label length, or -1 if NUL-terminated
 * @param[in] dest      Destination string buffer
 * @param[in] capacity  Destination buffer capacity
 * @param[out] info     Output container of IDNA processing details.
 * @param[out] len      Destination string length
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_uidna_label_to_unicode(i18n_uidna_h uidna, const i18n_uchar *label, int32_t length, i18n_uchar *dest, int32_t capacity, i18n_uidna_info_h *info, int32_t *len);

/**
 * @brief Converts a whole domain name into its ASCII form for DNS lookup.
 * @details The domain name might be modified according to the types of errors. Labels with severe errors will be left in (or turned into) their Unicode form.
 *          The #i18n_error_code_e indicates an error only in exceptional cases, such as a #I18N_ERROR_OUT_OF_MEMORY.
 * @since_tizen 6.0
 * @remarks @a info should be released with i18n_uidna_info_destroy().
 *
 * @param[in] uidna     A UIDNA instance
 * @param[in] name      Input domain name
 * @param[in] length    Label length, or -1 if NUL-terminated
 * @param[in] dest      Destination string buffer
 * @param[in] capacity  Destination buffer capacity
 * @param[out] info     Output container of IDNA processing details.
 * @param[out] len      Destination string length
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_uidna_name_to_ascii(i18n_uidna_h uidna, const i18n_uchar *name, int32_t length, i18n_uchar *dest, int32_t capacity, i18n_uidna_info_h *info, int32_t *len);

/**
 * @brief Converts a whole domain name into its Unicode form for human-readable display.
 * @details The #i18n_error_code_e indicates an error only in exceptional cases, such as a #I18N_ERROR_OUT_OF_MEMORY.
 * @since_tizen 6.0
 * @remarks @a info should be released with i18n_uidna_info_destroy().
 *
 * @param[in] uidna     A UIDNA instance
 * @param[in] name      Input domain name
 * @param[in] length    Label length, or -1 if NUL-terminated
 * @param[in] dest      Destination string buffer
 * @param[in] capacity  Destination buffer capacity
 * @param[out] info     Output container of IDNA processing details.
 * @param[out] len      Destination string length
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_uidna_name_to_unicode(i18n_uidna_h uidna, const i18n_uchar *name, int32_t length, i18n_uchar *dest, int32_t capacity, i18n_uidna_info_h *info, int32_t *len);

/**
 * @brief Converts a single domain name label (UTF8 encoded) into its ASCII form for DNS lookup.
 * @since_tizen 6.0
 * @remarks @a info should be released with i18n_uidna_info_destroy().
 *
 * @param[in] uidna     A UIDNA instance
 * @param[in] label     Input domain name label
 * @param[in] length    Label length, or -1 if NUL-terminated
 * @param[in] dest      Destination string buffer
 * @param[in] capacity  Destination buffer capacity
 * @param[out] info     Output container of IDNA processing details.
 * @param[out] len      Destination string length
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_uidna_label_to_ascii_UTF8(i18n_uidna_h uidna, const char *label, int32_t length, char *dest, int32_t capacity, i18n_uidna_info_h *info, int32_t *len);

/**
 * @brief Converts a single domain name label into its Unicode form for human-readable display.
 * @details UTF-8 version of i18n_uidna_label_to_unicode(), same behavior.
 * @since_tizen 6.0
 * @remarks @a info should be released with i18n_uidna_info_destroy().
 *
 * @param[in] uidna     A UIDNA instance
 * @param[in] label     Input domain name label
 * @param[in] length    Label length, or -1 if NUL-terminated
 * @param[in] dest      Destination string buffer
 * @param[in] capacity  Destination buffer capacity
 * @param[out] info     Output container of IDNA processing details.
 * @param[out] len      Destination string length
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_uidna_label_to_unicode_UTF8 (i18n_uidna_h uidna, const char *label, int32_t length, char *dest, int32_t capacity, i18n_uidna_info_h *info, int32_t *len);

/**
 * @brief Converts a whole domain name into its ASCII form for DNS lookup.
 * @details UTF-8 version of i18n_uidna_name_to_ascii(), same behavior.
 * @since_tizen 6.0
 * @remarks @a info should be released with i18n_uidna_info_destroy().
 *
 * @param[in] uidna     A UIDNA instance
 * @param[in] name      Input domain name
 * @param[in] length    Label length, or -1 if NUL-terminated
 * @param[in] dest      Destination string buffer
 * @param[in] capacity  Destination buffer capacity
 * @param[out] info     Output container of IDNA processing details.
 * @param[out] len      Destination string length
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_uidna_name_to_ascii_UTF8(i18n_uidna_h uidna, const char *name, int32_t length, char *dest, int32_t capacity, i18n_uidna_info_h *info, int32_t *len);

/**
 * @brief Converts a whole domain name into its Unicode form for human-readable display.
 * @details UTF-8 version of i18n_uidna_name_to_unicode(), same behavior.
 * @since_tizen 6.0
 * @remarks @a info should be released with i18n_uidna_info_destroy().
 *
 * @param[in] uidna     A UIDNA instance
 * @param[in] name      Input domain name
 * @param[in] length    Label length, or -1 if NUL-terminated
 * @param[in] dest      Destination string buffer
 * @param[in] capacity  Destination buffer capacity
 * @param[out] info     Output container of IDNA processing details.
 * @param[out] len      Destination string length
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_uidna_name_to_unicode_UTF8(i18n_uidna_h uidna, const char *name, int32_t length, char *dest, int32_t capacity, i18n_uidna_info_h *info, int32_t *len);

/**
 * @brief Gets whether transitional and nontransitional processing produce different results.
 * @since_tizen 6.0
 *
 * @param[in] info                   UIDNA info handle
 * @param[out] is_trans_different    TRUE if transitional and nontransitional processing produce different results.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uidna_info_get_is_trans_different(i18n_uidna_info_h info, i18n_ubool *is_trans_different);

/**
 * @brief Gets the bit set indicating UIDNA processing errors.
 * @since_tizen 6.0
 *
 * @param[in] info       UIDNA info handle
 * @param[out] errors    Bit set with UIDNA processing errors\n
 *                       0 if no errors, see #i18n_uidna_error_e
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uidna_info_get_errors(i18n_uidna_info_h info, uint32_t *errors);

/**
 * @brief Destroys a UIDNA info handle.
 * @since_tizen 6.0
 *
 * @param[in] info    The UIDNA info handle to be destroyed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uidna_info_destroy(i18n_uidna_info_h info);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
