/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_UIX_STICKER_CONSUMER_H__
#define __TIZEN_UIX_STICKER_CONSUMER_H__

#include <sticker_error.h>
#include <sticker_data.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file sticker_consumer.h
 * @brief This file contains sticker consumer's APIs.
 */

/**
 * @addtogroup CAPI_UIX_STICKER_CONSUMER_MODULE
 * @{
 */

/**
 * @brief Enumeration for event type.
 *
 * @since_tizen 5.5
 */
typedef enum {
    STICKER_CONSUMER_EVENT_TYPE_INSERT, /**< A sticker data has been added */
    STICKER_CONSUMER_EVENT_TYPE_DELETE, /**< A sticker data has been removed */
    STICKER_CONSUMER_EVENT_TYPE_UPDATE, /**< A sticker data has been updated */
} sticker_consumer_event_type_e;

/**
 * @brief The sticker consumer handle.
 * @since_tizen 5.5
 */
typedef struct sticker_consumer_s *sticker_consumer_h;

/**
 * @brief Called to retrieve all sticker data in the sticker database.
 * @details The sticker_consumer_data_foreach_all(), sticker_consumer_data_foreach_by_keyword(), sticker_consumer_data_foreach_by_group(),
 *          sticker_consumer_data_foreach_by_type() must be called to invoke this callback function, synchronously.
 * @since_tizen 5.5
 * @remarks @a data_handle should not be freed and can be used only in the callback.
 *          If you want to use it outside of the callback, you need to use a clone which can be obtained sticker_data_clone().
 * @param[in] data_handle The sticker data handle
 * @param[in] user_data The user data passed from the foreach function
 * @pre sticker_consumer_data_foreach_all() will invoke this callback.
 * @pre sticker_consumer_data_foreach_by_keyword() will invoke this callback.
 * @pre sticker_consumer_data_foreach_by_group() will invoke this callback.
 * @pre sticker_consumer_data_foreach_by_type() will invoke this callback.
 * @see sticker_consumer_data_foreach_all()
 * @see sticker_consumer_data_foreach_by_keyword()
 * @see sticker_consumer_data_foreach_by_group()
 * @see sticker_consumer_data_foreach_by_type()
 */
typedef void (*sticker_consumer_data_foreach_cb)(sticker_data_h data_handle, void *user_data);

/**
 * @brief Called to retrieve all group names in the sticker database.
 * @details The sticker_consumer_group_list_foreach_all() must be called to invoke this callback function, synchronously.
 * @since_tizen 5.5
 * @remarks @a group should not be freed and can be used only in the callback.
 * @param[in] group The group name of the sticker
 * @param[in] user_data The user data passed from the foreach function
 * @pre sticker_consumer_group_list_foreach_all() will invoke this callback.
 * @see sticker_consumer_group_list_foreach_all()
 */
typedef void (*sticker_consumer_group_list_foreach_cb)(const char *group, void *user_data);

/**
 * @brief Called to retrieve all keywords in the sticker database.
 * @details The sticker_consumer_keyword_list_foreach_all() must be called to invoke this callback function, synchronously.
 * @since_tizen 5.5
 * @remarks @a keyword should not be freed and can be used only in the callback.
 * @param[in] keyword The keyword of the sticker
 * @param[in] user_data The user data passed from the foreach function
 * @pre sticker_consumer_keyword_list_foreach_all() will invoke this callback.
 * @see sticker_consumer_keyword_list_foreach_all()
 */
typedef void (*sticker_consumer_keyword_list_foreach_cb)(const char *keyword, void *user_data);

/**
 * @brief Called when the stickers are inserted, deleted, or updated.
 * @since_tizen 5.5
 * @remarks @a data_handle should not be freed and can be used only in the callback.
 *          If you want to use it outside of the callback, you need to use a clone which can be obtained sticker_data_clone().
 *          When the @a event_type is #STICKER_CONSUMER_EVENT_TYPE_DELETE, sticker data can be obtained using @a data_handle,
 *          but the files have been already deleted.
 * @param[in] event_type The event type
 * @param[in] data_handle The sticker data handle
 * @param[in] user_data The user data passed from sticker_consumer_set_event_cb()
 * @pre The callback can be registered using sticker_consumer_set_event_cb()
 * @see sticker_consumer_set_event_cb()
 * @see sticker_consumer_unset_event_cb()
 */
typedef void (*sticker_consumer_event_cb)(sticker_consumer_event_type_e event_type, sticker_data_h data_handle, void *user_data);

/**
 * @brief Called to retrieve group images in the sticker database.
 * @details The sticker_consumer_group_image_list_foreach_all() must be called to invoke this callback function, synchronously.
 * @since_tizen 6.5
 * @remarks @a group and @a uri should not be freed and can be used only in the callback.
 * @param[in] group The group name of the stickers
 * @param[in] type The URI type of the sticker group image URI
 * @param[in] uri The URI of the sticker group image
 * @param[in] user_data The user data passed from the foreach function
 * @pre sticker_consumer_group_image_list_foreach_all() will invoke this callback.
 * @see sticker_consumer_group_image_list_foreach_all()
 */
