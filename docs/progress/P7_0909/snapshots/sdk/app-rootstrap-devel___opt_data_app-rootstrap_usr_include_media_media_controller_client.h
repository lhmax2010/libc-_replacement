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

#ifndef __TIZEN_MULTIMEDIA_MEDIA_CONTROLLER_CLIENT_H__
#define __TIZEN_MULTIMEDIA_MEDIA_CONTROLLER_CLIENT_H__

#include <media_controller_type.h>
#include <media_controller_playlist.h>
#include <media_controller_metadata.h>
#include <bundle.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_controller_client.h
 * @brief This file contains the media controller client API and functions related with handling media control. \n
 *        Functions include operations to get the latest status of media controller servers.
 *        It also includes functions that can process commands received from servers.
 */

/**
 * @addtogroup CAPI_MEDIA_CONTROLLER_CLIENT_MODULE
 * @{
 */

/**
 * @brief Called when the status of the media controller server is updated.
 * @since_tizen 2.4
 *
 * @remarks The callback is called in the dbus event loop.\n
 *              The @a server_name should not be released by the application.\n
 *              The @a server_name can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] server_name The app_id of the media controller server which sent the notification
 * @param[in] state The state of the updated media controller server
 * @param[in] user_data        The user data passed from the mc_client_set_server_updated_cb() function
 *
 * @pre mc_client_set_server_updated_cb()
 *
 * @see mc_client_set_server_updated_cb()
 */
typedef void (*mc_server_state_updated_cb)(const char *server_name, mc_server_state_e state, void *user_data);

/**
 * @brief Called when the playback information of the media controller server is updated.
 * @since_tizen 2.4
 *
 * @remarks The callback is called in the dbus event loop.\n
 *               The @a server_name and @a playback should not be released by the application. \n
 *               The @a server_name can be used only in the callback. To use outside, make a copy. \n
 *               And @a playback also can be used only in the callback.
 *
 * @param[in] server_name The app_id of the media controller server which sent the notification
 * @param[in] playback The playback information of the updated media controller server
 * @param[in] user_data        The user data passed from the mc_client_set_playback_updated_cb() function
 *
 * @pre mc_client_set_playback_updated_cb()
 *
 * @see mc_client_set_playback_updated_cb()
 * @see mc_client_get_playback_state()
 * @see mc_client_get_playback_position()
 * @see mc_client_get_playlist_item_info()
 */
typedef void (*mc_playback_updated_cb)(const char *server_name, mc_playback_h playback, void *user_data);

/**
 * @brief Called when the metadata of the media controller server is updated.
 * @since_tizen 2.4
 *
 * @remarks The callback is called in the dbus event loop.\n
 *               The @a server_name and @a metadata should not be released by the application. \n
 *               The @a server_name can be used only in the callback. To use outside, make a copy. \n
 *               And @a metadata also can be used only in the callback. To use outside, make a copy using mc_metadata_clone().
 *
 * @param[in] server_name The app_id of the media controller server which sent the notification
 * @param[in] metadata The metadata of the updated media controller server
 * @param[in] user_data        The user data passed from the mc_client_set_metadata_updated_cb() function
 *
 * @pre mc_client_set_metadata_updated_cb()
 *
 * @see mc_client_set_metadata_updated_cb()
 * @see mc_metadata_get()
 */
typedef void (*mc_metadata_updated_cb)(const char *server_name, mc_metadata_h metadata, void *user_data);

/**
 * @brief Called when the shuffle mode of the media controller server is updated.
 * @since_tizen 4.0
 *
 * @remarks The callback is called in the dbus event loop.\n
 *               The @a server_name should not be released by the application. \n
 *               The @a server_name can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] server_name The app_id of the media controller server which sent the notification
 * @param[in] mode The shuffle mode of the updated media controller server
 * @param[in] user_data        The user data passed from the mc_client_set_shuffle_mode_updated_cb() function
 *
 * @pre mc_client_set_shuffle_mode_updated_cb()
 *
 * @see mc_client_set_shuffle_mode_updated_cb()
 */
typedef void (*mc_shuffle_mode_updated_cb)(const char *server_name, mc_shuffle_mode_e mode, void *user_data);

/**
 * @brief Called when the repeat mode of the media controller server is updated.
 * @since_tizen 4.0
 *
 * @remarks The callback is called in the dbus event loop.\n
 *               The @a server_name should not be released by the application. \n
 *               The @a server_name can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] server_name The app_id of the media controller server which sent the notification
 * @param[in] mode The repeat mode of the updated media controller server
 * @param[in] user_data        The user data passed from the mc_client_set_repeat_mode_updated_cb() function
 *
 * @pre mc_client_set_repeat_mode_updated_cb()
 *
 * @see mc_client_set_repeat_mode_updated_cb()
 */
typedef void (*mc_repeat_mode_updated_cb)(const char *server_name, mc_repeat_mode_e mode, void *user_data);

/**
 * @brief Called when the playback ability support of the media controller server is updated.
 * @since_tizen 5.0
 *
 * @remarks The callback is called in the dbus event loop.\n
 *               The @a server_name should not be released by the application. \n
 *               The @a server_name can be used only in the callback. To use outside, make a copy. \n
 *               And @a ability also can be used only in the callback. To use outside, make a copy using mc_playback_ability_clone().
 *
 * @param[in] server_name The app_id of the media controller server which sent the notification
 * @param[in] ability The ability of the updated media controller server
 * @param[in] user_data        The user data passed from the mc_client_set_playback_ability_updated_cb() function
 *
 * @pre mc_client_set_playback_ability_updated_cb()
 *
 * @see mc_client_set_playback_ability_updated_cb()
 * @see mc_playback_action_is_supported()
 * @see mc_playback_ability_clone()
 */
typedef void (*mc_playback_ability_updated_cb)(const char *server_name, mc_playback_ability_h ability, void *user_data);

/**
 * @brief Called when a media controller server's support for an ability is updated.
 * @since_tizen 5.5
 *
 * @remarks The callback is called in the dbus event loop.\n
 *               The @a server_name should not be released by the application. \n
 *               The @a server_name can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] server_name The app_id of the media controller server which sent the notification
 * @param[in] ability The ability
 * @param[in] support The support value of the ability of the updated media controller server
 * @param[in] user_data        The user data passed from the callback function
 *
 * @see mc_client_set_ability_support_updated_cb()
 */
typedef void (*mc_ability_support_updated_cb)(const char *server_name, mc_ability_e ability, mc_ability_support_e support, void *user_data);

