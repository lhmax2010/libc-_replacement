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

#ifndef __TIZEN_MULTIMEDIA_MEDIA_CONTROLLER_TYPE_H__
#define __TIZEN_MULTIMEDIA_MEDIA_CONTROLLER_TYPE_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_controller_type.h
 * @brief This file contains API related to media controller enumerations and types. \n
 *        Listed APIs are called when client send and receive event.  \n
 */

/**
* @addtogroup CAPI_MEDIA_CONTROLLER_MODULE
 * @{
 */

#define MEDIA_CONTROLLER_ERROR_CLASS	TIZEN_ERROR_MEDIA_CONTROLLER

/**
 * @brief The structure type for the media server handle.
 * @since_tizen 2.4
 */
typedef void *mc_server_h;

/**
 * @brief The structure type for the media client handle.
 * @since_tizen 2.4
 */
typedef void *mc_client_h;

/**
 * @brief The structure type for the media controller playback handle.
 * @since_tizen 2.4
 */
typedef void *mc_playback_h;

/**
 * @brief The structure type for the media controller metadata handle.
 * @since_tizen 2.4
 */
typedef void *mc_metadata_h;

/**
 * @brief The structure type for the media controller playlist handle.
 * @since_tizen 4.0
 */
typedef void *mc_playlist_h;

/**
 * @brief The structure type for the media controller ability handle.
 * @since_tizen 5.0
 */
typedef void *mc_playback_ability_h;

/**
 * @brief The structure type for the media controller search handle.
 * @since_tizen 5.0
 */
typedef void *mc_search_h;

/**
 * @brief Enumeration for the media controller error.
 * @since_tizen 2.4
 */
typedef enum {
	MEDIA_CONTROLLER_ERROR_NONE						= TIZEN_ERROR_NONE,						/**< Successful */
	MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER		= TIZEN_ERROR_INVALID_PARAMETER,		/**< Invalid parameter */
	MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY			= TIZEN_ERROR_OUT_OF_MEMORY,			/**< Out of memory */
	MEDIA_CONTROLLER_ERROR_INVALID_OPERATION		= TIZEN_ERROR_INVALID_OPERATION,		/**< Invalid Operation */
	MEDIA_CONTROLLER_ERROR_FILE_NO_SPACE_ON_DEVICE		= TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE,	/**< No space left on device */
	MEDIA_CONTROLLER_ERROR_PERMISSION_DENIED		= TIZEN_ERROR_PERMISSION_DENIED,		/**< Permission denied */
	MEDIA_CONTROLLER_ERROR_ABILITY_LIMITED_BY_SERVER_APP	= MEDIA_CONTROLLER_ERROR_CLASS | 0x01,		/**< Limited by server application (since 5.5) */
} mc_error_e;

/**
 * @brief Enumeration for the media controller server state.
 * @since_tizen 2.4
 *
 * @see mc_client_get_latest_server_info()
 * @see mc_server_state_updated_cb()
 */
typedef enum {
	MC_SERVER_STATE_NONE = 0,					/**< None state*/
	MC_SERVER_STATE_ACTIVATE,					/**< Activate state*/
	MC_SERVER_STATE_DEACTIVATE,					/**< Deactivate state*/
} mc_server_state_e;

/**
 * @brief Enumeration for the media meta info.
 * @since_tizen 2.4
 *
 * @see mc_server_set_metadata()
 * @see mc_server_add_item_to_playlist()
 * @see mc_metadata_get()
 */
typedef enum {
	MC_META_MEDIA_TITLE = 0,			/**< Title */
	MC_META_MEDIA_ARTIST,				/**< Artist */
	MC_META_MEDIA_ALBUM,			/**< Album */
	MC_META_MEDIA_AUTHOR,			/**< Author */
	MC_META_MEDIA_GENRE,				/**< Genre */
	MC_META_MEDIA_DURATION,			/**< Duration */
	MC_META_MEDIA_DATE,				/**< Date */
	MC_META_MEDIA_COPYRIGHT,		/**< Copyright */
	MC_META_MEDIA_DESCRIPTION,		/**< Description */
	MC_META_MEDIA_TRACK_NUM,		/**< Track Number */
	MC_META_MEDIA_PICTURE,			/**< Picture. Album Art */
	MC_META_MEDIA_SEASON,			/**< Season (Since 5.5) */
	MC_META_MEDIA_EPISODE,			/**< Episode (Since 5.5) */
	MC_META_MEDIA_RESOLUTION,		/**< Content resolution (Since 5.5) */
} mc_meta_e;

