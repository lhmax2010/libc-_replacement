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


#ifndef __TIZEN_CONTENT_MEDIA_CONTENT_TYPE_H__
#define __TIZEN_CONTENT_MEDIA_CONTENT_TYPE_H__

#include <time.h>
#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_content_type.h
 * @brief This file contains API related to media-content enumerations for media data types, groups, orientations, \n
 *        classes of errors and definitions of media-data. \n
 *        Listed APIs are called when iterating over lists of album, group, bookmark and other media,  \n
 *        when media items are inserted completely and when notification of media DB change is subscribed.
 */

/**
* @addtogroup CAPI_MEDIA_CONTENT_MODULE
 * @{
 */

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Enumeration for the media file format.
 * @details The content type is determined based on the mime type. \n
 *          This type can also be used in the condition of the media filter.
 * @since_tizen 2.3
 * @remarks Since 4.0, #MEDIA_CONTENT_TYPE_OTHERS is related to the following feature:\n
 *          %http://tizen.org/feature/content.scanning.others\n
 *          If this feature is not supported on the device, #MEDIA_CONTENT_TYPE_OTHERS type file is not scanned.
 */
typedef enum {
	MEDIA_CONTENT_TYPE_IMAGE	= 0,	/**<The type of an image */
	MEDIA_CONTENT_TYPE_VIDEO	= 1,	/**<The type of a video */
	MEDIA_CONTENT_TYPE_SOUND	= 2,	/**<The type of sound */
	MEDIA_CONTENT_TYPE_MUSIC	= 3,	/**<The type of music */
	MEDIA_CONTENT_TYPE_OTHERS	= 4,	/**<The type of other */
	MEDIA_CONTENT_TYPE_BOOK		= 5,	/**<The type of book (Since 6.5)*/
} media_content_type_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Enumeration for media content DB update items.
 * @since_tizen 2.3
 */
typedef enum {
	MEDIA_ITEM_FILE		= 0,		/**< File type, an item updated to DB */
	MEDIA_ITEM_DIRECTORY	= 1,		/**< Directory type, an item updated to DB */
} media_content_db_update_item_type_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Enumeration for media content DB update types.
 * @since_tizen 2.3
 */
typedef enum {
	MEDIA_CONTENT_INSERT		= 0,		/**< Insert, the type of DB update */
	MEDIA_CONTENT_DELETE		= 1,		/**< Delete, The type of DB update */
	MEDIA_CONTENT_UPDATE	= 2,		/**< Update, The type of DB update */
} media_content_db_update_type_e;

/**
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @brief Enumeration for orientation types.
 * @since_tizen 2.3
 */
typedef enum {
	MEDIA_CONTENT_ORIENTATION_NOT_AVAILABLE  = 0,       /**< Not available*/
	MEDIA_CONTENT_ORIENTATION_NORMAL         = 1,       /**< Normal*/
	MEDIA_CONTENT_ORIENTATION_HFLIP          = 2,       /**< Flip horizontal*/
	MEDIA_CONTENT_ORIENTATION_ROT_180        = 3,       /**< Rotate 180 degrees*/
	MEDIA_CONTENT_ORIENTATION_VFLIP          = 4,       /**< Flip vertical*/
	MEDIA_CONTENT_ORIENTATION_TRANSPOSE      = 5,       /**< Transpose*/
	MEDIA_CONTENT_ORIENTATION_ROT_90         = 6,       /**< Rotate 90 degrees*/
	MEDIA_CONTENT_ORIENTATION_TRANSVERSE     = 7,       /**< Transverse*/
	MEDIA_CONTENT_ORIENTATION_ROT_270        = 8,       /**< Rotate 270 degrees*/
} media_content_orientation_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Enumeration for ordering.
 * @since_tizen 2.3
 */
typedef enum {
	MEDIA_CONTENT_ORDER_ASC   = 0,      /**< Ascending order*/
	MEDIA_CONTENT_ORDER_DESC  = 1,      /**< Descending order*/
	MEDIA_CONTENT_ORDER_OTHER = 2,		/**< order by order key**/
} media_content_order_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Enumeration for collations.
 * @since_tizen 2.3
 */
