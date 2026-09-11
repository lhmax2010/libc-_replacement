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



#ifndef __TIZEN_CONTENT_MEDIA_CONTENT_INTERNAL_H__
#define __TIZEN_CONTENT_MEDIA_CONTENT_INTERNAL_H__

#include <media_content_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @internal
 * @file media_content_internal.h
 * @brief This file contains API on main functional operations with storage that are related to media resources in the media database. \n
 *        Operations include: inserting a new storage in media to the media database, removing storage from database, \n
 */

/*Bookmark for C#*/
int media_bookmark_create(const char *media_id, time_t time, media_bookmark_h *bookmark);
int media_bookmark_set_thumbnail_path(media_bookmark_h bookmark, const char *path);
int media_bookmark_insert_to_db_v2(media_bookmark_h bookmark);

/*Playlist for C#*/
int media_playlist_create(media_playlist_h *playlist);
int media_playlist_get_play_order_v2(int playlist_id, int playlist_member_id, int *play_order);
int media_playlist_insert_to_db_v2(media_playlist_h playlist);
int media_playlist_update_to_db_v2(int playlist_id, media_playlist_h playlist);

/*Tag for C#*/
int media_tag_create(media_tag_h *tag);
int media_tag_insert_to_db_v2(media_tag_h tag);
int media_tag_update_to_db_v2(int tag_id, media_tag_h tag);

/*Filter for C#*/
int media_filter_set_condition_v2(filter_h filter, const char *condition);
int media_filter_get_condition_v2(filter_h filter, char **condition);
int media_filter_set_order_v2(filter_h filter, const char *order);
int media_filter_get_order_v2(filter_h filter, char **order);

/* Only for system session process */
int media_content_connect_with_uid(uid_t uid);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__TIZEN_CONTENT_MEDIA_CONTENT_INTERNAL_H__*/
