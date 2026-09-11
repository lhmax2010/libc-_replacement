/*
 * Copyright (c) 2011-2017 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_UI_EFL_UTIL_H__
#define __TIZEN_UI_EFL_UTIL_H__

#include <tizen.h>
#include <Evas.h>
#include <tbm_surface.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
# if __GNUC__ >= 4
#  ifndef API
#   define API __attribute__ ((visibility("default")))
#  endif
# endif
#endif

/**
 * @file efl_util.h
 */

/**
 * @addtogroup CAPI_EFL_UTIL_WIN_PROPERTY_MODULE
 * @{
 */

/**
 * @brief Enumeration for EFL UTIL ERROR.
 * @since_tizen 2.3
 */
typedef enum
{
   EFL_UTIL_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
   EFL_UTIL_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
   EFL_UTIL_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
   EFL_UTIL_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
   EFL_UTIL_ERROR_NO_SUCH_DEVICE = TIZEN_ERROR_NO_SUCH_DEVICE, /**< @platform No such device or address (@b Since: 2.4) */
   EFL_UTIL_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION, /**< @platform Function not implemented (@b Since: 2.4) */
   EFL_UTIL_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< @platform Not supported (@b Since: 2.4) */
   EFL_UTIL_ERROR_NOT_SUPPORTED_WINDOW_TYPE = TIZEN_ERROR_EFL_UTIL | 0x01, /**< Window type not supported */
   EFL_UTIL_ERROR_SCREENSHOT_INIT_FAIL = TIZEN_ERROR_EFL_UTIL | 0x02,  /**< @platform Screenshot initialization fail (@b Since: 2.4) */
   EFL_UTIL_ERROR_SCREENSHOT_EXECUTION_FAIL = TIZEN_ERROR_EFL_UTIL | 0x03,  /**< @platform Screenshot execution fail (@b Since: 2.4) */
   EFL_UTIL_ERROR_NO_RESOURCE_AVAILABLE = TIZEN_ERROR_EFL_UTIL | 0x04 /**< Resource is not available (@b Since: 4.0) */
} efl_util_error_e;

/**
 * @brief Enumeration of notification window's priority level.
 * @since_tizen 2.3
 */
typedef enum
{
   EFL_UTIL_NOTIFICATION_LEVEL_NONE    = -1, /**< No (reset) notification level. This value makes the window place in normal layer. (@b Since: 2.4) */
   EFL_UTIL_NOTIFICATION_LEVEL_DEFAULT = 10, /**< Default notification level. (@b Since: 2.4) */
   EFL_UTIL_NOTIFICATION_LEVEL_MEDIUM  = 20, /**< Higher notification level than default. (@b Since: 2.4) */
   EFL_UTIL_NOTIFICATION_LEVEL_HIGH    = 30, /**< Higher notification level than medium. (@b Since: 2.4) */
   EFL_UTIL_NOTIFICATION_LEVEL_TOP     = 40  /**< The highest notification level. (@b Since: 2.4) */
} efl_util_notification_level_e;

/**
 * @brief Enumeration of screen mode.
 * @since_tizen 2.4
 */
typedef enum
{
   EFL_UTIL_SCREEN_MODE_DEFAULT, /**< The mode which turns the screen off after a timeout. */
   EFL_UTIL_SCREEN_MODE_ALWAYS_ON, /**< The mode which keeps the screen turned on. */
} efl_util_screen_mode_e;

/**
 * @brief Sets the priority level for the specified notification window.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/window.priority.set
 * @remarks This API can be used for a notification type window only.
 *          Up to the version 2.4, it supports as async APIs.
 *          But it is synchronous call since Tizen 3.0
 * @param[in] window The EFL window
 * @param[in] level The notification window level
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_NOT_SUPPORTED_WINDOW_TYPE Window type not supported
 */
API int efl_util_set_notification_window_level(Evas_Object *window, efl_util_notification_level_e level);

/**
 * @brief Gets the priority level for the specified notification window.
 * @since_tizen 2.3
 *
 * @remarks This API can be used for a notification type window only.
 *          Up to the version 2.4, it supports as async APIs.
 *          But it is synchronous call since Tizen 3.0
 * @param[in] window The EFL window
 * @param[out] level The notification window level
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_NOT_SUPPORTED_WINDOW_TYPE Window type not supported
 */
API int efl_util_get_notification_window_level(Evas_Object *window, efl_util_notification_level_e *level);

/**
 * @brief Sets the alpha window's visual state to opaque state.
 * @details This API sets the alpha window's visual state to opaque state.
 *          If the alpha window sets the visual state to the opaque,
 *          then the window manager could handle it as the opaque window while calculating visibility.
 *          This API will have no effect when used by a non-alpha window.
 * @since_tizen 2.4
 * @param[in] window The EFL window
 * @param[in] opaque The value that indicates whether the window has set a visual state to opaque (0: unset, 1: set)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 */
API int efl_util_set_window_opaque_state(Evas_Object *window, int opaque);

/**
 * @brief Sets the window's screen mode.
 * @details This API is useful when the application need to keep the display turned on.
 *          If the application set the mode to #EFL_UTIL_SCREEN_MODE_ALWAYS_ON to its window and the window is shown wholly or partially,
 *          the window manager requests the display system to keep the display on as long as the window is shown.
 *          If the window is no longer shown, then the window manger request the display system to go back to normal operation.
 *          Default screen mode of window is #EFL_UTIL_SCREEN_MODE_DEFAULT.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks This API needs the privilege.
 *          If the application which is not get the privilege use this API, the window manager generates the permission deny error.
 * @param[in] window The EFL window
 * @param[in] mode The screen mode
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 */
API int efl_util_set_window_screen_mode(Evas_Object *window, efl_util_screen_mode_e mode);

