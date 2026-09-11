/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __TIZEN_NETWORK_BLUETOOTH_TYPE_EXTENSION_H__
#define __TIZEN_NETWORK_BLUETOOTH_TYPE_EXTENSION_H__

#include <glib.h>

 #ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @file bluetooth_type_extension.h
 */

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Enumerations for the call handling event
 * @since_tizen 2.3
 */
typedef enum {
	BT_HF_CALL_HANDLING_EVENT_ANSWER = 0x00,  /**< Request to answer an incoming call */
	BT_HF_CALL_HANDLING_EVENT_RELEASE,  /**< Request to release a call */
	BT_HF_CALL_HANDLING_EVENT_REJECT,  /**< Request to reject an incoming call */
	BT_HF_CALL_HANDLING_EVENT_RING,  /**< Request of ringing call */
	BT_HF_CALL_HANDLING_EVENT_CALL_STARTED,  /**< Request of Call started */
	BT_HF_CALL_HANDLING_EVENT_CALL_ENDED,  /**< Request of Call Ended */
	BT_HF_CALL_HANDLING_EVENT_VOICE_RECOGNITION_ENABLED,  /**< Request of voice recognition enabled */
	BT_HF_CALL_HANDLING_EVENT_VOICE_RECOGNITION_DISABLED,  /**< Request of voice recognition disabled */
	BT_HF_CALL_HANDLING_EVENT_VENDOR_DEP_CMD,  /**< Request of Vendor command */
	BT_HF_CALL_HANDLING_EVENT_WAITING,  /**< Request to waiting a call */
	BT_HF_CALL_HANDLING_EVENT_HELD,  /**< Request to hold a call */
	BT_HF_CALL_HANDLING_EVENT_UNHELD,  /**< Request to unhold calls */
	BT_HF_CALL_HANDLING_EVENT_SWAPPED,  /**< Request to swap calls */
} bt_hf_call_handling_event_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Enumerations for the call state
 * @since_tizen 2.3
 */
typedef enum {
	BT_HF_CALL_EVENT_IDLE = 0x00,  /**< Idle */
	BT_HF_CALL_EVENT_ANSWER,  /**< Answered */
	BT_HF_CALL_EVENT_HOLD,  /**< Held */
	BT_HF_CALL_EVENT_RETRIEVE,  /**< Retrieved */
	BT_HF_CALL_EVENT_DIAL,  /**< Dialing */
	BT_HF_CALL_EVENT_ALERT,  /**< Alerting */
	BT_HF_CALL_EVENT_INCOMING,  /**< Incoming */
	BT_HF_CALL_EVENT_REDIAL,  /**< Redialling */
	BT_HF_CALL_EVENT_RELEASE_ALL_NONACTIVE_CALLS,  /**< Release all nonactive calls */
	BT_HF_CALL_EVENT_ACCEPT_AND_RELEASE,  /**< Accept and Release */
	BT_HF_CALL_EVENT_ACCEPT_AND_HOLD,  /**< Accept and Hold */
	BT_HF_CALL_EVENT_ADD_TO_CONVERSATION,  /**< Add to the conversation */
	BT_HF_CALL_EVENT_AUDIO_MUTE_ON,  /**< Audio Mute On*/
	BT_HF_CALL_EVENT_AUDIO_MUTE_OFF,  /**< Audio Mute Off*/
} bt_hf_call_event_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Enumerations for the call event from Audio-Gateway device
 * @since_tizen 3.0
 */
typedef enum {
	BT_HF_REMOTE_CALL_EVENT_IDLE = 0x00,  /**< Idle. Neither setup call nor active call exist */
	BT_HF_REMOTE_CALL_EVENT_INCOMING,  /**< (Call-setup event) Received an incoming call on AG */
	BT_HF_REMOTE_CALL_EVENT_DIALING,  /**< (Call-setup event) Dialing an outgoing call on AG */
	BT_HF_REMOTE_CALL_EVENT_ALERTING,  /**< (Call-setup event) Remote party being alerted in an outgoing call of AG */
	BT_HF_REMOTE_CALL_EVENT_CALL_TERMINATED,  /**< (Call-setup event) Setup call is terminated without activating */
	BT_HF_REMOTE_CALL_EVENT_CALL_STARTED,  /**< (Active call state event) Call is started on AG */
	BT_HF_REMOTE_CALL_EVENT_CALL_ENDED,  /**< (Active call state event) Active call is terminated on AG */
	BT_HF_REMOTE_CALL_EVENT_UNHELD,  /**< (Call held event) No calls on hold */
	BT_HF_REMOTE_CALL_EVENT_SWAPPED,  /**< (Call held event) Call is placed on hold or active/held calls swapped */
	BT_HF_REMOTE_CALL_EVENT_HELD,  /**< (Call held event) Calls on hold, no active call */
	BT_HF_REMOTE_CALL_EVENT_RINGING,  /**< Incoming call is ringing event with number. This event is optional event. */
	BT_HF_REMOTE_CALL_EVENT_WAITING,  /**< Call Waiting notification in 3-way call scenario */
	BT_HF_REMOTE_CALL_EVENT_FAILED_TO_DIALING,  /**< Failed to dialing a outgoing call on AG */
} bt_hf_remote_call_event_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Enumerations for the multi call handling event
 * @since_tizen 2.3
 */