typedef enum {
	MEDIA_CONTENT_COLLATE_DEFAULT   = 0,        /**< Default collation BINARY */
	MEDIA_CONTENT_COLLATE_NOCASE    = 1,        /**< Collation NOCASE, not case sensitive */
	MEDIA_CONTENT_COLLATE_RTRIM     = 2,        /**< Collation RTRIM, trailing space characters are ignored */
	MEDIA_CONTENT_COLLATE_LOCALIZED = 3,        /**< Collation LOCALIZATION, NOCASE also applied */
	MEDIA_CONTENT_COLLATE_MIXED     = 4,        /**< Collation MIXED, natural sort order (Since 10.0) */
} media_content_collation_e;

/**
* @brief Error class.
* @details Class for Media Content error
* @since_tizen 2.3
*
*/
#define MEDIA_CONTENT_ERROR_CLASS				TIZEN_ERROR_MEDIA_CONTENT

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Enumeration for a media content error.
 * @since_tizen 2.3
 */
typedef enum {
	MEDIA_CONTENT_ERROR_NONE                    = TIZEN_ERROR_NONE,                    /**< Successful */
	MEDIA_CONTENT_ERROR_INVALID_PARAMETER       = TIZEN_ERROR_INVALID_PARAMETER,       /**< Invalid parameter */
	MEDIA_CONTENT_ERROR_OUT_OF_MEMORY           = TIZEN_ERROR_OUT_OF_MEMORY,           /**< Out of memory */
	MEDIA_CONTENT_ERROR_INVALID_OPERATION       = TIZEN_ERROR_INVALID_OPERATION,       /**< Invalid Operation */
	MEDIA_CONTENT_FILE_NO_SPACE_ON_DEVICE       = TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE, /**< No space left on device */
	MEDIA_CONTENT_ERROR_PERMISSION_DENIED		  = TIZEN_ERROR_PERMISSION_DENIED,		  /**< Permission denied */
	MEDIA_CONTENT_ERROR_DB_FAILED               = MEDIA_CONTENT_ERROR_CLASS | 0x01,    /**< DB operation failed */
	MEDIA_CONTENT_ERROR_DB_BUSY                 = MEDIA_CONTENT_ERROR_CLASS | 0x02,    /**< DB operation BUSY */
	MEDIA_CONTENT_ERROR_NETWORK                 = MEDIA_CONTENT_ERROR_CLASS | 0x03,    /**< Network Fail */
	MEDIA_CONTENT_ERROR_UNSUPPORTED_CONTENT     = MEDIA_CONTENT_ERROR_CLASS | 0x04,    /**< Unsupported Content */
	MEDIA_CONTENT_ERROR_NOT_SUPPORTED           = TIZEN_ERROR_NOT_SUPPORTED,           /**< Not supported */
} media_content_error_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Enumeration for a media group.
 * @since_tizen 2.3
 */
typedef enum {
	MEDIA_CONTENT_GROUP_DISPLAY_NAME = 0,    /**< @deprecated Media group ID for display name. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_TYPE,                /**< @deprecated Media group ID for a media type. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_MIME_TYPE,           /**< @deprecated Media group ID for a mime type. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_SIZE,                /**< @deprecated Media group ID for content size. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_ADDED_TIME,          /**< @deprecated Media group ID for the added time. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_MODIFIED_TIME,       /**< @deprecated Media group ID for the modified time. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_TITLE,               /**< @deprecated Media group ID for a content title. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_ARTIST,              /**< Media group ID for an artist*/
	MEDIA_CONTENT_GROUP_ALBUM_ARTIST,        /**< Media group ID for an album artist */
	MEDIA_CONTENT_GROUP_GENRE,               /**< Media group ID for a genre*/
	MEDIA_CONTENT_GROUP_COMPOSER,            /**< @deprecated Media group ID for a composer. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_YEAR,                /**< Media group ID for a year*/
	MEDIA_CONTENT_GROUP_RECORDED_DATE,       /**< @deprecated Media group ID for the recorded date. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_COPYRIGHT,           /**< @deprecated Media group ID for the copyright. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_TRACK_NUM,           /**< @deprecated Media group ID for a track number. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_DESCRIPTION,         /**< @deprecated Media group ID for a description. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_LONGITUDE,           /**< @deprecated Media group ID for the longitude. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_LATITUDE,            /**< @deprecated Media group ID for the latitude. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_ALTITUDE,            /**< @deprecated Media group ID for the altitude. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_RATING = 20,         /**< @deprecated Media group ID for a rating. Deprecated since 9.0*/
	MEDIA_CONTENT_GROUP_MAX = 29             /**< @deprecated Invalid media group ID. Deprecated since 9.0*/
} media_group_e;

