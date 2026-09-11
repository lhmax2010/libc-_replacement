/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_BASE_TIZEN_CORE_H__
#define __TIZEN_BASE_TIZEN_CORE_H__

#include <stdbool.h>
#include <stdint.h>
#include <tizen_error.h>
#include <poll.h>

#include <tizen_core_channel.h>
#include <tizen_core_event.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file tizen_core.h
 */

/**
 * @addtogroup CAPI_TIZEN_CORE_MODULE
 * @{
 */

/**
 * @brief Enumeration for the tizen core result.
 * @since_tizen 9.0
 */
typedef enum {
  TIZEN_CORE_ERROR_NONE = TIZEN_ERROR_NONE,  /**< Successful */
  TIZEN_CORE_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,  /**< Invalid parameter */
  TIZEN_CORE_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,  /**< Out of memory */
  TIZEN_CORE_ERROR_INVALID_CONTEXT = TIZEN_ERROR_APPLICATION | 0x01,  /**< Invalid context */
} tizen_core_error_e;

/**
 * @brief Enumeration for the priority of tizen core source.
 * @since_tizen 9.0
 */
typedef enum {
  TIZEN_CORE_PRIORITY_HIGH = -100,  /**< High priority */
  TIZEN_CORE_PRIORITY_DEFAULT = 0,  /**< Default priority */
  TIZEN_CORE_PRIORITY_HIGH_IDLE = 100,  /**< High idle priority */
  TIZEN_CORE_PRIORITY_DEFAULT_IDLE = 200,  /**< Default idle priority */
  TIZEN_CORE_PRIORITY_LOW = 300,  /**< Low priority */
} tizen_core_priority_e;

/**
 * @brief Enumeration for the event of tizen core poll fd.
 * @since_tizen 9.0
 * @see tizen_core_poll_fd_set_events()
 * @see tizen_core_poll_fd_get_events()
 * @see tizen_core_poll_fd_set_returned_events()
 * @see tizen_core_poll_fd_get_returned_events()
 */
typedef enum {
  TIZEN_CORE_POLL_EVENT_IN = POLLIN,  /**< There is data to read */
  TIZEN_CORE_POLL_EVENT_PRI = POLLPRI,  /**< There is some exceptional condition on the file descriptor */
  TIZEN_CORE_POLL_EVENT_OUT = POLLOUT,  /**< Writing is now possible */
  TIZEN_CORE_POLL_EVENT_ERR = POLLERR,  /**< Error condition */
  TIZEN_CORE_POLL_EVENT_HUP = POLLHUP,  /**< Hang up */
  TIZEN_CORE_POLL_EVENT_NVAL = POLLNVAL,  /**< Invalid request: fd not open */
} tizen_core_poll_event_e;

/**
 * @brief The tizen core handle.
 * @since_tizen 9.0
 */
typedef void *tizen_core_h;


/**
 * @brief The tizen core task handle.
 * @since_tizen 9.0
 */
typedef void *tizen_core_task_h;

/**
 * @brief The tizen core source handle.
 * @since_tizen 9.0
 */
typedef void *tizen_core_source_h;

/**
 * @brief The tizen core poll fd handle.
 * @since_tizen 9.0
 */
typedef void *tizen_core_poll_fd_h;

/**
 * @brief Called when the channel object is received.
 * @since_tizen 9.0
 * @remarks The @a object must not be deallocated by the application.
 *
 * @param[in] object The tizen core channel object handle
 * @param[in] user_data The user data passed from the registration function
 * @see tizen_core_add_channel()
 */
typedef void (*tizen_core_channel_receive_cb)(
    tizen_core_channel_object_h object, void *user_data);

/**
 * @brief Called when the tizen core idle or timer event is emitted.
 * @since_tizen 9.0
 *
 * @param[in] user_data The user data passed from the callback registration function
 * @return @c true to repeat calling the callback function,
 *         otherwise @c false to remove the added source.
 * @see tizen_core_add_idle_job()
 * @see tizen_core_add_timer()
 */
typedef bool (*tizen_core_task_cb)(void *user_data);

/**
 * @brief Called before all file descriptors are polled.
 * @details The @a source is the same object for which the callback was set.
 * @details The function returns a timeout value to ensure that the poll()
 *          call doesn't block too long and miss the next timeout.
 *          The timeout value is -1 to indicate that it doesn't mind how long
 *          the poll() call blocks.
 * @since_tizen 9.0
 * @remarks The @a source should be released using tizen_core_source_destroy() when no longer needed.
 *          The @a timeout should not be released. The @a timeout can be used only in the callback.
 *
 * @param[in] source The tizen core source handle
 * @param[out] timeout The timeout
 * @param[in] user_data The user data passed from the callback registration function
 * @return @c true to indicate that the source is always ready to be processed,
 *         otherwise @c false to wait until poll() has been called before it knows whether any events need to be processed.
 * @see tizen_core_source_set_prepare_cb()
 */