typedef enum {
	BT_HF_MULTI_CALL_HANDLING_EVENT_RELEASE_HELD_CALLS = 0x00,  /**< Request to release held calls */
	BT_HF_MULTI_CALL_HANDLING_EVENT_RELEASE_ACTIVE_CALLS,  /**< Request to release active calls */
	BT_HF_MULTI_CALL_HANDLING_EVENT_ACTIVATE_HELD_CALL,  /**< Request to put active calls into hold state and activate another (held or waiting) call */
	BT_HF_MULTI_CALL_HANDLING_EVENT_MERGE_CALLS,  /**< Request to add a held call to the conversation */
	BT_HF_MULTI_CALL_HANDLING_EVENT_EXPLICIT_CALL_TRANSFER,  /**< Request to let a user who has two calls to connect these two calls together and release its connections to both other parties */
} bt_hf_multi_call_handling_event_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  HF Call status information.
 * @since_tizen 2.3
 */
typedef struct {
	char *number;  /**< Phone Number */
	int direction;  /**< Direction :Incoming(1), Outgoing(0) */
	int status;  /**< Call Status :Active(0), Held(1), Waiting(5), Dailing(2) */
	int multi_party;  /**< Multiparty/conf call: Yes(1), No(0) */
	int index;  /**< Call index/ID */
} bt_hf_call_status_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Called when the SCO(Synchronous Connection Oriented link) state is changed.
 * @since_tizen 2.3
 *
 * @details  This callback is called when the SCO state is changed.
 * When you call bt_ag_open_sco() or bt_ag_close_sco(), this callback is also called with error result even though these functions failed.
 * @param[in] result  The result of changing the connection state
 * @param[in] opened  The state to be changed: (@c true = opened, @c  false = not opened)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_sco_state_changed_cb()
 * @see bt_ag_unset_sco_state_changed_cb()
 * @see bt_ag_open_sco()
 * @see bt_ag_close_sco()
 */
typedef void (*bt_ag_sco_state_changed_cb) (int result, bool opened, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Called when the SCO(Synchronous Connection Oriented link) state is changed.
 * @since_tizen 2.3
 *
 * @details  This callback is called when the SCO state is changed.
 * When you call bt_ag_open_sco() or bt_ag_close_sco(), this callback is also called with error result even though these functions failed.
 * @param[in] result  The result of changing the connection state
 * @param[in] opened  The state to be changed: (@c true = opened, @c  false = not opened)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_sco_state_changed_cb()
 * @see bt_ag_unset_sco_state_changed_cb()
 * @see bt_ag_open_sco()
 * @see bt_ag_close_sco()
 */
typedef void (*bt_hf_sco_state_changed_cb) (int result, bool opened, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Called when a call handling event happened from Hands-Free.
 * @since_tizen 2.3
 *
 * @param[in] event  The call handling event happened from Hands-Free
 * @param[in] call_id  The call ID
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_call_handling_event_cb()
 * @see bt_ag_unset_call_handling_event_cb()
 */
typedef void (*bt_hf_call_handling_event_cb) (bt_hf_call_handling_event_e event, char *phone_number, void *user_data);


/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Called  when a call event happend from Audio-Gateway device
 * @since_tizen 3.0
 *
 * @param[in] event The call state chagned event from remote Audio-Gateway device
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see bt_hf_set_remote_call_event_cb()
 * @see bt_hf_unset_remote_call_event_cb()
 */
typedef void (*bt_hf_remote_call_event_cb) (bt_hf_remote_call_event_e event, char *phone_number, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Called when a multi call handling event happened from Hands-Free.
 * @since_tizen 2.3
 *
 * @param[in] event  The call handling event happened from Hands-Free
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_multi_call_handling_event_cb()
 * @see bt_ag_unset_multi_call_handling_event_cb()
 */
typedef void (*bt_hf_multi_call_handling_event_cb) (bt_hf_multi_call_handling_event_e event, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Called when the speaker gain of the remote device is changed.
 * @since_tizen 2.3
 *
 * @param[in] gain The gain of speaker (0 ~ 15)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_hf_set_speaker_gain_changed_cb()
 * @see bt_hf_unset_speaker_gain_changed_cb()
 */
typedef void (*bt_hf_speaker_gain_changed_cb) (int gain, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Called when a call status updated event happened from Hands-Free.
 * @since_tizen 2.3
 * @remarks call_info_list has elements which consist of bt_hf_call_status_info_s
 * @remarks The @a call_info_list must be released with bt_hf_free_call_status_info_list() by you.
 *
 * @param[in] event  The call handling event happened from Hands-Free
 * @param[in] call_id  The call ID
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_hf_call_status_info_s
 * @see bt_hf_set_call_status_updated_event_cb()
 * @see bt_hf_unset_call_status_updated_event_cb()
 */
typedef void (*bt_hf_call_status_updated_event_cb) (GSList *call_info_list, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Called  when a call event happend from Audio-Gateway device
 * @since_tizen 3.0
 *
 * @param[in] event The call state chagned event from remote Audio-Gateway device
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see bt_hf_set_remote_call_event_cb()
 * @see bt_hf_unset_remote_call_event_cb()
 */
typedef void (*bt_hf_remote_call_event_cb) (bt_hf_remote_call_event_e event, char *phone_number, void *user_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_NETWORK_BLUETOOTH_TYPE_EXTENSION_H__ */
