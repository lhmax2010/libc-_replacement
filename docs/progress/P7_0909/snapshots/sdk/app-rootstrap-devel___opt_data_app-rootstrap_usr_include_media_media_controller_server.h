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

#ifndef __TIZEN_MULTIMEDIA_MEDIA_CONTROLLER_SERVER_H__
#define __TIZEN_MULTIMEDIA_MEDIA_CONTROLLER_SERVER_H__

#include <media_controller_type.h>
#include <media_controller_playlist.h>
#include <media_controller_metadata.h>
#include <bundle.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_controller_server.h
 * @brief This file contains the media controller server API and functions related with handling media controller server. \n
 *        Functions include operations to send the latest status of the server to clients.
 *        It also includes functions that can process commands received from clients.
 */

/**
 * @addtogroup CAPI_MEDIA_CONTROLLER_SERVER_MODULE
 * @{
 */

/**
 * @brief Called when the server receives the playback action command from the client.
 * @since_tizen 4.0
 *
 * @remarks The callback is called in the dbus event loop.\n
 *          If @a request_id is not null, the server should send the reply to the media controller client with the @a request_id by using mc_server_send_cmd_reply(). \n
 *          The @a client_name and @a request_id should not be released by the application. \n
 *          The @a client_name and @a request_id can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] client_name    The app_id of the media controller client
 * @param[in] request_id    The request_id of the media controller client
 * @param[in] action    The received playback action
 * @param[in] user_data        The user data passed from the mc_server_set_playback_action_cmd_received_cb() function
 *
 * @pre mc_server_set_playback_action_cmd_received_cb()
 *
 * @see mc_server_set_playback_action_cmd_received_cb()
 * @see mc_server_send_cmd_reply()
 */
typedef void (*mc_server_playback_action_cmd_received_cb)(const char* client_name, const char *request_id, mc_playback_action_e action, void *user_data);

/**
 * @brief Called when the server receives the playback position from the client.
 * @since_tizen 4.0
 *
 * @remarks The callback is called in the dbus event loop.\n
 *          If @a request_id is not null, the server should send the reply to the media controller client with the @a request_id by using mc_server_send_cmd_reply().\n
 *          The @a client_name and @a request_id should not be released by the application. \n
 *          The @a client_name and @a request_id can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] client_name    The app_id of the media controller client
 * @param[in] request_id    The request_id of the media controller client
 * @param[in] position    The received playback position (milliseconds)
 * @param[in] user_data        The user data passed from the mc_server_set_playback_position_cmd_received_cb() function
 *
 * @pre mc_server_set_playback_position_cmd_received_cb()
 *
 * @see mc_server_set_playback_position_cmd_received_cb()
 * @see mc_server_send_cmd_reply()
 */
typedef void (*mc_server_playback_position_cmd_received_cb)(const char* client_name, const char *request_id, unsigned long long position, void *user_data);

/**
 * @brief Called when the server receives the shuffle mode from the client.
 * @since_tizen 4.0
 *
 * @remarks The callback is called in the dbus event loop.\n
 *          If @a request_id is not null, the server should send the reply to the media controller client with the @a request_id by using mc_server_send_cmd_reply().\n
 *          The @a client_name and @a request_id should not be released by the application. \n
 *          The @a client_name and @a request_id can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] client_name    The app_id of the media controller client
 * @param[in] request_id    The request_id of the media controller client
 * @param[in] shuffle_mode    The received shuffle_mode
 * @param[in] user_data        The user data passed from the mc_server_set_shuffle_mode_cmd_received_cb() function
 *
 * @pre mc_server_set_shuffle_mode_cmd_received_cb()
 *
 * @see mc_server_set_shuffle_mode_cmd_received_cb()
 * @see mc_server_send_cmd_reply()
 */
typedef void (*mc_server_shuffle_mode_cmd_received_cb)(const char* client_name, const char *request_id, mc_shuffle_mode_e shuffle_mode, void *user_data);

/**
 * @brief Called when the server receives the repeat mode from the client.
 * @since_tizen 4.0
 *
 * @remarks The callback is called in the dbus event loop.\n
 *          If @a request_id is not null, the server should send the reply to the media controller client with the @a request_id by using mc_server_send_cmd_reply().\n
 *          The @a client_name and @a request_id should not be released by the application. \n
 *          The @a client_name and @a request_id can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] client_name    The app_id of the media controller client
 * @param[in] request_id    The request_id of the media controller client
 * @param[in] repeat_mode    The received repeat mode
 * @param[in] user_data        The user data passed from the mc_server_set_repeat_mode_cmd_received_cb() function
 *
 * @pre mc_server_set_repeat_mode_cmd_received_cb()
 *
 * @see mc_server_set_repeat_mode_cmd_received_cb()
 * @see mc_server_send_cmd_reply()
 */
typedef void (*mc_server_repeat_mode_cmd_received_cb)(const char* client_name, const char *request_id, mc_repeat_mode_e repeat_mode, void *user_data);