/**
 * @brief Gets the screen mode of the specified window.
 * @since_tizen 2.4
 * @param[in] window The EFL window
 * @param[out] mode The screen mode
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 */
API int efl_util_get_window_screen_mode(Evas_Object *window, efl_util_screen_mode_e *mode);

/**
 * @brief Sets the user's preferred brightness of the specified window.
 * @details This API is useful when the application need to change the brightness of the screen when it is appeared on the screen.
 *          If the application sets the brightness 0 to 100 to its window and the application window is shown wholly or partially,
 *          the window manager requests the display system to change the brightness of the screen using user's preferred brightness.
 *          If the window is no longer shown, then the window manger request the display system to go back to default brightness.
 *          If the brightness is less than 0, this means to use the default screen brightness.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks This API needs the privilege.
 *          If the application which is not get the privilege use this API, the window manager generates the permission deny error.
 * @param[in] window The EFL window
 * @param[in] brightness The preferred brightness
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_OUT_OF_MEMORY Out of memory
 * @see efl_util_get_window_brightness()
 */
API int efl_util_set_window_brightness(Evas_Object *window, int brightness);

/**
 * @brief Gets the user's preferred brightness of the specified window.
 * @since_tizen 3.0
 * @param[in] window The EFL window
 * @param[out] brightness The preferred brightness
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see efl_util_set_window_brightness()
 */
API int efl_util_get_window_brightness(Evas_Object *window, int *brightness);


/**
 * @}
 */

/**
 * @addtogroup CAPI_EFL_UTIL_INPUT_MODULE
 * @{
 */

 /**
  * @platform
  * @brief Definition for the input generator handle.
  * @since_tizen 2.4
  */

 typedef struct _efl_util_inputgen_h * efl_util_inputgen_h;

/**
 * @platform
 * @brief Enumeration of device type generated events.
 * @since_tizen 2.4
 */
typedef enum
{
   EFL_UTIL_INPUT_DEVTYPE_NONE = 0x0, /**< (Deprecated since 3.0.) */
   EFL_UTIL_INPUT_DEVTYPE_TOUCHSCREEN = (1 << 0), /**< Touch Screen device */
   EFL_UTIL_INPUT_DEVTYPE_KEYBOARD = (1 << 1), /**< Keyboard device */
   EFL_UTIL_INPUT_DEVTYPE_POINTER = (1 << 2), /**< Mouse device (Since 3.0) */
   EFL_UTIL_INPUT_DEVTYPE_ALL = EFL_UTIL_INPUT_DEVTYPE_TOUCHSCREEN |
                                EFL_UTIL_INPUT_DEVTYPE_KEYBOARD, /**< Both of touch screen and keyboard device (Deprecated since 3.0. Check all enumerations using OR operand instead) */
   EFL_UTIL_INPUT_DEVTYPE_MAX = (1 << 10) /**< (Deprecated since 3.0.) */
} efl_util_input_device_type_e;

/**
 * @platform
 * @brief Enumeration of touch event types.
 * @since_tizen 2.4
 */
typedef enum
{
   EFL_UTIL_INPUT_TOUCH_NONE, /**< (Deprecated since 3.0.) */
   EFL_UTIL_INPUT_TOUCH_BEGIN, /**< Finger press. It is same a behavior put your finger on touch screen */
   EFL_UTIL_INPUT_TOUCH_UPDATE, /**< Finger move. It is same a behavior move your finger on touch screen */
   EFL_UTIL_INPUT_TOUCH_END, /**< Finger release. It is same a behavior release your finger on touch screen */
   EFL_UTIL_INPUT_TOUCH_MAX = 10 /**< (Deprecated since 3.0.) */
} efl_util_input_touch_type_e;

/**
 * @platform
 * @brief Enumeration of pointer event types.
 * @since_tizen 3.0
 */
typedef enum
{
   EFL_UTIL_INPUT_POINTER_BUTTON_DOWN, /**< Mouse button press. */
   EFL_UTIL_INPUT_POINTER_BUTTON_UP, /**< Mouse move. */
   EFL_UTIL_INPUT_POINTER_MOVE, /**< Mouse button release. */
} efl_util_input_pointer_type_e;

/**
 * @platform
 * @brief Enumeration of pointer wheel event types.
 * @since_tizen 5.5
 */
typedef enum
{
   EFL_UTIL_INPUT_POINTER_WHEEL_VERT, /**< Vertical wheel. */
   EFL_UTIL_INPUT_POINTER_WHEEL_HORZ, /**< Horizontal wheel. */
} efl_util_input_pointer_wheel_type_e;

/**
   * @platform
   * @brief Initializes system and check input generate functions are supported, open devices generated events.
   * @since_tizen 2.4
   * @privlevel platform
   * @privilege %http://tizen.org/privilege/inputgenerator
   * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
   * @remarks The dev_type is changed into the unsigned int to perform bitwise operations. (Since 3.0)
   * @remarks The returned object should be released with efl_util_input_deinitialize_generator().
   * @param[in] dev_type The device type want to generate events (ex> #EFL_UTIL_INPUT_DEVTYPE_TOUCHSCREEN | #EFL_UTIL_INPUT_DEVTYPE_KEYBOARD)
   * @return #efl_util_inputgen_h on success, otherwise @c NULL
   * @exception #EFL_UTIL_ERROR_NONE Successful
   * @exception #EFL_UTIL_ERROR_PERMISSION_DENIED Has no permission to initialize input generator
   * @exception #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
   * @exception #EFL_UTIL_ERROR_NO_SUCH_DEVICE No such device or address
   * @exception #EFL_UTIL_ERROR_INVALID_OPERATION Function not implemented
   * @exception #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
   * @see efl_util_input_deinitialize_generator()
   */