/**
 * @brief Called when a media controller server's supported items for an ability is updated.
 * @since_tizen 5.5
 *
 * @remarks The callback is called in the dbus event loop.\n
 *               The @a server_name should not be released by the application. \n
 *               The @a server_name can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] server_name The app_id of the media controller server which sent the notification
 * @param[in] supported_items The supported items of the updated media controller server
 * @param[in] user_data        The user data passed from the callback function
 *
 * @see mc_client_set_display_mode_ability_updated_cb()
 * @see mc_client_set_display_rotation_ability_updated_cb()
 */
typedef void (*mc_ability_supported_items_updated_cb)(const char *server_name, unsigned int supported_items, void *user_data);

/**
 * @brief Called when requesting the list of created servers.
 * @since_tizen 2.4
 *
 * @remarks The callback is called in the main loop.\n
 *               The @a server_name should not be released by the application. \n
 *               The @a server_name can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] server_name The app_id of the created media controller server
 * @param[in] user_data        The user data passed from the mc_client_foreach_server() function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre mc_client_foreach_server()
 *
 * @see mc_client_foreach_server()
 */
typedef bool (*mc_activated_server_cb)(const char *server_name, void *user_data);

/**
 * @brief Called when the result of the command from the server is received.
 * @since_tizen 4.0
 *
 * @remarks The callback is called in the dbus event loop.\n @a data should be released using bundle_free().\n
 *              The @a server_name and @a request_id should not be released by the application. \n
 *              The @a server_name and @a request_id can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] server_name    The app_id of the media controller server which sent the reply
 * @param[in] request_id     The id of the command request
 * @param[in] result_code    The result code of the action, #mc_result_code_e since 6.0
 * @param[in] data The extra data
 * @param[in] user_data        The user data passed from the mc_client_set_cmd_reply_received_cb() function
 *
 * @pre mc_client_send_custom_cmd()
 * @pre mc_client_set_cmd_reply_received_cb()
 *
 * @see mc_client_send_custom_cmd()
 */
typedef void (*mc_cmd_reply_received_cb)(const char *server_name, const char *request_id, int result_code, bundle *data, void *user_data);

/**
 * @brief Called when requesting the list of subscribed servers.
 * @since_tizen 2.4
 *
 * @remarks The callback is called in the dbus event loop.\n
 *              The @a server_name should not be released by the application. \n
 *              The @a server_name can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] server_name The app_id of the subscribed media controller server
 * @param[in] user_data        The user data passed from the mc_client_foreach_server_subscribed() function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre mc_client_foreach_server_subscribed()
 *
 * @see mc_client_foreach_server_subscribed()
 */
typedef bool (*mc_subscribed_server_cb)(const char *server_name, void *user_data);

/**
 * @brief Called when the playlist of the media controller server is updated.
 * @since_tizen 4.0
 *
 * @remarks The callback is called in the dbus event loop.\n
 *              The @a server_name, @a playlist_name and @a playlist should not be released by the application. \n
 *              The @a server_name and @a playlist_name can be used only in the callback. To use outside, make a copy.\n
 *              And @a playlist also can be used only in the callback. To use outside, make a copy using mc_playlist_clone().
 *
 * @param[in] server_name The app_id of the media controller server which sent the notification
 * @param[in] mode The playlist update mode of the updated media controller server
 * @param[in] playlist_name Updated playlist name
 * @param[in] playlist        The handle of the media controller playlist
 * @param[in] user_data The user data passed from the mc_client_set_playlist_updated_cb() function
 *
 * @pre mc_client_set_playlist_updated_cb()
 *
 * @see mc_client_set_playlist_updated_cb()
 * @see mc_playlist_foreach_playlist()
 * @see mc_playlist_get_name()
 * @see mc_playlist_get_item_count()
 * @see mc_playlist_foreach_item()
 * @see mc_playlist_destroy()
 */
typedef void (*mc_playlist_updated_cb)(const char *server_name, mc_playlist_update_mode_e mode, const char *playlist_name, mc_playlist_h playlist, void *user_data);

/**
 * @brief Called when receiving custom event from media controller servers.
 * @since_tizen 4.0
 *
 * @remarks The callback is called in the dbus event loop.\n The @a data should be released using bundle_free(). \n
 *          The @a server_name, @a request_id and @a event should not be released by the application. \n
 *          The @a server_name, @a request_id and @a event can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] server_name The app_id of the subscribed media controller server
 * @param[in] request_id     The id of the custom event request
 * @param[in] event          The name of the custom event
 * @param[in] data           The data can include other information associated with the event
 * @param[in] user_data        The user data passed from the mc_client_set_custom_event_received_cb() function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre mc_client_set_custom_event_received_cb()
 *
 * @see mc_client_set_custom_event_received_cb()
 * @see mc_client_unset_custom_event_received_cb()
 */
typedef void (*mc_client_custom_event_received_cb)(const char *server_name, const char *request_id, const char *event, bundle *data, void *user_data);

/**
 * @brief Called when the status of a media controller server's boolean attribute (subtitles or 360 mode) is updated.
 * @since_tizen 5.5
 *
 * @remarks The callback is called in the dbus event loop.\n
 *               The @a server_name should not be released by the application. \n
 *               The @a server_name can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] server_name The app_id of the media controller server which sent the notification
 * @param[in] enabled The new status of the attribute
 * @param[in] user_data        The user data passed from the callback function
 *
 * @see mc_client_set_subtitles_updated_cb()
 * @see mc_client_set_360_mode_updated_cb()
 */
typedef void (*mc_bool_attribute_updated_cb)(const char *server_name, bool enabled, void *user_data);

/**
 * @brief Called when a media controller server's display mode is updated.
 * @since_tizen 5.5
 *
 * @remarks The callback is called in the dbus event loop.\n
 *               The @a server_name should not be released by the application. \n
 *               The @a server_name can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] server_name The app_id of the media controller server which sent the notification
 * @param[in] mode The display mode
 * @param[in] user_data        The user data passed from the callback function
 *
 * @see mc_client_set_display_mode_updated_cb()
 */
typedef void (*mc_display_mode_updated_cb)(const char *server_name, mc_display_mode_e mode, void *user_data);

/**
 * @brief Called when a media controller server's display rotation is updated.
 * @since_tizen 5.5
 *
 * @remarks The callback is called in the dbus event loop.\n
 *               The @a server_name should not be released by the application. \n
 *               The @a server_name can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] server_name The app_id of the media controller server which sent the notification
 * @param[in] rotation The display rotation
 * @param[in] user_data        The user data passed from the callback function
 *
 * @see mc_client_set_display_rotation_updated_cb()
 */
