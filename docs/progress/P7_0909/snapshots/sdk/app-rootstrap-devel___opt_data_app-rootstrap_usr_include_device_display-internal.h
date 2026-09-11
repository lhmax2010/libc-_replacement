/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_SYSTEM_DISPLAY_INTERNAL_H__
#define __TIZEN_SYSTEM_DISPLAY_INTERNAL_H__


#include <gio/gio.h>
#include <libsyscommon/libgdbus.h>

#include "device-error.h"
#include "display-enum.h"

/**
 * @addtogroup CAPI_SYSTEM_DEVICE_DISPLAY_INTERNAL_MODULE
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration for the adjustment display white balance.
 * @since_tizen 7.0
 */

typedef enum
{
	DISPLAY_WHITE_BALANCE_R_GAIN = 0, /**< White balance R Gain */
	DISPLAY_WHITE_BALANCE_G_GAIN, /**< White balance G Gain */
	DISPLAY_WHITE_BALANCE_B_GAIN, /**< White balance B Gain */
	DISPLAY_WHITE_BALANCE_R_OFFSET, /**< White balance R Offset */
	DISPLAY_WHITE_BALANCE_G_OFFSET, /**< White balance G Offset */
	DISPLAY_WHITE_BALANCE_B_OFFSET, /**< White balance B Offset */
} display_white_balance_e;


/**
 * @brief Enumeration for the available display rotation states.
 * @since_tizen 7.0
 */

typedef enum
{
	DEVICE_DISPLAY_ROTATION_ANGLE_UNKNOWN = -1,	/**< Unknown state. It can be seen in laid device */
	DEVICE_DISPLAY_ROTATION_ANGLE_DEGREE_0 = 0,	/**< Display rotation state is 0 degree */
	DEVICE_DISPLAY_ROTATION_ANGLE_DEGREE_90 = 90,	/**< Display rotation state is 90 degree */
	DEVICE_DISPLAY_ROTATION_ANGLE_DEGREE_180 = 180,	/**< Display rotation state is 180 degree */
	DEVICE_DISPLAY_ROTATION_ANGLE_DEGREE_270 = 270,	/**< Display rotation state is 270 degree */
} device_display_rotation_angle_e;


/**
 * @brief Enumeration for the display rotation directions.
 * @since_tizen 7.0
 */

typedef enum
{
	DEVICE_DISPLAY_ROTATION_DIRECTION_CLOCKWISE,			/**< Rotate with clockwise direction */
	DEVICE_DISPLAY_ROTATION_DIRECTION_COUNTER_CLOCKWISE,	/**< Rotate with counter clockwise direction */
} device_display_rotation_direction_e;


/**
 * @brief Enumeration for the display initial direction.
 * Initial direction means the display direction of degree @c 0.
 * @since_tizen 7.0
 */
typedef enum
{
	DEVICE_DISPLAY_INIT_DIRECTION_HORIZONTAL,	/**< Display initial direction is horizontal */
	DEVICE_DISPLAY_INIT_DIRECTION_VERTICAL,		/**< Display initial direction is vertical */
} device_display_init_direction_e;


/**
 * @brief Gets the brightness value of a specific display device based on its state.
 * @details The display device is identified by its index, which can be obtained using the device_get_display_numbers() function.\n
 *          The desired display state is specified in the @a state parameter, and the brightness level is returned in the @a brightness parameter.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks Ensure that the provided @a brightness pointer is valid and has enough memory allocated.
 * @param[in] display_index The index of the display \n
 *                          It can be greater than or equal to @c 0 and less than the number of displays returned by device_display_get_numbers(). \n
 *                          The index zero is always assigned to the main display
 * @param[out] brightness The current brightness value of the display
 * @param[in] state The enum value of the display state \n
 *                   - @c DISPLAY_STATE_NORMAL: Normal state \n
 *                   - @c DISPLAY_STATE_DIM: Screen Dim state \n
 *                   - @c DISPLAY_STATE_OFF: Screen Off state
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported in this device
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation not permitted
 *
 * @code
 * #include <device/display-internal.h>
 * ...
 * int number_of_display = 0, brightness = 0, display_index = 0;
 * display_state_e display_state = DISPLAY_STATE_NORMAL;
 * int ret = 0;
 * ...
 * ret = device_get_display_numbers(&number_of_display);
 * if (ret == DEVICE_ERROR_NONE) {
 *     display_index = number_of_display - 1;
 *     device_display_get_brightness_state(display_index, display_state, &brightness);
 *     ...
 * }
 * ...
 * @endcode
 * @see device_display_get_numbers()
 * @see device_display_set_brightness_state()
 * @see device_display_get_max_brightness_state()
 */