API efl_util_inputgen_h efl_util_input_initialize_generator(unsigned int dev_type);

/**
   * @platform
   * @brief Initializes system, check input generate functions are supported and then open events generator devices with given name.
   * @since_tizen 4.0
   * @privlevel platform
   * @privilege %http://tizen.org/privilege/inputgenerator
   * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
   * @remarks The dev_type is changed into the unsigned int to perform bitwise operations.
   * @remarks The returned object should be released with efl_util_input_deinitialize_generator().
   * @param[in] dev_type The device type want to generate events (ex> #EFL_UTIL_INPUT_DEVTYPE_TOUCHSCREEN | #EFL_UTIL_INPUT_DEVTYPE_KEYBOARD)
   * @param[in] name The device name (maximum 31 characters)
   * @return #efl_util_inputgen_h on success, otherwise @c NULL
   * @exception #EFL_UTIL_ERROR_NONE Successful
   * @exception #EFL_UTIL_ERROR_PERMISSION_DENIED Has no permission to initialize input generator
   * @exception #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
   * @exception #EFL_UTIL_ERROR_NO_SUCH_DEVICE No such device or address
   * @exception #EFL_UTIL_ERROR_INVALID_OPERATION Function not implemented
   * @exception #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
   * @see efl_util_input_deinitialize_generator()
   */
API efl_util_inputgen_h efl_util_input_initialize_generator_with_name(unsigned int dev_type, const char *name);

/**
   * @platform
   * @brief Initializes the system, checks if input generated functions are supported and then open events generator devices synchronously.
   * @since_tizen 5.0
   * @privlevel platform
   * @privilege %http://tizen.org/privilege/inputgenerator
   * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
   * @remarks @a dev_type is changed into the unsigned int to perform bitwise operations.
   * @remarks If the @a name is NULL, it will be set to "Input Generator"
   * @remarks The returned object should be released with efl_util_input_deinitialize_generator().
   * @param[in] dev_type The device type to generate events, values of #efl_util_input_device_type_e combined with bitwise 'or'\n
   *            Example: #EFL_UTIL_INPUT_DEVTYPE_TOUCHSCREEN | #EFL_UTIL_INPUT_DEVTYPE_KEYBOARD
   * @param[in] name The device name (maximum 31 characters, can be NULL)
   * @return #efl_util_inputgen_h on success, otherwise @c NULL
   * @exception #EFL_UTIL_ERROR_NONE Successful
   * @exception #EFL_UTIL_ERROR_PERMISSION_DENIED Has no permission to initialize input generator
   * @exception #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
   * @exception #EFL_UTIL_ERROR_NO_SUCH_DEVICE No such device or address
   * @exception #EFL_UTIL_ERROR_INVALID_OPERATION Function not implemented
   * @exception #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
   * @see efl_util_input_deinitialize_generator()
   */
API efl_util_inputgen_h efl_util_input_initialize_generator_with_sync(unsigned int dev_type, const char *name);

/**
   * @platform
   * @brief Requests to set the maximum touch count.
   * @since_tizen 6.5
   * @privlevel platform
   * @privilege %http://tizen.org/privilege/inputgenerator
   * @remarks If you would like to generate more touch count supported than the display server supports,
              you can use this API to increase the maximum touch count.
              Note that this API returns success only when there is no configuration enabled
              about maximum touch count in the display server.
   * @param[in] max_count The maximum number of touches want to generate.
   * @return @c 0 on success, otherwise a negative error value
   * @retval #EFL_UTIL_ERROR_NONE Successful
   * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED Has no permission to deinitialize input generator
   * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
   * @retval #EFL_UTIL_ERROR_NO_RESOURCE_AVAILABLE Resource is not available
   */
API int efl_util_input_set_touch_count(int max_count);

/**
   * @platform
   * @brief Deinitializes system and close opened devices.
   * @since_tizen 2.4
   * @privlevel platform
   * @privilege %http://tizen.org/privilege/inputgenerator
   * @param[in] inputgen_h The #efl_util_inputgen_h handle
   * @return @c 0 on success, otherwise a negative error value
   * @retval #EFL_UTIL_ERROR_NONE Successful
   * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED Has no permission to deinitialize input generator
   * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
   * @see efl_util_input_initialize_generator()
   */
API int efl_util_input_deinitialize_generator(efl_util_inputgen_h inputgen_h);

/**
   * @platform
   * @brief Generates all of key events using a opened device.
   * @since_tizen 2.4
   * @privlevel platform
   * @privilege %http://tizen.org/privilege/inputgenerator
   * @param[in] inputgen_h The #efl_util_inputgen_h handle
   * @param[in] key_name The key name want to generate
   * @param[in] pressed The value that select key press or release (0: release, 1: press)
   * @return @c 0 on success, otherwise a negative error value
   * @retval #EFL_UTIL_ERROR_NONE Successful
   * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED Has no permission to generate key
   * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
   */
API int efl_util_input_generate_key(efl_util_inputgen_h inputgen_h, const char *key_name, int pressed);

