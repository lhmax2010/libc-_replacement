/*
 * Copyright (c) 2011-2020 Samsung Electronics Co., Ltd All Rights Reserved
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
#ifndef __TIZEN_SYSTEM_SYSTEM_SETTINGS_H__
#define __TIZEN_SYSTEM_SYSTEM_SETTINGS_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file system_settings.h
 */

/**
 * @addtogroup CAPI_SYSTEM_SYSTEM_SETTINGS_MODULE
 * @{
 */

/**
 * @brief Enumeration for system settings error.
 * @since_tizen 2.3
 */
typedef enum {
	SYSTEM_SETTINGS_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	SYSTEM_SETTINGS_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	SYSTEM_SETTINGS_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< Internal I/O error */
	SYSTEM_SETTINGS_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported (Since 2.3.1) */
	SYSTEM_SETTINGS_ERROR_CALL_UNSUPPORTED_API = TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported (Since 2.3.1) */

	/* lock screen app error code */
	SYSTEM_SETTINGS_ERROR_LOCKSCREEN_APP_PASSWORD_MODE = TIZEN_ERROR_SYSTEM_SETTING | 0x01, /**< Current lock screen app set 'password' type */
} system_settings_error_e;

/**
 * @brief Enumeration for System Settings Key.
 * @since_tizen 2.3
 */