typedef bool (*tizen_core_source_prepare_cb)(tizen_core_source_h source,
                                             int *timeout, void *user_data);

/**
 * @brief Called when checks whether the source is ready to be processed or not.
 * @details The @a source is the same object for which the callback was set.
 * @details The function should return 'true' if it's ready to be dispatched.
 * @since_tizen 9.0
 * @remarks The @a source should be released using tizen_core_source_destroy() when no longer needed.
 *
 * @param[in] source The tizen core source handle
 * @param[in] user_data The user data passed from the callback registration function
 * @return @c true to indicate that the source is always ready to be processed,
 *         otherwise @c false to indicate that the source is not ready to be processed.
 * @see tizen_core_source_set_check_cb()
 */
typedef bool (*tizen_core_source_check_cb)(tizen_core_source_h source,
                                           void *user_data);

/**
 * @brief Called when dispatches events.
 * @details The @a source is the same object for which the callback was set.
 * @since_tizen 9.0
 * @remarks The @a source should be released using tizen_core_source_destroy() when no longer needed.
 *
 * @param[in] source The tizen core source handle
 * @param[in] user_data The user data passed from the callback registration function
 * @return @c true to indicate that the process is ready to dispatch the event.
 * @see tizen_core_source_set_dispatch_cb()
 */
typedef bool (*tizen_core_source_dispatch_cb)(tizen_core_source_h source,
                                              void *user_data);

/**
 * @brief Called when the source is finalized.
 * @details The @a source is the same object for which the callback was set.
 * @since_tizen 9.0
 * @remarks The @a source should not be released. The @a source is managed by the platform.
 * @remarks The @a source is managed by the platform and will be released when the callback returns.
 *
 * @param[in] source The tizen core source handle
 * @param[in] user_data The user data passed from the callback registration function
 * @see tizen_core_source_set_finalize_cb()
 */
typedef void (*tizen_core_source_finalize_cb)(tizen_core_source_h source,
                                              void *user_data);

/**
 * @brief Initializes the tizen core.
 * @since_tizen 9.0
 *
 * @see tizen_core_shutdown()
 * @see tizen_core_ready()
 */
void tizen_core_init(void);

/**
 * @brief Shutdowns the tizen core.
 * @since_tizen 9.0
 *
 * @see tizen_core_init()
 * @see tizen_core_ready()
 */
void tizen_core_shutdown(void);

/**
 * @brief Checks whether the tizen core is ready or not.
 * @since_tizen 9.0
 *
 * @return @c true if the tizen core is ready, otherwise @c false.
 *
 * @see tizen_core_init()
 * @see tizen_core_shutdown()
 */
bool tizen_core_ready(void);

/**
 * @brief Creates the tizen core task handle.
 * @details If the @a name is "main", the @a use_thread should be set to 'false'.
 *          Otherwise, it returns #TIZEN_CORE_ERROR_INVALID_PARAMETER.
 * @since_tizen 9.0
 * @remarks The @a task should be released using tizen_core_task_destroy().
 *
 * @param[in] name The name of the task
 * @param[in] use_thread true, if the task should be run in a separate thread, otherwise @c false
 * @param[out] task The tizen core task handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #TIZEN_CORE_ERROR_INVALID_CONTEXT Invalid context
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_task_h task = NULL;
  int ret;

  tizen_core_init();
  ret = tizen_core_task_create("main", false, &task);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    tizen_core_shutdown();
    return -1;
  }
}
 * @endcode
 *
 * @see tizen_core_task_destroy()
 */
int tizen_core_task_create(const char *name, bool use_thread,
                           tizen_core_task_h *task);

/**
 * @brief Destroys the tizen core task handle.
 * @since_tizen 9.0
 *
 * @param[in] task The tizen core task handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_task_h task = NULL;
  int ret;

  tizen_core_init();
  ret = tizen_core_task_create("main", false, &task);
  if (ret != TIZEN_CORE_ERROR_NONE)
    return ret;

  ret = tizen_core_task_destroy(task);
  if (ret != TIZEN_CORE_ERROR_NONE)
    return ret;
}
 * @endcode
 *
 * @see tizen_core_task_create()
 */
int tizen_core_task_destroy(tizen_core_task_h task);

/**
 * @brief Runs the main loop of the tizen core task.
 * @since_tizen 9.0
 *
 * @param[in] task The tizen core task handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_INVALID_CONTEXT Invalid context
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_task_h task = NULL;
  int ret;

  tizen_core_init();
  ret = tizen_core_task_create("worker", true, &task);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    tizen_core_shutdown();
    return ret;
  }

  ret = tizen_core_task_run(task);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to run task");
}
 * @endcode
 *
 * @see tizen_core_task_quit()
 */
int tizen_core_task_run(tizen_core_task_h task);