/**
   * @platform
   * @brief Generates a touch event using a opened device.
   * @since_tizen 2.4
   * @privlevel platform
   * @privilege %http://tizen.org/privilege/inputgenerator
   * @param[in] inputgen_h The #efl_util_inputgen_h handle
   * @param[in] idx The index of touched finger
   * @param[in] touch_type The touch type (ex> #EFL_UTIL_INPUT_TOUCH_BEGIN, #EFL_UTIL_INPUT_TOUCH_UPDATE, #EFL_UTIL_INPUT_TOUCH_END)
   * @param[in] x The x axis of touch point
   * @param[in] y The y axis of touch point
   * @return @c 0 on success, otherwise a negative error value
   * @retval #EFL_UTIL_ERROR_NONE Successful
   * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED Has no permission to generate touch
   * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
   */
API int efl_util_input_generate_touch(efl_util_inputgen_h inputgen_h, int idx, efl_util_input_touch_type_e touch_type, int x, int y);

/**
   * @platform
   * @brief Generates a pointer event using a opened device.
   * @since_tizen 3.0
   * @privlevel platform
   * @privilege %http://tizen.org/privilege/inputgenerator
   * @param[in] inputgen_h The #efl_util_inputgen_h handle
   * @param[in] buttons The number of button
   * @param[in] pointer_type The pointer type (ex> #EFL_UTIL_INPUT_POINTER_BUTTON_PRESS, #EFL_UTIL_INPUT_POINTER_BUTTON_UP, #EFL_UTIL_INPUT_POINTER_MOVE)
   * @param[in] x x coordination to move
   * @param[in] y y coordination to move
   * @return @c 0 on success, otherwise a negative error value
   * @retval #EFL_UTIL_ERROR_NONE Successful
   * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED Has no permission to generate pointer
   * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
   * @retval #EFL_UTIL_ERROR_NO_SUCH_DEVICE No such device or address
   * @retval #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
   */
API int efl_util_input_generate_pointer(efl_util_inputgen_h inputgen_h, int buttons, efl_util_input_pointer_type_e pointer_type, int x, int y);

/**
   * @platform
   * @brief Generates a wheel event using an opened device.
   * @details Commonly wheel events are generated with one of two orientations, vertical and horizontal.
   *          #efl_util_input_pointer_wheel_type_e has enums for orientations.
   *          For each orientation there is a value which represents both size and direction.
   *          For #EFL_UTIL_INPUT_POINTER_WHEEL_VERT, if the value is positive, the direction is "up", if it's negative, the direction is "down"
   *          For #EFL_UTIL_INPUT_POINTER_WHEEL_HORZ, if the value is positive, the direction is "right", if it's negative, the direction is "left"
   *          General mouse devices generate wheel events with value 1 or -1,
   *          but some mouse devices have a smooth wheel which generates wheel events with value bigger than absolute 1.
   * @since_tizen 5.5
   * @privlevel platform
   * @privilege %http://tizen.org/privilege/inputgenerator
   * @param[in] inputgen_h The #efl_util_inputgen_h handle
   * @param[in] wheel_type The wheel type (ex> #EFL_UTIL_INPUT_POINTER_WHEEL_VERT, #EFL_UTIL_INPUT_POINTER_WHEEL_HORZ)
   * @param[in] value The wheel value
   * @return @c 0 on success, otherwise a negative error value
   * @retval #EFL_UTIL_ERROR_NONE Successful
   * @retval #EFL_UTIL_ERROR_NOT_SUPPORTED Not supported
   * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED No permission to generate a wheel event
   * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
   * @retval #EFL_UTIL_ERROR_NO_SUCH_DEVICE No such device or address
   * @retval #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
   */
API int efl_util_input_generate_wheel(efl_util_inputgen_h inputgen_h, efl_util_input_pointer_wheel_type_e wheel_type, int value);

/**
   * @platform
   * @brief Generates a touch event using an opened device.
   * @details Normally touch events are represented by a specific point (x, y).
   *          But there are many contact points between touchscreen and fingers.
   *          The coordinates (x, y) are the center of the contact area, and the area where the finger and the touchscreen come into contact is represented with an ellipse.
   *          To give information about the size of the touching cllipse, use @a radius_x and @a radius_y.
   *          The value of @a idx is touch index used in multi-touch. 0 means first touch, 1 means second touch and the index increases further.
   *          General devices support 10 touch indices(0 ~ 9), but this may differ depending on the device.
   *          Some devices may not support the @a palm value, and this value is closely dependent on device.
   *          The value of @a x, @a y, @a radius_x, @a radius_y, @a pressure and @a palm must be greater than 0.
   *          The @a angle is the angle between the vertical edge of the device and the y-axis of the ellipse. If the @a angle is 0, then the ellipse's y-axis is parallel to the device's vertical edge.
   * @since_tizen 5.5
   * @privlevel platform
   * @privilege %http://tizen.org/privilege/inputgenerator
   * @param[in] inputgen_h The #efl_util_inputgen_h handle
   * @param[in] idx The index of the touching finger, used in multi-touch
   * @param[in] touch_type The touch type (ex> #EFL_UTIL_INPUT_TOUCH_BEGIN, #EFL_UTIL_INPUT_TOUCH_UPDATE, #EFL_UTIL_INPUT_TOUCH_END)
   * @param[in] x The x coordinate of the touch point
   * @param[in] y The y coordinate of the touch point
   * @param[in] radius_x The x-axis radius of the touching ellipse before rotation
   * @param[in] radius_y The y-axis radius of the touching ellipse before rotation
   * @param[in] pressure The pressure on the contact touch area
   * @param[in] angle The rotation angle of the touching ellipse, in degrees
   * @param[in] palm The palm area of the contact touch area
   * @return @c 0 on success, otherwise a negative error value
   * @retval #EFL_UTIL_ERROR_NONE Successful
   * @retval #EFL_UTIL_ERROR_NOT_SUPPORTED Not supported
   * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED No permission to generate a touch event
   * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
   * @retval #EFL_UTIL_ERROR_NO_SUCH_DEVICE No such device or address
   * @retval #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
   */
