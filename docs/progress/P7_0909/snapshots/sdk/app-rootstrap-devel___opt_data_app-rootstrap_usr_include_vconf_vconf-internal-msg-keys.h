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

#ifndef __VCONF_INTERNAL_MSG_KEYS_H__
#define __VCONF_INTERNAL_MSG_KEYS_H__
/**
 *
 */
#define VCONFKEY_MSG_SERVER_READY  "memory/msg/ready"

/**
 *
 */
#define VCONFKEY_MESSAGE_RECV_SMS_STATE  "db/msg/recv_sms"

/**
 *
 */
#define VCONFKEY_MESSAGE_RECV_MMS_STATE  "db/msg/recv_mms"

/**
 *  do not remove it's for enumeration (sangkoo.kim@samsung.com)
 */
#define VCONFKEY_MESSAGE_NETWORK_MODE  "db/msg/network_mode"

enum {
	VCONFKEY_MESSAGE_NETWORK_PS_ONLY = 0x01,
		VCONFKEY_MESSAGE_NETWORK_CS_ONLY = 0x02,
		VCONFKEY_MESSAGE_NETWORK_PS_PREFER = 0x03,
		VCONFKEY_MESSAGE_NETWORK_CS_PREFER = 0x04
};

/**
 *
 */
#define VCONFKEY_MESSAGE_ACTIVATED_CONVERSATION_ID  "db/msg/activated_conversation_id"

/**
 *
 */
#define VCONFKEY_MESSAGE_NOTIFICATION_PRIV_ID  "db/private/msg-service/notification_priv_id"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_NOTI_PRIV_ID  "db/private/msg-service/cb_noti_priv_id"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_MSG_NOTI_PRIV_ID  "db/private/msg-service/sim_msg_noti_priv_id"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_FULL_NOTI_PRIV_ID  "db/private/msg-service/sim_full_noti_id"

/**
 *
 */
#define VCONFKEY_MESSAGE_SENTFAIL_NOTI_ID  "db/private/msg-service/sentfail_noti_id"

/**
 *
 */
#define VCONFKEY_MESSAGE_READ_REPORT_SENT_NOTI_ID  "db/private/msg-service/read_report_sent_noti_id"

/**
 *
 */
#define VCONFKEY_MESSAGE_STORAGE_FULL_NOTI_ID  "db/private/msg-service/storage_full_noti_id"

/**
 *
 */
#define VCONFKEY_MESSAGE_READ_REPORT_SENT_MSG_ID  "db/private/msg-service/read_report_sent_msg_id"

/**
 *
 */
#define VCONFKEY_MESSAGE_VOICE_NOTI_ID1_SIM1  "db/private/msg-service/voice_noti_id1/1"

/**
 *
 */
#define VCONFKEY_MESSAGE_VOICE_NOTI_ID1_SIM2  "db/private/msg-service/voice_noti_id1/2"

/**
 *
 */
#define VCONFKEY_MESSAGE_VOICE_NOTI_ID1_SIM3  "db/private/msg-service/voice_noti_id1/3"

/**
 *
 */
#define VCONFKEY_MESSAGE_VOICE_NOTI_ID2_SIM1  "db/private/msg-service/voice_noti_id2/1"

/**
 *
 */
#define VCONFKEY_MESSAGE_VOICE_NOTI_ID2_SIM2  "db/private/msg-service/voice_noti_id2/2"

/**
 *
 */
#define VCONFKEY_MESSAGE_VOICE_NOTI_ID2_SIM3  "db/private/msg-service/voice_noti_id2/3"

/**
 *
 */
#define VCONFKEY_MESSAGE_AUTO_ERASE  "db/private/msg-service/general/auto_erase"

/**
 *
 */
#define VCONFKEY_MESSAGE_BLOCK_MESSAGE  "db/private/msg-service/general/block_msg"

/**
 *
 */