/**
 * @brief Checks whether the tizen core task is running or not.
 * @since_tizen 9.0
 *
 * @param[in] task The tizen core task handle
 * @param[out] running @c true if the core task is running, \n
 *                     otherwise @c false if not running
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_task_h task = NULL;
  bool running = false;
  int ret;

  tizen_core_init();
  ret = tizen_core_task_create("worker", true, &task);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    tizen_core_shutdown();
    return ret;
  }

  ret = tizen_core_task_run(task);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to run task");
    tizen_core_task_destroy(task);
    tizen_core_shutdown();
    return ret;
  }

  ret = tizen_core_task_is_running(task, &running);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to check whether the task is running or not");
  else
    dlog_print(DLOG_INFO, LOG_TAG, "worker task is %s", running ? "running" : "not running");
}
 * @endcode
 *
 * @see tizen_core_task_run()
 * @see tizen_core_task_quit()
 */
int tizen_core_task_is_running(tizen_core_task_h task, bool *running);

/**
 * @brief Exits the main loop of the tizen core task.
 * @since_tizen 9.0
 *
 * @param[in] task The tizen core task handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_task_h task = NULL;
  int ret;

  tizen_core_init();
  tizen_core_task_create("worker", true, &task);
  tizen_core_task_run(task);
  ret = tizen_core_task_quit(task);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to exit the main loop");
}
 * @endcode
 *
 * @see tizen_core_task_run()
 */
int tizen_core_task_quit(tizen_core_task_h task);

/**
 * @brief Gets the tizen core from the tizen core task handle.
 * @since_tizen 9.0
 * @remarks The @a core should not be released.
 * @remarks The @a core is available until the @a task is released.
 *
 * @param[in] task The tizen core task handle
 * @param[out] core The tizen core handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_task_h task = NULL;
  tizen_core_h core = NULL;
  int ret;

  tizen_core_init();
  tizen_core_task_create("worker", true, &task);
  ret = tizen_core_task_get_tizen_core(task, &core);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to get tizen core");
}
 * @endcode
 */
int tizen_core_task_get_tizen_core(tizen_core_task_h task, tizen_core_h *core);

/**
 * @brief Finds the tizen core from the name.
 * @since_tizen 9.0
 * @remarks The @a core should not be released.
 * @remarks The @a core is available until #tizen_core_task_h of the @a core is released.
 *
 * @param[in] name The tizen core task name
 * @param[out] core The tizen core handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_INVALID_CONTEXT Invalid context
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_h core = NULL;
  int ret;

  ret = tizen_core_find("worker", &core);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to find tizen core");
}
 * @endcode
 */
int tizen_core_find(const char *name, tizen_core_h *core);

/**
 * @brief Finds the tizen core from this thread.
 * @since_tizen 9.0
 * @remarks The @a core should not be released.
 * @remarks The @a core is available until #tizen_core_task_h of the @a core is released.
 *
 * @param[out] core The tizen core handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_INVALID_CONTEXT Invalid context
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_h core = NULL;
  int ret;

  ret = tizen_core_find_from_this_thread(&core);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to find tizen core from this thread");
}
 * @endcode
 */
int tizen_core_find_from_this_thread(tizen_core_h *core);

/**
 * @brief Adds an idle job to the tizen core.
 * @details The callback function will be called whenever there are
 *          no higher priority events pending to the tizen core.
 *          The given callback function is called repeatedly until it returns false,
 *          at which point the idler @a source is automatically destroyed and
 *          the callback function will not be called again.
 * @since_tizen 9.0
 * @remarks The @a source should be released using tizen_core_remove_source().
 *
 * @param[in] core The tizen core handle
 * @param[in] callback The callback function to be invoked when the idle job is emitted
 * @param[in] user_data The user data to be passed to the callback function
 * @param[out] source The tizen core source handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @code
#include <tizen_core.h>
static bool idle_cb(void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "idler is invoked");
  return true;
}

static tizen_core_source_h add_idle_job(void)
{
  tizen_core_source_h source = NULL;
  tizen_core_h core = NULL;
  int ret;

  tizen_core_find("main", &core);
  ret = tizen_core_add_idle_job(core, idle_cb, NULL, &source);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to add idle job");
    return NULL;
  }

  return source;
}
 * @endcode
 *
 * @see tizen_core_remove_source()
 * @see tizen_core_task_cb()
 */
int tizen_core_add_idle_job(tizen_core_h core, tizen_core_task_cb callback,
                            void *user_data, tizen_core_source_h *source);

/**
 * @brief Adds a timer to the tizen core.
 * @details The callback function will be called at regular intervals.
 *          The given callback function is called repeatedly until it returns false,
 *          at which point the timer @a source is automatically destroyed and
 *          the callback function will not be called again.
 * @since_tizen 9.0
 * @remarks The @a source should be released using tizen_core_remove_source().
 *
 * @param[in] core The tizen core handle
 * @param[in] interval The interval of the timer in milliseconds
 * @param[in] callback The callback function to be invoked when the timer is expired
 * @param[in] user_data The user data to be passed to the callback function
 * @param[out] source The tizen core source handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @code
#include <tizen_core.h>

static bool timeout_cb(void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "timeout cb is invoked");
  return true;
}

static tizen_core_source_h add_timer(unsigned int interval)
{
  tizen_core_source_h source = NULL;
  tizen_core_h core = NULL;
  int ret;

  tizen_core_find("main", &core);
  ret = tizen_core_add_timer(core, interval, timeout_cb, NULL, &source);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to add timer");
    return NULL;
  }

  return source;
}
 * @endcode
 *
 * @see tizen_core_remove_source()
 * @see tizen_core_task_cb()
 */
