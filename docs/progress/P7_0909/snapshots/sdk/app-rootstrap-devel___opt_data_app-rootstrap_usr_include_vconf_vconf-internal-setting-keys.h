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

#ifndef __VCONF_INTERNAL_SETTING_KEYS_H__
#define __VCONF_INTERNAL_SETTING_KEYS_H__
#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *  prefix string
 */
#define VCONFKEY_SETAPPL_PREFIX  "db/setting"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_SETAPPL_MEM_PREFIX  "memory/setting"

/**
 *
 */
#define VCONFKEY_SETAPPL_CALL_VIBRATION_PATTERN_STR  "db/setting/sound/call/vibration_pattern_path"

/**
 *
 */
#define VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL  "db/setting/sound/touch_sounds"

/**
 *
 */
#define VCONFKEY_SETAPPL_BUTTON_SOUNDS_BOOL  "db/setting/sound/button_sounds"

/**
 *
 */
#define VCONFKEY_SETAPPL_SOUND_LOCK_BOOL  "db/setting/sound/sound_lock"

/**
 *
 */
#define VCONFKEY_SETAPPL_REGION_AUTOMATIC_BOOL  "db/setting/region_automatic"

/**
 *  it's for wearable project
 */
#define VCONFKEY_SETAPPL_MEDIA_SOUND_VOLUME_INT  "db/setting/sound/media/sound_volume"

/**
 *  it's for wearable project
 */
#define VCONFKEY_SETAPPL_MEDIA_SOUND_VOLUME_BACKUP_INT  "db/setting/sound/media/sound_volume_backup"

/**
 *
 */
#define VCONFKEY_SETAPPL_LCD_BRIGHTNESS  "db/setting/Brightness"

enum {
	SETTING_BRIGHTNESS_LEVEL1 = 1,
		SETTING_BRIGHTNESS_LEVEL2,
		SETTING_BRIGHTNESS_LEVEL3,
		SETTING_BRIGHTNESS_LEVEL4,
		SETTING_BRIGHTNESS_LEVEL5,
		SETTING_BRIGHTNESS_LEVEL6,
		SETTING_BRIGHTNESS_LEVEL7,
		SETTING_BRIGHTNESS_LEVEL8,
		SETTING_BRIGHTNESS_LEVEL9,
		SETTING_BRIGHTNESS_LEVEL10
};

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_SETAPPL_EMERGENCY_LCD_BRIGHTNESS_INT  "db/setting/lcd_brightness_emergency"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_SETAPPL_EMERGENCY_LCD_TIMEOUT_INT  "db/setting/lcd_timeout_emergency"

/**
 *
 */
#define VCONFKEY_SETAPPL_LCD_AUTOMATIC_BRIGHTNESS  "db/setting/automatic_brightness_level"

/**
 *
 */
#define VCONFKEY_SETAPPL_LCD_AUTO_DISPLAY_ADJUSTMENT  "db/setting/auto_display_adjustment"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_SETAPPL_SCREEN_MODE_INT  "db/setting/screen_mode"

enum {
	SETTING_SCREEN_MODE_DYNAMIC = 0,
		SETTING_SCREEN_MODE_STANDARD,
		SETTING_SCREEN_MODE_NATURAL,
		SETTING_SCREEN_MODE_MOVIE,
		SETTING_SCREEN_MODE_MAX
};

/**
 *  Nonnegative value would be applied to timeout for screen dim state. Takes second time
 */
#define VCONFKEY_SETAPPL_TIMEOUT_DIM  "db/setting/timeout_dim"

/**
 *  setting - display - backlight time (value type : seconds)
 */
#define VCONFKEY_SETAPPL_LCD_TIMEOUT_NORMAL  "db/setting/lcd_backlight_normal"

/**
 *
 */
#define VCONFKEY_SETAPPL_CALL_RINGTONE_SOUND_VOLUME_INT  "db/setting/sound/call/ringtone_sound_volume"

/**
 *
 */
#define VCONFKEY_SETAPPL_CALL_RINGTONE_SOUND_VOLUME_BACKUP_INT  "db/setting/sound/call/ringtone_sound_volume_backup"

/**
 *
 */
#define VCONFKEY_SETAPPL_CALL_RINGTONE_PATH_STR  "db/setting/sound/call/ringtone_path"

/**
 *  User Ringtone List
 */
#define VCONFKEY_SETAPPL_CALL_RINGTONE_USER_LIST  "db/setting/sound/call/user_ringtone_list"

/**
 *
 */
#define VCONFKEY_SETAPPL_CALL_RINGTONE_PATH_WITH_RECOMMENDATION_TIME_STR  "db/setting/sound/call/ringtone_path_with_time"

/**
 *
 */
#define VCONFKEY_SETAPPL_CALL_RINGTONE_DEFAULT_PATH_STR  "db/setting/sound/call/ringtone_default_path"

/**
 *
 */
#define VCONFKEY_SETAPPL_CALL_VIBRATION_LEVEL_INT  "db/setting/sound/call/vibration_level"

/**
 *
 */
#define VCONFKEY_SETAPPL_CALL_VIBRATION_PATTERN_INT  "db/setting/sound/call/vibration_type"

