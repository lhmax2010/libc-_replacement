/*
 * vconf-internal-keys
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
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
 *
 */

#ifndef __VCONF_INTERNAL_TELEPHONY_KEYS_H__
#define __VCONF_INTERNAL_TELEPHONY_KEYS_H__
#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_TELEPHONY_PREFIX  "memory/telephony"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_TELEPHONY_DB_PREFIX  "db/telephony"

/**
 * It stands for telephony daemon load count between reboots
 */
#define VCONFKEY_TELEPHONY_DAEMON_LOAD_COUNT  "memory/telephony/daemon_load_count"

/**
 * It stands for current attached network name and status
 */
#define VCONFKEY_TELEPHONY_NWNAME  "memory/telephony/nw_name"

/**
 * It stands for current attached network PLMN value
 */
#define VCONFKEY_TELEPHONY_PLMN  "memory/telephony/plmn"

/**
 * It stands for current attached network LAC value
 */
#define VCONFKEY_TELEPHONY_LAC  "memory/telephony/lac"

/**
 * It stands for current attached Cell ID value
 */
#define VCONFKEY_TELEPHONY_CELLID  "memory/telephony/cell_id"

/**
 * It stands for which icon mode will be displayed
 */
#define VCONFKEY_TELEPHONY_ROAM_ICON_MODE  "memory/telephony/roam_icon_mode"

enum {
	VCONFKEY_TELEPHONY_ROAM_ICON_SOLID_MODE = 0x00,
		VCONFKEY_TELEPHONY_ROAM_ICON_OFF,
		VCONFKEY_TELEPHONY_ROAM_ICON_FLASHING_MODE
};

/**
 * It stands for current network type (3G, 2G, EDGE, Searching, Emergency, etc)
 */
#define VCONFKEY_TELEPHONY_SVCTYPE  "memory/telephony/svc_type"

enum {
	VCONFKEY_TELEPHONY_SVCTYPE_NONE     ,
		VCONFKEY_TELEPHONY_SVCTYPE_NOSVC    ,
		VCONFKEY_TELEPHONY_SVCTYPE_EMERGENCY,
		VCONFKEY_TELEPHONY_SVCTYPE_SEARCH   ,
		VCONFKEY_TELEPHONY_SVCTYPE_2G       ,
		VCONFKEY_TELEPHONY_SVCTYPE_2_5G     ,
		VCONFKEY_TELEPHONY_SVCTYPE_2_5G_EDGE,
		VCONFKEY_TELEPHONY_SVCTYPE_3G       ,
		VCONFKEY_TELEPHONY_SVCTYPE_HSDPA    ,
		VCONFKEY_TELEPHONY_SVCTYPE_LTE
};

/**
 * It stands for current network circuit service availability
 */
#define VCONFKEY_TELEPHONY_SVC_CS  "memory/telephony/svc_cs"

enum {
	VCONFKEY_TELEPHONY_SVC_CS_UNKNOWN = 0x00,
		VCONFKEY_TELEPHONY_SVC_CS_OFF,
		VCONFKEY_TELEPHONY_SVC_CS_ON
};

/**
 * It stands for current network packet service availability
 */
#define VCONFKEY_TELEPHONY_SVC_PS  "memory/telephony/svc_ps"

enum {
	VCONFKEY_TELEPHONY_SVC_PS_UNKNOWN = 0x00,
		VCONFKEY_TELEPHONY_SVC_PS_OFF,
		VCONFKEY_TELEPHONY_SVC_PS_ON
};

/**
 * It stands for current network zone is home zone or roaming zone
 */
#define VCONFKEY_TELEPHONY_SVC_ROAM  "memory/telephony/svc_roam"

enum {
	VCONFKEY_TELEPHONY_SVC_ROAM_OFF = 0x00,
		VCONFKEY_TELEPHONY_SVC_ROAM_ON
};

/**
 * It stands for current device has a SIM card or not
 */
#define VCONFKEY_TELEPHONY_SIM_SLOT  "memory/telephony/sim_slot"

enum {
	VCONFKEY_TELEPHONY_SIM_UNKNOWN = 0x00,
		VCONFKEY_TELEPHONY_SIM_INSERTED,
		VCONFKEY_TELEPHONY_SIM_NOT_PRESENT,
		VCONFKEY_TELEPHONY_SIM_CARD_ERROR
};

/**
 * It stands for current device has a second SIM card or not (for Dual SIM model)
 */
#define VCONFKEY_TELEPHONY_SIM_SLOT2  "memory/telephony/sim_slot2"

/**
 * It stands for the number of sim slot having current device
 */