int tizen_core_add_timer(tizen_core_h core, unsigned int interval,
                         tizen_core_task_cb callback, void *user_data,
                         tizen_core_source_h *source);

/**
 * @brief Adds a channel receiver to the tizen core.
 * @details The callback function will be called when the channel event is received.
 * @since_tizen 9.0
 * @remarks The @a source should be released using tizen_core_remove_source().
 *
 * @param[in] core The tizen core handle
 * @param[in] receiver The channel receiver handle
 * @param[in] callback The callback function to be invoked when the channel event is received
 * @param[in] user_data The user data to be passed to the callback function
 * @param[out] source The tizen core source handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @code
#include <tizen_core.h>
static void channel_receiver_cb(tizen_core_channel_object_h object, void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "channel object is received");
}

static tizen_core_source_h add_channel_event(tizen_core_channel_receiver_h receiver)
{
  tizen_core_source_h source = NULL;
  tizen_core_h core = NULL;
  int ret;

  tizen_core_find("main", &core);
  ret = tizen_core_add_channel(core, receiver, channel_receiver_cb, NULL, &source);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to add channel");
    return NULL;
  }

  return source;
}
 * @endcode
 *
 * @see tizen_core_remove_source()
 */
int tizen_core_add_channel(tizen_core_h core,
                           tizen_core_channel_receiver_h receiver,
                           tizen_core_channel_receive_cb callback,
                           void *user_data, tizen_core_source_h *source);

/**
 * @brief Adds an event to the tizen core.
 * @details The event handler added to the event handle is called
 *          using the corresponding core loop when the event emit occurs.
 *          When this function is called the ownership of the @a event is moved
 *          to the tizen core. The @a event will be released when the @a source
 *          is removed using tizen_core_remove_source().
 * @since_tizen 9.0
 * @remarks The @a source should be released using tizen_core_remove_source().
 * @remarks The @a event MUST not be released using tizen_core_event_destroy()
 *          after adding to the tizen core using this function.
 *
 * @param[in] core The tizen core handle
 * @param[in] event The tizen core event handle
 * @param[out] source The tizen core source handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @code
#include <tizen_core.h>
static tizen_core_source_h add_event_source(tizen_core_event_h event)
{
  tizen_core_source_h source = NULL;
  tizen_core_h core = NULL;
  int ret;

  tizen_core_find("main", &core);
  ret = tizen_core_add_event(core, event, &source);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to add event");
    return NULL;
  }

  return source;
}
 * @endcode
 *
 * @see tizen_core_remove_source()
 */
int tizen_core_add_event(tizen_core_h core, tizen_core_event_h event,
                         tizen_core_source_h *source);

/**
 * @brief Emits the event to the tizen core.
 * @details The event is emitted to the event handler registered in the corresponding tizen_core.
 * @since_tizen 9.0
 *
 * @param[in] core The tizen core handle
 * @param[in] object The tizen core event object handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
static int emit_event(tizen_core_event_object_h object)
{
  tizen_core_h core = NULL;
  int ret;

  tizen_core_find("main", &core);
  ret = tizen_core_emit_event(core, object);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to emit event");

  return ret;
}
 * @endcode
 *
 * @see tizen_core_add_event()
 */
int tizen_core_emit_event(tizen_core_h core, tizen_core_event_object_h object);

/**
 * @brief Adds a source to the tizen core.
 * @since_tizen 9.0
 * @remarks The @a source should be removed using tizen_core_remove_source().
 *
 * @param[in] core The tizen core handle
 * @param[in] source The tizen core source handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_source_h source = NULL;
  tizen_core_h core = NULL;
  int ret;

  tizen_core_source_create(&source);
  tizen_core_find("main", &core);
  ret = tizen_core_add_source(core, source);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to add source");
}
 * @endcode
 *
 * @see tizen_core_remove_source()
 */
int tizen_core_add_source(tizen_core_h core, tizen_core_source_h source);

/**
 * @brief Removes the source from the tizen core.
 * @since_tizen 9.0
 * @remarks If the source is not added to the tizen core, this function returns
 *          #TIZEN_CORE_ERROR_INVALID_CONTEXT.
 *
 * @param[in] core The tizen core handle
 * @param[in] source The tizen core source handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_INVALID_CONTEXT Invalid context
 *
 * @code
#include <tizen_core.h>
static void remove_source(tizen_core_source_h source)
{
  tizen_core_h core = NULL;
  int ret;

  tizen_core_find("main", &core);
  ret = tizen_core_remove_source(core, source);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to remove source");
  else
    dlog_print(DLOG_INFO, LOG_TAG, "source is removed");
}
 * @endcode
 *
 * @see tizen_core_add_source()
 */