int device_display_get_brightness_state(int display_index, display_state_e state, int *brightness);


/**
 * @brief Gets the maximum brightness value of a specific display device based on its state that can be set.
 * @details The display device is identified by its index, which can be obtained using the device_get_display_numbers() function.\n
 *          The desired display state is specified in the @a state parameter, and the max_brightness level is returned in the @a brightness parameter.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks Ensure that the provided @a brightness pointer is valid and has enough memory allocated.
 * @param[in] display_index The index of the display \n
 *                          It can be greater than or equal to @c 0 and less than the number of displays returned by device_display_get_numbers(). \n
 *                          The index zero is always assigned to the main display
 * @param[out] max_brightness The maximum brightness value of the display
 * @param[in] state The enum value of the display state \n
 *                   - @c DISPLAY_STATE_NORMAL: Normal state \n
 *                   - @c DISPLAY_STATE_DIM: Screen Dim state \n
 *                   - @c DISPLAY_STATE_OFF: Screen Off state
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported in this device
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation not permitted
 *
 * @code
 * #include <device/display-internal.h>
 * ...
 * int number_of_display = 0, max_brightness = 0, display_index = 0;
 * display_state_e display_state = DISPLAY_STATE_NORMAL;
 * int ret = 0;
 * ...
 * ret = device_get_display_numbers(&number_of_display);
 * if (ret == DEVICE_ERROR_NONE) {
 *     display_index = number_of_display - 1;
 *     device_display_get_max_brightness_state(display_index, display_state, &max_brightness);
 *     ...
 * }
 * ...
 * @endcode
 * @see device_display_get_numbers()
 * @see device_display_get_brightness_state()
 * @see device_display_set_brightness_state()
 */
int device_display_get_max_brightness_state(int display_index, display_state_e state, int *brightness);


/**
 * @brief Sets the brightness value to a specific display device based on its state.
 * @details The display device is identified by its index, which can be obtained using the device_get_display_numbers() function.
 *          The desired display state is specified in the @a state parameter, and the desired brightness level is specified in the @a brightness parameter.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks Ensure that the provided @a display_index, @a state, and @a brightness are valid values.
 * @param[in] display_index The index of the display \n
 *                          It can be greater than or equal to @c 0 and less than the number of displays returned by device_display_get_numbers(). \n
 *                          The index zero is always assigned to the main display
 * @param[in] brightness The new brightness value to set \n
 *                       The maximum value can be represented by device_display_get_max_brightness_state()
 * @param[in] state The enum value of the display state \n
 *                   - @c DISPLAY_STATE_NORMAL: Normal state \n
 *                   - @c DISPLAY_STATE_DIM: Screen Dim state \n
 *                   - @c DISPLAY_STATE_OFF: Screen Off state
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported in this device
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation not permitted
 *
 * @code
 * #include <device/display-internal.h>
 * ...
 * int number_of_display = 0, brightness = 100, display_index = 0;
 * display_state_e display_state = DISPLAY_STATE_NORMAL;
 * int ret = 0;
 * ...
 * ret = device_get_display_numbers(&number_of_display);
 * if (ret == DEVICE_ERROR_NONE) {
 *     display_index = number_of_display - 1;
 *     device_display_set_brightness_state(display_index, display_state, brightness);
 *     ...
 * }
 * ...
 * @endcode
 * @see device_display_get_numbers()
 * @see device_display_get_max_brightness()
 * @see device_display_set_brightness_state()
 */