#define VCONFKEY_TELEPHONY_SIM_SLOT_COUNT  "memory/telephony/sim_slot_count"

/**
 * It stands for current telephony phonebook is ready to give valid value or not
 */
#define VCONFKEY_TELEPHONY_SIM_PB_INIT  "memory/telephony/pb_init"

enum {
	VCONFKEY_TELEPHONY_SIM_PB_INIT_NONE = 0x00,
		VCONFKEY_TELEPHONY_SIM_PB_INIT_COMPLETED
};

/**
 * It stands for current sim cached data status as below
 */
#define VCONFKEY_TELEPHONY_SIM_STATUS  "memory/telephony/sim_status"

enum {
	VCONFKEY_TELEPHONY_SIM_STATUS_CARD_ERROR = 0x00,
		VCONFKEY_TELEPHONY_SIM_STATUS_CARD_NOT_PRESENT = 0x01,
		VCONFKEY_TELEPHONY_SIM_STATUS_INITIALIZING = 0x02,
		VCONFKEY_TELEPHONY_SIM_STATUS_INIT_COMPLETED = 0x03,
		VCONFKEY_TELEPHONY_SIM_STATUS_PIN_REQUIRED = 0x04,
		VCONFKEY_TELEPHONY_SIM_STATUS_PUK_REQUIRED = 0x05,
		VCONFKEY_TELEPHONY_SIM_STATUS_CARD_BLOCKED = 0x06,
		VCONFKEY_TELEPHONY_SIM_STATUS_NCK_REQUIRED = 0x07,
		VCONFKEY_TELEPHONY_SIM_STATUS_NSCK_REQUIRED = 0x08,
		VCONFKEY_TELEPHONY_SIM_STATUS_SPCK_REQUIRED = 0x09,
		VCONFKEY_TELEPHONY_SIM_STATUS_CCK_REQUIRED = 0x0a,
		VCONFKEY_TELEPHONY_SIM_STATUS_CARD_REMOVED = 0x0b,
		VCONFKEY_TELEPHONY_SIM_STATUS_SIM_LOCK_REQUIRED = 0x0c,
		VCONFKEY_TELEPHONY_SIM_STATUS_CARD_CRASHED = 0x0d,
		VCONFKEY_TELEPHONY_SIM_STATUS_UNKNOWN = 0xff
};

/**
 * It stands for current sim is changed or not
 */
#define VCONFKEY_TELEPHONY_SIM_IS_CHANGED  "memory/telephony/sim_is_changed"

/**
 * It stands for current call status (idle, connect, held)
 */
#define VCONFKEY_TELEPHONY_CALL_STATE  "memory/telephony/call_state"

enum {
	VCONFKEY_TELEPHONY_CALL_CONNECT_IDLE = 0x00,
		VCONFKEY_TELEPHONY_CALL_CONNECT_ACTIVE,
		VCONFKEY_TELEPHONY_CALL_CONNECT_HELD
};

/**
 * It stands for SIM1 Call Forwarding Indicator status - Always case (CFU)
 */
#define VCONFKEY_TELEPHONY_CALL_FORWARD_STATE  "db/telephony/call_forward_state"

enum {
	VCONFKEY_TELEPHONY_CALL_FORWARD_OFF = 0x00,
		VCONFKEY_TELEPHONY_CALL_FORWARD_ON = 0x01
};

/**
 * It stands for SIM2 Call Forwarding Indicator status - Always case (CFU)
 */
#define VCONFKEY_TELEPHONY_CALL_FORWARD_STATE2  "db/telephony/call_forward_state2"

/**
 * It stands for show my number status
 */
#define VCONFKEY_TELEPHONY_SS_CLI_STATE  "db/telephony/ss_cli_state"

enum {
	VCONFKEY_TELEPHONY_SS_CLI_BYNETWORK = 0x00,
		VCONFKEY_TELEPHONY_SS_CLI_SHOW,
		VCONFKEY_TELEPHONY_SS_CLI_HIDE
};

/**
 * It stands for show my number status of SIM2
 */
#define VCONFKEY_TELEPHONY_SS_CLI_STATE2  "db/telephony/ss_cli_state2"

/**
 * It stands for current telephony apis is ready to use or not
 */
#define VCONFKEY_TELEPHONY_TAPI_STATE  "memory/telephony/tapi_state"

enum {
	VCONFKEY_TELEPHONY_TAPI_STATE_NONE = 0x00,
		VCONFKEY_TELEPHONY_TAPI_STATE_READY
};

/**
 * It stands for display priority between SIM SPN and PLMN
 */
#define VCONFKEY_TELEPHONY_SPN_DISP_CONDITION  "memory/telephony/spn_disp_condition"

