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



#ifndef __TIZEN_CONTENT_MEDIA_BOOKMARK_H__
#define __TIZEN_CONTENT_MEDIA_BOOKMARK_H__

#include <media_content_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_bookmark.h
 * @brief This file contains API on main functional operations with bookmarks that are related to media resources in the media database. \n
 *        Operations include: inserting a new bookmark in media to the media database, removing bookmark from database, \n
 *        getting number of bookmarks, cloning and destroying bookmark, getting bookmark`s ID, time marked parameter and thumbnail.
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_BOOKMARK_MODULE
 * @{
 */

/**
 * @deprecated Deprecated since 9.0.
 * @brief Inserts a new bookmark in media on the specified time offset to the media database.
 * @since_tizen 2.3
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @param[in] media_id The media ID
 * @param[in] time The bookmark time offset (in seconds)
 * @param[in] thumbnail_path The thumbnail path of video bookmark\ n
 *                           If the media type is audio, then thumbnail is null.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED         DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY           DB Operation busy
 * @retval #MEDIA_CONTENT_ERROR_NETWORK           Network fail
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_bookmark_delete_from_db()
 */
int media_bookmark_insert_to_db(const char *media_id, time_t time, const char *thumbnail_path) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Removes a media bookmark from the media database.
 * @since_tizen 2.3
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @param[in] bookmark_id The ID of the media bookmark
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED         DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY           DB Operation busy
 * @retval #MEDIA_CONTENT_ERROR_NETWORK           Network fail
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_bookmark_insert_to_db()
 */
int media_bookmark_delete_from_db(int bookmark_id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the number of bookmarks with an optional filter from the media database.
 * @since_tizen 2.3
 *
 * @param[in] filter The handle to the media filter
 * @param[out] bookmark_count The count of the media bookmark
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED         DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY           DB Operation busy
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 */
int media_bookmark_get_bookmark_count_from_db(filter_h filter, int *bookmark_count) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Iterates through the bookmarks with an optional filter from the media database.
 * @details This function gets all bookmarks associated with the given filter and calls @a callback for every retrieved media bookmark.
 *          If @c NULL is passed to the @a filter, then no filtering is applied.
 * @since_tizen 4.0
 *
 * @param[in] filter The handle to the media filter
 * @param[in] callback The callback function to be invoked
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED         DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY           DB Operation busy
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_bookmark_cb().
 *
 * @see media_content_connect()
 * @see media_bookmark_cb()
 * @see media_filter_create()
 */
int media_bookmark_foreach_bookmark_from_db(filter_h filter, media_bookmark_cb callback, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Clones a media bookmark.
 * @details This function copies the media bookmark handle from a source to destination. There is no media_bookmark_create() function.
 *          The media_bookmark_h is created internally and available through media bookmark foreach function such as media_info_foreach_bookmark_from_db().
 *          To use this handle outside of these foreach functions, use this function.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a dst should be released using media_bookmark_destroy().
 *
 * @param[out] dst The destination handle to the media bookmark
 * @param[in] src The source handle to the media bookmark
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 *
 * @see media_bookmark_destroy()
 * @see media_info_foreach_bookmark_from_db()
 */
int media_bookmark_clone(media_bookmark_h *dst, media_bookmark_h src) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Destroys a media bookmark.
 * @details This function frees all the resources related to the bookmark handle. This handle
 *          no longer can be used to perform any operations. A new handle has to
 *          be created before the next use.
 *
 * @since_tizen 2.3
 *
 * @param[in] bookmark The handle to the media bookmark
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Get copy of bookmark handle by calling media_bookmark_clone().
 *
 * @see media_bookmark_clone()
 */
int media_bookmark_destroy(media_bookmark_h bookmark) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the bookmark ID.
 * @since_tizen 2.3
 *
 * @param[in] bookmark The handle to the media bookmark
 * @param[out] bookmark_id The ID of the media bookmark
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_bookmark_get_bookmark_id(media_bookmark_h bookmark, int *bookmark_id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the bookmark time marked parameter.
 * @details This function returns time offset in seconds from beginning of the movie on which bookmark
 *          was placed.
 *
 * @since_tizen 2.3
 *
 * @param[in] bookmark The handle to the media bookmark
 * @param[out] marked_time The bookmark time offset (in seconds)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_bookmark_get_marked_time(media_bookmark_h bookmark, time_t *marked_time) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the media bookmark thumbnail.
 * @since_tizen 2.3
 *
 * @remarks The @a path should be released using free().
 *
 * @param[in] bookmark The handle to the media bookmark
 * @param[out] path The thumbnail path of media bookmark
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int media_bookmark_get_thumbnail_path(media_bookmark_h bookmark, char **path) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the media bookmark name which user set.
 * @since_tizen 4.0
 *
 * @remarks The @a name should be released using free().
 *
 * @param[in] bookmark The handle to the media bookmark
 * @param[out] name The name of media bookmark. If name was not set, empty string is returned.
 *                               If User set bookmark name to NULL, name will be empty string also.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int media_bookmark_get_name(media_bookmark_h bookmark, char **name) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Sets the media bookmark name.
 * @since_tizen 4.0
 *
 * @param[in] bookmark The handle to the media bookmark
 * @param[in] name The name of media bookmark. Can be NULL, empty or non-empty string.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 *
 * @post media_bookmark_update_to_db()
 */
int media_bookmark_set_name(media_bookmark_h bookmark, const char *name) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Updates bookmark information to the media database.
 * @details The function updates the given bookmark meta in the media database. \n
 *          The function should be called after any change in bookmark attributes, to be updated to the media database. \n
 *          For example, after using media_bookmark_set_name() for setting the name of the bookmark, \n
 *          the media_bookmark_update_to_db() function should be called so as to update the given bookmark attributes in the media database.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @remarks Do not call this function in callback function of foreach function like media_info_foreach_bookmark_from_db().
 * @param[in] bookmark The handle to the media bookmark
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY DB Operation busy
 * @retval #MEDIA_CONTENT_ERROR_NETWORK Network fail
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied when Application has no privilege
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_bookmark_set_name()
 */
int media_bookmark_update_to_db(media_bookmark_h bookmark) TIZEN_DEPRECATED_API;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__TIZEN_CONTENT_MEDIA_BOOKMARK_H__*/