int device_display_set_brightness_state(int display_index, display_state_e state, int brightness);

/**
 * @brief Changes the display state based on a specific reason.
 * @details The desired display state is specified in the type parameter, and the reason for changing the display state is specified in the reason parameter.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks @a type parameter value cannot be DISPLAY_STATE_SCREEN_DIM, it is invalid.
 * @param[in] type The type is display state to change\n
 *                 DISPLAY_STATE_NORMAL    : change to normal\n
 *                 DISPLAY_STATE_SCREEN_OFF: change to off\n
 * @param[in] reason Reason that causes display chage state\n
 *                   It should be one of @c "palm", @c "gesture" or @c "event"
 * @param[in] timeout Timeout to change state\n
 * @param[in] cb Callback function for handling result of dbus method call\n
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <device/display-internal.h>
 * ...
 * void dbus_cb(GVariant *result, void *data, GError *err)
 * {
 *     int temp = 0;
 *
 *     if (!result) {
 *         _E("Can't get result of display state change request.:%s", err->message);
 *         return;
 *     }
 *     if (!g_variant_get_safe(result, "(i)", &temp)) {
 *         _E("Failed to get variant(%s): no call back message", g_variant_get_type_string(result));
 *         goto out;
 *     }
 *     _I("replay message(%d)", temp);
 * out:
 *     g_variant_unref(result);
 * }
 * ...
 * int main(void)
 * {
 *     int ret = 0;
 *
 *     ret = device_display_change_state_by_reason(DISPLAY_STATE_SCREEN_OFF, "palm", 0, dbus_cb);
 *     ...
 *     return 0;
 * }
 * ...
 * @endcode
 * @see device_display_change_state_by_reason()
 */
int device_display_change_state_by_reason(display_state_e type, const char *reason, int timeout, dbus_pending_cb cb);

/**
 * @platform
 * @brief Checks whether the current device's display feature is supported.
 * @details Checks whether the display feature is supported on the device.\n
 *          The display feature refers to the ability of the device to control the display settings and related properties.
 * @since_tizen 6.5
 * @remarks Checks the display feature(http://tizen.org/feature/display) value.
 * @return @c 1 if the display feature is supported, otherwise @c 0
 *
 * @code
 * #include <device/display-internal.h>
 * ...
 * int ret = 0;
 * ret = is_feature_display_supported();
 * if (!ret) {
 *     _E("display feature is not supported");
 *     ...
 * }
 * ...
 * @endcode
 */
int is_feature_display_supported(void);


/**
 * @brief Gets the rotation angle and initial direction of a specific display device.
 * @details The display device is identified by its index, which can be obtained using the device_get_display_numbers().
 *          The rotation angle is returned in the @a angle parameter,
 *          and the initial direction is returned in the @a init_direction parameter.
 * @since_tizen 7.0
 * @privilege %http://tizen.org/privilege/display
 * @remarks It shows the physical display angle, not the software screen angle.
 * @param[in] display_index The index of the display \n
 *                          It can be greater than or equal to @c 0 and less than the number of displays returned by device_display_get_numbers(). \n
 *                          The index zero is always assigned to the main display
 * @param[out] angle The type is display rotation angle\n
 *                 DEVICE_DISPLAY_ROTATION_ANGLE_UNKNOWN\n
 *                 DEVICE_DISPLAY_ROTATION_ANGLE_DEGREE_0\n
 *                 DEVICE_DISPLAY_ROTATION_ANGLE_DEGREE_90\n
 *                 DEVICE_DISPLAY_ROTATION_ANGLE_DEGREE_180\n
 *                 DEVICE_DISPLAY_ROTATION_ANGLE_DEGREE_270\n
 * @param[out] init_direction The type is display initial direction
 *                 When the rotation angle is @c 0 degree, initial direction means the display is a horizontal or vertical.\n
 *                 DEVICE_DISPLAY_INIT_DIRECTION_HORIZONTAL\n
 *                 DEVICE_DISPLAY_INIT_DIRECTION_VERTICAL\n
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported in this device
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation not permitted
 *
 * @code
 * #include <stdio.h>
 * #include <device/display.h>
 * ...
 * int main(void)
 * {
 *     int display_index = 0;
 *     device_display_rotation_angle_e angle;
 *     device_display_init_direction_e init_direction;
 *     int ret = 0;
 *     ...
 *     ret = device_display_get_rotation_angle(display_index, &angle, &init_direction);
 *     if (ret == DEVICE_ERROR_NONE) {
 *         ...
 *     }
 *     return 0;
 * }
 * ...
 * @endcode
 * @see device_display_set_rotation_angle()
 * @see device_display_rotation_angle_e
 * @see device_display_init_direction_e
 */
