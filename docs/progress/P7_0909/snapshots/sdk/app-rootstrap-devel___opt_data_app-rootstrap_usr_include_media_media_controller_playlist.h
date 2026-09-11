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

#ifndef __TIZEN_MULTIMEDIA_MEDIA_CONTROLLER_PLAYLIST_H__
#define __TIZEN_MULTIMEDIA_MEDIA_CONTROLLER_PLAYLIST_H__

#include <media_controller_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_controller_playlist.h
 * @brief This file contains the media controller playlist API and functions related with handling media control.
 */


/**
 * @addtogroup CAPI_MEDIA_CONTROLLER_PLAYLIST_MODULE
 * @{
 */

/**
 * @brief Called for every playlist.
 *
 * @since_tizen 4.0
 *
 * @remarks The @a playlist should not be released by the application. \n
 *              To use the @a playlist outside this function, copy the handle with the mc_playlist_clone() function. \n
 *               The callback is called in the main loop.
 *
 * @param[in] playlist        The handle of the media controller playlist
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre mc_playlist_foreach_playlist() will invoke this function.
 *
 * @see mc_playlist_clone()
 * @see mc_playlist_foreach_playlist()
 * @see mc_playlist_get_name()
 * @see mc_playlist_foreach_item()
 */
typedef bool (*mc_playlist_cb)(mc_playlist_h playlist, void *user_data);

/**
 * @brief Called for every playlist item in the playlist.
 *
 * @since_tizen 4.0
 *
 * @remarks The @a index and @a metadata should not be released by the application. \n
 *               The @a index can be used only in the callback. To use outside, make a copy.
 *               And @a metadata also can be used only in the callback. To use outside, make a copy with the mc_metadata_clone() function. \n
 *               The callback is called in the main loop.
 *
 * @param[in] index The ID of the playlist member
 * @param[in] metadata The handle to metadata of the playlist item
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre mc_playlist_foreach_item() will invoke this function.
 *
 * @see mc_playlist_foreach_item()
 * @see mc_metadata_clone()
 * @see mc_metadata_get()
 */
typedef bool (*mc_playlist_item_cb)(const char *index, mc_metadata_h metadata, void *user_data);

/**
 * @brief Gets the name of the playlist.
 * @since_tizen 4.0
 *
 * @remarks The @a playlist_name should be released using free().
 *
 * @param[in] playlist         The handle of the media controller playlist
 * @param[out] playlist_name      The name of the playlist
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre The playlist handle should be received by calling mc_playlist_foreach_playlist(), mc_playlist_get_playlist() or mc_playlist_updated_cb().
 *
 * @see mc_playlist_foreach_playlist()
 * @see mc_playlist_get_playlist()
 * @see mc_playlist_updated_cb()
 * @see mc_client_set_playlist_updated_cb()
 * @see mc_playlist_destroy()
 */
int mc_playlist_get_name(mc_playlist_h playlist, char **playlist_name);

/**
 * @brief Gets the number of the media item in a playlist.
 * @since_tizen 5.5
 *
 * @param[in] playlist        The handle of the media controller playlist
 * @param[out] item_count      The number of media items
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre The playlist handle should be received by calling mc_playlist_foreach_playlist(), mc_playlist_get_playlist() or mc_playlist_updated_cb().
 *
 * @see mc_playlist_foreach_playlist()
 * @see mc_playlist_get_playlist()
 * @see mc_playlist_updated_cb()
 * @see mc_client_set_playlist_updated_cb()
 * @see mc_playlist_destroy()
 */
int mc_playlist_get_item_count(mc_playlist_h playlist, int *item_count);

/**
 * @brief Iterates through media items in a playlist.
 * @details This function iterates through all items in the playlist.
 *          The callback function will be invoked for every retrieved playlist item.
 *          If there are no items on the playlist, the callback will not be invoked.
 * @since_tizen 4.0
 *
 * @param[in] playlist        The handle of the media controller playlist
 * @param[in] callback       The callback function to be invoked
 * @param[in] user_data     The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre The playlist handle should be received by calling mc_playlist_foreach_playlist(), mc_playlist_get_playlist() or mc_playlist_updated_cb().
 * @post This function invokes mc_playlist_item_cb().
 *
 * @see mc_playlist_foreach_playlist()
 * @see mc_playlist_get_playlist()
 * @see mc_playlist_updated_cb()
 * @see mc_client_set_playlist_updated_cb()
 * @see mc_playlist_item_cb()
 * @see mc_playlist_destroy()
 */
