/*
* Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/



#ifndef __TIZEN_CONTENT_MEDIA_FILTER_H__
#define __TIZEN_CONTENT_MEDIA_FILTER_H__

#include <media_content_type.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_filter.h
 * @brief This file contains the media filter API and related operation with filters. \n
 *        The functions include: creating and destroying media filter handles that are used to get the filtered information,  \n
 *        making free all resources related to the filter handle, limiting number of items returned, setting conditions for the filter,  \n
 *        setting and getting media filter's content order and ordering keyword either descending or ascending.
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_FILTER_MODULE
 * @{
 */

/**
 * @brief Creates a media filter handle.
 * @details This function creates a media filter handle. The handle can be
 *          used to get the filtered information based on filter properties i.e. offset, count, condition for searching and order.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a filter should be released using media_filter_destroy().
 *
 * @param[out] filter The handle to the media filter
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 *
 * @see media_filter_destroy()
 */
int media_filter_create(filter_h *filter);

/**
 * @brief Destroys a media filter handle.
 * @details The function frees all resources related to the media filter handle. The filter
 *          handle no longer can be used to perform any operations. A new filter handle
 *          has to be created before the next usage.
 *
 * @since_tizen 2.3
 *
 * @param[in] filter The handle to the media filter
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_filter_create()
 */
int media_filter_destroy(filter_h filter);

/**
 * @brief Sets the media filter offset and count.
 * @details This function sets the @a offset and @a count for the given filter used to limit number of items returned.
 *          For example, if you set the @a offset as @c 10 and @a count as @c 5, then only searched data from @c 10 to @c 14 will be returned when the filter is used with foreach functions.
 *
 * @since_tizen 2.3
 *
 * @param[in] filter The handle to the media filter
 * @param[in] offset The start position of the given media filter (Starting from zero)
 * @param[in] count The number of items to be searched with respect to the offset
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_filter_create()
 * @see media_filter_destroy()
 */
int media_filter_set_offset(filter_h filter, int offset, int count);

/**
 * @brief Sets the @a condition for the given @a filter.
 * @since_tizen 2.3
 *
 * @param[in] filter The handle to the media filter
 * @param[in] condition The condition which is used WHERE clause on a query
 * @param[in] collate_type The collate type for comparing two strings
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_filter_create()
 * @see media_filter_destroy()
 */
int media_filter_set_condition(filter_h filter, const char *condition, media_content_collation_e collate_type);

/**
 * @brief Sets the media filter content @a order_type and @a order_keyword i.e. either descending or ascending.
 * @since_tizen 2.3
 *
 * @remarks If @a order_type set to #MEDIA_CONTENT_ORDER_OTHER, media_filter sorts by referring only to the value of @a order_keyword. \n
 *          Therefore, @a order_keyword must include collation type and order type. \n
 *          e.g. MEDIA_DISPLAY_NAME COLLATE NOCASE DESC, MEDIA_MODIFIED_TIME DESC
 *
 * @param[in] filter The handle to the media filter
 * @param[in] order_type The search order type
 * @param[in] order_keyword The search order keyword
 * @param[in] collate_type The collate type for comparing two strings
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_filter_create()
 * @see media_filter_destroy()
 */
int media_filter_set_order(filter_h filter, media_content_order_e order_type, const char *order_keyword, media_content_collation_e collate_type);

/**
 * @brief Gets the @a offset and @a count for the given @a filter used to limit the number of items returned.
 * @since_tizen 2.3
 *
 * @param[in] filter The handle to the media filter
 * @param[out] offset The start position of the given media filter (Starting from zero)
 * @param[out] count The number of items to be searched with respect to the offset
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_filter_create()
 * @see media_filter_destroy()
 */
int media_filter_get_offset(filter_h filter, int *offset, int *count);

/**
 * @brief Gets the @a condition for the given @a filter.
 * @since_tizen 2.3
 *
 * @remarks The @a condition should be released using free().
 *
 * @param[in] filter The handle to the media filter
 * @param[out] condition The condition which is used WHERE clause on a query
 * @param[out] collate_type The collate type for comparing two strings
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_filter_create()
 * @see media_filter_destroy()
 */
int media_filter_get_condition(filter_h filter, char **condition, media_content_collation_e *collate_type);

/**
 * @brief Gets the media filter's content @a order_type and @a order_keyword i.e. either descending or ascending.
 * @since_tizen 2.3
 *
 * @remarks The @a order_keyword should be released using free().
 *
 * @param[in] filter The handle to the media filter
 * @param[out] order_type The search order type
 * @param[out] order_keyword The search order keyword
 * @param[out] collate_type The collate type for comparing two strings
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_filter_create()
 * @see media_filter_destroy()
 */
int media_filter_get_order(filter_h filter, media_content_order_e *order_type, char **order_keyword, media_content_collation_e *collate_type);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_CONTENT_MEDIA_FILTER_H__ */
