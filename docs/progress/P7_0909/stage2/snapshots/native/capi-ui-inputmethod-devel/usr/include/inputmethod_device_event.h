/*
 * Copyright (c) 2016-2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_UIX_INPUTMETHOD_DEVICE_EVENT_H__
#define __TIZEN_UIX_INPUTMETHOD_DEVICE_EVENT_H__

/**
 * @file inputmethod_device_event.h
 * @brief This file contains definitions for unconventional input device events.
 */

#include <Ecore_Input.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_UIX_INPUTMETHOD_MODULE
 * @{
 */

 /**
 * @brief The handle to retrieve unconventional input device specific event data.
 *
 * @since_tizen 3.0
 *
 * @see ime_event_set_process_input_device_event_cb()
 */
typedef void *ime_input_device_event_h;

/**
 * @brief Enumeration of unconventional input devices.
 *
 * @since_tizen 3.0
 *
 * @see ime_event_set_process_input_device_event_cb()
 */
typedef enum {
    IME_INPUT_DEVICE_TYPE_UNKNOWN, /**< Undefined unconventional input device */
    IME_INPUT_DEVICE_TYPE_ROTARY, /**< A rotary input device such as bezel that can be found on a wearable device */
} ime_input_device_type_e;

/**
 * @brief Enumeration of directions for rotary input device's rotation event.
 *
 * @since_tizen 3.0
 *
 * @see ime_input_device_rotary_get_direction()
 */
typedef enum
{
    IME_INPUT_DEVICE_ROTARY_DIRECTION_CLOCKWISE, /**< Rotary is rotated clockwise direction */
    IME_INPUT_DEVICE_ROTARY_DIRECTION_COUNTER_CLOCKWISE /**< Rotary is rotated counter clockwise direction */
} ime_input_device_rotary_direction_e;

/**
 * @brief Called when the input event is received from an unconventional input device that does not generate key events.
 *
 * @details This function processes the input event before an associated text input UI control does.
 *
 * @since_tizen 3.0
 *
 * @remarks @a device_type contains the information what kind of unconventional input device generated the given event,
 * and the handle @a device_event is used for obtaining device-specific input device event data.
 * @a device_event should not be released.
 *
 * @param[in] device_type The unconventional input device type
 * @param[in] device_event The handle for device_type specific input device event
 * @param[in] user_data User data to be passed to the callback function
 *
 * @pre The callback can be registered using ime_event_set_input_device_event_cb() function.
 *
 * @see ime_event_set_process_input_device_event_cb()
 */
typedef void(*ime_process_input_device_event_cb)(ime_input_device_type_e device_type, ime_input_device_event_h device_event, void *user_data);

/**
 * @brief Sets @c process_input_device_event event callback function.
 *
 * @since_tizen 3.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_process_input_device_event_cb() callback function is called when the event
 * is received from unconventional input devices that needs to be handled by IMEs.
 *
 * @param[in] callback_func @c inputmethod_process_input_device_event_cb callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_process_input_device_event_cb()
 * @see ime_event_unset_process_input_device_event_cb()
 * @see ime_run()
 *
 * @code
 static void inputmethod_create_cb(void *user_data);
 static void inputmethod_terminate_cb(void *user_data);
 static void inputmethod_show_cb(int context_id, ime_context_h context, void *user_data);
 static void inputmethod_hide_cb(int context_id, void *user_data);

 static void inputmethod_process_input_device_event_cb(ime_input_device_type_e device_type, ime_input_device_event_h device_event, void *user_data)
 {
    if (device_type == IME_INPUT_DEVICE_TYPE_ROTARY) {
        dlog_print(DLOG_INFO, "INPUTMETHOD", "Input device type is rotary\n");
    }
 }

 void ime_app_main(int argc, char **argv)
 {
    ime_callback_s basic_callback = {
        inputmethod_create_cb,
        inputmethod_terminate_cb,
        inputmethod_show_cb,
        inputmethod_hide_cb,
    };

    ime_event_set_process_input_device_event_cb(inputmethod_process_input_device_event_cb, NULL);

    ime_run(&basic_callback, NULL);
 }
 * @endcode
 */

int ime_event_set_process_input_device_event_cb(ime_process_input_device_event_cb callback_func, void *user_data);

/**
 * @brief Unsets @c process_input_device_event event callback function.
 *
 * @since_tizen 3.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_process_input_device_event_cb() callback function is called when the event
 * is received from unconventional input devices that needs to be handled by IMEs.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @see ime_process_input_device_event_cb()
 * @see ime_event_set_process_input_device_event_cb()
 */
int ime_event_unset_process_input_device_event_cb(void);

/**
 * @brief Gets the direction of the rotary input device event
 *
 * If the device_type parameter of the ime_process_input_device_event_cb() function indicates
 * the current input device type is #IME_INPUT_DEVICE_TYPE_ROTARY, then the device_event parameter
 * can be used to retrieve rotary device specific parameters, such as direction, as shown in the sample code.
 *
 * @since_tizen 3.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] event_handle The input device event handle
 * @param[out] direction The direction that the rotary input device was rotated to
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @see ime_input_device_rotary_direction_e
 * @see ime_process_input_device_event_cb()
 *
 * @code
 static void inputmethod_process_input_device_event_cb(ime_input_device_type_e device_type, ime_input_device_event_h device_event)
 {
    if (device_type == IME_INPUT_DEVICE_TYPE_ROTARY) {
        ime_input_device_rotary_direction_e direction;
        if (IME_ERROR_NONE == ime_input_device_rotary_get_direction(device_event, &direction)) {
            dlog_print(DLOG_INFO, "INPUTMETHOD", "Direction : %d", direction);
        }
    }
 }
 * @endcode
 */
int ime_input_device_rotary_get_direction(ime_input_device_event_h event_handle, ime_input_device_rotary_direction_e *direction);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __TIZEN_UIX_INPUTMETHOD_DEVICE_EVENT_H__ */
