/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @file utils_i18n_loc_disp_names.h
 * @version 0.1
 * @brief utils_i18n_loc_disp_names
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE Locale display names
 * @brief The locale display names module returns display names of Locales and components
 *        of Locales.
 * @section CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE Required Header
 *         \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE_OVERVIEW Overview
 * @details The locale display names module returns display names of Locales and components
 *          of Locales.
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE
 * @{
 */

/**
 * @brief Creates a default locale display names object.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *            i18n_loc_disp_name_destroy() function.
 *
 * @param[in]    locale            The display locale
 * @param[out]   loc_disp_names    The created locale display names object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_loc_disp_names_create(const char *locale, i18n_loc_disp_names_h *loc_disp_names);

/**
 * @brief Creates a locale display names object for the given dialect handling.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *            i18n_loc_disp_name_destroy() function.
 *
 * @param[in]    locale           The display locale
 * @param[in]    dialect          The dialect handling
 * @param[out]   loc_disp_names   The created locale display names object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_loc_disp_names_create_from_dialect(const char *locale, i18n_udialect_handling_e dialect,
	                                        i18n_loc_disp_names_h *loc_disp_names);

/**
 * @brief Creates a locale display names object for given display contexts.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *            i18n_loc_disp_name_destroy() function.
 *
 * @param[in]    locale            The display locale
 * @param[in]    contexts          Display contexts
 * @param[in]    length            Number of items in the context list
 * @param[out]   loc_disp_names    The created locale display names object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_loc_disp_names_create_from_context(const char *locale, i18n_udisplay_context_e *contexts,
	                                        int32_t length, i18n_loc_disp_names_h *loc_disp_names);

/**
 * @brief Destroys the locale display names object.
 * @since_tizen 5.0
 *
 * @param[in]  loc_disp_names   The locale display names object to be destroyed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 */
int i18n_loc_disp_names_destroy(i18n_loc_disp_names_h loc_disp_names);

/**
 * @brief Gets the locale used by the given locale display names object to determinate
          the display names.
 * @since_tizen 5.0
 * @remarks The returned locale should be freed by the caller with free() function.
 *
 * @param[in]   loc_disp_names      The locale display names object
 * @param[out]  locale              The display locale
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 */
int i18n_loc_disp_names_get_locale(i18n_loc_disp_names_h loc_disp_names, char **locale);

/**
 * @brief Gets the dialect handling used by the given locale display names object.
 * @since_tizen 5.0
 *
 * @param[in]   loc_disp_names      The locale display names object
 * @param[out]  handling            The dialect handling
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 */
int i18n_loc_disp_names_get_dialect_handling(i18n_loc_disp_names_h loc_disp_names,
	                                         i18n_udialect_handling_e *handling);

/**
 * @brief Gets the context from the given locale display names object.
 * @since_tizen 5.0
 *
 * @param[in]   loc_disp_names     The locale display names object
 * @param[in]   type               The display context type
 * @param[out]  context            The display context for the specific type
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 */
int i18n_loc_disp_names_get_context(i18n_loc_disp_names_h loc_disp_names,
	                                i18n_udisplay_context_type_e type,
	                                i18n_udisplay_context_e *context);

/**
 * @brief Gets the display name of the provided locale.
 * @since_tizen 5.0
 * @remarks The returned name should be freed by the caller with free() function.
 *
 * @param[in]   loc_disp_names     The locale display names object
 * @param[in]   locale             The locale whose display name is to be returned
 * @param[out]  name               The display name of the provided locale
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_loc_disp_names_get_loc_disp_name(i18n_loc_disp_names_h loc_disp_names,
	                                      const char *locale, char **name);

/**
 * @brief Gets the display name of the provided language code.
 * @since_tizen 5.0
 * @remarks The returned name should be freed by the caller with free() function.
 *
 * @param[in]   loc_disp_names     The locale display names object
 * @param[in]   language           The language code
 * @param[out]  name               The display name of the provided language code
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_loc_disp_names_get_language_disp_name(i18n_loc_disp_names_h loc_disp_names,
	                                           const char *language, char **name);

/**
 * @brief Gets the display name of the script code provided as string.
 * @since_tizen 5.0
 * @remarks The returned name should be freed by the caller with free() function.
 *
 * @param[in]   loc_disp_names     The locale display names object
 * @param[in]   script             The script code string
 * @param[out]  name               The display name of the provided script code
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_loc_disp_names_get_script_disp_name(i18n_loc_disp_names_h loc_disp_names,
	                                         const char *script, char **name);

/**
 * @brief Gets the display name of the script code provided as enumeration.
 * @since_tizen 5.0
 * @remarks The returned name should be freed by the caller with free() function.
 *
 * @param[in]   loc_disp_names     The locale display names object
 * @param[in]   code               The script code number
 * @param[out]  name               The display name of the provided region code
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_loc_disp_names_get_script_disp_name_with_script_code(i18n_loc_disp_names_h loc_disp_names,
	                                                          i18n_uscript_code_e code, char **name);

/**
 * @brief Gets the display name of the provided region code.
 * @since_tizen 5.0
 * @remarks The returned name should be freed by the caller with free() function.
 *
 * @param[in]   loc_disp_names     The locale display names object
 * @param[in]   region             The region code
 * @param[out]  name               The display name of the provided region code
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_loc_disp_names_get_region_disp_name(i18n_loc_disp_names_h loc_disp_names,
	                                         const char *region, char **name);

/**
 * @brief Gets the display name of the provided variant.
 * @since_tizen 5.0
 * @remarks The returned name should be freed by the caller with free() function.
 *
 * @param[in]   loc_disp_names     The locale display names object
 * @param[in]   variant            The variant
 * @param[out]  name               The display name of the provided variant
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_loc_disp_names_get_variant_disp_name(i18n_loc_disp_names_h loc_disp_names,
	                                          const char *variant, char **name);

/**
 * @brief Gets the display name of the provided locale key.
 * @since_tizen 5.0
 * @remarks The returned name should be freed by the caller with free() function.
 *
 * @param[in]   loc_disp_names     The locale display names object
 * @param[in]   key                The locale key name
 * @param[out]  name               The display name of the provided locale key
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_loc_disp_names_get_key_disp_name(i18n_loc_disp_names_h loc_disp_names,
	                                      const char *key, char **name);

/**
 * @brief Gets the display name of the provided locale key-value.
 * @since_tizen 5.0
 * @remarks The returned name should be freed by the caller with free() function.
 *
 * @param[in]   loc_disp_names     The locale display names object
 * @param[in]   key                The locale key name
 * @param[in]   value              The locale key value
 * @param[out]  name               The display name of the provided locale key
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_loc_disp_names_get_key_value_disp_name(i18n_loc_disp_names_h loc_disp_names,
	                                            const char *key, const char *value,
	                                            char **name);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