enum {
	VCONFKEY_TELEPHONY_DISP_INVALID = 0x00,
		VCONFKEY_TELEPHONY_DISP_SPN,
		VCONFKEY_TELEPHONY_DISP_PLMN,
		VCONFKEY_TELEPHONY_DISP_SPN_PLMN
};

/**
 * It stands for SPN name which is stored in SIM card
 */
#define VCONFKEY_TELEPHONY_SPN_NAME  "memory/telephony/spn"

/**
 * It stands for current network RSSI strength
 */
#define VCONFKEY_TELEPHONY_RSSI  "memory/telephony/rssi"

enum {
	VCONFKEY_TELEPHONY_RSSI_0 = 0x00,
		VCONFKEY_TELEPHONY_RSSI_1,
		VCONFKEY_TELEPHONY_RSSI_2,
		VCONFKEY_TELEPHONY_RSSI_3,
		VCONFKEY_TELEPHONY_RSSI_4,
		VCONFKEY_TELEPHONY_RSSI_5,
		VCONFKEY_TELEPHONY_RSSI_6
};

/**
 * It stands for packet service protocol type
 */
#define VCONFKEY_TELEPHONY_PSTYPE  "memory/telephony/ps_type"

enum {
	VCONFKEY_TELEPHONY_PSTYPE_NONE,
		VCONFKEY_TELEPHONY_PSTYPE_HSDPA,
		VCONFKEY_TELEPHONY_PSTYPE_HSUPA,
		VCONFKEY_TELEPHONY_PSTYPE_HSPA,
		VCONFKEY_TELEPHONY_PSTYPE_HSPAP
};

/**
 * It stands for cellular data connection enable status
 */
#define VCONFKEY_3G_ENABLE  "db/setting/3gEnabled"

/**
 * It stands for telephony server is ready or not
 */
#define VCONFKEY_TELEPHONY_READY  "memory/telephony/telephony_ready"

/**
 * It stands for NITZ GMT time
 */
#define VCONFKEY_TELEPHONY_NITZ_GMT  "memory/telephony/nitz_gmt"

/**
 * It stands for NITZ Event arrival time
 */
#define VCONFKEY_TELEPHONY_NITZ_EVENT_GMT  "memory/telephony/nitz_event_gmt"

/**
 * It stands for NITZ time zone
 */
#define VCONFKEY_TELEPHONY_NITZ_ZONE  "memory/telephony/nitz_zone"

/**
 * It stands for flight mode status
 */
#define VCONFKEY_TELEPHONY_FLIGHT_MODE  "db/telephony/flight_mode"

/**
 * It stands for network Access Technology
 */
#define VCONFKEY_TELEPHONY_SVC_ACT  "memory/telephony/svc_act"

enum {
	VCONFKEY_TELEPHONY_SVC_ACT_NONE,
		VCONFKEY_TELEPHONY_SVC_ACT_GSM,
		VCONFKEY_TELEPHONY_SVC_ACT_GPRS,
		VCONFKEY_TELEPHONY_SVC_ACT_EGPRS,
		VCONFKEY_TELEPHONY_SVC_ACT_UMTS,
		VCONFKEY_TELEPHONY_SVC_ACT_IS95A = 0x11,
		VCONFKEY_TELEPHONY_SVC_ACT_IS95B,
		VCONFKEY_TELEPHONY_SVC_ACT_CDMA_1X,
		VCONFKEY_TELEPHONY_SVC_ACT_EVDO_REV0,
		VCONFKEY_TELEPHONY_SVC_ACT_CDMA_1X_EVDO_REV0,
		VCONFKEY_TELEPHONY_SVC_ACT_EVDO_REVA,
		VCONFKEY_TELEPHONY_SVC_ACT_CDMA_1X_EVDO_REVA,
		VCONFKEY_TELEPHONY_SVC_ACT_EVDV,
		VCONFKEY_TELEPHONY_SVC_ACT_EHRPD,
		VCONFKEY_TELEPHONY_SVC_ACT_LTE = 0x21,
		VCONFKEY_TELEPHONY_SVC_ACT_NOT_SPECIFIED  = 0xFF
};

/**
 * It stands for SIM Power State for sim slot 1
 */
#define VCONFKEY_TELEPHONY_SIM_POWER_STATE1  "db/telephony/sim_power_state1"

/**
 * It stands for SIM Power State for sim slot 2
 */
#define VCONFKEY_TELEPHONY_SIM_POWER_STATE2  "db/telephony/sim_power_state2"

/**
 * It stands for Enable/Disable "Receive Incoming Call" in Dual SIM during PS connected state
 */