/**
 * @brief Enumeration for the media playback state of the media controller server.
 * @since_tizen 2.4
 *
 * @see mc_server_set_playback_state()
 * @see mc_client_get_playback_state()
 */
typedef enum {
	MC_PLAYBACK_STATE_NONE = 0,				/**< None */
	MC_PLAYBACK_STATE_PLAYING,				/**< Playing */
	MC_PLAYBACK_STATE_PAUSED,				/**< Paused */
	MC_PLAYBACK_STATE_STOPPED,				/**< Stopped */
	MC_PLAYBACK_STATE_MOVING_TO_NEXT = 8,		/**< Moving to the next item (Since 4.0)  */
	MC_PLAYBACK_STATE_MOVING_TO_PREVIOUS,	/**< Moving to the previous item (Since 4.0)  */
	MC_PLAYBACK_STATE_FAST_FORWARDING,		/**< Fast forwarding (Since 4.0)  */
	MC_PLAYBACK_STATE_REWINDING,				/**< Rewinding (Since 4.0)  */
	MC_PLAYBACK_STATE_CONNECTING,			/**< Connecting (Since 6.0)	*/
	MC_PLAYBACK_STATE_BUFFERING,			/**< Buffering (Since 6.0)  */
	MC_PLAYBACK_STATE_ERROR,				/**< Error (Since 6.0)	*/
} mc_playback_states_e;

/**
 * @brief Enumeration for the media playback action.
 * @details Media controller clients can control media controller server's playback by calling mc_client_send_playback_action_cmd() or mc_client_send_playlist_cmd(). \n
 *          Media controller servers have to set proper playback ability by using mc_server_set_playback_ability().
 * @since_tizen 4.0
 *
 * @see mc_server_set_playback_ability()
 * @see mc_client_send_playback_action_cmd()
 * @see mc_client_send_playlist_cmd()
 * @see mc_playback_action_is_supported()
 * @see mc_server_playback_action_cmd_received_cb()
 * @see mc_server_playlist_cmd_received_cb()
 */
typedef enum {
	MC_PLAYBACK_ACTION_PLAY,				/**< Play */
	MC_PLAYBACK_ACTION_PAUSE,			/**< Pause */
	MC_PLAYBACK_ACTION_STOP,				/**< Stop */
	MC_PLAYBACK_ACTION_NEXT,				/**< Next item */
	MC_PLAYBACK_ACTION_PREV,				/**< Previous item */
	MC_PLAYBACK_ACTION_FAST_FORWARD,	/**< Fast forward */
	MC_PLAYBACK_ACTION_REWIND,			/**< Rewind */
	MC_PLAYBACK_ACTION_TOGGLE_PLAY_PAUSE,		/**< Play/Pause toggle */
} mc_playback_action_e;

/**
 * @brief Enumeration for the shuffle mode of the media controller server.
 * @since_tizen 2.4
 *
 * @see mc_server_update_shuffle_mode()
 * @see mc_server_shuffle_mode_cmd_received_cb()
 * @see mc_client_get_server_shuffle_mode()
 * @see mc_client_send_shuffle_mode_cmd()
 * @see mc_shuffle_mode_updated_cb()
 */
typedef enum {
	MC_SHUFFLE_MODE_ON = 0,				/**< Shuffle mode on */
	MC_SHUFFLE_MODE_OFF,					/**< Shuffle mode off */
} mc_shuffle_mode_e;

/**
 * @brief Enumeration for the repeat mode of the media controller server.
 * @since_tizen 2.4
 *
 * @see mc_server_update_repeat_mode()
 * @see mc_server_repeat_mode_cmd_received_cb()
 * @see mc_client_get_server_repeat_mode()
 * @see mc_client_send_repeat_mode_cmd()
 * @see mc_repeat_mode_updated_cb()
 */