API int efl_util_input_generate_touch_axis(efl_util_inputgen_h inputgen_h, int idx, efl_util_input_touch_type_e touch_type, int x, int y, double radius_x, double radius_y, double pressure, double angle, double palm);

/**
 * @}
 */

/**
 * @addtogroup CAPI_EFL_UTIL_SCREENSHOT_MODULE
 * @{
 */

/**
 * @platform
 * @brief Definition for the screenshot handle.
 * @since_tizen 2.4
 */
typedef struct _efl_util_screenshot_h * efl_util_screenshot_h;

/**
 * @platform
 * @brief Initializes the screenshot.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/screenshot
 * @remarks The specific error code can be obtained using the get_last_result()
 * method. Error codes are described in Exception section.
 * @param[in] width width of the screenshot surface
 * @param[in] height height of the screenshot surface
 * @return #efl_util_screenshot_h on success, otherwise @c NULL
 * @retval #efl_util_screenshot_h  The screenshot handle
 * @exception #EFL_UTIL_ERROR_NONE Successful
 * @exception #EFL_UTIL_ERROR_PERMISSION_DENIED No permission for screenshot
 * @exception #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
 * @exception #EFL_UTIL_ERROR_SCREENSHOT_INIT_FAIL Initialization failure
 * @see efl_util_screenshot_deinitialize()
 */
API efl_util_screenshot_h efl_util_screenshot_initialize(int width, int height);

/**
 * @platform
 * @brief Takes a screenshot and get a tbm_surface handle.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/screenshot
 * @remarks The specific error code can be obtained using the get_last_result()
 *          The #tbm_surface_h must be free by caller
 * @param[in] screenshot #efl_util_screenshot_h handle
 * @return #tbm_surface_h on success, otherwise @c NULL
 * @retval #tbm_surface_h The TBM surface handle
 * @exception #EFL_UTIL_ERROR_NONE Successful
 * @exception #EFL_UTIL_ERROR_PERMISSION_DENIED No permission for screenshot
 * @exception #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #EFL_UTIL_ERROR_SCREENSHOT_EXECUTION_FAIL Execution failure
 * @see efl_util_screenshot_initialize()
 * @see efl_util_screenshot_deinitialize()
 */
API tbm_surface_h efl_util_screenshot_take_tbm_surface(efl_util_screenshot_h screenshot);

/**
 * @platform
 * @brief Deinitializes the screenshot.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/screenshot
 * @param[in]  screenshot  #efl_util_screenshot_h handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED No permission for screenshot
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see efl_util_screenshot_initialize()
 */
API int efl_util_screenshot_deinitialize(efl_util_screenshot_h screenshot);

/**
 * @}
 */

/**
 * @NOT_AVAILABLE_ON_TV
 * @addtogroup CAPI_EFL_UTIL_GESTURE_MODULE
 * @{
 */

/**
 * @brief Definition for the gesture handle.
 * @since_tizen 4.0
 */
typedef struct _efl_util_gesture_h * efl_util_gesture_h;

/**
 * @brief Definition for the edge swipe gesture event.
 * @since_tizen 4.0
 */
API extern int EFL_UTIL_EVENT_GESTURE_EDGE_SWIPE;

/**
 * @brief Definition for the edge drag gesture event.
 * @since_tizen 4.0
 */
API extern int EFL_UTIL_EVENT_GESTURE_EDGE_DRAG;

/**
 * @brief Definition for the tap gesture event.
 * @since_tizen 4.0
 */
API extern int EFL_UTIL_EVENT_GESTURE_TAP;

/**
 * @brief Definition for the palm cover gesture event.
 * @since_tizen 4.0
 */
API extern int EFL_UTIL_EVENT_GESTURE_PALM_COVER;

/**
 * @brief Enumeration of gesture type.
 * @since_tizen 4.0
 */
typedef enum _efl_util_gesture_type_e
{
   EFL_UTIL_GESTURE_TYPE_NONE       = 0,        /**< None gesture type */
   EFL_UTIL_GESTURE_TYPE_EDGE_SWIPE = (1 << 0), /**< Edge swipe gesture type */
   EFL_UTIL_GESTURE_TYPE_EDGE_DRAG  = (1 << 1), /**< Edge drag gesture type */
   EFL_UTIL_GESTURE_TYPE_TAP        = (1 << 2), /**< Tap gesture type */
   EFL_UTIL_GESTURE_TYPE_PALM_COVER = (1 << 3), /**< Palm cover gesture type */
   EFL_UTIL_GESTURE_TYPE_PAN        = (1 << 4), /**< Pan gesture type */
   EFL_UTIL_GESTURE_TYPE_PINCH      = (1 << 5), /**< Pinch gesture type */
   EFL_UTIL_GESTURE_TYPE_PALM_SWIPE = (1 << 6)  /**< Palm swipe gesture type */
} efl_util_gesture_type_e;

/**
 * @brief Enumeration of gesture mode.
 * @since_tizen 4.0
 */
