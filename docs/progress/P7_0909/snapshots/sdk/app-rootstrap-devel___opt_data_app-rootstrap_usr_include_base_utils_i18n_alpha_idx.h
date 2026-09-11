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
 * @file utils_i18n_alpha_idx.h
 * @version 0.1
 * @brief utils_i18n_alpha_idx
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE Alphabetic Index
 * @brief Alphabetic Index supports the creation of a UI index appropriate for a given language.
 * @section CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE_HEADER Required Header
 *			\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE_OVERVIEW Overview
 * @details The module can generate a list of labels for use as a UI "index",
 *			that is, a list of clickable characters (or character sequences)
 *			that allow the user to see a segment (bucket) of a larger "target" list.
 *			That is, each label corresponds to a bucket in the target list, where
 *			everything in the bucket is greater than or equal to the character
 *			(according to the locale's collation). Strings can be added to the index;
 *			they will be in sorted order in the right bucket.
 *			The module also supports having buckets for strings before the first (underflow),
 *			after the last (overflow), and between scripts (inflow). For example,
 *			if the index is constructed with labels for Russian and English,
 *			Greek characters would fall into an inflow bucket between the other two scripts.
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE
 * @{
 */

/**
 * @deprecated Deprecated since 5.0. Use i18n_alpha_idx_create_from_locale_id() instead.
 * @brief Creates an alphabetic index object for the specified locale.
 * @details If the locale's data does not include index characters,
 *			a set of them will be synthesized based on the locale's
 *			exemplar characters. The locale determines the sorting order
 *			for both the index characters and the user item names appearing
 *			under each Index character.
 * @since_tizen 2.3.2
 * @remarks The created object should be released by the caller with the
 *			#i18n_alpha_idx_destroy() function.
 *
 * @param[in] language	The language of the locale
 * @param[in] country	The country of the locale
 * @param[out] index	The created alphabetic index object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_alpha_idx_create(const char *language, const char *country,
						  i18n_alpha_idx_h *index) TIZEN_DEPRECATED_API;

/**
 * @brief Destroys the alphabetic index object.
 * @since_tizen 2.3.2
 *
 * @param[in] index	 The alphabetic index to be destroyed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_destroy(i18n_alpha_idx_h index);

/**
 * @deprecated Deprecated since 5.0. Use i18n_alpha_idx_add_labels_with_locale_id() instead.
 * @brief Adds the index characters from a specified locale to the index.
 * @details The labels are added to those that are already in the index;
 *			they do not replace the existing index characters. The collation
 *			order for this index is not changed; it remains that of the locale
 *			that was originally specified when creating this Index.
 * @since_tizen 2.3.2
 *
 * @param[in] index		Label will be added to this alphabetic index
 * @param[in] language	The language of the locale
 * @param[in] country	The country of the locale
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_add_labels(i18n_alpha_idx_h index, const char *language,
							  const char *country) TIZEN_DEPRECATED_API;

/**
 * @brief Adds the record to the alphabetic index.
 * @details Each record will be associated with an index Bucket
 *			based on the record's name.	 The list of records for
 *			each bucket will be sorted based on the collation ordering
 *			of the names in the index's locale. Records with duplicate
 *			names are permitted; they will be kept in the order that
 *			they were added.
 * @since_tizen 2.3.2
 *
 * @param[in] index	 Record will be added to this alphabetic index
 * @param[in] name	 The display name for the record.
 *					 The Record will be placed in a bucket based on this name.
 * @param[in] data	 An optional pointer to user data associated with this item
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_add_record(i18n_alpha_idx_h index, const char *name,
							  const void *data);

/**
 * @brief Sets the next bucket as current bucket in the index.
 * @since_tizen 2.3.2
 *
 * @param[in] index		  The alphabetic index, which contains buckets
 * @param[out] available  A flag set to @c true if the next bucket was available,
 *						  or @c false if there were no more buckets.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_ENUM_OUT_OF_SYNC The index is modified while
 * an enumeration of its contents are in process.
 */
int i18n_alpha_idx_get_next_bucket(i18n_alpha_idx_h index, bool *available);

/**
 * @brief Sets the next record as current record in current bucket of the index.
 * @details When i18n_alpha_idx_get_next_bucket() is called, record iteration is reset
 *			to just before the first record in the new bucket.
 * @since_tizen 2.3.2
 *
 * @param[in] index		  The alphabetic index, which contains buckets with records
 * @param[out] available  A flag set to @c true if the next record was available,
 *						  or @c false if there were no more records.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_ENUM_OUT_OF_SYNC The index is modified while
 * an enumeration of its contents are in process.
 */
int i18n_alpha_idx_get_next_record(i18n_alpha_idx_h index, bool *available);

/**
 * @brief Gets the number of <name, data> records in the current bucket.
 * @details If the current bucket iteration position is before the first
 *			label or after the last, return 0.
 * @since_tizen 2.3.2
 *
 * @param[in] index			  The alphabetic index, which contains buckets with records
 * @param[out] records_count  Number of <name, data> records in the current bucket
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_get_bucket_record_count(i18n_alpha_idx_h index, int32_t *records_count);

/**
 * @brief Gets the name of the label of the current bucket in alphabetic index.
 * @details If the iteration is before the first Bucket
 *			(i18n_alpha_idx_get_next_bucket() has not been called),
 *			or after the last, return an empty string.
 * @since_tizen 2.3.2
 * @remarks The obtained @a label should be released by the caller with the free() function.
 *
 * @param[in] index	  The alphabetic index, which contains buckets
 * @param[out] label  The name of the label of the current bucket
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_alpha_idx_get_bucket_label(i18n_alpha_idx_h index, char **label);

/**
 * @brief Gets the data pointer of the current record in a current bucket in alphabetic index.
 * @details Returns NULL if:
 *			- the current iteration position is before the first item in this Bucket,
 *			  or after the last,
 *			- the given @a index parameter is invalid.
 * @since_tizen 2.3.2
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 *
 * @param[in] index	 The alphabetic index, which contains buckets with records
 *
 * @return The data pointer of the current record in a current bucket
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid parameter
 */
const void *i18n_alpha_idx_get_record_data(i18n_alpha_idx_h index);

/**
 * @brief Gets the default label used for abbreviated buckets between other index characters.
 * @details For example, consider the labels when Latin and Greek are used:
 *			X Y Z ... &\#x0391; &\#x0392; &\#x0393;
 * @since_tizen 2.3.2
 * @remarks The obtained @a label should be released by the caller with the free() function.
 *
 * @param[in] index	 The alphabetic index, which contains buckets with records
 * @param[out] label The default label used for abbreviated bucket between other index characters
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_alpha_idx_get_inflow_label(i18n_alpha_idx_h index, char **label);

/**
 * @brief Sets the default label used for abbreviated buckets between other index characters.
 * @details An inflow label will be automatically inserted if two otherwise-adjacent label characters
 *			are from different scripts, e.g. Latin and Cyrillic, and a third script,
 *			e.g. Greek, sorts between the two. The default inflow character is an ellipsis (...)
 * @since_tizen 2.3.2
 *
 * @param[in] index	 The alphabetic index, which contains buckets with records
 * @param[in] label	 The new inflow label
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_set_inflow_label(i18n_alpha_idx_h index, const char *label);

/**
 * @brief Gets the special label used for items that sort after the last normal label,
 *		  and that would not otherwise have an appropriate label.
 * @since_tizen 2.3.2
 * @remarks The obtained @a label should be released by the caller with the free() function.
 *
 * @param[in] index	  The alphabetic index, which contains buckets with records
 * @param[out] label  The overflow label
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_alpha_idx_get_overflow_label(i18n_alpha_idx_h index, char **label);

/**
 * @brief Sets the special label used for items that sort after the last normal label,
 *		  and that would not otherwise have an appropriate label.
 * @since_tizen 2.3.2
 *
 * @param[in] index	 The alphabetic index, which contains buckets with records
 * @param[in] label	 The new overflow label
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_set_overflow_label(i18n_alpha_idx_h index, const char *label);

/**
 * @brief Gets the special label used for items that sort before the first normal label,
 *		  and that would not otherwise have an appropriate label.
 * @since_tizen 2.3.2
 * @remarks The obtained @a label should be released by the caller with the free() function.
 *
 * @param[in] index	  The alphabetic index, which contains buckets with records
 * @param[out] label  The underflow label
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_alpha_idx_get_underflow_label(i18n_alpha_idx_h index, char **label);

/**
 * @brief Sets the special label used for items that sort before the first normal label,
 *		  and that would not otherwise have an appropriate label.
 * @since_tizen 2.3.2
 *
 * @param[in] index	 The alphabetic index, which contains buckets with records
 * @param[in] label	 The new underflow label
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_set_underflow_label(i18n_alpha_idx_h index, const char *label);

/**
 * @brief Gets the limit on the number of labels permitted in the index.
 * @details The number does not include over, under and inflow labels.
 * @since_tizen 2.3.2
 *
 * @param[in] index				The alphabetic index, which contains buckets with records
 * @param[out] max_label_count	The maximum number of labels
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_get_max_label_count(i18n_alpha_idx_h index, int32_t *max_label_count);

/**
 * @brief Sets a limit on the number of labels permitted in the index.
 * @details The number does not include over, under and inflow labels.
 *			Currently, if the number is exceeded, then every nth item
 *			is removed to bring the count down. A more sophisticated
 *			mechanism may be available in the future.
 * @since_tizen 2.3.2
 *
 * @param[in] index			   The alphabetic index, which contains buckets with records
 * @param[in] max_label_count  The new maximum number of labels
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_set_max_label_count(i18n_alpha_idx_h index, int32_t max_label_count);

/**
 * @brief Remove all records from the index.
 * @details The set of Buckets, which define the headings under which records are classified,
 *			is not altered.
 * @since_tizen 2.3.2
 *
 * @param[in] index	 The alphabetic index, which contains buckets with records
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_clear_records(i18n_alpha_idx_h index);

/**
 * @brief Gets the number of labels in this index.
 * @since_tizen 2.3.2
 * @remarks Note: may trigger lazy index construction.
 *
 * @param[in] index			 The alphabetic index, which contains buckets with records
 * @param[out] bucket_count	 The number of labels in this index, including any under,
 *							 over or inflow labels
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_get_bucket_count(i18n_alpha_idx_h index, int32_t *bucket_count);

/**
 * @brief Gets the total number of records in this index, that is, the number of
 *		  <name, data> pairs added.
 * @since_tizen 2.3.2
 *
 * @param[in] index			The alphabetic index, which contains buckets with records
 * @param[out] record_count The number of records in this index, that is,
 *							the total number of (name, data) items added with
 *							i18n_alpha_idx_add_record()
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_get_record_count(i18n_alpha_idx_h index, int32_t *record_count);

/**
 * @brief Given the name of a record, returns the zero-based index of the bucket
 *		  in which the item should appear.
 * @details The name need not be in the index.
 *			A Record will not be added to the index by this function.
 *			Bucket numbers are zero-based, in Bucket iteration order.
 * @since_tizen 2.3.2
 *
 * @param[in] index			 The alphabetic index, which contains buckets with records
 * @param[in] item_name		 The name whose bucket position in the index is to be determined
 * @param[out] bucket_index	 The bucket number for this name
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_get_bucket_index(i18n_alpha_idx_h index, const char *item_name, int32_t *bucket_index);

/**
 * @brief Gets the zero based index of the current bucket of this index.
 * @details Sets the variable pointed by the @a bucket_index to -1 if no iteration
 *			is in process.
 * @since_tizen 2.3.2
 *
 * @param[in] index			 The alphabetic index, which contains buckets with records
 * @param[out] bucket_index	 The index of the current Bucket
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_get_current_bucket_index(i18n_alpha_idx_h index, int32_t *bucket_index);

/**
 * @brief Gets the type of the label for the current Bucket
 *		  (selected by the iteration over Buckets).
 * @since_tizen 2.3.2
 *
 * @param[in] index	 The alphabetic index, which contains buckets with records
 * @param[out] type	 The alphabetic index label type
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_get_bucket_label_type(i18n_alpha_idx_h index,
										 i18n_alpha_idx_label_type_e *type);

/**
 * @brief Gets the name of the current record.
 * @details If the Record iteration position is before the first or after the last record,
 *			sets the string pointed by the @a record_name parameter to NULL and returns
 *			the #I18N_ERROR_INDEX_OUTOFBOUNDS error code.
 * @since_tizen 2.3.2
 * @remarks The obtained @a record_name should be released by the caller with the free() function.
 *
 * @param[in] index		   The alphabetic index, which contains buckets with records
 * @param[out] record_name The name of the current index item
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #I18N_ERROR_INDEX_OUTOFBOUNDS Trying to access the index that is out of bounds
 */
int i18n_alpha_idx_get_record_name(i18n_alpha_idx_h index, char **record_name);

/**
 * @brief Resets the bucket iteration for this index.
 * @details The next call to i18n_alpha_idx_get_next_bucket() will restart the iteration at the first label.
 * @since_tizen 2.3.2
 *
 * @param[in] index	 The alphabetic index, which contains buckets with records
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_reset_bucket_iter(i18n_alpha_idx_h index);

/**
 * @brief Resets the record iteration for this index to before the first Record in the current Bucket.
 * @details The next call to i18n_alpha_idx_get_next_record() will restart the iteration at the first label.
 * @since_tizen 2.3.2
 *
 * @param[in] index	 The alphabetic index, which contains buckets with records
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_reset_record_iter(i18n_alpha_idx_h index);


/**
 * @brief Creates an alphabetic index object for the specified locale.
 * @details If the locale's data does not include index characters,
 *			a set of them will be synthesized based on the locale's
 *			exemplar characters. The locale determines the sorting order
 *			for both the index characters and the user item names appearing
 *			under each Index character.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 * 			#i18n_alpha_idx_destroy() function.
 *
 * @param[in]  locale_id	The locale string containing language and country code
 * @param[out] index		The created alphabetic index object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_alpha_idx_create_from_locale_id(const char *locale_id, i18n_alpha_idx_h *index);

/**
 * @brief Adds the index characters from a specified locale to the index.
 * @details The labels are added to those that are already in the index;
 *			they do not replace the existing index characters. The collation
 *			order for this index is not changed; it remains that of the locale
 *			that was originally specified when creating this Index.
 * @since_tizen 5.0
 *
 * @param[in] index			Label will be added to this alphabetic index
 * @param[in] locale_id		The locale string containing language and country code
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_alpha_idx_add_labels_with_locale_id(i18n_alpha_idx_h index, const char *locale_id);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