typedef enum {
	MC_REPEAT_MODE_ON = 0,				/**< Repeat mode on for all media*/
	MC_REPEAT_MODE_OFF,					/**< Repeat mode off */
	MC_REPEAT_MODE_ONE_MEDIA,			/**< Repeat mode on for one media (Since 4.0) */
} mc_repeat_mode_e;

/**
 * @brief Enumeration for the subscription type of the media controller client.
 * @since_tizen 2.4
 *
 * @see mc_client_subscribe()
 * @see mc_client_unsubscribe()
 * @see mc_client_foreach_server_subscribed()
 */
typedef enum {
	MC_SUBSCRIPTION_TYPE_SERVER_STATE = 0,					/**< Server state */
	MC_SUBSCRIPTION_TYPE_PLAYBACK,						/**< Playback */
	MC_SUBSCRIPTION_TYPE_METADATA,						/**< Metadata */
	MC_SUBSCRIPTION_TYPE_SHUFFLE_MODE,					/**< Shuffle mode */
	MC_SUBSCRIPTION_TYPE_REPEAT_MODE,					/**< Repeat mode */
	MC_SUBSCRIPTION_TYPE_PLAYLIST,						/**< Playlist (Since 4.0) */
	MC_SUBSCRIPTION_TYPE_PLAYBACK_ABILITY,				/**< Playback ability (Since 5.0) */
	MC_SUBSCRIPTION_TYPE_ABILITY_SUPPORT = 9,				/**< Ability support (Since 5.5) */
	MC_SUBSCRIPTION_TYPE_DISPLAY_MODE_ABILITY,			/**< Display mode ability (Since 5.5) */
	MC_SUBSCRIPTION_TYPE_DISPLAY_ROTATION_ABILITY,		/**< Display rotation ability (Since 5.5) */
} mc_subscription_type_e;

/**
 * @brief Enumeration for the playlist update mode.
 * @details Media controller clients can know the playlist change status of the media controller server by calling mc_client_set_playlist_updated_cb(). \n
 * @since_tizen 4.0
 *
 * @see mc_client_set_playlist_updated_cb()
 * @see mc_playlist_updated_cb()
 * @see mc_server_update_playlist_done()
 * @see mc_server_delete_playlist()
 */
typedef enum {
	MC_PLAYLIST_UPDATED,	/**< Create or Update playlist */
	MC_PLAYLIST_REMOVED,	/**< Remove playlist */
} mc_playlist_update_mode_e;

/**
 * @brief Enumeration for the content type of the media content by the media controller server.
 * @details Media controller clients can use this to know the playback status of the media controller server. \n
 *          And media controller clients can also use to search for the content provided by the media controller server.
 * @since_tizen 5.0
 *
 * @see mc_server_set_playback_content_type()
 * @see mc_client_get_playback_content_type()
 * @see mc_search_set_condition()
 */
typedef enum {
	MC_CONTENT_TYPE_IMAGE = 0,	/**< Image content type */
	MC_CONTENT_TYPE_VIDEO,	/**< Video content type */
	MC_CONTENT_TYPE_MUSIC,	/**< Music content type */
	MC_CONTENT_TYPE_OTHER,	/**< Other content type */
	MC_CONTENT_TYPE_UNDECIDED,	/**< Not decided */
} mc_content_type_e;

/**
 * @brief Enumeration for the content age rating of the media content by the media controller server.
 * @since_tizen 5.0
 *
 * @see mc_server_set_content_age_rating()
 * @see mc_client_get_age_rating()
 */
