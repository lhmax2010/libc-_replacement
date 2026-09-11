/*
 * Copyright (c) 2011 - 2016 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_APPFW_ALARM_EXTENSION_H
#define __TIZEN_APPFW_ALARM_EXTENSION_H

#include <tizen.h>
#include <time.h>
#include <app_control.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file app_alarm_extension.h
 */

/**
 * @addtogroup CAPI_ALARM_MODULE
 * @{
 */

/**
 * @brief Sets an exact alarm to be triggered after a specific time.
 * @details The alarm will go off @a delay seconds later.
 *          To cancel the alarm, call alarm_cancel() with @a alarm_id.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/alarm.set
 * @remarks This function only allows service application which has Background Category to set an exact alarm.
 *          If the application is uninstalled after setting an alarm, the alarm is cancelled automatically.
 *          If the operation of @a app_control is not specified, #APP_CONTROL_OPERATION_DEFAULT is used r the launch request.
 *          If the operation of @a app_control is #APP_CONTROL_OPERATION_DEFAULT, the package information is mandatory to explicitly launch the application.
 *          If the appid of @a app_control is not specified, this function is not allowed. In other words, the explicit @a app_control is only allowed.
 *          The @a app_control only supports service application which has Background Category with this function.
 *
 * @param[in] app_control The destination app_control to perform a specific task when the alarm is triggered
 * @param[in] delay The amount of time before the execution (in seconds)
 * @param[out] alarm_id	The alarm ID that uniquely identifies an alarm
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #ALARM_ERROR_NONE Successful
 * @retval #ALARM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #ALARM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ALARM_ERROR_INVALID_TIME Triggered time is invalid
 * @retval #ALARM_ERROR_CONNECTION_FAIL Failed to connect to an alarm server
 * @see alarm_cancel()
 * @see alarm_cancel_all()
 * @see alarm_get_scheduled_date()
 */
int alarm_schedule_service_once_after_delay(app_control_h app_control, int delay, int *alarm_id);

/**
 * @brief Sets an alarm to be triggered at a specific time.
 * @details The @a date describes the time of the first occurrence.
 *          To cancel the alarm, call alarm_cancel() with @a alarm_id.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/alarm.set
 * @remarks This function only allows service application which has Background Category to set an exact alarm.
 *          If application is uninstalled after setting an alarm, the alarm is cancelled automatically.
 *          If the operation of @a app_control is not specified, #APP_CONTROL_OPERATION_DEFAULT is used for the launch request.
 *          If the operation of @a app_control is #APP_CONTROL_OPERATION_DEFAULT, the package information is mandatory to explicitly launch the application.
 *          If the appid of @a app_control is not specified, this function is not allowed. In other words, the explicit @a app_control is only allowed.
 *          The @a app_control only supports service application which has Background Category with this function.
 *
 * @param[in] app_control The destination app_control to perform specific work when the alarm is triggered
 * @param[in] date The first active alarm time
 * @param[out] alarm_id The alarm ID that uniquely identifies an alarm
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #ALARM_ERROR_NONE   Successful
 * @retval #ALARM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #ALARM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ALARM_ERROR_INVALID_DATE Triggered date is invalid
 * @retval #ALARM_ERROR_CONNECTION_FAIL Failed to connect to an alarm server
 * @see alarm_cancel()
 * @see alarm_cancel_all()
 * @see alarm_get_scheduled_date()
 */
int alarm_schedule_service_once_at_date(app_control_h app_control, struct tm *date, int *alarm_id);

/**
 * @brief Changes the system time which tranferred by other module.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/systemsettings.admin
 * @param[in] new_time epoch time to be set
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #ALARM_ERROR_NONE Successful
 * @retval #ALARM_ERROR_CONNECTION_FAIL Failed to connect to an alarm server
 */
int alarm_set_systime(int new_time);

/**
 * @brief Changes the system time and compensates the time using propagation delay.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/systemsettings.admin
 * @param[in] new_time system time to be set (seconds, nanoseconds)
 * @param[in] req_time time to request to change the system time (seconds, nanoseconds)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #ALARM_ERROR_NONE Successful
 * @retval #ALARM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ALARM_ERROR_CONNECTION_FAIL Failed to connect to an alarm server
 */
int alarm_set_systime_with_propagation_delay(struct timespec new_time, struct timespec req_time);

/**
 * @brief Changes the timezone which tranferred by other module.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/systemsettings.admin
 * @param[in] tzpath_str the path to timezone definition file
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #ALARM_ERROR_NONE Successful
 * @retval #ALARM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ALARM_ERROR_CONNECTION_FAIL Failed to connect to an alarm server
 */
int alarm_set_timezone(char *tzpath_str);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_ALARM_EXTENSION_H */