#define VCONFKEY_MESSAGE_BLOCK_UNKNOWN_MSG  "db/private/msg-service/general/block_unknown_msg"

/**
 *
 */
#define VCONFKEY_MESSAGE_KEEP_COPY  "db/private/msg-service/general/keep_copy"

/**
 *
 */
#define VCONFKEY_MESSAGE_SMS_LIMIT  "db/private/msg-service/general/sms_limit"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_LIMIT  "db/private/msg-service/general/mms_limit"

/**
 *
 */
#define VCONFKEY_MESSAGE_MESSAGE_ID_COUNTER  "db/private/msg-service/general/msg_id_counter"

/**
 *
 */
#define VCONFKEY_MESSAGE_SETTING_NOTIFICATION  "db/private/msg-service/general/notification"

/**
 *
 */
#define VCONFKEY_MESSAGE_SETTING_PREVIEW  "db/private/msg-service/general/preview"

/**
 *
 */
#define VCONFKEY_MESSAGE_SETTING_RINGTONE_PATH  "db/private/msg-service/general/ringtone_path"

/**
 *
 */
#define VCONFKEY_MESSAGE_SETTING_RINGTONE_TYPE  "db/private/msg-service/general/ringtone_type"

/**
 *
 */
#define VCONFKEY_MESSAGE_SETTING_VIBRATION  "db/private/msg-service/general/vibration"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_UA_PROFILE  "db/private/msg-service/general/ua_profile"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_LANGUAGE_0  "db/private/msg-service/cb_msg/language/0"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_LANGUAGE_1  "db/private/msg-service/cb_msg/language/1"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_LANGUAGE_2  "db/private/msg-service/cb_msg/language/2"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_LANGUAGE_3  "db/private/msg-service/cb_msg/language/3"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_LANGUAGE_4  "db/private/msg-service/cb_msg/language/4"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_LANGUAGE_5  "db/private/msg-service/cb_msg/language/5"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_LANGUAGE_6  "db/private/msg-service/cb_msg/language/6"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_LANGUAGE_7  "db/private/msg-service/cb_msg/language/7"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_LANGUAGE_8  "db/private/msg-service/cb_msg/language/8"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_LANGUAGE_9  "db/private/msg-service/cb_msg/language/9"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_MAX_SIM_COUNT_SIM1  "db/private/msg-service/cb_msg/max_sim_count/1"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_MAX_SIM_COUNT_SIM2  "db/private/msg-service/cb_msg/max_sim_count/2"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_MAX_SIM_COUNT_SIM3  "db/private/msg-service/cb_msg/max_sim_count/3"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_RECEIVE_SIM1  "db/private/msg-service/cb_msg/receive/1"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_RECEIVE_SIM2  "db/private/msg-service/cb_msg/receive/2"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_RECEIVE_SIM3  "db/private/msg-service/cb_msg/receive/3"

/**
 *
 */
#define VCONFKEY_MESSAGE_CB_SAVE  "db/private/msg-service/cb_msg/save"

/**
 *
 */
#define VCONFKEY_MESSAGE_SMS_SEND_DCS  "db/private/msg-service/sms_send/dcs"

/**
 *
 */
#define VCONFKEY_MESSAGE_SMS_SEND_DELIVERY_REPORT  "db/private/msg-service/sms_send/delivery_report"

/**
 *
 */
#define VCONFKEY_MESSAGE_SMS_SEND_REPLY_PATH  "db/private/msg-service/sms_send/reply_path"

/**
 *
 */