enum {
	SETTING_CALL_ALERT_VIB_TYPE1 = 0,
		SETTING_CALL_ALERT_VIB_TYPE2,
		SETTING_CALL_ALERT_VIB_TYPE3,
		SETTING_CALL_ALERT_VIB_TYPE4,
		SETTING_CALL_ALERT_VIB_TYPE5,
		SETTING_CALL_ALERT_VIB_TYPE6,
		SETTING_CALL_ALERT_VIB_TYPE7,
		SETTING_CALL_ALERT_VIB_TYPE8,
		SETTING_CALL_ALERT_VIB_TYPE9,
		SETTING_CALL_ALERT_VIB_TYPE10,
		SETTING_CALL_ALERT_VIB_TYPE11,
		SETTING_CALL_ALERT_VIB_TYPE12,
		SETTING_CALL_ALERT_VIB_TYPE13,
		SETTING_CALL_ALERT_VIB_TYPE14,
		SETTING_CALL_ALERT_VIB_TYPE15,
		SETTING_CALL_ALERT_VIB_TYPE16,
		SETTING_CALL_ALERT_VIB_TYPE17,
		SETTING_CALL_ALERT_VIB_TYPE18,
		SETTING_CALL_ALERT_VIB_TYPE19,
		SETTING_CALL_ALERT_VIB_TYPE20,
		SETTING_CALL_ALERT_VIB_MAX
};

/**
 *
 */
#define VCONFKEY_SETAPPL_NOTI_MSG_ALERT_REP_TYPE_INT  "db/setting/sound/noti/msg_alert_rep_type"

enum {
	SETTING_SOUND_REP_ONCE = 0,
		SETTING_SOUND_REP_EVERY_2_MIN,
		SETTING_SOUND_REP_EVERY_5_MIN,
		SETTING_SOUND_REP_EVERY_10_MIN
};

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *  Header only key to keep ABI test successful
 */
#define VCONFKEY_SETAPPL_NOTI_RINGTONE_PATH_STR  "db/setting/sound/noti/msg_ringtone_path"

/**
 *  msg-service is using this key. so DO NOT REMOVE. (sangkoo.kim@samsung.com)
 */
#define VCONFKEY_SETAPPL_NOTI_MSG_RINGTONE_PATH_STR  "db/setting/sound/noti/msg_ringtone_path"

/**
 *
 */
#define VCONFKEY_SETAPPL_NOTI_RINGTONE_DEFAULT_PATH_STR  "db/setting/sound/noti/ringtone_default_path"

/**
 *
 */
#define VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT  "db/setting/sound/noti/vibration_level"

/**
 *
 */
#define VCONFKEY_SETAPPL_NOTI_VIBRATION_LONG_BUZZ  "db/setting/sound/noti/vibration_long_buzz"

/**
 *
 */
#define VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT  "db/setting/sound/touch_feedback/vibration_level"

enum {
	SETTING_VIB_FEEDBACK_LEVEL0 = 0,
		SETTING_VIB_FEEDBACK_LEVEL1,
		SETTING_VIB_FEEDBACK_LEVEL2,
		SETTING_VIB_FEEDBACK_LEVEL3,
		SETTING_VIB_FEEDBACK_LEVEL4,
		SETTING_VIB_FEEDBACK_LEVEL5
};

/**
 *
 */
#define VCONFKEY_SETAPPL_USB_IN_MODE_CHANGE  "memory/setting/usb_in_mode_change"

/**
 *
 */
#define VCONFKEY_SETAPPL_USB_MODE_INT  "memory/setting/usb_mode"

enum {
	SETTING_USB_NONE_MODE = -1,
		SETTING_USB_DEFAULT_MODE = 0,
		SETTING_USB_SAMSUNG_KIES = 0,
		SETTING_USB_DEBUG_MODE = 3,
		SETTING_USB_ETHERNET_MODE = 3,
		SETTING_USB_TETHERING_MODE = 4,
		SETTING_USB_ACCESSORY_MODE = 5
};

/**
 *
 */
#define VCONFKEY_SETAPPL_USB_DEBUG_MODE_BOOL  "db/setting/debug_mode"

/**
 *
 */
#define VCONFKEY_SETAPPL_USB_CONNECTION_MODE_INT  "db/setting/usb_connection_mode"

enum {
	SETTING_USB_CONNECTION_MODE_CHARGING_ONLY = 0,
		SETTING_USB_CONNECTION_MODE_USB_DEBUGGING
};

/**
 *
 */
#define VCONFKEY_SETAPPL_SELECT_NETWORK_INT  "db/setting/select_network"

enum {
	SETTING_SELECT_NETWORK_AUTOMATIC = 0,
		SETTING_SELECT_NETWORK_MANUAL,
		SETTING_SELECT_NETWORK_MAX
};

/**
 *
 */
#define VCONFKEY_SETAPPL_NETWORK_MODE_INT  "db/setting/network_mode"

enum {
	SETTING_NETWORK_MODE_AUTOMATIC = 0,
		SETTING_NETWORK_MODE_GSM_900_1800,
		SETTING_NETWORK_MODE_GSM_850_1900,
		SETTING_NETWORK_MODE_UTMS,
		SETTING_NETWORK_MODE_MAX
};

/**
 *
 */
#define VCONFKEY_SETAPPL_STATE_POWER_ON_LOCK_BOOL  "db/setting/power_on_lock"

/**
 *
 */
#define VCONFKEY_SETAPPL_PHONE_LOCK_ATTEMPTS_LEFT_INT  "db/setting/phone_lock_attempts_left"

/**
 *
 */
#define VCONFKEY_SETAPPL_PHONE_LOCK_TIMESTAMP_STR  "db/setting/phone_lock_timestamp"

/**
 *
 */
#define VCONFKEY_SETAPPL_SIM_LOCK_TIMESTAMP_STR  "db/setting/sim_lock_timestamp"

/**
 *
 */
#define VCONFKEY_SETAPPL_STATE_SIMPLE_PASSWORD_BOOL  "db/setting/simple_password"

/**
 *
 */
#define VCONFKEY_SETAPPL_SCREEN_LOCK_TYPE_INT  "db/setting/screen_lock_type"