typedef void (*sticker_consumer_group_image_list_foreach_cb)(const char *group, sticker_data_uri_type_e type, const char *uri, void *user_data);

/**
 * @brief Creates a sticker consumer handle.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediastorage
 * @remarks If the function succeeds, @a consumer_handle must be released with sticker_consumer_destroy().
 * @param[out] consumer_handle The sticker consumer handle
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @see sticker_consumer_destroy()
 */
int sticker_consumer_create(sticker_consumer_h *consumer_handle);

/**
 * @brief Destroys a sticker consumer handle.
 * @since_tizen 5.5
 * @param[in] consumer_handle The sticker consumer handle
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @see sticker_consumer_create()
 */
int sticker_consumer_destroy(sticker_consumer_h consumer_handle);

/**
 * @brief Retrieves all sticker data in the sticker database.
 * @details If you set the @a offset as @c 10 and @a count as @c 10, then only retrieved data from @c 10 to @c 19 will be invoked.
 * @since_tizen 5.5
 * @remarks It is not an error if @a result is smaller than @a count.
 * @param[in] consumer_handle The sticker consumer handle
 * @param[in] offset The start position (Starting from zero)
 * @param[in] count The number of stickers to be retrieved with respect to the offset
 * @param[out] result The number of stickers retrieved (zero indicates that no data was found)
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @post This function invokes sticker_consumer_data_foreach_cb() repeatedly for getting data.
 * @see sticker_consumer_data_foreach_cb()
 */
int sticker_consumer_data_foreach_all(sticker_consumer_h consumer_handle, int offset, int count, int *result, sticker_consumer_data_foreach_cb callback, void *user_data);

/**
 * @brief Retrieves all sticker data in the sticker database using keyword.
 * @details If you set the @a offset as @c 10 and @a count as @c 10, then only retrieved data from @c 10 to @c 19 will be invoked.
 * @since_tizen 5.5
 * @remarks It is not an error if @a result is smaller than @a count.
 * @param[in] consumer_handle The sticker consumer handle
 * @param[in] offset The start position (Starting from zero)
 * @param[in] count The number of stickers to be retrieved with respect to the offset
 * @param[out] result The number of stickers retrieved (zero indicates that no data was found)
 * @param[in] keyword The keyword of the sticker for getting sticker data
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @post This function invokes sticker_consumer_data_foreach_cb() repeatedly for getting data.
 * @see sticker_consumer_data_foreach_cb()
 */
int sticker_consumer_data_foreach_by_keyword(sticker_consumer_h consumer_handle, int offset, int count, int *result, const char *keyword, sticker_consumer_data_foreach_cb callback, void *user_data);

/**
 * @brief Retrieves all sticker data in the sticker database using group name.
 * @details If you set the @a offset as @c 10 and @a count as @c 10, then only retrieved data from @c 10 to @c 19 will be invoked.
 * @since_tizen 5.5
 * @remarks It is not an error if @a result is smaller than @a count.
 * @param[in] consumer_handle The sticker consumer handle
 * @param[in] offset The start position (Starting from zero)
 * @param[in] count The number of stickers to be retrieved with respect to the offset
 * @param[out] result The number of stickers retrieved (zero indicates that no data was found)
 * @param[in] group The group name of the sticker for getting sticker data
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @post This function invokes sticker_consumer_data_foreach_cb() repeatedly for getting data.
 * @see sticker_consumer_data_foreach_cb()
 */
int sticker_consumer_data_foreach_by_group(sticker_consumer_h consumer_handle, int offset, int count, int *result, const char *group, sticker_consumer_data_foreach_cb callback, void *user_data);

/**
 * @brief Retrieves all sticker data in the sticker database using URI type.
 * @details If you set the @a offset as @c 10 and @a count as @c 10, then only retrieved data from @c 10 to @c 19 will be invoked.
 * @since_tizen 5.5
 * @remarks It is not an error if @a result is smaller than @a count.
 * @param[in] consumer_handle The sticker consumer handle
 * @param[in] offset The start position (Starting from zero)
 * @param[in] count The number of stickers to be retrieved with respect to the offset
 * @param[out] result The number of stickers retrieved (zero indicates that no data was found)
 * @param[in] type The URI type of the sticker for getting sticker data
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @post This function invokes sticker_consumer_data_foreach_cb() repeatedly for getting data.
 * @see sticker_consumer_data_foreach_cb()
 */
int sticker_consumer_data_foreach_by_type(sticker_consumer_h consumer_handle, int offset, int count, int *result, sticker_data_uri_type_e type, sticker_consumer_data_foreach_cb callback, void *user_data);