#define VCONFKEY_MESSAGE_SMS_SEND_SAVE_STORAGE  "db/private/msg-service/sms_send/save_storage"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_RECV_ABROAD_NETWORK  "db/private/msg-service/mms_recv/abroad_network"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_RECV_HOME_NETWORK  "db/private/msg-service/mms_recv/home_network"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_RECV_DELIVERY_RECEIPT  "db/private/msg-service/mms_recv/delivery_receipt"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_RECV_READ_RECEIPT  "db/private/msg-service/mms_recv/read_receipt"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_RECV_REJECT_ADVERTISE  "db/private/msg-service/mms_recv/reject_advertisement"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_RECV_REJECT_UNKNOWN  "db/private/msg-service/mms_recv/reject_unknown"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_RECV_GROUP_MESSAGE  "db/private/msg-service/mms_recv/group_message"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_BODY_REPLYING  "db/private/msg-service/mms_send/body_replying"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_CREATION_MODE  "db/private/msg-service/mms_send/creation_mode"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_CUSTOM_DELIVERY  "db/private/msg-service/mms_send/custom_delivery"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_DELIVERY_REPORT  "db/private/msg-service/mms_send/delivery_report"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_DELIVERY_TIME  "db/private/msg-service/mms_send/delivery_time"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_EXPIRY_TIME  "db/private/msg-service/mms_send/expiry_time"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_HIDE_RECIPIENTS  "db/private/msg-service/mms_send/hide_recipients"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_KEEP_COPY  "db/private/msg-service/mms_send/keep_copy"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_MSG_CLASS  "db/private/msg-service/mms_send/msg_class"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_PRIORITY  "db/private/msg-service/mms_send/priority"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_READ_REPLY  "db/private/msg-service/mms_send/read_reply"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_REPLY_CHARGING  "db/private/msg-service/mms_send/reply_charging"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_REPLY_CHARGING_DEADLINE  "db/private/msg-service/mms_send/reply_charging_deadline"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_REPLY_CHARGING_SIZE  "db/private/msg-service/mms_send/reply_charging_size"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_REPORT_ALLOWED  "db/private/msg-service/mms_send/report_allowed"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_SEND_SENDER_VISIBILITY  "db/private/msg-service/mms_send/sender_visibility"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_STYLE_BG_COLOR_BLUE  "db/private/msg-service/mms_style/bg_color/blue"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_STYLE_BG_COLOR_GREEN  "db/private/msg-service/mms_style/bg_color/green"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_STYLE_BG_COLOR_HUE  "db/private/msg-service/mms_style/bg_color/hue"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_STYLE_BG_COLOR_RED  "db/private/msg-service/mms_style/bg_color/red"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_STYLE_FONT_COLOR_BLUE  "db/private/msg-service/mms_style/font_color/blue"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_STYLE_FONT_COLOR_GREEN  "db/private/msg-service/mms_style/font_color/green"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_STYLE_FONT_COLOR_HUE  "db/private/msg-service/mms_style/font_color/hue"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_STYLE_FONT_COLOR_RED  "db/private/msg-service/mms_style/font_color/red"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_STYLE_FONT_SIZE  "db/private/msg-service/mms_style/font_size"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_STYLE_FONT_STYLE_BOLD  "db/private/msg-service/mms_style/font_style/bold"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_STYLE_FONT_STYLE_ITALIC  "db/private/msg-service/mms_style/font_style/italic"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_STYLE_FONT_STYLE_UNDERLINE  "db/private/msg-service/mms_style/font_style/underline"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_STYLE_PAGE_DURATION  "db/private/msg-service/mms_style/page_dur"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_STYLE_PAGE_DURATION_MANUAL  "db/private/msg-service/mms_style/page_dur_manual"

/**
 *
 */
#define VCONFKEY_MESSAGE_MMS_STYLE_PAGE_CUSTOM_DURATION  "db/private/msg-service/mms_style/page_custom_dur"

/**
 *
 */
#define VCONFKEY_MESSAGE_PUSH_RECV_OPTION  "db/private/msg-service/push_msg/recv_option"

/**
 *
 */