enum {
	SETTING_SCREEN_LOCK_TYPE_NONE = 0,
		SETTING_SCREEN_LOCK_TYPE_SWIPE,
		SETTING_SCREEN_LOCK_TYPE_MOTION,
		SETTING_SCREEN_LOCK_TYPE_FINGERPRINT,
		SETTING_SCREEN_LOCK_TYPE_FACE_AND_VOICE,
		SETTING_SCREEN_LOCK_TYPE_SIMPLE_PASSWORD,
		SETTING_SCREEN_LOCK_TYPE_PASSWORD,
		SETTING_SCREEN_LOCK_TYPE_AUTO_LOCK,
		SETTING_SCREEN_LOCK_TYPE_OTHER,
		SETTING_SCREEN_LOCK_TYPE_PATTERN,
		SETTING_SCREEN_LOCK_TYPE_MAX
};

/**
 *
 */
#define VCONFKEY_SETAPPL_3RD_LOCK_PKG_NAME_STR  "db/setting/3rd_lock_pkg_name"

/**
 *
 */
#define VCONFKEY_SETAPPL_FONT_SIZE_INT  "db/setting/font_size"

enum {
	SETTING_FONT_SIZE_SMALL = 0,
		SETTING_FONT_SIZE_MIDDLE,
		SETTING_FONT_SIZE_LARGE,
		SETTING_FONT_SIZE_MAX
};

/**
 *
 */
#define VCONFKEY_SETAPPL_FONT_TYPE_INT  "db/setting/font_type"

/**
 *
 */
#define VCONFKEY_SETAPPL_BRIGHTNESS_AUTOMATIC_INT  "db/setting/brightness_automatic"

enum {
	SETTING_BRIGHTNESS_AUTOMATIC_OFF   = 0,
		SETTING_BRIGHTNESS_AUTOMATIC_ON   ,
		SETTING_BRIGHTNESS_AUTOMATIC_PAUSE
};

/**
 *
 */
#define VCONFKEY_SETAPPL_STATE_AUTOMATIC_TIME_UPDATE_BOOL  "db/setting/automatic_time_update"

/**
 *
 */
#define VCONFKEY_SETAPPL_DATE_FORMAT_INT  "db/setting/date_format"

enum {
	SETTING_DATE_FORMAT_DD_MM_YYYY = 0,
		SETTING_DATE_FORMAT_MM_DD_YYYY,
		SETTING_DATE_FORMAT_YYYY_MM_DD,
		SETTING_DATE_FORMAT_YYYY_DD_MM,
		SETTING_DATE_FORMAT_MAX
};

/**
 *
 */
#define VCONFKEY_SETAPPL_CITYNAME_INDEX_INT  "db/setting/cityname_id"

/**
 *
 */
#define VCONFKEY_SETAPPL_WEEKOFDAY_FORMAT_INT  "db/setting/weekofday_format"

enum {
	SETTING_WEEKOFDAY_FORMAT_SUNDAY = 0,
		SETTING_WEEKOFDAY_FORMAT_MONDAY,
		SETTING_WEEKOFDAY_FORMAT_TUESDAY,
		SETTING_WEEKOFDAY_FORMAT_WEDNESDAY,
		SETTING_WEEKOFDAY_FORMAT_THURSDAY,
		SETTING_WEEKOFDAY_FORMAT_FRIDAY,
		SETTING_WEEKOFDAY_FORMAT_SATURDAY,
		SETTING_WEEKOFDAY_FORMAT_MAX
};

/**
 *
 */
#define VCONFKEY_SETAPPL_LANG_INT  "db/setting/lang"

enum {
	SETTING_LANG_AUTOMATIC = 0,
		SETTING_LANG_KOREA,
		SETTING_LANG_ENGLISH,
		SETTING_LANG_CHINA ,
		SETTING_LANG_CANTONESE,
		SETTING_LANG_TAIWAN,
		SETTING_LANG_GERMAN,
		SETTING_LANG_DUTCH,
		SETTING_LANG_SPAINISH,
		SETTING_LANG_PORTUGUESE,
		SETTING_LANG_GREEK,
		SETTING_LANG_ITALIAN,
		SETTING_LANG_FRENCH,
		SETTING_LANG_TURKISH,
		SETTING_LANG_JAPAN,
		SETTING_LANG_RUSSIAN,
		SETTING_LANG_MAX
};

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_FONT_SIZE  "db/setting/accessibility/font_size"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_FONT_NAME  "db/setting/accessibility/font_name"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_TORCH_LIGHT  "memory/setting/accessibility/torch_light"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_RAPID_KEY_INPUT  "db/setting/accessibility/rapid_key_input"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_TURN_OFF_ALL_SOUNDS  "db/setting/accessibility/turn_off_all_sounds"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_LED_NOTIFY  "db/setting/accessibility/led_notify"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_LOCK_TIME_INT  "db/setting/accessibility/lock_time"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_POWER_KEY_HOLD  "db/setting/accessibility/power_key_hold"

enum {
	SETTING_POWERKEY_SHORTCUT_OFF = 0,
		SETTING_POWERKEY_SHORTCUT_ALWAYS_ASK = 1,
		SETTING_POWERKEY_SHORTCUT_SCREEN_READER_TTS,
		SETTING_POWERKEY_SHORTCUT_NEGATIVE_COLOURS,
		SETTING_POWERKEY_SHORTCUT_ZOOM,
		SETTING_POWERKEY_SHORTCUT_ASSISTIVE_LIGHT,
		SETTING_POWERKEY_SHORTCUT_SHOT_READER,
		SETTING_POWERKEY_SHORTCUT_ACCESSIBILITY,
		SETTING_POWERKEY_SHORTCUT_MAX
};