int mc_playlist_foreach_item(mc_playlist_h playlist, mc_playlist_item_cb callback, void *user_data);

/**
 * @brief Gets the number of playlists for the given @a app_id.
 * @details The media controller server can have several playlists. You can get a count of playlists only for the activated media controller server.
 *            If @a app_id is not the ID of an activated media controller server, this function returns #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server \n
 *               %http://tizen.org/privilege/mediacontroller.client \n
 *
 * @remarks You must add privilege %http://tizen.org/privilege/mediacontroller.server, if your application is a media controller server.
 *              You must add privilege %http://tizen.org/privilege/mediacontroller.client, if your application is a media controller client.
 *
 * @param[in] app_id                The app_id of the media controller server
 * @param[out] playlist_count      The number of playlists that the media controller server has
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see mc_client_foreach_server()
 */
int mc_playlist_get_playlist_count(const char *app_id, int *playlist_count);

/**
 * @brief Iterates over playlists of the media controller server.
 * @details This function iterates through all playlists of the given @a app_id.
 *          The media controller server can have several playlists. You can get playlists only for the activated media controller server.
 *          If @a app_id is not an ID of an activated media controller server, this function will return #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER.
 *          The callback function will be invoked for every retrieved playlist.
 *          If there are no playlists, the callback will not be invoked.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server \n
 *               %http://tizen.org/privilege/mediacontroller.client \n
 *
 * @remarks You must add privilege %http://tizen.org/privilege/mediacontroller.server, if your application is a media controller server.
 *              You must add privilege %http://tizen.org/privilege/mediacontroller.client, if your application is a media controller client.
 *
 * @param[in] app_id         The app_id of the media controller server
 * @param[in] callback       The callback function to be invoked
 * @param[in] user_data     The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @post This function invokes mc_playlist_cb().
 *
 * @see mc_playlist_cb()
 */
int mc_playlist_foreach_playlist(const char *app_id, mc_playlist_cb callback, void *user_data);

/**
 * @brief Gets the playlist handle.
 * @details This function creates a new playlist handle for the given @a app_id and @a playlist_name.
 *          You can get the playlist only for the activated media controller server.
 *          If @a app_id is not an ID of an activated media controller server, or if @a playlist_name is invalid, \n
 *          this function will return #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server \n
 *               %http://tizen.org/privilege/mediacontroller.client \n
 *
 * @remarks You must add privilege %http://tizen.org/privilege/mediacontroller.server, if your application is a media controller server.
 *              You must add privilege %http://tizen.org/privilege/mediacontroller.client, if your application is a media controller client.
 *              The @a playlist should be released using mc_playlist_destroy().
 *
 * @param[in] app_id         The app_id of the media controller server
 * @param[in] playlist_name The name of the playlist
 * @param[out] playlist       The handle of the media controller playlist
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mc_playlist_get_name()
 * @see mc_playlist_get_item_count()
 * @see mc_playlist_foreach_item()
 * @see mc_playlist_destroy()
 */
int mc_playlist_get_playlist(const char *app_id, const char *playlist_name, mc_playlist_h *playlist);

/**
 * @brief Clones a media controller playlist handle.
 * @details This function copies the media controller playlist handle from a source to
 *          destination. The #mc_playlist_h is created internally and available through media controller playlist functions.
 *
 * @since_tizen 4.0
 *
 * @remarks The @a dst should be released using mc_playlist_destroy().
 *
 * @param[in] src The source handle to the media controller playlist
 * @param[out] dst The destination handle to the media controller playlist
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mc_playlist_destroy()
 */
int mc_playlist_clone(mc_playlist_h src, mc_playlist_h *dst);

/**
 * @brief Destroys a media controller playlist handle.
 * @details This function frees all resources related to the media controller playlist handle. This
 *          handle no longer can be used to perform any operations. A new handle has to
 *          be created before next usage.
 *
 * @since_tizen 4.0
 *
 * @param[in] playlist        The handle of the media controller playlist
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mc_playlist_clone()
 */
int mc_playlist_destroy(mc_playlist_h playlist);


/**
* @}
*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MULTIMEDIA_MEDIA_CONTROLLER_PLAYLIST_H__ */
