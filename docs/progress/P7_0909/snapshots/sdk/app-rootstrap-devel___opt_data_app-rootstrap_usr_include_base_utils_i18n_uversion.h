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
 * @file utils_i18n_uversion.h
 * @version 0.1
 * @brief utils_i18n_uversion
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UVERSION_MODULE Uversion
 * @brief Uversion module provides API for accessing ICU version numbers.
 *
 * @section CAPI_BASE_UTILS_I18N_UVERSION_MODULE_HEADER Required Header
 *  \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UVERSION_MODULE_OVERVIEW Overview
 * @details Uversion module provides API for accessing ICU version numbers.
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UVERSION_MODULE
 * @{
 */

/**
 * @brief Gets the ICU release version.
 * @details The version array stores the version information for ICU. For example, release
 *          "1.3.31.2" is then represented as 0x01031F02.
 * @since_tizen 4.0
 *
 * @param[out]   version_array    An array which will contain the version
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uversion_get_version(i18n_uversion_info version_array);

/**
 * @brief Parses a string with dotted-decimal version information and fills in an #i18n_uversion_info
 *        array with the result.
 * @since_tizen 4.0
 *
 * @param[in]     version_string    A string with dotted-decimal version information, with up to
 *                                  #I18N_UVERSION_MAX_VERSION_LENGTH non-negative number fields
 *                                  with values of up to 255 each
 * @param[out]    version_array     An array which will contain the version
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uversion_from_string(const char *version_string, i18n_uversion_info version_array);

/**
 * @brief Parses a Unicode string with dotted-decimal version information and fills in
 *        an #i18n_uversion_info array with the result.
 * @since_tizen 4.0
 *
 * @param[in]     version_string    A Unicode string with dotted-decimal version information,
 *                                  with up to #I18N_UVERSION_MAX_VERSION_LENGTH non-negative
 *                                  number fields with values of up to 255 each
 * @param[out]    version_array     An array which will contain the version
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uversion_from_ustring(const i18n_uchar *version_string, i18n_uversion_info version_array);

/**
 * @brief Writes a string with dotted-decimal version information according to the input
 *        #i18n_uversion_info object.
 * @since_tizen 4.0
 *
 * @param[in]     version_array     The version information to be written as a string.
 * @param[out]    version_string    A string buffer that will be filled in with a string
 *                                  corresponding to the numeric version information in
 *                                  @a version_array. The buffer size must be at least
 *                                  #I18N_UVERSION_MAX_VERSION_STRING_LENGTH.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_uversion_to_string(const i18n_uversion_info version_array, char *version_string);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