/**
 *
 */
#define VCONFKEY_SETAPPL_EASY_TOUCH_MODE_BOOL  "db/setting/accessibility/easy_touch_mode"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_TTS  "db/setting/accessibility/tts"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_GREYSCALE  "db/setting/accessibility/greyscale"

/**
 *
 */
#define VCONFKEY_SETAPPL_PWRSV_CUSTMODE_BLTIME  "db/setting/pwrsv/custom_mode/backlight/time"

enum {
	SETTING_BACKLIGHT_TIME_8SEC = 8,
		SETTING_BACKLIGHT_TIME_15SEC = 15,
		SETTING_BACKLIGHT_TIME_30SEC = 30,
		SETTING_BACKLIGHT_TIME_1MIN = 60,
		SETTING_BACKLIGHT_TIME_3MIN = 180,
		SETTING_BACKLIGHT_TIME_10MIN = 600,
		SETTING_BACKLIGHT_TIME_MAX
};

/**
 *
 */
#define VCONFKEY_SETAPPL_AOD_ON_PIXEL_RATIO  "db/setting/aod_on_pixel_ratio"

/**
 *
 */
#define VCONFKEY_SETAPPL_PWRSV_CUSTMODE_CPU  "db/setting/pwrsv/custom_mode/cpu"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_TICKER_NOTI_DISPLAY_CONTENT_MESSASGES  "db/setting/ticker_noti/display_content/messages"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_TICKER_NOTI_DISPLAY_CONTENT_EMAIL  "db/setting/ticker_noti/display_content/email"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_TICKER_NOTI_DISPLAY_CONTENT_IM  "db/setting/ticker_noti/display_content/im"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_TICKER_NOTI_BADGE_EMAIL  "db/setting/ticker_noti/badge/email"

/**
 *
 */
#define VCONFKEY_LANGSET  "db/menu_widget/language"

/**
 *  This allows users to specify a list of locales that will be used in that order.
 */
#define VCONFKEY_SETAPPL_LANGUAGES  "db/setting/languages"

/**
 *
 */
#define VCONFKEY_REGIONFORMAT  "db/menu_widget/regionformat"

/**
 *
 */
#define VCONFKEY_SETAPPL_SELECTED_PACKAGE_NAME  "db/setting/menuscreen/package_name"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_SETAPPL_HOMESCREEN_EASYMODE_BOOL  "db/setting/homescreen/easymode"

/**
 *
 */
#define VCONFKEY_SETAPPL_ROAMING_NETWORK_INT  "db/setting/roaming_network"

enum {
	SETTING_ROAM_NET_AUTO_DOWNLOAD = 0,
		SETTING_ROAM_NET_MANUAL,
		SETTING_ROAM_NET_ALWAYS_REJECT,
		SETTING_ROAM_NET_MAX
};

/**
 *
 */
#define VCONFKEY_SETAPPL_DEFAULT_MEM_INSTALL_APPLICATIONS_INT  "db/setting/default_memory/install_applications"

/**
 *
 */
#define VCONFKEY_SETAPPL_DEFAULT_MEM_WIFI_DIRECT_INT  "db/setting/default_memory/wifi_direct"

/**
 *
 */
#define VCONFKEY_SETAPPL_DEFAULT_MEM_BLUETOOTH_INT  "db/setting/default_memory/bluetooth"

/**
 *
 */
#define VCONFKEY_SETAPPL_BATTERY_PERCENTAGE_BOOL  "db/setting/battery_percentage"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_HIGH_CONTRAST  "db/setting/accessibility/high_contrast"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_SCREEN_ZOOM  "db/setting/accessibility/screen_zoom"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_SCREEN_READER  "db/setting/accessibility/screen_reader"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_SCREEN_READER_DESCRIPTION  "db/setting/accessibility/screen_reader/description"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_SCREEN_READER_GRANULARITY  "db/setting/accessibility/screen_reader/granularity"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_SCREEN_READER_HAPTIC  "db/setting/accessibility/screen_reader/haptic"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_SCREEN_READER_KEYBOARD_FEEDBACK  "db/setting/accessibility/screen_reader/keyboard_feedback"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_SCREEN_READER_SOUND_FEEDBACK  "db/setting/accessibility/screen_reader/sound_feedback"

/**
 *
 */
#define VCONFKEY_SETAPPL_TIMEZONE_ID  "db/setting/timezone_id"

/**
 *
 */
#define VCONFKEY_SETAPPL_TIMEZONE_INT  "db/setting/timezone"

/**
 *
 */
#define VCONFKEY_SETAPPL_ROTATE_LOCK_BOOL  "db/setting/rotate_lock"

/**
 *
 */
#define VCONFKEY_SETAPPL_AUTO_ROTATE_SCREEN_BOOL  "db/setting/auto_rotate_screen"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_MONO_AUDIO  "db/setting/accessibility/mono_audio"

/**
 *
 */
#define VCONFKEY_REGIONFORMAT_TIME1224  "db/menu_widget/regionformat_time1224"

enum {
	VCONFKEY_TIME_FORMAT_12 = 1,
		VCONFKEY_TIME_FORMAT_24
};

/**
 *
 */
#define VCONFKEY_SETAPPL_SIM_LOCK_ATTEMPTS_LEFT_INT  "db/setting/sim_lock_attempts_left"

/**
 *
 */
#define VCONFKEY_SETAPPL_DEVICE_NAME_STR  "db/setting/device_name"

/**
 *
 */
#define VCONFKEY_SETAPPL_SOUND_STATUS_BOOL  "db/setting/sound/sound_on"

/**
 *
 */