/**
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @brief The structure type for the Media info handle.
 * @since_tizen 2.3
 */
typedef struct media_info_s *media_info_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_FOLDER_MODULE
 * @brief The structure type for the Media folder handle.
 * @since_tizen 2.3
 */
typedef struct media_folder_s *media_folder_h;

/**
 * @deprecated Deprecated since 9.0.
 * @ingroup CAPI_CONTENT_MEDIA_PLAYLIST_MODULE
 * @brief The structure type for the Media playlist handle.
 * @since_tizen 2.3
 */
typedef struct media_playlist_s *media_playlist_h;

/**
 * @deprecated Deprecated since 9.0.
 * @ingroup CAPI_CONTENT_MEDIA_TAG_MODULE
 * @brief The structure type for the Media tag handle.
 * @since_tizen 2.3
 */
typedef struct media_tag_s *media_tag_h;

/**
 * @deprecated Deprecated since 9.0.
 * @ingroup CAPI_CONTENT_MEDIA_BOOKMARK_MODULE
 * @brief The structure type for the Media bookmark handle.
 * @since_tizen 2.3
 */
typedef struct media_bookmark_s *media_bookmark_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_ALBUM_MODULE
 * @brief The structure type for the Media album handle.
 * @since_tizen 2.3
 */
typedef struct media_album_s *media_album_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_IMAGE_META_MODULE
 * @brief The structure type for the Image metadata handle.
 * @since_tizen 2.3
 */
typedef struct image_meta_s *image_meta_h;

/**
 * @deprecated Deprecated since 9.0.
 * @ingroup CAPI_CONTENT_MEDIA_VIDEO_META_MODULE
 * @brief The structure type for the Video metadata handle.
 * @since_tizen 2.3
 */
typedef struct video_meta_s *video_meta_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_AUDIO_META_MODULE
 * @brief The structure type for the Audio metadata handle.
 * @since_tizen 2.3
 */
typedef struct audio_meta_s *audio_meta_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_BOOK_META_MODULE
 * @brief The structure type for the Book metadata handle.
 * @since_tizen 6.5
 */
typedef struct book_meta_s *book_meta_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_FILTER_MODULE
 * @brief The structure type for the Media filter handle.
 * @since_tizen 2.3
 */
typedef struct filter_s *filter_h;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief The structure type for the Media content noti handle.
 * @since_tizen 3.0
 */
typedef void *media_content_noti_h;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Called when the media scanning is finished.
 * @details The following error codes can be received: \n
 *             #MEDIA_CONTENT_ERROR_NONE : Success \n
 *             #MEDIA_CONTENT_ERROR_INVALID_PARAMETER : Invalid parameter \n
 *             #MEDIA_CONTENT_ERROR_INVALID_OPERATION : Invalid operation \n
 *             #MEDIA_CONTENT_ERROR_PERMISSION_DENIED : Permission denied \n
 *             #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY : Out of memory \n
 *             #MEDIA_CONTENT_ERROR_DB_FAILED : DB Operation failed \n
 *             #MEDIA_CONTENT_ERROR_DB_BUSY : DB Operation busy \n
 *             #MEDIA_CONTENT_ERROR_NETWORK : Network fail \n
 * @since_tizen 2.3
 * @remarks The callback is called in a separate thread(not in the main loop).
 *
 * @param[in] error The error code
 * @param[in] user_data The user data passed from the foreach function
 *
 * @pre media_content_scan_folder().
 * @see media_content_scan_folder()
 */