typedef enum {
	MC_CONTENT_RATING_ALL = 0, 		/**< Suitable for all ages */
	MC_CONTENT_RATING_1_PLUS = 1, 	/**< Suitable for ages 1 and up*/
	MC_CONTENT_RATING_2_PLUS = 2, 	/**< Suitable for ages 2 and up*/
	MC_CONTENT_RATING_3_PLUS = 3, 	/**< Suitable for ages 3 and up*/
	MC_CONTENT_RATING_4_PLUS = 4, 	/**< Suitable for ages 4 and up*/
	MC_CONTENT_RATING_5_PLUS = 5, 	/**< Suitable for ages 5 and up*/
	MC_CONTENT_RATING_6_PLUS = 6, 	/**< Suitable for ages 6 and up*/
	MC_CONTENT_RATING_7_PLUS = 7, 	/**< Suitable for ages 7 and up*/
	MC_CONTENT_RATING_8_PLUS = 8, 	/**< Suitable for ages 8 and up*/
	MC_CONTENT_RATING_9_PLUS = 9, 	/**< Suitable for ages 9 and up*/
	MC_CONTENT_RATING_10_PLUS = 10, 	/**< Suitable for ages 10 and up*/
	MC_CONTENT_RATING_11_PLUS = 11, 	/**< Suitable for ages 11 and up*/
	MC_CONTENT_RATING_12_PLUS = 12, 	/**< Suitable for ages 12 and up*/
	MC_CONTENT_RATING_13_PLUS = 13, 	/**< Suitable for ages 13 and up*/
	MC_CONTENT_RATING_14_PLUS = 14, 	/**< Suitable for ages 14 and up*/
	MC_CONTENT_RATING_15_PLUS = 15, 	/**< Suitable for ages 15 and up*/
	MC_CONTENT_RATING_16_PLUS = 16, 	/**< Suitable for ages 16 and up*/
	MC_CONTENT_RATING_17_PLUS = 17, 	/**< Suitable for ages 17 and up*/
	MC_CONTENT_RATING_18_PLUS = 18, 	/**< Suitable for ages 18 and up*/
	MC_CONTENT_RATING_19_PLUS = 19, 	/**< Suitable for ages 19 and up*/
} mc_content_age_rating_e;

/**
 * @brief Enumeration for the support of the ability by the media controller server.
 * @since_tizen 5.0
 *
 * @remarks The media controller server should set the proper abilities. \n
 *          Only then can media controller clients send the appropriate control commands.
 *
 * @see mc_server_set_playback_ability()
 * @see mc_server_set_ability_support()
 * @see mc_server_set_display_mode_ability()
 * @see mc_server_set_display_rotation_ability()
 * @see mc_client_get_server_ability_support()
 * @see mc_ability_support_updated_cb()
 * @see mc_playback_action_is_supported()
 */
typedef enum {
	MC_ABILITY_SUPPORTED_YES = 0,		/**< Supported */
	MC_ABILITY_SUPPORTED_NO,			/**< Not supported */
	MC_ABILITY_SUPPORTED_UNDECIDED,	/**< Not decided */
} mc_ability_support_e;

/**
 * @brief Enumeration for the ability of the media controller server.
 * @since_tizen 5.5
 *
 * @see mc_server_set_ability_support()
 * @see mc_client_get_server_ability_support()
 * @see mc_ability_support_updated_cb()
 */
typedef enum {
	MC_ABILITY_SHUFFLE,				/**< Shuffle */
	MC_ABILITY_REPEAT,				/**< Repeat */
	MC_ABILITY_PLAYBACK_POSITION,	/**< Playback Position */
	MC_ABILITY_PLAYLIST,			/**< Playlist */
	MC_ABILITY_CLIENT_CUSTOM,		/**< Custom command from a client */
	MC_ABILITY_SEARCH,				/**< Search */
	MC_ABILITY_SUBTITLES,			/**< Subtitles display*/
	MC_ABILITY_360_MODE,			/**< 360 content mode display*/
} mc_ability_e;

/**
 * @brief Enumeration for the search category.
 * @details Media controller clients can use this to search for the content provided by the media controller server.
 * @since_tizen 5.0
 *
 * @see mc_search_set_condition()
 */
typedef enum {
	MC_SEARCH_NO_CATEGORY = 0,	/**< No search category */
	MC_SEARCH_TITLE,		/**< Search by content title */
	MC_SEARCH_ARTIST,		/**< Search by content artist */
	MC_SEARCH_ALBUM,		/**< Search by content album */
	MC_SEARCH_GENRE,		/**< Search by content genre */
	MC_SEARCH_TPO,		/**< Search by Time Place Occasion */
} mc_search_category_e;