typedef enum _efl_util_gesture_mode_e
{
   EFL_UTIL_GESTURE_MODE_NONE    = 0, /**< None gesture mode */
   EFL_UTIL_GESTURE_MODE_BEGIN,       /**< Begin a gesture event */
   EFL_UTIL_GESTURE_MODE_UPDATE,      /**< Continually update a gesture event */
   EFL_UTIL_GESTURE_MODE_END,         /**< End a gesture event */
   EFL_UTIL_GESTURE_MODE_DONE         /**< Occur a gesture event */
} efl_util_gesture_mode_e;

/**
 * @brief Enumeration of gesture edge.
 * @since_tizen 4.0
 */
typedef enum _efl_util_gesture_edge_e
{
   EFL_UTIL_GESTURE_EDGE_NONE    = 0, /**< None edge point */
   EFL_UTIL_GESTURE_EDGE_TOP,         /**< Top edge position of screen */
   EFL_UTIL_GESTURE_EDGE_RIGHT,       /**< Right edge position of screen */
   EFL_UTIL_GESTURE_EDGE_BOTTOM,      /**< Bottom edge position of screen */
   EFL_UTIL_GESTURE_EDGE_LEFT         /**< Left edge position of screen */
} efl_util_gesture_edge_e;

/**
 * @brief Enumeration of gesture edge size.
 * @since_tizen 4.0
 */
typedef enum _efl_util_gesture_edge_size_e
{
   EFL_UTIL_GESTURE_EDGE_SIZE_NONE,    /**< None size of edge */
   EFL_UTIL_GESTURE_EDGE_SIZE_FULL,    /**< Full size in the edge */
   EFL_UTIL_GESTURE_EDGE_SIZE_PARTIAL  /**< Part of edge */
} efl_util_gesture_edge_size_e;

/**
 * @platform
 * @brief Enumeration of gesture grab mode.
 * @since_tizen 9.0
 */
typedef enum _efl_util_gesture_grab_mode_e
{
   EFL_UTIL_GESTURE_GRAB_MODE_NONE = 0,  /**< none mode */
   EFL_UTIL_GESTURE_GRAB_MODE_EXCLUSIVE, /**< exclusive mode */
   EFL_UTIL_GESTURE_GRAB_MODE_SHARED,    /**< shared grab mode */
} efl_util_gesture_grab_mode_e;

/**
 * @brief Definition for the gesture data handle.
 * @since_tizen 4.0
 */
typedef void *efl_util_gesture_data;

/**
 * @brief Definition for the edge swipe gesture's event data structure.
 * @since_tizen 4.0
 */
typedef struct _efl_util_event_gesture_edge_swipe_s
{
   efl_util_gesture_mode_e mode;  /**< Mode of a gesture event */
   unsigned int fingers;          /**< Number of fingers */
   int sx;                        /**< Start x point of edge area */
   int sy;                        /**< Start y point of edge area */
   unsigned int edge;             /**< Start edge location */
} efl_util_event_gesture_edge_swipe_s;

/**
 * @brief Definition for the edge drag gesture's event data structure.
 * @since_tizen 4.0
 */
typedef struct efl_util_event_gesture_edge_drag_s
{
   efl_util_gesture_mode_e mode;  /**< Mode of a gesture event */
   unsigned int fingers;          /**< Number of fingers */
   int cx;                        /**< Center x point of edge area */
   int cy;                        /**< Center y point of edge area */
   unsigned int edge;             /**< Start edge location */
} efl_util_event_gesture_edge_drag_s;

/**
 * @brief Definition for the tap gesture's event data structure.
 * @since_tizen 4.0
 */
typedef struct _efl_util_event_gesture_tap_s
{
   efl_util_gesture_mode_e mode;  /**< Mode of a gesture event */
   unsigned int fingers;          /**< Number of fingers */
   unsigned int repeats;          /**< Number of tap repeats */
} efl_util_event_gesture_tap_s;

/**
 * @brief Definition for the palm cover gesture's event data structure.
 * @since_tizen 4.0
 */
typedef struct _efl_util_event_gesture_palm_cover_s
{
   efl_util_gesture_mode_e mode;  /**< Mode of a gesture event */
   unsigned int duration;         /**< Duration time of gesture behavior */
   int cx;                        /**< Center x point of palm covered area */
   int cy;                        /**< Center y point of palm covered area */
   unsigned int size;             /**< Size of palm covered area */
   double pressure;               /**< Pressure of touched finger */
} efl_util_event_gesture_palm_cover_s;

/**
 * @brief Initializes system and check if global gestures are supported.
 * @since_tizen 4.0
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *          Error codes are described in Exception section.
 * @remarks The returned object should be released with efl_util_gesture_deinitialize().
 * @return #efl_util_gesture_h on success, otherwise @c NULL
 * @exception #EFL_UTIL_ERROR_NONE Successful
 * @exception #EFL_UTIL_ERROR_NOT_SUPPORTED Not supported
 * @exception #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
 * @exception #EFL_UTIL_ERROR_NO_RESOURCE_AVAILABLE Resource is not available
 * @see efl_util_gesture_deinitialize()
 */
API efl_util_gesture_h efl_util_gesture_initialize(void);

/**
 * @brief Deinitializes system.
 * @since_tizen 4.0
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see efl_util_gesture_initialize()
 */
API int efl_util_gesture_deinitialize(efl_util_gesture_h gesture_h);

/**
 * @brief Generates a edge swipe gesture's grab info handle.
 * @since_tizen 4.0
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *          Error codes are described in Exception section.
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] fingers   The number of fingers
 * @param[in] edge      The position of edge
 * @return #efl_util_gesture_data on success, otherwise @c NULL
 * @exception #EFL_UTIL_ERROR_NONE Successful
 * @exception #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
 * @exception #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #EFL_UTIL_ERROR_NO_RESOURCE_AVAILABLE Resource is not available
 */