#define VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL  "db/setting/sound/vibration_on"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
신규 구현중
 */
#define VCONFKEY_SETAPPL_SOUND_MODE  "db/setting/sound/sound_mode"

enum {
	SETTING_SOUNDMODE_SOUND = 0,
		SETTING_SOUNDMODE_VIBRATION,
		SETTING_SOUNDMODE_SILENT,
		SETTING_SOUNDMODE_MAX
};

/**
 *
 */
#define VCONFKEY_SETAPPL_VIBRATE_WHEN_RINGING_BOOL  "db/setting/sound/vibrate_when_ringing"

/**
 *
 */
#define VCONFKEY_SETAPPL_VIBRATE_WHEN_NOTIFICATION_BOOL  "db/setting/sound/vibrate_when_notification"

/**
 *
 */
#define VCONFKEY_SETAPPL_HAPTIC_FEEDBACK_STATUS_BOOL  "db/setting/sound/haptic_feedback_on"

/**
 *
 */
#define VCONFKEY_SETAPPL_STATE_DATA_ROAMING_BOOL  "db/setting/data_roaming"

/**
 *
 */
#define VCONFKEY_SETAPPL_MOTION_ACTIVATION  "db/setting/motion_active"

/**
 *
 */
#define VCONFKEY_SETAPPL_USE_TILT  "db/setting/use_tilt"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_SETAPPL_TILT_SENSITIVITY  "db/setting/tilt_sensitivity"

/**
 *
 */
#define VCONFKEY_SETAPPL_USE_PANNING  "db/setting/use_panning"

/**
 *
 */
#define VCONFKEY_SETAPPL_USE_PANNING_BROWSER  "db/setting/use_panning_browser"

/**
 *
 */
#define VCONFKEY_SETAPPL_PANNING_SENSITIVITY  "db/setting/panning_sensitivity"

/**
 *
 */
#define VCONFKEY_SETAPPL_PANNING_BROWSER_SENSITIVITY  "db/setting/panning_browser_sensitivity"

/**
 *
 */
#define VCONFKEY_SETAPPL_USE_DOUBLE_TAP  "db/setting/use_double_tap"

/**
 *
 */
#define VCONFKEY_SETAPPL_USE_TURN_OVER  "db/setting/use_turn_over"

/**
 *
 */
#define VCONFKEY_SETAPPL_USE_PICK_UP  "db/setting/use_pick_up"

/**
 *
 */
#define VCONFKEY_SETAPPL_USE_PICK_UP_CALL  "db/setting/use_pick_up_call"

/**
 *
 */
#define VCONFKEY_SETAPPL_USE_SHAKE  "db/setting/use_shake"

/**
 *
 */
#define VCONFKEY_SETAPPL_USE_MUTE_PAUSE  "db/setting/use_mute_pause"

/**
 * SYSTEM_SETTINGS_KEY_WALLPAPER_HOME_SCREEN(system-settings) refers to this key, READ for public, WRITE for platform
 */
#define VCONFKEY_BGSET  "db/menu_widget/bgset"

/**
 *
 */
#define VCONFKEY_SETAPPL_DEVOPTION_BGPROCESS  "db/setting/devoption/bgprocess"

/**
 *
 */
#define VCONFKEY_SETAPPL_DEFAULT_RENDERING_ENGINE_TYPE_INT  "db/setting/default_rendering_engine"

enum {
	SETTING_RENDERING_ENGINE_HW ,
		SETTING_RENDERING_ENGINE_SW ,
		SETTING_RENDERING_ENGINE_MAX
};

/**
 *
 */
#define VCONFKEY_SETAPPL_DATA_LIMIT_INT  "db/setting/data_limit"

/**
 *
 */
#define VCONFKEY_SETAPPL_SELECT_OLD_NT_ACT  "db/setting/select_network_act"

/**
 *
 */
#define VCONFKEY_SETAPPL_LANG_AUTOMATIC_BOOL  "db/setting/lang_automatic"

/**
 *
 */
#define VCONFKEY_SETAPPL_DATA_USAGE_CYCLE_INT  "db/setting/data_usage_cycle"

/**
 *
 */
#define VCONFKEY_SETAPPL_DATA_EACH_MONTH  "db/setting/data_each_month"

/**
 *
 */
#define VCONFKEY_SETAPPL_DATA_LIMIT_UNIT_BOOL  "db/setting/data_limit_unit"

/**
 *
 */
#define VCONFKEY_SETAPPL_DATA_USAGE_ROAMING_STATUS  "db/setting/data_usage_roaming_status"

/**
 *
 */
#define VCONFKEY_SETAPPL_SET_DATA_USAGE_LIMIT_BOOL  "db/setting/set_data_usage_limit"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_TAP_HOLD_DELAY_STR  "db/setting/accessibility/taphold_delay"

/**
 *
 */
#define VCONFKEY_SETAPPL_SOUND_BALANCE_INT  "db/setting/accessibility/sound_balance"

/**
 *
 */
#define VCONFKEY_SETAPPL_SELECTED_NUM  "db/setting/selected_num"

/**
 *
 */
#define VCONFKEY_SETAPPL_SCREENMODE_SELNAME  "db/setting/screenmode/selected_name"

/**
 *
 */
#define VCONFKEY_LCD_TIMEOUT_NORMAL_BACKUP  "db/setting/lcd_timeout_normal_backup"

/**
 *
 */
#define VCONFKEY_SETAPPL_LED_INDICATOR_CHARGING  "db/setting/led_indicator/charging"

/**
 *
 */
#define VCONFKEY_SETAPPL_LED_INDICATOR_LOW_BATT  "db/setting/led_indicator/low_batt"

