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

#ifndef __BOARD_INTERNAL_H__
#define __BOARD_INTERNAL_H__

/**
 * @addtogroup CAPI_SYSTEM_DEVICE_BOARD_INTERNAL_MODULE
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Gets the current device's serial number of the board from the HAL.
 * @details Retrieves the serial number of the board and stores it in the provided @a buffer.
 * @since_tizen 6.5
 * @remarks The user must allocate the max_len size of memory for the @a buffer.
 * @param[out] buffer A pointer to the buffer where the serial number will be stored
 * @param[in] max_len The maximum length of the @a buffer
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
 * #include <device/board-internal.h>
 * ...
 * const int max_len = 256;
 * char buf[256] = {0,};
 * int ret = 0;
 * ...
 * ret = device_board_get_serial_number(buf, max_len);
 * ...
 * @endcode
 */
int device_board_get_serial_number(char *buffer, const int max_len);

/**
 * @brief Gets the current device's boot mode of the board from the HAL.
 * @details Retrieves the boot mode of the board and stores it in the provided @a buffer (e.g. normal).
 * @since_tizen 6.5
 * @remarks The user must allocate the max_len size of memory for the @a buffer.
 * @param[out] buffer A pointer to the buffer where the serial number will be stored
 * @param[in] max_len The maximum length of the @a buffer
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
 * include <device/board-internal.h>
 * ...
 * const int max_len = 256;
 * char buf[256] = {0,};
 * int ret = 0;
 * ...
 * ret = device_board_get_boot_mode(buf, max_len);
 * ...
 * @endcode
 */
int device_board_get_boot_mode(char *buffer, const int max_len);

/**
 * @brief Gets the current device's boot reason of the board from the HAL.
 * @details Retrieves the boot reason of the board and stores it in the provided @a buffer.
 * @since_tizen 6.5
 * @remarks The user must allocate the max_len size of memory for the @a buffer.
 * @param[out] buffer A pointer to the buffer where the serial number will be stored
 * @param[in] max_len The maximum length of the @a buffer
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
 * #include <device/board-internal.h>
 * ...
 * const int max_len = 256;
 * char buf[256] = {0,};
 * int ret = 0;
 * ...
 * ret = device_board_get_boot_reason(buf, max_len);
 * ...
 * @endcode
 */
int device_board_get_boot_reason(char *buffer, const int max_len);

/**
 * @brief Switches the current device's boot partition of the board.
 * @details Switches the boot partition of the board.
 *          If the argument is @c 'a' or @c 'b', the next reboot will boot into the specified partition ('a' or 'b').
 *          If the argument is @c '\0', the function will switch to the opposite partition of the currently booted partition.
 * @since_tizen 6.5
 * @remarks The switch operation will take effect only after the next reboot.
 * @param[in] partition_ab The partition to switch to.\n
 *            It can be either @c 'a', @c 'b', or @c '\0' .\n
 *            If it is @c '\0', the function will switch to the opposite partition of the currently booted partition
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
 * #include <device/board-internal.h>
 * ...
 * int ret = 0;
 * ...
 * ret = device_board_switch_partition('\0');
 * ...
 * @endcode
 */
int device_board_switch_partition(char partition_ab);

/**
 * @brief Sets the current device's a/b partition cloned flag as @c 0 value.
 * @details Sets the a/b partitions cloned flag to @c 0 .\n
 *          That means a/b partitions are not the same in binary level.
 * @since_tizen 6.5
 * @remarks It does not affect a/b partitions, clears only the partition a/b cloned state flag.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
 * #include <device/board-internal.h>
 * ...
 * int ret = 0;
 * ...
 * ret = device_board_clear_partition_ab_cloned();
 * ...
 * @endcode
 * @see device_board_get_partition_ab_cloned
 */
int device_board_clear_partition_ab_cloned(void);

/**
 * @brief Gets the current device's a/b partition cloned flag as an integer value.
 * @details If the a/b partitions are cloned, the function cloned parameter will be @c 1, Otherwise, @c 0.
 * @since_tizen 6.5
 * @remarks Ensure that the provided 'cloned' pointer is valid and has enough memory allocated.
 * @param[out] cloned A pointer to an integer where the cloned state will be stored\n
 *                    If the a/b partitions are cloned, the value will be set to @c 1, Otherwise, it will be set to @c 0
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
 * #include <device/board-internal.h>
 * ...
 * int ret = 0;
 * int cloned = 0;
 * ...
 * ret = device_board_get_partition_ab_cloned(&cloned);
 * ...
 * @endcode
 * @see device_board_clear_partition_ab_cloned
 */
int device_board_get_partition_ab_cloned(int *cloned);

/**
 * @brief Sets the current device's each status of the a/b partitions.
 * @details Sets the status of the a/b partitions.
 *          The status can be one of the following: @c "ok", @c "corrupted", or @c "failed".
 *          If the status is @c "ok", it indicates that there is no problem with the partition.
 *          If the status is @c "corrupted", it indicates that there is some issue with the partition.
 *          If the status is @c "failed", it indicates that the partition cannot be booted successfully.
 * @since_tizen 6.5
 * @remarks Ensure that the input parameters have proper value, especially char array should be terminated by null character.
 * @param[in] partition_ab The partition to set the status for. It can be either @c 'a', @c 'b' or @c '\0'.
 *                         Especially, if it is @c '\0', that means current partition
 * @param[in] status The status to set for the partition. It can be one of the following: @c "ok", @c "corrupted", or @c "failed"
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
 * #include <device/board-internal.h>
 * ...
 * int ret = 0;
 * ...
 * ret = device_board_set_partition_status('\0', "corrupted");
 * ...
 * @endcode
 */
int device_board_set_partition_status(char partition_ab, const char *status);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif
