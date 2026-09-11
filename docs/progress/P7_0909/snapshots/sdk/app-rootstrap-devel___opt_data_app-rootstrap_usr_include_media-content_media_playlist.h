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

#ifndef __TIZEN_CONTENT_MEDIA_PLAYLIST_H__
#define __TIZEN_CONTENT_MEDIA_PLAYLIST_H__

#include <media_content_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file
 * @brief This file contains the playlist API and functions related with handling playlists. \n
 *        Functions include operations to get the number of playlists, the number of media-info for the playlist  \n
 *        and all media files in the playlist, to clone, destroy, insert and delete playlist from DB,  \n
 *        to handle with name, ID, thumbnail, played order and media info of the playlist.
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_PLAYLIST_MODULE
 * @{
 */


/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the number of playlists for the passed @a filter from the media database.
 * @since_tizen 2.3
 *
 * @param[in] filter The handle to the media filter
 * @param[out] playlist_count The count of the media playlist
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
int media_playlist_get_playlist_count_from_db(filter_h filter, int *playlist_count) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Iterates through the media playlists with an optional @a filter from the media database.
 * @details This function gets all media playlists meeting the given filter.
 *          The callback function will be invoked for every retrieved media playlist.
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
 * @post This function invokes media_playlist_cb().
 *
 * @see media_playlist_cb()
 * @see media_content_connect()
 * @see media_filter_create()
 */
int media_playlist_foreach_playlist_from_db(filter_h filter, media_playlist_cb callback, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the number of the media info for the given playlist present in the media database.
 * @since_tizen 2.3
 *
 * @param[in] playlist_id The ID of the media playlist
 * @param[in] filter The handle to the media filter
 * @param[out] media_count The number of media items
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
int media_playlist_get_media_count_from_db(int playlist_id, filter_h filter, int *media_count) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Iterates through the media files with an optional @a filter in the given audio playlist from the media database.
 * @details This function gets all media files associated with the given media playlist and
 *          meeting desired filter option and calls @a callback for
 *          every retrieved media info. If @c NULL is passed to the @a filter, no filtering is applied.
 *
 * @since_tizen 2.3
 *
 * @param[in] playlist_id The ID of the media playlist
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
int media_playlist_foreach_media_from_db(int playlist_id, filter_h filter, playlist_member_cb callback, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Inserts a new playlist with the given name into the media database.
 * @since_tizen 2.3
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @remarks The @a playlist should be released using media_playlist_destroy().
 *
 * @param[in] name The name of the inserted playlist
 * @param[out] playlist The handle to the media playlist
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED         DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY           DB Operation busy
 * @retval #MEDIA_CONTENT_ERROR_NETWORK           Network fail
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_playlist_delete_from_db()
 */
int media_playlist_insert_to_db(const char *name, media_playlist_h *playlist) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Deletes the given playlist from the media database.
 * @since_tizen 2.3
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @param[in] playlist_id The ID of media playlist
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
 * @see media_playlist_insert_to_db()
 */
int media_playlist_delete_from_db(int playlist_id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the media playlist from the media database.
 *
 * @details This function creates a new media playlist handle from the media database by the given @a playlist_id.
 *          The media playlist will be created and will be filled with the playlist information.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a playlist should be released using media_playlist_destroy().
 *
 * @param[in] playlist_id The ID of the media playlist
 * @param[out] playlist The handle to the media playlist
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
 * @see media_playlist_destroy()
 */
int media_playlist_get_playlist_from_db(int playlist_id, media_playlist_h *playlist) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Destroys a playlist handle.
 * @details This function frees all resources related to the playlist handle. This
 *          handle no longer can be used to perform any operations. A new handle has to
 *          be created before next usage.
 *
 * @since_tizen 2.3
 *
 * @param[in] playlist The handle to the media playlist
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Get a copy of playlist handle by calling media_playlist_clone() or media_playlist_insert_to_db().
 *
 * @see media_playlist_clone()
 */
int media_playlist_destroy(media_playlist_h playlist) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Clones a playlist handle.
 * @details This function copies the media playlist handle from a source to
 *          destination. There is no media_playlist_create() function. The media_playlist_h is created internally and available through
 *          media playlist foreach function such as media_playlist_foreach_playlist_from_db().
 *          To use this handle outside of these foreach functions, use this function.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a dst should be released using media_playlist_destroy().
 *
 * @param[out] dst The destination handle to the media playlist
 * @param[in] src The source handle to the media playlist
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 *
 * @see media_playlist_destroy()
 * @see media_playlist_foreach_playlist_from_db()
 */
int media_playlist_clone(media_playlist_h *dst, media_playlist_h src) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the media playlist ID.
 * @since_tizen 2.3
 *
 * @param[in] playlist The handle to the media playlist
 * @param[out] playlist_id The ID of the media playlist
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_playlist_get_playlist_id(media_playlist_h playlist, int *playlist_id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets a name of the playlist.
 * @since_tizen 2.3
 *
 * @remarks The @a playlist_name should be released using free().
 *
 * @param[in] playlist The handle to the media playlist
 * @param[out] playlist_name The playlist name
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int media_playlist_get_name(media_playlist_h playlist, char **playlist_name) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Sets the name of the playlist.
 * @since_tizen 2.3
 *
 * @param[in] playlist The handle to the media playlist
 * @param[in] playlist_name The name of the media playlist
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 *
 * @post media_playlist_update_to_db()
 *
 */
int media_playlist_set_name(media_playlist_h playlist, const char *playlist_name) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets a thumbnail path of the playlist.
 * @since_tizen 2.3
 *
 * @remarks The @a path should be released using free().
 *
 * @param[in] playlist The handle to the media playlist
 * @param[out] path The path of the thumbnail
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int media_playlist_get_thumbnail_path(media_playlist_h playlist, char **path) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Sets the thumbnail path of the playlist.
 * @since_tizen 2.3
 *
 * @param[in] playlist The handle to the media playlist
 * @param[in] path The path of the thumbnail
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 *
 * @post media_playlist_update_to_db()
 */
int media_playlist_set_thumbnail_path(media_playlist_h playlist, const char *path) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 9.0.
 * @brief Sets the playing order in the playlist.
 * @since_tizen 2.3
 *
 * @param[in] playlist The handle to the media playlist
 * @param[in] playlist_member_id The ID of the playlist member
 * @param[in] play_order The playing order
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 *
 * @post media_playlist_update_to_db()
 */
int media_playlist_set_play_order(media_playlist_h playlist, int playlist_member_id, int play_order) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Adds a new media info to the playlist.
 * @since_tizen 2.3
 *
 * @param[in] playlist The handle to the media playlist
 * @param[in] media_id The media ID
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 *
 * @pre  This function requires opened connection to content service by media_content_connect().
 * @post media_playlist_update_to_db()
 *
 * @see media_content_connect()
 * @see media_playlist_remove_media()
 */
int media_playlist_add_media(media_playlist_h playlist, const char *media_id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Removes the playlist members related with the media from the given playlist.
 * @since_tizen 2.3
 *
 * @param[in] playlist The handle to the media playlist
 * @param[in] playlist_member_id The ID of the playlist member
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 *
 * @pre  This function requires opened connection to content service by media_content_connect().
 * @post media_playlist_update_to_db()
 *
 * @see media_content_connect()
 * @see media_playlist_add_media()
 */
int media_playlist_remove_media(media_playlist_h playlist, int playlist_member_id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the played order of the playlist.
 * @since_tizen 2.3
 *
 * @param[in] playlist The handle to the media playlist
 * @param[in] playlist_member_id The ID of the playlist member
 * @param[out] play_order The played order
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_playlist_get_play_order(media_playlist_h playlist, int playlist_member_id, int *play_order) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Updates the media playlist to the media database.
 *
 * @details The function updates the given media playlist in the media database.
 *          The function should be called after any change in the playlist, to be updated to the media database.
 *          For example, after using media_playlist_set_name() for setting the name of the playlist, the
 *          media_playlist_update_to_db() function should be called so as to update
 *          the given playlist attributes in the media database.
 *
 * @since_tizen 2.3
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @param[in] playlist The handle to the media playlist
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED         DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY           DB Operation busy
 * @retval #MEDIA_CONTENT_ERROR_NETWORK           Network fail
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_playlist_destroy()
 * @see media_playlist_add_media()
 * @see media_playlist_remove_media()
 * @see media_playlist_set_name()
 * @see media_playlist_set_play_order()
 */
int media_playlist_update_to_db(media_playlist_h playlist) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Imports the playlist from m3u playlist file.
 * @details This function reads a playlist from the m3u playlist file and insert into the db.
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @remarks The @a playlist should be released using media_playlist_destroy().
 * @remarks   %http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage. \n
 *                     %http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage. \n
 *                     This function does not support the file of extended m3u playlist.
 *
 * @param[in] playlist_name The name of the media playlist to save
 * @param[in] path The path to import the playlist file
 * @param[out] playlist The handle to the media playlist
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED         DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY           DB Operation busy
 * @retval #MEDIA_CONTENT_ERROR_NETWORK           Network fail
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_playlist_import_from_file(const char *playlist_name, const char *path, media_playlist_h *playlist) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Exports the playlist to m3u playlist file.
 * @since_tizen 2.4
 *
 * @remarks   %http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage. \n
 *                     %http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage.
 *
 * @param[in] playlist The handle to the media playlist
 * @param[in] path path The path to export the playlist
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED         DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY           DB Operation busy
 * @retval #MEDIA_CONTENT_ERROR_NETWORK           Network fail
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_playlist_export_to_file(media_playlist_h playlist, const char* path) TIZEN_DEPRECATED_API;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__TIZEN_CONTENT_MEDIA_PLAYLIST_H__*/