typedef void (*mc_display_rotation_updated_cb)(const char *server_name, mc_display_rotation_e rotation, void *user_data);

/**
 * @brief Creates a media controller client.
 * @details The media controller client binds the latest media controller server when handlers are created.
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a client should be released using mc_client_destroy(). You can create only one client handle for each process.
 *
 * @param[out] client The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mc_client_destroy()
 */
int mc_client_create(mc_client_h *client);

/**
 * @brief Sets the callback for monitoring status of media controller servers.
 * @details The media controller client which calls this function will receive notifications from all media controller servers.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client         The handle of the media controller client
 * @param[in] callback      The callback to be invoked when the media controller server status is changed
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_unset_server_updated_cb()
 * @see mc_server_create()
 * @see mc_server_destroy()
 */
int mc_client_set_server_updated_cb(mc_client_h client, mc_server_state_updated_cb callback, void *user_data);

/**
 * @brief Unsets the callback for monitoring status of media controller servers.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client         The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_server_updated_cb()
 */
int mc_client_unset_server_updated_cb(mc_client_h client);

/**
 * @brief Sets the callback for monitoring playback status of media controller servers.
 * @details The media controller client which calls this function will receive notifications from all media controller servers.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client         The handle of the media controller client
 * @param[in] callback      The callback to be invoked when the playback status is changed
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_unset_playback_updated_cb()
 * @see mc_server_update_playback_info()
 */
int mc_client_set_playback_updated_cb(mc_client_h client, mc_playback_updated_cb callback, void *user_data);

/**
 * @brief Unsets the callback for monitoring playback status of media controller servers.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client         The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_playback_updated_cb()
 */
int mc_client_unset_playback_updated_cb(mc_client_h client);

/**
 * @brief Sets the callback for monitoring metadata status of media controller servers.
 * @details The media controller client which calls this function will receive notifications from all media controller servers.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client         The handle of the media controller client
 * @param[in] callback      The callback to be invoked when the metadata status is changed
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_unset_metadata_updated_cb()
 * @see mc_server_update_metadata()
 */
int mc_client_set_metadata_updated_cb(mc_client_h client, mc_metadata_updated_cb callback, void *user_data);

/**
 * @brief Unsets the callback for monitoring metadata status of media controller servers.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client         The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_metadata_updated_cb()
 */
int mc_client_unset_metadata_updated_cb(mc_client_h client);

/**
 * @brief Sets the callback for monitoring shuffle mode of media controller servers.
 * @details The media controller client which calls this function will receive notifications from all media controller servers.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] callback      The callback to be invoked when the shuffle mode is changed
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_unset_shuffle_mode_updated_cb()
 * @see mc_server_update_shuffle_mode()
 */
int mc_client_set_shuffle_mode_updated_cb(mc_client_h client, mc_shuffle_mode_updated_cb callback, void *user_data);

/**
 * @brief Unsets the callback for monitoring shuffle mode of media controller servers.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_shuffle_mode_updated_cb()
 */
int mc_client_unset_shuffle_mode_updated_cb(mc_client_h client);

/**
 * @brief Sets the callback for monitoring repeat mode of media controller servers.
 * @details The media controller client which calls this function will receive notifications from all media controller servers.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] callback      The callback to be invoked when the repeat mode is changed
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_unset_repeat_mode_updated_cb()
 * @see mc_server_update_repeat_mode()
 */
int mc_client_set_repeat_mode_updated_cb(mc_client_h client, mc_repeat_mode_updated_cb callback, void *user_data);

/**
 * @brief Unsets the callback for monitoring repeat mode of media controller servers.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_repeat_mode_updated_cb()
 */
int mc_client_unset_repeat_mode_updated_cb(mc_client_h client);

/**
 * @brief Sets the callback for monitoring the playback ability of media controller servers.
 * @details The media controller client which calls this function will receive notifications from all media controller servers.
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] callback      The callback to be invoked when the ability is changed
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_unset_playback_ability_updated_cb()
 * @see mc_server_update_playback_ability()
 */
int mc_client_set_playback_ability_updated_cb(mc_client_h client, mc_playback_ability_updated_cb callback, void *user_data);

/**
 * @brief Unsets the callback for monitoring the playback ability of media controller servers.
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_playback_ability_updated_cb()
 */
int mc_client_unset_playback_ability_updated_cb(mc_client_h client);

/**
 * @brief Sets the callback for monitoring the media controller server's support for an ability.
 * @details The media controller client which calls this function will receive notifications from all media controller servers.
 *            This function can be used to monitor the following abilities:\n
 *	         #MC_ABILITY_SHUFFLE, \n
 *	         #MC_ABILITY_REPEAT, \n
 *	         #MC_ABILITY_PLAYBACK_POSITION, \n
 *	         #MC_ABILITY_PLAYLIST, \n
 *	         #MC_ABILITY_CLIENT_CUSTOM, \n
 *	         #MC_ABILITY_SEARCH, \n
 *	         #MC_ABILITY_SUBTITLES, \n
 *	         #MC_ABILITY_360_MODE, \n
 *            To monitor the media controller server's support for other abilities, use corresponding function. \n
 *            For a playback ability, use mc_client_set_playback_ability_updated_cb(). \n
 *            For a display mode ability, use mc_client_set_display_mode_ability_updated_cb(). \n
 *            For a display rotation ability, use mc_client_set_display_rotation_ability_updated_cb(). \n
 *
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] callback      The callback to be invoked when the ability is changed
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_unset_ability_support_updated_cb()
 * @see mc_client_set_playback_ability_updated_cb()
 * @see mc_client_set_display_mode_ability_updated_cb()
 * @see mc_client_set_display_rotation_ability_updated_cb()
 * @see mc_server_set_ability_support()
 */
int mc_client_set_ability_support_updated_cb(mc_client_h client, mc_ability_support_updated_cb callback, void *user_data);

