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


#ifndef __TIZEN_CONTENT_MEDIA_INFO_H__
#define __TIZEN_CONTENT_MEDIA_INFO_H__

#include <media_content_type.h>



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_info.h
 * @brief This file contains the media info function and related functions to proceed with it. \n
 *        You can use the functions to insert, delete, count and get list of content files from media database. \n
 *        You can get properties of content file such as size, mime_type, modified_time etc. And you can set properties such as favorite etc.
 */


/**
 * @addtogroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @{
 */

/**
 * @brief  Inserts the content file into the media database.
 * @details In general, you can use this function to insert content files into the media database. \n
 *          You can use media_content_scan_file()/media_content_scan_folder() function instead of this function. \n
 *          If media information exists in the media database, this function returns information stored in the database.\n
 *          Since 5.5, if media information already exists in the media database,
 *          this function returns the updated latest information. (Media database will be updated if necessary).
 *
 * @since_tizen 2.3
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write \n
 *                   %http://tizen.org/privilege/mediastorage \n
 *                   %http://tizen.org/privilege/externalstorage
 *
 * @remarks The @a info should be released using media_info_destroy(). \n
 *                   You must add the privilege %http://tizen.org/privilege/content.write. You need to add more privileges depending on your choice of contents path. \n
 *                   If you want to access only internal storage by using this function, you should add privilege %http://tizen.org/privilege/mediastorage. \n
 *                   If you want to access only external storage by using this function, you should add privilege %http://tizen.org/privilege/externalstorage. \n
 *                   If you want to access storages of both types, you must add all privileges. \n
 *                   Since 4.0, this function does not accept symbolic links. \n
 * @remarks Since 4.0, this function is related to the following feature:\n
 *              %http://tizen.org/feature/content.scanning.others\n
 *              If this feature is not supported on the device, #MEDIA_CONTENT_TYPE_OTHERS type files are not scanned.
 *
 * @param[in] path The path of the content file to add
 * @param[out] info The handle of the inserted content file
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
 * @retval #MEDIA_CONTENT_ERROR_NOT_SUPPORTED     Not supported
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_content_scan_file()
 * @see media_content_scan_folder()
 */
int media_info_insert_to_db(const char *path, media_info_h *info);

/**
 * @brief Inserts content files into the media database, asynchronously.
 * @details This function can insert multiple content files into the media database.
 *
 * @since_tizen 2.3
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write \n
 *                   %http://tizen.org/privilege/mediastorage \n
 *                   %http://tizen.org/privilege/externalstorage
 *
 * @remarks You must add privilege %http://tizen.org/privilege/content.write. And You add more privilege depending on your choice of contents path. \n
 *                   If you want to access only internal storage by using this function, you should add privilege %http://tizen.org/privilege/mediastorage. \n
 *                   Or if you want to access only external storage by using this function, you should add privilege %http://tizen.org/privilege/externalstorage. \n
 *                   If you can access both storage, you must add all privilege. \n
 *                   Since 4.0, This function does not allow a symbolic link. \n
 * @remarks Since 4.0, this function is related to the following feature:\n
 *              %http://tizen.org/feature/content.scanning.others\n
 *              If this feature is not supported on the device, MEDIA_CONTENT_TYPE_OTHERS type file is not scanned.
 *
 * @param[in] path_array The path array of the content files to add
 * @param[in] array_length The length of the array
 * @param[in] callback The callback function to be invoked when media items inserted completely
 * @param[in] user_data The user data to be passed to the callback function
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
 * @retval #MEDIA_CONTENT_ERROR_NOT_SUPPORTED     Not supported
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_insert_completed_cb()
 */
int media_info_insert_batch_to_db(const char **path_array, unsigned int array_length, media_insert_completed_cb callback, void *user_data);

