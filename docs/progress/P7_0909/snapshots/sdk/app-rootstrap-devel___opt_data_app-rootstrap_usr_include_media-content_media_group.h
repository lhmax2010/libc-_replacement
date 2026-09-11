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

#ifndef __TIZEN_CONTENT_MEDIA_GROUP_H__
#define __TIZEN_CONTENT_MEDIA_GROUP_H__


#include <media_content_type.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_group.h
 * @brief This file contains API related to handling different operations with album and other media data groups in DB.
 *        The following APIs are capable to get number of albums, media info in the given album from DB, \n
 *        to clone, destroy and get all albums and media files associated with the given media album from DB,  \n
 *        to get name, ID, artist, album art path from album; to get number of groups and their names, \n
 *        to get the number of media files and their content associated with the given group from DB.
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_ALBUM_MODULE
 * @{
 */

/**
 * @brief Gets the count of the album for the passed @a filter from the media database.
 * @details This function gets the count of the album associated with the given filter option. \n
 *          If @c NULL is passed to the @a filter, no filtering is applied.
 * @since_tizen 2.3
 *
 * @param[in] filter The handle to the media filter
 * @param[out] album_count The count of the media album
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
int media_album_get_album_count_from_db(filter_h filter, int *album_count);

/**
 * @brief Iterates through the media album with optional @a filter from the media database.
 * @details This function gets all media album handles meeting the given filter.
 *          The callback function will be invoked for every retrieved media album.
 *          If @c NULL is passed to the filter, no filtering is applied.
 *
 * @since_tizen 2.3
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
 * @pre  This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_album_cb().
 *
 * @see #media_album_cb
 * @see media_content_connect()
 * @see media_filter_create()
 */
int media_album_foreach_album_from_db(filter_h filter, media_album_cb callback, void *user_data);

/**
 * @brief Gets the count of the media for the passed @a filter in the given @a album_id from the media database.
 * @details This function gets the count of the media associated with the given @a album_id and
 *          meeting desired filter option. If @c NULL is passed to the @a filter, no filtering is applied.
 * @since_tizen 2.3
 *
 * @param[in] album_id The ID of the media album
 * @param[in] filter The handle to the media filter
 * @param[out] media_count The count of the media album
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
int media_album_get_media_count_from_db(int album_id, filter_h filter, int *media_count);

/**
 * @brief Iterates through the media files with an optional @a filter in the given media album from the media database.
 * @details This function gets all media files associated with the given media album and
 *          meeting desired filter option and calls @a callback for
 *          every retrieved media info. If @c NULL is passed to the @a filter, no filtering is applied.
 *
 * @since_tizen 2.3
 *
 * @param[in] album_id The ID of the media album
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
 * @pre  This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_info_cb().
 *
 * @see #media_info_cb
 * @see media_content_connect()
 * @see media_filter_create()
 */
int media_album_foreach_media_from_db(int album_id, filter_h filter, media_info_cb callback, void *user_data);

/**
 * @brief Destroys the album handle.
 * @details This function frees all resources related to the album handle. This handle
 *          can no longer be used to perform any operations. A new handle has to
 *          be created before the next use.
 *
 * @since_tizen 2.3
 *
 * @param[in] album The handle to the media album
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Get copy of album handle by calling media_album_clone().
 *
 * @see media_album_clone()
 */
int media_album_destroy(media_album_h album);

/**
 * @brief Clones a media album.
 * @details This function copies the media album handle from a source to
 *          destination. There is no media_album_create() function. The media_album_h is created internally and available through
 *          media album foreach function such as media_album_foreach_album_from_db(). To use this handle outside of these foreach functions,
 *          use this function.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a dst should be released using media_album_destroy().
 *
 * @param[out] dst The destination handle to the media album
 * @param[in] src The source handle to the media album
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 *
 * @see media_album_destroy()
 * @see media_album_foreach_album_from_db()
 */
int media_album_clone(media_album_h *dst, media_album_h src);

/**
 * @brief Gets the ID of the album.
 * @since_tizen 2.3
 *
 * @param[in] album The handle to the media album
 * @param[out] album_id The ID of the media album
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_album_foreach_album_from_db()
 */
int media_album_get_album_id(media_album_h album, int *album_id);

/**
 * @brief Gets the name of the album.
 * @since_tizen 2.3
 *
 * @remarks The @a album_name should be released using free().
 *
 * @param[in] album The handle to the media album
 * @param[out] album_name The name of the media album handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int media_album_get_name(media_album_h album, char **album_name);

/**
 * @brief Gets the name of the artist from the given album.
 * @since_tizen 2.3
 *
 * @remarks The @a artist should be released using free().
 *
 * @param[in] album The handle to the media album
 * @param[out] artist The name of the media artist
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int media_album_get_artist(media_album_h album, char **artist);

/**
 * @brief Gets the album art path from the album.
 * @since_tizen 2.3
 *
 * @remarks The @a album_art should be released using free().
 *
 * @param[in] album The handle to the media album
 * @param[out] album_art The path of the media album_art
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int media_album_get_album_art(media_album_h album, char **album_art);

/**
 * @brief Gets the media album from the media database.
 *
 * @details This function creates a new media album handle from the media database by the given @a album_id.
 *          Media album will be created and will be filled with the album information.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a album should be released using media_album_destroy().
 *
 * @param[in] album_id The ID of the media album
 * @param[out] album The handle to the media album
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
 *
 * @see media_content_connect()
 * @see media_album_destroy()
 */
int media_album_get_album_from_db(int album_id, media_album_h *album);


/**
 * @}
 */


/**
* @addtogroup CAPI_CONTENT_MEDIA_GROUP_MODULE
* @{
*/

 /**
 * @brief Gets the count of the given group for the passed @a filter from the media database.
 * @details This function gets the count of the group associated with the given group and
 *          meeting desired filter option. If @c NULL is passed to the @a filter, no filtering is applied.
 * @since_tizen 2.3
 *
 * @param[in] filter The handle to the media filter
 * @param[in] group The type of the media group
 * @param[out] group_count The count of the media group
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
int media_group_get_group_count_from_db(filter_h filter, media_group_e group, int *group_count);

 /**
 * @brief Iterates through the media group with an optional @a filter from the media database.
 * @details This function gets names of media group meeting the given filter.
 *          The callback function will be invoked for every retrieved media group.
 *          If @c NULL is passed to the filter, no filtering is applied.
 *
 * @since_tizen 2.3
 *
 * @param[in] filter The handle to the media filter
 * @param[in] group The type of the media group
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
 * @pre  This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_group_cb().
 *
 * @see #media_group_cb
 * @see media_content_connect()
 * @see media_filter_create()
 */
int media_group_foreach_group_from_db(filter_h filter, media_group_e group, media_group_cb callback, void *user_data);

 /**
 * @brief Gets the count of the media info for the given media group present in the media database.
 * @details This function gets the count of the group associated with the given @a group and
 *          meeting desired @a filter option. If @c NULL is passed to the @a filter, no filtering is applied.
 * @since_tizen 2.3
 *
 * @param[in] group_name The name of the media group
 * @param[in] group The type of the media group
 * @param[in] filter The handle to the media filter
 * @param[out] media_count The count of the media
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
 *
 * @see media_content_connect()
 */
int media_group_get_media_count_from_db(const char *group_name, media_group_e group, filter_h filter, int *media_count);

 /**
 * @brief Iterates through the media files with an optional @a filter in the given @a group from the media database.
 * @details This function gets all media files associated with the given group and
 *          meeting desired filter option and calls @a callback for
 *          every retrieved media info. If @c NULL is passed to the @a filter, no filtering is applied.
 *
 * @since_tizen 2.3
 *
 * @param[in] group_name The name of the media group
 * @param[in] group The type of the media group
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
 * @pre  This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_info_cb().
 *
 * @see media_info_cb()
 * @see media_content_connect()
 * @see media_filter_create()
 */
int media_group_foreach_media_from_db(const char *group_name, media_group_e group, filter_h filter, media_info_cb callback, void *user_data);

 /**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_CONTENT_MEDIA_GROUP_H__ */