typedef void (*media_scan_completed_cb)(media_content_error_e error, void * user_data);

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Called when the notification of the media DB change is subscribed.
 * @details The following error codes can be received: \n
 *             #MEDIA_CONTENT_ERROR_NONE : Success \n
 * @since_tizen 2.3
 *
 * @remarks The callback is called in a separate thread(not in the main loop).
 *
 * @param[in] error The error code
 * @param[in] pid The PID which publishes notification
 * @param[in] update_item The update item of notification
 * @param[in] update_type The update type of notification
 * @param[in] media_type The type of the media content (#media_content_type_e)
 * @param[in] id The ID of media or directory, which is updated
 * @param[in] path The path of the media or directory
 * @param[in] mime_type The MIME of the media info
 * @param[in] user_data The user data passed from the foreach function
 *
 * @pre media_content_add_db_updated_cb().
 * @see media_content_add_db_updated_cb()
 */
typedef void (*media_content_db_update_cb)(
				media_content_error_e error,
				int pid,
				media_content_db_update_item_type_e update_item,
				media_content_db_update_type_e update_type,
				media_content_type_e media_type,
				char *id,
				char *path,
				char *mime_type,
				void *user_data);


/**
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @brief Called for every available media info.
 * @details Iterates over a list of media info.
 * @since_tizen 2.3
 *
 * @remarks To use the @a media outside this function, copy the handle with media_info_clone() function. \n
 *                  The callback is called in the main loop.
 *
 * @param[in] media The handle to the media info
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_info_foreach_media_from_db(), media_folder_foreach_media_from_db() will invoke this function.
 *
 * @see media_info_clone()
 * @see media_album_foreach_media_from_db()
 * @see media_info_foreach_media_from_db()
 * @see media_folder_foreach_media_from_db()
 */
typedef bool (*media_info_cb)(media_info_h media, void *user_data);


/**
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @brief Called when media items are inserted completely.
 * @details The following error codes can be received: \n
 *             #MEDIA_CONTENT_ERROR_NONE : Success \n
 *             #MEDIA_CONTENT_ERROR_INVALID_PARAMETER : Invalid parameter \n
 *             #MEDIA_CONTENT_ERROR_INVALID_OPERATION : Invalid operation \n
 *             #MEDIA_CONTENT_ERROR_PERMISSION_DENIED : Permission denied \n
 *             #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY : Out of memory \n
 *             #MEDIA_CONTENT_ERROR_DB_FAILED : DB Operation failed \n
 *             #MEDIA_CONTENT_ERROR_DB_BUSY : DB Operation busy \n
 *             #MEDIA_CONTENT_ERROR_NETWORK : Network fail \n
 *             #MEDIA_CONTENT_ERROR_NOT_SUPPORTED : Not supported \n
 * @since_tizen 2.3
 *
 * @remarks The callback is called in a separate thread(not in the main loop).
 *
 * @param[in] error The error code
 * @param[in] user_data The user data passed from the foreach function
 *
 * @pre media_info_insert_batch_to_db()
 * @see media_info_insert_batch_to_db()
 */
typedef void (*media_insert_completed_cb)(media_content_error_e error, void * user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_FOLDER_MODULE
 * @brief Called for every available media folder.
 * @details Iterates over a list of folders.
 *
 * @since_tizen 2.3
 *
 * @remarks To use the @a folder outside this function, copy the handle with the media_folder_clone() function. \n
 *                  The callback is called in the main loop.
 *
 * @param[in] folder The handle to the media folder
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_folder_foreach_folder_from_db() will invoke this function.
 * @see media_folder_clone()
 * @see media_folder_foreach_folder_from_db()
 */
typedef bool (*media_folder_cb)(media_folder_h folder, void *user_data);

/**
 * @deprecated Deprecated since 9.0.
 * @ingroup CAPI_CONTENT_MEDIA_PLAYLIST_MODULE
 * @brief Called for every playlist in the obtained list of playlists.
 * @details Iterates over a playlist list.
 *
 * @since_tizen 2.3
 *
 * @remarks To use the @a playlist outside this function, copy the handle with the media_playlist_clone() function. \n
 *                  The callback is called in the main loop.
 *
 * @param[in] playlist The handle to the media playlist
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_playlist_foreach_playlist_from_db() will invoke this function.
 *
 * @see media_playlist_clone()
 * @see media_playlist_foreach_playlist_from_db()
 */
typedef bool (*media_playlist_cb)(media_playlist_h playlist, void *user_data);

/**
 * @deprecated Deprecated since 9.0.
 * @ingroup CAPI_CONTENT_MEDIA_PLAYLIST_MODULE
 * @brief Called for every media info with playlist member ID in the obtained list of media info.
 * @details Iterates over playlist members.
 *
 * @since_tizen 2.3
 *
 * @remarks To use the @a media outside this function, copy the handle with the media_info_clone() function. \n
 *                  The callback is called in the main loop.
 *
 * @param[in] playlist_member_id The ID of the playlist member
 * @param[in] media The handle to the media info
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_playlist_foreach_media_from_db() will invoke this function.
 *
 * @see media_info_clone()
 * @see media_playlist_foreach_media_from_db()
 */
typedef bool (*playlist_member_cb)(int playlist_member_id, media_info_h media, void *user_data);

/**
 * @deprecated Deprecated since 9.0.
 * @ingroup CAPI_CONTENT_MEDIA_TAG_MODULE
 * @brief Called for every tag in the obtained list of tags.
 * @details Iterates over a list of tags.
 *
 * @since_tizen 2.3
 *
 * @remarks To use the @a tag outside this function, copy the handle with the media_tag_clone() function. \n
 *                  The callback is called in the main loop.
 *
 * @param[in] tag The handle to the media tag
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_tag_foreach_tag_from_db(), media_info_foreach_tag_from_db() will invoke this function.
 *
 * @see media_tag_clone()
 * @see media_tag_foreach_tag_from_db()
 * @see media_info_foreach_tag_from_db()
 */
typedef bool (*media_tag_cb)(media_tag_h tag, void *user_data);

/**
 * @deprecated Deprecated since 9.0.
 * @ingroup CAPI_CONTENT_MEDIA_BOOKMARK_MODULE
 * @brief Called for every bookmark in the obtained list of bookmarks.
 * @details Iterates over a bookmark list.
 *
 * @since_tizen 2.3
 *
 * @remarks To use the @a bookmark outside this function, copy the handle with the media_bookmark_clone() function. \n
 *                  The callback is called in the main loop.
 *
 * @param[in] bookmark The handle to the media bookmark
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_info_foreach_bookmark_from_db() will invoke this function.
 * @see media_info_foreach_bookmark_from_db()
 */
typedef bool (*media_bookmark_cb)(media_bookmark_h bookmark, void *user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_ALBUM_MODULE
 * @brief Called for every album in the obtained list of groups.
 * @details Iterates over an album list.
 *
 * @since_tizen 2.3
 *
 * @remarks To use the @a album outside this function, copy the handle with the media_album_clone() function. \n
 *                  The callback is called in the main loop.
 *
 * @param[in] album The handle to the media album
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_album_foreach_album_from_db() will invoke this function.
 *
 * @see media_album_clone()
 * @see media_album_foreach_album_from_db()
 */
typedef bool (*media_album_cb)(media_album_h album, void *user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_GROUP_MODULE
 * @brief Called for every group in the obtained list of groups.
 * @details Iterates over a media group list.
 *
 * @since_tizen 2.3
 *
 * @remarks You should not free @a group_name returned by this function. \n
 *                  The callback is called in the main loop.
 *
 * @param[in] group_name The name of the media group
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_group_foreach_group_from_db() will invoke this function.
 * @see media_group_foreach_group_from_db()
 */
typedef bool (*media_group_cb)(const char *group_name, void *user_data);

/**
 * @}
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_FILTER_MODULE
 * @{
 */

 /**
 * @brief Media ID.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_ID "MEDIA_ID"

/**
 * @brief Media full path.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_PATH "MEDIA_PATH"

/**
 * @brief The file name including the extension.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_DISPLAY_NAME "MEDIA_DISPLAY_NAME"

/**
 * @brief Media type.
 * @details There are 5 media types:\n
 *               0-image, 1-video, 2-sound, 3-music, 4-other\n
 *               You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_TYPE "MEDIA_TYPE"

/**
 * @brief Media MIME type.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_MIME_TYPE "MEDIA_MIME_TYPE"


/**
 * @brief Media file size.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_SIZE "MEDIA_SIZE"

/**
 * @brief The time that content file was first added to media database.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_ADDED_TIME "MEDIA_ADDED_TIME"

/**
 * @brief The last modification time provided by the file system.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_MODIFIED_TIME "MEDIA_MODIFIED_TIME"

/**
 * @deprecated Deprecated since 9.0.
 * @brief  Media created time.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_TIMELINE "MEDIA_TIMELINE"

/**
 * @brief Media thumbnail path.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_THUMBNAIL_PATH "MEDIA_THUMBNAIL_PATH"

/**
 * @brief Media title get from tag or file name.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_TITLE "MEDIA_TITLE"

/**
 * @brief Media album name.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_ALBUM "MEDIA_ALBUM"

/**
 * @brief Media artist.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_ARTIST "MEDIA_ARTIST"

/**
 * @brief Media album artist.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_ALBUM_ARTIST "MEDIA_ALBUM_ARTIST"

/**
 * @brief Media genre.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_GENRE "MEDIA_GENRE"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Media composer.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_COMPOSER "MEDIA_COMPOSER"

/**
 * @brief Media year.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_YEAR "MEDIA_YEAR"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Media recorded date.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_RECORDED_DATE "MEDIA_RECORDED_DATE"

/**
 * @brief Media track number.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_TRACK_NUM "MEDIA_TRACK_NUM"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Media duration.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_DURATION "MEDIA_DURATION"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Media longitude.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_LONGITUDE "MEDIA_LONGITUDE"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Media latitude.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_LATITUDE "MEDIA_LATITUDE"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Media altitude.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_ALTITUDE "MEDIA_ALTITUDE"

/**
 * @brief Media width.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_WIDTH "MEDIA_WIDTH"

/**
 * @brief Media height.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_HEIGHT "MEDIA_HEIGHT"

/**
 * @brief Media datetaken.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_DATETAKEN "MEDIA_DATETAKEN"

/**
 * @deprecated Deprecated since 9.0.
 * @brief The time to register favourite.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_FAVOURITE "MEDIA_FAVOURITE"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Is DRM.
 * @details There are 2 types:\n
 *               0-not drm, 1-drm\n
 *               You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.3
 *
 */
#define MEDIA_IS_DRM "MEDIA_IS_DRM"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Virtual Reality content.
 * @details You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 3.0
 *
 */
#define MEDIA_360 "MEDIA_360"

 /**
 * @brief Folder ID.
 * @details You can use above define to set the condition of folder filter and order keyword.
 * @since_tizen 2.3
 */
#define FOLDER_ID "FOLDER_ID"

/**
 * @brief Folder full path.
 * @details You can use above define to set the condition of folder filter and order keyword.
 * @since_tizen 2.3
 */
#define FOLDER_PATH "FOLDER_PATH"

/**
 * @brief Folder base name.
 * @details You can use above define to set the condition of folder filter and order keyword.
 * @since_tizen 2.3
 */
#define FOLDER_NAME "FOLDER_NAME"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Playlist name.
 * @details You can use above define to set the condition of playlist filter and order keyword.
 * @since_tizen 2.3
 */
#define PLAYLIST_NAME "PLAYLIST_NAME"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Playlist id.
 * @details You can use above define to set the condition of playlist filter and order keyword.
 * @since_tizen 4.0
 */
#define MEDIA_PLAYLIST_ID "PLAYLIST_ID"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Playlist member order.
 * @details You can use above define to set the condition of playlist filter and order keyword.
 * @since_tizen 2.3
 */
#define PLAYLIST_MEMBER_ORDER "PLAYLIST_MEMBER_ORDER"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Media count in playlist.
 * @details You can use above define to set the condition of playlist filter and order keyword.
 * @since_tizen 2.3
 */
#define PLAYLIST_MEDIA_COUNT "PLAYLIST_MEDIA_COUNT"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Tag name.
 * @details You can use above define to set the condition of tag filter and order keyword.
 * @since_tizen 2.3
 */
#define TAG_NAME "TAG_NAME"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Tag id.
 * @details You can use above define to set the condition of tag filter and order keyword.
 * @since_tizen 4.0
 */
#define MEDIA_TAG_ID "TAG_ID"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Media count in tag.
 * @details You can use above define to set the condition of tag filter and order keyword.
 * @since_tizen 2.3
 */
#define TAG_MEDIA_COUNT "TAG_MEDIA_COUNT"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Bookmark marked time.
 * @details You can use above define to set the condition of bookmark filter and order keyword.
 * @since_tizen 2.3
 */
#define BOOKMARK_MARKED_TIME "BOOKMARK_MARKED_TIME"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Bookmark id.
 * @details You can use above define to set the condition of bookmark filter and order keyword.
 * @since_tizen 4.0
 */
#define MEDIA_BOOKMARK_ID "BOOKMARK_ID"

/**
 * @deprecated Deprecated since 9.0.
 * @brief Bookmark name.
 * @details You can use above define to set the condition of bookmark filter and order keyword.
 * @since_tizen 4.0
 */
#define BOOKMARK_NAME "BOOKMARK_NAME"

/**
 * @}
 */


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*__TIZEN_CONTENT_MEDIA_CONTENT_TYPE_H__*/
