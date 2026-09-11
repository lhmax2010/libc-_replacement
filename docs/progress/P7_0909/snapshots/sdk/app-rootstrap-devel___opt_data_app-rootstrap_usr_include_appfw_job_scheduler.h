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

#ifndef __TIZEN_APPFW_JOB_SCHEDULER_H__
#define __TIZEN_APPFW_JOB_SCHEDULER_H__

#include <job_error.h>
#include <job_info.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_JOB_SCHEDULER_MODULE
 * @{
 */

/**
 * @brief The job service handle.
 * @since_tizen 4.0
 */
typedef struct job_service_s *job_service_h;

/**
 * @brief Called when the job starts.
 * @since_tizen 4.0
 * @remarks The @a job_info MUST NOT be deallocated by the application.
 *          The @a job_info is managed by the platform and is valid only in the callback.
 * @param[in]   job_info        The job info handle
 * @param[in]   user_data       The user data passed from the callback registration function
 * @see    job_scheduler_service_add()
 * @see    #job_service_callback_s
 */
typedef void (*job_service_start_cb)(job_info_h job_info, void *user_data);

/**
 * @brief Called when the job stops.
 * @since_tizen 4.0
 * @remarks The @a job_info MUST not be deallocated by the application.
 *          The @a job_info is managed by the platform and is valid only in the callback.
 * @param[in]   job_info        The job info handle
 * @param[in]   user_data       The user data passed from the callback registration function
 * @see    job_scheduler_service_add()
 * @see    #job_service_callback_s
 */
typedef void (*job_service_stop_cb)(job_info_h job_info, void *user_data);

/**
 * @brief The structure type containing the set of callback functions for handling jobs.
 * @details It is one of the input parameter of the job_scheduler_service_add() function.
 * @since_tizen 4.0
 * @see    job_scheduler_service_add()
 * @see    job_service_start_cb()
 * @see    job_service_stop_cb()
 */
typedef struct {
	job_service_start_cb start;     /**< This callback function is called at the start of the job. */
	job_service_stop_cb stop;       /**< This callback function is called at the stop of the job. */
} job_service_callback_s;

/**
 * @brief Called to retrieve the job information.
 * @since_tizen 4.0
 * @remarks The @a job_info must not be deallocated by the application.
 * @param[in]   job_info        The job_info handle
 * @param[in]   user_data       The user data passed from the foreach function
 * @return @c true continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @pre    job_scheduler_foreach_job() will invoke this callback.
 * @see    job_scheduler_foreach_job()
 */
typedef bool (*job_scheduler_foreach_job_cb)(job_info_h job_info, void *user_data);

/**
 * @brief Initializes the job scheduler.
 * @since_tizen 4.0
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_IO_ERROR          IO error
 * @retval #JOB_ERROR_OUT_OF_MEMORY     Out of memory
 * @see    job_scheduler_finish()
 */
int job_scheduler_init(void);

/**
 * @brief Finalizes the job scheduler.
 * @since_tizen 4.0
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_NOT_INITIALIZED   Not initialized
 * @see    job_scheduler_init()
 */
int job_scheduler_finish(void);

/**
 * @brief Schedules the job.
 * @since_tizen 4.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/appmanager.launch
 * @param[in]   job_info        The job info handle
 * @param[in]   job_id          The ID of the job
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #JOB_ERROR_NOT_INITIALIZED   Not initialized
 * @retval #JOB_ERROR_IO_ERROR          IO error
 * @retval #JOB_ERROR_ALREADY_EXIST     Already exist
 * @retval #JOB_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #JOB_ERROR_PERMISSION_DENIED Permission denied
 * @pre    job_scheduler_init() MUST be called.
 * @see    job_scheduler_init()
 * @see    job_info_create()
 */
int job_scheduler_schedule(job_info_h job_info, const char *job_id);

/**
 * @brief Cancels the scheduled job.
 * @since_tizen 4.0
 * @param[in]   job_id          The ID of the scheduled job
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #JOB_ERROR_NOT_INITIALIZED   Not initialized
 * @retval #JOB_ERROR_IO_ERROR          IO error
 * @retval #JOB_ERROR_NO_SUCH_JOB       No such job
 * @pre    job_scheduler_init() MUST be called.
 * @see    job_scheduler_init()
 */
int job_scheduler_cancel(const char *job_id);

/**
 * @brief Cancels all scheduled jobs.
 * @since_tizen 4.0
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_NOT_INITIALIZED   Not initialized
 * @retval #JOB_ERROR_IO_ERROR          IO error
 * @pre    job_scheduler_init() MUST be called.
 * @see    job_scheduler_init()
 */
int job_scheduler_cancel_all(void);

/**
 * @brief Notifies that the job is finished.
 * @since_tizen 4.0
 * @remarks If the job has been launched, the system kept awake for a while.
 *          It is highly recommended to notify that the job is done,
 *          the system thus knows it does not need to be kept awake.
 * @param[in]   job_id          The ID of the scheduled job
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #JOB_ERROR_NOT_INITIALIZED   Not initialized
 * @retval #JOB_ERROR_NO_SUCH_JOB       No such job
 * @retval #JOB_ERROR_IO_ERROR          IO error
 * @pre    job_scheduler_init() MUST be called.
 * @see    job_scheduler_init()
 */
int job_scheduler_notify_job_finished(const char *job_id);

/**
 * @brief Retrieves all scheduled jobs.
 * @since_tizen 4.0
 * @param[in]   callback        The iteration callback function
 * @param[in]   user_data       The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #JOB_ERROR_NOT_INITIALIZED   Not initialized
 * @retval #JOB_ERROR_NO_SUCH_JOB       No scheduled jobs
 * @pre    job_scheduler_init() MUST be called.
 * @see    job_scheduler_init()
 */
int job_scheduler_foreach_job(job_scheduler_foreach_job_cb callback, void *user_data);

/**
 * @brief Adds job service handler for receiving the information of the scheduled job.
 * @since_tizen 4.0
 * @remarks The @a job_service handle should be released using job_scheduler_service_remove().
 * @param[in]   job_id          The ID of the job
 * @param[in]   callback        The set of callback functions to handle jobs
 * @param[in]   user_data       The user data to be passed to the callback function
 * @param[out]  job_service     The job service handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #JOB_ERROR_NOT_INITIALIZED   Not initialized
 * @retval #JOB_ERROR_OUT_OF_MEMORY     Out of memory
 * @pre    job_scheduler_init() MUST be called.
 * @see    job_scheduler_init()
 * @see    job_scheduler_service_remove()
 * @see    job_service_start_cb()
 * @see    job_service_stop_cb()
 * @see    job_scheduler_notify_job_finished()
 * @see    #job_service_callback_s
 */
int job_scheduler_service_add(const char *job_id, job_service_callback_s *callback, void *user_data, job_service_h *job_service);

/**
 * @brief Removes registered job service handler.
 * @since_tizen 4.0
 * @param[in]   job_service     The job service handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #JOB_ERROR_NONE              Successful
 * @retval #JOB_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #JOB_ERROR_NOT_INITIALIZED   Not initialized
 * @pre    job_scheduler_init() MUST be called.
 * @see    job_scheduler_init()
 * @see    job_scheduler_service_add()
 */
int job_scheduler_service_remove(job_service_h job_service);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_JOB_SCHEDULER_H__ */