API efl_util_gesture_data efl_util_gesture_edge_swipe_new(efl_util_gesture_h gesture_h, unsigned int fingers, efl_util_gesture_edge_e edge);

/**
 * @brief Frees a memory of edge swipe gesture's grab info handle.
 * @since_tizen 4.0
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] data      The #efl_util_gesture_data handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 */
API int efl_util_gesture_edge_swipe_free(efl_util_gesture_h gesture_h, efl_util_gesture_data data);


/**
 * @brief Sets a specific size of edge for edge swipe gesture.
 * @since_tizen 4.0
 * @param[in] data        The #efl_util_gesture_data handle
 * @param[in] edge_size   The #efl_util_gesture_edge_size_e enum
 * @param[in] start_point The start point of edge area
 * @param[in] end_point   The end point of edge area
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_NO_RESOURCE_AVAILABLE Resource is not available
 */
API int efl_util_gesture_edge_swipe_size_set(efl_util_gesture_data data, efl_util_gesture_edge_size_e edge_size, unsigned int start_point, unsigned int end_point);

/**
 * @brief Generates a edge drag gesture's grab info handle.
 * @since_tizen 4.0
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *          Error codes are described in Exception section.
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] fingers   The number of fingers
 * @param[in] edge      The position of edge
 * @return #efl_util_gesture_data on success, otherwise @c NULL
 * @exception #EFL_UTIL_ERROR_NONE Successful
 * @exception #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
 * @exception #EFL_UTIL_ERROR_NO_RESOURCE_AVAILABLE Resource is not available
 */
API efl_util_gesture_data efl_util_gesture_edge_drag_new(efl_util_gesture_h gesture_h, unsigned int fingers, efl_util_gesture_edge_e edge);

/**
 * @brief Frees a memory of edge drag gesture's grab info handle.
 * @since_tizen 4.0
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] data      The #efl_util_gesture_data handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 */
API int efl_util_gesture_edge_drag_free(efl_util_gesture_h gesture_h, efl_util_gesture_data data);

/**
 * @brief Sets a specific size of edge for edge drag gesture.
 * @since_tizen 4.0
 * @param[in] data        The #efl_util_gesture_data handle
 * @param[in] edge_size   The #efl_util_gesture_edge_size_e enum
 * @param[in] start_point The start point of edge area
 * @param[in] end_point   The end point of edge area
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_NO_RESOURCE_AVAILABLE Resource is not available
 */
API int efl_util_gesture_edge_drag_size_set(efl_util_gesture_data data, efl_util_gesture_edge_size_e edge_size, unsigned int start_point, unsigned int end_point);

/**
 * @brief Generates a tap gesture's grab info handle.
 * @since_tizen 4.0
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *          Error codes are described in Exception section.
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] fingers   The number of fingers
 * @param[in] repeats   The number of repeats
 * @return #efl_util_gesture_data on success, otherwise @c NULL
 * @exception #EFL_UTIL_ERROR_NONE Successful
 * @exception #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
 * @exception #EFL_UTIL_ERROR_NO_RESOURCE_AVAILABLE Resource is not available
 */
API efl_util_gesture_data efl_util_gesture_tap_new(efl_util_gesture_h gesture_h, unsigned int fingers, unsigned int repeats);

/**
 * @brief Frees a memory of tap gesture's grab info handle.
 * @since_tizen 4.0
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] data      The #efl_util_gesture_data handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 */
API int efl_util_gesture_tap_free(efl_util_gesture_h gesture_h, efl_util_gesture_data data);

/**
 * @brief Generates a palm cover gesture's grab info handle.
 * @since_tizen 4.0
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *          Error codes are described in Exception section.
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @return #efl_util_gesture_data on success, otherwise @c NULL
 * @exception #EFL_UTIL_ERROR_NONE Successful
 * @exception #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
 * @exception #EFL_UTIL_ERROR_NO_RESOURCE_AVAILABLE Resource is not available
 */
API efl_util_gesture_data efl_util_gesture_palm_cover_new(efl_util_gesture_h gesture_h);

/**
 * @brief Frees a memory of palm cover gesture's grab info handle.
 * @since_tizen 4.0
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] data      The #efl_util_gesture_data handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 */
API int efl_util_gesture_palm_cover_free(efl_util_gesture_h gesture_h, efl_util_gesture_data data);

/**
 * @platform
 * @brief Grabs a global gesture.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/gesturegrab
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] data      The #efl_util_gesture_data handle.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_NOT_SUPPORTED Not supported
 * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
 * @retval #EFL_UTIL_ERROR_NO_RESOURCE_AVAILABLE Resource is not available
 */
API int efl_util_gesture_grab(efl_util_gesture_h gesture_h, efl_util_gesture_data data);

/**
 * @platform
 * @brief Ungrabs a global gesture.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/gesturegrab
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] data      The #efl_util_gesture_data handle.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_NOT_SUPPORTED Not supported
 * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
 * @retval #EFL_UTIL_ERROR_NO_RESOURCE_AVAILABLE Resource is not available
 */
API int efl_util_gesture_ungrab(efl_util_gesture_h gesture_h, efl_util_gesture_data data);

/**
 * @brief Selects a global gesture on given window.
 * @since_tizen 4.0
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] window    The efl window
 * @param[in] data      The #efl_util_gesture_data handle.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
 * @retval #EFL_UTIL_ERROR_NO_RESOURCE_AVAILABLE Resource is not available
 */
API int efl_util_gesture_select(efl_util_gesture_h gesture_h, Evas_Object *window, efl_util_gesture_data data);