/**
 *
 */
#define VCONFKEY_SETAPPL_LED_INDICATOR_NOTIFICATIONS  "db/setting/led_indicator/notifications"

/**
 *
 */
#define VCONFKEY_SETAPPL_ROTATE_HOLD_BOOL  "memory/setting/rotate_hold"

/**
 *
 */
#define VCONFKEY_SETAPPL_PSMODE  "db/setting/psmode"

enum {
	SETTING_PSMODE_NORMAL = 0,
		SETTING_PSMODE_POWERFUL = 1,
		SETTING_PSMODE_ADVISOR = 1,
		SETTING_PSMODE_SURVIVAL = 2,
		SETTING_PSMODE_EMERGENCY = 2,
		SETTING_PSMODE_WEARABLE = 3,
		SETTING_PSMODE_WEARABLE_ENHANCED = 4,
		SETTING_PSMODE_MAX = 0
};

/**
 *
 */
#define VCONFKEY_SETAPPL_MMC_ENCRYPTION_STATUS_BOOL  "db/setting/mmc_encryption/status"

/**
 *
 */
#define VCONFKEY_SETAPPL_TOUCHKEY_LIGHT_DURATION  "db/setting/display/touchkey_light_duration"

enum {
	SETTING_TOUCHKEY_LIGIT_DURATION_90SEC = 90,
		SETTING_TOUCHKEY_LIGIT_DURATION_360SEC = 360,
		SETTING_TOUCHKEY_LIGIT_DURATION_ALWAYS_OFF = 0,
		SETTING_TOUCHKEY_LIGIT_DURATION_ALWAYS_ON = -1
};

/**
 *
 */
#define VCONFKEY_SETAPPL_SCREEN_CAPTURE_DESTINATION  "db/setting/display/screen_capture_destination"

enum {
	SETTING_SCREEN_CAPTURE_DESTINATION_ASK = 0,
		SETTING_SCREEN_CAPTURE_DESTINATION_GALLERY,
		SETTING_SCREEN_CAPTURE_DESTINATION_KEEPIT
};

/**
 *
 */
#define VCONFKEY_SETAPPL_SCREEN_CAPTURE_EDIT_AFTER_CAPTURE  "db/setting/display/edit_after_screen_capture"

/**
 *
 */
#define VCONFKEY_SETAPPL_NETWORK_RESTRICT_MODE  "db/setting/network_restrict_mode"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_SETAPPL_BLOCKINGMODE_LED_INDICATOR  "db/setting/blockingmode/led_indicator"

/**
 *
 */
#define VCONFKEY_SETAPPL_DYNAMIC_STATUS_BAR  "db/setting/display/dynamic_status_bar"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_SETAPPL_CLOCK_TYPE_INT  "db/setting/idle_clock_type"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_SETAPPL_HOURLY_ALERT_BOOL  "db/setting/hourly_alert"

/**
 *  it's for wearable
 */
#define VCONFKEY_SETAPPL_HOMESCREEN_TYPE_INT  "db/setting/homescreen_type"

enum {
	SETTING_HOMESCREEN_TYPE_1_1 = 0,
		SETTING_HOMESCREEN_TYPE_2_2 = 1
};

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_SETAPPL_MOBILE_DATA_POPUP_DONE  "db/setting/mobile_data_popup"

/**
 *
 */
#define VCONFKEY_SETAPPL_MOBILE_DATA_ON_REMINDER  "db/setting/network/mobile_data_on_reminder"

/**
 *
 */
#define VCONFKEY_SETAPPL_MOBILE_DATA_OFF_REMINDER  "db/setting/network/mobile_data_off_reminder"

/**
 *
 */
#define VCONFKEY_SETAPPL_USB_OTG_MODE  "db/setting/usb_otg"

/**
 *
 */
#define VCONFKEY_SETAPPL_FIREWALL_KEY  "db/setting/firewall"

/**
 *
 */
#define VCONFKEY_SETAPPL_PERSONAL_MODE_STATUS_BOOL  "memory/setting/personal"

/**
 *
 */
#define VCONFKEY_SETAPPL_PERSONAL_MODE_UNLOCK_METHOD_INT  "db/setting/personal_unlock_method"

/**
 *
 */
#define VCONFKEY_SETAPPL_DEVELOPER_OPTION_STATE  "db/setting/developer_option_state"

/**
 *
 */
#define VCONFKEY_SETAPPL_APP_HW_ACCELERATION  "db/setting/app_hw_acceleration"

enum {
	SETTING_HW_ACCELERATION_NONE = -1,
		SETTING_HW_ACCELERATION_OFF = 0,
		SETTING_HW_ACCELERATION_ON = 1
};

/**
 *
 */
#define VCONFKEY_SETAPPL_CHANGE_UI_THEME_INT  "db/setting/change_ui/theme"

/**
 *
 */
#define VCONFKEY_SETAPPL_WINDOW_GRAYTONE  "db/setting/win_gray"

/**
 *
 */
#define VCONFKEY_SETAPPL_WINDOW_FPS  "db/setting/win_fps"

/**
 *
 */
#define VCONFKEY_SETAPPL_EMERGENCY_UI_MODE  "db/setting/emergency/mode"

enum {
	EMERGENCY_UI_MODE_EMERGENCY = 1,
		EMERGENCY_UI_MODE_ULTRA_POWER_SAVING,
		EMERGENCY_UI_MODE_MAX
};

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_DARK_SCREEN  "db/setting/accessibility/dark_screen"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_SETAPPL_NETWORK_PERMIT_WITH_LCD_OFF_LIMIT  "db/setting/network_with_lcd_off_limit"

/**
 *
 */
