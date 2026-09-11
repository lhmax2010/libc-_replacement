/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_SYSTEM_INPUT_INTERNAL_H__
#define __TIZEN_SYSTEM_INPUT_INTERNAL_H__

/**
 * @addtogroup CAPI_SYSTEM_DEVICE_INPUT_INTERNAL_MODULE
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "device-error.h"

/**
 * @brief Enumeration for the distinction of input device types.
 * @since_tizen 7.0
 */
typedef enum
{
	DEVICE_INPUT_TYPE_UNKNOWN = 0, /**< Input device type which is not defined. */
	DEVICE_INPUT_TYPE_ALL, /**< Input device type which is all kinds of input devices */
	DEVICE_INPUT_TYPE_MOUSE, /**< Input device type which is mouse-type */
	DEVICE_INPUT_TYPE_KEYBOARD, /**< Input device type which is keyboard-type */
	DEVICE_INPUT_TYPE_TOUCHSCREEN, /**< Input device type which is touchscreen-type */

	DEVICE_INPUT_TYPE_CUSTOM_KNOB = 1000, /**< Input device type which is customed knob-type */
} device_input_type_e;

/**
 * @brief Gets the input default device ID for a specified input device type.
 * @details Retrieves the default device ID for a specified input device type.
 *          The input device type is specified in the @a input_device_type parameter, \n
 *          and the corresponding default device ID is returned in the @a input_device_id parameter.
 * @since_tizen 7.0
 * @remarks #DEVICE_ERROR_NOT_SUPPORTED is returned, when the trying to control DEVICE_INPUT_TYPE_KEYBOARD device
 *          and following feature is not supported: %http://tizen.org/feature/input.keyboard \n
 * @param[in] input_device_type The type to get input device information \n
 * @param[out] input_device_id The ID value to be get from the input devices by input_device_type \n
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported in this device
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation not permitted
 *
 * @code
 * #include <device/input-internal.h>
 * ...
 * device_input_type_e input_device_type = DEVICE_INPUT_TYPE_MOUSE;
 * int input_device_id = 0;
 * int ret = 0;
 * ...
 * ret = device_input_get_default_device(input_device_type, &input_device_id);
 * if (ret == DEVICE_ERROR_NONE) {
 *     ...
 * }
 * ...
 * @endcode
 * @see device_input_type_e
 */
int device_input_get_default_device(device_input_type_e input_device_type, int *input_device_id);

/**
 * @brief Gets input device ID list for a specified input device type.
 * @details Retrieves a list of device IDs for a specified input device type. \n
 *          The input device type is specified in the @a input_device_type parameter, \n
 *          and the corresponding list of device IDs is returned in the @a input_device_ids parameter. \n
 *          The number of device IDs in the list is returned in the @a num_input_device_ids parameter.
 * @since_tizen 7.0
 * @remarks #DEVICE_ERROR_NOT_SUPPORTED is returned, when the trying to control DEVICE_INPUT_TYPE_KEYBOARD device
 *          and following feature is not supported: %http://tizen.org/feature/input.keyboard \n
 *          after using input_device_ids, it should be freed by caller \n
 * @param[in] input_device_type The type to get input devices information \n
 * @param[out] input_deivce_ids The ID list value to be get from the input devices \n
 * @param[out] num_input_device_ids The number of IDs to be get from the input devices \n
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported in this device
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation not permitted
 *
 * @code
 * #include <device/input-internal.h>
 * ...
 * device_input_type_e input_device_type = DEVICE_INPUT_TYPE_ALL;
 * int *input_device_ids = NULL;
 * int num_of_input_device_ids = 0;
 * int ret = 0;
 * ...
 * ret = device_input_get_devices(input_device_type, &input_device_ids, &num_of_input_device_ids);
 * if (ret != DEVICE_ERROR_NONE) {
 *     return -1;
 * }
 * ...
 * if (input_device_ids) {
 *    free(input_device_ids);
 * }
 * ...
 * @endcode
 * @see device_input_type_e
 */
int device_input_get_devices(device_input_type_e input_device_type, int **input_deivce_ids, int *num_input_device_ids);