typedef enum {
	SYSTEM_SETTINGS_KEY_INCOMING_CALL_RINGTONE, /**< (string) The file path of the current ringtone */
	SYSTEM_SETTINGS_KEY_WALLPAPER_HOME_SCREEN, /**< (string) The file path of the current home screen wallpaper */
	SYSTEM_SETTINGS_KEY_WALLPAPER_LOCK_SCREEN, /**< (string) The file path of the current lock screen wallpaper */
	SYSTEM_SETTINGS_KEY_FONT_SIZE, /**< (int) The current system font size */
	SYSTEM_SETTINGS_KEY_FONT_TYPE, /**< (string) The current system font type */
	SYSTEM_SETTINGS_KEY_MOTION_ACTIVATION, /**< (bool) Indicates whether the motion service is activated */
	SYSTEM_SETTINGS_KEY_EMAIL_ALERT_RINGTONE,  /**< (string) The file path of the current email alert ringtone */
	SYSTEM_SETTINGS_KEY_USB_DEBUGGING_ENABLED,	/**< (bool) Indicates whether the USB debugging is enabled (Since 2.4) */
	SYSTEM_SETTINGS_KEY_3G_DATA_NETWORK_ENABLED,  /**< (bool) Indicates whether the 3G data network is enabled (Since 2.4) */
	SYSTEM_SETTINGS_KEY_LOCKSCREEN_APP = SYSTEM_SETTINGS_KEY_3G_DATA_NETWORK_ENABLED + 2,	 /**< (string) Indicates lockscreen app pkg name  */
	SYSTEM_SETTINGS_KEY_DEFAULT_FONT_TYPE,/**< (string) The current system default font type (only support Get) */
	SYSTEM_SETTINGS_KEY_LOCALE_COUNTRY,		/**< (string) Indicates the current country setting in the <LANGUAGE>_<REGION> syntax. The country setting is in the ISO 639-2 format, and the region setting is in the ISO 3166-1 alpha-2 format */
	SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE,	/**< (string) Indicates the current language setting in the <LANGUAGE>_<REGION> syntax. The language setting is in the ISO 639-2 format and the region setting is in the ISO 3166-1 alpha-2 format. */
	SYSTEM_SETTINGS_KEY_LOCALE_TIMEFORMAT_24HOUR,	/**< (bool) Indicates whether the 24-hour clock is used. If the value is @c false, the 12-hour clock is used. */
	SYSTEM_SETTINGS_KEY_LOCALE_TIMEZONE,	/**< (string) Indicates the current time zone. */
	SYSTEM_SETTINGS_KEY_TIME_CHANGED,		/**< (int) Once System changes time, this event occurs to notify time change. */
	SYSTEM_SETTINGS_KEY_SOUND_LOCK,					/**< GET (bool) Indicates whether the screen lock sound is enabled on the device. ex) LCD on/off sound */
	SYSTEM_SETTINGS_KEY_SOUND_SILENT_MODE,			/**< GET (bool) Indicates whether the device is in the silent mode. */
	SYSTEM_SETTINGS_KEY_SOUND_TOUCH,				/**< GET (bool) Indicates whether the screen touch sound is enabled on the device. */
	SYSTEM_SETTINGS_KEY_DISPLAY_SCREEN_ROTATION_AUTO,	/**< GET (bool) Indicates whether rotation control is automatic.*/
	SYSTEM_SETTINGS_KEY_DEVICE_NAME,					/**< GET (string) Indicates device name. */
	SYSTEM_SETTINGS_KEY_MOTION_ENABLED,					/**< GET (bool) Indicates whether the device user has enabled the motion feature. */
	SYSTEM_SETTINGS_KEY_NETWORK_WIFI_NOTIFICATION,		/**< GET (bool) Indicates whether Wi-Fi-related notifications are enabled on the device. */
	SYSTEM_SETTINGS_KEY_NETWORK_FLIGHT_MODE,			/**< GET (bool) Indicates whether the device is in the flight mode. */
	SYSTEM_SETTINGS_KEY_SCREEN_BACKLIGHT_TIME,			/**< (int) Indicates the backlight time (in seconds). @internal The following values can be used: 15, 30, 60, 120, 300, and 600. */
	SYSTEM_SETTINGS_KEY_SOUND_NOTIFICATION,				/**< (string) Indicates the file path of the current notification tone set by the user. */
	SYSTEM_SETTINGS_KEY_SOUND_NOTIFICATION_REPETITION_PERIOD,	/**< (int) Indicates the time period for notification repetitions. */
	SYSTEM_SETTINGS_KEY_LOCK_STATE,		/**< (int) Indicates the current lock state */
	SYSTEM_SETTINGS_KEY_ADS_ID, /**< (string) Indicates Ads ID for each device (Since 3.0) */
	SYSTEM_SETTINGS_KEY_ULTRA_DATA_SAVE, /**< (int) Indicates Ultra Data Save status, one of #system_settings_uds_state_e values (Since 4.0) */
	SYSTEM_SETTINGS_KEY_ULTRA_DATA_SAVE_PKG_LIST, /**< (string) Indicates Ultra Data Save Package List (Since 4.0), the list is a string containing whitelisted package names separated with semicolons (;) */
	SYSTEM_SETTINGS_KEY_ACCESSIBILITY_TTS,				/**< GET (bool) Indicates whether the accessibility TTS is enabled on the device. (Since 4.0) */
	SYSTEM_SETTINGS_KEY_VIBRATION,				/**< (bool) Indicates whether vibration is enabled on the device. (Since 4.0) */
	SYSTEM_SETTINGS_KEY_AUTOMATIC_TIME_UPDATE,	/**< (bool) Indicates whether updating time automatically is enabled on the device. (Since 5.0) */
	SYSTEM_SETTINGS_KEY_DEVELOPER_OPTION_STATE,	/**< (bool) Indicates whether developer option state is enabled on the device. (Since 5.0) */
	SYSTEM_SETTINGS_KEY_ACCESSIBILITY_GRAYSCALE,	/**< (bool) Indicates whether accessibility grayscale is enabled on the device. (Since 5.5) */
	SYSTEM_SETTINGS_KEY_ACCESSIBILITY_NEGATIVE_COLOR,	/**< (bool) Indicates whether accessibility negative color is enabled on the device. (Since 5.5) */
	SYSTEM_SETTINGS_KEY_ROTARY_EVENT_ENABLED,	/**< (bool) Indicates whether rotary event is enabled on the device. (Since 5.5) */
	SYSTEM_SETTINGS_KEY_OOBE,	/**< (bool) Indicates whether OOBE(Out Of Box Experience) is enabled on the device. (Since 10.0) */
	SYSTEM_SETTINGS_KEY_MAX, /**< max of system settings key */
} system_settings_key_e;