int tizen_core_remove_source(tizen_core_h core, tizen_core_source_h source);

/**
 * @brief Creates a tizen core source handle.
 * @since_tizen 9.0
 * @remarks The @a source should be released using tizen_core_source_destroy().
 *
 * @param[out] source The tizen core source handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #TIZEN_CORE_ERROR_INVALID_CONTEXT Invalid context
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_source_h source = NULL;
  int ret;

  ret = tizen_core_source_create(&source);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create source");
    return ret;
  }
}
 * @endcode
 *
 * @see tizen_core_add_source()
 * @see tizen_core_remove_source()
 * @see tizen_core_source_destroy()
 */
int tizen_core_source_create(tizen_core_source_h *source);

/**
 * @brief Destroys the tizen core source handle.
 * @since_tizen 9.0
 *
 * @param[in] source The tizen core source handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_source_h source = NULL;
  int ret;

  ret = tizen_core_source_create(&source);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create source");
    return ret;
  }

  ret = tizen_core_source_destroy(source);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to destroy source");
}
 * @endcode
 * @see tizen_core_source_create()
 * @see tizen_core_remove_source()
 */
int tizen_core_source_destroy(tizen_core_source_h source);

/**
 * @brief Waits for some events on the file descriptor.
 * @details The tizen core source must be created using the tizen_core_source_create().
 * @since_tizen 9.0
 *
 * @param[in] source The tizen core source handle
 * @param[in] poll_fd The poll fd handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_INVALID_CONTEXT Invalid context
 *
 * @code
#include <tizen_core.h>
static tizen_core_poll_fd_h add_poll_fd_to_source(tizen_core_source_h source, int fd)
{
  tizen_core_poll_fd_h poll_fd = NULL;
  int ret;

  tizen_core_poll_fd_create(&poll_fd);
  tizen_core_poll_fd_set_fd(poll_fd, fd);
  tizen_core_poll_fd_set_events(poll_fd, TIZEN_CORE_POLL_EVENT_IN);
  ret = tizen_core_source_add_poll(source, poll_fd);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to add poll fd");
    tizen_core_poll_fd_destroy(poll_fd);
    return NULL;
  }

  return poll_fd;
}
 * @endcode
 *
 * @see tizen_core_source_create()
 * @see tizen_core_source_set_prepare_cb()
 * @see tizen_core_source_set_check_cb()
 * @see tizen_core_source_set_dispatch_cb()
 * @see tizen_core_source_set_finalize_cb()
 * @see tizen_core_source_remove_poll()
 */
int tizen_core_source_add_poll(tizen_core_source_h source,
                               tizen_core_poll_fd_h poll_fd);

/**
 * @brief Removes to wait for some events on the file descriptor.
 * @since_tizen 9.0
 *
 * @param[in] source The tizen core source handle
 * @param[in] poll_fd The poll fd handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_INVALID_CONTEXT Invalid context
 *
 * @code
#include <tizen_core.h>
static void remove_poll_from_source(tizen_core_source_h source, tizen_core_poll_fd_h poll_fd)
{
  int ret;

  ret = tizen_core_source_remove_poll(source, poll_fd);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to remove poll fd");
  else
    dlog_print(DLOG_INFO, LOG_TAG, "Poll fd is removed");
}
 * @endcode
 *
 * @see tizen_core_source_create()
 * @see tizen_core_source_add_poll()
 */
int tizen_core_source_remove_poll(tizen_core_source_h source,
                                  tizen_core_poll_fd_h poll_fd);

/**
 * @brief Sets the prepare callback function to the tizen core source.
 * @details This function is not mandatory.
 *          If you want to set a timeout value to ensure that the poll(),
 *          the @a callback function should be set.
 * @since_tizen 9.0
 *
 * @param[in] source The tizen core source handle
 * @param[in] callback The callback function to be invoked when the source is ready to be processed
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
static bool source_prepare_cb(tizen_core_source_h source, int *timeout, void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "prepare callback is invoked");
  return true;
}

static bool source_check_cb(tizen_core_source_h source, void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "check callback is invoked");
  return true;
}

static bool source_dispatch_cb(tizen_core_source_h source, void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "dispatch callback is invoked");
  return true;
}

static tizen_core_source_h create_source(void)
{
  tizen_core_source_h source = NULL;
  int ret;

  ret = tizen_core_source_create(&source);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create source");
    return NULL;
  }

  ret = tizen_core_source_set_prepare_cb(source, source_prepare_cb, NULL);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to set prepare callback");
    tizen_core_source_destroy(source);
    return NULL;
  }

  tizen_core_source_set_check_cb(source, source_check_cb, NULL);
  tizen_core_source_set_dispatch_cb(source, source_dispatch_cb, NULL);

  return source;
}
 * @endcode
 *
 * @see tizen_core_source_create()
 * @see tizen_core_source_prepare_cb()
 * @see tizen_core_source_add_poll()
 */