/**
 * @brief Enumeration for the display mode of the media controller server.
 * @since_tizen 5.5
 *
 * @see mc_server_update_display_mode()
 * @see mc_server_display_mode_cmd_received_cb()
 * @see mc_client_get_server_display_mode()
 * @see mc_client_send_display_mode_cmd()
 * @see mc_display_mode_updated_cb()
 */
typedef enum {
	MC_DISPLAY_MODE_LETTER_BOX = 1 << 0,		/**< Letter box */
	MC_DISPLAY_MODE_ORIGIN_SIZE = 1 << 1,		/**< Origin size */
	MC_DISPLAY_MODE_FULL_SCREEN = 1 << 2,	/**< Full-screen */
	MC_DISPLAY_MODE_CROPPED_FULL = 1 << 3,	/**< Cropped full-screen */
} mc_display_mode_e;

/**
 * @brief Enumeration for the display rotation of the media controller server.
 * @since_tizen 5.5
 *
 * @see mc_server_update_display_rotation()
 * @see mc_server_display_rotation_cmd_received_cb()
 * @see mc_client_get_server_display_rotation()
 * @see mc_client_send_display_rotation_cmd()
 * @see mc_display_rotation_updated_cb()
 */
typedef enum {
	MC_DISPLAY_ROTATION_NONE = 1 << 0,	/**< Display is not rotated */
	MC_DISPLAY_ROTATION_90 = 1 << 1,		/**< Display is rotated 90 degrees */
	MC_DISPLAY_ROTATION_180 = 1 << 2,		/**< Display is rotated 180 degrees */
	MC_DISPLAY_ROTATION_270 = 1 << 3,		/**< Display is rotated 270 degrees */
} mc_display_rotation_e;


/**
 * @brief The result codes of the reply for the command or the event.
 * @since_tizen 6.0
 *
 * @see mc_cmd_reply_received_cb()
 * @see mc_client_send_event_reply()
 * @see mc_server_event_reply_received_cb()
 * @see mc_server_send_cmd_reply()
 */
typedef enum {
	MC_RESULT_CODE_SUCCESS,						/**< The command or the event has been successfully completed. */
	MC_RESULT_CODE_ALREADY_DONE = 200,			/**< The command or the event had already been completed. */
	MC_RESULT_CODE_ABORTED = 300,				/**< The command or the event is aborted by some external event (e.g. aborted play command by incoming call). */
	MC_RESULT_CODE_DENIED,						/**< The command or the event is denied due to application policy (e.g. cannot rewind in recording). */
	MC_RESULT_CODE_NOT_SUPPORTED,				/**< The command or the event is not supported. */
	MC_RESULT_CODE_INVALID,						/**< The command or the event is out of supported range or the limit is reached. */
	MC_RESULT_CODE_TIMEOUT = 400,				/**< Timeout has occurred. */
	MC_RESULT_CODE_APP_FAILED,					/**< The application has failed. */
	MC_RESULT_CODE_NO_MEDIA,					/**< The command or the event has failed because the application has no media. */
	MC_RESULT_CODE_NO_AUDIO_OUTPUT_DEVICE,		/**< The command or the event has failed because there is no audio output device. */
	MC_RESULT_CODE_NO_PEER,						/**< The command or the event has failed because there is no peer. */
	MC_RESULT_CODE_NETWORK_FAILED = 500,		/**< The network has failed. */
	MC_RESULT_CODE_NO_ACCOUNT = 600,			/**< The application needs to have an account to which it's logged in. */
	MC_RESULT_CODE_LOGIN_FAILED,				/**< The application could not log in. */
	MC_RESULT_CODE_UNKNOWN = 0x7FFFFFFF,		/**< Unknown error. */
} mc_result_code_e;

/**
 * @}
 */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MULTIMEDIA_MEDIA_CONTROLLER_TYPE_H__ */