/**
 * @brief Called when the Server receives playlist command from the client.
 * @since_tizen 4.0
 *
 * @remarks The callback is called in the dbus event loop. \n
 *          If @a request_id is not null, the server should send the reply to the media controller client with the @a request_id by using mc_server_send_cmd_reply().\n
 *          The @a client_name, @a request_id, @a playlist_name and @a index should not be released by the application. \n
 *          The @a client_name, @a request_id, @a playlist_name and @a index can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] client_name    The app_id of the media controller client
 * @param[in] request_id    The request_id of the media controller client
 * @param[in] playlist_name    The name of the server playlist
 * @param[in] index    The index of the media in playlist
 * @param[in] action    The received playback action
 * @param[in] position    The received playback position (milliseconds)
 * @param[in] user_data    The user data passed from the mc_server_set_playlist_cmd_received_cb() function
 *
 * @pre mc_server_set_playlist_cmd_received_cb()
 *
 * @see mc_server_set_playlist_cmd_received_cb()
 * @see mc_server_send_cmd_reply()
 */
typedef void (*mc_server_playlist_cmd_received_cb)(const char* client_name, const char *request_id, const char *playlist_name, const char *index, mc_playback_action_e action, unsigned long long position, void *user_data);

/**
 * @brief Called when the Server receives custom command from the client.
 * @since_tizen 4.0
 *
 * @remarks The callback is called in the dbus event loop. @a data should be released using bundle_free().\n
 *          If @a request_id is not null, the server should send the reply to the media controller client with the @a request_id by using mc_server_send_cmd_reply().\n
 *          The @a client_name, @a request_id and @a command should not be released by the application. \n
 *          The @a client_name, @a request_id and @a command can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] client_name    The app_id of the media controller client
 * @param[in] request_id     The id of the command request
 * @param[in] command    The received command
 * @param[in] data    The extra data
 * @param[in] user_data        The user data passed from the mc_server_set_custom_cmd_received_cb() function
 *
 * @pre mc_server_set_custom_cmd_received_cb()
 *
 * @see mc_server_set_custom_cmd_received_cb()
 * @see mc_server_unset_custom_cmd_received_cb()
 * @see mc_server_send_cmd_reply()
 */
typedef void (*mc_server_custom_cmd_received_cb)(const char* client_name, const char *request_id, const char *command, bundle *data, void *user_data);

/**
 * @brief Called when requesting the list of created clients.
 * @since_tizen 4.0
 *
 * @remarks The callback is called in the main loop. \n
 *          The @a client_name should not be released by the application. \n
 *          The @a client_name can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] client_name The app_id of the created media controller client
 * @param[in] user_data        The user data passed from the mc_client_foreach_server() function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre mc_server_foreach_client()
 *
 * @see mc_server_foreach_client()
 */
typedef bool (*mc_activated_client_cb)(const char *client_name, void *user_data);

/**
 * @brief Called when the result of the event from the client is received.
 * @since_tizen 4.0
 *
 * @remarks The callback is called in the dbus event loop. @a data should be released using bundle_free(). \n
 *          The @a client_name and @a request_id should not be released by the application. \n
 *          The @a client_name and @a request_id can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] client_name    The app_id of the media controller client
 * @param[in] request_id     The id of the event request
 * @param[in] result_code    The result code of the event, @a mc_result_code_e since 6.0
 * @param[in] data The extra data
 * @param[in] user_data        The user data passed from the mc_server_set_event_reply_received_cb() function
 *
 * @pre mc_server_send_custom_event()
 * @pre mc_server_set_event_reply_received_cb()
 *
 * @see mc_server_send_custom_event()
 * @see mc_server_set_event_reply_received_cb()
 * @see mc_server_unset_event_reply_received_cb()
 */
typedef void (*mc_server_event_reply_received_cb)(const char *client_name, const char *request_id, int result_code, bundle *data, void *user_data);

/**
 * @brief Called when the Server receives search command from the client.
 * @since_tizen 5.0
 *
 * @remarks The callback is called in the dbus event loop.\n
 *          If @a request_id is not null, the server should send the reply to the media controller client with the @a request_id by using mc_server_send_cmd_reply().\n
 *          The @a client_name, @a request_id and @a search should not be released by the application. \n
 *          The @a client_name and @a request_id can be used only in the callback. To use outside, make a copy.\n
 *          @a search also can be used only in the callback. To use outside, make a copy using mc_search_clone().
 *
 * @param[in] client_name    The app_id of the media controller client
 * @param[in] request_id     The id of the command request
 * @param[in] search    The handle to search
 * @param[in] user_data        The user data passed from the mc_server_set_search_cmd_received_cb() function
 *
 * @pre mc_server_set_search_cmd_received_cb()
 * @see mc_server_set_search_cmd_received_cb()
 * @see mc_server_unset_search_cmd_received_cb()
 * @see mc_server_send_cmd_reply()
 * @see mc_search_foreach_condition()
 * @see mc_search_clone()
 */
typedef void (*mc_server_search_cmd_received_cb)(const char *client_name, const char *request_id, mc_search_h search, void *user_data);

/**
 * @brief Called when the server receives the subtitle and 360 mode command from the client.
 * @since_tizen 5.5
 *
 * @remarks The callback is called in the dbus event loop.\n
 *          If @a request_id is not null, the server should send the reply to the media controller client with the @a request_id by using mc_server_send_cmd_reply().\n
 *          The @a client_name and @a request_id should not be released by the application. \n
 *          The @a client_name and @a request_id can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] client_name    The app_id of the media controller client
 * @param[in] request_id    The request_id of the media controller client
 * @param[in] enabled    The received command
 * @param[in] user_data        The user data passed from the callback function
 *
 * @pre mc_server_set_subtitles_cmd_received_cb() or mc_server_set_360_mode_cmd_received_cb()
 *
 * @see mc_server_set_subtitles_cmd_received_cb()
 * @see mc_server_set_360_mode_cmd_received_cb()
 * @see mc_server_send_cmd_reply()
 */