/**
 * @brief Enumeration for Idle Lock State.
 * @since_tizen 2.3.1
 */
typedef enum {
	SYSTEM_SETTINGS_LOCK_STATE_UNLOCK = 0, /**< Device is unlocked */
	SYSTEM_SETTINGS_LOCK_STATE_LOCK, /**< Device is locked */
	SYSTEM_SETTINGS_LOCK_STATE_LAUNCHING_LOCK /**< Device is being locked */
} system_settings_idle_lock_state_e;

/**
 * @brief Enumeration for font size.
 * @since_tizen 2.3
 */
typedef enum {
	SYSTEM_SETTINGS_FONT_SIZE_SMALL = 0, /**< A small size */
	SYSTEM_SETTINGS_FONT_SIZE_NORMAL, /**< A normal size */
	SYSTEM_SETTINGS_FONT_SIZE_LARGE, /**< A large size */
	SYSTEM_SETTINGS_FONT_SIZE_HUGE, /**< A huge size */
	SYSTEM_SETTINGS_FONT_SIZE_GIANT, /**< A giant size */
} system_settings_font_size_e;

/**
 * @brief Enumeration for Ultra Data Save.
 * @since_tizen 4.0
 */
typedef enum {
	SYSTEM_SETTINGS_UDS_OFF = 0,			/**< OFF */
	SYSTEM_SETTINGS_UDS_ON,					/**< ON */
	SYSTEM_SETTINGS_UDS_ON_WHITELISTED,		/**< UDS on and the app is whitelisted */
} system_settings_uds_state_e;

/**
 * @brief Called when the system settings changes.
 * @since_tizen 2.3
 * @param[in] key The key name of the system settings changed system settings
 * @param[in] user_data The user data passed from the callback registration function
 * @pre system_settings_set_changed_cb() will invoke this callback function.
 * @see system_settings_set_changed_cb()
 * @see system_settings_unset_changed_cb()
 */
typedef void (*system_settings_changed_cb)(system_settings_key_e key, void *user_data);


typedef void* system_settings_cb_id;

/**
 * @platform
 * @brief Sets the system settings value associated with the given key as an integer.
 * @details This API is used to set an int value for a key with an int type value in System Settings APIs. When using this function, be careful not to include keys of other types (such as string or bool) as parameters.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/systemsettings.admin
 * @remarks When the feature related to the @a key is not supported on the device, #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED will be returned.
 * @param[in] key The key name of the system settings
 * @param[out] value The new system settings value of the given key
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SYSTEM_SETTINGS_ERROR_NONE Successful
 * @retval #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED The related feature is not supported on the device
 * @retval #SYSTEM_SETTINGS_ERROR_PERMISSION_DENIED Permission violation error
 * @retval #SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SYSTEM_SETTINGS_ERROR_IO_ERROR Internal I/O error
  */
int system_settings_set_value_int(system_settings_key_e key, int value);

/**
 * @brief Gets the system settings value associated with the given key as an integer.
 * @details This API is used to get the int value set for a key with an int type value in System Settings APIs. When using this function, be careful not to include keys of other types (such as string or bool) as parameters
 * @since_tizen 2.3
 * @remarks When the feature related to the @a key is not supported on the device, #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED will be returned.
 * @param[in] key The key name of the system settings
 * @param[out] value The current system settings value of the given key
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SYSTEM_SETTINGS_ERROR_NONE Successful
 * @retval #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED The related feature is not supported on the device
 * @retval #SYSTEM_SETTINGS_ERROR_PERMISSION_DENIED Permission violation error
 * @retval #SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SYSTEM_SETTINGS_ERROR_IO_ERROR Internal I/O error
  * @warning %http://tizen.org/privilege/systemsettings (public level privilege) <b>MUST NOT</b> be declared to use this function since 2.3.1.
 */