/**
 * @brief Destroys media info.
 * @details The function frees all resources related to the media info handle. This handle
 *          can no longer be used to perform any operations. New media info handle has to
 *          be created before the next usage.
 *
 * @since_tizen 2.3
 *
 * @param[in] media The handle to the media info
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Get copy of media_info handle by calling media_info_clone().
 *
 * @see media_info_clone()
 */
int media_info_destroy(media_info_h media);

/**
 * @brief Clones the media info handle.
 *
 * @details This function copies the media info handle from a source to the destination.
 *          To use this handle outside of these foreach functions, use this function.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a dst should be released using media_info_destroy().
 *
 * @param[out] dst The destination handle to the media info
 * @param[in] src The source handle to the media info
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_info_destroy()
 * @see media_album_foreach_media_from_db()
 * @see media_playlist_foreach_media_from_db()
 * @see media_group_foreach_media_from_db()
 * @see media_tag_foreach_media_from_db()
 * @see media_info_foreach_media_from_db()
 * @see media_info_insert_to_db()
 * @see media_folder_foreach_media_from_db()
 */
int media_info_clone(media_info_h *dst, media_info_h src);

/**
 * @brief Gets the count of media info for the passed @a filter from the media database.
 * @details If @c NULL is passed to the @a filter, then no filtering is applied.
 *
 * @since_tizen 2.3
 *
 *
 * @param[in] filter The handle to the media filter
 * @param[out] media_count The count of media
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
 * @see media_filter_create()
 */
int media_info_get_media_count_from_db(filter_h filter, int *media_count);

/**
 * @brief Iterates through media info from the media database.
 * @details This function gets all media info handles meeting the given @a filter.
 *          The @a callback function will be invoked for every retrieved media info.
 *          If @c NULL is passed to the @a filter, then no filtering is applied.
 *
 * @since_tizen 2.3
 *
 * @remarks Do not call updating DB function like media_info_update_to_db() in your callback function,
 *                   your callback function is invoked as inline function.
 *                   So, your callback function is in read state in SQLite. When you are in read state, sometimes you do not update DB.
 *                   We do not recommend you call updating DB function in callback of foreach function.
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
 * @post This function invokes media_info_cb().
 *
 * @see media_content_connect()
 * @see #media_info_cb
 * @see media_filter_create()
 */
int media_info_foreach_media_from_db(filter_h filter, media_info_cb callback, void *user_data);

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the count of media tags for the passed @a filter in the given @a media_id from the media database.
 * @details If @c NULL is passed to the @a filter, then no filtering is applied.
 *
 * @since_tizen 2.3
 *
 * @param[in] media_id The media ID
 * @param[in] filter The handle to the media filter
 * @param[out] tag_count The count of the media tag
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
 * @see media_filter_create()
 */
int media_info_get_tag_count_from_db(const char *media_id, filter_h filter, int *tag_count) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Iterates through the media tag in the given media info from the media database.
 * @details This function gets all the media tags associated with the given @a media_id and calls @a callback for every retrieved media tag. \n
 *               If @c NULL is passed to the @a filter, then no filtering is applied.
 *
 * @since_tizen 2.3
 *
 * @param[in] media_id The media ID
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
 * @post This function invokes media_tag_cb().
 *
 * @see media_content_connect()
 * @see #media_tag_cb
 * @see media_filter_create()
 */