/**
 * @brief Deselects a global gesture on given window.
 * @since_tizen 4.0
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] window    The efl window
 * @param[in] data      The #efl_util_gesture_data handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
 * @retval #EFL_UTIL_ERROR_NO_RESOURCE_AVAILABLE Resource is not available
 */
API int efl_util_gesture_deselect(efl_util_gesture_h gesture_h, Evas_Object *window, efl_util_gesture_data data);

/**
 * @platform
 * @brief Activates or deactivates a global gesture.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/gestureactivation
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] type      The gesture type to activate /deactivate combined by #efl_util_gesture_type_e to bitwise operation
 * @param[in] active    The activated boolean value
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
 * @retval #EFL_UTIL_ERROR_NO_RESOURCE_AVAILABLE Resource is not available
 */
API int efl_util_gesture_activate_set(efl_util_gesture_h gesture_h, unsigned int type, Eina_Bool active);

/**
 * @brief Activates or deactivates a global gesture on given window.
 * @since_tizen 4.0
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] window    The efl window
 * @param[in] type      The gesture type to activate /deactivate combined by #efl_util_gesture_type_e to bitwise operation
 * @param[in] active    The activated boolean value
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
 * @retval #EFL_UTIL_ERROR_NO_RESOURCE_AVAILABLE Resource is not available
 */
API int efl_util_gesture_activate_set_on_window(efl_util_gesture_h gesture_h, Evas_Object *window, unsigned int type, Eina_Bool active);

/**
 * @brief Handle for the callback function of edge swipe gesture.
 * @since_tizen 7.0
 * @param[in] user_data    The data to be passed to the callback function
 * @param[in] mode    The mode of a gesture event
 * @param[in] fingers The number of fingers
 * @param[in] sx      The start x point of edge area
 * @param[in] sy      The start y point of edge area
 * @param[in] edge    The position of edge
 */
typedef void (*efl_util_gesture_edge_swipe_cb)(void *user_data, int mode, int fingers, int sx, int sy, int edge);

/**
 * @brief Handle for the callback function of edge drag gesture.
 * @since_tizen 7.0
 * @param[in] user_data    The data to be passed to the callback function
 * @param[in] mode    The mode of a gesture event
 * @param[in] fingers The number of fingers
 * @param[in] cx      The center x point of edge area
 * @param[in] cy      The center y point of edge area
 * @param[in] edge    The position of edge
 */
typedef void (*efl_util_gesture_edge_drag_cb)(void *user_data, int mode, int fingers, int cx, int cy, int edge);
/**
 * @brief Handle for the callback function of tap gesture.
 * @since_tizen 7.0
 * @param[in] user_data    The data to be passed to the callback function
 * @param[in] mode    The mode of a gesture event
 * @param[in] fingers The number of fingers
 * @param[in] repeats The number of repeats
 */
typedef void (*efl_util_gesture_tap_cb)(void *user_data, int mode, int fingers, int repeats);
/**
 * @brief Handle for the callback function of palm cover gesture.
 * @since_tizen 7.0
 * @param[in] user_data     The data to be passed to the callback function
 * @param[in] mode     The mode of a gesture event
 * @param[in] duration The duration time of gesture behavior
 * @param[in] cx       The center x point of palm covered area
 * @param[in] cy       The center y point of palm covered area
 * @param[in] size     The size of palm covered area
 * @param[in] pressure The pressure of touched finger
 */
typedef void (*efl_util_gesture_palm_cover_cb)(void *user_data, int mode, int duration, int cx, int cy, int size, double pressure);

/**
 * @brief Sets edge swipe gesture callback function of given efl util gesture handle.
 * @since_tizen 7.0
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] cb_func   The callback function pointer
 * @param[in] data      The data to be passed to the callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 */
API int efl_util_gesture_edge_swipe_cb_set(efl_util_gesture_h gesture_h, efl_util_gesture_edge_swipe_cb cb_func, void *data);

/**
 * @brief Sets edge drag gesture callback function of given efl util gesture handle.
 * @since_tizen 7.0
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] cb_func   The callback function pointer
 * @param[in] data      The data to be passed to the callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 */
API int efl_util_gesture_edge_drag_cb_set(efl_util_gesture_h gesture_h, efl_util_gesture_edge_drag_cb cb_func, void *data);

/**
 * @brief Sets tap gesture callback function of given efl util gesture handle.
 * @since_tizen 7.0
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] cb_func   The callback function pointer
 * @param[in] data      The data to be passed to the callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 */
API int efl_util_gesture_tap_cb_set(efl_util_gesture_h gesture_h, efl_util_gesture_tap_cb cb_func, void *data);

/**
 * @brief Sets palm cover gesture callback function of given efl util gesture handle.
 * @since_tizen 7.0
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] cb_func   The callback function pointer
 * @param[in] data      The data to be passed to the callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 */
API int efl_util_gesture_palm_cover_cb_set(efl_util_gesture_h gesture_h, efl_util_gesture_palm_cover_cb cb_func, void *data);

/**
 * @brief Sets gesture grab mode.
 * @since_tizen 9.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/gesturegrab
 * @param[in] gesture_h The #efl_util_gesture_h handle
 * @param[in] data      The data to be passed to the callback function
 * @param[in] mode      The #efl_util_gesture_grab_mode_e mode
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 */
API int efl_util_gesture_grab_mode_set(efl_util_gesture_h gesture_h, efl_util_gesture_data data, efl_util_gesture_grab_mode_e mode);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif /* __TIZEN_UI_EFL_UTIL_H__ */
