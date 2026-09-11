/*
 * Copyright (c) 2026 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_UIX_INPUTMETHOD_CORE_DEVICE_EVENT_H__
#define __TIZEN_UIX_INPUTMETHOD_CORE_DEVICE_EVENT_H__

/**
 * @file inputmethod_core_device_event.h
 * @brief This file contains definitions for unconventional input device events.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The handle to retrieve unconventional input device specific event data.
 *
 * @since_tizen 11.0
 *
 * @see inputmethod_event_set_process_input_device_event_cb()
 */
typedef struct _inputmethod_input_device_event *inputmethod_input_device_event_h;

/**
 * @brief Enumeration of unconventional input devices.
 *
 * @since_tizen 11.0
 *
 * @see inputmethod_event_set_process_input_device_event_cb()
 */
typedef enum {
    INPUTMETHOD_INPUT_DEVICE_TYPE_UNKNOWN, /**< Undefined unconventional input device */
    INPUTMETHOD_INPUT_DEVICE_TYPE_ROTARY, /**< A rotary input device such as bezel that can be found on a wearable device */
} inputmethod_input_device_type_e;

/**
 * @brief Enumeration of directions for rotary input device's rotation event.
 *
 * @since_tizen 11.0
 *
 * @see inputmethod_input_device_rotary_get_direction()
 */
typedef enum
{
    INPUTMETHOD_INPUT_DEVICE_ROTARY_DIRECTION_CLOCKWISE, /**< Rotary is rotated clockwise direction */
    INPUTMETHOD_INPUT_DEVICE_ROTARY_DIRECTION_COUNTER_CLOCKWISE /**< Rotary is rotated counter clockwise direction */
} inputmethod_input_device_rotary_direction_e;

/**
 * @brief Called when the input event is received from an unconventional input device that does not generate key events.
 *
 * @details This function processes the input event before an associated text input UI control does.
 *
 * @since_tizen 11.0
 *
 * @remarks @a device_type contains the information what kind of unconventional input device generated the given event,
 * and the handle @a device_event is used for obtaining device-specific input device event data.
 * @a device_event should not be released.
 *
 * @param[in] device_type The unconventional input device type
 * @param[in] device_event The handle for device_type specific input device event
 * @param[in] user_data User data to be passed to the callback function
 *
 * @pre The callback can be registered using inputmethod_event_set_process_input_device_event_cb() function.
 *
 * @see inputmethod_event_set_process_input_device_event_cb()
 */
typedef void(*inputmethod_process_input_device_event_cb)(inputmethod_input_device_type_e device_type, inputmethod_input_device_event_h device_event, void *user_data);

/**
 * @brief Sets @c process_input_device_event event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_process_input_device_event_cb() callback function is called when the event
 * is received from unconventional input devices that needs to be handled by IMEs.
 *
 * @param[in] callback_func @c process_input_device_event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_process_input_device_event_cb()
 * @see inputmethod_event_unset_process_input_device_event_cb()
 */
int inputmethod_event_set_process_input_device_event_cb(inputmethod_process_input_device_event_cb callback_func, void *user_data);

/**
 * @brief Unsets @c process_input_device_event event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_process_input_device_event_cb() callback function is called when the event
 * is received from unconventional input devices that needs to be handled by IMEs.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_process_input_device_event_cb()
 * @see inputmethod_event_set_process_input_device_event_cb()
 */
int inputmethod_event_unset_process_input_device_event_cb(void);

/**
 * @brief Gets the direction of the rotary input device event
 *
 * If the device_type parameter of the inputmethod_process_input_device_event_cb() function indicates
 * the current input device type is #INPUTMETHOD_INPUT_DEVICE_TYPE_ROTARY, then the device_event parameter
 * can be used to retrieve rotary device specific parameters, such as direction.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] event_handle The input device event handle
 * @param[out] direction The direction that the rotary input device was rotated to
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_input_device_rotary_direction_e
 * @see inputmethod_process_input_device_event_cb()
 */
int inputmethod_input_device_rotary_get_direction(inputmethod_input_device_event_h event_handle, inputmethod_input_device_rotary_direction_e *direction);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __TIZEN_UIX_INPUTMETHOD_CORE_DEVICE_EVENT_H__ */