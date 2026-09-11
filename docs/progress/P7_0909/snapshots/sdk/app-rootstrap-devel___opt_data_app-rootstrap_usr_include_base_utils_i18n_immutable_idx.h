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
 * @file utils_i18n_immutable_idx.h
 * @version 0.1
 * @brief utils_i18n_immutable_idx
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_IMMUTABLE_IDX_MODULE Immutable Index
 * @brief Immutable, thread-safe version of Alphabetic Index.
 * @section CAPI_BASE_UTILS_I18N_IMMUTABLE_IDX_MODULE_HEADER Required Header
 *         \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_IMMUTABLE_IDX_MODULE_OVERVIEW Overview
 * @details This module provides thread-safe methods for bucketing, and random access to buckets
 *          and their properties, but does not offer adding records to the index.
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_IMMUTABLE_IDX_MODULE
 * @{
 */

/**
 * @brief Creates an immutable index object.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *            i18n_immutable_idx_destroy() function.
 *
 * @param[in]  index                The alphabetic index the thread-safe version of which is to be created
 * @param[out] immutable_index      The created immutable index object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY        Out of memory
 */
int i18n_immutable_idx_create(i18n_alpha_idx_h index, i18n_immutable_idx_h *immutable_index);

/**
 * @brief Destroys the immutable index object.
 * @since_tizen 5.0
 *
 * @param[in] index  The immutable index to be destroyed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 */
int i18n_immutable_idx_destroy(i18n_immutable_idx_h index);

/**
 * @brief Returns the number of index buckets and labels, including underflow/inflow/overflow.
 * @since_tizen 5.0
 *
 * @param[in]   index   The immutable index object
 * @param[out]  count   The number of index buckets
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 */
int i18n_immutable_idx_get_bucket_count(i18n_immutable_idx_h index, int32_t *count);

/**
 * @brief Finds the index bucket for the given name and returns the number of that bucket.
 * @details Use i18n_immutable_idx_get_bucket_label() or i18n_immutable_idx_get_bucket_label_type()
 *          to get bucket properties.
 * @since_tizen 5.0
 *
 * @param[in]  index    The immutable index object
 * @param[in]  name     The string to be sorted into an index bucket
 * @param[out] number   The bucket number for the @a name
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 */
int i18n_immutable_idx_get_bucket_index(i18n_immutable_idx_h index, char *name, int32_t *number);

/**
 * @brief Returns the label string of the index-th bucket.
 * @since_tizen 5.0
 * @remarks The returned string should be freed by the caller with free() function.
 *
 * @param[in]  index            The immutable index object
 * @param[in]  bucket_index     The bucket number
 * @param[out] label            The label string for the bucket
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_INDEX_OUTOFBOUNDS    Desired position is out of bounds
 */
int i18n_immutable_idx_get_bucket_label(i18n_immutable_idx_h index, int32_t bucket_index, char **label);

/**
 * @brief Returns the label type of the index-th bucket.
 * @since_tizen 5.0
 *
 * @param[in]  index            The immutable index object
 * @param[in]  bucket_index     The bucket number
 * @param[out] type             The alphabetic index label type
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_INDEX_OUTOFBOUNDS    Desired position is out of bounds
 */
int i18n_immutable_idx_get_bucket_label_type(i18n_immutable_idx_h index, int32_t bucket_index, i18n_alpha_idx_label_type_e *type);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