typedef void (*mc_server_enable_cmd_received_cb)(const char* client_name, const char *request_id, bool enabled, void *user_data);

/**
 * @brief Called when the server receives the display mode command from the client.
 * @since_tizen 5.5
 *
 * @remarks The callback is called in the dbus event loop.\n
 *          If @a request_id is not null, the server should send the reply to the media controller client with the @a request_id by using mc_server_send_cmd_reply().\n
 *          The @a client_name and @a request_id should not be released by the application. \n
 *          The @a client_name and @a request_id can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] client_name    The app_id of the media controller client
 * @param[in] request_id    The request_id of the media controller client
 * @param[in] mode    The received display mode
 * @param[in] user_data        The user data passed from the mc_server_set_display_mode_cmd_received_cb() function
 *
 * @pre mc_server_set_display_mode_cmd_received_cb()
 *
 * @see mc_server_set_display_mode_cmd_received_cb()
 * @see mc_server_send_cmd_reply()
 */
typedef void (*mc_server_display_mode_cmd_received_cb)(const char* client_name, const char *request_id, mc_display_mode_e mode, void *user_data);

/**
 * @brief Called when the server receives the display rotation command from the client.
 * @since_tizen 5.5
 *
 * @remarks The callback is called in the dbus event loop.\n
 *          If @a request_id is not null, the server should send the reply to the media controller client with the @a request_id by using mc_server_send_cmd_reply().\n
 *          The @a client_name and @a request_id should not be released by the application. \n
 *          The @a client_name and @a request_id can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] client_name    The app_id of the media controller client
 * @param[in] request_id    The request_id of the media controller client
 * @param[in] rotation    The received display rotation
 * @param[in] user_data        The user data passed from the mc_server_set_display_mode_cmd_received_cb() function
 *
 * @pre mc_server_set_display_rotation_cmd_received_cb()
 *
 * @see mc_server_set_display_rotation_cmd_received_cb()
 * @see mc_server_send_cmd_reply()
 */
typedef void (*mc_server_display_rotation_cmd_received_cb)(const char* client_name, const char *request_id, mc_display_rotation_e rotation, void *user_data);

/**
 * @brief Creates a media controller server.
 * @details If this function is called, the server information with #MC_SERVER_STATE_ACTIVATE will be sent to media controller clients who set callback by using mc_client_set_server_updated_cb().
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks The @a server should be released using mc_server_destroy(). You can create only one server handle for each process.
 *
 * @param[out] server The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mc_server_destroy()
 * @see mc_client_set_server_updated_cb()
 */
int mc_server_create(mc_server_h *server);

/**
 * @brief Sets the playback state to update the latest state info.
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server The handle to media controller server
 * @param[in] state The state to set
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @post Apply the updated playback information by calling mc_server_update_playback_info().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_update_playback_info()
 */
int mc_server_set_playback_state(mc_server_h server, mc_playback_states_e state);

/**
 * @brief Sets the playback position to update the latest playback info.
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server The handle to media controller server
 * @param[in] position The position to set in milliseconds
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @post Apply the updated playback information by calling mc_server_update_playback_info().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_update_playback_info()
 */
int mc_server_set_playback_position(mc_server_h server, unsigned long long position);

/**
 * @brief Sets the playlist name and index of the current playing media in the playlist to update the latest playback info.
 * @details If a media controller server has a playlist, the server can register and share it with media controller clients.
 *            A playlist is a set of "index" and "media metadata".\n
 *            The media controller server can let media controller clients know the playlist name and index of current playing media by using this function.
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server The handle to media controller server
 * @param[in] playlist_name    The name of the playlist. The length of the playlist name should be less than 4096 bytes
 * @param[in] index    The index of the current playing media in the playlist
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @post Apply the updated playback information by calling mc_server_update_playback_info().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_update_playback_info()
 * @see mc_server_create_playlist()
 */
int mc_server_set_playlist_item_info(mc_server_h server, const char *playlist_name, const char *index);

/**
 * @brief Sets the playback content type to update the latest playback info.
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks If @a content_type is #MC_CONTENT_TYPE_UNDECIDED, this function will return #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER.
 *
 * @param[in] server The handle to media controller server
 * @param[in] content_type The content type to set the latest status
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @post Apply the updated playback information by calling mc_server_update_playback_info().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_update_playback_info()
 */
int mc_server_set_playback_content_type(mc_server_h server, mc_content_type_e content_type);

/**
 * @brief Sets the content age rating to update the latest playback info.
 * @details Default value is #MC_CONTENT_RATING_ALL.
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server The handle to media controller server
 * @param[in] age_rating Age rating of the content
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @post Apply the updated age rating information by calling mc_server_update_playback_info().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_update_playback_info()
 */
int mc_server_set_content_age_rating(mc_server_h server, mc_content_age_rating_e age_rating);

/**
 * @brief Updates the modified playback info.
 * @details If this function is called, the updated playback information will be sent to media controller clients who set callback by using mc_client_set_playback_updated_cb().
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_playback_state()
 * @see mc_server_set_playback_position()
 * @see mc_server_set_playlist_item_info()
 * @see mc_server_set_playback_content_type()
 * @see mc_server_set_content_age_rating()
 * @see mc_client_set_playback_updated_cb()
 */
