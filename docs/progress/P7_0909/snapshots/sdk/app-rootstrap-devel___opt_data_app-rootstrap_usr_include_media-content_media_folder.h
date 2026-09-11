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


#ifndef __TIZEN_CONTENT_MEDIA_FOLDER_H__
#define __TIZEN_CONTENT_MEDIA_FOLDER_H__


#include <media_content_type.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_folder.h
 * @brief This file contains API related to all operations with media folder in DB. \n
 *        These functions include getting the number of folders and media files filtered from DB,  \n
 *        iterating through media files and folders filtered in the given folder from DB;  \n
 *        cloning and destroying the media folder, getting its name, ID, absolute path.
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_FOLDER_MODULE
 * @{
 */


/**
 * @brief Gets the count of folder for the passed @a filter from the media database.
 * @since_tizen 2.3
 *
 * @param[in] filter The handle to the media filter
 * @param[out] folder_count The count of the media folder
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
int media_folder_get_folder_count_from_db(filter_h filter, int *folder_count);

/**
 * @brief Iterates through available media folders with optional @a filter from the media database.
 * @details This function gets the media folder meeting the given @a filter.
 *          The @a callback function will be invoked for every retrieved
 *          folder. If @c NULL is passed to the @a filter, no filtering is applied.
 *
 * @since_tizen 2.3
 *
 * @remarks We do not recommend you call updating DB function in callback of foreach function.
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
 * @pre  A filter handle has to be created by calling media_filter_create().
 * @post This function invokes media_folder_cb().
 *
 * @see media_content_connect()
 * @see media_folder_cb()
 * @see media_filter_create()
 */
int media_folder_foreach_folder_from_db(filter_h filter, media_folder_cb callback, void *user_data);

/**
 * @brief Gets the count of media files for the passed @a filter in the given @a folder_id from the media database.
 * @details This function gets count of media files associated with the given folder and
 *          meeting desired filter option. If @c NULL is passed to the @a filter, no filtering is applied.
 * @since_tizen 2.3
 *
 * @param[in] folder_id The ID of the media folder
 * @param[in] filter The handle to the media filter
 * @param[out] media_count The count of media folder items
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
int media_folder_get_media_count_from_db(const char *folder_id, filter_h filter, int *media_count);

/**
 * @brief Iterates through the media files with an optional @a filter in the given @a folder_id from the media database.
 * @details This function gets all media files associated with the given folder and
 *          meeting desired filter option and calls @a callback for
 *          every retrieved media item. If @c NULL is passed to the @a filter, no filtering is applied.
 *
 * @since_tizen 2.3
 *
 * @remarks   Do not call updating DB function like media_info_update_to_db() in your callback function,
 *                    your callback function is invoked as inline function. \n
 *                    So, your callback function is in read state in SQLite. When you are in read state, sometimes you do not update DB. \n
 *                    We do not recommend you call updating DB function in callback of foreach function.
 *
 * @param[in] folder_id The ID of the media folder
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
int media_folder_foreach_media_from_db(const char *folder_id, filter_h filter, media_info_cb callback, void *user_data);

/**
 * @brief Clones the media folder.
 * @details This function copies the media folder handle from a source to
 *          destination. There is no media_folder_create() function. The media_folder_h is created internally and available through
 *          media folder foreach function such as media_folder_foreach_folder_from_db(). To use this handle outside of these foreach functions,
 *          use this function.
 *
 * @since_tizen 2.3
 *
 * @remarks The @a dst should be released using media_folder_destroy().
 *
 * @param[out] dst The destination handle to the media folder
 * @param[in] src The source handle to the media folder
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_folder_destroy()
 * @see media_folder_foreach_folder_from_db()
 */
int media_folder_clone(media_folder_h *dst, media_folder_h src);

/**
 * @brief Destroys the media folder.
 * @details The function frees all resources related to the folder handle. This handle
 *          no longer can be used to perform any operations. A new handle has to
 *          be created before the next use.
 *
 * @since_tizen 2.3
 *
 * @param[in] folder The handle to the media folder
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre A copy of the media folder handle created by calling media_folder_clone().
 *
 * @see media_folder_clone()
 */
int media_folder_destroy(media_folder_h folder);

/**
 * @brief Gets the media folder ID.
 * @since_tizen 2.3
 *
 * @remarks The @a folder_id should be released using free().
 *
 * @param[in] folder The handle to the media folder
 * @param[out] folder_id The ID of the media folder
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_folder_get_folder_id(media_folder_h folder, char **folder_id);

/**
 * @brief Gets the absolute path to the media folder.
 * @since_tizen 2.3
 *
 * @remarks The @a path should be released using free().
 *
 * @param[in] folder The handle to the media folder
 * @param[out] path The path of the media folder
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 *
 */
int media_folder_get_path(media_folder_h folder, char **path);

/**
 * @brief Gets the media folder name.
 * @since_tizen 2.3
 *
 * @remarks The @a folder_name should be released using free().
 *
 * @param[in] folder The handle to the media folder
 * @param[out] folder_name The name of the media folder
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int media_folder_get_name(media_folder_h folder, char **folder_name);

/**
 * @brief Gets the media folder from the media database.
 *
 * @details This function creates a new media folder handle from the media database by the given @a folder_id.
 *          Media folder will be created, which is filled with folder information.
 * @since_tizen 2.3
 *
 * @remarks The @a folder should be released using media_folder_destroy().
 *
 * @param[in] folder_id The ID of the media folder
 * @param[out] folder The handle to the media folder
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY DB Operation busy
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_folder_destroy()
 *
 */
int media_folder_get_folder_from_db(const char *folder_id, media_folder_h *folder);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_CONTENT_MEDIA_FOLDER_H__ */