#define VCONFKEY_TELEPHONY_RECEIVE_INCOMING_CALL_BOOL  "db/telephony/dualsim/receive_incoming_call"

/**
 * It stands for Modem always on state during companion mode
 */
#define VCONFKEY_TELEPHONY_MODEM_ALWAYS_ON_STATE  "db/telephony/modem_always_on"

enum {
	VCONFKEY_TELEPHONY_MODEM_ALWAYS_OFF = 0x00,
		VCONFKEY_TELEPHONY_MODEM_ALWAYS_ON,
		VCONFKEY_TELEPHONY_MODEM_AUTO_ON_OFF
};

/**
 * It stands for activation is completed or not
 */
#define VCONFKEY_TELEPHONY_ACTIVATION_COMPLETED  "db/telephony/activation_completed"

/**
 * It stands for if phone is entered emergency call back mode for CDMA
 */
#define VCONFKEY_TELEPHONY_EMERGENCY_CB_MODE_CDMA  "memory/telephony/emergency_mode"

/**
 * It stands for MDN status
 */
#define VCONFKEY_TELEPHONY_MDN  "db/telephony/mdn"

/**
 * It stands for call alert signal type
 */
#define VCONFKEY_TELEPHONY_CALL_SIGNAL_TYPE  "memory/telephony/call_alert_signal_type"

/**
 * It stands for call alert pitch type
 */
#define VCONFKEY_TELEPHONY_CALL_PITCH_TYPE  "memory/telephony/call_alert_pitch_type"

/**
 * It stands for call signal
 */
#define VCONFKEY_TELEPHONY_CALL_SIGNAL  "memory/telephony/call_signal"

/**
 * It stands for default data subscription (Packet Switched)
 */
#define VCONFKEY_TELEPHONY_DB_DEFAULT_DATA_SUBS  "db/telephony/dualsim/default_data_service"

/**
 * It stands for preffered voice subscription
 */
#define VCONFKEY_TELEPHONY_PREFERRED_VOICE_SUBSCRIPTION  "db/telephony/dualsim/preferred_voice_subscription"

/**
 * It stands for default subscription (Circuit Switched
 */
#define VCONFKEY_TELEPHONY_DB_DEFAULT_SUBS  "db/telephony/dualsim/default_subscription"

/**
 * It stands for network mode of sim 1 (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_NETWORK_MODE_SIM_1  "db/private/telephony/sim1_network_mode"

/**
 * It stands for network mode of sim 2 (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_NETWORK_MODE_SIM_2  "db/private/telephony/sim2_network_mode"

/**
 * It stands for modem online count (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_MODEM_ON_COUNT  "memory/private/telephony/modem_on_count"

/**
 * It stands for ps data status (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_PS_DATA_STATUS  "memory/private/telephony/ps_data_status"

/**
 * It stands for no sim popup checkbox (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_NO_SIM_POPUP_CHECKBOX  "db/private/telephony/no_sim_popup_checkbox"

/**
 * It stands for data roaming popup checkbox (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_DATA_ROAMING_POPUP_CHECKBOX  "db/private/telephony/data_roaming_popup_checkbox"

/**
 * It stands for network mode is changed or not by user (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_NET_MODE_USER_CHANGED  "db/private/telephony/net_mode_user_changed"

/**
 * It stands for personal nck puk (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_PERSO_NWK_PUK  "memory/private/telephony/perso_nwk_puk"

/**
 * It stands for modem state (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_MODEM_STATE  "memory/private/telephony/modem_state"

/**
 * It stands for NITZ iso (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_NITZ_ISO  "db/private/telephony/nitz_iso"

/**
 * It stands for manual plmn (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_MANUAL_PLMN  "db/private/telephony/manual_plmn"

/**
 * It stands for nck unlock count (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_NCK_UNLOCK_COUNT  "memory/private/telephony/nck_unlock_count"

/**
 * It stands for last pdp context connected (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_PDP_LAST_CONNECTED_CONTEXT  "db/private/telephony/pdp_last_connected_context_status"

/**
 * It stands for last pdp context id connected (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_PDP_LAST_CONNECTED_CONTEXT_PROFILE_ID  "db/private/telephony/pdp_last_connected_context_profile_id"

/**
 * It stands for last pdp context plmn connected (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_PDP_LAST_CONNECTED_CONTEXT_PLMN  "db/private/telephony/pdp_last_connected_context_plmn"

/**
 * It stands for assert reason for modemd (private)
 */
#define VCONFKEY_TELEPHONY_PRIVATE_MODEMD_ASSERT_REASON  "memory/private/telephony/modemd_assert_reason"

#endif /*   __VCONF_INTERNAL_TELEPHONY_KEYS_H__    */