/**
 * @brief Changes the event state of an input device by input device ID.
 * @details The input device ID is specified in the @a input_device_id parameter, \n
 *          and the desired event state is specified in the @a on parameter.
 *          The event state is represented by a boolean value, \n
 *          where @c true indicates that events should be inhibited, and @c false indicates that events should be enabled.
 * @since_tizen 7.0
 * @remarks #DEVICE_ERROR_NOT_SUPPORTED is returned, when the trying to control DEVICE_INPUT_TYPE_KEYBOARD device
 *          and following feature is not supported: %http://tizen.org/feature/input.keyboard \n
 * @param[in] input_device_id The input ID of the input device under the sys/class/input node \n
 *                            This is for the distinction and control of input devices \n
 * @param[in] on The value to be set for the input device event state \n
 *               @c true means inhibit events, @c false means enable events \n
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported in this device
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation not permitted
 *
 * @code
 * #include <device/input-internal.h>
 * ...
 * device_input_type_e input_device_type = DEVICE_INPUT_TYPE_ALL;
 * int *input_device_ids = NULL;
 * int num_of_input_device_ids = 0;
 * int ret = 0;
 * ...
 * ret = device_input_get_devices(input_device_type, &input_device_ids, &num_of_input_device_ids);
 * if (ret != DEVICE_ERROR_NONE) {
 *     return -1;
 * }
 * for (int i = 0; i < num_of_input_device_ids; i++) {
 *    ret = device_input_set_event_state(input_device_ids[i], true);
 *    ...
 * }
 * ...
 * if (input_device_ids) {
 *     free(input_device_ids);
 * }
 * ...
 * @endcode
 * @see device_input_get_default_device()
 * @see device_input_get_devices()
 * @see device_input_get_event_state()
 */
int device_input_set_event_state(int input_device_id, bool on);

/**
 * @brief Gets the event state of an input device by the input device ID.
 * @details The input device ID is specified in the @a input_device_id parameter, \n
 *          The event state is represented by a @a on boolean parameter, \n
 *          where @c true indicates that events was inhibited, and @c false indicates that events was enabled.
 * @since_tizen 7.0
 * @remarks #DEVICE_ERROR_NOT_SUPPORTED is returned, when the trying to control DEVICE_INPUT_TYPE_KEYBOARD device
 *          and following feature is not supported: %http://tizen.org/feature/input.keyboard \n
 * @param[in] input_device_id The input ID of the input device under the sys/class/input node \n
 *                            This is for the distinction and control of input devices \n
 * @param[out] on The value to be get from the input device event state \n
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported in this device
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation not permitted
 *
 * @code
 * #include <device/input-internal.h>
 * ...
 * device_input_type_e input_device_type = DEVICE_INPUT_TYPE_CUSTOM_KNOB;
 * bool inhibited = false;
 * int defalt_input_device_id = 0;
 * int ret = 0;
 * ...
 * ret = device_input_get_default_device(input_device_type, &defalt_input_device_id);
 * if (ret == DEVICE_ERROR_NONE) {
 *     ret = device_input_get_event_state(defalt_input_device_id, &inhibited);
 *     ...
 * }
 * ...
 * @endcode
 * @see device_input_get_default_device()
 * @see device_input_get_devices()
 * @see device_input_set_event_state()
 */
int device_input_get_event_state(int input_device_id, bool *on);

/**
 * @brief Gets the name of an input device matched with the input device ID.
 * @details Retrieves the name of an input device by input device ID. \n
 *         The input device ID is specified in the @a input_device_id parameter, \n
 *         and the corresponding name of the input device is returned in the @a input_device_name parameter.
 * @since_tizen 7.0
 * @remarks #DEVICE_ERROR_NOT_SUPPORTED is returned, when the trying to control DEVICE_INPUT_TYPE_KEYBOARD device
 *          and following feature is not supported: %http://tizen.org/feature/input.keyboard \n
 *          after using @a input_device_name, it should be freed by caller \n
 * @param[in] input_device_type The type to get input device information \n
 * @param[out] input_device_name The name value to be get from the input devices by @a input_device_type \n
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported in this device
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation not permitted
 *
 * @code
 * #include <device/input-internal.h>
 * ...
 * int input_device_id = 0;
 * char *input_device_name = NULL;
 * int ret = device_input_get_default_device(DEVICE_INPUT_TYEP_CUSTOM_KNOB, &input_device_id);
 * if (ret != DEVICE_ERROR_NONE) {
 *    return -1;
 * }
 * ...
 * ret = device_input_get_device_name(input_device_id, &input_device_name);
 * ...
 * if (input_device_name) {
 *     free(input_device_name);
 * }
 * ...
 * @endcode
 * @see device_input_get_default_device()
 * @see device_input_get_devices()
 */
int device_input_get_device_name(int input_device_id, char **input_device_name);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif	/* __TIZEN_SYSTEM_INPUT_INTERNAL_H__ */