#define VCONFKEY_SETAPPL_SIM1_NAME  "db/setting/sim1_name"

/**
 *
 */
#define VCONFKEY_SETAPPL_SIM2_NAME  "db/setting/sim2_name"

/**
 *
 */
#define VCONFKEY_SETAPPL_SIM1_ICON  "db/setting/sim1_icon"

/**
 *
 */
#define VCONFKEY_SETAPPL_SIM2_ICON  "db/setting/sim2_icon"

enum {
	SIM_ICON_SIM1     = 0,
		SIM_ICON_SIM2    ,
		SIM_ICON_CALL    ,
		SIM_ICON_SMS     ,
		SIM_ICON_MMS     ,
		SIM_ICON_INTERNET,
		SIM_ICON_HOME    ,
		SIM_ICON_OFFICE  ,
		SIM_ICON_HEART   ,
		SIM_ICON_MAX
};

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_DUALSIM_VOICE_CALL_INT  "db/setting/dualsim/voice_call"

enum {
	VCONFKEY_DUALSIM_VOICE_CALL_UNKNOWN = -1,
		VCONFKEY_DUALSIM_VOICE_CALL_CURRENT_NETWORK = 0,
		VCONFKEY_DUALSIM_VOICE_CALL_ASK_ALWAYS,
		VCONFKEY_DUALSIM_VOICE_CALL_SIM1,
		VCONFKEY_DUALSIM_VOICE_CALL_SIM2
};

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_TELEPHONY_ALWAYS_ON_BOOL  "db/setting/dualsim/always_dualsim"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_DUALSIM_SELECTED_SIM  "db/setting/dualsim/selected_sim"

enum {
	VCONFKEY_DUALSIM_SELECT_UNKNOW = -1,
		VCONFKEY_DUALSIM_SELECT_BOTH_SIM = 0,
		VCONFKEY_DUALSIM_SELECT_SIM1_TO_SIM2,
		VCONFKEY_DUALSIM_SELECT_SIM2_TO_SIM1
};

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_SETAPPL_SCREENROTATION_DEG_INT  "db/setting/screen_rotation"

enum {
	SETTING_SCREENROTATION_0_DEGREE = 0,
		SETTING_SCREENROTATION_90_DEGREE = 1,
		SETTING_SCREENROTATION_180_DEGREE = 2,
		SETTING_SCREENROTATION_270_DEGREE = 3
};

/**
 *
 */
#define VCONFKEY_SETAPPL_PRIVACY_LOCK_TYPE_INT  "db/setting/lock_type"

enum {
	SETTING_PRIVACY_LOCK_TYPE_NONE = 0,
		SETTING_PRIVACY_LOCK_TYPE_PATTERN = 1
};

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *  it's for Wearable
 */
#define VCONFKEY_SETAPPL_EMERGENCY_STATUS_BOOL  "db/setting/emergency_status"

/**
 *
 */
#define VCONFKEY_SETAPPL_BLOCKMODE_WEARABLE_R_BOOL  "db/setting/blockmode_wearable_r"

/**
 *  ambient mode
 */
#define VCONFKEY_SETAPPL_AMBIENT_MODE_BOOL  "db/setting/ambient_mode"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_SETAPPL_WALLPAPER_CHANGED_NOTI_INT  "db/setting/wallpaper_changed"

enum {
	VCONFKEY_WALLPAPER_CHANGED_NOTI_NONE = 0,
		VCONFKEY_WALLPAPER_CHANGED_NOTI_GEAR = 1,
		VCONFKEY_WALLPAPER_CHANGED_NOTI_HM = 2
};

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_TTS_INDICATOR_INFORMATION_TIME  "db/setting/accessibility/tts/indicator/time"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_TTS_INDICATOR_INFORMATION_BATTERY  "db/setting/accessibility/tts/indicator/battery"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_TTS_INDICATOR_INFORMATION_SIGNAL_STRENGHT  "db/setting/accessibility/tts/indicator/signal_strenght"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_TTS_INDICATOR_INFORMATION_MISSED_EVENTS  "db/setting/accessibility/tts/indicator/missed_events"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_TTS_INDICATOR_INFORMATION_DATE  "db/setting/accessibility/tts/indicator/date"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_TTS_INDICATOR_INFORMATION_BLUETOOTH  "db/setting/accessibility/tts/indicator/bluetooth"

/**
 *
 */
#define VCONFKEY_SETAPPL_NOTIFICATION_INDICATOR  "db/setting/notification_indicator"

/**
 *
 */
#define VCONFKEY_SETAPPL_DOUBLE_PRESS_HOME_KEY  "db/setting/double_press_home_key"

enum {
	VCONFKEY_DOUBLE_PRESS_HOME_KEY_NONE = 0,
		VCONFKEY_DOUBLE_PRESS_HOME_KEY_LAST_APP = 1,
		VCONFKEY_DOUBLE_PRESS_HOME_KEY_RECENT_APPS = 2
};

/**
 *
 */
#define VCONFKEY_SETAPPL_AUTO_OPEN_APPS  "db/setting/auto_open_apps"

/**
 * Used to set/unset 'Do Not Distrub' mode
 */
#define VCONFKEY_SETAPPL_BLOCKMODE_WEARABLE_BOOL  "db/setting/blockmode_wearable"

/**
 *
 */
#define VCONFKEY_SETAPPL_D2D_CONVERGENCE  "db/setting/d2d_convergence"

/**
 *
 */
#define VCONFKEY_SETAPPL_D2D_CONVERGENCE_SERVICE  "db/setting/d2d_convergence_service"

/**
 * initialized value : set from adid UI app
 */
#define VCONFKEY_SETAPPL_AD_ID_OPT_OUT  "db/setting/ad_id_opt_out"