int device_display_get_rotation_angle(int display_index,
				device_display_rotation_angle_e *angle,
				device_display_init_direction_e *init_direction);

/**
 * @brief Sets the rotation angle and initial direction of a specific display device.
 * @details The display device is identified by its index, which can be obtained using the device_get_display_numbers() function.
 *          The desired rotation angle is specified in the @a angle parameter, and the desired rotation direction is specified in the @a rotation_direction parameter.
 * @since_tizen 7.0
 * @privilege %http://tizen.org/privilege/display
 * @remarks It shows the physical display angle, not the software screen angle.
 *          It is function to set specific fixed angle, not for additional turning.
 *          For example, when this function called with same angle twice, there will be no actual action on the second call.
 * @param[in] display_index The index of the display \n
 *                          It can be greater than or equal to @c 0 and less than the number of displays returned by device_display_get_numbers(). \n
 *                          The index zero is always assigned to the main display
 * @param[in] angle The type is display rotation angle\n
 *                 DEVICE_DISPLAY_ROTATION_ANGLE_DEGREE_0\n
 *                 DEVICE_DISPLAY_ROTATION_ANGLE_DEGREE_90\n
 *                 DEVICE_DISPLAY_ROTATION_ANGLE_DEGREE_180\n
 *                 DEVICE_DISPLAY_ROTATION_ANGLE_DEGREE_270\n
 * @param[in] rotation_direction The type is display rotation direction\n
 *	               DEVICE_DISPLAY_ROTATION_DIRECTION_CLOCKWISE\n
 *	               DEVICE_DISPLAY_ROTATION_DIRECTION_COUNTER_CLOCKWISE\n
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported in this device
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation not permitted
 *
 * @code
 * #include <stdio.h>
 * #include <device/display.h>
 * ...
 * int main(void)
 * {
 *     int display_index = 0;
 *     device_display_rotation_angle_e angle = DEVICE_DISPLAY_ROTATION_ANGLE_DEGREE_90;
 *     device_display_rotation_direction_e rotation_direction = DEVICE_DISPLAY_ROTATION_DIRECTION_CLOCKWISE;
 *     int ret = 0;
 *     ...
 *     ret = device_display_set_rotation_angle(display_index, angle, rotation_direction);
 *     if (ret == DEVICE_ERROR_NONE) {
 *         ...
 *     }
 *     return 0;
 * }
 * ...
 * @endcode
 * @see device_display_get_rotation_angle()
 * @see device_display_rotation_angle_e
 * @see device_display_rotation_direction_e
 */
int device_display_set_rotation_angle(int display_index,
				device_display_rotation_angle_e angle,
				device_display_rotation_direction_e rotation_direction);

/**
 * @platform
 * @brief Checks whether the current device's display state feature is supported.
 * @details Checks whether the display state feature is supported on the device.\n
 *          The display state feature refers to the ability of the device to control the display state related properties.
 * @since_tizen 6.5
 * @remarks Checks the display state feature(http://tizen.org/feature/display.state) value.
 * @return @c 1 if the display state feature is supported, otherwise @c 0
 *
 * @code
 * #include <device/display-internal.h>
 * ...
 * int ret = 0;
 * ret = is_feature_display_state_supported();
 * if (!ret) {
 *     _E("display state feature is not supported");
 *     ...
 * }
 * ...
 * @endcode
 */