int system_settings_get_value_int(system_settings_key_e key, int *value);

/**
 * @platform
 * @brief Sets the system settings value associated with the given key as a boolean.
 * @details This API is used to set a bool value for a key with a bool type value in System Settings APIs. When using this function, be careful not to include keys of other types (such as string or int) as parameters.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/systemsettings.admin
 * @remarks When the feature related to the @a key is not supported on the device, #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED will be returned.
 * @param[in] key The key name of the system settings
 * @param[out] value The new system settings value of the given key
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SYSTEM_SETTINGS_ERROR_NONE Successful
 * @retval #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED The related feature is not supported on the device
 * @retval #SYSTEM_SETTINGS_ERROR_PERMISSION_DENIED Permission violation error
 * @retval #SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SYSTEM_SETTINGS_ERROR_IO_ERROR Internal I/O error
  */
int system_settings_set_value_bool(system_settings_key_e key, bool value);

/**
 * @brief Gets the system settings value associated with the given key as a boolean.
 * @details This API is used to get the bool value set for a key with a bool type value in System Settings APIs. When using this function, be careful not to include keys of other types (such as string or int) as parameters
 * @since_tizen 2.3
 * @remarks When the feature related to the @a key is not supported on the device, #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED will be returned.
 * @param[in] key The key name of the system settings
 * @param[out] value The current system settings value of the given key
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SYSTEM_SETTINGS_ERROR_NONE Successful
 * @retval #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED The related feature is not supported on the device
 * @retval #SYSTEM_SETTINGS_ERROR_PERMISSION_DENIED Permission violation error
 * @retval #SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SYSTEM_SETTINGS_ERROR_IO_ERROR Internal I/O error
 * @warning %http://tizen.org/privilege/systemsettings (public level privilege) <b>MUST NOT</b> be declared to use this function since 2.3.1.
 */
int system_settings_get_value_bool(system_settings_key_e key, bool *value);

/**
 * @platform
 * @brief Sets the system settings value associated with the given key as a string.
 * @details This API is used to set a string value for a key that can be set string in System Settings APIs. When using this function, be careful not to include keys of other types (such as int or bool) as parameters.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/systemsettings.admin
 * @remarks #SYSTEM_SETTINGS_KEY_DEFAULT_FONT_TYPE is not available for setting. When the feature related to the @a key is not supported on the device, #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED will be returned.
 * @param[in] key The key name of the system settings
 * @param[out] value The new system settings value of the given key
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SYSTEM_SETTINGS_ERROR_NONE Successful
 * @retval #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED The related feature is not supported on the device
 * @retval #SYSTEM_SETTINGS_ERROR_PERMISSION_DENIED Permission violation error
 * @retval #SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SYSTEM_SETTINGS_ERROR_IO_ERROR Internal I/O error
 */
int system_settings_set_value_string(system_settings_key_e key, const char *value);

/**
 * @brief Gets the system settings value associated with the given key as a string.
 * @details This API is used to get the string value set for a string key in System Settings APIs. When using this function, be careful not to include keys of other types (such as int or bool) as parameters
 * @since_tizen 2.3
 * @remarks You must release @a value using free(). When the feature related to the @a key is not supported on the device, #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED will be returned.
 * @param[in] key The key name of the system settings
 * @param[out] value The current system settings value of the given key
 * @return 0 on success, otherwise a negative error value
 * @retval #SYSTEM_SETTINGS_ERROR_NONE Successful
 * @retval #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED The related feature is not supported on the device
 * @retval #SYSTEM_SETTINGS_ERROR_PERMISSION_DENIED Permission violation error
 * @retval #SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SYSTEM_SETTINGS_ERROR_IO_ERROR Internal I/O error
 * @code
 *  char *value = NULL;
 *  int retcode = system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_COUNTRY, &value);
 *  if (retcode == SYSTEM_SETTINGS_ERROR_NONE) {
 *    printf("%s", value);
 *    free(value);
 *  }
 * @endcode
 * @warning %http://tizen.org/privilege/systemsettings (public level privilege) <b>MUST NOT</b> be declared to use this function since 2.3.1.
 */