/**
 * @brief Unsets the callback for monitoring the media controller server's support for an ability.
 * @details The media controller client which calls this function will not receive notifications from all media controller servers.
 *            This function can be used for the following abilities:\n
 *	         #MC_ABILITY_SHUFFLE, \n
 *	         #MC_ABILITY_REPEAT, \n
 *	         #MC_ABILITY_PLAYBACK_POSITION, \n
 *	         #MC_ABILITY_PLAYLIST, \n
 *	         #MC_ABILITY_CLIENT_CUSTOM, \n
 *	         #MC_ABILITY_SEARCH, \n
 *	         #MC_ABILITY_SUBTITLES, \n
 *	         #MC_ABILITY_360_MODE, \n
 *            To stop monitoring the media controller server's support for other abilities, use corresponding function. \n
 *            For a playback ability, use mc_client_unset_playback_ability_updated_cb(). \n
 *            For a display mode ability, use mc_client_unset_display_mode_ability_updated_cb(). \n
 *            For a display rotation ability, use mc_client_unset_display_rotation_ability_updated_cb(). \n
 *
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_ability_support_updated_cb()
 * @see mc_client_unset_playback_ability_updated_cb()
 * @see mc_client_unset_display_mode_ability_updated_cb()
 * @see mc_client_unset_display_rotation_ability_updated_cb()
 */
int mc_client_unset_ability_support_updated_cb(mc_client_h client);

/**
 * @brief Sets the callback for monitoring the display mode ability of media controller servers.
 * @details The media controller client which calls this function will receive notifications from all media controller servers.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] callback      The callback to be invoked when the ability is changed
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_unset_display_mode_ability_updated_cb()
 * @see mc_server_set_display_mode_ability()
 */
int mc_client_set_display_mode_ability_updated_cb(mc_client_h client, mc_ability_supported_items_updated_cb callback, void *user_data);

/**
 * @brief Unsets the callback for monitoring the display mode ability of media controller servers.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_display_mode_ability_updated_cb()
 */
int mc_client_unset_display_mode_ability_updated_cb(mc_client_h client);

/**
 * @brief Sets the callback for monitoring the display rotation ability of media controller servers.
 * @details The media controller client which calls this function will receive notifications from all media controller servers.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] callback      The callback to be invoked when the ability is changed
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_unset_display_rotation_ability_updated_cb()
 * @see mc_server_set_display_rotation_ability()
 */
int mc_client_set_display_rotation_ability_updated_cb(mc_client_h client, mc_ability_supported_items_updated_cb callback, void *user_data);

/**
 * @brief Unsets the callback for monitoring the display rotation ability of media controller servers.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_display_rotation_ability_updated_cb()
 */
int mc_client_unset_display_rotation_ability_updated_cb(mc_client_h client);

/**
 * @brief Sets the callback for receiving the result of the command from the media controller servers.
 * @details The media controller client which calls this function will receives the result of the command from all media controller servers.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client         The handle of the media controller client
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
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_unset_cmd_reply_received_cb()
 * @see mc_server_send_cmd_reply()
 */