int is_feature_display_state_supported(void);

/**
 * @brief Change display white balance value from the display HAL module
 * @details The @a white_balance_type parameter and @a value is used to calucate the reference value for white.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks #DEVICE_ERROR_NOT_SUPPORTED is returned, when the following feature is not supported: %http://tizen.org/feature/display \n
 * @param[in] display_index The index of the display \n
 *                          It can be greater than or equal to @c 0 and less than the number of displays returned by device_display_get_numbers(). \n
 *                          The index zero is always assigned to the main display
 * @param[in] white_balance_type The type to adjust white balance value
 *                               DISPLAY_WHITE_BALANCE_R_GAIN \n
 *                               DISPLAY_WHITE_BALANCE_G_GAIN \n
 *                               DISPLAY_WHITE_BALANCE_B_GAIN \n
 *                               DISPLAY_WHITE_BALANCE_R_OFFSET \n
 *                               DISPLAY_WHITE_BALANCE_G_OFFSET \n
 *                               DISPLAY_WHITE_BALANCE_B_OFFSET
 * @param[in] value The value to be set for @a white_balance_type \n
 *                  It's range is (@c 0 ~ @c 2047)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/display-internal.h>
 * ...
 * int ret = 0;
 * ret = device_display_set_white_balance(DEVICE_WHITE_BALANCE_R_GAIN, 1024);
 * if (ret != DEVICE_ERROR_NONE) {
 *     ...
 * }
 * ...
 * ret = device_display_set_white_balance(DEVICE_WHITE_BALANCE_R_OFFSET, 1024);
 * if (ret != DEVICE_ERROR_NONE) {
 *     ...
 * }
 * ...
 * @endcode
 * @see device_display_get_numbers()
 * @see device_display_get_white_balance()
 * @see display_white_balance_e
 */
int device_display_set_white_balance(int display_index, display_white_balance_e white_balance_type, int value);

/**
 * @brief Gets display white balance value based on @a white_balance_type from the display HAL module.
 * @details The @a white_balance_type parameter and @a value is used to calucate the reference value for white.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks #DEVICE_ERROR_NOT_SUPPORTED is returned, when the following feature is not supported: %http://tizen.org/feature/display \n
 * @param[in] display_index The index of the display \n
*                          It can be greater than or equal to @c 0 and less than the number of displays returned by device_display_get_numbers(). \n
*                          The index zero is always assigned to the main display
 * @param[in] white_balance_type The type to adjust white balance value
 *                               DISPLAY_WHITE_BALANCE_R_GAIN \n
 *                               DISPLAY_WHITE_BALANCE_G_GAIN \n
 *                               DISPLAY_WHITE_BALANCE_B_GAIN \n
 *                               DISPLAY_WHITE_BALANCE_R_OFFSET \n
 *                               DISPLAY_WHITE_BALANCE_G_OFFSET \n
 *                               DISPLAY_WHITE_BALANCE_B_OFFSET
 * @param[out] value The value to be set for @a white_balance_type \n
 *                   It's range is (@c 0 ~ @c 2047)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported in this device
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation not permitted
 *
 * @code
 * #include <device/display-internal.h>
 * ...
 * int ret = 0, r_gain_value = 0, r_offset_value = 0;
 * ret = device_display_get_white_balance(DEVICE_WHITE_BALANCE_R_GAIN, &r_gain_value);
 * if (ret != DEVICE_ERROR_NONE) {
 *     ...
 * }
 * ...
 * ret = device_display_get_white_balance(DEVICE_WHITE_BALANCE_R_OFFSET, &r_offset_value);
 * if (ret != DEVICE_ERROR_NONE) {
 *     ...
 * }
 * ...
 * @endcode
 * @see device_display_get_numbers()
 * @see device_display_set_white_balance()
 * @see display_white_balance_e
 */
int device_display_get_white_balance(int display_index, display_white_balance_e white_balance_type, int *value);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif  // __TIZEN_SYSTEM_DISPLAY_INTERNAL_H__