int tizen_core_source_set_prepare_cb(
    tizen_core_source_h source, tizen_core_source_prepare_cb callback,
    void *user_data);

/**
 * @brief Sets the check callback function to the tizen core source.
 * @details This function is not mandatory.
 *          If the @a callback function is not set, the tizen core source is processed.
 * @since_tizen 9.0
 *
 * @param[in] source The tizen core source handle
 * @param[in] callback The callback function to be invoked when the source is ready to be processed
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
static bool source_prepare_cb(tizen_core_source_h source, int *timeout, void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "prepare callback is invoked");
  return true;
}

static bool source_check_cb(tizen_core_source_h source, void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "check callback is invoked");
  return true;
}

static bool source_dispatch_cb(tizen_core_source_h source, void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "dispatch callback is invoked");
  return true;
}

static tizen_core_source_h create_source(void)
{
  tizen_core_source_h source = NULL;
  int ret;

  ret = tizen_core_source_create(&source);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create source");
    return NULL;
  }

  tizen_core_source_set_prepare_cb(source, source_prepare_cb, NULL);
  tizen_core_source_set_dispatch_cb(source, source_dispatch_cb, NULL);

  ret = tizen_core_source_set_check_cb(source, source_check_cb, NULL);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to set check callback");
    tizen_core_source_destroy(source);
    return NULL;
  }

  return source;
}
 * @endcode
 *
 * @see tizen_core_source_create()
 * @see tizen_core_source_check_cb()
 */
int tizen_core_source_set_check_cb(tizen_core_source_h source,
                                   tizen_core_source_check_cb callback,
                                   void *user_data);

/**
 * @brief Sets the dispatch callback function to the tizen core source.
 * @details This function is mandatory. Before adding the @a source handle to
 *          the #tizen_core_h using tizen_core_add_source(),
 *          the @a callback function should be set.
 * @since_tizen 9.0
 *
 * @param[in] source The tizen core source handle
 * @param[in] callback The callback function to be dispatched events
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
static bool source_prepare_cb(tizen_core_source_h source, int *timeout, void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "prepare callback is invoked");
  return true;
}

static bool source_check_cb(tizen_core_source_h source, void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "check callback is invoked");
  return true;
}

static bool source_dispatch_cb(tizen_core_source_h source, void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "dispatch callback is invoked");
  return true;
}

static tizen_core_source_h create_source(void)
{
  tizen_core_source_h source = NULL;
  int ret;

  ret = tizen_core_source_create(&source);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create source");
    return NULL;
  }

  tizen_core_source_set_prepare_cb(source, source_prepare_cb, NULL);
  tizen_core_source_set_check_cb(source, source_check_cb, NULL);

  ret = tizen_core_source_set_dispatch_cb(source, source_dispatch_cb, NULL);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to set dispatch callback");
    tizen_core_source_destroy(source);
    return NULL;
  }

  return source;
}
 * @endcode
 *
 * @see tizen_core_source_create()
 * @see tizen_core_source_dispatch_cb()
 */
int tizen_core_source_set_dispatch_cb(
    tizen_core_source_h source, tizen_core_source_dispatch_cb callback,
    void *user_data);

/**
 * @brief Sets the finalize callback function to the tizen core source.
 * @details This function is not mandatory.
 *          If there is anything you need to do when the source is removed,
 *          use this function to set the callback function.
 * @since_tizen 9.0
 *
 * @param[in] source The tizen core source handle
 * @param[in] callback The callback function to finalize the source
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
static bool source_prepare_cb(tizen_core_source_h source, int *timeout, void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "prepare callback is invoked");
  return true;
}

static bool source_check_cb(tizen_core_source_h source, void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "check callback is invoked");
  return true;
}

static bool source_dispatch_cb(tizen_core_source_h source, void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "dispatch callback is invoked");
  return true;
}

static void source_finalize_cb(tizen_core_source_h source, void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "finalize callback is invoked");
}

static tizen_core_source_h create_source(void)
{
  tizen_core_source_h source = NULL;
  int ret;

  ret = tizen_core_source_create(&source);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create source");
    return NULL;
  }

  tizen_core_source_set_prepare_cb(source, source_prepare_cb, NULL);
  tizen_core_source_set_check_cb(source, source_check_cb, NULL);
  tizen_core_source_set_dispatch_cb(source, source_dispatch_cb, NULL);

  ret = tizen_core_source_set_finalize_cb(source, source_finalize_cb, NULL);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to set finalize callback");
    tizen_core_source_destroy(source);
    return NULL;
  }

  return source;
}
 * @endcode
 *
 * @see tizen_core_source_create()
 * @see tizen_core_source_finalize_cb()
 * @see tizen_core_remove_source()
 * @see tizen_core_source_destroy()
 */