/**
 * @brief Retrieves all group name in the sticker database.
 * @since_tizen 5.5
 * @param[in] consumer_handle The sticker consumer handle
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @post This function invokes sticker_consumer_group_list_foreach_cb() repeatedly for getting data.
 * @see sticker_consumer_group_list_foreach_cb()
 */
int sticker_consumer_group_list_foreach_all(sticker_consumer_h consumer_handle, sticker_consumer_group_list_foreach_cb callback, void *user_data);

/**
 * @brief Retrieves all keyword in the sticker database.
 * @since_tizen 5.5
 * @param[in] consumer_handle The sticker consumer handle
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @post This function invokes sticker_consumer_keyword_list_foreach_cb() repeatedly for getting data.
 * @see sticker_consumer_keyword_list_foreach_cb()
 */
int sticker_consumer_keyword_list_foreach_all(sticker_consumer_h consumer_handle, sticker_consumer_keyword_list_foreach_cb callback, void *user_data);

/**
 * @brief Retrieves all sticker data in the sticker database using display type.
 * @details If you set the @a offset as @c 10 and @a count as @c 10, then only retrieved data from @c 10 to @c 19 will be invoked.
 * @since_tizen 5.5
 * @remarks It is not an error if @a result is smaller than @a count.
 * @param[in] consumer_handle The sticker consumer handle
 * @param[in] offset The start position (Starting from zero)
 * @param[in] count The number of stickers to be retrieved with respect to the offset
 * @param[out] result The number of stickers retrieved (zero indicates that no data was found)
 * @param[in] type The display type of the sticker for getting sticker data
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @post This function invokes sticker_consumer_data_foreach_cb() repeatedly for getting data.
 * @see sticker_consumer_data_foreach_cb()
 */
int sticker_consumer_data_foreach_by_display_type(sticker_consumer_h consumer_handle, int offset, int count, int *result, sticker_data_display_type_e type, sticker_consumer_data_foreach_cb callback, void *user_data);

/**
 * @brief Retrieves all group names assigned to stickers with a matching display @a type.
 * @since_tizen 5.5
 * @param[in] consumer_handle The sticker consumer handle
 * @param[in] type The display type of the sticker for getting sticker data
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @post This function invokes sticker_consumer_group_list_foreach_cb() repeatedly for getting data.
 * @see sticker_consumer_group_list_foreach_cb()
 */
int sticker_consumer_group_list_foreach_by_display_type(sticker_consumer_h consumer_handle, sticker_data_display_type_e type, sticker_consumer_group_list_foreach_cb callback, void *user_data);

/**
 * @brief Adds entry to recently used stickers list.
 * @since_tizen 5.5
 * @param[in] consumer_handle The sticker consumer handle
 * @param[in] data_handle The sticker data handle
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @see sticker_consumer_get_recent_data_list()
 */
int sticker_consumer_add_recent_data(sticker_consumer_h consumer_handle, sticker_data_h data_handle);

/**
 * @brief Gets recently used stickers list.
 * @details The most recently used stickers are delivered in order.
 * @since_tizen 5.5
 * @remarks It is not an error if @a result is smaller than @a count.
 * @param[in] consumer_handle The sticker consumer handle
 * @param[in] count The number of stickers that you want to receive.
 *            If -1, the number of stickers is not restricted
 * @param[out] result The number of stickers received (zero indicates that no data was found)
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @post This function invokes sticker_consumer_data_foreach_cb() repeatedly for getting data.
 * @see sticker_consumer_data_foreach_cb()
 * @see sticker_consumer_add_recent_data()
 */
int sticker_consumer_get_recent_data_list(sticker_consumer_h consumer_handle, int count, int *result, sticker_consumer_data_foreach_cb callback, void *user_data);

/**
 * @brief Registers a callback function to be invoked when the stickers are inserted, deleted, or updated.
 * @since_tizen 5.5
 * @param[in] consumer_handle The sticker consumer handle
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @see sticker_consumer_event_cb()
 * @see sticker_consumer_unset_event_cb()
 */
int sticker_consumer_set_event_cb(sticker_consumer_h consumer_handle, sticker_consumer_event_cb callback, void *user_data);

/**
 * @brief Unregisters a callback function.
 * @since_tizen 5.5
 * @param[in] consumer_handle The sticker consumer handle
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @see sticker_consumer_event_cb()
 * @see sticker_consumer_set_event_cb()
 */
int sticker_consumer_unset_event_cb(sticker_consumer_h consumer_handle);

/**
 * @brief Retrieves images of all sticker groups in the database.
 * @since_tizen 6.5
 * @param[in] consumer_handle The sticker consumer handle
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @post This function invokes sticker_consumer_group_image_list_foreach_cb() repeatedly for getting data.
 * @see sticker_consumer_group_image_list_foreach_cb()
 */
int sticker_consumer_group_image_list_foreach_all(sticker_consumer_h consumer_handle, sticker_consumer_group_image_list_foreach_cb callback, void *user_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_UIX_STICKER_CONSUMER_H__ */