#define VCONFKEY_MESSAGE_PUSH_SERVICE_TYPE  "db/private/msg-service/push_msg/service_load"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_TOTAL_COUNT_SIM1  "db/private/msg-service/sim_count/total_cnt/1"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_TOTAL_COUNT_SIM2  "db/private/msg-service/sim_count/total_cnt/2"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_TOTAL_COUNT_SIM3  "db/private/msg-service/sim_count/total_cnt/3"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_USED_COUNT_SIM1  "db/private/msg-service/sim_count/used_cnt/1"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_USED_COUNT_SIM2  "db/private/msg-service/sim_count/used_cnt/2"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_USED_COUNT_SIM3  "db/private/msg-service/sim_count/used_cnt/3"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_CHANGED_SIM1  "memory/private/msg-service/sim_changed/1"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_CHANGED_SIM2  "memory/private/msg-service/sim_changed/2"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_CHANGED_SIM3  "memory/private/msg-service/sim_changed/3"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_MO_CONTROL_SIM1  "memory/private/msg-service/sim_mo_ctrl/1"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_MO_CONTROL_SIM2  "memory/private/msg-service/sim_mo_ctrl/2"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_MO_CONTROL_SIM3  "memory/private/msg-service/sim_mo_ctrl/3"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_SERVICE_TABLE_SIM1  "memory/private/msg-service/sim_st/1"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_SERVICE_TABLE_SIM2  "memory/private/msg-service/sim_st/2"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_SERVICE_TABLE_SIM3  "memory/private/msg-service/sim_st/3"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_SUBS_ID_SIM1  "memory/private/msg-service/sim_subs_id/1"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_SUBS_ID_SIM2  "memory/private/msg-service/sim_subs_id/2"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_SUBS_ID_SIM3  "memory/private/msg-service/sim_subs_id/3"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_MSISDN_SIM1  "memory/private/msg-service/msisdn/1"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_MSISDN_SIM2  "memory/private/msg-service/msisdn/2"

/**
 *
 */
#define VCONFKEY_MESSAGE_SIM_MSISDN_SIM3  "memory/private/msg-service/msisdn/3"

/**
 *
 */
#define VCONFKEY_MESSAGE_VOICEMAIL_ALPHA_ID_SIM1  "db/private/msg-service/voice_mail/voice_mail_alphaid/1"

/**
 *
 */
#define VCONFKEY_MESSAGE_VOICEMAIL_ALPHA_ID_SIM2  "db/private/msg-service/voice_mail/voice_mail_alphaid/2"

/**
 *
 */
#define VCONFKEY_MESSAGE_VOICEMAIL_ALPHA_ID_SIM3  "db/private/msg-service/voice_mail/voice_mail_alphaid/3"

/**
 *
 */
#define VCONFKEY_MESSAGE_VOICEMAIL_COUNT_SIM1  "db/private/msg-service/voice_mail/voice_mail_count/1"

/**
 *
 */
#define VCONFKEY_MESSAGE_VOICEMAIL_COUNT_SIM2  "db/private/msg-service/voice_mail/voice_mail_count/2"

/**
 *
 */
#define VCONFKEY_MESSAGE_VOICEMAIL_COUNT_SIM3  "db/private/msg-service/voice_mail/voice_mail_count/3"

/**
 *
 */
#define VCONFKEY_MESSAGE_VOICEMAIL_NUMBER_SIM1  "db/private/msg-service/voice_mail/voice_mail_number/1"

/**
 *
 */
#define VCONFKEY_MESSAGE_VOICEMAIL_NUMBER_SIM2  "db/private/msg-service/voice_mail/voice_mail_number/2"

/**
 *
 */
#define VCONFKEY_MESSAGE_VOICEMAIL_NUMBER_SIM3  "db/private/msg-service/voice_mail/voice_mail_number/3"

/**
 *
 */
#define VCONFKEY_MESSAGE_MSGSIZE_OPTION  "db/private/msg-service/size_opt/msg_size"

/**
 *
 */
#define VCONFKEY_MESSAGE_NETWORK_SIM  "memory/private/msg-service/default_network_sim"

#endif /*   __VCONF_INTERNAL_MSG_KEYS_H__    */