int tizen_core_source_set_finalize_cb(
    tizen_core_source_h source, tizen_core_source_finalize_cb callback,
    void *user_data);

/**
 * @brief Sets the priority to the tizen core source.
 * @since_tizen 9.0
 *
 * @param[in] source The tizen core source handle
 * @param[in] priority The priority value
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
static bool idle_job_cb(void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "idler is invoked");
  return true;
}

static tizen_core_source_h create_idle_source(void)
{
  tizen_core_source_h source = NULL;
  tizen_core_h core = NULL;
  int ret;

  tizen_core_find("main", &core);
  ret = tizen_core_add_idle_job(core, idle_job_cb, NULL, &source);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to add idle job");
    return NULL;
  }

  ret = tizen_core_source_set_priority(source, TIZEN_CORE_PRIORITY_HIGH);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to set priority");
    tizen_core_source_destroy(source);
    return NULL;
  }

  return source;
}
 * @endcode
 *
 * @see tizen_core_priority_e
 */
int tizen_core_source_set_priority(tizen_core_source_h source,
                                   tizen_core_priority_e priority);

/**
 * @brief Creates a tizen core poll fd handle.
 * @since_tizen 9.0
 * @remarks The @a poll_fd should be released using tizen_core_poll_fd_destroy().
 *
 * @param[out] poll_fd The tizen core poll fd handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_poll_fd_h poll_fd = NULL;
  int ret;

  ret = tizen_core_poll_fd_create(&poll_fd);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create poll fd");
    return;
  }
}
 * @endcode
 *
 * @see tizen_core_poll_fd_destroy()
 */
int tizen_core_poll_fd_create(tizen_core_poll_fd_h *poll_fd);

/**
 * @brief Destroys the tizen core poll fd (file descriptor) handle.
 * @since_tizen 9.0
 * @remarks If the handle is already added to the source using the tizen_core_source_add_poll(),
 *          the handle will be removed from the source automatically after this function is called.
 *
 * @param[in] poll_fd The tizen core poll fd handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_poll_fd_h poll_fd = NULL;
  int ret;

  ret = tizen_core_poll_fd_create(&poll_fd);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create poll fd");
    return;
  }

  ret = tizen_core_poll_fd_destroy(poll_fd);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to destroy poll fd");
  }
}
 * @endcode
 *
 * @see tizen_core_poll_fd_create()
 */
int tizen_core_poll_fd_destroy(tizen_core_poll_fd_h poll_fd);

/**
 * @brief Sets the file descriptor to the poll fd handle.
 * @since_tizen 9.0
 *
 * @param[in] poll_fd The tizen core poll fd handle
 * @param[in] fd The file descriptor
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_poll_fd_h poll_fd = NULL;
  int ret;

  ret = tizen_core_poll_fd_create(&poll_fd);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create poll fd");
    return;
  }

  ret = tizen_core_poll_fd_set_fd(poll_fd, 0);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to set fd to poll fd");
    tizen_core_poll_fd_destroy(poll_fd);
    return;
  }
}
 * @endcode
 *
 * @see tizen_core_poll_fd_get_fd()
 */
int tizen_core_poll_fd_set_fd(tizen_core_poll_fd_h poll_fd, int fd);

/**
 * @brief Gets the file descriptor from the poll fd handle.
 * @since_tizen 9.0
 *
 * @param[in] poll_fd The tizen core poll fd handle
 * @param[out] fd The file descriptor
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_poll_fd_h poll_fd = NULL;
  int fd = -1;
  int ret;

  ret = tizen_core_poll_fd_create(&poll_fd);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create poll fd");
    return;
  }

  tizen_core_poll_fd_set_fd(poll_fd, 0);

  ret = tizen_core_poll_fd_get_fd(poll_fd, &fd);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to get fd from poll fd");
    tizen_core_poll_fd_destroy(poll_fd);
    return;
  }
}
 * @endcode
 *
 * @see tizen_core_poll_fd_set_fd()
 */
int tizen_core_poll_fd_get_fd(tizen_core_poll_fd_h poll_fd, int *fd);

/**
 * @brief Sets the requested events to the poll fd handle.
 * @since_tizen 9.0
 *
 * @param[in] poll_fd The tizen core poll fd handle
 * @param[in] events The requested events
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_poll_fd_h poll_fd = NULL;
  int ret;

  ret = tizen_core_poll_fd_create(&poll_fd);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create poll fd");
    return;
  }

  tizen_core_poll_fd_set_fd(poll_fd, 0);

  ret = tizen_core_poll_fd_set_events(poll_fd,
      TIZEN_CORE_POLL_EVENT_IN | TIZEN_CORE_POLL_EVENT_OUT);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to set events");
    tizen_core_poll_fd_destroy(poll_fd);
    return;
  }
}
 * @endcode
 *
 * @see tizen_core_poll_fd_get_events()
 * @see tizen_core_poll_event_e
 */
int tizen_core_poll_fd_set_events(tizen_core_poll_fd_h poll_fd,
                                  uint16_t events);