int mc_server_update_playback_info(mc_server_h server);

/**
 * @brief Sets the metadata to update the latest metadata info.
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server The handle to media controller server
 * @param[in] attribute    The key attribute name to set
 * @param[in] value      The value of the attribute
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @post Apply the updated metadata information by calling mc_server_update_metadata().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_update_metadata()
 */
int mc_server_set_metadata(mc_server_h server, mc_meta_e attribute, const char *value);

/**
 * @brief Updates the modified metadata info.
 * @details If this function is called, the updated metadata will be sent to media controller clients who set callback by using mc_client_set_metadata_updated_cb().
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @pre Set metadata by calling mc_server_set_metadata().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_metadata()
 * @see mc_client_set_metadata_updated_cb()
 */
int mc_server_update_metadata(mc_server_h server);

/**
 * @brief Updates the modified shuffle mode.
 * @details If this function is called, the updated mode information will be sent to media controller clients who set callback by using mc_client_set_shuffle_mode_updated_cb().
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server The handle to media controller server
 * @param[in] mode The shuffle mode to update the latest status
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_client_set_shuffle_mode_updated_cb()
 */
int mc_server_update_shuffle_mode(mc_server_h server, mc_shuffle_mode_e mode);

/**
 * @brief Updates the modified repeat mode.
 * @details If this function is called, the updated mode information will be sent to media controller clients who set callback by using mc_client_set_repeat_mode_updated_cb().
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server The handle to media controller server
 * @param[in] mode The repeat mode to update the latest status
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_client_set_repeat_mode_updated_cb()
 */
int mc_server_update_repeat_mode(mc_server_h server, mc_repeat_mode_e mode);

/**
 * @brief Updates the modified subtitles display status.
 * @details If this function is called, the updated subtitles display status will be sent to media controller clients who set callback by using mc_client_set_subtitles_updated_cb().
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks The default subtitles display is false.
 *
 * @param[in] server The handle to media controller server
 * @param[in] enabled The subtitles display status to update the latest status
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_client_set_subtitles_updated_cb()
 */
int mc_server_update_subtitles_enabled(mc_server_h server, bool enabled);

/**
 * @brief Updates the modified 360 mode display status.
 * @details If this function is called, the updated 360 mode display status will be sent to media controller clients who set callback by using mc_client_set_360_mode_updated_cb().
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks The default 360 mode display is false.
 *
 * @param[in] server The handle to media controller server
 * @param[in] enabled The 360 mode display status to update the latest status
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_client_set_360_mode_updated_cb()
 */
int mc_server_update_360_mode_enabled(mc_server_h server, bool enabled);

/**
 * @brief Updates the modified display mode.
 * @details If this function is called, the updated display mode information will be sent to media controller clients who set callback by using mc_client_set_display_mode_updated_cb().
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks The default display mode is #MC_DISPLAY_MODE_FULL_SCREEN.
 *
 * @param[in] server The handle to media controller server
 * @param[in] mode The display mode to update the latest status
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_client_set_display_mode_updated_cb()
 */
int mc_server_update_display_mode(mc_server_h server, mc_display_mode_e mode);

/**
 * @brief Updates the modified display rotation.
 * @details If this function is called, the updated display rotation information will be sent to media controller clients who set callback by using mc_client_set_display_rotation_updated_cb().
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks The default display rotation is #MC_DISPLAY_ROTATION_NONE.
 *
 * @param[in] server The handle to media controller server
 * @param[in] rotation The display rotation to update the latest status
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_client_set_display_rotation_updated_cb()
 */
int mc_server_update_display_rotation(mc_server_h server, mc_display_rotation_e rotation);

/**
 * @brief Sets the callback for receiving playback action command from client.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is recommended to set the corresponding ability's support using mc_server_set_playback_ability(). It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @param[in] callback      The callback to be invoked when media controller server receives playback action from client.
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_unset_playback_action_cmd_received_cb()
 * @see mc_server_set_playback_ability()
 * @see mc_client_send_playback_action_cmd()
 */
