/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_SYSTEM_IR_H__
#define __TIZEN_SYSTEM_IR_H__


#include <stdbool.h>
#include "device-error.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_IR_MODULE
 * @{
 */


/**
 * @brief Gets the information the IR module is available on the device.
 * @details Gets the boolean value whether the IR module is available on the device. \n
 *          If the IR module is available, the function returns @c true in the @a available parameter. \n
 *          Otherwise, it returns @c false.
 * @since_tizen 3.0
 * @remarks Ensure that the provided @a available pointer is valid and has enough memory allocated.
 * @privlevel public
 * @privilege %http://tizen.org/privilege/use_ir
 * @param[out] available The information whether IR is available
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <stdio.h>
 * #include <device/ir.h>
 * ...
 * bool ir_available;
 * int ret = device_ir_is_available(&ir_available);
 * if (ret != DEVICE_ERROR_NONE) {
 *     return -1;
 * }
 * ...
 * if (ir_available) {
 *     printf("IR module is available.\n");
 * } else {
 *     printf("IR module is not available.\n");
 * }
 * ...
 * @endcode
 */
int device_ir_is_available(bool *available);


/**
 * @brief Transmits IR command with the specified carrier frequency and pattern.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/use_ir
 * @param[in] carrier_frequency Carrier frequency to transmit IR command (Hertz)
 * @param[in] pattern Integer array of IR command
 * @param[in] size Size of IR command pattern
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <stdio.h>
 * #include <device/ir.h>
 * ...
 * int carrier_frequency = 38000; // Carrier frequency in Hertz
 * int pattern[] = {900, 450, 450, 450, 450, 450, 450, 450, 450, 450}; // Example IR command pattern
 * int size = sizeof(pattern) / sizeof(pattern[0]); // Size of the IR command pattern
 * int ret = device_ir_transmit(carrier_frequency, pattern, size);
 * if (ret != DEVICE_ERROR_NONE) {
 *     return -1;
 * }
 * printf("IR command transmitted successfully.\n");
 * ...
 * @endcode
 * @see device_ir_is_available()
 */
int device_ir_transmit(int carrier_frequency, int *pattern, int size);


/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif  /* __TIZEN_SYSTEM_IR_H__ */
