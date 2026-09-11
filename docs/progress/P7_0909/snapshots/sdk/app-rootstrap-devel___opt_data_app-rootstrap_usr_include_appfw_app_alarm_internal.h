/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_APPFW_ALARM_INTERNAL_H
#define __TIZEN_APPFW_ALARM_INTERNAL_H

#include <tizen.h>
#include <time.h>
#include <app_control.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file app_alarm_internal.h
 */

/**
 * @addtogroup CAPI_ALARM_MODULE
 * @{
 */

/**
 * @brief Sets an alarm to be triggered periodically, starting at a specific time.
 * @details The @a date describes the time of the first occurrence.
 *          To cancel the alarm, call alarm_cancel() with @a alarm_id.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/alarm.set
 * @privilege %http://tizen.org/privilege/appmanager.launch
 * @remarks This function only for in-house applications.
 *          This function only allows service application which has Background Category to set an exact alarm.
 *          If the application is uninstalled after setting an alarm, the alarm is cancelled automatically.
 *          If the operation of @a app_control is not specified, #APP_CONTROL_OPERATION_DEFAULT is used for the launch request.
 *          If the operation of @a app_control is #APP_CONTROL_OPERATION_DEFAULT, the package information is mandatory to explicitly launch the application.
 *          If the appid of @a app_control is not specified, this function is not allowed. In other words, the explicit @a app_control is only allowed.
 *          The @a app_control only supports service application which has Background Category with this function.
 * @param[in] app_control The destination app_control to perform a specific task when the alarm is triggered
 * @param[in] date The first active alarm time
 * @param[in] period The amount of time between subsequent alarms (in seconds).
 * @param[out] alarm_id The alarm ID that uniquely identifies an alarm
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #ALARM_ERROR_NONE Successful
 * @retval #ALARM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #ALARM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ALARM_ERROR_INVALID_TIME Triggered time is invalid
 * @retval #ALARM_ERROR_CONNECTION_FAIL Failed to connect to an alarm server
 * @see alarm_cancel()
 * @see alarm_cancel_all()
 * @see alarm_get_scheduled_date()
 */
int alarm_schedule_service_with_recurrence_seconds(app_control_h app_control,
		struct tm *date, int period, int *alarm_id);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_ALARM_INTERNAL_H */