int system_settings_get_value_string(system_settings_key_e key, char **value);

/**
 * @brief Sets a change event callback for the given system settings key.
 * @details This function is used to specify a callback function that is called when a key value changes. Each key can have only one callback function specified. To release this callback function, you can use the system_settings_unset_changed_cb() function
 * @since_tizen 2.3
 * @remarks #SYSTEM_SETTINGS_KEY_DEFAULT_FONT_TYPE is not available for set_changed_cb. When the feature related to the @a key is not supported on the device, #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED will be returned.
 * @param[in] key The key name of the system settings
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #SYSTEM_SETTINGS_ERROR_NONE Successful
 * @retval #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED The related feature is not supported on the device
 * @retval #SYSTEM_SETTINGS_ERROR_PERMISSION_DENIED Permission violation error
 * @retval #SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @post system_settings_changed_cb() will be invoked.
 *
 * @see system_settings_unset_changed_cb()
 * @see system_settings_changed_cb()
 * @warning %http://tizen.org/privilege/systemsettings (public level privilege) <b>MUST NOT</b> be declared to use this function since 2.3.1.
 *
*/
int system_settings_set_changed_cb(system_settings_key_e key, system_settings_changed_cb callback, void *user_data);

/**
 * @brief Unsets the callback function.
 * @details This function is used to release the callback function specified by system_settings_set_changed_cb().
 * @since_tizen 2.3
 * @remarks #SYSTEM_SETTINGS_KEY_DEFAULT_FONT_TYPE is not available for set_changed_cb. When the feature related to the @a key is not supported on the device, #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED will be returned.
 * @param[in] key The key name of the system settings
 * @return 0 on success, otherwise a negative error value
 * @retval #SYSTEM_SETTINGS_ERROR_NONE Successful
 * @retval #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED The related feature is not supported on the device
 * @retval #SYSTEM_SETTINGS_ERROR_PERMISSION_DENIED Permission violation error
 * @retval #SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see system_settings_set_changed_cb()
 * @warning %http://tizen.org/privilege/systemsettings (public level privilege) <b>MUST NOT</b> be declared to use this function since 2.3.1.
 */
int system_settings_unset_changed_cb(system_settings_key_e key);

/**
 * @platform
 * @brief Called to get each string value from string typed list.
 * @details This callback is used in system_settings_foreach_value_string(). Through this callback, the stored string list is passed one by one.
 * @since_tizen 3.0
 * @param[in] index zero based number indicating index of node in a list
 * @param[in] value value returned from the list
 * @param[in] cb_data  The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop, otherwise false to break out of the loop
 */
typedef bool (*system_settings_iter_cb)(int index, const char *value, void *cb_data);

/**
 * @platform
 * @brief Iterate the system settings value associated with the given key as a string type.
 * @details This function is used to pass the path list of ringtones one by one through a callback when using #SYSTEM_SETTINGS_KEY_INCOMING_CALL_RINGTONE.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/systemsettings.admin
 * @remarks When the feature related to the @a key is not supported on the device, #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED will be returned.
 * @param[in] key The key name of the system settings
 * @param[in] callback The callback-function name for iteration
 * @param[in] user_data  The user data passed from caller-side
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SYSTEM_SETTINGS_ERROR_NONE Successful
 * @retval #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED The related feature is not supported on the device
 * @retval #SYSTEM_SETTINGS_ERROR_PERMISSION_DENIED Permission violation error
 * @retval #SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SYSTEM_SETTINGS_ERROR_IO_ERROR Internal I/O error
 */