/**
 * @brief Gets the requested events from the poll fd handle.
 * @since_tizen 9.0
 *
 * @param[in] poll_fd The tizen core poll fd handle
 * @param[out] events The requested events
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_poll_fd_h poll_fd = NULL;
  uint16_t events;
  int ret;

  ret = tizen_core_poll_fd_create(&poll_fd);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create poll fd");
    return;
  }

  tizen_core_poll_fd_set_fd(poll_fd, 0);
  tizen_core_poll_fd_set_events(poll_fd,
      TIZEN_CORE_POLL_EVENT_IN | TIZEN_CORE_POLL_EVENT_HUP);

  ret = tizen_core_poll_fd_get_events(poll_fd, &events);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to get events from poll fd");
    tizen_core_poll_fd_destroy(poll_fd);
    return;
  }
}
 * @endcode
 *
 * @see tizen_core_poll_fd_set_events()
 * @see tizen_core_poll_event_e
 */
int tizen_core_poll_fd_get_events(tizen_core_poll_fd_h poll_fd,
                                  uint16_t *events);

/**
 * @brief Sets the returned events to the poll fd handle.
 * @since_tizen 9.0
 *
 * @param[in] poll_fd The tizen core poll fd handle
 * @param[in] returned_events The returned events
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_poll_fd_h poll_fd = NULL;
  int ret;

  ret = tizen_core_poll_fd_create(&poll_fd);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create poll fd");
    return;
  }

  ret = tizen_core_poll_fd_set_returned_events(poll_fd,
      TIZEN_CORE_POLL_EVENT_IN | TIZEN_CORE_POLL_EVENT_OUT);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to set returned events");
    tizen_core_poll_fd_destroy(poll_fd);
    return;
  }
}
 * @endcode
 *
 * @see tizen_core_poll_fd_get_events()
 * @see tizen_core_poll_event_e
 */
int tizen_core_poll_fd_set_returned_events(tizen_core_poll_fd_h poll_fd,
                                           uint16_t returned_events);

/**
 * @brief Gets the returned events from the poll fd handle.
 * @since_tizen 9.0
 *
 * @param[in] poll_fd The tizen core poll fd handle
 * @param[out] returned_events The returned events
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
static bool source_prepare_cb(tizen_core_source_h source, int *timeout, void *user_data)
{
  *timeout = -1;
  return true;
}

static bool source_check_cb(tizen_core_source_h source, void *user_data)
{
  tizen_core_poll_fd_h poll_fd = (tizen_core_poll_fd_h)user_data;
  uint16_t returned_events = 0;

  tizen_core_poll_fd_get_returned_events(poll_fd, &returned_events);
  if (returned_events & TIZEN_CORE_POLL_EVENT_IN)
    return true;

  return false;
}

static bool source_dispatch_cb(tizen_core_source_h source, void *user_data)
{
  tizen_core_poll_fd_h poll_fd = (tizen_core_poll_fd_h)user_data;
  ssize_t bytes;
  int ret = 0;
  int fd;

  tizen_core_poll_fd_get_fd(poll_fd, &fd);
  bytes = read(fd, (void *)&ret, sizeof(ret));
  if (bytes < 0) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to read data. errno=%d", errno);
  } else if (bytes == 0) {
    dlog_print(DLOG_ERROR, LOG_TAG, "socket was closed. fd=%d", fd);
    tizen_core_source_destroy(source);
  } else {
    dlog_print(DLOG_INFO, LOG_TAG, "result=%d", ret);
  }

  return true;
}

static void source_finalize_cb(tizen_core_source_h source, void *user_data)
{
  tizen_core_poll_fd_h poll_fd = (tizen_core_poll_fd_h)user_data;

  tizen_core_poll_fd_destroy(poll_fd);
}

static tizen_core_source_h create_io_source(int fd, uint16_t events)
{
  tizen_core_source_h source = NULL;
  tizen_core_poll_fd_h poll_fd = NULL;
  tizen_core_h core = NULL;

  tizen_core_poll_fd_create(&poll_fd);
  tizen_core_poll_fd_set_fd(poll_fd, fd);
  tizen_core_poll_fd_set_events(poll_fd, events);

  tizen_core_source_create(&source);
  tizen_core_source_set_prepare_cb(source, source_prepare_cb, poll_fd);
  tizen_core_source_set_check_cb(source, source_check_cb, poll_fd);
  tizen_core_source_set_dispatch_cb(source, source_dispatch_cb, poll_fd);
  tizen_core_source_set_finalize_cb(source, source_finalize_cb, poll_fd);

  tizen_core_find("main", &core);
  tizen_core_add_source(core, source);

  return source;
}
 * @endcode
 *
 * @see tizen_core_poll_fd_set_events()
 * @see tizen_core_poll_event_e
 */
int tizen_core_poll_fd_get_returned_events(tizen_core_poll_fd_h poll_fd,
                                           uint16_t *returned_events);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_BASE_TIZEN_CORE_H__ */