/**
 * initialized value : set from adid UI app/first boot value from setup wizard app.
 */
#define VCONFKEY_SETAPPL_AD_ID  "db/setting/ad_id"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *  b2clocksetting use this key for setting-sound
 */
#define VCONFKEY_SETAPPL_PERFERED_ARM_LEFT_BOOL  "db/setting/prefer_arm_left"

/**
 *  org.tizen.whome refers to this key, bt-syspopup refers to VCONFKEY_SETUP_WIZARD_LOCK.
 */
#define VCONFKEY_SETUP_WIZARD_STATE  "memory/setup_wizard/state"

enum {
	VCONFKEY_SETUP_WIZARD_UNLOCK = 0x00,
		VCONFKEY_SETUP_WIZARD_LOCK
};

/**
 *
 */
#define VCONFKEY_SETAPPL_LCD_DIM_BRIGHTNESS  "db/setting/Brightness_Dim"

/**
 *
 */
#define VCONFKEY_SETAPPL_DATA_RESTRICTION_INT  "db/setting/data_restriction"

enum {
	RESTRICTION_STATE_INIT = -1,
		RESTRICTION_STATE_UNSET = 0,
		RESTRICTION_STATE_SET = 1
};

/**
 *
 */
#define VCONFKEY_SETAPPL_ENHANCE_TOUCH_SENSITIVITY  "db/setting/enhance_touch_sensitivity"

/**
 *
 */
#define VCONFKEY_SETAPPL_WAKEUP_BY_BEZEL_ENABLE  "db/setting/wakeup_by_bezel_enable"

/**
 *
 */
#define VCONFKEY_SETAPPL_THEATER_MODE_ENABLE  "db/setting/theater_mode_enable"

/**
 *
 */
#define VCONFKEY_SETAPPL_GOODNIGHT_MODE_ENABLE  "db/setting/goodnight_mode_enable"

/**
 *  LBM module of deviced refers to this key.
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_AUTOMATIC_LOW_BRIGHTNESS  "db/setting/automatic_low_brightness"

/**
 *
 */
#define VCONFKEY_SETAPPL_ROTARY_EVENT_ENABLED_BOOL  "db/setting/rotary_event_enabled"

/**
 *  Display wakeup from LCDOFF by touchscreen.
 */
#define VCONFKEY_SETAPPL_TOUCH_WAKEUP_ENABLE  "db/setting/touch_wakeup_enable"

/**
 *  Diagnostics
 */
#define VCONFKEY_SETAPPL_DIAGNOSTICS  "db/setting/diagnostics"

/**
 *  Diagnostics
 */
#define VCONFKEY_SETAPPL_DIAGNOSTICS_IDENTIFIED  "db/setting/diagnostics/identified"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_SWIPE_UP_DOWN_ENABLE  "db/setting/accessibility/swipe_up_down_enable"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_SWIPE_UP_DOWN_TYPE  "db/setting/accessibility/swipe_up_down_type"

enum {
	VCONFKEY_SETAPPL_ACCESSIBILITY_SWIPE_UP_DOWN_TYPE_ACCESSIBILITY_VOLUME = 0,
		VCONFKEY_SETAPPL_ACCESSIBILITY_SWIPE_UP_DOWN_TYPE_SPEECH_RATE = 1,
		VCONFKEY_SETAPPL_ACCESSIBILITY_SWIPE_UP_DOWN_TYPE_DARK_SCREEN = 2,
		VCONFKEY_SETAPPL_ACCESSIBILITY_SWIPE_UP_DOWN_TYPE_CHARACTER_GRANULARITY_CHARACTERS = 3,
		VCONFKEY_SETAPPL_ACCESSIBILITY_SWIPE_UP_DOWN_TYPE_CHARACTER_GRANULARITY_WORDS = 4
};

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_VIBRATION_FEEDBACK_ENABLE  "db/setting/accessibility/vibration_feedback_enable"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_SOUND_FEEDBACK_ENABLE  "db/setting/accessibility/sound_feedback_enable"

/**
 *
 */
#define VCONFKEY_SETAPPL_SOFT_KEY  "db/setting/soft_key"

/**
 *  It is to enable or disable Doze Mode.
 */
#define VCONFKEY_SETAPPL_DOZE_MODE  "db/setting/doze_mode"

/**
 *
 */
#define VCONFKEY_SETAPPL_TRAY_TURNUPMODE  "db/setting/tray/turnupmode"

enum {
	VCONFKEY_SETAPPL_TRAY_TURNUPMODE_NORMAL = 0,
		VCONFKEY_SETAPPL_TRAY_TURNUPMODE_HIDDEN = 1
};

/**
 *
 */
#define VCONFKEY_SETAPPL_TRAY_SORTORDER  "db/setting/tray/sortorder"

enum {
	VCONFKEY_SETAPPL_TRAY_SORTORDER_AUTO = 0,
		VCONFKEY_SETAPPL_TRAY_SORTORDER_CUSTOM = 1
};

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_SOUND_FEEDBACK_THEME  "db/setting/accessibility/sound_feedback_theme"

/**
 *
 */
#define VCONFKEY_SETAPPL_ACCESSIBILITY_VIBRATION_FEEDBACK_THEME  "db/setting/accessibility/vibration_feedback_theme"

/**
 *
 */
#define VCONFKEY_SETAPPL_OOBE  "db/setting/oobe"

/**
 * It's a current theme ID.
 */
#define VCONFKEY_SETAPPL_CURRENT_THEME_ID  "db/setting/current_theme/id"

#endif /*   __VCONF_INTERNAL_SETTING_KEYS_H__    */