int system_settings_foreach_value_string(system_settings_key_e key, system_settings_iter_cb callback, void *user_data);

/**
 * @platform
 * @brief Adds the system settings value associated with the given key as a string type if it supports List iteration.
 * @details This function is used to add the path of the ringtone when using a KEY such as #SYSTEM_SETTINGS_KEY_INCOMING_CALL_RINGTONE.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/systemsettings.admin
 * @remarks When the feature related to the @a key is not supported on the device, #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED will be returned.
 * @param[in] key The key name of the system settings
 * @param[in] value string typed value to be appended
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SYSTEM_SETTINGS_ERROR_NONE Successful
 * @retval #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED The related feature is not supported on the device
 * @retval #SYSTEM_SETTINGS_ERROR_PERMISSION_DENIED Permission violation error
 * @retval #SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SYSTEM_SETTINGS_ERROR_IO_ERROR Internal I/O error
 */
int system_settings_add_value_string(system_settings_key_e key, const char *value);

/**
 * @platform
 * @brief Delete the system settings value associated with the given key as an string type if it supports List iteration.
 * @details This function is used to delete the path of the ringtone when using a KEY such as #SYSTEM_SETTINGS_KEY_INCOMING_CALL_RINGTONE.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/systemsettings.admin
 * @remarks When the feature related to the @a key is not supported on the device, #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED will be returned.
 * @param[in] key The key name of the system settings
 * @param[in] value string typed value to be removed
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SYSTEM_SETTINGS_ERROR_NONE Successful
 * @retval #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED The related feature is not supported on the device
 * @retval #SYSTEM_SETTINGS_ERROR_PERMISSION_DENIED Permission violation error
 * @retval #SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SYSTEM_SETTINGS_ERROR_IO_ERROR Internal I/O error
 */
int system_settings_delete_value_string(system_settings_key_e key, const char *value);

/**
 * @brief Adds a change event callback for the given system settings key.
 * @details The difference between this function and system_settings_set_changed_cb() is that system_settings_set_changed_cb() can set only one callback for a given key, while system_settings_add_changed_cb() can set multiple callbacks for a given key.
 * @since_tizen 5.0
 * @remarks The @a key cannot be #SYSTEM_SETTINGS_KEY_DEFAULT_FONT_TYPE. When the feature related to the @a key is not supported on the device, #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED will be returned.

 * @param[in] key The key name of the system settings
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #SYSTEM_SETTINGS_ERROR_NONE Successful
 * @retval #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED The related feature is not supported on the device
 * @retval #SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @post system_settings_changed_cb() will be invoked.
 *
 * @see system_settings_remove_changed_cb()
 * @see system_settings_changed_cb()
 *
*/
int system_settings_add_changed_cb(system_settings_key_e key, system_settings_changed_cb callback, void *user_data);

/**
 * @brief Removes a change event callback function.
 * @details The difference between this function and system_settings_unset_changed_cb() is that system_settings_unset_changed_cb() unsets the callback set with system_settings_set_changed_cb(), while system_settings_remove_changed_cb() removes callbacks added with system_settings_add_changed_cb()
 * @since_tizen 5.0
 * @remarks The @a key cannot be #SYSTEM_SETTINGS_KEY_DEFAULT_FONT_TYPE. When the feature related to the @a key is not supported on the device, #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED will be returned.
 * @param[in] key The key name of the system settings
 * @param[in] callback The callback function to be removed
 * @return 0 on success, otherwise a negative error value
 * @retval #SYSTEM_SETTINGS_ERROR_NONE Successful
 * @retval #SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED The related feature is not supported on the device
 * @retval #SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see system_settings_add_changed_cb()
 */
int system_settings_remove_changed_cb(system_settings_key_e key, system_settings_changed_cb callback);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_SYSTEM_SETTINGS_H__ */