int mc_client_set_cmd_reply_received_cb(mc_client_h client, mc_cmd_reply_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback for receiving common reply of media controller servers.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client         The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_cmd_reply_received_cb()
 */
int mc_client_unset_cmd_reply_received_cb(mc_client_h client);

/**
 * @brief Sets the callback for monitoring playlist status of media controller servers.
 * @details The media controller client which calls this function will receive notifications from all media controller servers.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client         The handle of the media controller client
 * @param[in] callback      The callback to be invoked when the playlist status is changed
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_playlist_updated_cb()
 * @see mc_client_unset_playlist_updated_cb()
 * @see mc_server_update_playlist_done()
 * @see mc_server_delete_playlist()
 */
int mc_client_set_playlist_updated_cb(mc_client_h client, mc_playlist_updated_cb callback, void *user_data);

/**
 * @brief Unsets the callback for monitoring playlist status of media controller servers.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client         The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_playlist_updated_cb()
 */
int mc_client_unset_playlist_updated_cb(mc_client_h client);

/**
 * @brief Sets the callback for monitoring the media controller server's subtitles display.
 * @details The media controller client which calls this function will receive notifications from all media controller servers.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] callback      The callback to be invoked when the subtitles status is changed
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_unset_subtitles_updated_cb()
 * @see mc_server_update_subtitles_enabled()
 */
int mc_client_set_subtitles_updated_cb(mc_client_h client, mc_bool_attribute_updated_cb callback, void *user_data);

/**
 * @brief Unsets the callback for monitoring the media controller server's subtitles display.
 * @details The media controller client which calls this function will not receive notifications from all media controller servers.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_subtitles_updated_cb()
 */
int mc_client_unset_subtitles_updated_cb(mc_client_h client);

/**
 * @brief Sets the callback for monitoring the media controller server's 360 mode display.
 * @details The media controller client which calls this function will receive notifications from all media controller servers.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] callback      The callback to be invoked when the 360 mode is changed
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_unset_360_mode_updated_cb()
 * @see mc_server_update_360_mode_enabled()
 */
int mc_client_set_360_mode_updated_cb(mc_client_h client, mc_bool_attribute_updated_cb callback, void *user_data);

/**
 * @brief Unsets the callback for monitoring the media controller server's 360 mode display.
 * @details The media controller client which calls this function will not receive notifications from all media controller servers.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_360_mode_updated_cb()
 */
int mc_client_unset_360_mode_updated_cb(mc_client_h client);

/**
 * @brief Sets the callback for monitoring the media controller server's display mode.
 * @details The media controller client which calls this function will receive notifications from all media controller servers.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] callback      The callback to be invoked when the display mode is changed
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_unset_display_mode_updated_cb()
 * @see mc_server_update_display_mode()
 */
int mc_client_set_display_mode_updated_cb(mc_client_h client, mc_display_mode_updated_cb callback, void *user_data);

/**
 * @brief Unsets the callback for monitoring the media controller server's display mode.
 * @details The media controller client which calls this function will not receive notifications from all media controller servers.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_display_mode_updated_cb()
 */
int mc_client_unset_display_mode_updated_cb(mc_client_h client);

/**
 * @brief Sets the callback for monitoring the media controller server's display rotation.
 * @details The media controller client which calls this function will receive notifications from all media controller servers.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] callback      The callback to be invoked when the display rotation is changed
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_unset_display_rotation_updated_cb()
 * @see mc_server_update_display_rotation()
 */
int mc_client_set_display_rotation_updated_cb(mc_client_h client, mc_display_rotation_updated_cb callback, void *user_data);

/**
 * @brief Unsets the callback for monitoring the media controller server's display rotation.
 * @details The media controller client which calls this function will not receive notifications from all media controller servers.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_display_rotation_updated_cb()
 */
int mc_client_unset_display_rotation_updated_cb(mc_client_h client);

/**
 * @brief Sets the callback for receiving the custom event from a media controller servers.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks If client need to reply to the result of the event, use mc_client_send_event_reply()
 *
 * @param[in] client    The handle to media controller client
 * @param[in] callback      The callback to be invoked when the media controller client receives custom event from a media controller server
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
 * @see mc_client_create()
 * @see mc_client_custom_event_received_cb()
 * @see mc_client_unset_custom_event_received_cb()
 * @see mc_client_send_event_reply()
 * @see mc_client_destroy()
 * @see mc_server_send_custom_event()
 */
int mc_client_set_custom_event_received_cb(mc_client_h client, mc_client_custom_event_received_cb callback, void *user_data);

/**
 * @brief Unsets the callback for receiving the custom event from a media controller server.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller server handle by calling mc_server_create().
 * @see mc_client_create()
 * @see mc_client_custom_event_received_cb()
 * @see mc_client_set_custom_event_received_cb()
 * @see mc_client_destroy()
 */
int mc_client_unset_custom_event_received_cb(mc_client_h client);

/**
 * @brief Replies with the result of the requested event to the media controller server.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The media controller client get the @a server_name and @a request_id through the mc_client_custom_event_received_cb(). \n
 *          If @a request_id is not null there, the media controller client should send the reply to the media controller server with the @a request_id. \n
 *          If @a request_id is null, this function will return #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER.\n
 *          If the @a server_name application doesn't support receiving the event reply, this function will return #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP since 5.5.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[in] request_id    The id of the event request, received in the mc_client_set_custom_event_received_cb() function
 * @param[in] result_code    The result code of custom event, #mc_result_code_e since 6.0
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
 * @pre Receive event from media controller servers through mc_client_set_custom_event_received_cb().
 * @see mc_client_create()
 * @see mc_client_set_custom_event_received_cb()
 * @see mc_client_unset_custom_event_received_cb()
 * @see mc_client_destroy()
 * @see mc_server_set_event_reply_received_cb()
 */
int mc_client_send_event_reply(mc_client_h client, const char *server_name, const char *request_id, int result_code, bundle *data);

/**
 * @brief Subscribes media controller server for monitoring status.
 * @details If media controller client subscribe media controller server,
 *              the media controller client receive callback from subscribed media controller server. \n
 *              If media controller client subscribe media controller server one or more,
 *              the media controller client can receive callback from only subscribed media controller server. \n
 *              If you want to subscribe for the all media controller server again,
 *              unset mode update callback and set the callback for the monitoring status again.
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] subscription_type    The subscription type
 * @param[in] server_name    The app_id of the media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create()
 * @pre Set the callback for monitoring status of the media controller server
 * @post Unsubscribe the media controller server for monitoring status by calling mc_client_unsubscribe()
 * @see mc_client_create()
 * @see mc_client_unsubscribe()
 */
int mc_client_subscribe(mc_client_h client, mc_subscription_type_e subscription_type, const char *server_name);

/**
 * @brief Unsubscribes media controller server for monitoring status.
 * @details If media controller client unsubscribe media controller server, \n
 *              the media controller client don't receive callback from unsubscribed media controller server. \n
 *              If media controller client unsubscribe all subscribed media controller server,
 *              the media controller client don't receive callback from all media controller server. \n
 *              After unset and set update callback function is called again, the media controller client can receive callback from all media controller servers.
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] subscription_type    The subscription type
 * @param[in] server_name    The app_id of the media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create()
 * @pre Subscribe the media controller server for monitoring status by calling mc_client_subscribe()
 * @see mc_client_create()
 * @see mc_client_subscribe()
 */
int mc_client_unsubscribe(mc_client_h client, mc_subscription_type_e subscription_type, const char *server_name);

/**
 * @brief Retrieves all subscribed Server.
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] subscription_type    The subscription type
 * @param[in] callback      The callback to be invoked when the list of the subscribed media controller server
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create()
 * @pre Subscribe the media controller server for monitoring status by calling mc_client_subscribe()
 * @see mc_client_create()
 * @see mc_client_subscribe()
 * @see mc_client_unsubscribe()
 */
int mc_client_foreach_server_subscribed(mc_client_h client, mc_subscription_type_e subscription_type, mc_subscribed_server_cb callback, void *user_data);

/**
 * @brief Gets the playback state.
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] playback    The handle to playback
 * @param[out] state      The state of the playback
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The playback update callback should be set with mc_client_set_playback_updated_cb().
 * @see mc_client_set_playback_updated_cb()
 */
int mc_client_get_playback_state(mc_playback_h playback, mc_playback_states_e *state);

/**
 * @brief Gets the playback position.
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] playback    The handle to playback
 * @param[out] position      The position of the playback in milliseconds
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The playback update callback should be set with mc_client_set_playback_updated_cb().
 * @see mc_client_set_playback_updated_cb()
 */
int mc_client_get_playback_position(mc_playback_h playback, unsigned long long *position);

/**
 * @brief Gets the playlist name and index of the media in playlist.
 * @details Gets the playlist name and index of the current playing media in a playlist. \n
 *          If the media controller server didn't set the info, @a playlist_name and @a index will be set to NULL.
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a playlist_name and @a index should be released using free().
 *
 * @param[in] playback    The handle to playback
 * @param[out] playlist_name    The playlist name media included
 * @param[out] index    The index of the media
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre The playback update callback should be set with mc_client_set_playback_updated_cb().
 * @see mc_client_set_playback_updated_cb()
 * @see mc_playlist_foreach_playlist()
 */
int mc_client_get_playlist_item_info(mc_playback_h playback, char **playlist_name, char **index);

/**
 * @brief Gets the playback content type.
 * @details Gets the content type of the current playing media. If there is no content type info, result value is #MC_CONTENT_TYPE_UNDECIDED.
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] playback    The handle to playback
 * @param[out] content_type    The content type of the playback
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre The playback update callback should be set with mc_client_set_playback_updated_cb().
 * @see mc_client_set_playback_updated_cb()
 */
int mc_client_get_playback_content_type(mc_playback_h playback, mc_content_type_e *content_type);

/**
 * @brief Gets the age rating of the content.
 * @details Gets the age rating of the current playing media.
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] playback    The handle to playback
 * @param[out] age_rating    Age rating of the content
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre The playback update callback should be set with mc_client_set_playback_updated_cb().
 * @see mc_client_set_playback_updated_cb()
 */
int mc_client_get_age_rating(mc_playback_h playback, mc_content_age_rating_e *age_rating);

/**
 * @brief Destroys playback.
 * @since_tizen 2.4
 *
 * @param[in] playback    The handle to playback
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see mc_client_set_playback_updated_cb()
 * @see mc_client_get_server_playback_info()
 */
int mc_client_destroy_playback(mc_playback_h playback);

/**
 * @brief Gets the last activated media controller server info, even if the server is currently deactivated.
 * @details The media controller client will get the most recently updated information by the server.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a server_name should be released using free(). \n
 *               If there is no activated media controller server, return value of the server name is NULL. \n
 *               Before a media controller server sets the playback state to #MC_PLAYBACK_STATE_PLAYING, the @a server_state is #MC_SERVER_STATE_NONE. \n
 *               After a media controller server sets the playback state to #MC_PLAYBACK_STATE_PLAYING, the @a server_state is #MC_SERVER_STATE_ACTIVATE. \n
 *               After a media controller server is destroyed, @a server_state is #MC_SERVER_STATE_DEACTIVATE.
 *
 * @param[in] client    The handle of the media controller client
 * @param[out] server_name    The app_id of the latest media controller server
 * @param[out] server_state      The state of the latest media controller server
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 */
int mc_client_get_latest_server_info(mc_client_h client, char **server_name, mc_server_state_e *server_state);

/**
 * @brief Gets the latest playback info of the media controller server.
 * @details The media controller client will get the most recently updated information from activated @a server_name. \n
 *          But if @a server_name is the latest server, client will get the information even though the server is deactivated.
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a playback should be released using mc_client_destroy_playback(). \n
 *                   If there is no playback info, return value of the playback is NULL.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[out] playback    The handle to playback
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_server_update_playback_info()
 */
int mc_client_get_server_playback_info(mc_client_h client, const char *server_name, mc_playback_h *playback);

/**
 * @brief Gets the latest metadata of the media controller server.
 * @details The media controller client will get the most recently updated information from activated @a server_name. \n
 *          But if @a server_name is the latest server, client will get the information even though the server is deactivated.
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a metadata should be released using mc_metadata_destroy(). \n
 *                   If there is no metadata, return value of the @a metadata is NULL.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[out] metadata    The handle to metadata
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_server_update_metadata()
 * @see mc_metadata_get()
 */
int mc_client_get_server_metadata(mc_client_h client, const char *server_name, mc_metadata_h *metadata);

/**
 * @brief Gets the latest shuffle mode of the media controller server.
 * @details The media controller client will get the most recently updated information from activated @a server_name. \n
 *          But if @a server_name is the latest server, client will get the information even though the server is deactivated.
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks If there is no shuffle mode info, return value is #MC_SHUFFLE_MODE_OFF.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[out] mode    The info of the latest shuffle mode
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_server_update_shuffle_mode()
 */
int mc_client_get_server_shuffle_mode(mc_client_h client, const char *server_name, mc_shuffle_mode_e *mode);

/**
 * @brief Gets the latest repeat mode of the media controller server.
 * @details The media controller client will get the most recently updated information from activated @a server_name. \n
 *          But if @a server_name is the latest server, client will get the information even though the server is deactivated.
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks If there is no repeat mode info, return value is #MC_REPEAT_MODE_OFF.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[out] mode    The info of the latest shuffle mode
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_server_update_repeat_mode()
 */
int mc_client_get_server_repeat_mode(mc_client_h client, const char *server_name, mc_repeat_mode_e *mode);

/**
 * @brief Gets the latest icon URI of the media controller server.
 * @details The media controller client will get the icon URI of activated @a server_name. \n
 *          But if @a server_name is the latest server, client will get the information even though the server is deactivated.
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a uri should be released using free(). \n
 *                   If there is no URI info, return value of the uri is NULL.
 *
 * @param[in] client The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[out] uri    The icon URI
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_server_set_icon()
 */
int mc_client_get_server_icon(mc_client_h client, const char *server_name, char **uri);

/**
 * @brief Gets the latest subtitles display status of the media controller server.
 * @details The media controller client will get the most recently updated information from activated @a server_name. \n
 *          But if @a server_name is the latest server, client will get the information even though the server is deactivated.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[out] enabled    The status of the latest subtitles display
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_server_update_subtitles_enabled()
 */
int mc_client_get_server_subtitles_enabled(mc_client_h client, const char *server_name, bool *enabled);

/**
 * @brief Gets the latest 360 mode display status of the media controller server.
 * @details The media controller client will get the most recently updated information from activated @a server_name. \n
 *          But if @a server_name is the latest server, client will get the information even though the server is deactivated.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[out] enabled    The status of the latest 360 mode display
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_server_update_360_mode_enabled()
 */
int mc_client_get_server_360_mode_enabled(mc_client_h client, const char *server_name, bool *enabled);

/**
 * @brief Gets the latest display mode of the media controller server.
 * @details The media controller client will get the most recently updated information from activated @a server_name. \n
 *          But if @a server_name is the latest server, client will get the information even though the server is deactivated.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The default display mode is #MC_DISPLAY_MODE_FULL_SCREEN.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[out] mode    The info of the latest display mode
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_server_update_display_mode()
 */
int mc_client_get_server_display_mode(mc_client_h client, const char *server_name, mc_display_mode_e *mode);

/**
 * @brief Gets the latest display rotation of the media controller server.
 * @details The media controller client will get the most recently updated information from activated @a server_name. \n
 *          But if @a server_name is the latest server, client will get the information even though the server is deactivated.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The default display rotation is #MC_DISPLAY_ROTATION_NONE.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[out] rotation    The info of the latest display rotation
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_server_update_display_rotation()
 */
int mc_client_get_server_display_rotation(mc_client_h client, const char *server_name, mc_display_rotation_e *rotation);


/**
 * @brief Gets the latest playback ability of the media controller server.
 * @details The media controller client will get the most recently updated information from activated @a server_name. \n
 *          But if @a server_name is the latest server, client will get the information even though the server is deactivated.
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a ability should be released using mc_playback_ability_destroy().\n
 *              If the latest media controller server is deactivated, this function will return #MC_ABILITY_SUPPORTED_UNDECIDED.\n
 *
 * @param[in] client The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[out] ability    The handle to ability
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @pre Get a server name handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_get_latest_server_info()
 * @see mc_client_foreach_server()
 * @see mc_playback_action_is_supported()
 * @see mc_playback_ability_destroy()
 * @see mc_server_update_playback_ability()
 */
int mc_client_get_server_playback_ability(mc_client_h client, const char *server_name, mc_playback_ability_h *ability);

/**
 * @brief Gets the latest ability support of the media controller server.
 * @details The media controller client will get the most recently updated information from activated @a server_name. \n
 *          But if @a server_name is the latest server, client will get the information even though the server is deactivated.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks If the server's ability support is not set, the result value is #MC_ABILITY_SUPPORTED_UNDECIDED. \n
 *              It means you can send the corresponding command to the server, but it's not guaranteed that the server can handle it. \n
 *              If the latest media controller server is deactivated, this function will return #MC_ABILITY_SUPPORTED_UNDECIDED.
 *
 * @param[in] client The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[in] ability The ability
 * @param[out] support    The support value of the ability
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @pre Get a server name handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_get_latest_server_info()
 * @see mc_client_foreach_server()
 * @see mc_client_get_server_playback_ability()
 * @see mc_client_get_server_display_mode_ability()
 * @see mc_client_get_server_display_rotation_ability()
 * @see mc_server_set_ability_support()
 */
int mc_client_get_server_ability_support(mc_client_h client, const char *server_name, mc_ability_e ability, mc_ability_support_e *support);

/**
 * @brief Gets the latest supported display mode of the media controller server.
 * @details The media controller client will get the most recently updated information from activated @a server_name. \n
 *          But if @a server_name is the latest server, client will get the information even though the server is deactivated.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks If a display mode is not present in the @a supported_modes, then the server's support for this mode is #MC_ABILITY_SUPPORTED_NO and you can't send the corresponding command to the server. \n
 *              If the latest media controller server is deactivated, this function will return #MC_ABILITY_SUPPORTED_NO.
 *
 * @param[in] client The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[out] supported_modes    The supported display mode, values of #mc_display_mode_e combined with bitwise 'or'.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @pre Get a server name handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_get_latest_server_info()
 * @see mc_client_foreach_server()
 * @see mc_client_get_server_playback_ability()
 * @see mc_client_get_server_ability_support()
 * @see mc_client_get_server_display_rotation_ability()
 * @see mc_server_set_display_mode_ability()
 */
int mc_client_get_server_display_mode_ability(mc_client_h client, const char *server_name, unsigned int *supported_modes);

/**
 * @brief Gets the latest supported display rotation of the media controller server.
 * @details The media controller client will get the most recently updated information from activated @a server_name. \n
 *          But if @a server_name is the latest server, client will get the information even though the server is deactivated.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks If a display rotation is not present in the @a supported_rotations, then the server's support for this mode is #MC_ABILITY_SUPPORTED_NO and you can't send the corresponding command to the server. \n
 *              If the latest media controller server is deactivated, this function will return #MC_ABILITY_SUPPORTED_NO.
 *
 * @param[in] client The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[out] supported_rotations    The supported display rotation, values of #mc_display_rotation_e combined with bitwise 'or'
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @pre Get a server name handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_get_latest_server_info()
 * @see mc_client_foreach_server()
 * @see mc_client_get_server_playback_ability()
 * @see mc_client_get_server_ability_support()
 * @see mc_client_get_server_display_mode_ability()
 * @see mc_server_set_display_rotation_ability()
 */
int mc_client_get_server_display_rotation_ability(mc_client_h client, const char *server_name, unsigned int *supported_rotations);

/**
 * @brief Retrieves all activated servers.
 * @details This function gets all created media controller servers.
 *          The callback function will be invoked for every created media controller server.
 *          If there are no media controller servers, the callback will not be invoked.
 * @since_tizen 2.4
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] callback      The callback function to be invoked, will be called for each server
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_server_create()
 */
int mc_client_foreach_server(mc_client_h client, mc_activated_server_cb callback, void *user_data);

/**
 * @brief Sends the playback action command to server.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a request_id should be released using free().\n
 *          If the @a request_id is null, the server will not send the reply of the command.\n
 *          If the @a server_name application doesn't support the playback action command, this function will return #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP since 5.5.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[in] action   The playback action command to send to the media controller server
 * @param[out] request_id   The id of the command request, it will be passed to the mc_cmd_reply_received_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP Limited by server application
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_get_server_playback_info()
 * @see mc_client_set_cmd_reply_received_cb()
 * @see mc_server_set_playback_action_cmd_received_cb()
 */
int mc_client_send_playback_action_cmd(mc_client_h client, const char *server_name, mc_playback_action_e action, char **request_id);

/**
 * @brief Sends the playback position command to server.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a request_id should be released using free().\n
 *          If the @a request_id is null, the server will not send the reply of the command.\n
 *          If the @a server_name application doesn't support the playback position command, this function will return #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP since 5.5.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[in] position   The position of the playback in milliseconds to send to media controller server
 * @param[out] request_id   The id of the command request, it will be passed to the mc_cmd_reply_received_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP Limited by server application
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_get_server_playback_info()
 * @see mc_client_set_cmd_reply_received_cb()
 * @see mc_server_set_playback_position_cmd_received_cb()
 */
int mc_client_send_playback_position_cmd(mc_client_h client, const char *server_name, unsigned long long position, char **request_id);

/**
 * @brief Sends the shuffle mode command to server.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a request_id should be released using free().\n
 *          If the @a request_id is null, the server will not send the reply of the command.\n
 *          If the @a server_name application doesn't support the shuffle mode, this function will return #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP since 5.5.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[in] shuffle_mode   The shuffle mode to send to media controller server
 * @param[out] request_id   The id of the command request, it will be passed to the mc_cmd_reply_received_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP Limited by server application
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_get_server_playback_info()
 * @see mc_client_set_cmd_reply_received_cb()
 * @see mc_server_set_shuffle_mode_cmd_received_cb()
 */
int mc_client_send_shuffle_mode_cmd(mc_client_h client, const char *server_name, mc_shuffle_mode_e shuffle_mode, char **request_id);

/**
 * @brief Sends the repeat mode command to server.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a request_id should be released using free().\n
 *          If the @a request_id is null, the server will not send the reply of the command.\n
 *          If the @a server_name application doesn't support the repeat mode, this function will return #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP since 5.5.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[in] repeat_mode   The repeat mode to send to media controller server
 * @param[out] request_id   The id of the command request, it will be passed to the mc_cmd_reply_received_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP Limited by server application
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_get_server_playback_info()
 * @see mc_client_set_cmd_reply_received_cb()
 * @see mc_server_set_repeat_mode_cmd_received_cb()
 */
int mc_client_send_repeat_mode_cmd(mc_client_h client, const char *server_name, mc_repeat_mode_e repeat_mode, char **request_id);

/**
 * @brief Sends the playlist command to server.
 * @details The media controller client can send "index" of the playlist to the media controller server with playback action and position.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a request_id should be released using free().\n
 *          If the @a request_id is null, the server will not send the reply of the command.\n
 *          If the @a server_name application doesn't support the playlist command, this function will return #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP since 5.5.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[in] playlist_name    The playlist name of the server
 * @param[in] index    The index of the media in playlist to send to the media controller server
 * @param[in] action    The playback action command to send to the media controller server
 * @param[in] position    The position of the playback in milliseconds to send to media controller server
 * @param[out] request_id   The id of the command request, it will be passed to the mc_cmd_reply_received_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP Limited by server application
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_cmd_reply_received_cb()
 * @see mc_server_set_playlist_cmd_received_cb()
 */
int mc_client_send_playlist_cmd(mc_client_h client, const char *server_name, const char *playlist_name, const char *index, mc_playback_action_e action, unsigned long long position, char **request_id);

/**
 * @brief Sends the custom command to server.
 * @details If the server sends the result of the command, the media controller client will get the result of the custom command by mc_cmd_reply_received_cb() callback.
 * @since_tizen 4.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a request_id should be released using free().\n
 *          If the @a request_id is null, the server will not send the reply of the command.\n
 *          If the @a server_name application doesn't support the custom command, this function will return #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP since 5.5.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[in] command      The command to be sent
 * @param[in] data      The extra data
 * @param[out] request_id   The id of the command request, it will be passed to the mc_cmd_reply_received_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP Limited by server application
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_cmd_reply_received_cb()
 * @see mc_server_set_custom_cmd_received_cb()
 */
int mc_client_send_custom_cmd(mc_client_h client, const char *server_name, const char *command, bundle *data, char **request_id);

/**
 * @brief Sends the search command to server.
 * @details If the server sends the result of the command, the media controller client will get the result of the search command by mc_cmd_reply_received_cb() callback.
 * @since_tizen 5.0
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a request_id should be released using free().\n
 *          If the @a request_id is null, the server will not send the reply of the command.\n
 *          If the @a server_name application doesn't support the search command, this function will return #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP since 5.5.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[in] search      The search handle to be sent
 * @param[out] request_id   The id of the command request, it will be passed to the mc_cmd_reply_received_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP Limited by server application
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @pre Create search handle and set data by calling mc_search_create() and mc_search_set_condition().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_cmd_reply_received_cb()
 * @see mc_search_create()
 * @see mc_search_set_condition()
 * @see mc_server_set_search_cmd_received_cb()
 */
int mc_client_send_search_cmd(mc_client_h client, const char *server_name, mc_search_h search, char **request_id);

/**
 * @brief Sends the subtitles command to server.
 * @details If the server sends the result of the command, the media controller client will get the result of the subtitles command by mc_cmd_reply_received_cb() callback.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a request_id should be released using free().\n
 *          If the @a request_id is null, the server will not send the reply of the command.\n
 *          If the @a server_name application doesn't support the subtitles command, this function will return #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[in] enable      The request status to send to media controller server
 * @param[out] request_id   The id of the command request, it will be passed to the mc_cmd_reply_received_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP Limited by server application
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_cmd_reply_received_cb()
 * @see mc_server_set_subtitles_cmd_received_cb()
 */
int mc_client_send_subtitles_cmd(mc_client_h client, const char *server_name, bool enable, char **request_id);

/**
 * @brief Sends the 360 mode command to server.
 * @details If the server sends the result of the command, the media controller client will get the result of the 360 mode command by mc_cmd_reply_received_cb() callback.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a request_id should be released using free().\n
 *          If the @a request_id is null, the server will not send the reply of the command.\n
 *          If the @a server_name application doesn't support the 360 mode command, this function will return #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[in] enable      The request status to send to media controller server
 * @param[out] request_id   The id of the command request, it will be passed to the mc_cmd_reply_received_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP Limited by server application
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_cmd_reply_received_cb()
 * @see mc_server_set_360_mode_cmd_received_cb()
 */
int mc_client_send_360_mode_cmd(mc_client_h client, const char *server_name, bool enable, char **request_id);

/**
 * @brief Sends the display mode command to server.
 * @details If the server sends the result of the command, the media controller client will get the result of the display mode command by mc_cmd_reply_received_cb() callback.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a request_id should be released using free().\n
 *          If the @a request_id is null, the server will not send the reply of the command.\n
 *          If the @a server_name application doesn't support the display mode command, this function will return #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[in] mode      The display mode to send to media controller server
 * @param[out] request_id   The id of the command request, it will be passed to the mc_cmd_reply_received_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP Limited by server application
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_cmd_reply_received_cb()
 * @see mc_server_set_display_mode_cmd_received_cb()
 */
int mc_client_send_display_mode_cmd(mc_client_h client, const char *server_name, mc_display_mode_e mode, char **request_id);

/**
 * @brief Sends the display rotation command to server.
 * @details If the server sends the result of the command, the media controller client will get the result of the display rotation command by mc_cmd_reply_received_cb() callback.
 * @since_tizen 5.5
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediacontroller.client
 *
 * @remarks The @a request_id should be released using free().\n
 *          If the @a request_id is null, the server will not send the reply of the command.\n
 *          If the @a server_name application doesn't support the display rotation command, this function will return #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP.
 *
 * @param[in] client    The handle of the media controller client
 * @param[in] server_name    The app_id of the media controller server
 * @param[in] rotation      The display rotation to send to media controller server
 * @param[out] request_id   The id of the command request, it will be passed to the mc_cmd_reply_received_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP Limited by server application
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 * @see mc_client_destroy()
 * @see mc_client_set_cmd_reply_received_cb()
 * @see mc_server_set_display_rotation_cmd_received_cb()
 */
int mc_client_send_display_rotation_cmd(mc_client_h client, const char *server_name, mc_display_rotation_e rotation, char **request_id);

/**
 * @brief Destroys client.
 * @since_tizen 2.4
 * @param[in] client The handle of the media controller client
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller client handle by calling mc_client_create().
 * @see mc_client_create()
 */
int mc_client_destroy(mc_client_h client);


/**
* @}
*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MULTIMEDIA_MEDIA_CONTROLLER_CLIENT_H__ */
