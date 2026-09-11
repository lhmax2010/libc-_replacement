/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_APPFW_JOB_INFO_H__
#define __TIZEN_APPFW_JOB_INFO_H__

#include <job_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_JOB_SCHEDULER_MODULE
 * @{
 */

/**
 * @brief Enumeration for the job trigger events.
 * @since_tizen 4.0
 */
typedef enum {
	JOB_TRIGGER_EVENT_BATTERY_LEVEL_EMPTY,          /**< The battery level status: Empty */
	JOB_TRIGGER_EVENT_BATTERY_LEVEL_CRITICAL,       /**< The battery level status: Critical */
	JOB_TRIGGER_EVENT_BATTERY_LEVEL_LOW,            /**< The battery level status: Low */
	JOB_TRIGGER_EVENT_BATTERY_LEVEL_HIGH,           /**< The battery level status: High */
	JOB_TRIGGER_EVENT_CHARGER_STATE_CONNECTED,      /**< The charger state: Connected */
	JOB_TRIGGER_EVENT_CHARGER_STATE_DISCONNECTED,   /**< The charger state: Disconnected */
	JOB_TRIGGER_EVENT_DISPLAY_STATE_ON,             /**< The display state: On */
	JOB_TRIGGER_EVENT_DISPLAY_STATE_OFF,            /**< The display state: Off */
	JOB_TRIGGER_EVENT_DISPLAY_STATE_DIM,            /**< The display state: Dim */
	JOB_TRIGGER_EVENT_EARJACK_STATE_CONNECTED,      /**< The earjack state: Connected */
	JOB_TRIGGER_EVENT_EARJACK_STATE_DISCONNECTED,   /**< The earjack state: Disconnected */
	JOB_TRIGGER_EVENT_GPS_STATE_DISABLED,           /**< The GPS state: Disabled */
	JOB_TRIGGER_EVENT_GPS_STATE_SEARCHING,          /**< The GPS state: Searching */
	JOB_TRIGGER_EVENT_GPS_STATE_CONNECTED,          /**< The GPS state: Connected */
	JOB_TRIGGER_EVENT_POWERSAVE_MODE_ENABLED,       /**< The Power-saving mode: Enabled */
	JOB_TRIGGER_EVENT_POWERSAVE_MODE_DISABLED,      /**< The Power-saving mode: Disabled */
	JOB_TRIGGER_EVENT_USB_STATE_CONNECTED,          /**< The USB state: Connected */
	JOB_TRIGGER_EVENT_USB_STATE_DISCONNECTED,       /**< The USB state: Disconnected */
	JOB_TRIGGER_EVENT_WIFI_STATE_DISABLED,          /**< The WiFi state: Disabled @n Privilege: %http://tizen.org/privilege/network.get */
	JOB_TRIGGER_EVENT_WIFI_STATE_DISCONNECTED,      /**< The WiFi state: Disconnected @n Privilege: %http://tizen.org/privilege/network.get */
	JOB_TRIGGER_EVENT_WIFI_STATE_CONNECTED,         /**< The WiFi state: Connected @n Privilege: %http://tizen.org/privilege/network.get */
} job_trigger_event_e;

/**
 * @brief The job info handle.
 * @since_tizen 4.0
 */
typedef struct job_info_s *job_info_h;

/**
 * @brief Creates the job info handle.
 * @since_tizen 4.0
 * @remarks The @a job_info must be released by using job_info_destroy().
 *          The job will be started by the periodic interval or the trigger events.
 * @param[out]  job_info        The job info handle to be newly created on success
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #JOB_ERROR_OUT_OF_MEMORY     Out of memory
 * @see    job_info_destroy()
 * @see    job_info_set_periodic()
 * @see    job_info_add_trigger_event()
 */
int job_info_create(job_info_h *job_info);

/**
 * @brief Destroys the job info handle and releases all its resources.
 * @since_tizen 4.0
 * @param[in]   job_info        The job info handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @see    job_info_create()
 */
int job_info_destroy(job_info_h job_info);

/**
 * @brief Sets the periodic interval of the job.
 * @since_tizen 4.0
 * @remarks The platform does not guarantee the accuracy of the given interval for minimizing the wakeups of the device.
 *          Thus you should not rely on it for timing.
 *          If this function is called after adding the job trigger events by using job_info_add_trigger_event(),
 *          the function returns a negative error value.
 * @param[in]   job_info        The job info handle
 * @param[in]   interval        The periodic interval (in minutes)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @see    job_info_destroy()
 */
int job_info_set_periodic(job_info_h job_info, unsigned int interval);

/**
 * @brief Sets the job to be automatically scheduled after the device reboots.
 * @since_tizen 4.0
 * @remarks If it's a one-time job, setting it to a persistent job returns
 *          a negative error value on scheduling by using job_scheduler_schedule().
 * @param[in]   job_info        The job info handle
 * @param[in]   persistent      @c true, if the job needs to be automatically scheduled
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @see    job_info_set_once()
 * @see    job_scheduler_schedule()
 */
int job_info_set_persistent(job_info_h job_info, bool persistent);

/**
 * @brief Sets the job not to be repeated.
 * @since_tizen 4.0
 * @remarks If it's a persistent job, setting it to a one-time job returns
 *          a negative error value on scheduling by using job_scheduler_schedule().
 * @param[in]   job_info        The job info handle
 * @param[in]   once            @c true, if the job does not need to be repeated
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @see    job_info_set_persistent()
 * @see    job_scheduler_schedule()
 */
int job_info_set_once(job_info_h job_info, bool once);

/**
 * @brief Sets the timeout interval of the requirements.
 * @since_tizen 4.0
 * @remarks If the requirements are not satisfied within the timeout value,
 *          the job does not start.
 * @param[in]   job_info        The job info handle
 * @param[in]   timeout         The timeout interval (ms)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 */
int job_info_set_requirement_timeout(job_info_h job_info, unsigned int timeout);

/**
 * @brief Sets that the battery level must not be low to run the job.
 * @since_tizen 4.0
 * @param[in]   job_info        The job info handle
 * @param[in]   battery_not_low @c true, the battery level must not be low.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #JOB_ERROR_IO_ERROR          IO error
 * @retval #JOB_ERROR_NOT_INITIALIZED   Not initialized
 * @retval #JOB_ERROR_NOT_SUPPORTED     Not supported
 * @pre    job_scheduler_init() must be called.
 * @see    job_scheduler_init()
 */
int job_info_set_requires_battery_not_low(job_info_h job_info, bool battery_not_low);

/**
 * @brief Sets that the battery must be charging to run the job.
 * @since_tizen 4.0
 * @param[in]   job_info        The job info handle
 * @param[in]   charging        @c true, the battery must be charging.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #JOB_ERROR_IO_ERROR          IO error
 * @retval #JOB_ERROR_NOT_INITIALIZED   Not initialized
 * @retval #JOB_ERROR_NOT_SUPPORTED     Not supported
 * @pre    job_scheduler_init() must be called.
 * @see    job_scheduler_init()
 */
int job_info_set_requires_charging(job_info_h job_info, bool charging);

/**
 * @brief Sets that the WiFi must be connected to run the job.
 * @since_tizen 4.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/network.get
 * @param[in]   job_info        The job info handle
 * @param[in]   wifi_connection @c true, the WiFi must be connected.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #JOB_ERROR_IO_ERROR          IO error
 * @retval #JOB_ERROR_NOT_INITIALIZED   Not initialized
 * @retval #JOB_ERROR_NOT_SUPPORTED     Not supported
 * @retval #JOB_ERROR_PERMISSION_DENIED Permission denied
 * @pre    job_scheduler_init() must be called.
 * @see    job_scheduler_init()
 */
int job_info_set_requires_wifi_connection(job_info_h job_info, bool wifi_connection);

/**
 * @brief Adds the trigger event to the job info handle.
 * @since_tizen 4.0
 * @remarks The periodic interval of the job info handle must be 0.
 *          If the interval of the job info handle is not 0, this function returns a negative error value.
 *          Regarding the @a event, the application may require the following privilege:
 *              %http://tizen.org/privilege/network.get @n
 *          If the application does not have the necessary privilege, this function returns #JOB_ERROR_PERMISSION_DENIED.
 *          See #job_trigger_event_e to find the corresponding privilege of each event.
 * @param[in]   job_info        The job info handle
 * @param[in]   event           The job trigger event
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #JOB_ERROR_IO_ERROR          IO error
 * @retval #JOB_ERROR_NOT_INITIALIZED   Not initialized
 * @retval #JOB_ERROR_NOT_SUPPORTED     Not supported
 * @retval #JOB_ERROR_ALREADY_EXIST     Already exist
 * @retval #JOB_ERROR_PERMISSION_DENIED Permission denied
 * @pre    job_scheduler_init() must be called.
 * @see    job_scheduler_init()
 * @see    job_info_remove_trigger_event()
 * @see    job_trigger_event_e
 */
int job_info_add_trigger_event(job_info_h job_info, job_trigger_event_e event);

/**
 * @brief Removes the trigger event from the job info handle.
 * @since_tizen 4.0
 * @remarks The periodic interval of the job info handle must be 0.
 *          If the interval of the job info handle is not 0, this function returns a negative error value.
 * @param[in]   job_info        The job info handle
 * @param[in]   event           The job trigger event
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @see    job_info_add_trigger_event()
 * @see    job_trigger_event_e
 */
int job_info_remove_trigger_event(job_info_h job_info, job_trigger_event_e event);

/**
 * @brief Gets the ID of the job.
 * @since_tizen 4.0
 * @remarks The @a job_info must be scheduled by using job_scheduler_schedule().
 *          If the job_info is not scheduled, this function returns a negative error value.
 *          The @a job_id must be released by using free().
 * @param[in]   job_info        The job info handle
 * @param[out]  job_id          The ID of the job
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #JOB_ERROR_OUT_OF_MEMORY     Out of memory
 * @see    job_scheduler_schedule()
 */
int job_info_get_job_id(job_info_h job_info, char **job_id);

/**
 * @brief Clones the given job_info handle.
 * @since_tizen 4.0
 * @remarks The newly created job_info handle must be released by using job_info_destroy().
 * @param[in]   job_info        The job info handle
 * @param[out]  clone           If successful, a newly created job info handle will be returned
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #JOB_ERROR_OUT_OF_MEMORY     Out of memory
 * @see    job_info_destroy()
 */
int job_info_clone(job_info_h job_info, job_info_h *clone);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_JOB_INFO_H__ */