int media_info_foreach_tag_from_db(const char *media_id, filter_h filter, media_tag_cb callback, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the number of bookmarks for the passed @a filter in the given media ID from the media database.
 * @details If @c NULL is passed to the @a filter, then no filtering is applied.
 *
 * @since_tizen 2.3
 *
 * @param[in] media_id The media ID
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
 * @see media_filter_create()
 */
int media_info_get_bookmark_count_from_db(const char *media_id, filter_h filter, int *bookmark_count) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Iterates through the media bookmark in the given media info from the media database.
 * @details This function gets all media bookmarks associated with the given media and calls @a callback for every retrieved media bookmark.
 *                If @c NULL is passed to the @a filter, then no filtering is applied.
 *
 * @since_tizen 2.3
 *
 * @param[in] media_id The media ID
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
 * @post This function invokes media_bookmark_cb().
 *
 * @see media_content_connect()
 * @see media_bookmark_cb()
 * @see media_filter_create()
 */
int media_info_foreach_bookmark_from_db(const char *media_id, filter_h filter, media_bookmark_cb callback, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @brief Gets the image metadata handle for a given media info.
 * @details This function returns an image metadata handle retrieved from the media info.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a image should be released using image_meta_destroy().
 *
 * @param[in] media The handle to the media info
 * @param[out] image The handle to the image metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see image_meta_destroy()
 */
int media_info_get_image(media_info_h media, image_meta_h *image);

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets a video metadata handle for a given media info.
 * @details This function returns a video metadata handle retrieved from the media info handle.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a video should be released using video_meta_destroy().
 *
 * @param[in] media The handle to the media info
 * @param[out] video The handle to the video metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see video_meta_destroy()
 */
int media_info_get_video(media_info_h media, video_meta_h *video) TIZEN_DEPRECATED_API;

/**
 * @brief Gets an audio metadata handle for a given media info.
 * @details This function returns an audio metadata handle retrieved from the media info handle.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a audio should be released using audio_meta_destroy().
 *
 * @param[in] media The handle to the media info
 * @param[out] audio The handle to the audio metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see audio_meta_destroy()
 */
int media_info_get_audio(media_info_h media, audio_meta_h *audio);

/**
 * @brief Gets a book metadata handle for a given media info.
 * @details This function returns a book metadata handle retrieved from the media info handle. \n
 *          The title and description information of the book can be obtained from @a media.
 *
 * @since_tizen 6.5
 *
 * @remarks The @a book should be released using book_meta_destroy().
 *
 * @param[in] media The handle to the media info
 * @param[out] book The handle to the book metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_info_get_title()
 * @see media_info_get_description()
 * @see book_meta_destroy()
 */
int media_info_get_book(media_info_h media, book_meta_h *book);

/**
 * @brief Gets the media ID.
 * @since_tizen 2.3
 * @remarks The @a media_id should be released using free().
 *
 * @param[in] media The handle to the media info
 * @param[out] media_id The media ID
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_get_media_id(media_info_h media, char **media_id);

/**
 * @brief Gets the full path of the content file.
 * @since_tizen 2.3
 *
 * @remarks The @a path should be released using free().
 *
 * @param[in] media The handle to the media info
 * @param[out] path The full path of the content file
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_file_path(media_info_h media, char **path);

/**
 * @brief Gets the file name including the extension of the content file.
 * @since_tizen 2.3
 *
 * @remarks The @a name should be released using free().
 *
 * @param[in] media The handle to the media info
 * @param[out] name The file name including the extension of the content file
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_get_display_name(media_info_h media, char **name);

/**
 * @brief Gets the content type of the content file.
 * @since_tizen 2.3
 *
 * @param[in] media The handle to the media info
 * @param[out] type The content type of the content file (#media_content_type_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #media_content_type_e
 */
int media_info_get_media_type(media_info_h media, media_content_type_e *type);

/**
 * @brief Gets the MIME type of the content file.
 * @since_tizen 2.3
 *
 * @remarks The @a mime_type should be released using free().
 *
 * @param[in] media The handle to the media info
 * @param[out] mime_type The MIME type of the content file
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_get_mime_type(media_info_h media, char **mime_type);

/**
 * @brief Gets the content file size.
 * @since_tizen 2.3
 *
 * @param[in] media The handle to the media info
 * @param[out] size The content file size
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_get_size(media_info_h media, unsigned long long *size);

/**
 * @brief Gets the added time of the content file.
 * @details The added time refers to the time that content file was first added to media database.
 * @since_tizen 2.3
 *
 * @param[in] media The handle to the media info
 * @param[out] added_time The added time to the media database
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_get_added_time(media_info_h media, time_t *added_time);

/**
 * @brief Gets the modified time of the content file.
 * @details The modified time refers to the last modification time provided by the file system.
 * @since_tizen 2.3
 *
 * @param[in] media The handle to the media info
 * @param[out] time The last modification time of the content file
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_get_modified_time(media_info_h media, time_t *time);

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the timeline of content file.
 * @details If the content file has the creation time information (like Content recorded date or Image creation date), that value is timeline. \n
 *               Otherwise, timeline value is the same as modified time.
 * @since_tizen 2.3
 *
 * @param[in] media The handle to the media info
 * @param[out] time The timeline of content file
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_get_timeline(media_info_h media, time_t *time) TIZEN_DEPRECATED_API;

/**
 * @brief Gets the thumbnail path of content file.
 * @since_tizen 2.3
 *
 * @remarks The @a path should be released using free(). \n
 *                   If the thumbnail extraction for the given media has not been requested yet, this function returns NULL. \n
 *                   To create a thumbnail, you should use media_info_generate_thumbnail() function. \n
 *                   This function returns an empty string if media_info_generate_thumbnail() has failed to create a thumbnail for the given media.
 *
 * @param[in] media The handle to the media info
 * @param[out] path The thumbnail path
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_get_thumbnail_path(media_info_h media, char **path);

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the description of content file.
 * @details If the value is an empty string, the method returns "Unknown". \n
 *                Since 3.0, if the media info has no description, the method returns empty string.
 * @since_tizen 2.3
 *
 * @remarks The @a description should be released using free().
 *
 * @param[in] media The handle to the media info
 * @param[out] description The description of the content file
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_get_description(media_info_h media, char **description) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the longitude of content file.
 * @since_tizen 2.3
 *
 * @param[in] media The handle to the media info
 * @param[out] longitude The longitude of the content file
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_get_longitude(media_info_h media, double *longitude) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the latitude of content file.
 * @since_tizen 2.3
 *
 * @param[in] media The handle to the media info
 * @param[out] latitude The latitude of the content file
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_latitude(media_info_h media, double* latitude) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the altitude of content file.
 * @since_tizen 2.3
 *
 * @param[in] media The handle to the media info
 * @param[out] altitude The altitude of the content file
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_get_altitude(media_info_h media, double* altitude) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the rating of content file.
 * @since_tizen 2.3
 *
 * @param[in] media The handle to the media info
 * @param[out] rating The rating of the content file
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_get_rating(media_info_h media, int *rating) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the favorite status of content file which User set.
 * @since_tizen 2.3
 *
 * @param[in] media The handle to the media info
 * @param[out] favorite @c true if content file is set as favorite,
 *                       otherwise @c false if content file is not set as favorite
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_info_set_favorite()
 */
int media_info_get_favorite(media_info_h media, bool* favorite) TIZEN_DEPRECATED_API;

/**
 * @brief Gets the title of content file.
 * @details If the content file does not have a title, this method returns the filename without the extension.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a title should be released using free().
 *
 * @param[in] media The handle to the media info
 * @param[out] title The title of the content file
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_get_title(media_info_h media, char **title);

/**
 * @deprecated Deprecated since 9.0.
 * @brief Checks whether the media is protected via DRM.
 * @since_tizen 2.3
 *
 * @param[in] media The handle to the media info
 * @param[out] is_drm @c true if media is DRM media,
 *                    otherwise @c false if media is not DRM media
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_is_drm(media_info_h media, bool *is_drm) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Checks whether the content file is 360 content.
 * @since_tizen 3.0
 *
 * @param[in] media The handle to the media info
 * @param[out] is_360 @c true if media is 360 content,
 *                    otherwise @c false if media is not 360 content
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_is_360_content(media_info_h media, bool *is_360) TIZEN_DEPRECATED_API;

/**
 * @brief Gets the media info from the media database.
 *
 * @details This function creates a new media handle from the media database by the given @a media_id.
 *          Media info will be created and filled with information.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a media should be released using media_info_destroy().
 *
 * @param[in] media_id The media ID
 * @param[out] media The handle to the media info
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY DB Operation busy
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_info_destroy()
 */
int media_info_get_media_from_db(const char *media_id, media_info_h *media);

/**
 * @brief Gets the media info from the media database using path.
 *
 * @details This function creates a new media handle filled with information from the database by the given @a media_path.
 *
 * @since_tizen 6.5
 *
 * @remarks The @a media should be released using media_info_destroy().\n
 *          If you want to access only internal storage by using this function, you should add privilege %http://tizen.org/privilege/mediastorage. \n
 *          If you want to access only external storage by using this function, you should add privilege %http://tizen.org/privilege/externalstorage. \n
 *          If you want to access storages of both types, you must add all privileges. \n
 *
 * @param[in] media_path The media path
 * @param[out] media The handle to the media info
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY DB Operation busy
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_info_destroy()
 */
int media_info_get_media_from_db_by_path(const char *media_path, media_info_h *media);

/**
 * @deprecated Deprecated since 9.0.
 * @brief Sets the favorite of media info.
 * @details This function can mark favorite of the media. If set to @c true, this function record the time of the change moment. \n
 *                So, If you use it in order parameter, you can sort the order of the time was a favorite. \n
 *                Or, if you use it in condition parameter, you can get the result of the favorite media.
 *
 * @since_tizen 2.3
 *
 * @param[in] media The handle to the media info
 * @param[in] favorite Set @c true to set the media info as favorite,
 *                     otherwise set @c false to not set the media info as favorite
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_set_favorite(media_info_h media, bool favorite) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Updates the media info to the media database.
 *
 * @details The function updates the given media info in the media database.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @remarks The function should be called after any change in media, to be updated to the media
 *          database. For example, after using media_info_set_favorite()
 *          for setting the name of the media, the media_info_update_to_db() function should be called so as to update
 *          the given media info attributes in the media database.
 *
 * @param[in] media The handle to the media info
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
 * @see media_info_set_favorite()
 */
int media_info_update_to_db(media_info_h media) TIZEN_DEPRECATED_API;

/**
 * @brief Moves the media info to the given destination path in the media database.
 * @details After moving or renaming a file in the filesystem, call this function to update the database. \n
 *               If the source path and destination path are the same, then this function does nothing.
 *               If you want to refresh media information, you should use media_content_scan_file() function.
 *
 * @since_tizen 2.3
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write \n
 *                   %http://tizen.org/privilege/mediastorage \n
 *                   %http://tizen.org/privilege/externalstorage
 *
 * @remarks You must add privilege %http://tizen.org/privilege/content.write. And You add more privilege depending on your choice of contents path. \n
 *                   If you want to access only internal storage by using this function, you should add privilege %http://tizen.org/privilege/mediastorage. \n
 *                   Or if you want to access only external storage by using this function, you should add privilege %http://tizen.org/privilege/externalstorage. \n
 *                   If you can access both storage, you should add all privilege. \n
 *                   Since 4.0, this function does not allow symbolic links. \n
 *                   This function does not support USB storage before 5.0. Since 5.0, USB storage is supported. \n
 *                   Since 5.0, the thumbnail is removed if it exists.
 *
 * @param[in] media The handle to the media info
 * @param[in] dst_path The path of destination
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter (Especially, if the request is duplicated, this error returns.)
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
 * @see media_content_scan_folder()
 * @see media_info_generate_thumbnail()
 */
int media_info_move_to_db(media_info_h media, const char* dst_path);

/**
 * @brief Generates a thumbnail file for the given media, synchronously.
 *
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @remarks %http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage. \n
 *          %http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage. \n
 *          Items in external storage are not supported, with the exception of MMC.
 *
 * @param[in] media The handle to the media info
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
 * @see media_info_get_thumbnail_path()
 */
int media_info_generate_thumbnail(media_info_h media);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_CONTENT_MEDIA_INFO_H__ */