int mc_server_set_playback_action_cmd_received_cb(mc_server_h server, mc_server_playback_action_cmd_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback for receiving playback action command from client.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is recommended to set the corresponding ability's support using mc_server_set_playback_ability(). It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_playback_action_cmd_received_cb()
 * @see mc_server_set_playback_ability()
 */
int mc_server_unset_playback_action_cmd_received_cb(mc_server_h server);

/**
 * @brief Sets the callback for receiving playback position command from client.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is recommended to set the corresponding ability's support using mc_server_set_ability_support(). It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @param[in] callback      The callback to be invoked when media controller server receives position command from client
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_unset_playback_position_cmd_received_cb()
 * @see mc_server_set_ability_support()
 * @see mc_client_send_playback_position_cmd()
 */
int mc_server_set_playback_position_cmd_received_cb(mc_server_h server, mc_server_playback_position_cmd_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback for receiving playback position command from client.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is recommended to set the corresponding ability's support using mc_server_set_ability_support(). It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_playback_position_cmd_received_cb()
 * @see mc_server_set_playback_ability()
 */
int mc_server_unset_playback_position_cmd_received_cb(mc_server_h server);

/**
 * @brief Sets the callback for receiving shuffle mode command from client.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is recommended to set the corresponding ability's support using mc_server_set_ability_support(). It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @param[in] callback      The callback to be invoked when media controller server receives shuffle mode from client
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_unset_shuffle_mode_cmd_received_cb()
 * @see mc_server_set_ability_support()
 * @see mc_client_send_shuffle_mode_cmd()
 */
int mc_server_set_shuffle_mode_cmd_received_cb(mc_server_h server, mc_server_shuffle_mode_cmd_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback for receiving shuffle mode command from client.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is recommended to set the corresponding ability's support using mc_server_set_ability_support(). It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_shuffle_mode_cmd_received_cb()
 * @see mc_server_set_ability_support()
 */
int mc_server_unset_shuffle_mode_cmd_received_cb(mc_server_h server);

/**
 * @brief Sets the callback for receiving repeat mode command from client.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is recommended to set the corresponding ability's support using mc_server_set_ability_support(). It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @param[in] callback      The callback to be invoked when media controller server receives repeat mode from client
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_unset_repeat_mode_cmd_received_cb()
 * @see mc_server_set_ability_support()
 * @see mc_client_send_repeat_mode_cmd()
 */
int mc_server_set_repeat_mode_cmd_received_cb(mc_server_h server, mc_server_repeat_mode_cmd_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback for receiving repeat mode command from client.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is recommended to set the corresponding ability's support using mc_server_set_ability_support(). It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_repeat_mode_cmd_received_cb()
 * @see mc_server_set_ability_support()
 */
int mc_server_unset_repeat_mode_cmd_received_cb(mc_server_h server);

/**
 * @brief Sets the callback for receiving playlist command from client.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is recommended to set the corresponding ability's support using mc_server_set_ability_support(). It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @param[in] callback      The callback to be invoked when media controller server receives playlist command from client
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_unset_playlist_cmd_received_cb()
 * @see mc_server_set_ability_support()
 * @see mc_client_send_playlist_cmd()
 */
int mc_server_set_playlist_cmd_received_cb(mc_server_h server, mc_server_playlist_cmd_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback for receiving playlist command from client.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is recommended to set the corresponding ability's support using mc_server_set_ability_support(). It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_playlist_cmd_received_cb()
 * @see mc_server_set_ability_support()
 */
int mc_server_unset_playlist_cmd_received_cb(mc_server_h server);

/**
 * @brief Sets the callback for receiving custom command from client.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is recommended to set the corresponding ability's support using mc_server_set_ability_support(). It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @param[in] callback      The callback to be invoked when media controller server receives custom command from client
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_unset_custom_cmd_received_cb()
 * @see mc_server_set_ability_support()
 * @see mc_client_send_custom_cmd()
 */
int mc_server_set_custom_cmd_received_cb(mc_server_h server, mc_server_custom_cmd_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback for receiving custom command from client.
 * @since_tizen 4.0
 *
 * @remarks It is recommended to set the corresponding ability's support using mc_server_set_ability_support(). It will let the client know about the server's abilities and what commands the client can send.
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server    The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_custom_cmd_received_cb()
 * @see mc_server_set_ability_support()
 */
int mc_server_unset_custom_cmd_received_cb(mc_server_h server);

/**
 * @brief Sets the callback for receiving subtitles command from client.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is mandatory to set the corresponding ability's support using mc_server_set_ability_support(). \n
 *              If not, server can't receive command from clients. It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @param[in] callback      The callback to be invoked when media controller server receives subtitles command from client
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_unset_subtitles_cmd_received_cb()
 * @see mc_server_set_ability_support()
 * @see mc_client_send_subtitles_cmd()
 */
int mc_server_set_subtitles_cmd_received_cb(mc_server_h server, mc_server_enable_cmd_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback for receiving subtitles command from client.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is mandatory to set the corresponding ability's support using mc_server_set_ability_support(). \n
 *              If not, server can't receive command from clients. It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_subtitles_cmd_received_cb()
 * @see mc_server_set_ability_support()
 */
int mc_server_unset_subtitles_cmd_received_cb(mc_server_h server);

/**
 * @brief Sets the callback for receiving 360 mode command from client.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is mandatory to set the corresponding ability's support using mc_server_set_ability_support(). \n
 *              If not, server can't receive command from clients. It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @param[in] callback      The callback to be invoked when media controller server receives 360 mode command from client
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_unset_360_mode_cmd_received_cb()
 * @see mc_server_set_ability_support()
 * @see mc_client_send_360_mode_cmd()
 */
int mc_server_set_360_mode_cmd_received_cb(mc_server_h server, mc_server_enable_cmd_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback for receiving 360 mode command from client.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is mandatory to set the corresponding ability's support using mc_server_set_ability_support(). \n
 *              If not, server can't receive command from clients. It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_360_mode_cmd_received_cb()
 * @see mc_server_set_ability_support()
 */
int mc_server_unset_360_mode_cmd_received_cb(mc_server_h server);

/**
 * @brief Sets the callback for receiving display mode command from client.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is mandatory to set the corresponding ability's support using mc_server_set_display_mode_ability(). \n
 *              If not, server can't receive command from clients. It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @param[in] callback      The callback to be invoked when media controller server receives display mode command from client
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_unset_display_mode_cmd_received_cb()
 * @see mc_server_set_display_mode_ability()
 * @see mc_client_send_display_mode_cmd()
 */
int mc_server_set_display_mode_cmd_received_cb(mc_server_h server, mc_server_display_mode_cmd_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback for receiving display mode command from client.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is mandatory to set the corresponding ability's support using mc_server_set_display_mode_ability(). \n
 *              If not, server can't receive command from clients. It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_display_mode_cmd_received_cb()
 * @see mc_server_set_display_mode_ability()
 */
int mc_server_unset_display_mode_cmd_received_cb(mc_server_h server);

/**
 * @brief Sets the callback for receiving display rotation command from client.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is mandatory to set the corresponding ability's support using mc_server_set_display_rotation_ability(). \n
 *              If not, server can't receive command from clients. It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @param[in] callback      The callback to be invoked when media controller server receives display rotation command from client
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_unset_display_rotation_cmd_received_cb()
 * @see mc_server_set_display_rotation_ability()
 * @see mc_client_send_display_rotation_cmd()
 */
int mc_server_set_display_rotation_cmd_received_cb(mc_server_h server, mc_server_display_rotation_cmd_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback for receiving display rotation command from client.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is mandatory to set the corresponding ability's support using mc_server_set_display_rotation_ability(). \n
 *              If not, server can't receive command from clients. It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_display_rotation_cmd_received_cb()
 * @see mc_server_set_display_rotation_ability()
 */
int mc_server_unset_display_rotation_cmd_received_cb(mc_server_h server);

/**
 * @brief Replies the result of the requested command to the media controller client.
 * @details If this function is called, reply for the command will be sent to the media controller client who set callback by using mc_client_set_cmd_reply_received_cb().
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks When the server receives a command, this function can be called by mc_server_playback_action_cmd_received_cb(), mc_server_playback_position_cmd_received_cb(), mc_server_shuffle_mode_cmd_received_cb(), mc_server_repeat_mode_cmd_received_cb(), mc_server_playlist_cmd_received_cb() and mc_server_custom_cmd_received_cb().\n
 *          If @a request_id is null, this function will return #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER.
 *
 * @param[in] server    The handle to media controller server
 * @param[in] client_name    The app_id of the media controller client
 * @param[in] request_id    The id of the command request, received in the mc_server_custom_cmd_received_cb() function
 * @param[in] result_code    The result code of custom command, #mc_result_code_e since 6.0
 * @param[in] data                The extra data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_playback_action_cmd_received_cb()
 * @see mc_server_playback_position_cmd_received_cb()
 * @see mc_server_shuffle_mode_cmd_received_cb()
 * @see mc_server_repeat_mode_cmd_received_cb()
 * @see mc_server_playlist_cmd_received_cb()
 * @see mc_server_custom_cmd_received_cb()
 * @see mc_server_destroy()
 * @see mc_client_set_cmd_reply_received_cb()
 */
int mc_server_send_cmd_reply(mc_server_h server, const char *client_name, const char *request_id, int result_code, bundle *data);

/**
 * @brief Retrieves all created clients.
 * @details This function gets all created media controller clients.
 *          The callback function will be invoked for every created media controller client.
 *          If there are no media controller clients, the callback will not be invoked.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server    The handle to media controller server
 * @param[in] callback      The callback function to be invoked, will be called for each client
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_activated_client_cb()
 * @see mc_server_destroy()
 */
int mc_server_foreach_client(mc_server_h server, mc_activated_client_cb callback, void *user_data);

/**
 * @brief Sends the customized event with the bundle data to the media controller client.
 * @details If the event is needed in the client, this function should be called. @a data is a bundle.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks The @a request_id should be released using free().\n
 *          If the @a request_id is null, the client will not send the reply of the custom event.
 *
 * @param[in] server The handle to media controller server
 * @param[in] client_name The name of the client which receive the event
 * @param[in] event The name of the event
 * @param[in] data The data can include other information associated with the event
 * @param[out] request_id   The id of the event request, it will be passed to the mc_server_event_reply_received_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_event_reply_received_cb()
 * @see mc_server_set_event_reply_received_cb()
 * @see mc_server_destroy()
 * @see mc_server_foreach_client()
 * @see mc_client_set_custom_event_received_cb()
 */
int mc_server_send_custom_event(mc_server_h server, const char *client_name, const char *event, bundle *data, char **request_id);

/**
 * @brief Sets the callback for receiving the result of the event from the media controller client.
 * @details The media controller server which calls this function will receive the result of the event from all media controller clients.\n
 *             The media controller server can send event by using mc_server_send_custom_event().
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server         The handle of the media controller server
 * @param[in] callback      The callback to be invoked when the reply is received
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @pre mc_server_send_custom_event().
 * @see mc_server_create()
 * @see mc_server_unset_event_reply_received_cb()
 * @see mc_server_destroy()
 * @see mc_server_send_custom_event()
 * @see mc_client_send_event_reply()
 */
int mc_server_set_event_reply_received_cb(mc_server_h server, mc_server_event_reply_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback for receiving event reply of the media controller client.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server         The handle of the media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_server_create()
 * @see mc_server_set_event_reply_received_cb()
 * @see mc_server_destroy()
 */
int mc_server_unset_event_reply_received_cb(mc_server_h server);

/**
 * @brief Sets the callback for receiving search command from client.
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is recommended to set the corresponding ability's support using mc_server_set_ability_support(). It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @param[in] callback      The callback to be invoked when media controller server receives search command from client
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_unset_search_cmd_received_cb()
 * @see mc_server_set_ability_support()
 * @see mc_client_send_search_cmd()
 */
int mc_server_set_search_cmd_received_cb(mc_server_h server, mc_server_search_cmd_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback for receiving search command from client.
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It is recommended to set the corresponding ability's support using mc_server_set_ability_support(). It will let the client know about the server's abilities and what commands the client can send.
 *
 * @param[in] server    The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_search_cmd_received_cb()
 * @see mc_server_set_ability_support()
 */
int mc_server_unset_search_cmd_received_cb(mc_server_h server);

/**
 * @brief Destroys media controller server.
 * @details If this function is called, the changed server's state to #MC_SERVER_STATE_DEACTIVATE will be sent to media controller clients who set callback by using mc_client_set_server_updated_cb().
 * @since_tizen 2.4
 *
 * @remarks After the server is destroyed, all its abilities will be initialized to the default automatically.
 *
 * @param[in] server The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_client_set_server_updated_cb()
 */
int mc_server_destroy(mc_server_h server);

/**
 * @brief Creates a new playlist with the given name.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks The @a playlist should be released using mc_playlist_destroy().\n
 *              When the server is destroyed, all its playlists will be deleted automatically.
 *
 * @param[in] server The handle to media controller server
 * @param[in] playlist_name The name of the created playlist
 * @param[out] playlist  The handle of the media controller playlist
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
 * @pre Create a media controller server handle by calling mc_server_create().
 * @post You can call mc_server_add_item_to_playlist() if you have items to add in the playlist and should call mc_server_update_playlist_done() to register it.
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_add_item_to_playlist()
 * @see mc_server_update_playlist_done()
 * @see mc_server_delete_playlist()
 * @see mc_playlist_destroy()
 */
int mc_server_create_playlist(mc_server_h server, const char *playlist_name, mc_playlist_h *playlist);

/**
 * @brief Adds a new item to the playlist.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It's possible to add at most 100 items at once. \n
 *              If you have more items, register the first 100 items by calling mc_server_update_playlist_done(), then add the remaining items.
 *
 * @param[in] server The handle to media controller server
 * @param[in] playlist  The handle of the media controller playlist
 * @param[in] index The index of the playlist item. It should not be NULL
 * @param[in] attribute    The key attribute name to set
 * @param[in] value      The value of the attribute
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
 * @pre Create a media controller server handle by calling mc_server_create().
 * @pre Create a media controller playlist handle by calling mc_server_create_playlist().
 * @post You should call mc_server_update_playlist_done() to register it.
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_create_playlist()
 * @see mc_server_update_playlist_done()
 * @see mc_playlist_destroy()
 */
int mc_server_add_item_to_playlist(mc_server_h server, mc_playlist_h playlist, const char *index, mc_meta_e attribute, const char *value);

/**
 * @brief Registers items to the playlist.
 * @details If this function is called, the updated playlist information with mode #MC_PLAYLIST_UPDATED will be sent to media controller clients who set callback by using mc_client_set_playlist_updated_cb().
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server The handle to media controller server
 * @param[in] playlist  The handle of the media controller playlist
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
 * @pre Create a media controller server handle by calling mc_server_create().
 * @pre Create a media controller playlist handle by calling mc_server_create_playlist().
 * @pre Add items to the playlist by calling mc_server_add_item_to_playlist().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_create_playlist()
 * @see mc_server_add_item_to_playlist()
 * @see mc_playlist_destroy()
 * @see mc_client_set_playlist_updated_cb()
 */
int mc_server_update_playlist_done(mc_server_h server, mc_playlist_h playlist);

/**
 * @brief Deletes the playlist of the media controller server.
 * @details If this function is called, the updated playlist information with mode #MC_PLAYLIST_REMOVED will be sent to media controller clients who set callback by using mc_client_set_playlist_updated_cb().
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server The handle to media controller server
 * @param[in] playlist  The handle of the media controller playlist
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
 * @pre Create a media controller server handle by calling mc_server_create().
 * @pre Create a media controller playlist handle by calling mc_server_create_playlist().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_create_playlist()
 * @see mc_playlist_destroy()
 * @see mc_client_set_playlist_updated_cb()
 */
int mc_server_delete_playlist(mc_server_h server, mc_playlist_h playlist);

/**
 * @brief Sets the playback ability's support of the media controller server.
 * @details This function allows setting the support for the following abilities: \n
 *          #MC_PLAYBACK_ACTION_PLAY, \n
 *          #MC_PLAYBACK_ACTION_PAUSE, \n
 *          #MC_PLAYBACK_ACTION_STOP, \n
 *          #MC_PLAYBACK_ACTION_NEXT, \n
 *          #MC_PLAYBACK_ACTION_PREV, \n
 *          #MC_PLAYBACK_ACTION_FAST_FORWARD, \n
 *          #MC_PLAYBACK_ACTION_REWIND, \n
 *          #MC_PLAYBACK_ACTION_TOGGLE_PLAY_PAUSE, \n
 *          To set the support for other abilities, use corresponding function. \n
 *          For a ability like shuffle, repeat, use mc_server_set_ability_support(). \n
 *          For a display mode ability, use mc_server_set_display_mode_ability(). \n
 *          For a display rotation ability, use mc_server_set_display_rotation_ability(). \n
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks If @a support is #MC_ABILITY_SUPPORTED_UNDECIDED, this function will return #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER.
 *              Default value is #MC_ABILITY_SUPPORTED_UNDECIDED. \n
 *              To notify media controller clients about the changes of the playback's ability support, use mc_server_update_playback_ability().
 *
 * @param[in] server The handle to media controller server
 * @param[in] action   The playback action
 * @param[in] support    The support value of the ability
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @post Update abilities of a media controller server by calling mc_server_update_playback_ability().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_update_playback_ability()
 */
int mc_server_set_playback_ability(mc_server_h server, mc_playback_action_e action, mc_ability_support_e support);

/**
 * @brief Updates the modified ability info.
 * @details If this function is called, the updated ability will be sent to media controller clients who set callback by using mc_client_set_playback_ability_updated_cb().
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @param[in] server The handle to media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @pre Set abilities of a media controller server by calling mc_server_set_playback_ability().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_playback_ability()
 * @see mc_client_set_playback_ability_updated_cb()
 */
int mc_server_update_playback_ability(mc_server_h server);

/**
 * @brief Sets the ability's support of the media controller.
 * @details This function allows setting the support for the following abilities: \n
 *          #MC_ABILITY_SHUFFLE, \n
 *          #MC_ABILITY_REPEAT, \n
 *          #MC_ABILITY_PLAYBACK_POSITION, \n
 *          #MC_ABILITY_PLAYLIST, \n
 *          #MC_ABILITY_CLIENT_CUSTOM, \n
 *          #MC_ABILITY_SEARCH, \n
 *          #MC_ABILITY_SUBTITLES, \n
 *          #MC_ABILITY_360_MODE, \n
 *          If this function is called, the updated ability will be sent to media controller clients who set callback by using mc_client_set_ability_support_updated_cb().
 *          To set the support for other abilities, use corresponding function. \n
 *          For a playback ability, use mc_server_set_playback_ability() and mc_server_update_playback_ability(). \n
 *          For a display mode ability, use mc_server_set_display_mode_ability(). \n
 *          For a display rotation ability, use mc_server_set_display_rotation_ability(). \n
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks If @a support is #MC_ABILITY_SUPPORTED_UNDECIDED, this function will return #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER. \n
 *              Default value is #MC_ABILITY_SUPPORTED_UNDECIDED. \n
 *              But for the #MC_ABILITY_SUBTITLES, #MC_ABILITY_360_MODE default value is #MC_ABILITY_SUPPORTED_NO. \n
 *              Clients can send command to servers when server's ability is #MC_ABILITY_SUPPORTED_YES or #MC_ABILITY_SUPPORTED_UNDECIDED.
 *              So, it is required that server set the corresponding ability's support to communicate with clients.
 *
 * @param[in] server The handle to media controller server
 * @param[in] ability The ability to set support
 * @param[in] support    The support value of the ability
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_playback_ability()
 * @see mc_server_update_playback_ability()
 * @see mc_server_set_display_mode_ability()
 * @see mc_server_set_display_rotation_ability()
 * @see mc_client_set_ability_support_updated_cb()
 */
int mc_server_set_ability_support(mc_server_h server, mc_ability_e ability, mc_ability_support_e support);

/**
 * @brief Sets the display mode ability of the media controller.
 * @details If this function is called, the updated display mode will be sent to media controller clients who set callback by using mc_client_set_display_mode_ability_updated_cb().
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks If @a support is #MC_ABILITY_SUPPORTED_UNDECIDED, this function will return #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER.
 *              The default display mode ability is #MC_ABILITY_SUPPORTED_NO. \n
 *              It means client can't send the corresponding command to the server, so to receive command from clients, set ability as #MC_ABILITY_SUPPORTED_YES.
 *
 * @param[in] server The handle to media controller server
 * @param[in] mode The display mode, values of #mc_display_mode_e combined with bitwise 'or'.
 * @param[in] support    The support value of the ability
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_server_set_display_mode_cmd_received_cb()
 * @see mc_client_set_display_mode_ability_updated_cb()
 */
int mc_server_set_display_mode_ability(mc_server_h server, unsigned int mode, mc_ability_support_e support);

/**
 * @brief Sets the display rotation ability of the media controller.
 * @details If this function is called, the updated display rotation will be sent to media controller clients who set callback by using mc_client_set_display_rotation_ability_updated_cb().
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks If @a support is #MC_ABILITY_SUPPORTED_UNDECIDED, this function will return #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER.
 *              The default display rotation ability is #MC_ABILITY_SUPPORTED_NO. \n
 *              It means client can't send the corresponding command to the server, so to receive command from clients, set ability as #MC_ABILITY_SUPPORTED_YES.
 *
 * @param[in] server The handle to media controller server
 * @param[in] rotation The display rotation, values of #mc_display_rotation_e combined with bitwise 'or'
 * @param[in] support    The support value of the ability
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_client_set_display_rotation_ability_updated_cb()
 */
int mc_server_set_display_rotation_ability(mc_server_h server, unsigned int rotation, mc_ability_support_e support);

/**
 * @brief Sets the icon URI of media controller server.
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.server
 *
 * @remarks It's possible to set URI to NULL if there is no icon.
 *
 * @param[in] server The handle to media controller server
 * @param[in] uri    The icon URI
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_server_create()
 * @see mc_server_destroy()
 * @see mc_client_get_server_icon()
 */
int mc_server_set_icon(mc_server_h server, const char *uri);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MULTIMEDIA_MEDIA_CONTROLLER_SERVER_H__ */
